package R1322IAeCSR

import chisel3._
import chisel3.util._

// 访存单元: 地址对齐、写数据移位、写掩码、读数据符号/零扩展。
class LSU extends Module {
  val io = IO(new Bundle {
    val exu_to_lsu = Flipped(Decoupled(new EXU_to_LSU_Lane_Message))

    // SimpleBus 数据访存接口 (写通道 + 读返回)。
    // 注: 本核流水化, load 的读地址由 EXU 在 E 级直接发往 dmem, 这里的 lsu_addr 是
    //     store 在 M 级发往 dmem 的写地址; lsu_rdata 是读回的数据。
    val lsu_addr  = Output(UInt(32.W))
    val lsu_wdata = Output(UInt(32.W))
    val lsu_wmask = Output(UInt(4.W))
    val lsu_wen   = Output(Bool())
    val lsu_rdata = Input(UInt(32.W))

    val lsu_to_wbu = Decoupled(new LSU_to_WBU_Lane_Message)

    val ebreak_out = Output(Bool())

    val debug = new Bundle {
      val is_load            = Output(Bool())
      val is_store           = Output(Bool())
      val addr               = Output(UInt(32.W))
      val read_origin        = Output(UInt(32.W))
      val final_wb_data      = Output(UInt(32.W))
      val store_mask         = Output(UInt(4.W))
      val store_data_shifted = Output(UInt(32.W))
    }
  })

  io.exu_to_lsu.ready := true.B
  io.lsu_to_wbu.valid := io.exu_to_lsu.valid

  val op          = io.exu_to_lsu.bits.op
  val is_load     = op.isLoad
  val is_store    = op.isStore
  val byte_offset = io.exu_to_lsu.bits.addr(1, 0)

  // ---------------- 写通路 ----------------
  io.lsu_wen  := is_store
  io.lsu_addr := io.exu_to_lsu.bits.addr & ~3.U(32.W)

  val wdata = io.exu_to_lsu.bits.store_data
  val shifted_wdata = Mux(op.is_sb || op.is_sh, wdata << (byte_offset << 3), wdata)
  io.lsu_wdata := shifted_wdata

  io.lsu_wmask := MuxCase(0.U(4.W), Seq(
    op.is_sw -> "b1111".U(4.W),
    op.is_sh -> Mux(byte_offset === 0.U, "b0011".U(4.W),
              Mux(byte_offset === 2.U, "b1100".U(4.W), 0.U(4.W))),
    op.is_sb -> (1.U(4.W) << byte_offset)
  ))

  // ---------------- 读通路 ----------------
  val aligned = io.lsu_rdata
  val byte0 = aligned(7, 0)
  val byte1 = aligned(15, 8)
  val byte2 = aligned(23, 16)
  val byte3 = aligned(31, 24)
  val half0 = Cat(byte1, byte0)
  val half1 = Cat(byte3, byte2)

  val byte_sel = Mux(byte_offset(1),
                     Mux(byte_offset(0), byte3, byte2),
                     Mux(byte_offset(0), byte1, byte0))
  val half_sel = Mux(byte_offset(1), half1, half0)

  val rdata = MuxCase(0.U(32.W), Seq(
    op.is_lw   -> aligned,
    op.is_lh   -> Cat(Fill(16, half_sel(15)), half_sel),
    op.is_lhu  -> Cat(0.U(16.W), half_sel),
    op.is_lb   -> Cat(Fill(24, byte_sel(7)), byte_sel),
    op.is_lbu  -> Cat(0.U(24.W), byte_sel)
  ))

  // load 从存储器取, 其余 (含 CSR 穿透) 直接用 EXU 传来的值
  val wb_data = Mux(is_load, rdata, io.exu_to_lsu.bits.wb_data)
  io.lsu_to_wbu.bits.rd          := io.exu_to_lsu.bits.rd
  io.lsu_to_wbu.bits.grf_wb_data := wb_data

  io.ebreak_out := op.is_ebreak

  // ---------------- 调试 ----------------
  io.debug.is_load            := is_load
  io.debug.is_store           := is_store
  io.debug.addr               := io.exu_to_lsu.bits.addr
  io.debug.read_origin        := aligned
  io.debug.final_wb_data      := wb_data
  io.debug.store_mask         := io.lsu_wmask
  io.debug.store_data_shifted := shifted_wdata
}

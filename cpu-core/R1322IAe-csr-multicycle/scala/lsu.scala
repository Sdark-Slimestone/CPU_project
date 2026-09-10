package R1322IAeCSR

import chisel3._
import chisel3.util._

// 访存单元，支持字节/半字/字读写
class LSU extends Module {
  val io = IO(new Bundle {
    /* 原端口保留
    val exu_to_lsu = new Bundle {
      val op = new Bundle {
        val is_lb     = Input(Bool())
        val is_lh     = Input(Bool())
        val is_lw     = Input(Bool())
        val is_lbu    = Input(Bool())
        val is_lhu    = Input(Bool())
        val is_sb     = Input(Bool())
        val is_sh     = Input(Bool())
        val is_sw     = Input(Bool())
        val is_ebreak = Input(Bool())
      }
      val paddr = new Bundle {
        val addr = Input(UInt(32.W))
      }
      val data = new Bundle {
        val store_data = Input(UInt(32.W))
      }
      val exu_through_lsu_to_wbu = new Bundle {
        val rd = Input(UInt(5.W))
        val grf_wb_data = Input(UInt(32.W))
      }
    }
    原端口保留结束 */

    //====================================新增: E/M 单lane总线端口====================================
    val exu_to_lsu = Flipped(Decoupled(new EXU_to_LSU_Lane_Message))
    //====================================新增: E/M 单lane总线端口====================================

    val lsu_to_dmem = new Bundle {
      val addr       = Output(UInt(32.W))
      val store_data = Output(UInt(32.W))
      val mask       = Output(UInt(4.W))
      val wen        = Output(Bool())
      val ren        = Output(Bool())
    }

    val dmem_to_lsu = new Bundle {
      val load_data = Input(UInt(32.W))
    }

    /* 原端口保留
    val lsu_to_wbu = new Bundle {
      val rd = Output(UInt(5.W))
      val grf_wb_data = Output(UInt(32.W))
    }
    原端口保留结束 */

    //====================================新增: M/W 单lane总线端口====================================
    val lsu_to_wbu = Decoupled(new LSU_to_WBU_Lane_Message)
    //====================================新增: M/W 单lane总线端口====================================

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

  //====================================新增: E/M 单lane ready + M/W 单lane valid====================================
  io.exu_to_lsu.ready := true.B
  /* 原 valid 保留
  io.lsu_to_wbu.valid := true.B
  原 valid 保留结束 */
  //====================================新增: M/W 单lane valid 随输入====================================
  io.lsu_to_wbu.valid := io.exu_to_lsu.valid
  //====================================新增: M/W 单lane valid 随输入====================================

  // 地址对齐和字节偏移
  val aligned_addr = io.exu_to_lsu.bits.addr & ~3.U(32.W)
  val byte_offset = io.exu_to_lsu.bits.addr(1, 0)

  val is_load = io.exu_to_lsu.bits.op.is_lb || io.exu_to_lsu.bits.op.is_lh || io.exu_to_lsu.bits.op.is_lw ||
                io.exu_to_lsu.bits.op.is_lbu || io.exu_to_lsu.bits.op.is_lhu
  val is_store = io.exu_to_lsu.bits.op.is_sb || io.exu_to_lsu.bits.op.is_sh || io.exu_to_lsu.bits.op.is_sw

  // 读写使能
  io.lsu_to_dmem.wen := is_store
  io.lsu_to_dmem.ren := is_load

  // 对齐后的地址输出
  io.lsu_to_dmem.addr := aligned_addr

  // 写数据移位（小端序）
  val wdata = io.exu_to_lsu.bits.store_data
  val shifted_wdata = MuxCase(wdata, Seq(
    (io.exu_to_lsu.bits.op.is_sb) -> (wdata << (byte_offset << 3)),
    (io.exu_to_lsu.bits.op.is_sh) -> (wdata << (byte_offset << 3))
  ))
  io.lsu_to_dmem.store_data := shifted_wdata

  // 写掩码生成
  io.lsu_to_dmem.mask := MuxCase(0.U(4.W), Seq(
    io.exu_to_lsu.bits.op.is_sw -> "b1111".U(4.W),
    io.exu_to_lsu.bits.op.is_sh -> Mux(byte_offset === 0.U, "b0011".U(4.W),
                             Mux(byte_offset === 2.U, "b1100".U(4.W), 0.U(4.W))),
    io.exu_to_lsu.bits.op.is_sb -> (1.U(4.W) << byte_offset)
  ))

  // 读数据通路
  val aligned = io.dmem_to_lsu.load_data
  val byte0 = aligned(7,0)
  val byte1 = aligned(15,8)
  val byte2 = aligned(23,16)
  val byte3 = aligned(31,24)
  val half0 = Cat(byte1, byte0)
  val half1 = Cat(byte3, byte2)

  val byte_sel = Mux(byte_offset(1),
    Mux(byte_offset(0), byte3, byte2),
    Mux(byte_offset(0), byte1, byte0)
  )
  val half_sel = Mux(byte_offset(1), half1, half0)

  val rdata = MuxCase(0.U(32.W), Seq(
    io.exu_to_lsu.bits.op.is_lw   -> aligned,
    io.exu_to_lsu.bits.op.is_lh   -> Cat(Fill(16, half_sel(15)), half_sel),
    io.exu_to_lsu.bits.op.is_lhu  -> Cat(0.U(16.W), half_sel),
    io.exu_to_lsu.bits.op.is_lb   -> Cat(Fill(24, byte_sel(7)), byte_sel),
    io.exu_to_lsu.bits.op.is_lbu  -> Cat(0.U(24.W), byte_sel)
  ))

  // 写回数据：load指令从存储器取，其他（含CSR穿透）直接用EXU传来的值
  val wb_data = Mux(is_load, rdata, io.exu_to_lsu.bits.wb_data)

  io.lsu_to_wbu.bits.rd := io.exu_to_lsu.bits.rd
  io.lsu_to_wbu.bits.grf_wb_data := wb_data

  // ebreak 透传
  io.ebreak_out := io.exu_to_lsu.bits.op.is_ebreak

  // debug
  io.debug.is_load            := is_load
  io.debug.is_store           := is_store
  io.debug.addr               := io.exu_to_lsu.bits.addr
  io.debug.read_origin        := aligned
  io.debug.final_wb_data      := wb_data
  io.debug.store_mask         := io.lsu_to_dmem.mask
  io.debug.store_data_shifted := shifted_wdata
}
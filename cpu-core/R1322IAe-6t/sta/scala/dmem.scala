package R1322IAeCSR

import chisel3._
import chisel3.util._

// 双端口同步 SRAM 宏黑盒 (2R1W, 时序/面积见 fakeram45_2r1w_256x32.lib)
class fakeram45_2r1w_256x32 extends BlackBox {
  val io = IO(new Bundle {
    val clk       = Input(Clock())
    val we_in     = Input(Bool())
    val ce_in     = Input(Bool())
    val waddr_in  = Input(UInt(8.W))
    val wd_in     = Input(UInt(32.W))
    val w_mask_in = Input(UInt(32.W))
    val raddr1    = Input(UInt(8.W))
    val rdata1    = Output(UInt(32.W))
    val raddr2    = Input(UInt(8.W))
    val rdata2    = Output(UInt(32.W))
  })
  override def desiredName = "fakeram45_2r1w_256x32"
}

// 数据存储器 DMEM (STA 版, 单个 2R1W 同步 SRAM 宏, 只做存储, 不做转发)。
// 读 (E 级, 来自 EXU) 与写 (M 级, 来自 LSU) 不同拍; 2 个读口 + 1 个写口天然支持双发射。
// store->load 转发由独立的 StoreBuffer 模块完成。
class dmem extends Module {
  val io = IO(new Bundle {
    val exu_to_dmem_1 = new Bundle {
      val addr = Input(UInt(32.W))
      val ren  = Input(Bool())
    }
    val exu_to_dmem_2 = new Bundle {
      val addr = Input(UInt(32.W))
      val ren  = Input(Bool())
    }

    val lsu_to_dmem_1 = new Bundle {
      val addr       = Input(UInt(32.W))
      val store_data = Input(UInt(32.W))
      val mask       = Input(UInt(4.W))
      val wen        = Input(Bool())
    }
    val lsu_to_dmem_2 = new Bundle {
      val addr       = Input(UInt(32.W))
      val store_data = Input(UInt(32.W))
      val mask       = Input(UInt(4.W))
      val wen        = Input(Bool())
    }

    // 原始读数据 (M 级), 交给 StoreBuffer 做转发合并
    val dmem_rdata_1 = Output(UInt(32.W))
    val dmem_rdata_2 = Output(UInt(32.W))

    val ebreak = Input(Bool())
  })

  def bitmask(m: UInt): UInt =
    Cat(Fill(8, m(3)), Fill(8, m(2)), Fill(8, m(1)), Fill(8, m(0)))

  val w_wen  = io.lsu_to_dmem_1.wen || io.lsu_to_dmem_2.wen
  val w_addr = Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.addr,       io.lsu_to_dmem_2.addr)
  val w_data = Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.store_data, io.lsu_to_dmem_2.store_data)
  val w_mask = Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.mask,       io.lsu_to_dmem_2.mask)

  val mem = Module(new fakeram45_2r1w_256x32)

  mem.io.clk       := clock
  mem.io.we_in     := w_wen
  mem.io.ce_in     := true.B
  mem.io.waddr_in  := w_addr(9, 2)
  mem.io.wd_in     := w_data
  mem.io.w_mask_in := bitmask(w_mask)
  mem.io.raddr1    := io.exu_to_dmem_1.addr(9, 2)
  mem.io.raddr2    := io.exu_to_dmem_2.addr(9, 2)

  io.dmem_rdata_1 := mem.io.rdata1
  io.dmem_rdata_2 := mem.io.rdata2
}

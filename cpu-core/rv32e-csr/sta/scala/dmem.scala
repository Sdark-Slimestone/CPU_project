package rv32ecsr

import chisel3._
import chisel3.util._

// 双端口同步 SRAM 宏黑盒 (2R1W, 见 fakeram45_2r1w_256x32.lib)
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

// 数据存储器 (STA SRAM 版): fakeram45 2R1W 同步 SRAM 宏, 接口与原 DPIMemory 一致。
class dmem extends Module {
  val io = IO(new Bundle {
    val io_clk = Input(Clock())
    val wen    = Input(Bool())
    val waddr  = Input(UInt(32.W))
    val wdata  = Input(UInt(32.W))
    val wmask  = Input(UInt(4.W))
    val ren    = Input(Bool())
    val raddr  = Input(UInt(32.W))
    val rdata  = Output(UInt(32.W))
    val ebreak = Input(Bool())
  })

  def bitmask(m: UInt): UInt =
    Cat(Fill(8, m(3)), Fill(8, m(2)), Fill(8, m(1)), Fill(8, m(0)))

  val mem = Module(new fakeram45_2r1w_256x32)
  mem.io.clk       := io.io_clk
  mem.io.we_in     := io.wen
  mem.io.ce_in     := true.B
  mem.io.waddr_in  := io.waddr(9, 2)
  mem.io.wd_in     := io.wdata
  mem.io.w_mask_in := bitmask(io.wmask)
  mem.io.raddr1    := io.raddr(9, 2)
  mem.io.raddr2    := 0.U
  io.rdata := mem.io.rdata1
}

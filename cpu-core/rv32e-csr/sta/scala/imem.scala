package rv32ecsr

import chisel3._
import chisel3.util._

// 单端口同步 SRAM 宏黑盒 (时序/面积见 pdk/nangate45/lib/fakeram45_256x34.lib)
class fakeram45_256x34 extends BlackBox {
  val io = IO(new Bundle {
    val clk       = Input(Clock())
    val addr_in   = Input(UInt(8.W))
    val wd_in     = Input(UInt(34.W))
    val w_mask_in = Input(UInt(34.W))
    val we_in     = Input(Bool())
    val ce_in     = Input(Bool())
    val rd_out    = Output(UInt(34.W))
  })
  override def desiredName = "fakeram45_256x34"
}

// 指令存储器 (STA SRAM 版): fakeram45 同步 SRAM 宏, 接口与原 DPIMemory 一致。
class imem extends Module {
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

  val mem = Module(new fakeram45_256x34)
  mem.io.clk       := io.io_clk
  mem.io.addr_in   := io.raddr(9, 2)
  mem.io.wd_in     := 0.U
  mem.io.w_mask_in := 0.U
  mem.io.we_in     := false.B
  mem.io.ce_in     := true.B
  io.rdata := mem.io.rd_out(31, 0)
}

package R1322IAeCSR

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

// 指令存储器 (STA SRAM 版): 双端口读, 2 个 fakeram45 同步 SRAM 宏 (每槽一份)。
class imem extends Module {
  val io = IO(new Bundle {
    val ifu_to_imem = new Bundle {
      val addr1 = Input(UInt(32.W))
      val addr2 = Input(UInt(32.W))
    }
    val imem_to_ifu = new Bundle {
      val inst1 = Output(UInt(32.W))
      val inst2 = Output(UInt(32.W))
    }
  })

  val mem1 = Module(new fakeram45_256x34)
  val mem2 = Module(new fakeram45_256x34)

  for ((m, a) <- Seq((mem1, io.ifu_to_imem.addr1), (mem2, io.ifu_to_imem.addr2))) {
    m.io.clk       := clock
    m.io.addr_in   := a(9, 2)
    m.io.wd_in     := 0.U
    m.io.w_mask_in := 0.U
    m.io.we_in     := false.B
    m.io.ce_in     := true.B
  }

  io.imem_to_ifu.inst1 := mem1.io.rd_out(31, 0)
  io.imem_to_ifu.inst2 := mem2.io.rd_out(31, 0)
}

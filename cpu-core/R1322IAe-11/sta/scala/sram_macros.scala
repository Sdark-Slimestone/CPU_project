package R1322IAeCSR

import chisel3._
import chisel3.util._

// SRAM 宏黑盒 (单端口, 同步读; 时序/面积见 pdk/nangate45/lib/fakeram45_*.lib)。
// 仅用于 make sta 的面积/时序评估。fakeram45_256x34 在 imem.scala 中声明。
class fakeram45_64x7 extends BlackBox {
  val io = IO(new Bundle {
    val clk       = Input(Clock())
    val addr_in   = Input(UInt(6.W))
    val wd_in     = Input(UInt(7.W))
    val w_mask_in = Input(UInt(7.W))
    val we_in     = Input(Bool())
    val ce_in     = Input(Bool())
    val rd_out    = Output(UInt(7.W))
  })
  override def desiredName = "fakeram45_64x7"
}

class fakeram45_64x96 extends BlackBox {
  val io = IO(new Bundle {
    val clk       = Input(Clock())
    val addr_in   = Input(UInt(6.W))
    val wd_in     = Input(UInt(96.W))
    val w_mask_in = Input(UInt(96.W))
    val we_in     = Input(Bool())
    val ce_in     = Input(Bool())
    val rd_out    = Output(UInt(96.W))
  })
  override def desiredName = "fakeram45_64x96"
}

// 用于 BTB/锦标赛预测器 (原声明在 STA 版 imem.scala; -11 起 imem/dmem 复用主版本,
// 故把这两个黑盒挪到这里)。
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

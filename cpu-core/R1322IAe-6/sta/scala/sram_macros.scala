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

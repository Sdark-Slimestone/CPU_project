package R322SAe

import chisel3._
import chisel3.util._

class DPIMemory extends BlackBox with HasBlackBoxResource {
  val io = IO(new Bundle {
    val io_clk = Input(Clock())
    val wen    = Input(Bool())
    val waddr  = Input(UInt(32.W))
    val wdata  = Input(UInt(32.W))
    val wmask  = Input(UInt(4.W))
    val ren1   = Input(Bool())
    val raddr1 = Input(UInt(32.W))
    val rdata1 = Output(UInt(32.W))
    val ren2   = Input(Bool())
    val raddr2 = Input(UInt(32.W))
    val rdata2 = Output(UInt(32.W))
    val ebreak = Input(Bool())
  })
  addResource("/DPI_Memory.v")
  override def desiredName = "DPI_Memory"
}
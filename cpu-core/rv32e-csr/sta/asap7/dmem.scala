package rv32ecsr

import chisel3._
import chisel3.util._

// 数据存储器 (ASAP7 版): RegisterFile, 接口与原 DPIMemory 一致。
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

  val register_file = Module(new RegisterFile)
  register_file.io.io_clk := io.io_clk
  register_file.io.wen    := io.wen
  register_file.io.waddr  := io.waddr(9, 2)
  register_file.io.wdata  := io.wdata
  register_file.io.wmask  := io.wmask
  register_file.io.raddr1 := io.raddr(9, 2)
  register_file.io.raddr2 := 0.U
  io.rdata := register_file.io.rdata1
}

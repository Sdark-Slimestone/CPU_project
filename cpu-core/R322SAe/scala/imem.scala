package R322SAe

import chisel3._
import chisel3.util._

// IMEM 顶层：对外使用 ifu_to_imem / imem_to_ifu 接口，内部固定读使能，写端口为0
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

  val memory = Module(new DPIMemory)

  memory.io.io_clk := clock

  memory.io.ren1   := true.B
  memory.io.raddr1 := io.ifu_to_imem.addr1
  io.imem_to_ifu.inst1 := memory.io.rdata1

  memory.io.ren2   := true.B
  memory.io.raddr2 := io.ifu_to_imem.addr2
  io.imem_to_ifu.inst2 := memory.io.rdata2

  memory.io.wen   := false.B
  memory.io.waddr := 0.U
  memory.io.wdata := 0.U
  memory.io.wmask := 0.U

  memory.io.ebreak := false.B
}
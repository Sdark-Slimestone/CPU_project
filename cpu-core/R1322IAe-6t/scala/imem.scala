package R1322IAeCSR

import chisel3._
import chisel3.util._

// 指令存储器: 双端口组合读 (支持双发射取指)。
// 日常构建走 DPI-C 软件 RAM (DPIMemory 黑盒); make sta 时被 sta/scala/imem.scala 覆盖。
class imem extends Module {
  val io = IO(new Bundle {
    // SimpleBus 取指读接口: 发 raddr+reqValid, 等 respValid 回 rdata (DPI 同步读)
    val ifu_raddr  = Input(UInt(32.W))
    val ifu_raddr2 = Input(UInt(32.W))
    val ifu_rdata  = Output(UInt(32.W))
    val ifu_rdata2 = Output(UInt(32.W))
    val ifu_reqValid  = Input(Bool())
    val ifu_respValid = Output(Bool())
    val ifu_respReady = Input(Bool())
    val ifu_flush     = Input(Bool())
  })

  val memory = Module(new DPIMemory(MemCfg.latency))

  memory.io.io_clk := clock
  memory.io.reqValid1 := io.ifu_reqValid
  memory.io.raddr1 := io.ifu_raddr
  memory.io.reqValid2 := io.ifu_reqValid
  memory.io.raddr2 := io.ifu_raddr2
  memory.io.respReady1 := io.ifu_respReady
  memory.io.respReady2 := io.ifu_respReady
  memory.io.flush := io.ifu_flush
  io.ifu_rdata  := memory.io.rdata1
  io.ifu_rdata2 := memory.io.rdata2
  io.ifu_respValid := memory.io.respValid1

  memory.io.wen   := false.B
  memory.io.waddr := 0.U
  memory.io.wdata := 0.U
  memory.io.wmask := 0.U
  memory.io.ebreak := false.B
}

package R1322IAeCSR

import chisel3._
import chisel3.util._

// 指令存储器: 双读口, SimpleBus 握手 (reqValid/reqReady, respValid/respReady), 可变延迟。
// 日常构建走 DPI-C 软件 RAM (DPIMemory 黑盒); make sta 时被 sta/scala/imem.scala 覆盖。
// 两个读口成对使用 (raddr/raddr2), 共用 reqValid/respReady; 响应须两口都有效。
class imem extends Module {
  val io = IO(new Bundle {
    val ifu_raddr  = Input(UInt(32.W))
    val ifu_raddr2 = Input(UInt(32.W))
    val ifu_rdata  = Output(UInt(32.W))
    val ifu_rdata2 = Output(UInt(32.W))
    val ifu_reqValid  = Input(Bool())
    val ifu_reqReady  = Output(Bool())
    val ifu_respValid = Output(Bool())
    val ifu_respReady = Input(Bool())
    val ifu_flush     = Input(Bool())
    val dbg_a1   = Output(UInt(32.W))
    val dbg_hd1  = Output(Bool())
  })

  val memory = Module(new DPIMemory(MemCfg.latency, MemCfg.random, MemCfg.maxLat,
                                    MemCfg.depth, MemCfg.depth, if (MemCfg.randReq) 1 else 0))

  memory.io.io_clk := clock
  memory.io.reqValid1 := io.ifu_reqValid
  memory.io.raddr1 := io.ifu_raddr
  memory.io.reqTag1 := 0.U
  memory.io.reqValid2 := io.ifu_reqValid
  memory.io.raddr2 := io.ifu_raddr2
  memory.io.reqTag2 := 0.U
  memory.io.respReady1 := io.ifu_respReady
  memory.io.respReady2 := io.ifu_respReady
  memory.io.flush := io.ifu_flush
  io.ifu_rdata  := memory.io.rdata1
  io.ifu_rdata2 := memory.io.rdata2
  io.ifu_reqReady  := memory.io.reqReady1 && memory.io.reqReady2
  io.ifu_respValid := memory.io.respValid1 && memory.io.respValid2
  io.dbg_a1  := memory.io.dbg_a1
  io.dbg_hd1 := memory.io.dbg_hd1

  memory.io.wen   := false.B
  memory.io.waddr := 0.U
  memory.io.wdata := 0.U
  memory.io.wmask := 0.U
  memory.io.ebreak := false.B
}

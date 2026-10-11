package R1322IAeCSR

import chisel3._
import chisel3.util._

// 指令存储器: AXI4-Lite master (只读)。
//   - 两个 AXI4-Lite 读通道成对使用 (raddr/raddr2), 各发一笔读事务, 共用 arvalid/rready;
//     R 通道响应须两口都有效 (r_valid1 && r_valid2) 才算这一对取回。
//   - 指令口不写内存, 故 AW/W/B 恒为 0 (b_ready=1); slave 侧 assert 其一直为 0。
// 日常构建走 DPI-C 软件 RAM (DPIMemory 黑盒); make sta 时被 sta/scala/imem.scala 覆盖。
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

  val memory = Module(new DPIMemory(MemCfg.latency, MemCfg.depth, MemCfg.depth))

  memory.io.io_clk := clock

  // ---- 读通道 1: IFU 的 raddr ----
  memory.io.ar_valid1 := io.ifu_reqValid
  memory.io.ar_addr1  := io.ifu_raddr
  memory.io.ar_prot1  := 0.U(3.W)
  memory.io.r_ready1  := io.ifu_respReady
  io.ifu_rdata := memory.io.r_data1

  // ---- 读通道 2: IFU 的 raddr2 ----
  memory.io.ar_valid2 := io.ifu_reqValid
  memory.io.ar_addr2  := io.ifu_raddr2
  memory.io.ar_prot2  := 0.U(3.W)
  memory.io.r_ready2  := io.ifu_respReady
  io.ifu_rdata2 := memory.io.r_data2

  // 请求被接收: 两个读通道的地址都被 slave 接受 (AXI 允许两口在不同拍接受)。
  io.ifu_reqReady  := memory.io.ar_ready1 && memory.io.ar_ready2
  // 响应有效: 两个读通道的 R 都有效, 这一对取指才成对消费。
  io.ifu_respValid := memory.io.r_valid1 && memory.io.r_valid2

  // ---- 写通道: 指令口只读, 恒置 0 ----
  memory.io.aw_valid := false.B
  memory.io.aw_addr  := 0.U
  memory.io.aw_prot  := 0.U
  memory.io.w_valid  := false.B
  memory.io.w_data   := 0.U
  memory.io.w_strb   := 0.U
  memory.io.b_ready  := true.B
  // 只读断言: 存储器不会返回 B (无写事务)。
  assert(!memory.io.b_valid, "IFU/IMEM must not issue write transactions")

  memory.io.flush  := io.ifu_flush
  memory.io.ebreak := false.B
  io.dbg_a1  := memory.io.dbg_a1
  io.dbg_hd1 := memory.io.dbg_hd1
}

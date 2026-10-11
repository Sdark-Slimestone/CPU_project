package R1322IAeCSR

import chisel3._
import chisel3.util._

// 数据存储器 DMEM (AXI4-Lite master, 只做存储, 不做转发)。
//   - 两个 AXI4-Lite 读通道: lane1/lane2 各一笔 load 读事务 (ar_valid=ren, r_ready=consume)。
//   - 一个 AXI4-Lite 写通道: lane1 优先; AW 与 W 同拍发出 (store 在 M 级只有效一拍)。
// 非阻塞 load: 读请求在 E 级发出 (ren=issue), 响应到达时由 top 经独立写口回写 GRF。
class dmem extends Module {
  val io = IO(new Bundle {
    // 读请求 (M 级)
    val exu_to_dmem_1 = new Bundle { val addr = Input(UInt(32.W)); val ren = Input(Bool()); val tag = Input(UInt(16.W)) }
    val exu_to_dmem_2 = new Bundle { val addr = Input(UInt(32.W)); val ren = Input(Bool()); val tag = Input(UInt(16.W)) }
    // 请求接收握手 (非阻塞 load: M 级等 reqReady 背压)
    val reqReady_1 = Output(Bool())
    val reqReady_2 = Output(Bool())

    // 写请求 (LSU, M 级)
    val lsu_to_dmem_1 = new Bundle {
      val addr = Input(UInt(32.W)); val store_data = Input(UInt(32.W))
      val mask = Input(UInt(4.W)); val wen = Input(Bool())
    }
    val lsu_to_dmem_2 = new Bundle {
      val addr = Input(UInt(32.W)); val store_data = Input(UInt(32.W))
      val mask = Input(UInt(4.W)); val wen = Input(Bool())
    }

    // 原始读数据 (M 级), 交给 StoreBuffer 做转发合并
    val dmem_rdata_1 = Output(UInt(32.W))
    val dmem_rdata_2 = Output(UInt(32.W))
    // load 响应有效与消费
    val load_respValid_1 = Output(Bool())
    val load_respValid_2 = Output(Bool())
    val load_respTag_1   = Output(UInt(16.W))
    val load_respTag_2   = Output(UInt(16.W))
    val load_respReady_1 = Input(Bool())
    val load_respReady_2 = Input(Bool())

    val ebreak = Input(Bool())
    val flush  = Input(Bool())
    val dbg_a1 = Output(UInt(32.W))
    val dbg_hd1 = Output(Bool())
    val dbg_cnt1 = Output(UInt(9.W))
  })

  val memory = Module(new DPIMemory(MemCfg.latency, MemCfg.dmemDepth, MemCfg.dmemDepth))
  memory.io.io_clk := clock

  // ---- 读通道 1 (lane1) ----
  memory.io.ar_valid1 := io.exu_to_dmem_1.ren
  memory.io.ar_addr1  := io.exu_to_dmem_1.addr
  memory.io.ar_prot1  := 0.U(3.W)
  memory.io.r_ready1  := io.load_respReady_1
  io.dmem_rdata_1     := memory.io.r_data1
  io.reqReady_1       := memory.io.ar_ready1
  io.load_respValid_1 := memory.io.r_valid1

  // ---- 读通道 2 (lane2) ----
  memory.io.ar_valid2 := io.exu_to_dmem_2.ren
  memory.io.ar_addr2  := io.exu_to_dmem_2.addr
  memory.io.ar_prot2  := 0.U(3.W)
  memory.io.r_ready2  := io.load_respReady_2
  io.dmem_rdata_2     := memory.io.r_data2
  io.reqReady_2       := memory.io.ar_ready2
  io.load_respValid_2 := memory.io.r_valid2

  // AXI 无 tag; load 元数据在 top 的每 lane 队列里按序管理, tag 端口保留但恒 0。
  io.load_respTag_1 := 0.U
  io.load_respTag_2 := 0.U

  // ---- 写通道 (lane1 优先) ----
  val w_wen  = io.lsu_to_dmem_1.wen || io.lsu_to_dmem_2.wen
  val w_addr = Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.addr,       io.lsu_to_dmem_2.addr)
  val w_data = Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.store_data, io.lsu_to_dmem_2.store_data)
  val w_strb = Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.mask,       io.lsu_to_dmem_2.mask)

  memory.io.aw_valid := w_wen
  memory.io.aw_addr  := w_addr
  memory.io.aw_prot  := 0.U(3.W)
  memory.io.w_valid  := w_wen
  memory.io.w_data   := w_data
  memory.io.w_strb   := w_strb
  memory.io.b_ready  := true.B

  memory.io.flush  := io.flush
  memory.io.ebreak := io.ebreak

  io.dbg_a1 := memory.io.dbg_a1
  io.dbg_hd1 := memory.io.dbg_hd1
  io.dbg_cnt1 := memory.io.dbg_cnt1
}

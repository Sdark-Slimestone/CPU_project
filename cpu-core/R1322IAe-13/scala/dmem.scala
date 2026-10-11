package R1322IAeCSR

import chisel3._
import chisel3.util._

// 数据存储器 (AXI4-Lite master, **单读通道 + 单写通道**)。
//   - 读: lane1/lane2 的 load 请求经 LaneReqFifo 原子接收 (两 lane 同拍要么都收要么都不收);
//     逐条发 AR, 用 inflightQ 记录每条在途读的 lane, R 响应据此路由回 lane1/lane2。
//   - 写: lane1 优先, 单写通道直发 AW/W (slave 每拍可接收, 支持背靠背 store)。
// 只做存储, 不做转发; load 结果由 top 经独立写口回写 GRF。
class dmem extends Module {
  val io = IO(new Bundle {
    // 读请求 (E/M 级); size: 0=1B,1=2B,2=4B (lb/lh/lw)
    val exu_to_dmem_1 = new Bundle { val addr = Input(UInt(32.W)); val ren = Input(Bool()); val tag = Input(UInt(16.W)); val size = Input(UInt(2.W)) }
    val exu_to_dmem_2 = new Bundle { val addr = Input(UInt(32.W)); val ren = Input(Bool()); val tag = Input(UInt(16.W)); val size = Input(UInt(2.W)) }
    val reqReady_1 = Output(Bool())
    val reqReady_2 = Output(Bool())

    // 写请求 (LSU, M 级)
    val lsu_to_dmem_1 = new Bundle {
      val addr = Input(UInt(32.W)); val store_data = Input(UInt(32.W))
      val mask = Input(UInt(4.W)); val wen = Input(Bool()); val size = Input(UInt(2.W))
    }
    val lsu_to_dmem_2 = new Bundle {
      val addr = Input(UInt(32.W)); val store_data = Input(UInt(32.W))
      val mask = Input(UInt(4.W)); val wen = Input(Bool()); val size = Input(UInt(2.W))
    }

    val dmem_rdata_1 = Output(UInt(32.W))
    val dmem_rdata_2 = Output(UInt(32.W))
    val load_respValid_1 = Output(Bool())
    val load_respValid_2 = Output(Bool())
    val load_respTag_1   = Output(UInt(16.W))
    val load_respTag_2   = Output(UInt(16.W))
    val load_respReady_1 = Input(Bool())
    val load_respReady_2 = Input(Bool())

    val ebreak = Input(Bool())
    val flush  = Input(Bool())
    val stall  = Input(Bool())          // pipe_stall (M 是否前进)
    val wStall = Output(Bool())         // 写缓冲满 -> 请求冻结 M
    val dbg_a1 = Output(UInt(32.W))
    val dbg_hd1 = Output(Bool())
    val dbg_cnt1 = Output(UInt(9.W))

    val axi_rd = new AXI4LiteRd    // 单读通道 (-> 仲裁器)
    val axi_wr = new AXI4LiteWr    // 单写通道 (-> 仲裁器)
  })

  // ================= 读通道 =================
  val issueQ = Module(new LaneReqFifo(MemCfg.dmemDepth))
  val inflightQ = Module(new MetaFIFOBridge(Bool(), MemCfg.dmemDepth))

  issueQ.io.flush := io.flush
  issueQ.io.enq1_valid := io.exu_to_dmem_1.ren
  issueQ.io.enq1_addr  := io.exu_to_dmem_1.addr
  issueQ.io.enq1_size  := io.exu_to_dmem_1.size
  issueQ.io.enq2_valid := io.exu_to_dmem_2.ren
  issueQ.io.enq2_addr  := io.exu_to_dmem_2.addr
  issueQ.io.enq2_size  := io.exu_to_dmem_2.size
  // 两 lane 同拍发出时原子接收 (enq_ready 同时考虑 nReq), 故两口 ready 一致。
  io.reqReady_1 := issueQ.io.enq_ready
  io.reqReady_2 := issueQ.io.enq_ready

  io.axi_rd.arvalid := issueQ.io.deq_valid
  io.axi_rd.araddr  := issueQ.io.deq_addr                       // 字节地址 (窄传输按 size 对齐)
  io.axi_rd.arlen   := 0.U
  io.axi_rd.arsize  := issueQ.io.deq_size                       // 实际访问位宽
  io.axi_rd.arburst := 1.U                                      // INCR
  io.axi_rd.arid    := 0.U
  val arFire = io.axi_rd.arvalid && io.axi_rd.arready
  issueQ.io.deq_ready := arFire

  inflightQ.io.flush := io.flush
  inflightQ.io.enq.valid := arFire
  inflightQ.io.enq.bits  := issueQ.io.deq_lane

  val rLane     = inflightQ.io.deq.bits
  val laneReady = Mux(rLane, io.load_respReady_2, io.load_respReady_1)
  val respV     = io.axi_rd.rvalid && inflightQ.io.deq.valid
  io.load_respValid_1 := respV && !rLane
  io.load_respValid_2 := respV &&  rLane
  io.dmem_rdata_1 := io.axi_rd.rdata
  io.dmem_rdata_2 := io.axi_rd.rdata
  io.axi_rd.rready := inflightQ.io.deq.valid && laneReady
  inflightQ.io.deq.ready := respV && laneReady

  // AXI 无 tag (load 元数据在 top 的每 lane 队列里按序管理)。
  io.load_respTag_1 := 0.U
  io.load_respTag_2 := 0.U

  // ================= 写通道 (lane1 优先, 1 深度保持缓冲) =================
  // 单拍 store 若 slave 当拍不就绪 (如 APB 设备) 会丢; 故用保持缓冲, 直到握手成功。
  val w_wen  = io.lsu_to_dmem_1.wen || io.lsu_to_dmem_2.wen
  val w_addr = Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.addr,       io.lsu_to_dmem_2.addr)
  val w_data = Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.store_data, io.lsu_to_dmem_2.store_data)
  val w_strb = Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.mask,       io.lsu_to_dmem_2.mask)
  val w_size = Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.size,       io.lsu_to_dmem_2.size)

  val wValid = RegInit(false.B)
  val wAddr  = Reg(UInt(32.W))
  val wData  = Reg(UInt(32.W))
  val wStrb  = Reg(UInt(4.W))
  val wSize  = Reg(UInt(2.W))
  val wSeen  = RegInit(false.B)   // 当前 M 级 store 是否已入缓冲 (防 M 停顿期间重复入队 -> double write)

  io.axi_wr.awvalid := wValid
  io.axi_wr.awaddr  := wAddr
  io.axi_wr.awlen   := 0.U
  io.axi_wr.awsize  := wSize
  io.axi_wr.awburst := 1.U
  io.axi_wr.awid    := 0.U
  io.axi_wr.wvalid  := wValid
  io.axi_wr.wdata   := wData
  io.axi_wr.wstrb   := wStrb
  io.axi_wr.wlast   := true.B
  io.axi_wr.bready  := true.B

  val wFire = io.axi_wr.awvalid && io.axi_wr.awready && io.axi_wr.wvalid && io.axi_wr.wready
  val wFree = !wValid || wFire                       // 缓冲空 (或本拍正排空)
  val wLat  = w_wen && !wSeen && wFree               // 接收一笔新 store
  io.wStall := w_wen && !wSeen && !wFree             // 缓冲被占, 冻结 M 直到排空

  when (wLat) {
    wSeen := true.B
    wValid := true.B
    wAddr := w_addr; wData := w_data; wStrb := w_strb; wSize := w_size
  }
  // M 前进 (本拍未因任何原因停顿) -> 当前 store 已离开 M, 允许下一笔入队
  when (!io.stall) { wSeen := false.B }
  when (wFire && !wLat) { wValid := false.B }

  io.dbg_a1 := 0.U
  io.dbg_hd1 := false.B
  io.dbg_cnt1 := issueQ.io.count
}

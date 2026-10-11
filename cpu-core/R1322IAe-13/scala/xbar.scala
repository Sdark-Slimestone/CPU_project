package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== AXI4 多路开关 (Xbar) ===================================
// 1 个 master (读 m_rd + 写 m_wr) -> N 个 slave (s_rd/s_wr), 完整 AXI4 单拍事务。
//   - 依据请求地址 (araddr/awaddr) 做地址译码, 选中某个下游 slave;
//   - 读响应**保序**: 转发 AR 时把选中的 slave 编号压入 reqQ, R 只有在其 slave 成为队头时
//     才回送 master (其余 slave 的 rready 拉低 → 它们保持响应), 无额外缓冲即保证顺序
//     (这样下游 AXIArbiter 的 owner FIFO 才能正确按序回配 master);
//   - 写路径组合转发 (store 单拍 fire-and-forget, 下游均每拍可接收, 不丢 store),
//     用 writeQ 记录每笔写的归属, 保证 B 按序回送。
//   - 本核用法: slave0 = CLINT (内部), slave1 = SoC (外部, 全地址范围兜底)。
//
// ranges: 每个 slave 的 [base, base+size) 字节区间 (与 s_rd/s_wr 下标一一对应)。
class AXI4LiteXbar(val ranges: Seq[(BigInt, BigInt)]) extends Module {
  val N = ranges.length
  require(N >= 1 && N <= 15)
  val selW = log2Ceil(N)

  val io = IO(new Bundle {
    val m_rd = Flipped(new AXI4LiteRd)
    val m_wr = Flipped(new AXI4LiteWr)
    val s_rd = Vec(N, new AXI4LiteRd)
    val s_wr = Vec(N, new AXI4LiteWr)
  })

  // 地址译码: PriorityEncoder -> 命中下标 (低下标优先); 无命中返回 0。
  def decode(addr: UInt): UInt = {
    val hits = ranges.map { case (b, s) => addr >= b.U && addr < (b + s).U }
    PriorityEncoder(hits)
  }

  // ===================== 读通道 =====================
  val rSel = decode(io.m_rd.araddr)
  val reqQ = Module(new Queue(UInt(selW.W), MemCfg.depth))
  val rSlaveReady = VecInit(io.s_rd.map(_.arready))(rSel)
  val rGo = io.m_rd.arvalid && reqQ.io.enq.ready && rSlaveReady
  io.m_rd.arready := rGo
  for (i <- 0 until N) {
    io.s_rd(i).arvalid := io.m_rd.arvalid && (rSel === i.U)
    io.s_rd(i).araddr  := io.m_rd.araddr
    io.s_rd(i).arlen   := io.m_rd.arlen
    io.s_rd(i).arsize  := io.m_rd.arsize
    io.s_rd(i).arburst := io.m_rd.arburst
    io.s_rd(i).arid    := io.m_rd.arid
  }
  reqQ.io.enq.valid := rGo
  reqQ.io.enq.bits  := rSel

  val rHead = reqQ.io.deq.bits
  val rSlaveValid = VecInit((0 until N).map(i => (rHead === i.U) && io.s_rd(i).rvalid)).reduce(_ || _)
  val rValid = reqQ.io.deq.valid && rSlaveValid
  io.m_rd.rvalid := rValid
  io.m_rd.rdata  := MuxCase(0.U(32.W), (0 until N).map(i => (rHead === i.U) -> io.s_rd(i).rdata))
  io.m_rd.rresp  := MuxCase(AXIResp.OKAY, (0 until N).map(i => (rHead === i.U) -> io.s_rd(i).rresp))
  io.m_rd.rlast  := MuxCase(true.B, (0 until N).map(i => (rHead === i.U) -> io.s_rd(i).rlast))
  io.m_rd.rid    := MuxCase(0.U(4.W), (0 until N).map(i => (rHead === i.U) -> io.s_rd(i).rid))
  val rFire = rValid && io.m_rd.rready
  for (i <- 0 until N) {
    io.s_rd(i).rready := reqQ.io.deq.valid && (rHead === i.U) && rFire
  }
  reqQ.io.deq.ready := rFire

  // ===================== 写通道 =====================
  val wSel = decode(io.m_wr.awaddr)
  val wSlaveReady = VecInit(io.s_wr.map(s => s.awready && s.wready))(wSel)
  val wrQ = Module(new Queue(UInt(selW.W), 8))
  val wGo = io.m_wr.awvalid && io.m_wr.wvalid && wrQ.io.enq.ready && wSlaveReady
  io.m_wr.awready := wGo
  io.m_wr.wready  := wGo
  for (i <- 0 until N) {
    io.s_wr(i).awvalid := io.m_wr.awvalid && (wSel === i.U)
    io.s_wr(i).awaddr  := io.m_wr.awaddr
    io.s_wr(i).awlen   := io.m_wr.awlen
    io.s_wr(i).awsize  := io.m_wr.awsize
    io.s_wr(i).awburst := io.m_wr.awburst
    io.s_wr(i).awid    := io.m_wr.awid
    io.s_wr(i).wvalid  := io.m_wr.wvalid && (wSel === i.U)
    io.s_wr(i).wdata   := io.m_wr.wdata
    io.s_wr(i).wstrb   := io.m_wr.wstrb
    io.s_wr(i).wlast   := io.m_wr.wlast
  }
  wrQ.io.enq.valid := wGo
  wrQ.io.enq.bits  := wSel

  val wHead = wrQ.io.deq.bits
  val wSlaveValid = VecInit((0 until N).map(i => (wHead === i.U) && io.s_wr(i).bvalid)).reduce(_ || _)
  val bValid = wrQ.io.deq.valid && wSlaveValid
  io.m_wr.bvalid := bValid
  io.m_wr.bresp  := MuxCase(AXIResp.OKAY, (0 until N).map(i => (wHead === i.U) -> io.s_wr(i).bresp))
  io.m_wr.bid    := MuxCase(0.U(4.W), (0 until N).map(i => (wHead === i.U) -> io.s_wr(i).bid))
  val bFire = bValid && io.m_wr.bready
  for (i <- 0 until N) {
    io.s_wr(i).bready := wrQ.io.deq.valid && (wHead === i.U) && io.m_wr.bready
  }
  wrQ.io.deq.ready := bFire
}

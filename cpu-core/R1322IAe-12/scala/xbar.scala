package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== AXI4-Lite 多路开关 (Xbar) ==============================
// 1 个 master (读通道 m_rd + 写通道 m_wr) -> N 个 slave (s_rd/s_wr)。
//   - 依据请求地址 (araddr/awaddr) 做**地址译码**, 选中某个下游 slave;
//   - 地址不属于任何 slave -> 内部 default slave 返回 **DECERR** (地址译码错);
//   - 读响应**保序**: 转发 AR 时把选中的 slave 编号压入 reqQ, R 只有在其 slave 成为队头时
//     才回送给 master (其余 slave 的 rready 拉低 → 它们保持响应), 无额外缓冲即保证顺序
//     (这样下游 AXIArbiter 的 owner FIFO 才能正确按序回配 master);
//   - 写路径组合转发 (本核的写是"单拍 fire-and-forget", 下游均每拍可接收, 不丢 store),
//     用 writeQ 记录每笔写的归属, 保证 B 按序回送。
//
// ranges: 每个 slave 的 [base, base+size) 字节区间 (与 s_rd/s_wr 的下标一一对应)。
class AXI4LiteXbar(val ranges: Seq[(BigInt, BigInt)]) extends Module {
  val N = ranges.length
  require(N >= 1 && N <= 15)
  val SEL_ERR = N                       // 未命中 -> default slave
  val selW = log2Ceil(N + 1)

  val io = IO(new Bundle {
    val m_rd = Flipped(new AXI4LiteRd)
    val m_wr = Flipped(new AXI4LiteWr)
    val s_rd = Vec(N, new AXI4LiteRd)
    val s_wr = Vec(N, new AXI4LiteWr)
  })

  // 地址译码: 返回 (选中下标, 是否命中)。未命中时下标取 0 (仅用于安全索引)。
  def decode(addr: UInt): (UInt, Bool) = {
    val hits = ranges.map { case (b, s) => addr >= b.U && addr < (b + s).U }
    val hit  = hits.reduce(_ || _)
    (PriorityEncoder(hits), hit)
  }

  // ===================== 读通道 =====================
  val (rSel, rHit) = decode(io.m_rd.araddr)
  val reqQ = Module(new Queue(UInt(selW.W), MemCfg.depth))
  val rSlaveReady = VecInit(io.s_rd.map(_.arready))(rSel)
  val rGo = io.m_rd.arvalid && reqQ.io.enq.ready && Mux(rHit, rSlaveReady, true.B)
  io.m_rd.arready := rGo
  for (i <- 0 until N) {
    io.s_rd(i).arvalid := io.m_rd.arvalid && rHit && (rSel === i.U)
    io.s_rd(i).araddr  := io.m_rd.araddr
    io.s_rd(i).arprot  := io.m_rd.arprot
  }
  reqQ.io.enq.valid := rGo
  reqQ.io.enq.bits  := Mux(rHit, rSel, SEL_ERR.U)

  val rHead    = reqQ.io.deq.bits
  val rHeadErr = rHead === SEL_ERR.U
  val rSlaveValid = VecInit((0 until N).map(i => (rHead === i.U) && io.s_rd(i).rvalid)).reduce(_ || _)
  val rValid = reqQ.io.deq.valid && (rHeadErr || rSlaveValid)
  io.m_rd.rvalid := rValid
  io.m_rd.rdata  := Mux(rHeadErr, 0.U,
                    MuxCase(0.U(32.W), (0 until N).map(i => (rHead === i.U) -> io.s_rd(i).rdata)))
  io.m_rd.rresp  := Mux(rHeadErr, AXIResp.DECERR,
                    MuxCase(AXIResp.OKAY, (0 until N).map(i => (rHead === i.U) -> io.s_rd(i).rresp)))
  val rFire = rValid && io.m_rd.rready
  for (i <- 0 until N) {
    io.s_rd(i).rready := reqQ.io.deq.valid && (rHead === i.U) && rFire
  }
  reqQ.io.deq.ready := rFire

  // ===================== 写通道 =====================
  val (wSel, wHit) = decode(io.m_wr.awaddr)
  val wSlaveReady = VecInit(io.s_wr.map(s => s.awready && s.wready))(wSel)
  val wrQ = Module(new Queue(UInt(selW.W), 8))
  val wGo = io.m_wr.awvalid && io.m_wr.wvalid && wrQ.io.enq.ready &&
            Mux(wHit, wSlaveReady, true.B)
  io.m_wr.awready := wGo
  io.m_wr.wready  := wGo
  for (i <- 0 until N) {
    io.s_wr(i).awvalid := io.m_wr.awvalid && wHit && (wSel === i.U)
    io.s_wr(i).awaddr  := io.m_wr.awaddr
    io.s_wr(i).awprot  := io.m_wr.awprot
    io.s_wr(i).wvalid  := io.m_wr.wvalid && wHit && (wSel === i.U)
    io.s_wr(i).wdata   := io.m_wr.wdata
    io.s_wr(i).wstrb   := io.m_wr.wstrb
  }
  wrQ.io.enq.valid := wGo
  wrQ.io.enq.bits  := Mux(wHit, wSel, SEL_ERR.U)

  val wHead    = wrQ.io.deq.bits
  val wHeadErr = wHead === SEL_ERR.U
  val wSlaveValid = VecInit((0 until N).map(i => (wHead === i.U) && io.s_wr(i).bvalid)).reduce(_ || _)
  val bValid = wrQ.io.deq.valid && (wHeadErr || wSlaveValid)
  io.m_wr.bvalid := bValid
  io.m_wr.bresp  := Mux(wHeadErr, AXIResp.DECERR,
                    MuxCase(AXIResp.OKAY, (0 until N).map(i => (wHead === i.U) -> io.s_wr(i).bresp)))
  val bFire = bValid && io.m_wr.bready
  for (i <- 0 until N) {
    io.s_wr(i).bready := wrQ.io.deq.valid && (wHead === i.U) && io.m_wr.bready
  }
  wrQ.io.deq.ready := bFire
}

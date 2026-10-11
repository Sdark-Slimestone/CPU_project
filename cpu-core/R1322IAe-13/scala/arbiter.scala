package R1322IAeCSR

import chisel3._
import chisel3.util._

// AXI4-Lite 仲裁器: 两个 master 共享一个 slave 接口 (单口存储器)。
//   - m0 = IFU (只读), m1 = LSU (读写)。
//   - 读地址 (AR): 按 round-robin 授予; 每转发一笔读, 把所有者写入 owner FIFO;
//     R 响应按 owner FIFO 队头路由回对应 master, 保证 (同一 slave 内) 响应保序。
//   - 读响应 head-of-line: 若队头 master 未 ready, 其它响应等待 (AXI 保序要求)。
//   - 写通道: IFU 无写事务, 故写通道退化为 LSU -> slave 直连 (slave 每拍可接收,
//     支持背靠背 store)。m0 的 AW/W/B 恒 0。
// 说明: valid 均不依赖对方 ready (无死锁); 授予只在对方 valid 才撤销 (由 AXI 保证)。
class AXIArbiter(val rdDepth: Int = 16) extends Module {
  val io = IO(new Bundle {
    val m0_rd = Flipped(new AXI4LiteRd)   // IFU
    val m1_rd = Flipped(new AXI4LiteRd)   // LSU
    val s_rd  = new AXI4LiteRd            // -> 存储器

    val m0_wr = Flipped(new AXI4LiteWr)   // IFU (恒 0)
    val m1_wr = Flipped(new AXI4LiteWr)   // LSU
    val s_wr  = new AXI4LiteWr            // -> 存储器
  })

  // ================= 读地址仲裁 (round-robin) =================
  val lastRd = RegInit(false.B)               // 上次授予的是 m1?
  val ownerQ = Module(new Queue(Bool(), rdDepth))   // false = m0, true = m1

  val want0 = io.m0_rd.arvalid
  val want1 = io.m1_rd.arvalid
  val pick0 = want0 && (!want1 || lastRd)
  val pick1 = want1 && !pick0
  val canFwd = ownerQ.io.enq.ready
  val arValid = canFwd && (pick0 || pick1)

  io.s_rd.arvalid := arValid
  io.s_rd.araddr  := Mux(pick1, io.m1_rd.araddr, io.m0_rd.araddr)
  io.s_rd.arlen   := Mux(pick1, io.m1_rd.arlen,  io.m0_rd.arlen)
  io.s_rd.arsize  := Mux(pick1, io.m1_rd.arsize, io.m0_rd.arsize)
  io.s_rd.arburst := Mux(pick1, io.m1_rd.arburst,io.m0_rd.arburst)
  io.s_rd.arid    := Mux(pick1, io.m1_rd.arid,   io.m0_rd.arid)

  val arFire = arValid && io.s_rd.arready
  io.m0_rd.arready := arFire && pick0
  io.m1_rd.arready := arFire && pick1
  ownerQ.io.enq.valid := arFire
  ownerQ.io.enq.bits  := pick1
  when (arFire) { lastRd := pick1 }

  // ================= 读响应路由 =================
  val owner    = ownerQ.io.deq.bits          // false = m0
  val selReady = Mux(owner, io.m1_rd.rready, io.m0_rd.rready)
  io.s_rd.rready := selReady

  val rFire = io.s_rd.rvalid && io.s_rd.rready
  io.m0_rd.rvalid := io.s_rd.rvalid && !owner
  io.m1_rd.rvalid := io.s_rd.rvalid && owner
  io.m0_rd.rdata  := io.s_rd.rdata
  io.m1_rd.rdata  := io.s_rd.rdata
  io.m0_rd.rresp  := io.s_rd.rresp
  io.m1_rd.rresp  := io.s_rd.rresp
  io.m0_rd.rlast  := io.s_rd.rlast
  io.m1_rd.rlast  := io.s_rd.rlast
  io.m0_rd.rid    := io.s_rd.rid
  io.m1_rd.rid    := io.s_rd.rid
  ownerQ.io.deq.ready := rFire

  // ================= 写通道: m1 直连, m0 恒 0 =================
  io.s_wr.awvalid := io.m1_wr.awvalid
  io.s_wr.awaddr  := io.m1_wr.awaddr
  io.s_wr.awlen   := io.m1_wr.awlen
  io.s_wr.awsize  := io.m1_wr.awsize
  io.s_wr.awburst := io.m1_wr.awburst
  io.s_wr.awid    := io.m1_wr.awid
  io.m1_wr.awready := io.s_wr.awready

  io.s_wr.wvalid := io.m1_wr.wvalid
  io.s_wr.wdata  := io.m1_wr.wdata
  io.s_wr.wstrb  := io.m1_wr.wstrb
  io.s_wr.wlast  := io.m1_wr.wlast
  io.m1_wr.wready := io.s_wr.wready

  io.m1_wr.bvalid := io.s_wr.bvalid
  io.m1_wr.bresp  := io.s_wr.bresp
  io.m1_wr.bid    := io.s_wr.bid
  io.s_wr.bready  := io.m1_wr.bready

  // IFU 不写存储器: AW/W/B 恒 0 (B 无请求, bvalid 恒 0)。
  io.m0_wr.awready := false.B
  io.m0_wr.wready  := false.B
  io.m0_wr.bvalid  := false.B
  io.m0_wr.bresp   := 0.U(2.W)
  io.m0_wr.bid     := 0.U
}

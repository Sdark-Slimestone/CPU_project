package R1322IAeCSR

import chisel3._
import chisel3.util._

// 指令存储器 (AXI4 read master, 单通道, 一次一笔 AR)。
//  - `fetch_req`(Decoupled) 收下每个取指对 {addr, pc, num, pred};
//  - 逐地址发 AR (等上一笔 R 回来再发下一笔), 每两个 R 拼成一对;
//  - 拼好的一对把 **元数据 (pc/num/pred) 与两条指令** 一起压入响应 FIFO 返回给 IFU,
//    因此 pc/指令永远成对, 不会因两个 FIFO 失步而错位;
//  - flush (重定向): 清空两个 FIFO; 已转发未返回的读用 discard 计数丢弃, 保证后续配对对齐。
class imem extends Module {
  val io = IO(new Bundle {
    val fetch_req  = Flipped(Decoupled(new FetchReq))
    val fetch_resp = Decoupled(new FetchResp)
    val flush      = Input(Bool())

    val axi = new AXI4LiteRd

    val dbg_a1  = Output(UInt(32.W))
    val dbg_hd1 = Output(Bool())
  })

  // ---------------- 请求 FIFO (含元数据) ----------------
  val reqFifo = Module(new MetaFIFOBridge(new FetchReq, MemCfg.depth))
  reqFifo.io.flush := io.flush
  reqFifo.io.enq.valid := io.fetch_req.valid
  reqFifo.io.enq.bits  := io.fetch_req.bits
  io.fetch_req.ready   := reqFifo.io.enq.ready

  // ---------------- 响应 FIFO (元数据 + 两条指令) ----------------
  val respFifo = Module(new MetaFIFOBridge(new FetchResp, MemCfg.depth))
  respFifo.io.flush := io.flush
  io.fetch_resp.valid := respFifo.io.deq.valid
  io.fetch_resp.bits  := respFifo.io.deq.bits
  respFifo.io.deq.ready := io.fetch_resp.ready

  // ---------------- 接收: 每两个 R 组成一对 ----------------
  val rPhase = RegInit(false.B)
  val d1     = Reg(UInt(32.W))
  val outstanding = RegInit(0.U(16.W))
  val discard     = RegInit(0.U(16.W))
  val discarding  = discard =/= 0.U

  io.axi.rready := Mux(discarding, true.B, respFifo.io.enq.ready)
  val rFire = io.axi.rvalid && io.axi.rready

  // ---------------- 发射: 一次一笔 AR, 等 R 回来再发下一笔 ----------------
  val sVal   = RegInit(false.B)
  val sPhase = RegInit(false.B)     // false: 发 base; true: 发 base+4
  val sBase  = Reg(UInt(32.W))
  val rWait  = RegInit(false.B)
  val cur    = Reg(new FetchReq)    // 当前对的元数据

  val deqFire = reqFifo.io.deq.valid && !sVal && !io.flush && !discarding
  reqFifo.io.deq.ready := deqFire

  val arValid = sVal && !rWait && !io.flush && !discarding
  val arFire  = arValid && io.axi.arready
  io.axi.arvalid := arValid
  io.axi.araddr  := (Mux(sPhase, sBase + 4.U, sBase))(31, 2) ## 0.U(2.W)
  io.axi.arlen   := 0.U
  io.axi.arsize  := 2.U
  io.axi.arburst := 1.U
  io.axi.arid    := 0.U

  val outNext = outstanding + arFire.asUInt - rFire.asUInt

  when (io.flush) {
    sVal := false.B; sPhase := false.B; rWait := false.B
  } .elsewhen (deqFire) {
    sBase := reqFifo.io.deq.bits.addr
    cur   := reqFifo.io.deq.bits
    sVal := true.B; sPhase := false.B; rWait := false.B
  } .elsewhen (arFire) {
    rWait := true.B
  } .elsewhen (rFire) {
    rWait := false.B
    when (sPhase) { sVal := false.B; sPhase := false.B }
    .otherwise    { sPhase := true.B }
  }

  // 配对完成: 把元数据 + 两条指令一起入响应 FIFO
  val pairBits = Wire(new FetchResp)
  pairBits.pc    := cur.pc
  pairBits.num   := cur.num
  pairBits.pred  := cur.pred
  pairBits.inst0 := d1
  pairBits.inst1 := io.axi.rdata
  respFifo.io.enq.valid := false.B
  respFifo.io.enq.bits  := pairBits

  when (io.flush) {
    discard := outNext
    rPhase  := false.B
  } .elsewhen (rFire) {
    when (discarding) {
      discard := discard - 1.U
    } .elsewhen (rPhase) {
      respFifo.io.enq.valid := true.B
      rPhase := false.B
    } .otherwise {
      d1 := io.axi.rdata
      rPhase := true.B
    }
  }
  outstanding := outNext

  io.dbg_a1  := 0.U
  io.dbg_hd1 := false.B
}

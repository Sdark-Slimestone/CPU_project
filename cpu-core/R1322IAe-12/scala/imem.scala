package R1322IAeCSR

import chisel3._
import chisel3.util._

// 指令存储器 (AXI4-Lite master, **单个读通道**)。
// 对 IFU 仍呈现"一次要一对指令 (raddr/raddr2, 两口响应都到齐)"的接口, 但内部通过
// 一个 AR/R 通道分两拍取回: 每个接受的取指对只存基址 a1 (a2 = a1 + 4), 逐地址发 AR,
// 每两个 R 响应拼成一对送入 pairFifo。
//   - ifu_reqReady = baseFifo 有空位 (可接收新的一对);
//   - ifu_respValid = pairFifo 非空 (一对指令就绪), ifu_respReady 消费。
// 重定向 (ifu_flush) 时清空本地 FIFO; 已转发但未返回的读会照常返回 (共享存储器不能被
// 单独冲掉), 用 discard 计数把它们丢弃, 从而保证后续配对重新对齐。
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

    val axi = new AXI4LiteRd     // 单个 AXI4-Lite 读通道 (-> 仲裁器)

    val dbg_a1  = Output(UInt(32.W))
    val dbg_hd1 = Output(Bool())
  })

  val flush = io.ifu_flush

  // ---------------- 基址 FIFO (每接受一对压入 a1) ----------------
  val baseFifo = Module(new MetaFIFOBridge(UInt(32.W), MemCfg.depth))
  baseFifo.io.flush := flush
  baseFifo.io.enq.valid := io.ifu_reqValid && !flush
  baseFifo.io.enq.bits  := io.ifu_raddr
  io.ifu_reqReady := baseFifo.io.enq.ready

  // ---------------- 响应成对 FIFO ----------------
  // 深度 = IFU 元数据 FIFO 深度: 保证"已接受未消费"的对数不会超过它, 于是 imem 永远
  // 能接收并缓存响应 (rready 不长时间拉低)。否则共享存储器按序返回时, 一个取指响应
  // 卡在队头会挡住后面 LSU 的 load 响应, 造成死锁 (no-outstanding-ordering)。
  val pairFifo = Module(new MetaFIFOBridge(Vec(2, UInt(32.W)), MemCfg.depth))
  pairFifo.io.flush := flush
  pairFifo.io.deq.ready := io.ifu_respReady
  io.ifu_respValid := pairFifo.io.deq.valid
  io.ifu_rdata  := pairFifo.io.deq.bits(0)
  io.ifu_rdata2 := pairFifo.io.deq.bits(1)

  // ---------------- 发射: 逐地址发 AR (a1, 然后 a1+4) ----------------
  val sVal   = RegInit(false.B)
  val sPhase = RegInit(false.B)      // false: 发 base; true: 发 base+4
  val sBase  = Reg(UInt(32.W))

  val deqFire = baseFifo.io.deq.valid && !sVal && !flush
  baseFifo.io.deq.ready := deqFire

  val arValid = sVal && !flush
  val arFire  = arValid && io.axi.arready
  io.axi.arvalid := arValid
  io.axi.araddr  := (Mux(sPhase, sBase + 4.U, sBase))(31, 2) ## 0.U(2.W)   // 4字节对齐
  io.axi.arprot  := 0.U(3.W)

  when (flush) {
    sVal := false.B
    sPhase := false.B
  } .elsewhen (deqFire) {
    sBase := baseFifo.io.deq.bits
    sVal := true.B
    sPhase := false.B
  } .elsewhen (arFire) {
    when (sPhase) { sVal := false.B; sPhase := false.B }
    .otherwise    { sPhase := true.B }
  }

  // ---------------- 接收: 每两个响应组成一对 ----------------
  val rPhase = RegInit(false.B)      // false: 期待第一个字
  val d1     = Reg(UInt(32.W))
  val outstanding = RegInit(0.U(16.W))
  val discard     = RegInit(0.U(16.W))
  val discarding  = discard =/= 0.U

  pairFifo.io.enq.valid := false.B
  pairFifo.io.enq.bits  := VecInit(d1, io.axi.rdata)

  io.axi.rready := Mux(discarding, true.B, pairFifo.io.enq.ready)
  val rFire   = io.axi.rvalid && io.axi.rready
  val outNext = outstanding + arFire.asUInt - rFire.asUInt

  when (flush) {
    discard := outNext        // 丢弃所有在途响应 (本地 FIFO 已清)
    rPhase  := false.B
  } .elsewhen (rFire) {
    when (discarding) {
      discard := discard - 1.U
    } .elsewhen (rPhase) {
      pairFifo.io.enq.valid := true.B
      pairFifo.io.enq.bits  := VecInit(d1, io.axi.rdata)
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

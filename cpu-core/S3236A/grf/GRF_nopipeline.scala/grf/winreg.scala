package S3242A

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

class winregbody extends Module {
  val regCount = 5
  val readvec  = 1
  val writevec = 1
  val addrwidth = 3
  val datawidth = 256

  val io = IO(new Bundle {
    val readAddrs    = Input(Vec(readvec, UInt(addrwidth.W)))
    val readDatas    = Output(Vec(readvec, UInt(datawidth.W)))
    val writeAddrs   = Input(Vec(writevec, UInt(addrwidth.W)))
    val writeDatas   = Input(Vec(writevec, UInt(datawidth.W)))
    val writeEnables = Input(Vec(writevec, Bool()))
  })

  val regs = RegInit(VecInit(Seq.fill(regCount)(0.U(datawidth.W))))

  for (wp <- 0 until writevec) {
    regs(io.writeAddrs(wp)) := Mux(io.writeEnables(wp), io.writeDatas(wp), regs(io.writeAddrs(wp)))
  }
  for (rp <- 0 until readvec) {
    io.readDatas(rp) := regs(io.readAddrs(rp))
  }
}

class winregfile extends Module {
  val io = IO(new Bundle{
    val cachetofile = Input(UInt(768.W))
    val filetocache = Output(UInt(768.W))
    val filetocache_enable = Output(UInt(3.W))
    val iscall = Input(Bool())
    val isret = Input(Bool())
    val ig = Input(Bool())
    val winoverflow_stack_pushtrap = Output(Bool())
    val winoverflow_stack_poptrap = Output(Bool())
    val toomuch_fuctioncall = Output(Bool())
    val wincode_out = Output(UInt(3.W))
  })

  // 寄存器定义
  val wincode = RegInit(0.U(3.W))
  val overflowcounter = RegInit(0.U(16.W))

  // 预计算加减结果
  val wincode_plus1 = wincode + 1.U(3.W)
  val wincode_minus1 = wincode - 1.U(3.W)
  val overflow_plus1 = overflowcounter + 1.U(16.W)
  val overflow_minus1 = overflowcounter - 1.U(16.W)

  // 更新 wincode
  wincode := PriorityMux(Seq(
    io.ig -> wincode,
    (io.iscall && (wincode === 6.U)) -> 6.U,
    (io.iscall && (wincode =/= 6.U)) -> wincode_plus1,
    (io.isret && (wincode =/= 0.U) && (overflowcounter === 0.U)) -> wincode_minus1,
    (io.isret && (wincode === 0.U)) -> 0.U,
    true.B -> wincode
  ))

  // 更新 overflowcounter
  overflowcounter := PriorityMux(Seq(
    io.ig -> overflowcounter,
    (io.iscall && (wincode === 6.U)) -> overflow_plus1,
    (io.isret && (overflowcounter =/= 0.U)) -> overflow_minus1,
    true.B -> overflowcounter
  ))

  // 当前逻辑窗口深度（自动扩展为16位）
  val current_win_number = overflowcounter + wincode

  // 陷阱信号
  io.winoverflow_stack_pushtrap := io.iscall && (current_win_number >= 6.U(16.W)) && !io.ig
  io.winoverflow_stack_poptrap := io.isret && (current_win_number > 6.U(16.W)) && !io.ig
  io.toomuch_fuctioncall := io.iscall && (current_win_number === 65535.U(16.W)) && !io.ig

  // 数据拆分
  val cachetofileData = VecInit(
    io.cachetofile(255, 0),
    io.cachetofile(511, 256),
    io.cachetofile(767, 512)
  )
  val filetocacheData = Wire(Vec(3, UInt(256.W)))

  // 实例化三个体
  val body0 = Module(new winregbody)
  val body1 = Module(new winregbody)
  val body2 = Module(new winregbody)

  // 地址映射表
  val callwriteaddr0 = MuxLookup(wincode, 0.U)(Seq(0.U -> 0.U, 1.U -> 1.U, 2.U -> 2.U, 3.U -> 2.U, 4.U -> 3.U, 5.U -> 4.U))
  val callwriteaddr1 = MuxLookup(wincode, 0.U)(Seq(0.U -> 0.U, 1.U -> 1.U, 2.U -> 1.U, 3.U -> 2.U, 4.U -> 3.U, 5.U -> 3.U))
  val callwriteaddr2 = MuxLookup(wincode, 0.U)(Seq(0.U -> 0.U, 1.U -> 0.U, 2.U -> 1.U, 3.U -> 2.U, 4.U -> 2.U, 5.U -> 3.U))
  val callreadaddr0  = MuxLookup(wincode, 0.U)(Seq(0.U -> 1.U, 1.U -> 2.U, 2.U -> 2.U, 3.U -> 3.U, 4.U -> 4.U, 5.U -> 4.U))
  val callreadaddr1  = MuxLookup(wincode, 0.U)(Seq(0.U -> 1.U, 1.U -> 1.U, 2.U -> 2.U, 3.U -> 3.U, 4.U -> 3.U, 5.U -> 4.U))
  val callreadaddr2  = MuxLookup(wincode, 0.U)(Seq(0.U -> 0.U, 1.U -> 1.U, 2.U -> 2.U, 3.U -> 2.U, 4.U -> 3.U, 5.U -> 4.U))
  val doExchangewhencall = io.iscall && (wincode <= 5.U)

  val retwriteaddr0 = MuxLookup(wincode, 0.U)(Seq(1.U -> 1.U, 2.U -> 2.U, 3.U -> 2.U, 4.U -> 3.U, 5.U -> 4.U, 6.U -> 4.U))
  val retwriteaddr1 = MuxLookup(wincode, 0.U)(Seq(1.U -> 1.U, 2.U -> 1.U, 3.U -> 2.U, 4.U -> 3.U, 5.U -> 3.U, 6.U -> 4.U))
  val retwriteaddr2 = MuxLookup(wincode, 0.U)(Seq(1.U -> 0.U, 2.U -> 1.U, 3.U -> 2.U, 4.U -> 2.U, 5.U -> 3.U, 6.U -> 4.U))
  val retreadaddr0  = MuxLookup(wincode, 0.U)(Seq(1.U -> 0.U, 2.U -> 1.U, 3.U -> 2.U, 4.U -> 2.U, 5.U -> 3.U, 6.U -> 4.U))
  val retreadaddr1  = MuxLookup(wincode, 0.U)(Seq(1.U -> 0.U, 2.U -> 1.U, 3.U -> 1.U, 4.U -> 2.U, 5.U -> 3.U, 6.U -> 3.U))
  val retreadaddr2  = MuxLookup(wincode, 0.U)(Seq(1.U -> 0.U, 2.U -> 0.U, 3.U -> 1.U, 4.U -> 2.U, 5.U -> 2.U, 6.U -> 3.U))
  val doExchangewhenret = io.isret && (wincode >= 1.U) && (current_win_number <= 6.U)

  val callreadmask = MuxLookup(wincode, 0.U)(Seq(0.U -> 3.U(3.W), 1.U -> 5.U(3.W), 2.U -> 6.U(3.W), 3.U -> 3.U(3.W), 4.U -> 5.U(3.W), 5.U -> 6.U(3.W)))
  val retreadmask  = MuxLookup(wincode, 0.U)(Seq(1.U -> 3.U(3.W), 2.U -> 5.U(3.W), 3.U -> 6.U(3.W), 4.U -> 3.U(3.W), 5.U -> 5.U(3.W), 6.U -> 6.U(3.W)))

  // 读地址：ig 时固定读地址 0
  body0.io.readAddrs(0) := Mux(io.ig, 0.U, Mux(io.iscall, callreadaddr0, Mux(io.isret, retreadaddr0, 0.U)))
  body1.io.readAddrs(0) := Mux(io.ig, 0.U, Mux(io.iscall, callreadaddr1, Mux(io.isret, retreadaddr1, 0.U)))
  body2.io.readAddrs(0) := Mux(io.ig, 0.U, Mux(io.iscall, callreadaddr2, Mux(io.isret, retreadaddr2, 0.U)))

  // 读出数据
  filetocacheData(0) := body0.io.readDatas(0)
  filetocacheData(1) := body1.io.readDatas(0)
  filetocacheData(2) := body2.io.readDatas(0)

  // 写地址（ig 时不关心，因为写使能被禁止）
  body0.io.writeAddrs(0) := Mux(io.iscall, callwriteaddr0, Mux(io.isret, retwriteaddr0, 0.U))
  body1.io.writeAddrs(0) := Mux(io.iscall, callwriteaddr1, Mux(io.isret, retwriteaddr1, 0.U))
  body2.io.writeAddrs(0) := Mux(io.iscall, callwriteaddr2, Mux(io.isret, retwriteaddr2, 0.U))

  // 写数据
  body0.io.writeDatas(0) := cachetofileData(0)
  body1.io.writeDatas(0) := cachetofileData(1)
  body2.io.writeDatas(0) := cachetofileData(2)

  // 写使能：ig 时禁止写入
  body0.io.writeEnables(0) := !io.ig && (doExchangewhencall || doExchangewhenret)
  body1.io.writeEnables(0) := !io.ig && (doExchangewhencall || doExchangewhenret)
  body2.io.writeEnables(0) := !io.ig && (doExchangewhencall || doExchangewhenret)

  // 输出拼接
  io.filetocache := Cat(filetocacheData(2), filetocacheData(1), filetocacheData(0))
  io.filetocache_enable := Mux(io.ig, 7.U(3.W), Mux(io.iscall, callreadmask, Mux(io.isret, retreadmask, 0.U(3.W))))
  io.wincode_out := wincode
}


object winregfile extends App {
  ChiselStage.emitSystemVerilogFile(
    new winregfile,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}

//面积39825 功耗0.05727w


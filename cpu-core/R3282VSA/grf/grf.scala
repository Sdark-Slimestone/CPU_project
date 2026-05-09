package R3262VSA

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

class top extends Module {
  val io = IO(new Bundle {
    val iscall = Input(Bool())
    val isret  = Input(Bool())

    val readAddr   = Input(Vec(10, UInt(5.W)))
    val readData   = Output(Vec(10, UInt(32.W)))
    val writeAddr  = Input(Vec(4, UInt(5.W)))
    val writeData  = Input(Vec(4, UInt(32.W)))
    val writeEnable = Input(Vec(4, Bool()))

    val cur_wincode     = Output(UInt(3.W))
    val overflowcounter = Output(UInt(16.W))
    val prev_wincode    = Output(UInt(3.W))
    val pushtrap        = Output(Bool())
    val poptrap         = Output(Bool())
    val toomuch         = Output(Bool())
  })

  // 窗口控制器
  val fsm = Module(new win_controller_P)
  fsm.io.iscall := io.iscall
  fsm.io.isret  := io.isret

  // 后端正则体
  val wb = Module(new winreg_P)
  wb.io.iscall := io.iscall
  wb.io.isret  := io.isret
  wb.io.cur_wincode     := fsm.io.cur_wincode
  wb.io.overflowcounter := fsm.io.overflowcounter
  wb.io.prev_wincode    := fsm.io.prev_wincode

  // 前端缓存（已含写使能）
  val wc = Module(new winregcache)
  wc.io.iscall := io.iscall
  wc.io.isret  := io.isret
  wc.io.cur_wincode     := fsm.io.cur_wincode
  wc.io.overflowcounter := fsm.io.overflowcounter
  wc.io.prev_wincode    := fsm.io.prev_wincode
  wc.io.pushtrap := fsm.io.winoverflow_stack_pushtrap
  wc.io.poptrap  := fsm.io.winoverflow_stack_poptrap
  wc.io.readAddr := io.readAddr

  // 前后端数据连接
  wb.io.cachetofile := wc.io.cachetofile
  wc.io.filetocache := wb.io.filetocache
  wc.io.filetocache_enable := wb.io.filetocache_enable
  wb.io.writeback := wc.io.writeback
  wb.io.writeback_enable := wc.io.writeback_enable

  // 全局寄存器堆（8读4写）
  val grf = Module(new globalregfileup)

  // ---------- 读端口连接 ----------
  // grf 只有 8 个读端口，因此只使用前 8 个读地址连接 grf
  for (i <- 0 until 8) {
    grf.io.readAddrs(i) := io.readAddr(i)(2, 0)          // 取低3位
  }

  // 输出读数据：地址<8 且读端口索引<8 时从 grf 读，否则从 wc 读
  for (i <- 0 until 10) {
    val addr = io.readAddr(i)
    val fromWc = wc.io.readdatas(i)
    val fromGrf = if (i < 8) grf.io.readDatas(i) else 0.U(32.W)  // 端口不足时给0
    io.readData(i) := Mux(addr < 8.U && i.U < 8.U, fromGrf, fromWc)
  }

  // ---------- 写端口连接 ----------
  for (i <- 0 until 4) {
    val addr = io.writeAddr(i)
    val data = io.writeData(i)
    val en   = io.writeEnable(i)

    // 写全局寄存器堆
    grf.io.writeAddrs(i) := addr(2, 0)
    grf.io.writeDatas(i) := data
    grf.io.writeEnables(i) := en && (addr < 8.U)

    // 写窗口缓存
    wc.io.writeAddr(i) := addr
    wc.io.writeData(i) := data
    wc.io.writeEnable(i) := en && (addr >= 8.U)
  }

  // 输出调试信号
  io.cur_wincode     := fsm.io.cur_wincode
  io.overflowcounter := fsm.io.overflowcounter
  io.prev_wincode    := fsm.io.prev_wincode
  io.pushtrap        := fsm.io.winoverflow_stack_pushtrap
  io.poptrap         := fsm.io.winoverflow_stack_poptrap
  io.toomuch         := fsm.io.toomuch_fuctioncall
}

object top extends App {
  ChiselStage.emitSystemVerilogFile(
    new top,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}
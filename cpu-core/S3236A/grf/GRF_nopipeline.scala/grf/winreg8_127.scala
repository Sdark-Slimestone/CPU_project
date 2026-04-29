package S3242A

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

class winreg8_127 extends Module {
  val io = IO(new Bundle {
    // 来自 winregfile 的陷阱信号
    val winoverflow_stack_pushtrap = Output(Bool())
    val winoverflow_stack_poptrap  = Output(Bool())
    val toomuch_fuctioncall        = Output(Bool())

    // 来自 wincacheup 的 CPU 访问端口
    val readAddrs    = Input(Vec(12, UInt(5.W)))
    val readDatas    = Output(Vec(12, UInt(32.W)))
    val writeAddrs   = Input(Vec(5, UInt(5.W)))
    val writeDatas   = Input(Vec(5, UInt(32.W)))
    val writeEnables = Input(Vec(5, Bool()))

    // 控制信号（同时驱动 winregfile 和 wincacheup）
    val iscall = Input(Bool())
    val isret  = Input(Bool())
    val ig     = Input(Bool())
  })

  // 实例化子模块
  val regfile = Module(new winregfile)
  val cache   = Module(new wincacheup)

  // 连接 regfile 与 cache 之间的数据通路
  regfile.io.cachetofile := cache.io.cachetofile
  cache.io.filetocache   := regfile.io.filetocache
  cache.io.filetocache_enable := regfile.io.filetocache_enable
  cache.io.wincode  := regfile.io.wincode_out

  // 连接控制信号
  regfile.io.iscall := io.iscall
  regfile.io.isret  := io.isret
  regfile.io.ig     := io.ig
  cache.io.call     := io.iscall
  cache.io.ret      := io.isret
  cache.io.ig       := io.ig

  // 输出陷阱信号
  io.winoverflow_stack_pushtrap := regfile.io.winoverflow_stack_pushtrap
  io.winoverflow_stack_poptrap  := regfile.io.winoverflow_stack_poptrap
  io.toomuch_fuctioncall        := regfile.io.toomuch_fuctioncall

  // 连接 CPU 访问 cache 的端口
  cache.io.readAddrs    := io.readAddrs
  io.readDatas          := cache.io.readDatas
  cache.io.writeAddrs   := io.writeAddrs
  cache.io.writeDatas   := io.writeDatas
  cache.io.writeEnables := io.writeEnables
}

object winreg8_127 extends App {
  ChiselStage.emitSystemVerilogFile(
    new winreg8_127,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}

//面积75417 功耗0.2050w
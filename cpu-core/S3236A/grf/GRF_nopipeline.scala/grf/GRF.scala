package S3242A

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

class GRF extends Module {
  val io = IO(new Bundle {
    // CPU 读写端口（12读5写）
    val readAddrs    = Input(Vec(12, UInt(5.W)))
    val readDatas    = Output(Vec(12, UInt(32.W)))
    val writeAddrs   = Input(Vec(5, UInt(5.W)))
    val writeDatas   = Input(Vec(5, UInt(32.W)))
    val writeEnables = Input(Vec(5, Bool()))

    // 控制信号（传递给窗口系统）
    val iscall = Input(Bool())
    val isret  = Input(Bool())
    val ig     = Input(Bool())

    // 窗口系统的陷阱信号输出
    val winoverflow_stack_pushtrap = Output(Bool())
    val winoverflow_stack_poptrap  = Output(Bool())
    val toomuch_fuctioncall        = Output(Bool())
  })

  // 实例化全局寄存器文件
  val globalRegfile = Module(new globalregfileup)

  // 实例化窗口系统
  val winSystem = Module(new winreg8_127)   // 注意：winreg8_127 就是之前连接好的顶层模块

  // 地址映射：将 CPU 地址 8~31 转换为窗口系统的 0~23 
  def mapToWin(addr: UInt): UInt = addr - 8.U

  // 读端口：根据地址范围选择数据源 
  for (rp <- 0 until 12) {
    val addr = io.readAddrs(rp)
    val isGlobal = addr < 8.U
    // 全局寄存器的读地址
    globalRegfile.io.readAddrs(rp) := addr
    val globalData = globalRegfile.io.readDatas(rp)
    // 窗口系统的读地址（仅当地址 >=8 时有效，否则填0）
    winSystem.io.readAddrs(rp) := Mux(isGlobal, 0.U, mapToWin(addr))
    val winData = winSystem.io.readDatas(rp)
    io.readDatas(rp) := Mux(isGlobal, globalData, winData)
  }

  // 写端口：根据地址范围分配写使能和地址 
  for (wp <- 0 until 5) {
    val addr = io.writeAddrs(wp)
    val isGlobal = addr < 8.U
    // 全局寄存器写端口
    globalRegfile.io.writeAddrs(wp) := addr
    globalRegfile.io.writeDatas(wp) := io.writeDatas(wp)
    globalRegfile.io.writeEnables(wp) := io.writeEnables(wp) && isGlobal
    // 窗口系统写端口
    winSystem.io.writeAddrs(wp) := Mux(isGlobal, 0.U, mapToWin(addr))
    winSystem.io.writeDatas(wp) := io.writeDatas(wp)
    winSystem.io.writeEnables(wp) := io.writeEnables(wp) && !isGlobal
  }

  // 控制信号连接 
  winSystem.io.iscall := io.iscall
  winSystem.io.isret  := io.isret
  winSystem.io.ig     := io.ig

  // 输出陷阱信号
  io.winoverflow_stack_pushtrap := winSystem.io.winoverflow_stack_pushtrap
  io.winoverflow_stack_poptrap  := winSystem.io.winoverflow_stack_poptrap
  io.toomuch_fuctioncall        := winSystem.io.toomuch_fuctioncall
}

object GRF extends App {
  ChiselStage.emitSystemVerilogFile(
    new GRF,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}

//面积88535 功耗0.261w
//修复重叠区不一致问题
//面积89236 功耗0.2169w
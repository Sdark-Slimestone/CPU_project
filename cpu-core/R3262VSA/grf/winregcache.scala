package S3242A

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

class winregcache extends Module {
  val io = IO(new Bundle {
    // 控制输入（来自状态机或外部）
    val iscall = Input(Bool())
    val isret  = Input(Bool())
    val ig     = Input(Bool())
    val cur_wincode     = Input(UInt(3.W))
    val overflowcounter = Input(UInt(16.W))
    val prev_wincode    = Input(UInt(3.W))

    // 数据接口（和file交互）
    val cachetofile = Output(UInt(768.W))
    val filetocache = Input(UInt(768.W))
    val filetocache_enable = Input(UInt(24.W))
    val writeback = Output(UInt(768.W))
    val writeback_enable = Output(UInt(24.W))

    // 数据接口 （和cpu交互）
    val readAddr = Input(UInt(5.W))
    val readdatas = Output(Vec(10, UInt(32.W)))
    val writeaddr = Input(UInt(5.W))
    val writedatas = Input(Vec(4, UInt(32.W)))
  })

  // 实例化cache模块
  val cache = registers(24, 10, 4, 5, 32)

  // 输出，接cachetofile
  val cachetofile_stage0 = Wire(UInt(768.W)) 
  for(cachetofilewp <- 0 until 23){
    cachetofile_stage0(cachetofilewp * 32, ((cachetofilewp + 1) * 32) - 1) := regs(wp)
  }

  // 窗口逻辑
  def laddrtopaddr (io.readAddr): (UInt) = {

  }
}
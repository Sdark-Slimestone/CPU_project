package rv32ecsr

import chisel3._
import chisel3.util._

class IFU extends Module {
  val io = IO(new Bundle {
    val take_branch   = Input(Bool())
    val branch_target = Input(UInt(32.W))
    val take_trap     = Input(Bool())
    val trap_pc       = Input(UInt(32.W))
    val take_mret     = Input(Bool())
    val mret_pc       = Input(UInt(32.W))
    val is_ebreak     = Input(Bool())         // 暂停 PC 更新
    val imemAddr      = Output(UInt(32.W))
    val inst          = Input(UInt(32.W))
    val pctogrf       = Output(UInt(32.W))
    val debug_pc      = Output(UInt(32.W))
    val current_pc    = Output(UInt(32.W))
  })

  val pcReg   = RegInit("h80000000".U(32.W))
  val pcPlus4 = pcReg + 4.U

  // 更新 PC:
  // 优先级: ebreak 暂停 > mret 返回 > 异常跳转 > 分支跳转 > 顺序执行
  pcReg := Mux(io.is_ebreak, pcReg,
            Mux(io.take_mret, io.mret_pc,
            Mux(io.take_trap, io.trap_pc,
            Mux(io.take_branch, io.branch_target, pcPlus4))))

  io.imemAddr   := pcReg
  io.current_pc := pcReg
  io.pctogrf    := pcPlus4
  io.debug_pc   := pcReg
}
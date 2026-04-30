package rv32e

import chisel3._
import chisel3.util._

class IFU extends Module {
  val io = IO(new Bundle {
    val take_branch   = Input(Bool())
    val branch_target = Input(UInt(32.W))
    val is_ebreak     = Input(Bool())         // 新增
    val imemAddr      = Output(UInt(32.W))
    val inst          = Input(UInt(32.W))
    val pctogrf       = Output(UInt(32.W))
    val debug_pc      = Output(UInt(32.W))
    val current_pc    = Output(UInt(32.W))
  })

  val pcReg   = RegInit("h80000000".U(32.W))
  val pcPlus4 = pcReg + 4.U

  pcReg := Mux(io.is_ebreak, pcReg,
            Mux(io.take_branch, io.branch_target, pcPlus4))

  io.imemAddr   := pcReg
  io.current_pc := pcReg
  io.pctogrf    := pcPlus4
  io.debug_pc   := pcReg
}
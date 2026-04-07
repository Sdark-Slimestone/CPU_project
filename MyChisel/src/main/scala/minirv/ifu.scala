package minirv

import chisel3._
import chisel3.util._

class IFU extends Module {
  val io = IO(new Bundle {
    // 来自执行单元的跳转控制
    val jalr         = Input(Bool())           // jalr 指令有效
    val inputfromBRU = Input(UInt(32.W))       // 跳转目标地址（来自 BRU）

    // 与指令存储器接口
    val imemAddr = Output(UInt(32.W))
    val inst     = Input(UInt(32.W))

    // 提供给 GRF 的 PC+4（用于 jalr 写回）
    val pctogrf  = Output(UInt(32.W))
  })

  val pcReg = RegInit("h80000000".U(32.W))
  val currentPc = pcReg
  val currentPcPlus4 = currentPc + 4.U

  // 更新 PC：跳转时用 BRU 的目标地址，否则顺序执行 PC+4
  pcReg := Mux(io.jalr, io.inputfromBRU, currentPcPlus4)

  // 输出当前 PC 用于取指，以及当前 PC+4 用于写回
  io.imemAddr := currentPc
  io.pctogrf := currentPcPlus4
}
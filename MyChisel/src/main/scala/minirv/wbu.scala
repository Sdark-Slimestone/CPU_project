package minirv

import chisel3._
import chisel3.util._

class WBU extends Module {
  val io = IO(new Bundle {
    // 控制信号（来自 IDU）
    val add   = Input(Bool())
    val addi  = Input(Bool())
    val jalr  = Input(Bool())
    val lbu   = Input(Bool())
    val lw    = Input(Bool())
    val lui   = Input(Bool())
    // 注：sw, sb 不写寄存器，故不出现

    // 数据源
    val inputfromALU = Input(UInt(32.W))
    val inputfromPC = Input(UInt(32.W))   // 来自 IFU
    val inputfromRAM = Input(UInt(32.W))   // 来自 LSU
    val inputfromIDU = Input(UInt(32.W))   // 来自 IDU（lui 的立即数）

    // 输出
    val wbData = Output(UInt(32.W))
    val regWen = Output(Bool())
    val debug_regWen = Output(Bool())
    val debug_wbData = Output(UInt(32.W))
  })

  // 写回数据选择
  io.wbData := MuxCase(0.U(32.W), Seq(
    (io.add || io.addi) -> io.inputfromALU,
    io.jalr             -> io.inputfromPC,
    (io.lbu || io.lw)   -> io.inputfromRAM,
    io.lui              -> io.inputfromIDU
  ))

  // 写使能：需要写寄存器的指令集合
  io.regWen := io.add || io.addi || io.jalr || io.lbu || io.lw || io.lui

  io.debug_regWen := io.regWen
  io.debug_wbData := io.wbData
}




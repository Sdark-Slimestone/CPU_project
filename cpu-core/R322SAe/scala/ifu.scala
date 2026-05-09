package R322SAe

import chisel3._
import chisel3.util._


//正常状态+8取两条指令给idu，如果idu鉴定为冲突返回stall信号，下周期重新取上周期第二条指令和下一条指令
//所有仲裁由idu做，ifu只负责取
//冲突时idu丢弃第二条指令
//正常 pc 0 8 16。。。
//一直冲突 pc 0 4 8 。。。
class IFU extends Module {
  val io = IO(new Bundle {
    val take_branch    = Input(Bool())
    val branch_target  = Input(UInt(32.W))
    val is_stall       = Input(Bool())
    //输出到imem的地址
    val imemAddr1      = Output(UInt(32.W))
    val imemAddr2      = Output(UInt(32.W))
    //取回来的指令
    val inst1fromimem  = Input(UInt(32.W))
    val inst2fromimem  = Input(UInt(32.W))
    //debug
    val debug_pc       = Output(UInt(32.W))
    //输出指令
    val inst1toidu     = Output(UInt(32.W))
    val inst2toidu     = Output(UInt(32.W))
    //输出指令下一条指令的地址
    val inst1nexttoidu = Output(UInt(32.W))
    val inst2nexttoidu = Output(UInt(32.W))
    //debug
    val current_pc     = Output(UInt(32.W))
  })


  //  jal/jalr/br 指令只能作为第一条指令进行单发射
  val pcReg   = RegInit("h80000000".U(32.W))
  val pcPlus4 = pcReg + 4.U
  val pcPlus8 = pcReg + 8.U
  val pcPlus12 = pcReg + 12.U
  
  //  pc值变化
  pcReg := Mux(io.is_stall, pcPlus4, Mux(io.take_branch, io.branch_target, pcPlus8))

  // 下一条指令地址计算
  val inst1next = pcPlus4 
  val inst2next = pcPlus8

  //接线
  io.imemAddr1 := pcReg
  io.imemAddr2 := pcPlus4

  io.inst1toidu := io.inst1fromimem
  io.inst2toidu := io.inst2fromimem
  io.inst1nexttoidu := inst1next
  io.inst2nexttoidu := inst2next

  
}
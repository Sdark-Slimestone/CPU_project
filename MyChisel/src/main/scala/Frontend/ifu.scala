package R1322IAe

import chisel3._
import chisel3.util._


//正常状态+8取两条指令给idu，如果idu鉴定为冲突返回stall信号，下周期重新取上周期第二条指令和下一条指令
//所有仲裁由idu做，ifu只负责取
//冲突时idu丢弃第二条指令
//正常 pc 0 8 16。。。
//一直冲突 pc 0 4 8 。。。
class IFU extends Module {
  val io = IO(new Bundle {
    val exu_to_ifu = new Bundle{
      val take_branch    = Input(Bool())
      val branch_target  = Input(UInt(32.W))
    }

    val idu_to_ifu = new Bundle{
      val is_stall       = Input(Bool())
    }
    val ifu_to_imem = new Bundle{
      val addr1      = Output(UInt(32.W))
      val addr2      = Output(UInt(32.W))
    }

    val imem_to_ifu = new Bundle{
      val inst1  = Input(UInt(32.W))
      val inst2  = Input(UInt(32.W))
    }

    val ifu_to_idu = new Bundle{
      val inst1     = Output(UInt(32.W))
      val inst2     = Output(UInt(32.W))
      val inst1_pc     = Output(UInt(32.W))
      val inst2_pc     = Output(UInt(32.W))
      val inst1_nextpc = Output(UInt(32.W))
      val inst2_nextpc = Output(UInt(32.W))
    }

    val debug = new Bundle{
      val debug_inst1_pc = Output(UInt(32.W))
      val debug_inst2_pc = Output(UInt(32.W))
    }
  })


  //  jal/jalr/br 指令只能作为第一条指令进行单发射
  val pcReg   = RegInit("h80000000".U(32.W))
  val pcPlus4 = pcReg + 4.U
  val pcPlus8 = pcReg + 8.U
  
  //  pc值变化
  pcReg := Mux(io.exu_to_ifu.take_branch, io.exu_to_ifu.branch_target, Mux(io.idu_to_ifu.is_stall, pcPlus4, pcPlus8))

  // 下一条指令地址计算
  val inst1next = pcPlus4 
  val inst2next = pcPlus8

  //接线
  io.ifu_to_imem.addr1 := pcReg
  io.ifu_to_imem.addr2 := pcPlus4

  io.ifu_to_idu.inst1 := io.imem_to_ifu.inst1
  io.ifu_to_idu.inst2 := io.imem_to_ifu.inst2
  io.ifu_to_idu.inst1_pc := pcReg
  io.ifu_to_idu.inst2_pc := pcPlus4
  io.ifu_to_idu.inst1_nextpc := inst1next
  io.ifu_to_idu.inst2_nextpc := inst2next

  io.debug.debug_inst1_pc := pcReg
  io.debug.debug_inst2_pc := pcPlus4

  
}
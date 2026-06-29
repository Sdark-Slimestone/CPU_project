package R1322IAeCSR

import chisel3._
import chisel3.util._

// 指令获取单元，支持双发射、分支跳转、异常和mret
class IFU extends Module {
  val io = IO(new Bundle {
    val exu_to_ifu = new Bundle{
      val take_branch    = Input(Bool())
      val branch_target  = Input(UInt(32.W))
    }

    val idu_to_ifu = new Bundle{
      val is_stall       = Input(Bool())
    }

    // CSR 异常/mret 接口
    val csr_to_ifu = new Bundle {
      val take_trap = Input(Bool())
      val trap_pc   = Input(UInt(32.W))
      val take_mret = Input(Bool())
      val mret_pc   = Input(UInt(32.W))
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

  // PC 寄存器，初始化为 0x80000000
  val pcReg   = RegInit("h80000000".U(32.W))
  val pcPlus4 = pcReg + 4.U
  val pcPlus8 = pcReg + 8.U

  // PC 更新逻辑：
  // 优先级: 异常跳转 > mret 返回 > 分支跳转 > stall(单步) > 正常+8
  pcReg := Mux(io.csr_to_ifu.take_trap, io.csr_to_ifu.trap_pc,
            Mux(io.csr_to_ifu.take_mret, io.csr_to_ifu.mret_pc,
            Mux(io.exu_to_ifu.take_branch, io.exu_to_ifu.branch_target,
            Mux(io.idu_to_ifu.is_stall, pcPlus4, pcPlus8))))

  // 取指地址输出
  io.ifu_to_imem.addr1 := pcReg
  io.ifu_to_imem.addr2 := pcPlus4

  // 指令输出到 IDU
  io.ifu_to_idu.inst1 := io.imem_to_ifu.inst1
  io.ifu_to_idu.inst2 := io.imem_to_ifu.inst2
  io.ifu_to_idu.inst1_pc := pcReg
  io.ifu_to_idu.inst2_pc := pcPlus4
  io.ifu_to_idu.inst1_nextpc := pcPlus4
  io.ifu_to_idu.inst2_nextpc := pcPlus8

  // 调试输出
  io.debug.debug_inst1_pc := pcReg
  io.debug.debug_inst2_pc := pcPlus4
}
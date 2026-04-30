package minirv

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

// 指令操作解码模块：根据 opcode、funct3、funct7 识别指定指令
class InstructionDecoder extends Module {
  val io = IO(new Bundle {
    val inst = Input(UInt(32.W))
    val is_addi = Output(Bool())
    val is_add  = Output(Bool())
    val is_jalr = Output(Bool())
    val is_lui  = Output(Bool())
    val is_lbu  = Output(Bool())
    val is_lw   = Output(Bool())
    val is_sb   = Output(Bool())
    val is_sw   = Output(Bool())
    val is_ebreak = Output(Bool())
  })

  val opcode = io.inst(6, 0)
  val funct3 = io.inst(14, 12)
  val funct7 = io.inst(31, 25)

  val OPCODE_ADDI  = "b0010011".U(7.W)  // 0x13
  val OPCODE_ADD   = "b0110011".U(7.W)  // 0x33
  val OPCODE_JALR  = "b1100111".U(7.W)  // 0x67
  val OPCODE_LUI   = "b0110111".U(7.W)  // 0x37
  val OPCODE_LOAD  = "b0000011".U(7.W)  // 0x03
  val OPCODE_STORE = "b0100011".U(7.W)  // 0x23
  val OPCODE_EBREAK= "b1110011".U(7.W)  // 0x73

  io.is_addi := (opcode === OPCODE_ADDI) && (funct3 === 0.U)
  io.is_add  := (opcode === OPCODE_ADD)  && (funct3 === 0.U) && (funct7 === 0.U)
  io.is_jalr := (opcode === OPCODE_JALR) && (funct3 === 0.U)
  io.is_lui  := (opcode === OPCODE_LUI)
  io.is_lbu  := (opcode === OPCODE_LOAD) && (funct3 === 4.U)
  io.is_lw   := (opcode === OPCODE_LOAD) && (funct3 === 2.U)
  io.is_sb   := (opcode === OPCODE_STORE) && (funct3 === 0.U)
  io.is_sw   := (opcode === OPCODE_STORE) && (funct3 === 2.U)
  io.is_ebreak := (opcode === OPCODE_EBREAK) && (funct3 === 0.U) && (funct7 === 0.U) &&
                  (io.inst(19,15) === 0.U) && (io.inst(11,7) === 0.U)
}

// 解码出指令信息
class InformationDecoder extends Module {
  val io = IO(new Bundle {
    val addi = Input(Bool())
    val add = Input(Bool())
    val lui = Input(Bool())
    val jalr = Input(Bool())
    val lbu = Input(Bool())
    val lw = Input(Bool())
    val sw = Input(Bool())
    val sb = Input(Bool())
    val inst = Input(UInt(32.W))
    val rd    = Output(UInt(5.W))
    val rs1   = Output(UInt(5.W))
    val rs2   = Output(UInt(5.W))
    val imm20 = Output(UInt(32.W))
    val imm12 = Output(UInt(32.W))
  })

  val imm12_i = io.inst(31, 20)
  val imm12_s = io.inst(31, 25) ## io.inst(11, 7)

  io.rd := Mux(io.addi || io.add || io.jalr || io.lui || io.lbu || io.lw,
               io.inst(11, 7), 0.U(5.W))
  io.rs1 := Mux(io.addi || io.add || io.jalr || io.lbu || io.lw || io.sw || io.sb,
                io.inst(19, 15), 0.U(5.W))
  io.rs2 := Mux(io.add || io.sw || io.sb,
                io.inst(24, 20), 0.U(5.W))
  io.imm20 := Mux(io.lui, io.inst(31, 12) ## 0.U(12.W), 0.U(32.W))
  
  val imm12_pre_extended = Mux(io.addi || io.jalr || io.lbu || io.lw || io.sw || io.sb,
                               Mux(io.addi || io.jalr || io.lbu || io.lw, imm12_i, imm12_s),
                               0.U(12.W))
  val extend = Fill(20, imm12_pre_extended(11))
  io.imm12 := Mux(io.addi || io.jalr || io.lbu || io.lw || io.sw || io.sb,
                  extend ## imm12_pre_extended, 0.U(32.W))
}

// 顶层模块 idu，输出指令类型和操作数值
class idu extends Module {
  val io = IO(new Bundle {
    val inst = Input(UInt(32.W))
    // 指令类型输出
    val is_addi   = Output(Bool())
    val is_add    = Output(Bool())
    val is_jalr   = Output(Bool())
    val is_lui    = Output(Bool())
    val is_lbu    = Output(Bool())
    val is_lw     = Output(Bool())
    val is_sb     = Output(Bool())
    val is_sw     = Output(Bool())
    val is_ebreak = Output(Bool())   // <-- 新增 ebreak 输出

    // 操作数数值输出
    val rd    = Output(UInt(5.W))
    val rs1   = Output(UInt(5.W))
    val rs2   = Output(UInt(5.W))
    val imm20 = Output(UInt(32.W))
    val imm12 = Output(UInt(32.W))

    //指令输出
    val debug_inst = Output(UInt(32.W))
  })

  val instDecoder = Module(new InstructionDecoder)
  instDecoder.io.inst := io.inst

  val infoDecoder = Module(new InformationDecoder)
  infoDecoder.io.inst := io.inst
  infoDecoder.io.addi := instDecoder.io.is_addi
  infoDecoder.io.add  := instDecoder.io.is_add
  infoDecoder.io.jalr := instDecoder.io.is_jalr
  infoDecoder.io.lui  := instDecoder.io.is_lui
  infoDecoder.io.lbu  := instDecoder.io.is_lbu
  infoDecoder.io.lw   := instDecoder.io.is_lw
  infoDecoder.io.sb   := instDecoder.io.is_sb
  infoDecoder.io.sw   := instDecoder.io.is_sw

  // 输出指令类型
  io.is_addi   := instDecoder.io.is_addi
  io.is_add    := instDecoder.io.is_add
  io.is_jalr   := instDecoder.io.is_jalr
  io.is_lui    := instDecoder.io.is_lui
  io.is_lbu    := instDecoder.io.is_lbu
  io.is_lw     := instDecoder.io.is_lw
  io.is_sb     := instDecoder.io.is_sb
  io.is_sw     := instDecoder.io.is_sw
  io.is_ebreak := instDecoder.io.is_ebreak   //  连接 ebreak 信号

  // 输出操作数数值
  io.rd    := infoDecoder.io.rd
  io.rs1   := infoDecoder.io.rs1
  io.rs2   := infoDecoder.io.rs2
  io.imm20 := infoDecoder.io.imm20
  io.imm12 := infoDecoder.io.imm12

  io.debug_inst := io.inst
}

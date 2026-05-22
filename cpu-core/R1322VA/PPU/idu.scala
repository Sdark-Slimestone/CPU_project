package rv32e

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

// 指令操作解码模块：根据 opcode、funct3、funct7 识别指定指令
class InstructionDecoder extends Module {
  val io = IO(new Bundle {
    val inst = Input(UInt(32.W))
    val splui     = Output(Bool())
    val spaddi    = Output(Bool())
    val strdli    = Output(Bool())
    val cntli     = Output(Bool())
    val pcb       = Output(Bool())
    val pacb      = Output(Bool())
    val pfend     = Output(Bool())
  })

  val opcode = io.inst(3, 0)

  val OPCODE_splui     = "b0001".U(4.W)
  val OPCODE_spaddi    = "b0010".U(4.W)
  val OPCODE_strdli    = "b0011".U(4.W)
  val OPCODE_cntli     = "b0100".U(4.W)
  val OPCODE_pcb       = "b0101".U(4.W)
  val OPCODE_pacb      = "b0110".U(4.W)
  val OPCODE_pfend     = "b0111".U(4.W)

  io.splui    := opcode === OPCODE_splui
  io.spaddi   := opcode === OPCODE_spaddi
  io.strdli   := opcode === OPCODE_strdli
  io.cntli    := opcode === OPCODE_cntli
  io.pcb      := opcode === OPCODE_pcb
  io.pacb     := opcede === OPCODE_pacb
  io.pfend    := opcode === OPCODE_pfend
}

class InformationDecoder extends Module {
  val io = IO(new Bundle {
    val inst      = Input(UInt(32.W))
    val is_splui  = Input(Bool())
    val is_spaddi = Input(Bool())
    val is_strdli = Input(Bool())
    val is_cntli  = Input(Bool())
    val is_pcb    = Input(Bool())
    val is_pacb   = Input(Bool())
    val is_pfend  = Input(Bool())
    
    val spimmh        = Output(UInt(16.W))
    val spimml        = Output(UInt(16.W))
    val strdimm       = Output(UInt(16.W))
    val cntimm        = Output(UInt(16.W))
    val strd_rd       = Output(UInt(4.W))
    val cnt_rd        = Output(UInt(4.W))
    val pacb_rs       = Output(UInt(4.W))
    val pcb_jump0     = Output(UInt(6.W))
    val pcb_jump1     = Output(UInt(6.W))
    val pacb_jump0    = Output(UInt(6.W))
    val pacb_jump1    = Output(UInt(6.W))
  })

  val wr_rd = io.is_lui || io.is_auipc || io.is_jal || io.is_jalr ||
              io.is_lb || io.is_lh || io.is_lw || io.is_lbu || io.is_lhu ||
              io.is_addi || io.is_slti || io.is_sltiu || io.is_xori ||
              io.is_ori || io.is_andi || io.is_slli || io.is_srli || io.is_srai ||
              io.is_add || io.is_sub || io.is_sll || io.is_slt || io.is_sltu ||
              io.is_xor || io.is_srl || io.is_sra || io.is_or || io.is_and
  io.rd := Mux(wr_rd, io.inst(11, 7), 0.U(5.W))

  val use_rs1 = io.is_jalr ||
                io.is_beq || io.is_bne || io.is_blt || io.is_bge || io.is_bltu || io.is_bgeu ||
                io.is_lb || io.is_lh || io.is_lw || io.is_lbu || io.is_lhu ||
                io.is_sb || io.is_sh || io.is_sw ||
                io.is_addi || io.is_slti || io.is_sltiu || io.is_xori ||
                io.is_ori || io.is_andi || io.is_slli || io.is_srli || io.is_srai ||
                io.is_add || io.is_sub || io.is_sll || io.is_slt || io.is_sltu ||
                io.is_xor || io.is_srl || io.is_sra || io.is_or || io.is_and
  io.rs1 := Mux(use_rs1, io.inst(19, 15), 0.U(5.W))

  val use_rs2 = io.is_beq || io.is_bne || io.is_blt || io.is_bge || io.is_bltu || io.is_bgeu ||
                io.is_sb || io.is_sh || io.is_sw ||
                io.is_add || io.is_sub || io.is_sll || io.is_slt || io.is_sltu ||
                io.is_xor || io.is_srl || io.is_sra || io.is_or || io.is_and
  io.rs2 := Mux(use_rs2, io.inst(24, 20), 0.U(5.W))

  val imm_i_raw = io.inst(31, 20)
  val imm_i_sext = Cat(Fill(20, imm_i_raw(11)), imm_i_raw)
  val need_imm_i = io.is_jalr || io.is_lb || io.is_lh || io.is_lw || io.is_lbu || io.is_lhu ||
                   io.is_addi || io.is_slti || io.is_sltiu || io.is_xori ||
                   io.is_ori || io.is_andi || io.is_slli || io.is_srli || io.is_srai
  io.imm_i := Mux(need_imm_i, imm_i_sext, 0.U(32.W))

  val imm_s_raw = Cat(io.inst(31, 25), io.inst(11, 7))
  val imm_s_sext = Cat(Fill(20, imm_s_raw(11)), imm_s_raw)
  val need_imm_s = io.is_sb || io.is_sh || io.is_sw
  io.imm_s := Mux(need_imm_s, imm_s_sext, 0.U(32.W))

  val imm_b_raw = Cat(io.inst(31), io.inst(7), io.inst(30, 25), io.inst(11, 8), 0.U(1.W))
  val imm_b_sext = Cat(Fill(19, imm_b_raw(12)), imm_b_raw)
  val need_imm_b = io.is_beq || io.is_bne || io.is_blt || io.is_bge || io.is_bltu || io.is_bgeu
  io.imm_b := Mux(need_imm_b, imm_b_sext, 0.U(32.W))

  val imm_u_raw = Cat(io.inst(31, 12), 0.U(12.W))
  val need_imm_u = io.is_lui || io.is_auipc
  io.imm_u := Mux(need_imm_u, imm_u_raw, 0.U(32.W))

  val imm_j_raw = Cat(io.inst(31), io.inst(19, 12), io.inst(20), io.inst(30, 21), 0.U(1.W))
  val imm_j_sext = Cat(Fill(11, imm_j_raw(20)), imm_j_raw)
  val need_imm_j = io.is_jal
  io.imm_j := Mux(need_imm_j, imm_j_sext, 0.U(32.W))
}

class idu extends Module {
  val io = IO(new Bundle {
    val inst        = Input(UInt(32.W))
    // 所有指令识别信号（供其他模块使用）
    val is_lui      = Output(Bool())
    val is_auipc    = Output(Bool())
    val is_jal      = Output(Bool())
    val is_jalr     = Output(Bool())
    val is_beq      = Output(Bool())
    val is_bne      = Output(Bool())
    val is_blt      = Output(Bool())
    val is_bge      = Output(Bool())
    val is_bltu     = Output(Bool())
    val is_bgeu     = Output(Bool())
    val is_lb       = Output(Bool())
    val is_lh       = Output(Bool())
    val is_lw       = Output(Bool())
    val is_lbu      = Output(Bool())
    val is_lhu      = Output(Bool())
    val is_sb       = Output(Bool())
    val is_sh       = Output(Bool())
    val is_sw       = Output(Bool())
    val is_addi     = Output(Bool())
    val is_slti     = Output(Bool())
    val is_sltiu    = Output(Bool())
    val is_xori     = Output(Bool())
    val is_ori      = Output(Bool())
    val is_andi     = Output(Bool())
    val is_slli     = Output(Bool())
    val is_srli     = Output(Bool())
    val is_srai     = Output(Bool())
    val is_add      = Output(Bool())
    val is_sub      = Output(Bool())
    val is_sll      = Output(Bool())
    val is_slt      = Output(Bool())
    val is_sltu     = Output(Bool())
    val is_xor      = Output(Bool())
    val is_srl      = Output(Bool())
    val is_sra      = Output(Bool())
    val is_or       = Output(Bool())
    val is_and      = Output(Bool())
    val is_ebreak   = Output(Bool())
    // 寄存器地址
    val rd          = Output(UInt(5.W))
    val rs1         = Output(UInt(5.W))
    val rs2         = Output(UInt(5.W))
    // 立即数
    val imm_i       = Output(UInt(32.W))
    val imm_s       = Output(UInt(32.W))
    val imm_b       = Output(UInt(32.W))
    val imm_u       = Output(UInt(32.W))
    val imm_j       = Output(UInt(32.W))
    // 兼容旧端口
    val imm12       = Output(UInt(32.W))
    val imm20       = Output(UInt(32.W))
    val debug_inst  = Output(UInt(32.W))
  })

  val instDecoder = Module(new InstructionDecoder)
  instDecoder.io.inst := io.inst

  val infoDecoder = Module(new InformationDecoder)
  infoDecoder.io.inst     := io.inst
  infoDecoder.io.is_lui   := instDecoder.io.is_lui
  infoDecoder.io.is_auipc := instDecoder.io.is_auipc
  infoDecoder.io.is_jal   := instDecoder.io.is_jal
  infoDecoder.io.is_jalr  := instDecoder.io.is_jalr
  infoDecoder.io.is_beq   := instDecoder.io.is_beq
  infoDecoder.io.is_bne   := instDecoder.io.is_bne
  infoDecoder.io.is_blt   := instDecoder.io.is_blt
  infoDecoder.io.is_bge   := instDecoder.io.is_bge
  infoDecoder.io.is_bltu  := instDecoder.io.is_bltu
  infoDecoder.io.is_bgeu  := instDecoder.io.is_bgeu
  infoDecoder.io.is_lb    := instDecoder.io.is_lb
  infoDecoder.io.is_lh    := instDecoder.io.is_lh
  infoDecoder.io.is_lw    := instDecoder.io.is_lw
  infoDecoder.io.is_lbu   := instDecoder.io.is_lbu
  infoDecoder.io.is_lhu   := instDecoder.io.is_lhu
  infoDecoder.io.is_sb    := instDecoder.io.is_sb
  infoDecoder.io.is_sh    := instDecoder.io.is_sh
  infoDecoder.io.is_sw    := instDecoder.io.is_sw
  infoDecoder.io.is_addi  := instDecoder.io.is_addi
  infoDecoder.io.is_slti  := instDecoder.io.is_slti
  infoDecoder.io.is_sltiu := instDecoder.io.is_sltiu
  infoDecoder.io.is_xori  := instDecoder.io.is_xori
  infoDecoder.io.is_ori   := instDecoder.io.is_ori
  infoDecoder.io.is_andi  := instDecoder.io.is_andi
  infoDecoder.io.is_slli  := instDecoder.io.is_slli
  infoDecoder.io.is_srli  := instDecoder.io.is_srli
  infoDecoder.io.is_srai  := instDecoder.io.is_srai
  infoDecoder.io.is_add   := instDecoder.io.is_add
  infoDecoder.io.is_sub   := instDecoder.io.is_sub
  infoDecoder.io.is_sll   := instDecoder.io.is_sll
  infoDecoder.io.is_slt   := instDecoder.io.is_slt
  infoDecoder.io.is_sltu  := instDecoder.io.is_sltu
  infoDecoder.io.is_xor   := instDecoder.io.is_xor
  infoDecoder.io.is_srl   := instDecoder.io.is_srl
  infoDecoder.io.is_sra   := instDecoder.io.is_sra
  infoDecoder.io.is_or    := instDecoder.io.is_or
  infoDecoder.io.is_and   := instDecoder.io.is_and

  // 输出所有指令识别信号
  io.is_lui   := instDecoder.io.is_lui
  io.is_auipc := instDecoder.io.is_auipc
  io.is_jal   := instDecoder.io.is_jal
  io.is_jalr  := instDecoder.io.is_jalr
  io.is_beq   := instDecoder.io.is_beq
  io.is_bne   := instDecoder.io.is_bne
  io.is_blt   := instDecoder.io.is_blt
  io.is_bge   := instDecoder.io.is_bge
  io.is_bltu  := instDecoder.io.is_bltu
  io.is_bgeu  := instDecoder.io.is_bgeu
  io.is_lb    := instDecoder.io.is_lb
  io.is_lh    := instDecoder.io.is_lh
  io.is_lw    := instDecoder.io.is_lw
  io.is_lbu   := instDecoder.io.is_lbu
  io.is_lhu   := instDecoder.io.is_lhu
  io.is_sb    := instDecoder.io.is_sb
  io.is_sh    := instDecoder.io.is_sh
  io.is_sw    := instDecoder.io.is_sw
  io.is_addi  := instDecoder.io.is_addi
  io.is_slti  := instDecoder.io.is_slti
  io.is_sltiu := instDecoder.io.is_sltiu
  io.is_xori  := instDecoder.io.is_xori
  io.is_ori   := instDecoder.io.is_ori
  io.is_andi  := instDecoder.io.is_andi
  io.is_slli  := instDecoder.io.is_slli
  io.is_srli  := instDecoder.io.is_srli
  io.is_srai  := instDecoder.io.is_srai
  io.is_add   := instDecoder.io.is_add
  io.is_sub   := instDecoder.io.is_sub
  io.is_sll   := instDecoder.io.is_sll
  io.is_slt   := instDecoder.io.is_slt
  io.is_sltu  := instDecoder.io.is_sltu
  io.is_xor   := instDecoder.io.is_xor
  io.is_srl   := instDecoder.io.is_srl
  io.is_sra   := instDecoder.io.is_sra
  io.is_or    := instDecoder.io.is_or
  io.is_and   := instDecoder.io.is_and
  io.is_ebreak := instDecoder.io.is_ebreak

  io.rd  := infoDecoder.io.rd
  io.rs1 := infoDecoder.io.rs1
  io.rs2 := infoDecoder.io.rs2

  io.imm_i := infoDecoder.io.imm_i
  io.imm_s := infoDecoder.io.imm_s
  io.imm_b := infoDecoder.io.imm_b
  io.imm_u := infoDecoder.io.imm_u
  io.imm_j := infoDecoder.io.imm_j
  io.imm12 := infoDecoder.io.imm_i
  io.imm20 := infoDecoder.io.imm_u

  io.debug_inst := io.inst
}
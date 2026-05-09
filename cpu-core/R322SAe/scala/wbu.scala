package rv32e

import chisel3._
import chisel3.util._

class WBU extends Module {
  val io = IO(new Bundle {
    val is_lui    = Input(Bool())
    val is_auipc  = Input(Bool())
    val is_jal    = Input(Bool())
    val is_jalr   = Input(Bool())
    val is_lb     = Input(Bool())
    val is_lh     = Input(Bool())
    val is_lw     = Input(Bool())
    val is_lbu    = Input(Bool())
    val is_lhu    = Input(Bool())
    val is_addi   = Input(Bool())
    val is_slti   = Input(Bool())
    val is_sltiu  = Input(Bool())
    val is_xori   = Input(Bool())
    val is_ori    = Input(Bool())
    val is_andi   = Input(Bool())
    val is_slli   = Input(Bool())
    val is_srli   = Input(Bool())
    val is_srai   = Input(Bool())
    val is_add    = Input(Bool())
    val is_sub    = Input(Bool())
    val is_sll    = Input(Bool())
    val is_slt    = Input(Bool())
    val is_sltu   = Input(Bool())
    val is_xor    = Input(Bool())
    val is_srl    = Input(Bool())
    val is_sra    = Input(Bool())
    val is_or     = Input(Bool())
    val is_and    = Input(Bool())

    val inputfromALU = Input(UInt(32.W))
    val inputfromPC  = Input(UInt(32.W))       // PC+4 (for JAL, JALR)
    val inputfromRAM = Input(UInt(32.W))
    val inputfromAUIPC = Input(UInt(32.W))     // AUIPC 结果已在 ALU 计算（PC+imm_u）

    val wbData = Output(UInt(32.W))
    val regWen = Output(Bool())

    val debug_regWen = Output(Bool())
    val debug_wbData = Output(UInt(32.W))
  })

  // 所有需要写寄存器的指令
  val need_wb = io.is_lui || io.is_auipc || io.is_jal || io.is_jalr ||
                io.is_lb || io.is_lh || io.is_lw || io.is_lbu || io.is_lhu ||
                io.is_addi || io.is_slti || io.is_sltiu || io.is_xori ||
                io.is_ori || io.is_andi || io.is_slli || io.is_srli || io.is_srai ||
                io.is_add || io.is_sub || io.is_sll || io.is_slt || io.is_sltu ||
                io.is_xor || io.is_srl || io.is_sra || io.is_or || io.is_and

  io.regWen := need_wb

  io.wbData := MuxCase(0.U(32.W), Seq(
    (io.is_lui)                    -> io.inputfromALU,   // LUI 已通过 ALU 直通
    (io.is_auipc)                  -> io.inputfromAUIPC, // AUIPC 结果来自 ALU (PC+imm_u)
    (io.is_jal || io.is_jalr)      -> io.inputfromPC,    // JAL/JALR 写回 PC+4
    (io.is_lb || io.is_lh || io.is_lw || io.is_lbu || io.is_lhu) -> io.inputfromRAM,
    // 其余 ALU 结果（包括所有 I/R 型运算）
    (io.is_addi || io.is_slti || io.is_sltiu || io.is_xori || io.is_ori || io.is_andi ||
     io.is_slli || io.is_srli || io.is_srai ||
     io.is_add || io.is_sub || io.is_sll || io.is_slt || io.is_sltu ||
     io.is_xor || io.is_srl || io.is_sra || io.is_or || io.is_and) -> io.inputfromALU
  ))

  io.debug_regWen := io.regWen
  io.debug_wbData := io.wbData
}
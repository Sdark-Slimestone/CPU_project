package R1322IAeCSR

import chisel3._
import chisel3.util._

// 译码结果: 控制信号 + 寄存器地址 + 立即数
class DecodeResult extends Bundle {
  val ctrl = new CtrlSignals
  val rd   = UInt(5.W)
  val rs1  = UInt(5.W)
  val rs2  = UInt(5.W)
  val imm  = UInt(32.W)
}

// 粗粒度控制信号: 直接由 opcode 一级比较得到 (不等一热 OR 链), 供 IDU 冒险检测使用,
// 缩短 D 级关键路径。对所有合法指令与 CtrlSignals 的派生谓词等价 (非法编码下偏保守)。
class CoarseCtrl extends Bundle {
  val is_control = Bool()
  val is_load    = Bool()
  val is_store   = Bool()
  val is_branch  = Bool()
  val is_jump    = Bool()
  val is_muldiv  = Bool()
}

// 指令译码器: inst -> DecodeResult。
// 解码与信息提取合并在一个模块内, 避免旧版 InstructionDecoder/InformationDecoder
// 之间近百行逐信号手工连线; ctrl 里的派生谓词直接用于选通 rd/rs1/rs2/imm。
class Decoder extends Module {
  val io = IO(new Bundle {
    val inst       = Input(UInt(32.W))
    val out        = Output(new DecodeResult)
    val coarse     = Output(new CoarseCtrl)
    val debug_inst = Output(UInt(32.W))
  })

  val c = io.out.ctrl

  val opcode = io.inst(6, 0)
  val funct3 = io.inst(14, 12)
  val funct7 = io.inst(31, 25)

  val OPCODE_LUI    = "b0110111".U(7.W)
  val OPCODE_AUIPC  = "b0010111".U(7.W)
  val OPCODE_JAL    = "b1101111".U(7.W)
  val OPCODE_JALR   = "b1100111".U(7.W)
  val OPCODE_B      = "b1100011".U(7.W)
  val OPCODE_LOAD   = "b0000011".U(7.W)
  val OPCODE_S      = "b0100011".U(7.W)
  val OPCODE_I      = "b0010011".U(7.W)
  val OPCODE_R      = "b0110011".U(7.W)
  val OPCODE_SYSTEM = "b1110011".U(7.W)

  val is_system = opcode === OPCODE_SYSTEM
  val is_csr    = is_system && (funct3 =/= 0.U)

  // opcode 级类别: 每个 opcode 只比较一次, 供控制位/rd/rs/imm 复用
  // (避免下游对十几~几十个一热信号再做 OR 链)
  val op_lui   = opcode === OPCODE_LUI
  val op_auipc = opcode === OPCODE_AUIPC
  val op_jal   = opcode === OPCODE_JAL
  val op_jalr  = opcode === OPCODE_JALR
  val op_b     = opcode === OPCODE_B
  val op_load  = opcode === OPCODE_LOAD
  val op_s     = opcode === OPCODE_S
  val op_i     = opcode === OPCODE_I
  val op_r     = opcode === OPCODE_R

  // ---------------- 操作码识别 ----------------
  c.is_lui   := op_lui
  c.is_auipc := op_auipc
  c.is_jal   := op_jal
  c.is_jalr  := op_jalr && (funct3 === 0.U)

  c.is_beq  := op_b && (funct3 === 0.U)
  c.is_bne  := op_b && (funct3 === 1.U)
  c.is_blt  := op_b && (funct3 === 4.U)
  c.is_bge  := op_b && (funct3 === 5.U)
  c.is_bltu := op_b && (funct3 === 6.U)
  c.is_bgeu := op_b && (funct3 === 7.U)

  c.is_lb  := op_load && (funct3 === 0.U)
  c.is_lh  := op_load && (funct3 === 1.U)
  c.is_lw  := op_load && (funct3 === 2.U)
  c.is_lbu := op_load && (funct3 === 4.U)
  c.is_lhu := op_load && (funct3 === 5.U)

  c.is_sb := op_s && (funct3 === 0.U)
  c.is_sh := op_s && (funct3 === 1.U)
  c.is_sw := op_s && (funct3 === 2.U)

  c.is_addi  := op_i && (funct3 === 0.U)
  c.is_slti  := op_i && (funct3 === 2.U)
  c.is_sltiu := op_i && (funct3 === 3.U)
  c.is_xori  := op_i && (funct3 === 4.U)
  c.is_ori   := op_i && (funct3 === 6.U)
  c.is_andi  := op_i && (funct3 === 7.U)
  c.is_slli  := op_i && (funct3 === 1.U) && (funct7 === 0.U)
  c.is_srli  := op_i && (funct3 === 5.U) && (funct7 === 0.U)
  c.is_srai  := op_i && (funct3 === 5.U) && (funct7 === "b0100000".U(7.W))

  c.is_add := op_r && (funct3 === 0.U) && (funct7 === 0.U)
  c.is_sub := op_r && (funct3 === 0.U) && (funct7 === "b0100000".U(7.W))
  c.is_sll := op_r && (funct3 === 1.U) && (funct7 === 0.U)
  c.is_slt := op_r && (funct3 === 2.U) && (funct7 === 0.U)
  c.is_sltu:= op_r && (funct3 === 3.U) && (funct7 === 0.U)
  c.is_xor := op_r && (funct3 === 4.U) && (funct7 === 0.U)
  c.is_srl := op_r && (funct3 === 5.U) && (funct7 === 0.U)
  c.is_sra := op_r && (funct3 === 5.U) && (funct7 === "b0100000".U(7.W))
  c.is_or  := op_r && (funct3 === 6.U) && (funct7 === 0.U)
  c.is_and := op_r && (funct3 === 7.U) && (funct7 === 0.U)

  // RV32M: opcode=R, funct7=0000001, funct3 选 8 条
  val is_m = op_r && (funct7 === "b0000001".U(7.W))
  c.is_mul    := is_m && (funct3 === 0.U)
  c.is_mulh   := is_m && (funct3 === 1.U)
  c.is_mulhsu := is_m && (funct3 === 2.U)
  c.is_mulhu  := is_m && (funct3 === 3.U)
  c.is_div    := is_m && (funct3 === 4.U)
  c.is_divu   := is_m && (funct3 === 5.U)
  c.is_rem    := is_m && (funct3 === 6.U)
  c.is_remu   := is_m && (funct3 === 7.U)

  c.is_csrrw  := is_csr && (funct3 === 1.U)
  c.is_csrrs  := is_csr && (funct3 === 2.U)
  c.is_csrrc  := is_csr && (funct3 === 3.U)
  c.is_csrrwi := is_csr && (funct3 === 5.U)
  c.is_csrrsi := is_csr && (funct3 === 6.U)
  c.is_csrrci := is_csr && (funct3 === 7.U)

  // ecall: funct3=0, imm=0, rs1=0, rd=0
  c.is_ecall := is_system && (funct3 === 0.U) &&
                (io.inst(31, 20) === 0.U) && (io.inst(19, 15) === 0.U) &&
                (io.inst(11, 7) === 0.U) && (funct7 === 0.U)

  // ebreak: funct3=0, imm=1, rs1=0, rd=0
  c.is_ebreak := is_system && (funct3 === 0.U) &&
                 (io.inst(31, 20) === 1.U) && (io.inst(19, 15) === 0.U) &&
                 (io.inst(11, 7) === 0.U) && (funct7 === 0.U)

  // mret: funct3=0, imm=0x302, rs1=0, rd=0
  c.is_mret := is_system && (funct3 === 0.U) &&
               (io.inst(31, 20) === 0x302.U(12.W)) && (io.inst(19, 15) === 0.U) &&
               (io.inst(11, 7) === 0.U)

  // ---------------- 粗粒度控制信号 (一级 opcode 比较) ----------------
  io.coarse.is_jump    := op_jal || op_jalr
  io.coarse.is_branch  := op_b
  io.coarse.is_load    := op_load
  io.coarse.is_store   := op_s
  io.coarse.is_control := io.coarse.is_jump || io.coarse.is_branch || is_system
  // RV32M: 8 条指令共用 opcode=R, funct7=0000001, 一次比较即可 (供 IDU 单发射判定)
  io.coarse.is_muldiv  := is_m

  // 粗粒度谓词写入 CtrlSignals (供 EXU 直接使用, 避免下游多热 OR)
  c.is_jump    := op_jal || op_jalr
  c.is_branch  := op_b
  c.is_load    := op_load
  c.is_store   := op_s
  c.is_imm_op  := op_i
  c.is_alu_op  := op_lui || op_auipc || op_i || op_r
  c.is_csr     := is_csr

  // ---------------- 寄存器地址 ----------------
  // 用 opcode 类别直接选通, 不再对十几~几十个一热信号做 OR
  val wr_rd = op_lui || op_auipc || op_jal || op_jalr || op_load || op_i || op_r || is_csr
  io.out.rd  := Mux(wr_rd, io.inst(11, 7), 0.U(5.W))

  val csr_use_rs1 = c.is_csrrw || c.is_csrrs || c.is_csrrc
  val use_rs1 = op_jalr || op_b || op_load || op_s || op_i || op_r || csr_use_rs1
  io.out.rs1 := Mux(use_rs1, io.inst(19, 15), 0.U(5.W))

  val use_rs2 = op_b || op_s || op_r
  io.out.rs2 := Mux(use_rs2, io.inst(24, 20), 0.U(5.W))

  // ---------------- 立即数提取与符号扩展 ----------------
  val imm_i_raw  = io.inst(31, 20)
  val imm_i_sext = Cat(Fill(20, imm_i_raw(11)), imm_i_raw)

  val imm_s_raw  = Cat(io.inst(31, 25), io.inst(11, 7))
  val imm_s_sext = Cat(Fill(20, imm_s_raw(11)), imm_s_raw)

  val imm_b_raw  = Cat(io.inst(31), io.inst(7), io.inst(30, 25), io.inst(11, 8), 0.U(1.W))
  val imm_b_sext = Cat(Fill(19, imm_b_raw(12)), imm_b_raw)

  val imm_u_raw  = Cat(io.inst(31, 12), 0.U(12.W))

  val imm_j_raw  = Cat(io.inst(31), io.inst(19, 12), io.inst(20), io.inst(30, 21), 0.U(1.W))
  val imm_j_sext = Cat(Fill(11, imm_j_raw(20)), imm_j_raw)

  io.out.imm := PriorityMux(Seq(
    (op_jalr || op_load || op_i) -> imm_i_sext,
    op_s                         -> imm_s_sext,
    op_b                         -> imm_b_sext,
    (op_lui || op_auipc)         -> imm_u_raw,
    op_jal                       -> imm_j_sext,
    true.B                       -> 0.U(32.W)
  ))

  io.debug_inst := io.inst
}

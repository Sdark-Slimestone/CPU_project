package R322SAe

import chisel3._
import chisel3.util._

class EXU extends Module {
  val io = IO(new Bundle {
    val idu_to_exu1 = new Bundle {
      val dec1_op = new Bundle {
        val is_lui    = Input(Bool())
        val is_auipc  = Input(Bool())
        val is_jal    = Input(Bool())
        val is_jalr   = Input(Bool())
        val is_beq    = Input(Bool())
        val is_bne    = Input(Bool())
        val is_blt    = Input(Bool())
        val is_bge    = Input(Bool())
        val is_bltu   = Input(Bool())
        val is_bgeu   = Input(Bool())
        val is_lb     = Input(Bool())
        val is_lh     = Input(Bool())
        val is_lw     = Input(Bool())
        val is_lbu    = Input(Bool())
        val is_lhu    = Input(Bool())
        val is_sb     = Input(Bool())
        val is_sh     = Input(Bool())
        val is_sw     = Input(Bool())
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
        val is_ebreak = Input(Bool())
      }
      val dec1_imm = Input(UInt(32.W))
      val dec1_val = new Bundle {
        val rs1_val = Input(UInt(32.W))
        val rs2_val = Input(UInt(32.W))
        val nextpc  = Input(UInt(32.W))
      }
      val dec1_rd = Input(UInt(5.W))
    }

    val exu_to_lsu = new Bundle {
      val op = new Bundle {
        val is_lb     = Output(Bool())
        val is_lh     = Output(Bool())
        val is_lw     = Output(Bool())
        val is_lbu    = Output(Bool())
        val is_lhu    = Output(Bool())
        val is_sb     = Output(Bool())
        val is_sh     = Output(Bool())
        val is_sw     = Output(Bool())
        val is_ebreak = Output(Bool())
      }

      val paddr = new Bundle {
        val addr = Output(UInt(32.W))
      }

      val data = new Bundle {
        val store_data = Output(UInt(32.W))
      }

      val exu_through_lsu_to_wbu = new Bundle {
        val rd = Output(UInt(5.W))
        val grf_wb_data = Output(UInt(32.W))
      }
    }

    val exu_to_ifu = new Bundle {
      val take_branch   = Output(Bool())
      val branch_target = Output(UInt(32.W))
    }

    //debug
    val debug_alu_out = Output(UInt(32.W))
    val debug_alu_source1 = Output(UInt(32.W))
    val debug_alu_source2 = Output(UInt(32.W))
    val debug_agu_out = Output(UInt(32.W))
  })

  // 基础信号
  val pc   = io.idu_to_exu1.dec1_val.nextpc - 4.U
  val rs1  = io.idu_to_exu1.dec1_val.rs1_val
  val rs2  = io.idu_to_exu1.dec1_val.rs2_val
  val imm  = io.idu_to_exu1.dec1_imm

  // 移位量
  val is_shift_i = io.idu_to_exu1.dec1_op.is_slli ||
                   io.idu_to_exu1.dec1_op.is_srli ||
                   io.idu_to_exu1.dec1_op.is_srai
  val shamt = Mux(is_shift_i, imm(4,0), rs2(4,0))

  // ---------- ALU 结果 ----------
  val aluOut_lui    = imm
  val aluOut_auipc  = pc + imm
  val aluOut_add    = rs1 + Mux(io.idu_to_exu1.dec1_op.is_addi, imm, rs2)
  val aluOut_sub    = rs1 - rs2
  val aluOut_slt    = Mux(rs1.asSInt < Mux(io.idu_to_exu1.dec1_op.is_slti, imm.asSInt, rs2.asSInt), 1.U, 0.U)
  val aluOut_sltu   = Mux(rs1 < Mux(io.idu_to_exu1.dec1_op.is_sltiu, imm, rs2), 1.U, 0.U)
  val aluOut_xor    = rs1 ^ Mux(io.idu_to_exu1.dec1_op.is_xori, imm, rs2)
  val aluOut_or     = rs1 | Mux(io.idu_to_exu1.dec1_op.is_ori, imm, rs2)
  val aluOut_and    = rs1 & Mux(io.idu_to_exu1.dec1_op.is_andi, imm, rs2)
  val aluOut_sll    = rs1 << shamt
  val aluOut_srl    = rs1 >> shamt
  val aluOut_sra    = (rs1.asSInt >> shamt).asUInt
  val aluOut_jal    = pc + 4.U

  val alu_out = MuxCase(0.U, Seq(
    io.idu_to_exu1.dec1_op.is_lui    -> aluOut_lui,
    io.idu_to_exu1.dec1_op.is_auipc  -> aluOut_auipc,
    (io.idu_to_exu1.dec1_op.is_addi || io.idu_to_exu1.dec1_op.is_add) -> aluOut_add,
    io.idu_to_exu1.dec1_op.is_sub    -> aluOut_sub,
    (io.idu_to_exu1.dec1_op.is_slti || io.idu_to_exu1.dec1_op.is_slt) -> aluOut_slt,
    (io.idu_to_exu1.dec1_op.is_sltiu || io.idu_to_exu1.dec1_op.is_sltu) -> aluOut_sltu,
    (io.idu_to_exu1.dec1_op.is_xori || io.idu_to_exu1.dec1_op.is_xor) -> aluOut_xor,
    (io.idu_to_exu1.dec1_op.is_ori  || io.idu_to_exu1.dec1_op.is_or)  -> aluOut_or,
    (io.idu_to_exu1.dec1_op.is_andi || io.idu_to_exu1.dec1_op.is_and) -> aluOut_and,
    (io.idu_to_exu1.dec1_op.is_slli || io.idu_to_exu1.dec1_op.is_sll) -> aluOut_sll,
    (io.idu_to_exu1.dec1_op.is_srli || io.idu_to_exu1.dec1_op.is_srl) -> aluOut_srl,
    (io.idu_to_exu1.dec1_op.is_srai || io.idu_to_exu1.dec1_op.is_sra) -> aluOut_sra,
    (io.idu_to_exu1.dec1_op.is_jal  || io.idu_to_exu1.dec1_op.is_jalr) -> aluOut_jal
  ))

  // ---------- 分支条件 ----------
  val beq_cond  = rs1 === rs2
  val bne_cond  = rs1 =/= rs2
  val blt_cond  = rs1.asSInt < rs2.asSInt
  val bge_cond  = rs1.asSInt >= rs2.asSInt
  val bltu_cond = rs1 < rs2
  val bgeu_cond = rs1 >= rs2

  val branch_cond = MuxCase(false.B, Seq(
    io.idu_to_exu1.dec1_op.is_beq  -> beq_cond,
    io.idu_to_exu1.dec1_op.is_bne  -> bne_cond,
    io.idu_to_exu1.dec1_op.is_blt  -> blt_cond,
    io.idu_to_exu1.dec1_op.is_bge  -> bge_cond,
    io.idu_to_exu1.dec1_op.is_bltu -> bltu_cond,
    io.idu_to_exu1.dec1_op.is_bgeu -> bgeu_cond
  ))

  val is_jump   = io.idu_to_exu1.dec1_op.is_jal || io.idu_to_exu1.dec1_op.is_jalr
  val is_branch = io.idu_to_exu1.dec1_op.is_beq || io.idu_to_exu1.dec1_op.is_bne ||
                  io.idu_to_exu1.dec1_op.is_blt || io.idu_to_exu1.dec1_op.is_bge ||
                  io.idu_to_exu1.dec1_op.is_bltu || io.idu_to_exu1.dec1_op.is_bgeu
  val need_branch = is_jump || (is_branch && branch_cond)

  // 跳转目标
  val target_jal   = pc + imm
  val target_jalr  = (rs1 + imm) & ~1.U(32.W)
  val target_branch = pc + imm
  val branch_target_final = MuxCase(0.U, Seq(
    io.idu_to_exu1.dec1_op.is_jal  -> target_jal,
    io.idu_to_exu1.dec1_op.is_jalr -> target_jalr,
    is_branch                      -> target_branch
  ))

  io.exu_to_ifu.take_branch   := need_branch
  io.exu_to_ifu.branch_target := branch_target_final

  // 访存接口
  val mem_addr = rs1 + imm
  io.exu_to_lsu.op.is_lb   := io.idu_to_exu1.dec1_op.is_lb
  io.exu_to_lsu.op.is_lh   := io.idu_to_exu1.dec1_op.is_lh
  io.exu_to_lsu.op.is_lw   := io.idu_to_exu1.dec1_op.is_lw
  io.exu_to_lsu.op.is_lbu  := io.idu_to_exu1.dec1_op.is_lbu
  io.exu_to_lsu.op.is_lhu  := io.idu_to_exu1.dec1_op.is_lhu
  io.exu_to_lsu.op.is_sb   := io.idu_to_exu1.dec1_op.is_sb
  io.exu_to_lsu.op.is_sh   := io.idu_to_exu1.dec1_op.is_sh
  io.exu_to_lsu.op.is_sw   := io.idu_to_exu1.dec1_op.is_sw
  io.exu_to_lsu.op.is_ebreak := io.idu_to_exu1.dec1_op.is_ebreak
  io.exu_to_lsu.paddr.addr := mem_addr
  io.exu_to_lsu.data.store_data := rs2

  // 写回控制
  val need_wb = (io.idu_to_exu1.dec1_op.is_lui    ||
                 io.idu_to_exu1.dec1_op.is_auipc  ||
                 io.idu_to_exu1.dec1_op.is_jal    ||
                 io.idu_to_exu1.dec1_op.is_jalr   ||
                 io.idu_to_exu1.dec1_op.is_addi   ||
                 io.idu_to_exu1.dec1_op.is_slti   ||
                 io.idu_to_exu1.dec1_op.is_sltiu  ||
                 io.idu_to_exu1.dec1_op.is_xori   ||
                 io.idu_to_exu1.dec1_op.is_ori    ||
                 io.idu_to_exu1.dec1_op.is_andi   ||
                 io.idu_to_exu1.dec1_op.is_slli   ||
                 io.idu_to_exu1.dec1_op.is_srli   ||
                 io.idu_to_exu1.dec1_op.is_srai   ||
                 io.idu_to_exu1.dec1_op.is_add    ||
                 io.idu_to_exu1.dec1_op.is_sub    ||
                 io.idu_to_exu1.dec1_op.is_sll    ||
                 io.idu_to_exu1.dec1_op.is_slt    ||
                 io.idu_to_exu1.dec1_op.is_sltu   ||
                 io.idu_to_exu1.dec1_op.is_xor    ||
                 io.idu_to_exu1.dec1_op.is_srl    ||
                 io.idu_to_exu1.dec1_op.is_sra    ||
                 io.idu_to_exu1.dec1_op.is_or     ||
                 io.idu_to_exu1.dec1_op.is_and) &&
                !io.idu_to_exu1.dec1_op.is_ebreak

  val is_load = io.idu_to_exu1.dec1_op.is_lb  || io.idu_to_exu1.dec1_op.is_lh  ||
                io.idu_to_exu1.dec1_op.is_lw  || io.idu_to_exu1.dec1_op.is_lbu ||
                io.idu_to_exu1.dec1_op.is_lhu

  val rd_final = Mux(need_wb || is_load, io.idu_to_exu1.dec1_rd, 0.U(5.W))
  val wb_data  = Mux(is_load, 0.U(32.W), alu_out)

  io.exu_to_lsu.exu_through_lsu_to_wbu.rd := rd_final
  io.exu_to_lsu.exu_through_lsu_to_wbu.grf_wb_data := wb_data

  // debug 接线
  io.debug_alu_out      := alu_out
  io.debug_alu_source1  := MuxCase(rs1, Seq(
    io.idu_to_exu1.dec1_op.is_auipc -> pc,
    io.idu_to_exu1.dec1_op.is_jal   -> pc,
    io.idu_to_exu1.dec1_op.is_lui   -> 0.U(32.W)
  ))
  io.debug_alu_source2  := Mux(io.idu_to_exu1.dec1_op.is_addi ||
                              io.idu_to_exu1.dec1_op.is_slti ||
                              io.idu_to_exu1.dec1_op.is_sltiu ||
                              io.idu_to_exu1.dec1_op.is_xori ||
                              io.idu_to_exu1.dec1_op.is_ori ||
                              io.idu_to_exu1.dec1_op.is_andi ||
                              io.idu_to_exu1.dec1_op.is_slli ||
                              io.idu_to_exu1.dec1_op.is_srli ||
                              io.idu_to_exu1.dec1_op.is_srai ||
                              io.idu_to_exu1.dec1_op.is_jalr ||
                              io.idu_to_exu1.dec1_op.is_lb ||
                              io.idu_to_exu1.dec1_op.is_lh ||
                              io.idu_to_exu1.dec1_op.is_lw ||
                              io.idu_to_exu1.dec1_op.is_lbu ||
                              io.idu_to_exu1.dec1_op.is_lhu ||
                              io.idu_to_exu1.dec1_op.is_sb ||
                              io.idu_to_exu1.dec1_op.is_sh ||
                              io.idu_to_exu1.dec1_op.is_sw,
                              imm, rs2)
  io.debug_agu_out      := mem_addr
}
package R1322IAeCSR

import chisel3._
import chisel3.util._

// 执行单元，负责 ALU/分支/访存，并内嵌 CSR 控制逻辑（CSR 模块以独立文件存在）
class EXU extends Module {
  val io = IO(new Bundle {
    /* 原端口保留
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
        val is_csrrw  = Input(Bool())
        val is_csrrs  = Input(Bool())
        val is_csrrc  = Input(Bool())
        val is_csrrwi = Input(Bool())
        val is_csrrsi = Input(Bool())
        val is_csrrci = Input(Bool())
        val is_ecall  = Input(Bool())
        val is_mret   = Input(Bool())
      }
      val dec1_imm = Input(UInt(32.W))
      val dec1_val = new Bundle {
        val rs1_val = Input(UInt(32.W))
        val rs2_val = Input(UInt(32.W))
        val nextpc  = Input(UInt(32.W))
      }
      val dec1_rd   = Input(UInt(5.W))
      val is_stall  = Input(Bool())
      val inst1_pc  = Input(UInt(32.W))
      val inst1     = Input(UInt(32.W))
    }
    原端口保留结束 */

    //====================================新增: D/E 单lane总线端口====================================
    val idu_to_exu1 = Flipped(Decoupled(new IDU_to_EXU_Lane_Message))
    //====================================新增: D/E 单lane总线端口====================================


    /* 原端口保留
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
    原端口保留结束 */

    //====================================新增: E/M 单lane总线端口====================================
    val exu_to_lsu = Decoupled(new EXU_to_LSU_Lane_Message)
    //====================================新增: E/M 单lane总线端口====================================

    val exu_to_ifu = new Bundle {
      val take_branch   = Output(Bool())
      val branch_target = Output(UInt(32.W))
    }

    // CSR → IFU
    val csr_to_ifu = new Bundle {
      val take_trap = Output(Bool())
      val trap_pc   = Output(UInt(32.W))
      val take_mret = Output(Bool())
      val mret_pc   = Output(UInt(32.W))
    }

    // CSR 写回 → GRF
    val csr_to_grf = new Bundle {
      val wen   = Output(Bool())
      val waddr = Output(UInt(5.W))
      val wdata = Output(UInt(32.W))
    }

    // CSR debug
    val debug_csr = new Bundle {
      val mcycle    = Output(UInt(64.W))
      val minstret  = Output(UInt(64.W))
      val mstatus   = Output(UInt(32.W))
      val mie       = Output(UInt(32.W))
      val mtvec     = Output(UInt(32.W))
      val mepc      = Output(UInt(32.W))
      val mcause    = Output(UInt(32.W))
      val mtval     = Output(UInt(32.W))
      val mip       = Output(UInt(32.W))
      val mscratch  = Output(UInt(32.W))
      val mvendorid = Output(UInt(32.W))
      val marchid   = Output(UInt(32.W))
      val mimpid    = Output(UInt(32.W))
      val mhartid   = Output(UInt(32.W))
    }

    val debug_alu_out = Output(UInt(32.W))
    val debug_alu_source1 = Output(UInt(32.W))
    val debug_alu_source2 = Output(UInt(32.W))
    val debug_agu_out = Output(UInt(32.W))
  })

  //====================================新增: D/E 单lane ready====================================
  io.idu_to_exu1.ready := true.B
  //====================================新增: D/E 单lane ready====================================

  /* 原 valid 保留
  io.exu_to_lsu.valid := true.B
  原 valid 保留结束 */
  //====================================新增: E/M 单lane valid 随输入====================================
  io.exu_to_lsu.valid := io.idu_to_exu1.valid
  //====================================新增: E/M 单lane valid 随输入====================================

  // ============== CSR 实例化与控制逻辑 ==============
  val csr = Module(new CSR)

  val csr_addr = io.idu_to_exu1.bits.inst1(31, 20)
  csr.io.exu_to_csr.addr := csr_addr

  val is_csr_write = io.idu_to_exu1.bits.op.is_csrrw || io.idu_to_exu1.bits.op.is_csrrs ||
                     io.idu_to_exu1.bits.op.is_csrrc || io.idu_to_exu1.bits.op.is_csrrwi ||
                     io.idu_to_exu1.bits.op.is_csrrsi || io.idu_to_exu1.bits.op.is_csrrci

  csr.io.exu_to_csr.op := MuxCase(0.U(3.W), Seq(
    (io.idu_to_exu1.bits.op.is_csrrs || io.idu_to_exu1.bits.op.is_csrrsi) -> 1.U,
    (io.idu_to_exu1.bits.op.is_csrrc || io.idu_to_exu1.bits.op.is_csrrci) -> 2.U
  ))

  val use_imm_csr = io.idu_to_exu1.bits.op.is_csrrwi || io.idu_to_exu1.bits.op.is_csrrsi || io.idu_to_exu1.bits.op.is_csrrci
  val uimm = io.idu_to_exu1.bits.inst1(19, 15)
  val rs1_csr_nonzero = io.idu_to_exu1.bits.inst1(19, 15) =/= 0.U
  val uimm_nonzero = uimm =/= 0.U

  csr.io.exu_to_csr.use_imm := use_imm_csr
  csr.io.exu_to_csr.rs1_val := io.idu_to_exu1.bits.rs1_val

  val csrrw_or_wi = io.idu_to_exu1.bits.op.is_csrrw || io.idu_to_exu1.bits.op.is_csrrwi
  val csr_write_cond = Mux(csrrw_or_wi, true.B,
                       Mux(use_imm_csr, uimm_nonzero, rs1_csr_nonzero))
  csr.io.exu_to_csr.wen := is_csr_write && csr_write_cond
  csr.io.exu_to_csr.waddr := csr_addr
  csr.io.exu_to_csr.wdata := Mux(use_imm_csr, Cat(0.U(27.W), uimm), io.idu_to_exu1.bits.rs1_val)

  csr.io.exu_to_csr.ecall      := io.idu_to_exu1.bits.op.is_ecall
  csr.io.exu_to_csr.mret       := io.idu_to_exu1.bits.op.is_mret
  csr.io.exu_to_csr.is_ebreak  := io.idu_to_exu1.bits.op.is_ebreak
  csr.io.exu_to_csr.current_pc := io.idu_to_exu1.bits.inst1_pc

  val inst_ok = !io.idu_to_exu1.bits.op.is_ebreak && !io.idu_to_exu1.bits.op.is_ecall
  csr.io.exu_to_csr.inst_retire := Mux(inst_ok, Mux(io.idu_to_exu1.bits.is_stall, 1.U(2.W), 2.U(2.W)), 0.U(2.W))

  // CSR → IFU
  io.csr_to_ifu.take_trap := csr.io.csr_to_exu.take_trap
  io.csr_to_ifu.trap_pc   := csr.io.csr_to_exu.trap_pc
  io.csr_to_ifu.take_mret := io.idu_to_exu1.bits.op.is_mret
  io.csr_to_ifu.mret_pc   := csr.io.csr_to_exu.debug_mepc

  // CSR 写回 → GRF
  val is_csr = io.idu_to_exu1.bits.op.is_csrrw || io.idu_to_exu1.bits.op.is_csrrs ||
               io.idu_to_exu1.bits.op.is_csrrc || io.idu_to_exu1.bits.op.is_csrrwi ||
               io.idu_to_exu1.bits.op.is_csrrsi || io.idu_to_exu1.bits.op.is_csrrci
  val csr_rd = io.idu_to_exu1.bits.inst1(11, 7)
  io.csr_to_grf.wen   := is_csr && (csr_rd =/= 0.U)
  io.csr_to_grf.waddr := csr_rd
  io.csr_to_grf.wdata := csr.io.csr_to_exu.rdata

  // CSR debug
  io.debug_csr.mcycle    := csr.io.csr_to_exu.debug_mcycle
  io.debug_csr.minstret  := csr.io.csr_to_exu.debug_minstret
  io.debug_csr.mstatus   := csr.io.csr_to_exu.debug_mstatus
  io.debug_csr.mie       := csr.io.csr_to_exu.debug_mie
  io.debug_csr.mtvec     := csr.io.csr_to_exu.debug_mtvec
  io.debug_csr.mepc      := csr.io.csr_to_exu.debug_mepc
  io.debug_csr.mcause    := csr.io.csr_to_exu.debug_mcause
  io.debug_csr.mtval     := csr.io.csr_to_exu.debug_mtval
  io.debug_csr.mip       := csr.io.csr_to_exu.debug_mip
  io.debug_csr.mscratch  := csr.io.csr_to_exu.debug_mscratch
  io.debug_csr.mvendorid := csr.io.csr_to_exu.debug_mvendorid
  io.debug_csr.marchid   := csr.io.csr_to_exu.debug_marchid
  io.debug_csr.mimpid    := csr.io.csr_to_exu.debug_mimpid
  io.debug_csr.mhartid   := csr.io.csr_to_exu.debug_mhartid

  // ============== EXU 原有逻辑（不变） ==============
  val pc   = io.idu_to_exu1.bits.nextpc - 4.U
  val rs1  = io.idu_to_exu1.bits.rs1_val
  val rs2  = io.idu_to_exu1.bits.rs2_val
  val imm  = io.idu_to_exu1.bits.imm

  val is_shift_i = io.idu_to_exu1.bits.op.is_slli ||
                   io.idu_to_exu1.bits.op.is_srli ||
                   io.idu_to_exu1.bits.op.is_srai
  val shamt = Mux(is_shift_i, imm(4,0), rs2(4,0))

  val aluOut_lui    = imm
  val aluOut_auipc  = pc + imm
  val aluOut_add    = rs1 + Mux(io.idu_to_exu1.bits.op.is_addi, imm, rs2)
  val aluOut_sub    = rs1 - rs2
  val aluOut_slt    = Mux(rs1.asSInt < Mux(io.idu_to_exu1.bits.op.is_slti, imm.asSInt, rs2.asSInt), 1.U, 0.U)
  val aluOut_sltu   = Mux(rs1 < Mux(io.idu_to_exu1.bits.op.is_sltiu, imm, rs2), 1.U, 0.U)
  val aluOut_xor    = rs1 ^ Mux(io.idu_to_exu1.bits.op.is_xori, imm, rs2)
  val aluOut_or     = rs1 | Mux(io.idu_to_exu1.bits.op.is_ori, imm, rs2)
  val aluOut_and    = rs1 & Mux(io.idu_to_exu1.bits.op.is_andi, imm, rs2)
  val aluOut_sll    = rs1 << shamt
  val aluOut_srl    = rs1 >> shamt
  val aluOut_sra    = (rs1.asSInt >> shamt).asUInt
  val aluOut_jal    = pc + 4.U

  val alu_out = MuxCase(0.U, Seq(
    io.idu_to_exu1.bits.op.is_lui    -> aluOut_lui,
    io.idu_to_exu1.bits.op.is_auipc  -> aluOut_auipc,
    (io.idu_to_exu1.bits.op.is_addi || io.idu_to_exu1.bits.op.is_add) -> aluOut_add,
    io.idu_to_exu1.bits.op.is_sub    -> aluOut_sub,
    (io.idu_to_exu1.bits.op.is_slti || io.idu_to_exu1.bits.op.is_slt) -> aluOut_slt,
    (io.idu_to_exu1.bits.op.is_sltiu || io.idu_to_exu1.bits.op.is_sltu) -> aluOut_sltu,
    (io.idu_to_exu1.bits.op.is_xori || io.idu_to_exu1.bits.op.is_xor) -> aluOut_xor,
    (io.idu_to_exu1.bits.op.is_ori  || io.idu_to_exu1.bits.op.is_or)  -> aluOut_or,
    (io.idu_to_exu1.bits.op.is_andi || io.idu_to_exu1.bits.op.is_and) -> aluOut_and,
    (io.idu_to_exu1.bits.op.is_slli || io.idu_to_exu1.bits.op.is_sll) -> aluOut_sll,
    (io.idu_to_exu1.bits.op.is_srli || io.idu_to_exu1.bits.op.is_srl) -> aluOut_srl,
    (io.idu_to_exu1.bits.op.is_srai || io.idu_to_exu1.bits.op.is_sra) -> aluOut_sra,
    (io.idu_to_exu1.bits.op.is_jal  || io.idu_to_exu1.bits.op.is_jalr) -> aluOut_jal
  ))

  val beq_cond  = rs1 === rs2
  val bne_cond  = rs1 =/= rs2
  val blt_cond  = rs1.asSInt < rs2.asSInt
  val bge_cond  = rs1.asSInt >= rs2.asSInt
  val bltu_cond = rs1 < rs2
  val bgeu_cond = rs1 >= rs2

  val branch_cond = MuxCase(false.B, Seq(
    io.idu_to_exu1.bits.op.is_beq  -> beq_cond,
    io.idu_to_exu1.bits.op.is_bne  -> bne_cond,
    io.idu_to_exu1.bits.op.is_blt  -> blt_cond,
    io.idu_to_exu1.bits.op.is_bge  -> bge_cond,
    io.idu_to_exu1.bits.op.is_bltu -> bltu_cond,
    io.idu_to_exu1.bits.op.is_bgeu -> bgeu_cond
  ))

  val is_jump   = io.idu_to_exu1.bits.op.is_jal || io.idu_to_exu1.bits.op.is_jalr
  val is_branch = io.idu_to_exu1.bits.op.is_beq || io.idu_to_exu1.bits.op.is_bne ||
                  io.idu_to_exu1.bits.op.is_blt || io.idu_to_exu1.bits.op.is_bge ||
                  io.idu_to_exu1.bits.op.is_bltu || io.idu_to_exu1.bits.op.is_bgeu
  val need_branch = is_jump || (is_branch && branch_cond)

  val target_jal   = pc + imm
  val target_jalr  = (rs1 + imm) & ~1.U(32.W)
  val target_branch = pc + imm
  val branch_target_final = MuxCase(0.U, Seq(
    io.idu_to_exu1.bits.op.is_jal  -> target_jal,
    io.idu_to_exu1.bits.op.is_jalr -> target_jalr,
    is_branch                      -> target_branch
  ))

  io.exu_to_ifu.take_branch   := need_branch
  io.exu_to_ifu.branch_target := branch_target_final

  val mem_addr = rs1 + imm
  io.exu_to_lsu.bits.op.is_lb   := io.idu_to_exu1.bits.op.is_lb
  io.exu_to_lsu.bits.op.is_lh   := io.idu_to_exu1.bits.op.is_lh
  io.exu_to_lsu.bits.op.is_lw   := io.idu_to_exu1.bits.op.is_lw
  io.exu_to_lsu.bits.op.is_lbu  := io.idu_to_exu1.bits.op.is_lbu
  io.exu_to_lsu.bits.op.is_lhu  := io.idu_to_exu1.bits.op.is_lhu
  io.exu_to_lsu.bits.op.is_sb   := io.idu_to_exu1.bits.op.is_sb
  io.exu_to_lsu.bits.op.is_sh   := io.idu_to_exu1.bits.op.is_sh
  io.exu_to_lsu.bits.op.is_sw   := io.idu_to_exu1.bits.op.is_sw
  io.exu_to_lsu.bits.op.is_ebreak := io.idu_to_exu1.bits.op.is_ebreak
  io.exu_to_lsu.bits.addr := mem_addr
  io.exu_to_lsu.bits.store_data := rs2

  val need_wb = (io.idu_to_exu1.bits.op.is_lui    ||
                 io.idu_to_exu1.bits.op.is_auipc  ||
                 io.idu_to_exu1.bits.op.is_jal    ||
                 io.idu_to_exu1.bits.op.is_jalr   ||
                 io.idu_to_exu1.bits.op.is_addi   ||
                 io.idu_to_exu1.bits.op.is_slti   ||
                 io.idu_to_exu1.bits.op.is_sltiu  ||
                 io.idu_to_exu1.bits.op.is_xori   ||
                 io.idu_to_exu1.bits.op.is_ori    ||
                 io.idu_to_exu1.bits.op.is_andi   ||
                 io.idu_to_exu1.bits.op.is_slli   ||
                 io.idu_to_exu1.bits.op.is_srli   ||
                 io.idu_to_exu1.bits.op.is_srai   ||
                 io.idu_to_exu1.bits.op.is_add    ||
                 io.idu_to_exu1.bits.op.is_sub    ||
                 io.idu_to_exu1.bits.op.is_sll    ||
                 io.idu_to_exu1.bits.op.is_slt    ||
                 io.idu_to_exu1.bits.op.is_sltu   ||
                 io.idu_to_exu1.bits.op.is_xor    ||
                 io.idu_to_exu1.bits.op.is_srl    ||
                 io.idu_to_exu1.bits.op.is_sra    ||
                 io.idu_to_exu1.bits.op.is_or     ||
                 io.idu_to_exu1.bits.op.is_and) &&
                !io.idu_to_exu1.bits.op.is_ebreak

  val is_load = io.idu_to_exu1.bits.op.is_lb  || io.idu_to_exu1.bits.op.is_lh  ||
                io.idu_to_exu1.bits.op.is_lw  || io.idu_to_exu1.bits.op.is_lbu ||
                io.idu_to_exu1.bits.op.is_lhu

  val rd_final = Mux(need_wb || is_load, io.idu_to_exu1.bits.rd, 0.U(5.W))
  val wb_data  = Mux(is_load, 0.U(32.W), alu_out)

  io.exu_to_lsu.bits.rd := rd_final
  io.exu_to_lsu.bits.wb_data := wb_data

  // debug
  io.debug_alu_out      := alu_out
  io.debug_alu_source1  := MuxCase(rs1, Seq(
    io.idu_to_exu1.bits.op.is_auipc -> pc,
    io.idu_to_exu1.bits.op.is_jal   -> pc,
    io.idu_to_exu1.bits.op.is_lui   -> 0.U(32.W)
  ))
  io.debug_alu_source2  := Mux(io.idu_to_exu1.bits.op.is_addi ||
                              io.idu_to_exu1.bits.op.is_slti ||
                              io.idu_to_exu1.bits.op.is_sltiu ||
                              io.idu_to_exu1.bits.op.is_xori ||
                              io.idu_to_exu1.bits.op.is_ori ||
                              io.idu_to_exu1.bits.op.is_andi ||
                              io.idu_to_exu1.bits.op.is_slli ||
                              io.idu_to_exu1.bits.op.is_srli ||
                              io.idu_to_exu1.bits.op.is_srai ||
                              io.idu_to_exu1.bits.op.is_jalr ||
                              io.idu_to_exu1.bits.op.is_lb ||
                              io.idu_to_exu1.bits.op.is_lh ||
                              io.idu_to_exu1.bits.op.is_lw ||
                              io.idu_to_exu1.bits.op.is_lbu ||
                              io.idu_to_exu1.bits.op.is_lhu ||
                              io.idu_to_exu1.bits.op.is_sb ||
                              io.idu_to_exu1.bits.op.is_sh ||
                              io.idu_to_exu1.bits.op.is_sw,
                              imm, rs2)
  io.debug_agu_out      := mem_addr
}
package R1322IAeCSR

import chisel3._
import chisel3.util._

// 执行单元: ALU / 分支比较 / 访存地址生成, 并内嵌 CSR 控制逻辑。
class EXU extends Module {
  val io = IO(new Bundle {
    val idu_to_exu1 = Flipped(Decoupled(new IDU_to_EXU_Lane_Message))

    val exu_to_lsu = Decoupled(new EXU_to_LSU_Lane_Message)

    val exu_to_ifu = new Bundle {
      val take_branch   = Output(Bool())
      val branch_target = Output(UInt(32.W))
    }

    // CSR -> IFU
    val csr_to_ifu = new Bundle {
      val take_trap = Output(Bool())
      val trap_pc   = Output(UInt(32.W))
      val take_mret = Output(Bool())
      val mret_pc   = Output(UInt(32.W))
    }

    // CSR 写回 -> GRF
    val csr_to_grf = new Bundle {
      val wen   = Output(Bool())
      val waddr = Output(UInt(5.W))
      val wdata = Output(UInt(32.W))
    }

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

    val debug_alu_out     = Output(UInt(32.W))
    val debug_alu_source1 = Output(UInt(32.W))
    val debug_alu_source2 = Output(UInt(32.W))
    val debug_agu_out     = Output(UInt(32.W))
  })

  io.idu_to_exu1.ready := true.B
  io.exu_to_lsu.valid  := io.idu_to_exu1.valid

  // ---------------- 操作数 ----------------
  val c    = io.idu_to_exu1.bits.ctrl
  val inst = io.idu_to_exu1.bits.inst
  val pc   = io.idu_to_exu1.bits.pc
  val rs1  = io.idu_to_exu1.bits.rs1_val
  val rs2  = io.idu_to_exu1.bits.rs2_val
  val imm  = io.idu_to_exu1.bits.imm

  // ================= CSR =================
  val csr = Module(new CSR)

  val csr_addr = inst(31, 20)
  csr.io.exu_to_csr.addr := csr_addr
  csr.io.exu_to_csr.op := MuxCase(0.U(3.W), Seq(
    (c.is_csrrs || c.is_csrrsi) -> 1.U,
    (c.is_csrrc || c.is_csrrci) -> 2.U
  ))

  val use_imm_csr = c.is_csrrwi || c.is_csrrsi || c.is_csrrci
  val uimm        = inst(19, 15)
  val csrrw_or_wi = c.is_csrrw || c.is_csrrwi

  csr.io.exu_to_csr.use_imm := use_imm_csr
  csr.io.exu_to_csr.rs1_val := rs1
  csr.io.exu_to_csr.wen     := c.is_csr &&
    Mux(csrrw_or_wi, true.B, Mux(use_imm_csr, uimm =/= 0.U, inst(19, 15) =/= 0.U))
  csr.io.exu_to_csr.waddr   := csr_addr
  csr.io.exu_to_csr.wdata   := Mux(use_imm_csr, Cat(0.U(27.W), uimm), rs1)

  csr.io.exu_to_csr.ecall      := c.is_ecall
  csr.io.exu_to_csr.mret       := c.is_mret
  csr.io.exu_to_csr.is_ebreak  := c.is_ebreak
  csr.io.exu_to_csr.current_pc := pc

  val inst_ok = !c.is_ebreak && !c.is_ecall
  csr.io.exu_to_csr.inst_retire :=
    Mux(inst_ok, Mux(io.idu_to_exu1.bits.is_stall, 1.U(2.W), 2.U(2.W)), 0.U(2.W))

  io.csr_to_ifu.take_trap := csr.io.csr_to_exu.take_trap
  io.csr_to_ifu.trap_pc   := csr.io.csr_to_exu.trap_pc
  io.csr_to_ifu.take_mret := c.is_mret
  io.csr_to_ifu.mret_pc   := csr.io.csr_to_exu.debug_mepc

  val csr_rd = inst(11, 7)
  io.csr_to_grf.wen   := c.is_csr && (csr_rd =/= 0.U)
  io.csr_to_grf.waddr := csr_rd
  io.csr_to_grf.wdata := csr.io.csr_to_exu.rdata

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

  // ================= ALU =================
  val is_shift_i = c.is_slli || c.is_srli || c.is_srai
  val shamt = Mux(is_shift_i, imm(4, 0), rs2(4, 0))
  // 立即数型指令统一用 imm 作第二操作数, 寄存器型用 rs2 (省去 5 个 Mux)
  val op2 = Mux(c.is_imm_op, imm, rs2)

  val alu_out = MuxCase(0.U, Seq(
    c.is_lui                     -> imm,
    c.is_auipc                   -> (pc + imm),
    (c.is_addi || c.is_add)      -> (rs1 + op2),
    c.is_sub                     -> (rs1 - rs2),
    (c.is_slti || c.is_slt)      -> Mux(rs1.asSInt < op2.asSInt, 1.U, 0.U),
    (c.is_sltiu || c.is_sltu)    -> Mux(rs1 < op2, 1.U, 0.U),
    (c.is_xori || c.is_xor)      -> (rs1 ^ op2),
    (c.is_ori || c.is_or)        -> (rs1 | op2),
    (c.is_andi || c.is_and)      -> (rs1 & op2),
    (c.is_slli || c.is_sll)      -> (rs1 << shamt),
    (c.is_srli || c.is_srl)      -> (rs1 >> shamt),
    (c.is_srai || c.is_sra)      -> (rs1.asSInt >> shamt).asUInt,
    (c.is_jal || c.is_jalr)      -> (pc + 4.U)
  ))

  // ---------------- 分支 ----------------
  // 复用 3 个比较器: eq / lt(有符号) / ltu(无符号), 取反派生其余
  val cmp_eq  = rs1 === rs2
  val cmp_lt  = rs1.asSInt < rs2.asSInt
  val cmp_ltu = rs1 < rs2

  val branch_cond = MuxCase(false.B, Seq(
    c.is_beq  -> cmp_eq,
    c.is_bne  -> !cmp_eq,
    c.is_blt  -> cmp_lt,
    c.is_bge  -> !cmp_lt,
    c.is_bltu -> cmp_ltu,
    c.is_bgeu -> !cmp_ltu
  ))

  val need_branch = c.is_jump || (c.is_branch && branch_cond)

  val branch_target_final = MuxCase(0.U, Seq(
    c.is_jal  -> (pc + imm),
    c.is_jalr -> ((rs1 + imm) & ~1.U(32.W)),
    c.is_branch -> (pc + imm)
  ))

  io.exu_to_ifu.take_branch   := need_branch
  io.exu_to_ifu.branch_target := branch_target_final

  // ---------------- 访存 ----------------
  val mem_addr = rs1 + imm
  io.exu_to_lsu.bits.op.is_lb     := c.is_lb
  io.exu_to_lsu.bits.op.is_lh     := c.is_lh
  io.exu_to_lsu.bits.op.is_lw     := c.is_lw
  io.exu_to_lsu.bits.op.is_lbu    := c.is_lbu
  io.exu_to_lsu.bits.op.is_lhu    := c.is_lhu
  io.exu_to_lsu.bits.op.is_sb     := c.is_sb
  io.exu_to_lsu.bits.op.is_sh     := c.is_sh
  io.exu_to_lsu.bits.op.is_sw     := c.is_sw
  io.exu_to_lsu.bits.op.is_ebreak := c.is_ebreak
  io.exu_to_lsu.bits.addr         := mem_addr
  io.exu_to_lsu.bits.store_data   := rs2

  // ---------------- 写回选择 ----------------
  val writes_int = c.is_alu_op || c.is_jump
  val rd_final   = Mux(writes_int || c.is_load, io.idu_to_exu1.bits.rd, 0.U(5.W))
  // load 的写回数据由 LSU 用读回值覆盖, 这里直接给 ALU 结果 (省一级 Mux)
  val wb_data    = alu_out

  io.exu_to_lsu.bits.rd      := rd_final
  io.exu_to_lsu.bits.wb_data := wb_data

  // ---------------- 调试 ----------------
  io.debug_alu_out     := alu_out
  io.debug_alu_source1 := MuxCase(rs1, Seq(
    c.is_auipc -> pc,
    c.is_jal   -> pc,
    c.is_lui   -> 0.U(32.W)
  ))
  io.debug_alu_source2 := Mux(c.is_imm_op || c.is_jalr || c.is_load || c.is_store, imm, rs2)
  io.debug_agu_out     := mem_addr
}

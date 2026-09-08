package R1322IAeCSRMC

import chisel3._
import chisel3.util._

// 多周期 EXU: IDU 的 slave, LSU 的 master
// 内部包含两条执行 lane(lane2 只做运算, 控制类指令恒为单发射,
// 因此 CSR/分支/异常只存在于 lane1).
// 指令在寄存器中暂存一个周期, 计算出运算结果与访存请求后发给 LSU;
// 所有一次性副作用(CSR写/重定向/指令退休计数)都在 commit(= out.fire,
// 消息被 LSU 接收)那一周期生效, 保证每条指令的副作用恰好发生一次.
class EXU extends Module {
  val io = IO(new Bundle {
    // 总线输入: 译码结果 <- IDU
    val in = Flipped(Decoupled(new IDMsg))

    // 总线输出: 执行结果(运算结果 + 访存请求) -> LSU
    val out = Decoupled(new EXMsg)

    // 分支重定向 -> IFU
    val exu_to_ifu = new Bundle {
      val take_branch   = Output(Bool())
      val branch_target = Output(UInt(32.W))
    }

    // CSR 异常/mret 重定向 -> IFU
    val csr_to_ifu = new Bundle {
      val take_trap = Output(Bool())
      val trap_pc   = Output(UInt(32.W))
      val take_mret = Output(Bool())
      val mret_pc   = Output(UInt(32.W))
    }

    // CSR 写回 -> GRF (CSR指令的旧CSR值写回rd)
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

    // EXU debug
    val debug_exu1 = new Bundle {
      val alu_out     = Output(UInt(32.W))
      val alu_source1 = Output(UInt(32.W))
      val alu_source2 = Output(UInt(32.W))
      val agu_out     = Output(UInt(32.W))
    }
    val debug_exu2 = new Bundle {
      val alu_out     = Output(UInt(32.W))
      val alu_source1 = Output(UInt(32.W))
      val alu_source2 = Output(UInt(32.W))
      val agu_out     = Output(UInt(32.W))
    }
  })

  // ---------- 级寄存器: 暂存译码结果 ----------
  val msg  = RegEnable(io.in.bits, io.in.fire)
  val busy = RegInit(false.B)
  io.in.ready  := !busy
  io.out.valid := busy
  when (io.in.fire)  { busy := true.B  }
  when (io.out.fire) { busy := false.B }

  // 提交信号: 消息被 LSU 接收, 本包指令的副作用在此周期生效
  val commit = io.out.fire

  val d1 = msg.dec1_op
  val d2 = msg.dec2_op

  // ============== CSR 实例化与控制逻辑(仅 lane1) ==============
  val csr = Module(new CSR)

  val csr_addr = msg.inst1(31, 20)
  csr.io.exu_to_csr.addr := csr_addr

  val is_csr_write = d1.is_csrrw || d1.is_csrrs || d1.is_csrrc ||
                     d1.is_csrrwi || d1.is_csrrsi || d1.is_csrrci

  csr.io.exu_to_csr.op := MuxCase(0.U(3.W), Seq(
    (d1.is_csrrs || d1.is_csrrsi) -> 1.U,
    (d1.is_csrrc || d1.is_csrrci) -> 2.U
  ))

  val use_imm_csr = d1.is_csrrwi || d1.is_csrrsi || d1.is_csrrci
  val uimm = msg.inst1(19, 15)
  val rs1_csr_nonzero = msg.inst1(19, 15) =/= 0.U
  val uimm_nonzero = uimm =/= 0.U

  csr.io.exu_to_csr.use_imm := use_imm_csr
  csr.io.exu_to_csr.rs1_val := msg.dec1_rs1_val

  val csrrw_or_wi = d1.is_csrrw || d1.is_csrrwi
  val csr_write_cond = Mux(csrrw_or_wi, true.B,
                       Mux(use_imm_csr, uimm_nonzero, rs1_csr_nonzero))
  // CSR 写只在提交周期发生一次
  csr.io.exu_to_csr.wen := is_csr_write && csr_write_cond && commit
  csr.io.exu_to_csr.waddr := csr_addr
  csr.io.exu_to_csr.wdata := Mux(use_imm_csr, Cat(0.U(27.W), uimm), msg.dec1_rs1_val)

  csr.io.exu_to_csr.ecall      := d1.is_ecall  && commit
  csr.io.exu_to_csr.mret       := d1.is_mret   && commit
  csr.io.exu_to_csr.is_ebreak  := d1.is_ebreak && commit
  csr.io.exu_to_csr.current_pc := msg.inst1_pc

  // 指令退休计数: 只在提交周期计数
  val inst_ok = !d1.is_ebreak && !d1.is_ecall
  csr.io.exu_to_csr.inst_retire :=
    Mux(commit, Mux(inst_ok, Mux(msg.is_stall, 1.U(2.W), 2.U(2.W)), 0.U(2.W)), 0.U(2.W))

  // CSR -> IFU 重定向(提交周期发出, IFU 暂存)
  io.csr_to_ifu.take_trap := csr.io.csr_to_exu.take_trap && commit
  io.csr_to_ifu.trap_pc   := csr.io.csr_to_exu.trap_pc
  io.csr_to_ifu.take_mret := d1.is_mret && commit
  io.csr_to_ifu.mret_pc   := csr.io.csr_to_exu.debug_mepc

  // CSR 写回 -> GRF (提交周期写一次)
  val is_csr = d1.is_csrrw || d1.is_csrrs || d1.is_csrrc ||
               d1.is_csrrwi || d1.is_csrrsi || d1.is_csrrci
  val csr_rd = msg.inst1(11, 7)
  io.csr_to_grf.wen   := is_csr && (csr_rd =/= 0.U) && commit
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

  // ============== Lane1: 第一条指令(ALU/分支) ==============
  val pc1   = msg.dec1_nextpc - 4.U
  val rs1_1 = msg.dec1_rs1_val
  val rs2_1 = msg.dec1_rs2_val
  val imm1  = msg.dec1_imm

  val is_shift_i1 = d1.is_slli || d1.is_srli || d1.is_srai
  val shamt1 = Mux(is_shift_i1, imm1(4,0), rs2_1(4,0))

  val alu1_lui   = imm1
  val alu1_auipc = pc1 + imm1
  val alu1_add   = rs1_1 + Mux(d1.is_addi, imm1, rs2_1)
  val alu1_sub   = rs1_1 - rs2_1
  val alu1_slt   = Mux(rs1_1.asSInt < Mux(d1.is_slti, imm1.asSInt, rs2_1.asSInt), 1.U, 0.U)
  val alu1_sltu  = Mux(rs1_1 < Mux(d1.is_sltiu, imm1, rs2_1), 1.U, 0.U)
  val alu1_xor   = rs1_1 ^ Mux(d1.is_xori, imm1, rs2_1)
  val alu1_or    = rs1_1 | Mux(d1.is_ori, imm1, rs2_1)
  val alu1_and   = rs1_1 & Mux(d1.is_andi, imm1, rs2_1)
  val alu1_sll   = rs1_1 << shamt1
  val alu1_srl   = rs1_1 >> shamt1
  val alu1_sra   = (rs1_1.asSInt >> shamt1).asUInt
  val alu1_jal   = pc1 + 4.U

  val alu1_out = MuxCase(0.U, Seq(
    d1.is_lui   -> alu1_lui,
    d1.is_auipc -> alu1_auipc,
    (d1.is_addi || d1.is_add) -> alu1_add,
    d1.is_sub   -> alu1_sub,
    (d1.is_slti || d1.is_slt) -> alu1_slt,
    (d1.is_sltiu || d1.is_sltu) -> alu1_sltu,
    (d1.is_xori || d1.is_xor) -> alu1_xor,
    (d1.is_ori  || d1.is_or)  -> alu1_or,
    (d1.is_andi || d1.is_and) -> alu1_and,
    (d1.is_slli || d1.is_sll) -> alu1_sll,
    (d1.is_srli || d1.is_srl) -> alu1_srl,
    (d1.is_srai || d1.is_sra) -> alu1_sra,
    (d1.is_jal  || d1.is_jalr) -> alu1_jal
  ))

  val beq_cond  = rs1_1 === rs2_1
  val bne_cond  = rs1_1 =/= rs2_1
  val blt_cond  = rs1_1.asSInt < rs2_1.asSInt
  val bge_cond  = rs1_1.asSInt >= rs2_1.asSInt
  val bltu_cond = rs1_1 < rs2_1
  val bgeu_cond = rs1_1 >= rs2_1

  val branch1_cond = MuxCase(false.B, Seq(
    d1.is_beq  -> beq_cond,
    d1.is_bne  -> bne_cond,
    d1.is_blt  -> blt_cond,
    d1.is_bge  -> bge_cond,
    d1.is_bltu -> bltu_cond,
    d1.is_bgeu -> bgeu_cond
  ))

  val is_jump1   = d1.is_jal || d1.is_jalr
  val is_branch1 = d1.is_beq || d1.is_bne || d1.is_blt || d1.is_bge ||
                   d1.is_bltu || d1.is_bgeu
  val need_branch1 = is_jump1 || (is_branch1 && branch1_cond)

  val target1_jal    = pc1 + imm1
  val target1_jalr   = (rs1_1 + imm1) & ~1.U(32.W)
  val target1_branch = pc1 + imm1
  val branch1_target = MuxCase(0.U, Seq(
    d1.is_jal  -> target1_jal,
    d1.is_jalr -> target1_jalr,
    is_branch1 -> target1_branch
  ))

  // 分支重定向只在提交周期发出
  io.exu_to_ifu.take_branch   := need_branch1 && commit
  io.exu_to_ifu.branch_target := branch1_target

  val mem1_addr = rs1_1 + imm1

  val need_wb1 = (d1.is_lui   || d1.is_auipc || d1.is_jal || d1.is_jalr ||
                  d1.is_addi  || d1.is_slti  || d1.is_sltiu || d1.is_xori ||
                  d1.is_ori   || d1.is_andi  || d1.is_slli  || d1.is_srli ||
                  d1.is_srai  || d1.is_add   || d1.is_sub   || d1.is_sll ||
                  d1.is_slt   || d1.is_sltu  || d1.is_xor   || d1.is_srl ||
                  d1.is_sra   || d1.is_or    || d1.is_and) && !d1.is_ebreak

  val is_load1 = d1.is_lb || d1.is_lh || d1.is_lw || d1.is_lbu || d1.is_lhu

  val rd_final1 = Mux(need_wb1 || is_load1, msg.dec1_rd, 0.U(5.W))
  val wb_data1  = Mux(is_load1, 0.U(32.W), alu1_out)

  // ============== Lane2: 第二条指令(仅运算) ==============
  val pc2   = msg.dec2_nextpc - 4.U
  val rs1_2 = msg.dec2_rs1_val
  val rs2_2 = msg.dec2_rs2_val
  val imm2  = msg.dec2_imm

  val is_shift_i2 = d2.is_slli || d2.is_srli || d2.is_srai
  val shamt2 = Mux(is_shift_i2, imm2(4,0), rs2_2(4,0))

  val alu2_lui   = imm2
  val alu2_auipc = pc2 + imm2
  val alu2_add   = rs1_2 + Mux(d2.is_addi, imm2, rs2_2)
  val alu2_sub   = rs1_2 - rs2_2
  val alu2_slt   = Mux(rs1_2.asSInt < Mux(d2.is_slti, imm2.asSInt, rs2_2.asSInt), 1.U, 0.U)
  val alu2_sltu  = Mux(rs1_2 < Mux(d2.is_sltiu, imm2, rs2_2), 1.U, 0.U)
  val alu2_xor   = rs1_2 ^ Mux(d2.is_xori, imm2, rs2_2)
  val alu2_or    = rs1_2 | Mux(d2.is_ori, imm2, rs2_2)
  val alu2_and   = rs1_2 & Mux(d2.is_andi, imm2, rs2_2)
  val alu2_sll   = rs1_2 << shamt2
  val alu2_srl   = rs1_2 >> shamt2
  val alu2_sra   = (rs1_2.asSInt >> shamt2).asUInt
  val alu2_jal   = pc2 + 4.U

  val alu2_out = MuxCase(0.U, Seq(
    d2.is_lui   -> alu2_lui,
    d2.is_auipc -> alu2_auipc,
    (d2.is_addi || d2.is_add) -> alu2_add,
    d2.is_sub   -> alu2_sub,
    (d2.is_slti || d2.is_slt) -> alu2_slt,
    (d2.is_sltiu || d2.is_sltu) -> alu2_sltu,
    (d2.is_xori || d2.is_xor) -> alu2_xor,
    (d2.is_ori  || d2.is_or)  -> alu2_or,
    (d2.is_andi || d2.is_and) -> alu2_and,
    (d2.is_slli || d2.is_sll) -> alu2_sll,
    (d2.is_srli || d2.is_srl) -> alu2_srl,
    (d2.is_srai || d2.is_sra) -> alu2_sra,
    (d2.is_jal  || d2.is_jalr) -> alu2_jal
  ))

  val mem2_addr = rs1_2 + imm2

  val need_wb2 = (d2.is_lui   || d2.is_auipc || d2.is_jal || d2.is_jalr ||
                  d2.is_addi  || d2.is_slti  || d2.is_sltiu || d2.is_xori ||
                  d2.is_ori   || d2.is_andi  || d2.is_slli  || d2.is_srli ||
                  d2.is_srai  || d2.is_add   || d2.is_sub   || d2.is_sll ||
                  d2.is_slt   || d2.is_sltu  || d2.is_xor   || d2.is_srl ||
                  d2.is_sra   || d2.is_or    || d2.is_and) && !d2.is_ebreak

  val is_load2 = d2.is_lb || d2.is_lh || d2.is_lw || d2.is_lbu || d2.is_lhu

  val rd_final2 = Mux(need_wb2 || is_load2, msg.dec2_rd, 0.U(5.W))
  val wb_data2  = Mux(is_load2, 0.U(32.W), alu2_out)

  // ---------- 总线输出: 运算结果 + 访存请求 ----------
  io.out.bits.op1.is_lb   := d1.is_lb
  io.out.bits.op1.is_lh   := d1.is_lh
  io.out.bits.op1.is_lw   := d1.is_lw
  io.out.bits.op1.is_lbu  := d1.is_lbu
  io.out.bits.op1.is_lhu  := d1.is_lhu
  io.out.bits.op1.is_sb   := d1.is_sb
  io.out.bits.op1.is_sh   := d1.is_sh
  io.out.bits.op1.is_sw   := d1.is_sw
  io.out.bits.addr1       := mem1_addr
  io.out.bits.store_data1 := rs2_1
  io.out.bits.rd1         := rd_final1
  io.out.bits.wb_data1    := wb_data1

  io.out.bits.op2.is_lb   := d2.is_lb
  io.out.bits.op2.is_lh   := d2.is_lh
  io.out.bits.op2.is_lw   := d2.is_lw
  io.out.bits.op2.is_lbu  := d2.is_lbu
  io.out.bits.op2.is_lhu  := d2.is_lhu
  io.out.bits.op2.is_sb   := d2.is_sb
  io.out.bits.op2.is_sh   := d2.is_sh
  io.out.bits.op2.is_sw   := d2.is_sw
  io.out.bits.addr2       := mem2_addr
  io.out.bits.store_data2 := rs2_2
  io.out.bits.rd2         := rd_final2
  io.out.bits.wb_data2    := wb_data2

  io.out.bits.is_ebreak := d1.is_ebreak

  // ---------- debug ----------
  io.debug_exu1.alu_out     := alu1_out
  io.debug_exu1.alu_source1 := MuxCase(rs1_1, Seq(
    d1.is_auipc -> pc1,
    d1.is_jal   -> pc1,
    d1.is_lui   -> 0.U(32.W)
  ))
  io.debug_exu1.alu_source2 := Mux(d1.is_addi || d1.is_slti || d1.is_sltiu ||
                                   d1.is_xori || d1.is_ori || d1.is_andi ||
                                   d1.is_slli || d1.is_srli || d1.is_srai ||
                                   d1.is_jalr ||
                                   d1.is_lb || d1.is_lh || d1.is_lw ||
                                   d1.is_lbu || d1.is_lhu ||
                                   d1.is_sb || d1.is_sh || d1.is_sw,
                                   imm1, rs2_1)
  io.debug_exu1.agu_out     := mem1_addr

  io.debug_exu2.alu_out     := alu2_out
  io.debug_exu2.alu_source1 := MuxCase(rs1_2, Seq(
    d2.is_auipc -> pc2,
    d2.is_jal   -> pc2,
    d2.is_lui   -> 0.U(32.W)
  ))
  io.debug_exu2.alu_source2 := Mux(d2.is_addi || d2.is_slti || d2.is_sltiu ||
                                   d2.is_xori || d2.is_ori || d2.is_andi ||
                                   d2.is_slli || d2.is_srli || d2.is_srai ||
                                   d2.is_jalr ||
                                   d2.is_lb || d2.is_lh || d2.is_lw ||
                                   d2.is_lbu || d2.is_lhu ||
                                   d2.is_sb || d2.is_sh || d2.is_sw,
                                   imm2, rs2_2)
  io.debug_exu2.agu_out     := mem2_addr
}

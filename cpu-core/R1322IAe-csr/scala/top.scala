package R1322IAeCSR

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

// 双端口黑盒存储器，通过 DPI-C 与 C++ 交互
class DPIMemory extends BlackBox {
  val io = IO(new Bundle {
    val io_clk = Input(Clock())
    val wen    = Input(Bool())
    val waddr  = Input(UInt(32.W))
    val wdata  = Input(UInt(32.W))
    val wmask  = Input(UInt(4.W))
    val ren1   = Input(Bool())
    val raddr1 = Input(UInt(32.W))
    val rdata1 = Output(UInt(32.W))
    val ren2   = Input(Bool())
    val raddr2 = Input(UInt(32.W))
    val rdata2 = Output(UInt(32.W))
    val ebreak = Input(Bool())
  })
  override def desiredName = "DPI_Memory"
}

class top extends Module {
  val io = IO(new Bundle {
    // ---------- IFU debug ----------
    val debug_inst1_pc = Output(UInt(32.W))
    val debug_inst2_pc = Output(UInt(32.W))

    // ---------- IDU debug ----------
    val debug_inst1 = Output(UInt(32.W))
    val debug_inst2 = Output(UInt(32.W))
    val debug_stall = Output(Bool())

    // ---------- EXU1 debug ----------
    val debug_exu1_alu_out     = Output(UInt(32.W))
    val debug_exu1_alu_source1 = Output(UInt(32.W))
    val debug_exu1_alu_source2 = Output(UInt(32.W))
    val debug_exu1_agu_out     = Output(UInt(32.W))

    // ---------- EXU2 debug ----------
    val debug_exu2_alu_out     = Output(UInt(32.W))
    val debug_exu2_alu_source1 = Output(UInt(32.W))
    val debug_exu2_alu_source2 = Output(UInt(32.W))
    val debug_exu2_agu_out     = Output(UInt(32.W))

    // ---------- LSU1 debug ----------
    val debug_lsu1_is_load          = Output(Bool())
    val debug_lsu1_is_store         = Output(Bool())
    val debug_lsu1_addr             = Output(UInt(32.W))
    val debug_lsu1_read_origin      = Output(UInt(32.W))
    val debug_lsu1_final_wb_data    = Output(UInt(32.W))
    val debug_lsu1_store_mask       = Output(UInt(4.W))
    val debug_lsu1_store_data_shifted = Output(UInt(32.W))

    // ---------- LSU2 debug ----------
    val debug_lsu2_is_load          = Output(Bool())
    val debug_lsu2_is_store         = Output(Bool())
    val debug_lsu2_addr             = Output(UInt(32.W))
    val debug_lsu2_read_origin      = Output(UInt(32.W))
    val debug_lsu2_final_wb_data    = Output(UInt(32.W))
    val debug_lsu2_store_mask       = Output(UInt(4.W))
    val debug_lsu2_store_data_shifted = Output(UInt(32.W))

    // ---------- WBU debug ----------
    val debug_wbu_valid1   = Output(Bool())
    val debug_wbu_valid2   = Output(Bool())
    val debug_wbu_conflict = Output(Bool())
    val debug_wbu_rd1      = Output(UInt(5.W))
    val debug_wbu_rd2      = Output(UInt(5.W))
    val debug_wbu_wr1_addr = Output(UInt(5.W))
    val debug_wbu_wr2_addr = Output(UInt(5.W))

    // ---------- GRF debug ----------
    val debug_grf_regs   = Output(Vec(16, UInt(32.W)))
    val debug_grf_rden   = Output(Bool())
    val debug_grf_rdaddr = Output(UInt(5.W))
    val debug_grf_input  = Output(UInt(32.W))

    // ---------- CSR debug ----------
    val debug_mcycle   = Output(UInt(64.W))
    val debug_minstret = Output(UInt(64.W))
    val debug_mstatus  = Output(UInt(32.W))
    val debug_mcause   = Output(UInt(32.W))
    val debug_mepc     = Output(UInt(32.W))
  })

  // ========== 实例化所有模块 ==========
  val ifu  = Module(new IFU)
  val idu  = Module(new idu)
  val exu1 = Module(new EXU)
  val exu2 = Module(new EXU)
  val lsu1 = Module(new LSU)
  val lsu2 = Module(new LSU)
  val wbu  = Module(new wbu)
  val grf  = Module(new GRF)
  val csr  = Module(new CSR)
  val imem = Module(new imem)
  val dmem = Module(new dmem)

  // ===================== IFU ↔ IMEM =====================
  ifu.io.ifu_to_imem <> imem.io.ifu_to_imem
  ifu.io.imem_to_ifu <> imem.io.imem_to_ifu

  // ===================== IFU ↔ IDU =====================
  ifu.io.ifu_to_idu <> idu.io.ifu_to_idu
  ifu.io.idu_to_ifu <> idu.io.idu_to_ifu

  // ===================== IDU ↔ GRF =====================
  idu.io.idu_to_grf <> grf.io.idu_to_grf
  grf.io.grf_to_idu <> idu.io.grf_to_idu

  // ===================== IDU → EXU1 =====================
  idu.io.idu_to_exu1 <> exu1.io.idu_to_exu1

  // ===================== IDU → EXU2 =====================
  exu2.io.idu_to_exu1.dec1_op.is_lui    := idu.io.idu_to_exu2.dec2_op.is_lui
  exu2.io.idu_to_exu1.dec1_op.is_auipc  := idu.io.idu_to_exu2.dec2_op.is_auipc
  exu2.io.idu_to_exu1.dec1_op.is_jal    := idu.io.idu_to_exu2.dec2_op.is_jal
  exu2.io.idu_to_exu1.dec1_op.is_jalr   := idu.io.idu_to_exu2.dec2_op.is_jalr
  exu2.io.idu_to_exu1.dec1_op.is_beq    := idu.io.idu_to_exu2.dec2_op.is_beq
  exu2.io.idu_to_exu1.dec1_op.is_bne    := idu.io.idu_to_exu2.dec2_op.is_bne
  exu2.io.idu_to_exu1.dec1_op.is_blt    := idu.io.idu_to_exu2.dec2_op.is_blt
  exu2.io.idu_to_exu1.dec1_op.is_bge    := idu.io.idu_to_exu2.dec2_op.is_bge
  exu2.io.idu_to_exu1.dec1_op.is_bltu   := idu.io.idu_to_exu2.dec2_op.is_bltu
  exu2.io.idu_to_exu1.dec1_op.is_bgeu   := idu.io.idu_to_exu2.dec2_op.is_bgeu
  exu2.io.idu_to_exu1.dec1_op.is_lb     := idu.io.idu_to_exu2.dec2_op.is_lb
  exu2.io.idu_to_exu1.dec1_op.is_lh     := idu.io.idu_to_exu2.dec2_op.is_lh
  exu2.io.idu_to_exu1.dec1_op.is_lw     := idu.io.idu_to_exu2.dec2_op.is_lw
  exu2.io.idu_to_exu1.dec1_op.is_lbu    := idu.io.idu_to_exu2.dec2_op.is_lbu
  exu2.io.idu_to_exu1.dec1_op.is_lhu    := idu.io.idu_to_exu2.dec2_op.is_lhu
  exu2.io.idu_to_exu1.dec1_op.is_sb     := idu.io.idu_to_exu2.dec2_op.is_sb
  exu2.io.idu_to_exu1.dec1_op.is_sh     := idu.io.idu_to_exu2.dec2_op.is_sh
  exu2.io.idu_to_exu1.dec1_op.is_sw     := idu.io.idu_to_exu2.dec2_op.is_sw
  exu2.io.idu_to_exu1.dec1_op.is_addi   := idu.io.idu_to_exu2.dec2_op.is_addi
  exu2.io.idu_to_exu1.dec1_op.is_slti   := idu.io.idu_to_exu2.dec2_op.is_slti
  exu2.io.idu_to_exu1.dec1_op.is_sltiu  := idu.io.idu_to_exu2.dec2_op.is_sltiu
  exu2.io.idu_to_exu1.dec1_op.is_xori   := idu.io.idu_to_exu2.dec2_op.is_xori
  exu2.io.idu_to_exu1.dec1_op.is_ori    := idu.io.idu_to_exu2.dec2_op.is_ori
  exu2.io.idu_to_exu1.dec1_op.is_andi   := idu.io.idu_to_exu2.dec2_op.is_andi
  exu2.io.idu_to_exu1.dec1_op.is_slli   := idu.io.idu_to_exu2.dec2_op.is_slli
  exu2.io.idu_to_exu1.dec1_op.is_srli   := idu.io.idu_to_exu2.dec2_op.is_srli
  exu2.io.idu_to_exu1.dec1_op.is_srai   := idu.io.idu_to_exu2.dec2_op.is_srai
  exu2.io.idu_to_exu1.dec1_op.is_add    := idu.io.idu_to_exu2.dec2_op.is_add
  exu2.io.idu_to_exu1.dec1_op.is_sub    := idu.io.idu_to_exu2.dec2_op.is_sub
  exu2.io.idu_to_exu1.dec1_op.is_sll    := idu.io.idu_to_exu2.dec2_op.is_sll
  exu2.io.idu_to_exu1.dec1_op.is_slt    := idu.io.idu_to_exu2.dec2_op.is_slt
  exu2.io.idu_to_exu1.dec1_op.is_sltu   := idu.io.idu_to_exu2.dec2_op.is_sltu
  exu2.io.idu_to_exu1.dec1_op.is_xor    := idu.io.idu_to_exu2.dec2_op.is_xor
  exu2.io.idu_to_exu1.dec1_op.is_srl    := idu.io.idu_to_exu2.dec2_op.is_srl
  exu2.io.idu_to_exu1.dec1_op.is_sra    := idu.io.idu_to_exu2.dec2_op.is_sra
  exu2.io.idu_to_exu1.dec1_op.is_or     := idu.io.idu_to_exu2.dec2_op.is_or
  exu2.io.idu_to_exu1.dec1_op.is_and    := idu.io.idu_to_exu2.dec2_op.is_and
  exu2.io.idu_to_exu1.dec1_op.is_ebreak := idu.io.idu_to_exu2.dec2_op.is_ebreak
  exu2.io.idu_to_exu1.dec1_op.is_csrrw  := idu.io.idu_to_exu2.dec2_op.is_csrrw
  exu2.io.idu_to_exu1.dec1_op.is_csrrs  := idu.io.idu_to_exu2.dec2_op.is_csrrs
  exu2.io.idu_to_exu1.dec1_op.is_csrrc  := idu.io.idu_to_exu2.dec2_op.is_csrrc
  exu2.io.idu_to_exu1.dec1_op.is_csrrwi := idu.io.idu_to_exu2.dec2_op.is_csrrwi
  exu2.io.idu_to_exu1.dec1_op.is_csrrsi := idu.io.idu_to_exu2.dec2_op.is_csrrsi
  exu2.io.idu_to_exu1.dec1_op.is_csrrci := idu.io.idu_to_exu2.dec2_op.is_csrrci
  exu2.io.idu_to_exu1.dec1_op.is_ecall  := idu.io.idu_to_exu2.dec2_op.is_ecall
  exu2.io.idu_to_exu1.dec1_op.is_mret   := idu.io.idu_to_exu2.dec2_op.is_mret
  exu2.io.idu_to_exu1.dec1_imm := idu.io.idu_to_exu2.dec2_imm
  exu2.io.idu_to_exu1.dec1_val.rs1_val := idu.io.idu_to_exu2.dec2_val.rs1_val
  exu2.io.idu_to_exu1.dec1_val.rs2_val := idu.io.idu_to_exu2.dec2_val.rs2_val
  exu2.io.idu_to_exu1.dec1_val.nextpc  := idu.io.idu_to_exu2.dec2_val.nextpc
  exu2.io.idu_to_exu1.dec1_rd          := idu.io.idu_to_exu2.dec2_rd

  // ===================== EXU1/2 → LSU1/2 =====================
  exu1.io.exu_to_lsu <> lsu1.io.exu_to_lsu
  exu2.io.exu_to_lsu <> lsu2.io.exu_to_lsu

  // ===================== LSU1/2 → WBU =====================
  wbu.io.lsu_to_wbu_1.rd          := lsu1.io.lsu_to_wbu.rd
  wbu.io.lsu_to_wbu_1.grf_wb_data := lsu1.io.lsu_to_wbu.grf_wb_data
  wbu.io.lsu_to_wbu_2.rd          := lsu2.io.lsu_to_wbu.rd
  wbu.io.lsu_to_wbu_2.grf_wb_data := lsu2.io.lsu_to_wbu.grf_wb_data

  // ===================== LSU1/2 ↔ DMEM =====================
  dmem.io.lsu_to_dmem_1.addr       := lsu1.io.lsu_to_dmem.addr
  dmem.io.lsu_to_dmem_1.store_data := lsu1.io.lsu_to_dmem.store_data
  dmem.io.lsu_to_dmem_1.mask       := lsu1.io.lsu_to_dmem.mask
  dmem.io.lsu_to_dmem_1.wen        := lsu1.io.lsu_to_dmem.wen
  dmem.io.lsu_to_dmem_1.ren        := lsu1.io.lsu_to_dmem.ren
  dmem.io.lsu_to_dmem_2.addr       := lsu2.io.lsu_to_dmem.addr
  dmem.io.lsu_to_dmem_2.store_data := lsu2.io.lsu_to_dmem.store_data
  dmem.io.lsu_to_dmem_2.mask       := lsu2.io.lsu_to_dmem.mask
  dmem.io.lsu_to_dmem_2.wen        := lsu2.io.lsu_to_dmem.wen
  dmem.io.lsu_to_dmem_2.ren        := lsu2.io.lsu_to_dmem.ren
  lsu1.io.dmem_to_lsu.load_data := dmem.io.dmem_to_lsu_1.load_data
  lsu2.io.dmem_to_lsu.load_data := dmem.io.dmem_to_lsu_2.load_data
  dmem.io.ebreak := lsu1.io.ebreak_out || lsu2.io.ebreak_out

  // ===================== WBU → GRF =====================
  grf.io.wbu_to_grf.wr1 <> wbu.io.wbu_to_grf.wr1
  grf.io.wbu_to_grf.wr2 <> wbu.io.wbu_to_grf.wr2

  // ===================== IFU CSR 接口 =====================
  ifu.io.csr_to_ifu.take_trap := csr.io.take_trap
  ifu.io.csr_to_ifu.trap_pc   := csr.io.trap_pc
  ifu.io.csr_to_ifu.take_mret := idu.io.idu_to_top.is_mret
  ifu.io.csr_to_ifu.mret_pc   := csr.io.debug_mepc

  // ===================== CSR 连接 =====================
  val csr_addr = idu.io.idu_to_top.inst1(31, 20)
  csr.io.csr_addr := csr_addr

  // CSR 操作类型判断
  val is_csr_write = idu.io.idu_to_top.is_csrrw || idu.io.idu_to_top.is_csrrs ||
                     idu.io.idu_to_top.is_csrrc || idu.io.idu_to_top.is_csrrwi ||
                     idu.io.idu_to_top.is_csrrsi || idu.io.idu_to_top.is_csrrci

  // csr_op: 0=csrrw(写), 1=csrrs(置位), 2=csrrc(清除)
  csr.io.csr_op := MuxCase(0.U(3.W), Seq(
    (idu.io.idu_to_top.is_csrrs || idu.io.idu_to_top.is_csrrsi) -> 1.U,
    (idu.io.idu_to_top.is_csrrc || idu.io.idu_to_top.is_csrrci) -> 2.U
  ))

  // CSR 写使能
  val use_imm_csr = idu.io.idu_to_top.is_csrrwi || idu.io.idu_to_top.is_csrrsi || idu.io.idu_to_top.is_csrrci
  val uimm = idu.io.idu_to_top.inst1(19, 15)
  val rs1_csr_nonzero = idu.io.idu_to_top.inst1(19, 15) =/= 0.U
  val uimm_nonzero = uimm =/= 0.U

  csr.io.use_imm := use_imm_csr
  csr.io.rs1_val := idu.io.idu_to_top.rs1_val

  val csrrw_or_wi = idu.io.idu_to_top.is_csrrw || idu.io.idu_to_top.is_csrrwi
  val csr_write_cond = Mux(csrrw_or_wi, true.B,
                       Mux(use_imm_csr, uimm_nonzero, rs1_csr_nonzero))
  csr.io.csr_wen := is_csr_write && csr_write_cond
  csr.io.csr_waddr := csr_addr
  csr.io.csr_wdata := Mux(use_imm_csr, Cat(0.U(27.W), uimm), idu.io.idu_to_top.rs1_val)

  // 异常接口
  csr.io.ecall      := idu.io.idu_to_top.is_ecall
  csr.io.mret       := idu.io.idu_to_top.is_mret
  csr.io.is_ebreak  := idu.io.idu_to_top.is_ebreak
  csr.io.current_pc := idu.io.idu_to_top.inst1_pc

  // 指令退休计数：正常指令（非ecall/ebreak）退休
  val normal_inst = !idu.io.idu_to_top.is_ebreak && !idu.io.idu_to_top.is_ecall
  csr.io.inst_retire := normal_inst

  // ===================== CSR 写回 GRF =====================
  // CSR 指令写回旧CSR值到rd（顶层直接连接GRF）
  val is_csr = idu.io.idu_to_top.is_csrrw || idu.io.idu_to_top.is_csrrs ||
               idu.io.idu_to_top.is_csrrc || idu.io.idu_to_top.is_csrrwi ||
               idu.io.idu_to_top.is_csrrsi || idu.io.idu_to_top.is_csrrci
  val csr_rd = idu.io.idu_to_top.inst1(11, 7)
  grf.io.csr_to_grf.wen   := is_csr && (csr_rd =/= 0.U)
  grf.io.csr_to_grf.waddr := csr_rd
  grf.io.csr_to_grf.wdata := csr.io.csr_rdata

  // ===================== 分支合并 =====================
  val exu1_valid = exu1.io.exu_to_ifu.take_branch
  val exu2_valid = exu2.io.exu_to_ifu.take_branch
  ifu.io.exu_to_ifu.take_branch   := exu1_valid || exu2_valid
  ifu.io.exu_to_ifu.branch_target := Mux(exu1_valid,
                                         exu1.io.exu_to_ifu.branch_target,
                                         exu2.io.exu_to_ifu.branch_target)

  // ===================== 所有 debug 连接 =====================
  // --- IFU ---
  io.debug_inst1_pc := ifu.io.debug.debug_inst1_pc
  io.debug_inst2_pc := ifu.io.debug.debug_inst2_pc

  // --- IDU ---
  io.debug_inst1 := idu.io.idu_debug.debug_inst1
  io.debug_inst2 := idu.io.idu_debug.debug_inst2
  io.debug_stall := idu.io.idu_debug.is_stall

  // --- EXU1 ---
  io.debug_exu1_alu_out     := exu1.io.debug_alu_out
  io.debug_exu1_alu_source1 := exu1.io.debug_alu_source1
  io.debug_exu1_alu_source2 := exu1.io.debug_alu_source2
  io.debug_exu1_agu_out     := exu1.io.debug_agu_out

  // --- EXU2 ---
  io.debug_exu2_alu_out     := exu2.io.debug_alu_out
  io.debug_exu2_alu_source1 := exu2.io.debug_alu_source1
  io.debug_exu2_alu_source2 := exu2.io.debug_alu_source2
  io.debug_exu2_agu_out     := exu2.io.debug_agu_out

  // --- LSU1 ---
  io.debug_lsu1_is_load            := lsu1.io.debug.is_load
  io.debug_lsu1_is_store           := lsu1.io.debug.is_store
  io.debug_lsu1_addr               := lsu1.io.debug.addr
  io.debug_lsu1_read_origin        := lsu1.io.debug.read_origin
  io.debug_lsu1_final_wb_data      := lsu1.io.debug.final_wb_data
  io.debug_lsu1_store_mask          := lsu1.io.debug.store_mask
  io.debug_lsu1_store_data_shifted := lsu1.io.debug.store_data_shifted

  // --- LSU2 ---
  io.debug_lsu2_is_load            := lsu2.io.debug.is_load
  io.debug_lsu2_is_store           := lsu2.io.debug.is_store
  io.debug_lsu2_addr               := lsu2.io.debug.addr
  io.debug_lsu2_read_origin        := lsu2.io.debug.read_origin
  io.debug_lsu2_final_wb_data      := lsu2.io.debug.final_wb_data
  io.debug_lsu2_store_mask          := lsu2.io.debug.store_mask
  io.debug_lsu2_store_data_shifted := lsu2.io.debug.store_data_shifted

  // --- WBU ---
  io.debug_wbu_valid1   := wbu.io.debug.valid1
  io.debug_wbu_valid2   := wbu.io.debug.valid2
  io.debug_wbu_conflict := wbu.io.debug.conflict
  io.debug_wbu_rd1      := wbu.io.debug.rd1
  io.debug_wbu_rd2      := wbu.io.debug.rd2
  io.debug_wbu_wr1_addr := wbu.io.debug.wr1_addr
  io.debug_wbu_wr2_addr := wbu.io.debug.wr2_addr

  // --- GRF ---
  io.debug_grf_regs   := grf.io.debug_regs
  io.debug_grf_rden   := grf.io.debug_rden
  io.debug_grf_rdaddr := grf.io.debug_rdaddr
  io.debug_grf_input  := grf.io.debug_input

  // --- CSR ---
  io.debug_mcycle   := csr.io.debug_mcycle
  io.debug_minstret := csr.io.debug_minstret
  io.debug_mstatus  := csr.io.debug_mstatus
  io.debug_mcause   := csr.io.debug_mcause
  io.debug_mepc     := csr.io.debug_mepc
}

object top extends App {
  ChiselStage.emitSystemVerilogFile(
    new top,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}
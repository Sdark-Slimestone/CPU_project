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
    val debug_mcycle    = Output(UInt(64.W))
    val debug_minstret  = Output(UInt(64.W))
    val debug_mstatus   = Output(UInt(32.W))
    val debug_mie       = Output(UInt(32.W))
    val debug_mtvec     = Output(UInt(32.W))
    val debug_mepc      = Output(UInt(32.W))
    val debug_mcause    = Output(UInt(32.W))
    val debug_mtval     = Output(UInt(32.W))
    val debug_mip       = Output(UInt(32.W))
    val debug_mscratch  = Output(UInt(32.W))
    val debug_mvendorid = Output(UInt(32.W))
    val debug_marchid   = Output(UInt(32.W))
    val debug_mimpid    = Output(UInt(32.W))
    val debug_mhartid   = Output(UInt(32.W))
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
  exu2.io.idu_to_exu1.dec1_op  := idu.io.idu_to_exu2.dec2_op
  exu2.io.idu_to_exu1.dec1_imm := idu.io.idu_to_exu2.dec2_imm
  exu2.io.idu_to_exu1.dec1_val := idu.io.idu_to_exu2.dec2_val
  exu2.io.idu_to_exu1.dec1_rd  := idu.io.idu_to_exu2.dec2_rd

  // ===================== EXU CSR 输入（来自 IDU+IDU） =====================
  // EXU1 接收第一条指令的 CSR 直通信号
  exu1.io.idu_csr.is_csrrw  := idu.io.idu_to_top.is_csrrw
  exu1.io.idu_csr.is_csrrs  := idu.io.idu_to_top.is_csrrs
  exu1.io.idu_csr.is_csrrc  := idu.io.idu_to_top.is_csrrc
  exu1.io.idu_csr.is_csrrwi := idu.io.idu_to_top.is_csrrwi
  exu1.io.idu_csr.is_csrrsi := idu.io.idu_to_top.is_csrrsi
  exu1.io.idu_csr.is_csrrci := idu.io.idu_to_top.is_csrrci
  exu1.io.idu_csr.is_ecall  := idu.io.idu_to_top.is_ecall
  exu1.io.idu_csr.is_mret   := idu.io.idu_to_top.is_mret
  exu1.io.idu_csr.is_ebreak := idu.io.idu_to_top.is_ebreak
  exu1.io.idu_csr.inst1_pc  := idu.io.idu_to_top.inst1_pc
  exu1.io.idu_csr.inst1     := idu.io.idu_to_top.inst1
  exu1.io.idu_csr.rs1_val   := idu.io.idu_to_top.rs1_val
  exu1.io.is_stall          := idu.io.idu_to_ifu.is_stall

  // EXU2 的 CSR 输入清零（CSR 只在第一条指令有效）
  exu2.io.idu_csr.is_csrrw  := false.B
  exu2.io.idu_csr.is_csrrs  := false.B
  exu2.io.idu_csr.is_csrrc  := false.B
  exu2.io.idu_csr.is_csrrwi := false.B
  exu2.io.idu_csr.is_csrrsi := false.B
  exu2.io.idu_csr.is_csrrci := false.B
  exu2.io.idu_csr.is_ecall  := false.B
  exu2.io.idu_csr.is_mret   := false.B
  exu2.io.idu_csr.is_ebreak := false.B
  exu2.io.idu_csr.inst1_pc  := 0.U
  exu2.io.idu_csr.inst1     := 0.U
  exu2.io.idu_csr.rs1_val   := 0.U
  exu2.io.is_stall          := false.B

  // ===================== IFU CSR 接口 =====================
  ifu.io.csr_to_ifu.take_trap := exu1.io.csr_to_ifu.take_trap
  ifu.io.csr_to_ifu.trap_pc   := exu1.io.csr_to_ifu.trap_pc
  ifu.io.csr_to_ifu.take_mret := exu1.io.csr_to_ifu.take_mret
  ifu.io.csr_to_ifu.mret_pc   := exu1.io.csr_to_ifu.mret_pc

  // ===================== CSR 写回 GRF =====================
  grf.io.csr_to_grf.wen   := exu1.io.csr_to_grf.wen
  grf.io.csr_to_grf.waddr := exu1.io.csr_to_grf.waddr
  grf.io.csr_to_grf.wdata := exu1.io.csr_to_grf.wdata

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
  wbu.io.wbu_to_grf <> grf.io.wbu_to_grf

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
  io.debug_mcycle    := exu1.io.debug_csr.mcycle
  io.debug_minstret  := exu1.io.debug_csr.minstret
  io.debug_mstatus   := exu1.io.debug_csr.mstatus
  io.debug_mie       := exu1.io.debug_csr.mie
  io.debug_mtvec     := exu1.io.debug_csr.mtvec
  io.debug_mepc      := exu1.io.debug_csr.mepc
  io.debug_mcause    := exu1.io.debug_csr.mcause
  io.debug_mtval     := exu1.io.debug_csr.mtval
  io.debug_mip       := exu1.io.debug_csr.mip
  io.debug_mscratch  := exu1.io.debug_csr.mscratch
  io.debug_mvendorid := exu1.io.debug_csr.mvendorid
  io.debug_marchid   := exu1.io.debug_csr.marchid
  io.debug_mimpid    := exu1.io.debug_csr.mimpid
  io.debug_mhartid   := exu1.io.debug_csr.mhartid
}

object top extends App {
  ChiselStage.emitSystemVerilogFile(
    new top,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}
package R1322IAeCSRMC

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

// 多周期(分布式控制)双发射 NPC, 经典五级结构:
//   IFU --Decoupled(IFMsg)--> IDU --Decoupled(IDMsg)--> EXU
//       --Decoupled(EXMsg)--> LSU --Decoupled(WBMsg)--> WBU
// 级间用 StageConnect 抽象连接(arch = "multi"), 一次在飞行中最多一个取指包,
// WBU 的 done 完成信号回传 IFU 触发下一包取指, 分支/异常重定向在 EXU 提交时发出
class top extends Module {
  val io = IO(new Bundle {
    // ---------- IFU/IDU debug ----------
    val debug_inst1_pc = Output(UInt(32.W))
    val debug_inst2_pc = Output(UInt(32.W))

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
  val exu  = Module(new EXU)
  val lsu  = Module(new LSU)
  val wbu  = Module(new wbu)
  val grf  = Module(new GRF)
  val imem = Module(new imem)
  val dmem = Module(new dmem)

  // ===================== IFU ↔ IMEM =====================
  ifu.io.ifu_to_imem <> imem.io.ifu_to_imem
  ifu.io.imem_to_ifu <> imem.io.imem_to_ifu

  // ===================== 总线连接 =====================
  // IFU -> IDU -> EXU -> LSU -> WBU, 由 StageConnect 按 arch 连接
  StageConnect(ifu.io.out, idu.io.in)
  StageConnect(idu.io.out, exu.io.in)
  StageConnect(exu.io.out, lsu.io.in)
  StageConnect(lsu.io.out, wbu.io.in)

  // ===================== IDU ↔ GRF =====================
  idu.io.idu_to_grf <> grf.io.idu_to_grf
  grf.io.grf_to_idu <> idu.io.grf_to_idu

  // ===================== LSU ↔ DMEM =====================
  dmem.io.lsu_to_dmem_1 <> lsu.io.lsu_to_dmem_1
  dmem.io.lsu_to_dmem_2 <> lsu.io.lsu_to_dmem_2
  dmem.io.dmem_to_lsu_1 <> lsu.io.dmem_to_lsu_1
  dmem.io.dmem_to_lsu_2 <> lsu.io.dmem_to_lsu_2
  dmem.io.ebreak := lsu.io.ebreak_out

  // ===================== WBU → GRF =====================
  wbu.io.wbu_to_grf <> grf.io.wbu_to_grf

  // ===================== 重定向/流控回传 IFU =====================
  ifu.io.exu_to_ifu <> exu.io.exu_to_ifu
  ifu.io.csr_to_ifu <> exu.io.csr_to_ifu
  ifu.io.idu_to_ifu <> idu.io.idu_to_ifu
  ifu.io.wbu_to_ifu <> wbu.io.wbu_to_ifu

  // ===================== CSR 写回 GRF =====================
  grf.io.csr_to_grf <> exu.io.csr_to_grf

  // ===================== 所有 debug 连接 =====================
  // --- IFU/IDU (显示当前在飞行的取指包) ---
  io.debug_inst1_pc := idu.io.idu_debug.debug_inst1_pc
  io.debug_inst2_pc := idu.io.idu_debug.debug_inst2_pc
  io.debug_inst1    := idu.io.idu_debug.debug_inst1
  io.debug_inst2    := idu.io.idu_debug.debug_inst2
  io.debug_stall    := idu.io.idu_debug.is_stall

  // --- EXU1 ---
  io.debug_exu1_alu_out     := exu.io.debug_exu1.alu_out
  io.debug_exu1_alu_source1 := exu.io.debug_exu1.alu_source1
  io.debug_exu1_alu_source2 := exu.io.debug_exu1.alu_source2
  io.debug_exu1_agu_out     := exu.io.debug_exu1.agu_out

  // --- EXU2 ---
  io.debug_exu2_alu_out     := exu.io.debug_exu2.alu_out
  io.debug_exu2_alu_source1 := exu.io.debug_exu2.alu_source1
  io.debug_exu2_alu_source2 := exu.io.debug_exu2.alu_source2
  io.debug_exu2_agu_out     := exu.io.debug_exu2.agu_out

  // --- LSU1 ---
  io.debug_lsu1_is_load            := lsu.io.debug_lsu1.is_load
  io.debug_lsu1_is_store           := lsu.io.debug_lsu1.is_store
  io.debug_lsu1_addr               := lsu.io.debug_lsu1.addr
  io.debug_lsu1_read_origin        := lsu.io.debug_lsu1.read_origin
  io.debug_lsu1_final_wb_data      := lsu.io.debug_lsu1.final_wb_data
  io.debug_lsu1_store_mask         := lsu.io.debug_lsu1.store_mask
  io.debug_lsu1_store_data_shifted := lsu.io.debug_lsu1.store_data_shifted

  // --- LSU2 ---
  io.debug_lsu2_is_load            := lsu.io.debug_lsu2.is_load
  io.debug_lsu2_is_store           := lsu.io.debug_lsu2.is_store
  io.debug_lsu2_addr               := lsu.io.debug_lsu2.addr
  io.debug_lsu2_read_origin        := lsu.io.debug_lsu2.read_origin
  io.debug_lsu2_final_wb_data      := lsu.io.debug_lsu2.final_wb_data
  io.debug_lsu2_store_mask         := lsu.io.debug_lsu2.store_mask
  io.debug_lsu2_store_data_shifted := lsu.io.debug_lsu2.store_data_shifted

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
  io.debug_mcycle    := exu.io.debug_csr.mcycle
  io.debug_minstret  := exu.io.debug_csr.minstret
  io.debug_mstatus   := exu.io.debug_csr.mstatus
  io.debug_mie       := exu.io.debug_csr.mie
  io.debug_mtvec     := exu.io.debug_csr.mtvec
  io.debug_mepc      := exu.io.debug_csr.mepc
  io.debug_mcause    := exu.io.debug_csr.mcause
  io.debug_mtval     := exu.io.debug_csr.mtval
  io.debug_mip       := exu.io.debug_csr.mip
  io.debug_mscratch  := exu.io.debug_csr.mscratch
  io.debug_mvendorid := exu.io.debug_csr.mvendorid
  io.debug_marchid   := exu.io.debug_csr.marchid
  io.debug_mimpid    := exu.io.debug_csr.mimpid
  io.debug_mhartid   := exu.io.debug_csr.mhartid
}

object top extends App {
  ChiselStage.emitSystemVerilogFile(
    new top,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}

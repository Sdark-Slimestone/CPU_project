package R1322IAeCSR

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

// 双端口黑盒存储器, 通过 DPI-C 与 C++ 交互
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

// 顶层: 把 IFU / IDU / EXU / LSU / WBU 五个模块 + GRF/CSR/存储器 用流水级寄存器连起来。
// 与 R1322IAe-1 (一次一包的分布式多周期) 的区别只在级间: 把"空才能收"的 StageConnect
// 换成每拍吞吐的 StagePipe, 并补齐前端重定向/冲刷与跨包 interlock; 各级内部逻辑不变。
class top extends Module {
  val io = IO(new Bundle {
    // ---------- IFU debug ----------
    val debug_inst1_pc = Output(UInt(32.W))
    val debug_inst2_pc = Output(UInt(32.W))

    // ---------- IDU debug ----------
    val debug_inst1 = Output(UInt(32.W))
    val debug_inst2 = Output(UInt(32.W))
    val debug_stall = Output(Bool())

    // ---------- 瓶颈计数 (prof) ----------
    val debug_d_valid      = Output(Bool())
    val debug_d_stall      = Output(Bool())
    val debug_stall_load   = Output(Bool())
    val debug_stall_csr    = Output(Bool())
    val debug_stall_mem    = Output(Bool())
    val debug_single_issue = Output(Bool())
    val debug_single_raw   = Output(Bool())
    val debug_single_ctl1  = Output(Bool())
    val debug_single_mem   = Output(Bool())
    val debug_single_ctl2  = Output(Bool())
    val debug_exu_redirect = Output(Bool())
    val debug_pred_redirect= Output(Bool())
    val debug_e_valid      = Output(Bool())
    val debug_m_valid      = Output(Bool())
    val debug_w_valid      = Output(Bool())
    val debug_retire1      = Output(Bool())
    val debug_retire2      = Output(Bool())

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
    val debug_lsu1_is_load            = Output(Bool())
    val debug_lsu1_is_store           = Output(Bool())
    val debug_lsu1_addr               = Output(UInt(32.W))
    val debug_lsu1_read_origin        = Output(UInt(32.W))
    val debug_lsu1_final_wb_data      = Output(UInt(32.W))
    val debug_lsu1_store_mask         = Output(UInt(4.W))
    val debug_lsu1_store_data_shifted = Output(UInt(32.W))

    // ---------- LSU2 debug ----------
    val debug_lsu2_is_load            = Output(Bool())
    val debug_lsu2_is_store           = Output(Bool())
    val debug_lsu2_addr               = Output(UInt(32.W))
    val debug_lsu2_read_origin        = Output(UInt(32.W))
    val debug_lsu2_final_wb_data      = Output(UInt(32.W))
    val debug_lsu2_store_mask         = Output(UInt(4.W))
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

  // ========== 实例化 ==========
  val ifu  = Module(new IFU)
  val idu  = Module(new IDU)
  val exu1 = Module(new EXU)
  val exu2 = Module(new EXU)
  val lsu1 = Module(new LSU)
  val lsu2 = Module(new LSU)
  val wbu  = Module(new WBU)
  val grf  = Module(new GRF)
  val imem = Module(new imem)
  val dmem = Module(new dmem)

  // ===================== IFU <-> IMEM =====================
  ifu.io.ifu_to_imem <> imem.io.ifu_to_imem
  ifu.io.imem_to_ifu <> imem.io.imem_to_ifu

  // ===================== 流水控制: 重定向 / 冻结 (BTFN 静态预测) =====================
  // 预测方向 (与 IDU 一致): 后向条件分支 / jal -> taken, 其余 not-taken。
  // EXU 只在"实际方向 != 预测方向"时重定向 (预测正确就不冲队列)。
  val e_ctrl   = exu1.io.idu_to_exu1.bits.ctrl
  val e_imm    = exu1.io.idu_to_exu1.bits.imm
  val e_pred_taken = e_ctrl.is_jal || (e_ctrl.is_branch && e_imm(31))
  val e_actual     = exu1.io.exu_to_ifu.take_branch   // jump || (branch && cond)
  val exu_mispred  = exu1.io.idu_to_exu1.valid && (e_actual =/= e_pred_taken)
  val exu_redirect = exu1.io.idu_to_exu1.valid &&
                     (exu1.io.csr_to_ifu.take_trap ||
                      exu1.io.csr_to_ifu.take_mret ||
                      exu_mispred)
  // 重定向目标: trap/mret 优先; 实际 taken 用 EXU 目标; 预测 taken 但实际 not-taken -> pc+4
  val exu_redirect_pc = Mux(exu1.io.csr_to_ifu.take_trap, exu1.io.csr_to_ifu.trap_pc,
                        Mux(exu1.io.csr_to_ifu.take_mret, exu1.io.csr_to_ifu.mret_pc,
                        Mux(e_actual, exu1.io.exu_to_ifu.branch_target,
                            exu1.io.idu_to_exu1.bits.pc + 4.U)))

  val redirect    = exu_redirect || idu.io.pred_redirect
  val redirect_pc = Mux(exu_redirect, exu_redirect_pc, idu.io.pred_redirect_pc)

  ifu.io.redirect    := redirect
  ifu.io.redirect_pc := redirect_pc

  val d_stall = idu.io.stall

  // ===================== 取指队列 (IF <-> IDU 解耦, 按条弹出) =====================
  val fq = Module(new FetchQueue(4))
  fq.io.push_valid  := ifu.io.push_valid
  fq.io.push_inst   := ifu.io.push_inst
  fq.io.push_pc     := ifu.io.push_pc
  ifu.io.push_ready := fq.io.push_ready

  idu.io.from_fq.inst1 := fq.io.out_inst(0)
  idu.io.from_fq.inst2 := fq.io.out_inst(1)
  idu.io.from_fq.pc1   := fq.io.out_pc(0)
  idu.io.from_fq.pc2   := fq.io.out_pc(1)
  idu.io.from_fq.valid := fq.io.out_valid
  fq.io.pop_cnt := idu.io.fq_pop
  fq.io.flush   := redirect

  // 包内冲突: 队头有两条但只弹 1 条
  val idu_single_issue = (fq.io.out_valid === 2.U) &&
                         idu.io.debug.is_stall && !d_stall

  // ===================== IDU <-> GRF =====================
  idu.io.idu_to_grf <> grf.io.idu_to_grf
  grf.io.grf_to_idu <> idu.io.grf_to_idu

  // ===================== 跨包冒险来源 (给 IDU 判 load-use/CSR 停顿) =====================
  // E 级 (D/E 寄存器输出 = EXU 输入)
  idu.io.fwd.exu_rd(0)       := exu1.io.idu_to_exu1.bits.rd
  idu.io.fwd.exu_valid(0)    := exu1.io.idu_to_exu1.valid
  idu.io.fwd.exu_is_store(0) := exu1.io.exu_to_lsu.bits.op.isStore
  idu.io.fwd.exu_is_load(0)  := exu1.io.exu_to_lsu.bits.op.isLoad
  idu.io.fwd.exu_is_csr(0)   := exu1.io.idu_to_exu1.bits.ctrl.is_csr
  idu.io.fwd.exu_rd(1)       := exu2.io.idu_to_exu1.bits.rd
  idu.io.fwd.exu_valid(1)    := exu2.io.idu_to_exu1.valid
  idu.io.fwd.exu_is_store(1) := exu2.io.exu_to_lsu.bits.op.isStore
  idu.io.fwd.exu_is_load(1)  := exu2.io.exu_to_lsu.bits.op.isLoad
  idu.io.fwd.exu_is_csr(1)   := exu2.io.idu_to_exu1.bits.ctrl.is_csr

  // M 级 store (访存顺序冒险用)
  idu.io.fwd.lsu_valid(0)    := lsu1.io.exu_to_lsu.valid
  idu.io.fwd.lsu_is_store(0) := lsu1.io.exu_to_lsu.bits.op.isStore
  idu.io.fwd.lsu_valid(1)    := lsu2.io.exu_to_lsu.valid
  idu.io.fwd.lsu_is_store(1) := lsu2.io.exu_to_lsu.bits.op.isStore

  // ===================== CSR 写回 GRF =====================
  grf.io.csr_to_grf.wen   := exu1.io.csr_to_grf.wen
  grf.io.csr_to_grf.waddr := exu1.io.csr_to_grf.waddr
  grf.io.csr_to_grf.wdata := exu1.io.csr_to_grf.wdata

  // ===================== 教科书转发: EX/MEM(M 级) + MEM/WB(W 级) -> EX 操作数 =====================
  val fwd1 = Module(new LaneForward)
  val fwd2 = Module(new LaneForward)
  for (f <- Seq(fwd1, fwd2)) {
    // M 级生产者 = E/M 流水寄存器输出 (LSU 输入)
    f.io.m_rd(0)     := lsu1.io.exu_to_lsu.bits.rd
    f.io.m_valid(0)  := lsu1.io.exu_to_lsu.valid
    f.io.m_isLoad(0) := lsu1.io.exu_to_lsu.bits.op.isLoad
    f.io.m_data(0)   := lsu1.io.exu_to_lsu.bits.wb_data
    f.io.m_rd(1)     := lsu2.io.exu_to_lsu.bits.rd
    f.io.m_valid(1)  := lsu2.io.exu_to_lsu.valid
    f.io.m_isLoad(1) := lsu2.io.exu_to_lsu.bits.op.isLoad
    f.io.m_data(1)   := lsu2.io.exu_to_lsu.bits.wb_data

    // W 级生产者 = M/W 流水寄存器输出 (WBU 输入)
    f.io.w_rd(0)    := wbu.io.lsu_to_wbu_1.bits.rd
    f.io.w_valid(0) := wbu.io.lsu_to_wbu_1.valid
    f.io.w_data(0)  := wbu.io.lsu_to_wbu_1.bits.grf_wb_data
    f.io.w_rd(1)    := wbu.io.lsu_to_wbu_2.bits.rd
    f.io.w_valid(1) := wbu.io.lsu_to_wbu_2.valid
    f.io.w_data(1)  := wbu.io.lsu_to_wbu_2.bits.grf_wb_data
  }

  // ===================== IDU -> [D/E 流水寄存器] -> 转发 -> EXU1/EXU2 =====================
  StagePipe(idu.io.idu_to_exu1, fwd1.io.in, false.B, exu_redirect)
  fwd1.io.out <> exu1.io.idu_to_exu1
  StagePipe(idu.io.idu_to_exu2, fwd2.io.in, false.B, exu_redirect)
  fwd2.io.out <> exu2.io.idu_to_exu1

  // ===================== EXU1/2 -> [E/M 流水寄存器] -> LSU1/2 =====================
  StagePipe(exu1.io.exu_to_lsu, lsu1.io.exu_to_lsu, false.B, false.B)
  StagePipe(exu2.io.exu_to_lsu, lsu2.io.exu_to_lsu, false.B, false.B)

  // ===================== LSU1/2 -> [M/W 流水寄存器] -> WBU =====================
  StagePipe(lsu1.io.lsu_to_wbu, wbu.io.lsu_to_wbu_1, false.B, false.B)
  StagePipe(lsu2.io.lsu_to_wbu, wbu.io.lsu_to_wbu_2, false.B, false.B)

  // ===================== LSU1/2 <-> DMEM =====================
  // 读请求 (E 级) 由 EXU 直接发给 dmem; 写请求 (M 级) 由 LSU 发给 dmem
  dmem.io.exu_to_dmem_1.addr := exu1.io.exu_to_lsu.bits.addr
  dmem.io.exu_to_dmem_1.ren  := exu1.io.exu_to_lsu.bits.op.isLoad
  dmem.io.exu_to_dmem_2.addr := exu2.io.exu_to_lsu.bits.addr
  dmem.io.exu_to_dmem_2.ren  := exu2.io.exu_to_lsu.bits.op.isLoad
  dmem.io.lsu_to_dmem_1 <> lsu1.io.lsu_to_dmem
  dmem.io.lsu_to_dmem_2 <> lsu2.io.lsu_to_dmem
  lsu1.io.dmem_to_lsu.load_data := dmem.io.dmem_to_lsu_1.load_data
  lsu2.io.dmem_to_lsu.load_data := dmem.io.dmem_to_lsu_2.load_data
  dmem.io.ebreak := lsu1.io.ebreak_out || lsu2.io.ebreak_out

  // ===================== WBU -> GRF =====================
  wbu.io.wbu_to_grf <> grf.io.wbu_to_grf

  // ===================== debug 导出 =====================
  io.debug_inst1_pc := ifu.io.debug.inst1_pc
  io.debug_inst2_pc := ifu.io.debug.inst2_pc

  io.debug_inst1 := idu.io.debug.debug_inst1
  io.debug_inst2 := idu.io.debug.debug_inst2
  io.debug_stall := idu.io.debug.is_stall

  io.debug_d_valid      := fq.io.out_valid >= 1.U
  io.debug_d_stall      := d_stall
  io.debug_stall_load   := idu.io.debug.stall_e_load
  io.debug_stall_csr    := idu.io.debug.stall_e_csr
  io.debug_stall_mem    := idu.io.debug.stall_mem
  io.debug_single_issue := idu_single_issue
  io.debug_single_raw   := idu.io.debug.single_raw
  io.debug_single_ctl1  := idu.io.debug.single_ctl1
  io.debug_single_mem   := idu.io.debug.single_ramwaw
  io.debug_single_ctl2  := idu.io.debug.single_ctl2
  io.debug_exu_redirect := exu_redirect
  io.debug_pred_redirect:= idu.io.pred_redirect
  io.debug_e_valid      := exu1.io.idu_to_exu1.valid || exu2.io.idu_to_exu1.valid
  io.debug_m_valid      := lsu1.io.exu_to_lsu.valid || lsu2.io.exu_to_lsu.valid
  io.debug_w_valid      := wbu.io.lsu_to_wbu_1.valid || wbu.io.lsu_to_wbu_2.valid
  io.debug_retire1      := wbu.io.lsu_to_wbu_1.valid
  io.debug_retire2      := wbu.io.lsu_to_wbu_2.valid

  io.debug_exu1_alu_out     := exu1.io.debug_alu_out
  io.debug_exu1_alu_source1 := exu1.io.debug_alu_source1
  io.debug_exu1_alu_source2 := exu1.io.debug_alu_source2
  io.debug_exu1_agu_out     := exu1.io.debug_agu_out

  io.debug_exu2_alu_out     := exu2.io.debug_alu_out
  io.debug_exu2_alu_source1 := exu2.io.debug_alu_source1
  io.debug_exu2_alu_source2 := exu2.io.debug_alu_source2
  io.debug_exu2_agu_out     := exu2.io.debug_agu_out

  io.debug_lsu1_is_load            := lsu1.io.debug.is_load
  io.debug_lsu1_is_store           := lsu1.io.debug.is_store
  io.debug_lsu1_addr               := lsu1.io.debug.addr
  io.debug_lsu1_read_origin        := lsu1.io.debug.read_origin
  io.debug_lsu1_final_wb_data      := lsu1.io.debug.final_wb_data
  io.debug_lsu1_store_mask         := lsu1.io.debug.store_mask
  io.debug_lsu1_store_data_shifted := lsu1.io.debug.store_data_shifted

  io.debug_lsu2_is_load            := lsu2.io.debug.is_load
  io.debug_lsu2_is_store           := lsu2.io.debug.is_store
  io.debug_lsu2_addr               := lsu2.io.debug.addr
  io.debug_lsu2_read_origin        := lsu2.io.debug.read_origin
  io.debug_lsu2_final_wb_data      := lsu2.io.debug.final_wb_data
  io.debug_lsu2_store_mask         := lsu2.io.debug.store_mask
  io.debug_lsu2_store_data_shifted := lsu2.io.debug.store_data_shifted

  io.debug_wbu_valid1   := wbu.io.debug.valid1
  io.debug_wbu_valid2   := wbu.io.debug.valid2
  io.debug_wbu_conflict := wbu.io.debug.conflict
  io.debug_wbu_rd1      := wbu.io.debug.rd1
  io.debug_wbu_rd2      := wbu.io.debug.rd2
  io.debug_wbu_wr1_addr := wbu.io.debug.wr1_addr
  io.debug_wbu_wr2_addr := wbu.io.debug.wr2_addr

  io.debug_grf_regs   := grf.io.debug_regs
  io.debug_grf_rden   := grf.io.debug_rden
  io.debug_grf_rdaddr := grf.io.debug_rdaddr
  io.debug_grf_input  := grf.io.debug_input

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

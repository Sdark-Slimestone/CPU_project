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
    val debug_stall_reason = Output(UInt(4.W))
    val debug_occupancy = Output(UInt(4.W))   // 同时有效的流水级数(F/D,D1/D2,D2/E,E/M,M/W)
    val debug_retire = Output(Bool())          // WBU 正在退休一个包(每拍 1 个)
    val debug_d_stall  = Output(Bool())        // D 级因 load-use 停(弹 0)
    val debug_redirect = Output(Bool())        // 本拍有重定向(冲刷队列)
    val debug_qempty   = Output(Bool())        // 队列不足 2 条, D 等(空泡)
    val debug_pop1     = Output(Bool())        // 本拍只弹 1 条(单发射)
    val debug_qflush   = Output(Bool())        // 队列被冲刷(EXU 重定向)
    val debug_qcnt     = Output(UInt(4.W))
    val debug_pred1    = Output(Bool())        // BTB 对 pc 命中
    val debug_pushcnt  = Output(UInt(2.W))

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
  val ifu        = Module(new IFU)
  val idu1 = Module(new idu1) // IDU1 级: 译码 + GRF 组合读
  val idu2 = Module(new idu2) // IDU2 级: 冒险检测 + lane 选通
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

  // ===================== 取指队列(解耦前端, 路线B) =====================
  val fq = Module(new FetchQueue(4))

  // ===================== 分支预测: BTFN 静态方向预测(不使用 BTB) =====================
  // 规则: 条件分支向后(imm 为负)预测 taken, 向前预测 not-taken。
  // 重定向在 IDU2 译码级做(见下方 "IDU2 BTFN 重定向"); EXU 用同一规则校验。
  // BTB 目标缓冲先屏蔽: ifu 的预测口恒不命中(顺序取指), push_max 恒 2。
  ifu.io.btb_hit1  := false.B
  ifu.io.btb_tgt1  := 0.U
  ifu.io.btb_hit2  := false.B
  ifu.io.btb_tgt2  := 0.U
  exu1.io.use_btfn := true.B
  exu2.io.use_btfn := false.B

  // IFU -> 队列
  fq.io.push_inst := ifu.io.push_inst
  fq.io.push_pc   := ifu.io.push_pc
  fq.io.push_next := ifu.io.push_next
  fq.io.push_max  := ifu.io.push_max
  ifu.io.push_cnt := fq.io.push_cnt

  // 队列队头 -> IDU1(等两条都有才发, 避免半个包)
  idu1.io.from_fq.inst1        := fq.io.out_inst(0)
  idu1.io.from_fq.inst2        := fq.io.out_inst(1)
  idu1.io.from_fq.inst1_pc     := fq.io.out_pc(0)
  idu1.io.from_fq.inst2_pc     := fq.io.out_pc(1)
  idu1.io.from_fq.inst1_nextpc := fq.io.out_next(0)
  idu1.io.from_fq.inst2_nextpc := fq.io.out_next(1)
  idu1.io.from_fq.valid        := fq.io.out_valid(1)

  // IDU1 <-> IDU2 组合相连(合并成一级 D: 译码+GRF读+转发+冲突判决)
  idu1.io.idu1_to_idu2 <> idu2.io.idu1_to_idu2

  // ===================== 流水控制(重定向 / 弹出) =====================
  // 重定向只来自 EXU: 分支实际跳/异常/mret -> 改 pc + 冲刷队列(队列里超前的包作废)
  val exu1_redirect = exu1.io.idu_to_exu1.valid &&
                      (exu1.io.exu_to_ifu.take_branch ||
                       exu1.io.csr_to_ifu.take_trap ||
                       exu1.io.csr_to_ifu.take_mret)
  val exu2_redirect = exu2.io.idu_to_exu1.valid && exu2.io.exu_to_ifu.take_branch
  val exu_redirect  = exu1_redirect || exu2_redirect
  val exu1_redirect_pc = Mux(exu1.io.csr_to_ifu.take_trap, exu1.io.csr_to_ifu.trap_pc,
                         Mux(exu1.io.csr_to_ifu.take_mret, exu1.io.csr_to_ifu.mret_pc,
                             exu1.io.exu_to_ifu.branch_target))
  val exu_redirect_pc = Mux(exu1_redirect, exu1_redirect_pc, exu2.io.exu_to_ifu.branch_target)

  // ---------- IDU2 译码级 BTFN 预测重定向 ----------
  // lane1 是"向后条件分支"时: 译码级就能算出目标(pc+imm, 不依赖寄存器), 直接改 IF pc。
  // idu2.scala 对 lane1 控制指令必单发射(向后分支 -> ctl1_need_single), 所以不会带上错误路径的 lane2。
  val idu2_l1 = idu2.io.idu1_to_idu2.bits.lane1
  val idu2_l1_is_branch = idu2_l1.op.is_beq || idu2_l1.op.is_bne || idu2_l1.op.is_blt ||
                          idu2_l1.op.is_bge || idu2_l1.op.is_bltu || idu2_l1.op.is_bgeu
  val idu2_btfn_taken = idu2_l1_is_branch && idu2_l1.imm(31)
  // 单发射(is_stall) 且当前包有效、且不是 load-use 阻塞时, 才能安全用译码级目标重定向
  val idu2_btfn_redirect = idu2.io.idu1_to_idu2.valid &&
                           idu2.io.idu_to_ifu.is_stall && !idu2.io.stall &&
                           idu2_btfn_taken
  val idu2_btfn_pc = idu2_l1.pc + idu2_l1.imm

  val redirect    = exu_redirect || idu2_btfn_redirect
  val redirect_pc = Mux(exu_redirect, exu_redirect_pc, idu2_btfn_pc)

  ifu.io.redirect    := redirect
  ifu.io.redirect_pc := redirect_pc
  fq.io.flush        := redirect

  // 队列弹出: load-use 阻塞弹 0; 包内冲突只弹 1(第二条留队列, 不重取/不冲刷); 否则弹 2
  fq.io.pop_cnt := Mux(idu2.io.stall, 0.U,
                    Mux(idu2.io.idu_to_ifu.is_stall, 1.U, 2.U))

  val flush_dexe = exu_redirect   // D2/E 桥冲刷

  // ===================== 跨包 RAW 转发来源(EXU=K1, LSU=K2, WBU=K3) =====================
  val exu1_is_csr = exu1.io.idu_to_exu1.bits.op.is_csrrw || exu1.io.idu_to_exu1.bits.op.is_csrrs ||
                    exu1.io.idu_to_exu1.bits.op.is_csrrc || exu1.io.idu_to_exu1.bits.op.is_csrrwi ||
                    exu1.io.idu_to_exu1.bits.op.is_csrrsi || exu1.io.idu_to_exu1.bits.op.is_csrrci
  val exu2_is_csr = exu2.io.idu_to_exu1.bits.op.is_csrrw || exu2.io.idu_to_exu1.bits.op.is_csrrs ||
                    exu2.io.idu_to_exu1.bits.op.is_csrrc || exu2.io.idu_to_exu1.bits.op.is_csrrwi ||
                    exu2.io.idu_to_exu1.bits.op.is_csrrsi || exu2.io.idu_to_exu1.bits.op.is_csrrci
  val exu1_is_load = exu1.io.idu_to_exu1.bits.op.is_lb || exu1.io.idu_to_exu1.bits.op.is_lh ||
                     exu1.io.idu_to_exu1.bits.op.is_lw || exu1.io.idu_to_exu1.bits.op.is_lbu ||
                     exu1.io.idu_to_exu1.bits.op.is_lhu
  val exu2_is_load = exu2.io.idu_to_exu1.bits.op.is_lb || exu2.io.idu_to_exu1.bits.op.is_lh ||
                     exu2.io.idu_to_exu1.bits.op.is_lw || exu2.io.idu_to_exu1.bits.op.is_lbu ||
                     exu2.io.idu_to_exu1.bits.op.is_lhu

  // K=1: EXU(ALU 结果本拍可得; load 数据没好 -> is_load 让 IDU2 阻塞)
  idu2.io.fwd.exu_rd(0)      := exu1.io.idu_to_exu1.bits.rd
  idu2.io.fwd.exu_valid(0)   := exu1.io.idu_to_exu1.valid
  idu2.io.fwd.exu_is_load(0) := exu1_is_load
  idu2.io.fwd.exu_data(0)    := Mux(exu1_is_csr, exu1.io.csr_to_grf.wdata, exu1.io.exu_to_lsu.bits.wb_data)
  idu2.io.fwd.exu_rd(1)      := exu2.io.idu_to_exu1.bits.rd
  idu2.io.fwd.exu_valid(1)   := exu2.io.idu_to_exu1.valid
  idu2.io.fwd.exu_is_load(1) := exu2_is_load
  idu2.io.fwd.exu_data(1)    := Mux(exu2_is_csr, exu2.io.csr_to_grf.wdata, exu2.io.exu_to_lsu.bits.wb_data)

  // K=2: LSU(含 load 数据)
  idu2.io.fwd.lsu_rd(0)    := lsu1.io.lsu_to_wbu.bits.rd
  idu2.io.fwd.lsu_valid(0) := lsu1.io.lsu_to_wbu.valid
  idu2.io.fwd.lsu_data(0)  := lsu1.io.lsu_to_wbu.bits.grf_wb_data
  idu2.io.fwd.lsu_rd(1)    := lsu2.io.lsu_to_wbu.bits.rd
  idu2.io.fwd.lsu_valid(1) := lsu2.io.lsu_to_wbu.valid
  idu2.io.fwd.lsu_data(1)  := lsu2.io.lsu_to_wbu.bits.grf_wb_data

  // K=3: WBU(M/W 寄存器的写回值)
  idu2.io.fwd.wbu_rd(0)    := wbu.io.lsu_to_wbu_1.bits.rd
  idu2.io.fwd.wbu_valid(0) := wbu.io.lsu_to_wbu_1.valid
  idu2.io.fwd.wbu_data(0)  := wbu.io.lsu_to_wbu_1.bits.grf_wb_data
  idu2.io.fwd.wbu_rd(1)    := wbu.io.lsu_to_wbu_2.bits.rd
  idu2.io.fwd.wbu_valid(1) := wbu.io.lsu_to_wbu_2.valid
  idu2.io.fwd.wbu_data(1)  := wbu.io.lsu_to_wbu_2.bits.grf_wb_data

  // ===================== EXU 操作数口的 MEM→EX bypass =====================
  exu1.io.bypass.lsu_rd(0)    := lsu1.io.lsu_to_wbu.bits.rd
  exu1.io.bypass.lsu_valid(0) := lsu1.io.lsu_to_wbu.valid
  exu1.io.bypass.lsu_data(0)  := lsu1.io.lsu_to_wbu.bits.grf_wb_data
  exu1.io.bypass.lsu_rd(1)    := lsu2.io.lsu_to_wbu.bits.rd
  exu1.io.bypass.lsu_valid(1) := lsu2.io.lsu_to_wbu.valid
  exu1.io.bypass.lsu_data(1)  := lsu2.io.lsu_to_wbu.bits.grf_wb_data
  exu2.io.bypass.lsu_rd(0)    := lsu1.io.lsu_to_wbu.bits.rd
  exu2.io.bypass.lsu_valid(0) := lsu1.io.lsu_to_wbu.valid
  exu2.io.bypass.lsu_data(0)  := lsu1.io.lsu_to_wbu.bits.grf_wb_data
  exu2.io.bypass.lsu_rd(1)    := lsu2.io.lsu_to_wbu.bits.rd
  exu2.io.bypass.lsu_valid(1) := lsu2.io.lsu_to_wbu.valid
  exu2.io.bypass.lsu_data(1)  := lsu2.io.lsu_to_wbu.bits.grf_wb_data

  // 包内前递: exu1(lane1) 的结果 -> exu2(lane2) 操作数(lane1 是 load 时不发, 走单发射)
  exu1.io.intra_fwd.rd    := 0.U
  exu1.io.intra_fwd.data  := 0.U
  exu1.io.intra_fwd.valid := false.B
  exu2.io.intra_fwd.rd    := exu1.io.exu_to_lsu.bits.rd
  exu2.io.intra_fwd.data  := exu1.io.exu_to_lsu.bits.wb_data
  exu2.io.intra_fwd.valid := exu1.io.idu_to_exu1.valid && !exu1_is_load

  // BTFN 只给 lane1 用; lane2 按 not-taken, 跳了由 exu2 自己重定向
  // 解耦前端: D 不再做 BTFN 预测重定向, 分支实际跳了才由 EXU 重定向(会冲刷队列)


  // lane1 真的重定向(向前分支实际跳了/trap/mret) 时, 同包 lane2 属错误路径 -> kill
  exu1.io.kill := false.B
  exu2.io.kill := exu1_redirect

  // ===================== IDU2 ↔ GRF =====================
  idu2.io.idu_to_grf <> grf.io.idu_to_grf
  grf.io.grf_to_idu <> idu2.io.grf_to_idu

  /* 原接线保留
  // ===================== IDU → EXU1 =====================
  idu.io.idu_to_exu1 <> exu1.io.idu_to_exu1

  // ===================== IDU → EXU2 =====================
  exu2.io.idu_to_exu1.dec1_op  := idu.io.idu_to_exu2.dec2_op
  exu2.io.idu_to_exu1.dec1_imm := idu.io.idu_to_exu2.dec2_imm
  exu2.io.idu_to_exu1.dec1_val := idu.io.idu_to_exu2.dec2_val
  exu2.io.idu_to_exu1.dec1_rd  := idu.io.idu_to_exu2.dec2_rd
  exu2.io.idu_to_exu1.is_stall := false.B
  exu2.io.idu_to_exu1.inst1_pc := 0.U
  exu2.io.idu_to_exu1.inst1    := 0.U
  原接线保留结束 */

  // ===================== IDU2 → [D/E流水桥] → EXU1/EXU2 =====================
  StagePipe(idu2.io.idu_to_exu1, exu1.io.idu_to_exu1, false.B, flush_dexe)
  StagePipe(idu2.io.idu_to_exu2, exu2.io.idu_to_exu1, false.B, flush_dexe)

  // ===================== CSR 写回 GRF =====================
  grf.io.csr_to_grf.wen   := exu1.io.csr_to_grf.wen
  grf.io.csr_to_grf.waddr := exu1.io.csr_to_grf.waddr
  grf.io.csr_to_grf.wdata := exu1.io.csr_to_grf.wdata

  /* 原接线保留
  // ===================== EXU1/2 → LSU1/2 =====================
  exu1.io.exu_to_lsu <> lsu1.io.exu_to_lsu
  exu2.io.exu_to_lsu <> lsu2.io.exu_to_lsu

  // ===================== LSU1/2 → WBU =====================
  wbu.io.lsu_to_wbu_1.rd          := lsu1.io.lsu_to_wbu.rd
  wbu.io.lsu_to_wbu_1.grf_wb_data := lsu1.io.lsu_to_wbu.grf_wb_data
  wbu.io.lsu_to_wbu_2.rd          := lsu2.io.lsu_to_wbu.rd
  wbu.io.lsu_to_wbu_2.grf_wb_data := lsu2.io.lsu_to_wbu.grf_wb_data
  原接线保留结束 */

  // ===================== EXU1/2 → [E/M流水桥] → LSU1/2 =====================
  StagePipe(exu1.io.exu_to_lsu, lsu1.io.exu_to_lsu, false.B, false.B)
  StagePipe(exu2.io.exu_to_lsu, lsu2.io.exu_to_lsu, false.B, false.B)

  // ===================== LSU1/2 → [M/W流水桥] → WBU =====================
  StagePipe(lsu1.io.lsu_to_wbu, wbu.io.lsu_to_wbu_1, false.B, false.B)
  StagePipe(lsu2.io.lsu_to_wbu, wbu.io.lsu_to_wbu_2, false.B, false.B)

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

  // ===================== 所有 debug 连接 =====================
  // --- IFU ---
  io.debug_inst1_pc := ifu.io.debug.debug_inst1_pc
  io.debug_inst2_pc := ifu.io.debug.debug_inst2_pc

  // --- IDU2(冒险检测后) ---
  io.debug_inst1 := idu2.io.idu_debug.debug_inst1
  io.debug_inst2 := idu2.io.idu_debug.debug_inst2
  io.debug_stall := idu2.io.idu_debug.is_stall
  io.debug_stall_reason := idu2.io.idu_debug.reason
  io.debug_occupancy := fq.io.out_valid(1).asUInt +& idu2.io.idu1_to_idu2.valid.asUInt +&
                        exu1.io.idu_to_exu1.valid.asUInt +& lsu1.io.exu_to_lsu.valid.asUInt +&
                        wbu.io.lsu_to_wbu_1.valid.asUInt
  io.debug_retire := wbu.io.lsu_to_wbu_1.valid
  io.debug_d_stall  := idu2.io.stall
  io.debug_redirect := exu_redirect
  io.debug_qempty   := !fq.io.out_valid(1)
  io.debug_pop1     := (fq.io.pop_cnt === 1.U) && fq.io.out_valid(1)
  io.debug_qflush   := fq.io.flush
  io.debug_qcnt     := fq.io.count
  io.debug_pred1    := ifu.io.btb_hit1
  io.debug_pushcnt  := fq.io.push_cnt

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
package R1322IAeCSR

import chisel3._
import chisel3.util._
import chisel3.experimental.IntParam
import _root_.circt.stage.ChiselStage

// 双端口黑盒存储器, 通过 DPI-C 与 C++ 交互。
// 读口为 SimpleBus: reqValid 发请求, respValid 响应有效(保持到 respReady), respReady 接收。
// latency = 读延迟拍 (对应 Verilog 的 LATENCY 参数)。
class DPIMemory(val latency: Int = 1) extends BlackBox(Map("LATENCY" -> IntParam(latency))) {
  val io = IO(new Bundle {
    val io_clk = Input(Clock())
    val wen    = Input(Bool())
    val waddr  = Input(UInt(32.W))
    val wdata  = Input(UInt(32.W))
    val wmask  = Input(UInt(4.W))
    val reqValid1 = Input(Bool())
    val raddr1    = Input(UInt(32.W))
    val rdata1    = Output(UInt(32.W))
    val respValid1 = Output(Bool())
    val respReady1 = Input(Bool())
    val reqValid2 = Input(Bool())
    val raddr2    = Input(UInt(32.W))
    val rdata2    = Output(UInt(32.W))
    val respValid2 = Output(Bool())
    val respReady2 = Input(Bool())
    val flush  = Input(Bool())
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
    val debug_exu_ctl      = Output(Bool())   // 到达 EXU 的控制流指令(分支/跳转)
    val debug_exu_mispred  = Output(Bool())   // 方向预测错
    val debug_exu_jalr     = Output(Bool())
    val debug_exu_jal      = Output(Bool())
    val debug_exu_br       = Output(Bool())
    // 控制流分类 (0=无 1=条件分支 2=jal 3=jalr返回 4=jalr非返回) + 目标校验分类
    val debug_ctl_class    = Output(UInt(3.W))
    val debug_ctl_lane2    = Output(Bool())   // 该控制流在 lane2 (ctlUse2)
    val debug_exu_tgt_mis  = Output(Bool())   // 方向对但目标错
    val debug_tgtmis_class = Output(UInt(3.W))
    // 条件分支方向诊断
    val debug_exu_actual      = Output(Bool())      // 实际方向 (taken)
    val debug_exu_pc          = Output(UInt(32.W))  // 选中控制流 PC
    val debug_exu_gsh_pred    = Output(Bool())      // gshare 分量预测
    val debug_exu_loc_pred    = Output(Bool())      // 局部分量预测
    val debug_exu_static_pred = Output(Bool())      // 静态分量预测 (后向=1)
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
    val debug_grf_regs   = Output(Vec(32, UInt(32.W)))
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
  val btb   = Module(new BTB(64))
  val tourn = Module(new TournamentPredictor)
  val ipred = Module(new IndirectPredictor)
  val lc    = Module(new LoopCounter)
  val mdu   = Module(new MDU)
  val sb   = Module(new StoreBuffer)

  // ===================== IFU <-> IMEM =====================
  imem.io.ifu_raddr  := ifu.io.ifu_raddr
  imem.io.ifu_raddr2 := ifu.io.ifu_raddr2
  imem.io.ifu_reqValid  := ifu.io.ifu_reqValid
  imem.io.ifu_respReady := ifu.io.ifu_respReady
  ifu.io.ifu_rdata   := imem.io.ifu_rdata
  ifu.io.ifu_rdata2  := imem.io.ifu_rdata2
  ifu.io.ifu_respValid := imem.io.ifu_respValid

  // ===================== 地址预测 (BTB) 查表 =====================
  btb.io.if_pc  := ifu.io.look_pc1
  btb.io.if_pc2 := ifu.io.look_pc2
  ifu.io.btb_hit1    := btb.io.if_hit
  ifu.io.btb_tgt1    := btb.io.if_tgt
  ifu.io.btb_return1 := btb.io.if_return
  ifu.io.btb_isjal1  := btb.io.if_isjal
  ifu.io.btb_hit2    := btb.io.if_hit2
  ifu.io.btb_tgt2    := btb.io.if_tgt2
  ifu.io.btb_return2 := btb.io.if_return2
  ifu.io.btb_isjal2  := btb.io.if_isjal2

  // ===================== 间接预测 (RAS + IBTB) 查表 =====================
  ipred.io.if_pc  := ifu.io.look_pc1
  ipred.io.if_pc2 := ifu.io.look_pc2
  ifu.io.ibtb_hit1 := ipred.io.if_ibtb_hit
  ifu.io.ibtb_tgt1 := ipred.io.if_ibtb_tgt
  ifu.io.ibtb_hit2 := ipred.io.if_ibtb_hit2
  ifu.io.ibtb_tgt2 := ipred.io.if_ibtb_tgt2
  ifu.io.ras_top   := ipred.io.ras_top
  ifu.io.ras_valid := ipred.io.ras_valid

  // ===================== 循环计数器 查表 =====================
  lc.io.if_pc  := ifu.io.look_pc1
  lc.io.if_pc2 := ifu.io.look_pc2
  ifu.io.lc_hit1   := lc.io.if_hit
  ifu.io.lc_taken1 := lc.io.if_taken
  ifu.io.lc_hit2   := lc.io.if_hit2
  ifu.io.lc_taken2 := lc.io.if_taken2

  // ===================== 方向预测 (三选一锦标赛) 查表 =====================
  tourn.io.if_pc      := ifu.io.look_pc1
  tourn.io.if_pc2     := ifu.io.look_pc2
  tourn.io.if_static  := btb.io.if_static    // 静态分量来自 BTB
  tourn.io.if_static2 := btb.io.if_static2
  ifu.io.dir_taken1   := tourn.io.if_dir
  ifu.io.dir_gsh1     := tourn.io.if_gsh
  ifu.io.dir_loc1     := tourn.io.if_loc
  ifu.io.dir_gshIdx1  := tourn.io.if_gshIdx
  ifu.io.dir_locHist1 := tourn.io.if_locHist
  ifu.io.dir_taken2   := tourn.io.if_dir2
  ifu.io.dir_gsh2     := tourn.io.if_gsh2
  ifu.io.dir_loc2     := tourn.io.if_loc2
  ifu.io.dir_gshIdx2  := tourn.io.if_gshIdx2
  ifu.io.dir_locHist2 := tourn.io.if_locHist2

  // ===================== 流水控制: 重定向 / 冻结 (选中控制 lane) =====================
  // 每对最多一条 branch/jump (系统指令单发射恒在 lane1); 选中它做重定向与预测器更新。
  val e1 = exu1.io.idu_to_exu1
  val e2 = exu2.io.idu_to_exu1
  val lane1_ctl = e1.valid && (e1.bits.ctrl.is_branch || e1.bits.ctrl.is_jump)
  val lane2_ctl = e2.valid && (e2.bits.ctrl.is_branch || e2.bits.ctrl.is_jump)
  val ctlUse2   = lane2_ctl && !lane1_ctl

  val e_ctrl        = Mux(ctlUse2, e2.bits.ctrl, e1.bits.ctrl)
  val e_pred_taken  = Mux(ctlUse2, e2.bits.pred.taken, e1.bits.pred.taken)
  val e_pred_target = Mux(ctlUse2, e2.bits.pred.pred_target, e1.bits.pred.pred_target)
  val e_pc          = Mux(ctlUse2, e2.bits.pc, e1.bits.pc)
  val e_rd          = Mux(ctlUse2, e2.bits.rd, e1.bits.rd)
  val e_rs1         = Mux(ctlUse2, e2.bits.rs1_addr, e1.bits.rs1_addr)
  val e_imm         = Mux(ctlUse2, e2.bits.imm, e1.bits.imm)
  val e_actual      = Mux(ctlUse2, exu2.io.exu_to_ifu.take_branch, exu1.io.exu_to_ifu.take_branch)
  val e_bt          = Mux(ctlUse2, exu2.io.exu_to_ifu.branch_target, exu1.io.exu_to_ifu.branch_target)
  val e_ctl_valid   = lane1_ctl || lane2_ctl

  val exu_mispred  = e_ctl_valid && (e_actual =/= e_pred_taken)
  // trap/mret 只在 lane1 (系统指令单发射)
  val exu_redirect = exu_mispred ||
                     (exu1.io.idu_to_exu1.valid &&
                      (exu1.io.csr_to_ifu.take_trap || exu1.io.csr_to_ifu.take_mret))
  // 重定向各分量分别寄存, 目标 mux 移到应用拍 (从寄存器出发), 避开 E 关键路径。
  val exu_redirect_r    = RegNext(exu_redirect, false.B)
  val exu_trap_r        = RegNext(exu1.io.csr_to_ifu.take_trap, false.B)
  val exu_trap_pc_r     = RegNext(exu1.io.csr_to_ifu.trap_pc, 0.U(32.W))
  val exu_mret_r        = RegNext(exu1.io.csr_to_ifu.take_mret, false.B)
  val exu_mret_pc_r     = RegNext(exu1.io.csr_to_ifu.mret_pc, 0.U(32.W))
  val exu_actual_r      = RegNext(e_actual, false.B)
  val exu_bt_r          = RegNext(e_bt, 0.U(32.W))
  val exu_pc4_r         = RegNext(e_pc + 4.U, 0.U(32.W))
  // 常见情形 (分支实际 taken -> branch_target) 放最外层; trap/mret 的 actual 恒为 0。
  val exu_redirect_pc_r = Mux(exu_actual_r, exu_bt_r,
                          Mux(exu_trap_r, exu_trap_pc_r,
                          Mux(exu_mret_r, exu_mret_pc_r, exu_pc4_r)))

  // 目标校验 (RAS/IBTB 目标可能失效): E 级寄存后下一拍组合比较, 直接作重定向源。
  val exu_ptk_r   = RegNext(e_pred_taken, false.B)
  val exu_pt_r    = RegNext(e_pred_target, 0.U(32.W))
  val exu_dirok_r = RegNext(e_actual === e_pred_taken, false.B)
  val exu_valid_r = RegNext(e_ctl_valid, false.B)
  val tgt_mis     = exu_valid_r && exu_ptk_r && exu_dirok_r && (exu_bt_r =/= exu_pt_r)

  // lane1 为 branch/jump 且 (方向预测错 或 目标错) -> 取指沿预测路径取来的 lane2 是错路径,
  // 杀其 M/W (store 在下面门控)。注: 不能只看 "lane1 taken" —— 预测 taken 时 lane2 恰是目标(正确)。
  val lane1_ctl_r  = RegNext(lane1_ctl, false.B)
  val lane1_misp_r = RegNext(lane1_ctl && (exu1.io.exu_to_ifu.take_branch =/= e1.bits.pred.taken), false.B)
  val kill2        = lane1_misp_r || (lane1_ctl_r && tgt_mis)

  // 同拍 kill (组合): lane1 控制流在 E 级就决议, 该拍 lane2 (若在 E) 可能是错路径。
  // 用来门控 lane2 的 E 级 load 读 (越界读会让仿真报 BAD TRAP); 寄存版 kill2 管 M 级 store/写回。
  val lane1_dir_mis_now = lane1_ctl && (exu1.io.exu_to_ifu.take_branch =/= e1.bits.pred.taken)
  val lane1_tgt_mis_now = lane1_ctl && e1.bits.pred.taken &&
                          (exu1.io.exu_to_ifu.take_branch === e1.bits.pred.taken) &&
                          exu1.io.exu_to_ifu.take_branch &&
                          (exu1.io.exu_to_ifu.branch_target =/= e1.bits.pred.pred_target)
  val kill2_now = lane1_dir_mis_now || lane1_tgt_mis_now

  // D/E 冲刷 (flush_de): 重定向拍之后清掉 D/E 里的错路径指令。
  // L>1: 若重定向源 (E 级控制指令) 因访存冻结仍滞留在 E, 则延后冲刷到它离开 E 再清,
  //      否则寄存化的 flush 会把重定向源 (以及同拍更老的 lane) 一起冲掉。
  //      用 D/E 寄存器原始输出 (e1_out/e2_out, 未经 kill) 判断, 避免组合环。
  // L=1: 沿用原行为 (逐位一致)。
  val e1_out = Wire(Decoupled(new IDU_to_EXU_Lane_Message))
  val e2_out = Wire(Decoupled(new IDU_to_EXU_Lane_Message))
  val redir_src_pc  = RegNext(e_pc, 0.U(32.W))
  val e_holds_src   = (e1_out.valid && (e1_out.bits.pc === redir_src_pc)) ||
                      (e2_out.valid && (e2_out.bits.pc === redir_src_pc))
  val flush_pending = RegInit(false.B)
  val flush_de: Bool =
    if (MemCfg.latency <= 1) exu_redirect_r || tgt_mis
    else (flush_pending || tgt_mis) && !e_holds_src
  when (exu_redirect || (tgt_mis && e_holds_src)) { flush_pending := true.B }
  .elsewhen (flush_de)                           { flush_pending := false.B }
  // 目标校验拍错路径指令仍在 E 且 valid, 会更新预测器; 抑制 RAS/IBTB 更新。
  // (方向预测错的错路径指令由 fwd.kill=flush_de 直接清零 valid, e_ctl_valid=0, 已天然挡住)
  val upd_ok = !tgt_mis

  // ===================== 多周期乘除单元 (RV32M) =====================
  // M 指令恒单发射 (IDU 保证在 lane1)。E 级检测到 M 指令且 MDU 空闲时启动;
  // 运算期间停住 D/E 与前端 (mdu_stall), EXU 向下游插空泡 (见 EXU.stall);
  // done 那拍放行, 结果经 EXU 写回通路进 M/W。
  val e1_md     = e1.valid && e1.bits.ctrl.is_muldiv
  val e1_is_div = e1.valid && !e1.bits.ctrl.is_mul_op
  mdu.io.start  := e1_md && mdu.io.idle
  mdu.io.cancel := flush_de
  mdu.io.is_div := e1_is_div
  mdu.io.op     := e1.bits.inst(14, 12)
  mdu.io.a      := e1.bits.rs1_val
  mdu.io.b      := e1.bits.rs2_val
  val mdu_stall = (e1_md && mdu.io.idle) || (mdu.io.busy && !mdu.io.done)
  exu1.io.mdu_result := mdu.io.out
  exu1.io.mdu_sel    := e1_md
  exu2.io.mdu_result := 0.U(32.W)
  exu2.io.mdu_sel    := false.B

  // ===================== LSU load 等待 (SimpleBus 延迟) =====================
  // 延迟 L>1 时读数据要 L 拍才回; 把 load 在 E 级停 L-1 拍 (EXU 持续读同地址, 数据稳定后放行)。
  // 用 holding/cnt 状态机按"每次 load 进入 E"计时, 处理背靠背 load。L=1 时 mem_stall 恒 0。
  val e_load = (e1.valid && e1.bits.ctrl.is_load) || (e2.valid && e2.bits.ctrl.is_load)
  val lHold = RegInit(false.B)
  val lCnt  = RegInit(0.U(5.W))
  val holdSet: Bool = if (MemCfg.latency > 1) e_load else false.B
  when (lHold) {
    when (lCnt === 0.U) { lHold := false.B } .otherwise { lCnt := lCnt - 1.U }
  } .elsewhen (holdSet) {
    lHold := true.B
    lCnt  := math.max(MemCfg.latency - 2, 0).U
  }
  val mem_stall_raw: Bool =
    if (MemCfg.latency > 1) (e_load && !lHold) || (lHold && (lCnt =/= 0.U))
    else false.B
  val mem_stall: Bool = mem_stall_raw
  val pipe_stall = mdu_stall || mem_stall
  exu1.io.stall := pipe_stall
  exu2.io.stall := pipe_stall

  // ===================== RA 别名追踪 (RAS 一致性) =====================
  // 编译器返回惯例: mv t0,ra; ...; jalr x0,0(t0) —— rs1 不是 ra, 但持有 ra 的副本。
  // 维护 raAlias[r]: 该寄存器当前是否持有 ra 的副本 (含经 mv 传递)。E 级按序更新:
  //   - mv rd,rs (=addi rd,rs,0) 且 (rs==ra 或 raAlias[rs]) -> raAlias[rd]=1
  //   - 任何其它写 rd 的指令 -> raAlias[rd]=0
  // 注: 同拍 lane1->lane2 的传递未处理 (实际 libgcc 模式中间隔着 jal)。
  val raAlias = RegInit(VecInit(Seq.fill(32)(false.B)))
  def raCopyLane(valid: Bool, ctrl: CtrlSignals, rd: UInt, rs1: UInt, imm: UInt): Bool =
    valid && ctrl.is_addi && (imm === 0.U) && (rd =/= 0.U) && ((rs1 === 1.U) || raAlias(rs1))
  val raW1 = e1.valid && e1.bits.ctrl.writes_rd && (e1.bits.rd =/= 0.U)
  val raW2 = e2.valid && e2.bits.ctrl.writes_rd && (e2.bits.rd =/= 0.U)
  val raA1 = raCopyLane(e1.valid, e1.bits.ctrl, e1.bits.rd, e1.bits.rs1_addr, e1.bits.imm)
  val raA2 = raCopyLane(e2.valid, e2.bits.ctrl, e2.bits.rd, e2.bits.rs1_addr, e2.bits.imm)
  for (i <- 0 until 32) {
    raAlias(i) := Mux(raW2 && (e2.bits.rd === i.U), raA2,
                   Mux(raW1 && (e1.bits.rd === i.U), raA1, raAlias(i)))
  }

  // 控制指令分类 (调用/返回/间接)
  // 返回: jalr rd=0, 且 rs1==ra 或 rs1 是 ra 的别名 (编译器返回惯例)。
  val e_is_return   = e_ctrl.is_jalr && (e_rd === 0.U) && ((e_rs1 === 1.U) || raAlias(e_rs1))
  val e_is_call     = (e_ctrl.is_jal && (e_rd === 1.U)) || (e_ctrl.is_jalr && (e_rd === 1.U))
  val e_is_indirect = e_ctrl.is_jalr && !e_is_return

  // 控制流分类 (prof 用): 0=无 1=条件分支 2=jal 3=jalr返回 4=jalr非返回
  val ctl_class = MuxCase(0.U(3.W), Seq(
    e_ctrl.is_branch -> 1.U,
    e_ctrl.is_jal    -> 2.U,
    e_is_return      -> 3.U,
    e_is_indirect    -> 4.U
  ))

  // 地址预测更新 (条件分支 + jal + 返回 jalr; 非返回 jalr 交给 IBTB)
  btb.io.upd_valid     := e_ctl_valid && (e_ctrl.is_branch || e_ctrl.is_jal || e_is_return)
  btb.io.upd_pc        := e_pc
  btb.io.upd_taken     := e_actual
  btb.io.upd_target    := e_bt
  btb.io.upd_static    := e_ctrl.is_jal || (e_ctrl.is_branch && e_imm(31))
  btb.io.upd_is_return := e_is_return
  btb.io.upd_is_jal    := e_ctrl.is_jal

  // 间接预测更新 (RAS 压/弹 + IBTB 训练)
  ipred.io.upd_valid       := upd_ok && e_ctl_valid && (e_ctrl.is_jal || e_ctrl.is_jalr)
  ipred.io.upd_pc          := e_pc
  ipred.io.upd_target      := e_bt
  ipred.io.upd_is_call     := e_is_call
  ipred.io.upd_is_return   := e_is_return
  ipred.io.upd_is_indirect := e_is_indirect

  // 循环计数器更新 (仅回边条件分支)
  lc.io.upd_valid := e_ctl_valid && e_ctrl.is_branch && e_imm(31)
  lc.io.upd_pc    := e_pc
  lc.io.upd_taken := e_actual

  // 方向预测更新 (仅条件分支; GHR 决议时更新)
  tourn.io.upd_valid       := e_ctl_valid && e_ctrl.is_branch
  tourn.io.upd_pc          := e_pc
  tourn.io.upd_taken       := e_actual
  tourn.io.upd_dir_pred    := Mux(ctlUse2, e2.bits.pred.dir_taken, e1.bits.pred.dir_taken)
  tourn.io.upd_gsh_pred    := Mux(ctlUse2, e2.bits.pred.gsh_pred, e1.bits.pred.gsh_pred)
  tourn.io.upd_loc_pred    := Mux(ctlUse2, e2.bits.pred.loc_pred, e1.bits.pred.loc_pred)
  tourn.io.upd_static_pred := e_imm(31)
  tourn.io.upd_gsh_idx     := Mux(ctlUse2, e2.bits.pred.gsh_idx, e1.bits.pred.gsh_idx)
  tourn.io.upd_loc_hist    := Mux(ctlUse2, e2.bits.pred.loc_hist, e1.bits.pred.loc_hist)

  // 注: 试过把 IDU 兜底重定向也寄存器化 (切 FQ->译码->重定向->imem 长链), 但最坏路径
  // 只是被顶到 exu_pc4_r 上 (整体均匀贴限), fmax 836->822 反而变差且 +1 拍/重定向, 已回退。
  val redirect    = exu_redirect_r || tgt_mis || idu.io.pred_redirect
  val redirect_pc = Mux(exu_redirect_r, exu_redirect_pc_r,
                    Mux(tgt_mis, exu_bt_r, idu.io.pred_redirect_pc))

  ifu.io.redirect    := redirect
  ifu.io.redirect_pc := redirect_pc
  // 重定向冲掉取指在途读 (取指超前于重定向点); dmem 的 load 在 E/M 更老, 不能冲。
  imem.io.ifu_flush  := redirect
  dmem.io.flush      := false.B

  val d_stall = idu.io.stall
  idu.io.mdu_stall := pipe_stall

  // ===================== 取指队列 (IF <-> IDU 解耦, 按条弹出) =====================
  val fq = Module(new FetchQueue(4))
  fq.io.push_valid  := ifu.io.push_valid
  fq.io.push_num    := ifu.io.push_num
  fq.io.push_inst   := ifu.io.push_inst
  fq.io.push_pc     := ifu.io.push_pc
  fq.io.push_pred   := ifu.io.push_pred
  ifu.io.push_ready := fq.io.push_ready

  idu.io.from_fq.inst1 := fq.io.out_inst(0)
  idu.io.from_fq.inst2 := fq.io.out_inst(1)
  idu.io.from_fq.pc1   := fq.io.out_pc(0)
  idu.io.from_fq.pc2   := fq.io.out_pc(1)
  idu.io.from_fq.pred  := fq.io.out_pred
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
  idu.io.fwd.lsu_rd(0)       := lsu1.io.exu_to_lsu.bits.rd
  idu.io.fwd.lsu_is_load(0)  := lsu1.io.exu_to_lsu.bits.op.isLoad
  idu.io.fwd.lsu_valid(1)    := lsu2.io.exu_to_lsu.valid
  idu.io.fwd.lsu_is_store(1) := lsu2.io.exu_to_lsu.bits.op.isStore
  idu.io.fwd.lsu_rd(1)       := lsu2.io.exu_to_lsu.bits.rd
  idu.io.fwd.lsu_is_load(1)  := lsu2.io.exu_to_lsu.bits.op.isLoad

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
  // D/E 寄存器在因访存延迟冻结 (pipe_stall) 时, 会顺带用转发结果刷新操作数 (eRefresh):
  // 否则冻结期间 M/W 级生产者流出会让 E 级已冻结指令的操作数变陈旧。
  ERefreshPipe(idu.io.idu_to_exu1, e1_out, fwd1.io.out.bits, pipe_stall, flush_de)
  ERefreshPipe(idu.io.idu_to_exu2, e2_out, fwd2.io.out.bits, pipe_stall, flush_de)
  fwd1.io.in <> e1_out
  fwd1.io.kill := flush_de
  fwd2.io.in <> e2_out
  fwd2.io.kill := flush_de
  fwd1.io.out <> exu1.io.idu_to_exu1
  // 注: 曾试过包内 lane1->lane2 转发 (exu1.alu_out 直接旁路给 exu2), IPC 1.09->1.22,
  // 但 E 级 ALU->ALU 串联使 fmax 从 868 掉到 605 MHz, 运行时间反升 25%, 已回退。
  // 需先把转发 mux 挪到读级/拆开 (见文档"后续") 再启用。
  fwd2.io.out <> exu2.io.idu_to_exu1

  // ===================== EXU1/2 -> [E/M 流水寄存器] -> LSU1/2 =====================
  // M 级目标重定向时冲刷 E/M, 杀掉错路径的 M 指令
  StagePipe(exu1.io.exu_to_lsu, lsu1.io.exu_to_lsu, false.B, false.B)
  // lane1 分支 taken -> lane2 错路径, 冲刷其 E/M
  StagePipe(exu2.io.exu_to_lsu, lsu2.io.exu_to_lsu, false.B, kill2)

  // ===================== LSU1/2 -> [M/W 流水寄存器] -> WBU =====================
  // M 级目标重定向时冲刷 M/W, 阻止 M 级错路径指令写回
  StagePipe(lsu1.io.lsu_to_wbu, wbu.io.lsu_to_wbu_1, false.B, false.B)
  // lane1 分支 taken -> lane2 错路径, 冲刷其 M/W (阻止错路径写回)
  StagePipe(lsu2.io.lsu_to_wbu, wbu.io.lsu_to_wbu_2, false.B, kill2)

  // ===================== LSU1/2 <-> DMEM =====================
  // 读请求 (E 级) 由 EXU 直接发给 dmem; 写请求 (M 级) 由 LSU 发给 dmem
  dmem.io.exu_to_dmem_1.addr := exu1.io.exu_to_lsu.bits.addr
  dmem.io.exu_to_dmem_1.ren  := exu1.io.exu_to_lsu.bits.op.isLoad
  dmem.io.exu_to_dmem_2.addr := exu2.io.exu_to_lsu.bits.addr
  // lane1 控制流同拍误预测 -> lane2 是错路径, 门控其 E 级 load 读 (防越界读触发 BAD TRAP)。
  dmem.io.exu_to_dmem_2.ren  := exu2.io.exu_to_lsu.bits.op.isLoad && !kill2_now
  dmem.io.lsu_to_dmem_1.addr       := lsu1.io.lsu_addr
  dmem.io.lsu_to_dmem_1.store_data := lsu1.io.lsu_wdata
  dmem.io.lsu_to_dmem_1.mask       := lsu1.io.lsu_wmask
  dmem.io.lsu_to_dmem_1.wen        := lsu1.io.lsu_wen
  // lane2 store 在 lane1 分支 taken 时是错路径, 门控其写使能 (避免错误写内存)
  val m2_wen = lsu2.io.lsu_wen && !kill2
  dmem.io.lsu_to_dmem_2.addr       := lsu2.io.lsu_addr
  dmem.io.lsu_to_dmem_2.store_data := lsu2.io.lsu_wdata
  dmem.io.lsu_to_dmem_2.mask       := lsu2.io.lsu_wmask
  dmem.io.lsu_to_dmem_2.wen        := m2_wen
  dmem.io.ebreak := lsu1.io.ebreak_out || lsu2.io.ebreak_out

  // ===================== StoreBuffer (store->load 转发) =====================
  // 压入 M 级 store (lane1 优先), 查询 E 级 load 地址, 合并内存读值
  sb.io.push_valid := lsu1.io.lsu_wen || m2_wen
  sb.io.push_addr  := Mux(lsu1.io.lsu_wen, lsu1.io.lsu_addr,  lsu2.io.lsu_addr)
  sb.io.push_data  := Mux(lsu1.io.lsu_wen, lsu1.io.lsu_wdata, lsu2.io.lsu_wdata)
  sb.io.push_mask  := Mux(lsu1.io.lsu_wen, lsu1.io.lsu_wmask, lsu2.io.lsu_wmask)
  sb.io.q1_addr := exu1.io.exu_to_lsu.bits.addr
  sb.io.q1_ren  := exu1.io.exu_to_lsu.bits.op.isLoad
  sb.io.q2_addr := exu2.io.exu_to_lsu.bits.addr
  sb.io.q2_ren  := exu2.io.exu_to_lsu.bits.op.isLoad
  sb.io.same_valid := lsu1.io.lsu_wen
  sb.io.same_addr  := lsu1.io.lsu_addr
  sb.io.same_data  := lsu1.io.lsu_wdata
  sb.io.same_mask  := lsu1.io.lsu_wmask
  sb.io.mem1 := dmem.io.dmem_rdata_1
  sb.io.mem2 := dmem.io.dmem_rdata_2
  lsu1.io.lsu_rdata := sb.io.ld1
  lsu2.io.lsu_rdata := sb.io.ld2

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
  io.debug_exu_ctl      := e_ctl_valid
  io.debug_exu_mispred  := exu_mispred
  io.debug_exu_jalr     := e_ctl_valid && e_ctrl.is_jalr
  io.debug_exu_jal      := e_ctl_valid && e_ctrl.is_jal
  io.debug_exu_br       := e_ctl_valid && e_ctrl.is_branch
  io.debug_ctl_class    := Mux(e_ctl_valid, ctl_class, 0.U(3.W))
  io.debug_ctl_lane2    := ctlUse2
  io.debug_exu_actual      := e_actual
  io.debug_exu_pc          := e_pc
  io.debug_exu_gsh_pred    := Mux(ctlUse2, e2.bits.pred.gsh_pred, e1.bits.pred.gsh_pred)
  io.debug_exu_loc_pred    := Mux(ctlUse2, e2.bits.pred.loc_pred, e1.bits.pred.loc_pred)
  io.debug_exu_static_pred := e_imm(31)
  io.debug_exu_tgt_mis  := tgt_mis
  io.debug_tgtmis_class := RegNext(Mux(e_ctl_valid, ctl_class, 0.U(3.W)), 0.U(3.W))
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

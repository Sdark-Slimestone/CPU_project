package R1322IAeCSR

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

// AXI4 master 视角顶层 IO (字段名扁平 -> 端口名 io_master_*)。
class AXI4MasterIO extends Bundle {
  val awready = Input(Bool());  val awvalid = Output(Bool())
  val awid    = Output(UInt(4.W));  val awaddr = Output(UInt(32.W))
  val awlen   = Output(UInt(8.W));  val awsize = Output(UInt(3.W)); val awburst = Output(UInt(2.W))
  val wready  = Input(Bool());  val wvalid = Output(Bool())
  val wdata   = Output(UInt(32.W)); val wstrb = Output(UInt(4.W)); val wlast = Output(Bool())
  val bready  = Output(Bool()); val bvalid = Input(Bool()); val bid = Input(UInt(4.W)); val bresp = Input(UInt(2.W))
  val arready = Input(Bool());  val arvalid = Output(Bool())
  val arid    = Output(UInt(4.W)); val araddr = Output(UInt(32.W))
  val arlen   = Output(UInt(8.W)); val arsize = Output(UInt(3.W)); val arburst = Output(UInt(2.W))
  val rready  = Output(Bool()); val rvalid = Input(Bool()); val rid = Input(UInt(4.W)); val rdata = Input(UInt(32.W)); val rresp = Input(UInt(2.W)); val rlast = Input(Bool())
}
// AXI4 slave 视角 (CPU 不用; io_slave_* 仅占位)。
class AXI4SlaveIO extends Bundle {
  val awready = Output(Bool()); val awvalid = Input(Bool())
  val awid    = Input(UInt(4.W));  val awaddr = Input(UInt(32.W))
  val awlen   = Input(UInt(8.W));  val awsize = Input(UInt(3.W)); val awburst = Input(UInt(2.W))
  val wready  = Output(Bool()); val wvalid = Input(Bool())
  val wdata   = Input(UInt(32.W)); val wstrb = Input(UInt(4.W)); val wlast = Input(Bool())
  val bready  = Input(Bool());  val bvalid = Output(Bool()); val bid = Output(UInt(4.W)); val bresp = Output(UInt(2.W))
  val arready = Output(Bool()); val arvalid = Input(Bool())
  val arid    = Input(UInt(4.W));  val araddr = Input(UInt(32.W))
  val arlen   = Input(UInt(8.W));  val arsize = Input(UInt(3.W)); val arburst = Input(UInt(2.W))
  val rready  = Input(Bool());  val rvalid = Output(Bool()); val rid = Output(UInt(4.W)); val rdata = Output(UInt(32.W)); val rresp = Output(UInt(2.W)); val rlast = Output(Bool())
}

// 顶层 ysyx_26030107: 接入 ysyxSoC 的 CPU (完整 AXI4)。端口严格匹配 ysyxSoC/spec/cpu-interface.md。
//   IFU/LSU -> AXIArbiter -> AXI4LiteXbar -> {CLINT(内部), SoC(外部 io_master)}。
//   不含 SoC 的 SRAM/MROM/UART (由 ysyxSoC 提供); 含 CLINT。
class ysyx_26030107 extends Module {
  val io = IO(new Bundle {
    val interrupt = Input(Bool())
    val master = new AXI4MasterIO
    val slave  = new AXI4SlaveIO
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
  val arb   = Module(new AXIArbiter(MemCfg.depth))
  val xbar  = Module(new AXI4LiteXbar(Seq(
    (BigInt("2000000", 16), BigInt("10000", 16)),       // 0: CLINT [0x0200_0000, 0x0201_0000)
    (BigInt(0), BigInt("100000000", 16))                // 1: SoC (兜底 0x0..0x1_0000_0000)
  )))
  val clint = Module(new AXI4LiteCLINT)

  def ldSize(op: MemOp): UInt = Mux(op.is_lb || op.is_lbu, 0.U, Mux(op.is_lh || op.is_lhu, 1.U, 2.U))
  def stSize(op: MemOp): UInt = Mux(op.is_sb, 0.U, Mux(op.is_sh, 1.U, 2.U))

  // ===================== IFU <-> IMEM =====================
  ifu.io.fetch_req  <> imem.io.fetch_req
  imem.io.fetch_resp <> ifu.io.fetch_resp

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
  // -7 的 load 会握手等待, 重定向源可能因访存冻结滞留在 E, 故一律用"延后冲刷"。
  val flush_de: Bool = (flush_pending || tgt_mis) && !e_holds_src
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

  // ===================== LSU load 非阻塞 + load 记分板 =====================
  // 读请求改到 M 级发出, load 不再占用 E; 响应到达时经独立写口回写 GRF;
  // IDU 用 ld_pend 位图只停「依赖该 rd」的指令。dmem 读口加深 (dmemDepth), 允许
  // 背靠背 load 同时在飞 (MLP) -> 隐藏访存延迟。访存顺序: 只在「有 load 在途」时停住
  // 后续 store (load 之间可并行, 但 store 必须等更老的 load 完成, 保证 load 读到旧值)。
  val e_load1 = e1.valid && e1.bits.ctrl.is_load
  val e_load2 = e2.valid && e2.bits.ctrl.is_load && !kill2_now

  // E 级发出读请求 (发不出去就冻结 E 等 reqReady); 一接收就不再等响应 (非阻塞)。
  // 请求门控掉被重定向/错路径冲刷的 load (避免孤儿响应)。
  val ldreq1 = exu1.io.exu_to_lsu.bits.op.isLoad && !flush_de
  val ldreq2 = exu2.io.exu_to_lsu.bits.op.isLoad && !kill2_now && !flush_de
  val issue1 = e_load1 && ldreq1
  val issue2 = e_load2 && ldreq2

  val reqReady1 = dmem.io.reqReady_1
  val reqReady2 = dmem.io.reqReady_2
  val mem_stall: Bool = (issue1 && !reqReady1) || (issue2 && !reqReady2) || dmem.io.wStall
  val pipe_stall = mdu_stall || mem_stall
  dmem.io.stall := pipe_stall
  exu1.io.stall := pipe_stall
  exu2.io.stall := pipe_stall

  val acc1 = issue1 && reqReady1
  val acc2 = issue2 && reqReady2

  // 每 lane 一个在途 load 队列 (响应按 lane 内序返回)
  val ldq1 = Module(new Queue(new LdMeta, MemCfg.dmemDepth))
  val ldq2 = Module(new Queue(new LdMeta, MemCfg.dmemDepth))
  ldq1.io.enq.valid := acc1
  ldq1.io.enq.bits.rd  := e1.bits.rd
  ldq1.io.enq.bits.op  := exu1.io.exu_to_lsu.bits.op
  ldq1.io.enq.bits.off := exu1.io.exu_to_lsu.bits.addr(1, 0)
  ldq2.io.enq.valid := acc2
  ldq2.io.enq.bits.rd  := e2.bits.rd
  ldq2.io.enq.bits.op  := exu2.io.exu_to_lsu.bits.op
  ldq2.io.enq.bits.off := exu2.io.exu_to_lsu.bits.addr(1, 0)

  val rv1 = dmem.io.load_respValid_1 && ldq1.io.deq.valid
  val rv2 = dmem.io.load_respValid_2 && ldq2.io.deq.valid
  // 同拍两 lane 都有响应时只消费 lane1 (GRF 只有一个 load 写口), lane2 保持 respValid 到下一拍。
  val resp1 = rv1
  val resp2 = rv2 && !rv1
  ldq1.io.deq.ready := resp1
  ldq2.io.deq.ready := resp2
  dmem.io.load_respReady_1 := resp1
  dmem.io.load_respReady_2 := resp2

  // load 读回值抽取 (与 LSU 一致: lw/lh/lhu/lb/lbu + 字节选择/符号扩展)
  def loadExtract(op: MemOp, off: UInt, w: UInt): UInt = {
    val b0 = w(7, 0); val b1 = w(15, 8); val b2 = w(23, 16); val b3 = w(31, 24)
    val half0 = Cat(b1, b0); val half1 = Cat(b3, b2)
    val bs = Mux(off(1), Mux(off(0), b3, b2), Mux(off(0), b1, b0))
    val hs = Mux(off(1), half1, half0)
    MuxCase(0.U(32.W), Seq(
      op.is_lw  -> w,
      op.is_lh  -> Cat(Fill(16, hs(15)), hs),
      op.is_lhu -> Cat(0.U(16.W), hs),
      op.is_lb  -> Cat(Fill(24, bs(7)), bs),
      op.is_lbu -> Cat(0.U(24.W), bs)
    ))
  }
  val ld_val1 = loadExtract(ldq1.io.deq.bits.op, ldq1.io.deq.bits.off, dmem.io.dmem_rdata_1)
  val ld_val2 = loadExtract(ldq2.io.deq.bits.op, ldq2.io.deq.bits.off, dmem.io.dmem_rdata_2)

  // load 记分板 (P1: 直接比较, 去掉 32 位位图查表)。待决 rd 最多 4 个:
  // 两条 lane 的在途队头 + E 级两条 load。访存全局串行化 (IDU 的 mem_ser) 保证不超。
  val if_v1 = ldq1.io.deq.valid && !resp1
  val if_v2 = ldq2.io.deq.valid && !resp2
  val pendRd = VecInit(ldq1.io.deq.bits.rd, ldq2.io.deq.bits.rd, e1.bits.rd, e2.bits.rd)
  val pendV  = VecInit(if_v1, if_v2, e_load1, e_load2)
  val mem_busy = pendV.reduce(_ || _)

  // 响应回写 GRF (独立写口)
  grf.io.load_to_grf.wen   := resp1 || resp2
  grf.io.load_to_grf.waddr := Mux(resp1, ldq1.io.deq.bits.rd, ldq2.io.deq.bits.rd)
  grf.io.load_to_grf.wdata := Mux(resp1, ld_val1, ld_val2)

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
  imem.io.flush := redirect
  dmem.io.flush      := false.B

  val d_stall = idu.io.stall
  idu.io.mdu_stall := pipe_stall
  idu.io.pendRd    := pendRd
  idu.io.pendV     := pendV
  idu.io.mem_busy  := mem_busy

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
  // load 请求未被接收 (mem_stall) 时冻结 E/M, 保持 M 级 load 的地址直到 reqReady。
  StagePipe(exu1.io.exu_to_lsu, lsu1.io.exu_to_lsu, mem_stall, false.B)
  // lane1 分支 taken -> lane2 错路径, 冲刷其 E/M
  StagePipe(exu2.io.exu_to_lsu, lsu2.io.exu_to_lsu, mem_stall, kill2)

  // ===================== LSU1/2 -> [M/W 流水寄存器] -> WBU =====================
  // M 级目标重定向时冲刷 M/W, 阻止 M 级错路径指令写回
  StagePipe(lsu1.io.lsu_to_wbu, wbu.io.lsu_to_wbu_1, false.B, false.B)
  // lane1 分支 taken -> lane2 错路径, 冲刷其 M/W (阻止错路径写回)
  StagePipe(lsu2.io.lsu_to_wbu, wbu.io.lsu_to_wbu_2, false.B, kill2)

  // ===================== LSU1/2 <-> DMEM =====================
  // 读请求 (M 级) 由 LSU 发给 dmem; 写请求 (M 级) 由 LSU 发给 dmem。
  dmem.io.exu_to_dmem_1.addr := exu1.io.exu_to_lsu.bits.addr
  dmem.io.exu_to_dmem_1.ren  := issue1
  dmem.io.exu_to_dmem_1.tag  := 0.U
  dmem.io.exu_to_dmem_1.size := ldSize(exu1.io.exu_to_lsu.bits.op)
  dmem.io.exu_to_dmem_2.addr := exu2.io.exu_to_lsu.bits.addr
  dmem.io.exu_to_dmem_2.ren  := issue2
  dmem.io.exu_to_dmem_2.tag  := 0.U
  dmem.io.exu_to_dmem_2.size := ldSize(exu2.io.exu_to_lsu.bits.op)
  dmem.io.lsu_to_dmem_1.addr       := lsu1.io.exu_to_lsu.bits.addr
  dmem.io.lsu_to_dmem_1.store_data := lsu1.io.lsu_wdata
  dmem.io.lsu_to_dmem_1.mask       := lsu1.io.lsu_wmask
  dmem.io.lsu_to_dmem_1.wen        := lsu1.io.lsu_wen
  dmem.io.lsu_to_dmem_1.size       := stSize(lsu1.io.exu_to_lsu.bits.op)
  // lane2 store 在 lane1 分支 taken 时是错路径, 门控其写使能 (避免错误写内存)
  val m2_wen = lsu2.io.lsu_wen && !kill2
  dmem.io.lsu_to_dmem_2.addr       := lsu2.io.exu_to_lsu.bits.addr
  dmem.io.lsu_to_dmem_2.store_data := lsu2.io.lsu_wdata
  dmem.io.lsu_to_dmem_2.mask       := lsu2.io.lsu_wmask
  dmem.io.lsu_to_dmem_2.wen        := m2_wen
  dmem.io.lsu_to_dmem_2.size       := stSize(lsu2.io.exu_to_lsu.bits.op)
  dmem.io.ebreak := lsu1.io.ebreak_out || lsu2.io.ebreak_out
  // 请求未接收时保持 M (不让 load 前进到 W)
  // 请求在 E 级发出, M 级不再需要 hold (load 到 M 直接过 W, 写回走独立写口)
  lsu1.io.hold := false.B
  lsu2.io.hold := false.B
  // load 响应: 在响应到达那拍经独立写口连同 StoreBuffer 转发值写回 GRF (见上)。
  // (load_respReady 已在上面的 load 单元里驱动)

  // ===================== StoreBuffer (已不再需要) =====================
  // 非阻塞 load 的读发生在响应到达那拍, 此时更老的 store 都已写入存储器 (按序), 故
  // store->load 转发不再需要。LSU 的读值改为直接来自存储器, 由响应写口回写 GRF。
  lsu1.io.lsu_rdata := 0.U(32.W)
  lsu2.io.lsu_rdata := 0.U(32.W)

  // ===================== 仲裁器 + Xbar + CLINT / SoC =====================
  arb.io.m0_rd <> imem.io.axi
  arb.io.m1_rd <> dmem.io.axi_rd
  arb.io.m1_wr <> dmem.io.axi_wr
  arb.io.m0_wr.awvalid := false.B
  arb.io.m0_wr.awaddr  := 0.U
  arb.io.m0_wr.awlen   := 0.U
  arb.io.m0_wr.awsize  := 0.U
  arb.io.m0_wr.awburst := 0.U
  arb.io.m0_wr.awid    := 0.U
  arb.io.m0_wr.wvalid  := false.B
  arb.io.m0_wr.wdata   := 0.U
  arb.io.m0_wr.wstrb   := 0.U
  arb.io.m0_wr.wlast   := false.B
  arb.io.m0_wr.bready  := true.B

  arb.io.s_rd <> xbar.io.m_rd
  arb.io.s_wr <> xbar.io.m_wr

  // slave0 -> CLINT (内部)
  xbar.io.s_rd(0) <> clint.io.axi_rd
  xbar.io.s_wr(0) <> clint.io.axi_wr

  // slave1 -> SoC (io_master, 完整 AXI4)
  io.master.araddr  := xbar.io.s_rd(1).araddr
  io.master.arlen   := xbar.io.s_rd(1).arlen
  io.master.arsize  := xbar.io.s_rd(1).arsize
  io.master.arburst := xbar.io.s_rd(1).arburst
  io.master.arid    := xbar.io.s_rd(1).arid
  io.master.arvalid := xbar.io.s_rd(1).arvalid
  xbar.io.s_rd(1).arready := io.master.arready
  xbar.io.s_rd(1).rvalid := io.master.rvalid
  xbar.io.s_rd(1).rdata  := io.master.rdata
  xbar.io.s_rd(1).rresp  := io.master.rresp
  xbar.io.s_rd(1).rlast  := io.master.rlast
  xbar.io.s_rd(1).rid    := io.master.rid
  io.master.rready := xbar.io.s_rd(1).rready

  io.master.awaddr  := xbar.io.s_wr(1).awaddr
  io.master.awlen   := xbar.io.s_wr(1).awlen
  io.master.awsize  := xbar.io.s_wr(1).awsize
  io.master.awburst := xbar.io.s_wr(1).awburst
  io.master.awid    := xbar.io.s_wr(1).awid
  io.master.awvalid := xbar.io.s_wr(1).awvalid
  xbar.io.s_wr(1).awready := io.master.awready
  io.master.wdata   := xbar.io.s_wr(1).wdata
  io.master.wstrb   := xbar.io.s_wr(1).wstrb
  io.master.wlast   := xbar.io.s_wr(1).wlast
  io.master.wvalid  := xbar.io.s_wr(1).wvalid
  xbar.io.s_wr(1).wready := io.master.wready
  xbar.io.s_wr(1).bvalid := io.master.bvalid
  xbar.io.s_wr(1).bresp  := io.master.bresp
  xbar.io.s_wr(1).bid    := io.master.bid
  io.master.bready  := xbar.io.s_wr(1).bready

  // io.slave 不用 (ChipLink DMA): 输出恒 0
  io.slave.awready := false.B
  io.slave.wready  := false.B
  io.slave.bvalid  := false.B
  io.slave.bid     := 0.U
  io.slave.bresp   := 0.U
  io.slave.arready := false.B
  io.slave.rvalid  := false.B
  io.slave.rid     := 0.U
  io.slave.rdata   := 0.U
  io.slave.rresp   := 0.U
  io.slave.rlast   := false.B

  // ===================== WBU -> GRF =====================
  wbu.io.wbu_to_grf <> grf.io.wbu_to_grf
}

object top extends App {
  sys.env.get("YSYX_RESET_PC").foreach(s => BootCfg.resetPC = BigInt(s, 16))
  ChiselStage.emitSystemVerilogFile(
    new ysyx_26030107,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}

package R1322IAeCSR

import chisel3._
import chisel3.util._

// 流水取指单元 (同步 imem + 方向/地址预测)。
//  - pcIssue 保存"本拍要发出的取指地址"; 同步 imem 数据下一拍返回, 对应上一拍发出的地址。
//  - 每拍用发出的地址 (pcIssue, pcIssue+4) 查 地址预测器(BTB) 和 方向预测器:
//      * 槽1 预测 taken 且 BTB 命中 -> 下一条取指地址 = target, 本对只压入分支本身 (push_num=1);
//      * 槽2 预测 taken 且 BTB 命中 -> 下一条取指地址 = target2, 本对两条都压入;
//      * 都不满足 -> 顺序推进 pcIssue+8。
//    (方向预测 taken 但 BTB 未命中时无法在此跳转, 由译码级兜底处理。)
//  - 把返回的一整对(或第一条)压入取指队列。
//  - redirect (EXU 分支/trap/mret, 或 IDU 译码级兜底) 优先: 改 pc, 丢在返回/待压入的对。
class IFU extends Module {
  val io = IO(new Bundle {
    val redirect    = Input(Bool())
    val redirect_pc = Input(UInt(32.W))

    // SimpleBus 取指读接口 (只读): 本拍发 raddr+reqValid, 等 respValid 回 rdata。
    // 双发射用两个读口 (raddr/raddr2, 共用 reqValid/respValid/respReady)。
    val ifu_raddr  = Output(UInt(32.W))
    val ifu_raddr2 = Output(UInt(32.W))
    val ifu_rdata  = Input(UInt(32.W))
    val ifu_rdata2 = Input(UInt(32.W))
    val ifu_reqValid  = Output(Bool())
    val ifu_respValid = Input(Bool())
    val ifu_respReady = Output(Bool())

    // 查表地址
    val look_pc1 = Output(UInt(32.W))
    val look_pc2 = Output(UInt(32.W))

    // 地址预测 (BTB)
    val btb_hit1    = Input(Bool())
    val btb_tgt1    = Input(UInt(32.W))
    val btb_return1 = Input(Bool())
    val btb_isjal1  = Input(Bool())
    val btb_hit2    = Input(Bool())
    val btb_tgt2    = Input(UInt(32.W))
    val btb_return2 = Input(Bool())
    val btb_isjal2  = Input(Bool())

    // 间接预测 (IBTB + RAS)
    val ibtb_hit1 = Input(Bool())
    val ibtb_tgt1 = Input(UInt(32.W))
    val ibtb_hit2 = Input(Bool())
    val ibtb_tgt2 = Input(UInt(32.W))
    val ras_top   = Input(UInt(32.W))
    val ras_valid = Input(Bool())

    // 循环计数器
    val lc_hit1   = Input(Bool())
    val lc_taken1 = Input(Bool())
    val lc_hit2   = Input(Bool())
    val lc_taken2 = Input(Bool())

    // 方向预测 (锦标赛) 结论 + 携带信息
    val dir_taken1   = Input(Bool())
    val dir_gsh1     = Input(Bool())
    val dir_loc1     = Input(Bool())
    val dir_gshIdx1  = Input(UInt(8.W))
    val dir_locHist1 = Input(UInt(6.W))
    val dir_taken2   = Input(Bool())
    val dir_gsh2     = Input(Bool())
    val dir_loc2     = Input(Bool())
    val dir_gshIdx2  = Input(UInt(8.W))
    val dir_locHist2 = Input(UInt(6.W))

    // 压入取指队列
    val push_valid = Output(Bool())
    val push_num   = Output(UInt(2.W))   // 本对压入几条 (1/2)
    val push_inst  = Output(Vec(2, UInt(32.W)))
    val push_pc    = Output(Vec(2, UInt(32.W)))
    val push_pred  = Output(Vec(2, new PredInfo))
    val push_ready = Input(Bool())

    val debug = new Bundle {
      val inst1_pc = Output(UInt(32.W))
      val inst2_pc = Output(UInt(32.W))
    }
  })

  val pcIssue  = RegInit("h80000000".U(32.W))   // 下一条要发出的取指地址
  // 取指返回流水: 深度 = 存储器延迟。第 LATENCY-1 级是"最老、数据已到达"的一条。
  val LAT = MemCfg.latency
  require(LAT >= 1, "存储器延迟至少 1")
  val retValidQ = RegInit(VecInit(Seq.fill(LAT)(false.B)))
  val retPCQ    = RegInit(VecInit(Seq.fill(LAT)(0.U(32.W))))
  val retNumQ   = RegInit(VecInit(Seq.fill(LAT)(1.U(2.W))))
  val retPredQ  = RegInit(VecInit(Seq.fill(LAT)(0.U.asTypeOf(Vec(2, new PredInfo)))))

  val oldestValid = retValidQ(LAT - 1)
  val stall       = oldestValid && !io.push_ready
  val issue       = !stall && !io.redirect

  val issueAddr = Mux(io.redirect, io.redirect_pc, pcIssue)

  io.ifu_raddr  := issueAddr
  io.ifu_raddr2 := issueAddr + 4.U
  io.ifu_reqValid  := issue          // 只有真发请求时才置 valid
  io.ifu_respReady := io.push_ready  // 能接收响应时才让存储器前进

  // 方向/地址预测按 pcIssue 查 (顺序取指寄存器), 让 redirect 直接旁路预测表, 不把
  // 查表串进 "redirect -> pc" 这条关键路径。
  io.look_pc1 := pcIssue
  io.look_pc2 := pcIssue + 4.U

  // 取指目标/是否跳转:
  //   IBTB 命中(间接) -> 目标取自 IBTB;
  //   BTB 命中且是返回 -> 目标取自 RAS 栈顶;
  //   BTB 命中普通分支/jal -> 目标取自 BTB, 方向用方向预测器结论。
  val fetchTgt1 = Mux(io.ibtb_hit1, io.ibtb_tgt1,
                  Mux(io.btb_return1, io.ras_top, io.btb_tgt1))
  val fetchTgt2 = Mux(io.ibtb_hit2, io.ibtb_tgt2,
                  Mux(io.btb_return2, io.ras_top, io.btb_tgt2))
  // 返回: 仅在 RAS 非空时按 taken 预测; 否则顺序取, 由 EXU 纠正。
  // jal: 无条件恒 taken, 直接判 taken (不走方向预测器, 避免 sel 别名把 jal 方向盖成 not-taken)。
  // 循环计数器高置信时覆盖方向预测器的结论 (精确预测循环退出)。
  val dir1 = Mux(io.lc_hit1, io.lc_taken1, io.dir_taken1)
  val dir2 = Mux(io.lc_hit2, io.lc_taken2, io.dir_taken2)
  val fetchTaken1 = io.ibtb_hit1 ||
    (io.btb_hit1 && Mux(io.btb_return1, io.ras_valid, io.btb_isjal1 || dir1))
  val fetchTaken2 = io.ibtb_hit2 ||
    (io.btb_hit2 && Mux(io.btb_return2, io.ras_valid, io.btb_isjal2 || dir2))

  val nextIssue = Mux(fetchTaken1, fetchTgt1,
                  Mux(fetchTaken2, fetchTgt2, pcIssue + 8.U))
  val pushMax   = Mux(fetchTaken1, 1.U(2.W), 2.U(2.W))

  // 本对每条的方向预测信息 (随指令压入队列; taken 由 IDU 填)
  val predOut = Wire(Vec(2, new PredInfo))
  predOut(0).fetch_taken := fetchTaken1
  predOut(0).dir_taken   := io.dir_taken1
  predOut(0).taken       := false.B
  predOut(0).gsh_pred    := io.dir_gsh1
  predOut(0).loc_pred    := io.dir_loc1
  predOut(0).gsh_idx     := io.dir_gshIdx1
  predOut(0).loc_hist    := io.dir_locHist1
  predOut(0).pred_target := fetchTgt1
  predOut(0).lc_hit      := io.lc_hit1
  predOut(1).fetch_taken := fetchTaken2
  predOut(1).dir_taken   := io.dir_taken2
  predOut(1).taken       := false.B
  predOut(1).gsh_pred    := io.dir_gsh2
  predOut(1).loc_pred    := io.dir_loc2
  predOut(1).gsh_idx     := io.dir_gshIdx2
  predOut(1).loc_hist    := io.dir_locHist2
  predOut(1).pred_target := fetchTgt2
  predOut(1).lc_hit      := io.lc_hit2

  io.push_valid := oldestValid
  io.push_num   := retNumQ(LAT - 1)
  io.push_inst(0) := io.ifu_rdata
  io.push_inst(1) := io.ifu_rdata2
  io.push_pc(0)   := retPCQ(LAT - 1)
  io.push_pc(1)   := retPCQ(LAT - 1) + 4.U
  io.push_pred    := retPredQ(LAT - 1)

  when (io.redirect) {
    for (k <- 0 until LAT) { retValidQ(k) := false.B }
    pcIssue  := io.redirect_pc
  } .elsewhen (!stall) {
    for (k <- LAT - 1 to 1 by -1) {
      retValidQ(k) := retValidQ(k - 1)
      retPCQ(k)    := retPCQ(k - 1)
      retNumQ(k)   := retNumQ(k - 1)
      retPredQ(k)  := retPredQ(k - 1)
    }
    retValidQ(0) := true.B
    retPCQ(0)    := issueAddr
    retNumQ(0)   := pushMax
    retPredQ(0)  := predOut
    pcIssue      := nextIssue
  }

  io.debug.inst1_pc := retPCQ(LAT - 1)
  io.debug.inst2_pc := retPCQ(LAT - 1) + 4.U
}

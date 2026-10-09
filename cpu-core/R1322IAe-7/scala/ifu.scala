package R1322IAeCSR

import chisel3._
import chisel3.util._

// 取指单元 (按 SimpleBus 握手解耦, 支持任意/随机读延迟)。
//  - pcIssue 保存"本拍要发出的取指地址"; 发出 (reqValid+raddr) 后由存储器在随机拍数后
//    返回 respValid+rdata。用 MetaFIFO 记录"已发出未返回"的请求元数据, 队头即当前响应
//    (存储器按序返回)。
//  - 一对取指用两个读口 (raddr/raddr2), 共用 reqValid; 只有当两个口的响应同时有效
//    (respValid1&&respValid2) 才把这一对压入取指队列。这样两个口在消费点强制对齐。
//  - 每拍用 pcIssue 查预测器决定下一条取指地址 (与 -6 相同)。
//  - redirect (EXU 分支/trap/mret, 或 IDU 兜底): 清空元数据 FIFO 与在途读, 改 pc。
class IFU extends Module {
  val io = IO(new Bundle {
    val redirect    = Input(Bool())
    val redirect_pc = Input(UInt(32.W))

    // SimpleBus 取指读接口 (只读): 发 raddr+reqValid, 等 respValid 回 rdata。
    // 双发射用两个读口 (raddr/raddr2, 共用 reqValid/respReady/respValid 判决)。
    val ifu_raddr  = Output(UInt(32.W))
    val ifu_raddr2 = Output(UInt(32.W))
    val ifu_rdata  = Input(UInt(32.W))
    val ifu_rdata2 = Input(UInt(32.W))
    val ifu_reqValid  = Output(Bool())
    val ifu_reqReady  = Input(Bool())
    val ifu_respValid = Input(Bool())   // 两个读口响应都有效时由 imem 给出
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
  val issueAddr = Mux(io.redirect, io.redirect_pc, pcIssue)

  // 未完成取指请求的元数据 FIFO: {pc, num, pred}
  val metaBits = new Bundle {
    val pc   = UInt(32.W)
    val num  = UInt(2.W)
    val pred = Vec(2, new PredInfo)
  }
  val metaEnq = Wire(Decoupled(metaBits))
  val metaDeq = Wire(Decoupled(metaBits))
  MetaFIFO(metaEnq, metaDeq, MemCfg.depth, io.redirect)

  io.ifu_raddr  := issueAddr
  io.ifu_raddr2 := issueAddr + 4.U
  // reset 期间不要发请求: 元数据 FIFO 会被复位清零, 而存储器黑盒无复位, 否则两边错位。
  io.ifu_reqValid := !reset.asBool && !io.redirect && metaEnq.ready

  // 方向/地址预测按 pcIssue 查 (顺序取指寄存器), 让 redirect 直接旁路预测表。
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

  // 发出请求: 请求被存储器接受 (reqReady) 才推进 pcIssue, 并记录元数据。
  metaEnq.valid     := io.ifu_reqValid && io.ifu_reqReady
  metaEnq.bits.pc   := issueAddr
  metaEnq.bits.num  := pushMax
  metaEnq.bits.pred := predOut
  when (metaEnq.valid) { pcIssue := nextIssue }
  when (io.redirect)   { pcIssue := io.redirect_pc }

  // 响应: 两读口都有效才压入队列; push_ready 才消费 (respReady 同时给两读口)。
  io.push_valid := io.ifu_respValid
  io.push_num   := metaDeq.bits.num
  io.push_inst(0) := io.ifu_rdata
  io.push_inst(1) := io.ifu_rdata2
  io.push_pc(0)   := metaDeq.bits.pc
  io.push_pc(1)   := metaDeq.bits.pc + 4.U
  io.push_pred    := metaDeq.bits.pred
  io.ifu_respReady := io.ifu_respValid && io.push_ready
  metaDeq.ready    := io.ifu_respValid && io.push_ready

  io.debug.inst1_pc := metaDeq.bits.pc
  io.debug.inst2_pc := metaDeq.bits.pc + 4.U
}

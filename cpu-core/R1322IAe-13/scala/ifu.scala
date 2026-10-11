package R1322IAeCSR

import chisel3._
import chisel3.util._

// 取指单元 (发射 pcIssue; 预测下一条取指地址)。
//  - 通过 `fetch_req`(Decoupled) 把这一对的 {addr, pc, num, pred} 一起发给 imem;
//    imem 取回后把同样的元数据与两条指令**放在同一个响应队列**里经 `fetch_resp` 返回,
//    因此 pc/指令/pred 永远成对流动, 不会因两个独立 FIFO 在重定向时失步而错位。
//  - redirect (EXU 分支/trap/mret, 或 IDU 兜底): 改 pcIssue, 并让 imem 冲刷在途取指。
class IFU extends Module {
  val io = IO(new Bundle {
    val redirect    = Input(Bool())
    val redirect_pc = Input(UInt(32.W))

    val fetch_req  = Decoupled(new FetchReq)            // -> imem
    val fetch_resp = Flipped(Decoupled(new FetchResp))  // <- imem

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
    val push_num   = Output(UInt(2.W))
    val push_inst  = Output(Vec(2, UInt(32.W)))
    val push_pc    = Output(Vec(2, UInt(32.W)))
    val push_pred  = Output(Vec(2, new PredInfo))
    val push_ready = Input(Bool())

    val debug = new Bundle {
      val inst1_pc = Output(UInt(32.W))
      val inst2_pc = Output(UInt(32.W))
    }
  })

  val pcIssue  = RegInit(BootCfg.resetPC.U(32.W))   // 复位取指地址 (默认 MROM 0x2000_0000; flash 变体 0x3000_0000)
  val issueAddr = Mux(io.redirect, io.redirect_pc, pcIssue)

  // 方向/地址预测按 pcIssue 查 (顺序取指寄存器), redirect 直接旁路预测表。
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

  // 本对每条的方向预测信息 (随指令流动; taken 由 IDU 填)
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

  // 发请求: reset / redirect 期间不发。接受 (fire) 才推进 pcIssue。
  io.fetch_req.valid     := !reset.asBool && !io.redirect
  io.fetch_req.bits.addr := issueAddr
  io.fetch_req.bits.pc   := issueAddr
  io.fetch_req.bits.num  := pushMax
  io.fetch_req.bits.pred := predOut
  when (io.fetch_req.fire) { pcIssue := nextIssue }
  when (io.redirect)       { pcIssue := io.redirect_pc }

  // 收响应: 元数据与指令一起返回。
  io.push_valid   := io.fetch_resp.valid
  io.push_num     := io.fetch_resp.bits.num
  io.push_inst(0) := io.fetch_resp.bits.inst0
  io.push_inst(1) := io.fetch_resp.bits.inst1
  io.push_pc(0)   := io.fetch_resp.bits.pc
  io.push_pc(1)   := io.fetch_resp.bits.pc + 4.U
  io.push_pred    := io.fetch_resp.bits.pred
  io.fetch_resp.ready := io.push_ready

  io.debug.inst1_pc := io.fetch_resp.bits.pc
  io.debug.inst2_pc := io.fetch_resp.bits.pc + 4.U
}

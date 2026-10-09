package R1322IAeCSR

import chisel3._
import chisel3.util._

// 流水取指单元 (同步 imem + BTB 取指级预测)。
//  - pcIssue 保存"本拍要发出的取指地址"; 同步 imem 数据下一拍返回, 对应上一拍发出的地址。
//  - 每拍用发出的地址 (issueAddr, issueAddr+4) 查 BTB:
//      * 第一槽命中 -> 预测 taken: 下一条取指地址 = target, 且本对只压入分支本身 (push_num=1);
//      * 第二槽命中 -> 下一条取指地址 = target2, 本对两条都压入;
//      * 都不命中   -> 顺序推进 issueAddr+8。
//  - 把返回的一整对(或第一条)压入取指队列。
//  - redirect (EXU 分支/trap/mret, 或 IDU 译码级 BTFN) 优先: 改 pc, 丢在返回/待压入的对。
class IFU extends Module {
  val io = IO(new Bundle {
    val redirect    = Input(Bool())
    val redirect_pc = Input(UInt(32.W))

    val ifu_to_imem = new Bundle {
      val addr1 = Output(UInt(32.W))
      val addr2 = Output(UInt(32.W))
    }
    val imem_to_ifu = new Bundle {
      val inst1 = Input(UInt(32.W))
      val inst2 = Input(UInt(32.W))
    }

    // BTB 查询
    val look_pc1 = Output(UInt(32.W))
    val look_pc2 = Output(UInt(32.W))
    val btb_hit1 = Input(Bool())
    val btb_tgt1 = Input(UInt(32.W))
    val btb_hit2 = Input(Bool())
    val btb_tgt2 = Input(UInt(32.W))

    // 压入取指队列
    val push_valid = Output(Bool())
    val push_num   = Output(UInt(2.W))   // 本对压入几条 (1/2)
    val push_inst  = Output(Vec(2, UInt(32.W)))
    val push_pc    = Output(Vec(2, UInt(32.W)))
    val push_pred  = Output(Vec(2, Bool()))
    val push_ready = Input(Bool())

    val debug = new Bundle {
      val inst1_pc = Output(UInt(32.W))
      val inst2_pc = Output(UInt(32.W))
    }
  })

  val pcIssue  = RegInit("h80000000".U(32.W))   // 本拍发出的取指地址
  val retValid = RegInit(false.B)               // 本拍返回数据是否有效(尚未压入队列)
  val retPC    = RegInit(0.U(32.W))             // 本拍返回数据对应的包首地址
  val retNum   = RegInit(1.U(2.W))              // 本对压入几条
  val retPred  = RegInit(VecInit(Seq.fill(2)(false.B)))  // 本对每条的取指级预测

  val consumed = retValid && io.push_ready
  val issue    = !retValid || consumed

  val issueAddr = Mux(io.redirect, io.redirect_pc,
                  Mux(issue, pcIssue, retPC))

  io.ifu_to_imem.addr1 := issueAddr
  io.ifu_to_imem.addr2 := issueAddr + 4.U

  // BTB 预测: 只按 pcIssue (顺序取指寄存器) 查, 让 redirect 直接旁路 BTB, 不把
  // BTB 查表串进 "EXU -> redirect -> pc" 这条关键路径。
  io.look_pc1 := pcIssue
  io.look_pc2 := pcIssue + 4.U
  val nextIssue = Mux(io.btb_hit1, io.btb_tgt1,
                  Mux(io.btb_hit2, io.btb_tgt2, pcIssue + 8.U))
  val pushMax   = Mux(io.btb_hit1, 1.U(2.W), 2.U(2.W))

  io.push_valid := retValid
  io.push_num   := retNum
  io.push_inst(0) := io.imem_to_ifu.inst1
  io.push_inst(1) := io.imem_to_ifu.inst2
  io.push_pc(0)   := retPC
  io.push_pc(1)   := retPC + 4.U
  io.push_pred    := retPred

  when (io.redirect) {
    retValid := false.B
    pcIssue  := io.redirect_pc
  } .elsewhen (issue) {
    retValid   := true.B
    retPC      := issueAddr
    retNum     := pushMax
    retPred(0) := io.btb_hit1
    retPred(1) := io.btb_hit2
    pcIssue    := nextIssue
  }

  io.debug.inst1_pc := retPC
  io.debug.inst2_pc := retPC + 4.U
}

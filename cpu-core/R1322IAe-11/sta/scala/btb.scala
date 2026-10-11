package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== 地址预测器 (BTB) — STA SRAM 版 ==============================
// 与 scala/btb.scala (寄存器版) 接口一致, 但表存 fakeram45_64x96 宏 (2 槽各一份), 同步读。
// 条目打包: [0]=valid, [32:1]=tag(全 PC), [64:33]=target, [65]=static_dir, [66]=is_return, [67]=is_jal。
// 仅用于 make sta 的面积/时序评估, 不做功能仿真 (功能版见 scala/btb.scala)。
class BTB(val depth: Int = 64) extends Module {
  require(depth == 64, "STA SRAM 版固定 64 项")
  val idxW = 6

  val io = IO(new Bundle {
    val if_pc     = Input(UInt(32.W))
    val if_hit    = Output(Bool())
    val if_tgt    = Output(UInt(32.W))
    val if_static = Output(Bool())
    val if_return = Output(Bool())
    val if_isjal  = Output(Bool())
    val if_pc2     = Input(UInt(32.W))
    val if_hit2    = Output(Bool())
    val if_tgt2    = Output(UInt(32.W))
    val if_static2 = Output(Bool())
    val if_return2 = Output(Bool())
    val if_isjal2  = Output(Bool())

    val upd_valid     = Input(Bool())
    val upd_pc        = Input(UInt(32.W))
    val upd_taken     = Input(Bool())
    val upd_target    = Input(UInt(32.W))
    val upd_static    = Input(Bool())
    val upd_is_return = Input(Bool())
    val upd_is_jal    = Input(Bool())
  })

  def pack(tag: UInt, tgt: UInt, stat: Bool, ret: Bool, jal: Bool): UInt =
    Cat(jal, ret, stat, tgt, tag, 1.U(1.W))   // 1+32+32+1+1+1 = 68b

  // 更新输入打一拍, 切断 "转发->EXU->branch_target->SRAM wd_in" 写路径 (更新晚一拍无碍)
  val uValid = RegNext(io.upd_valid, false.B)
  val uPc    = RegNext(io.upd_pc, 0.U(32.W))
  val uTgt   = RegNext(io.upd_target, 0.U(32.W))
  val uStat  = RegNext(io.upd_static, false.B)
  val uRet   = RegNext(io.upd_is_return, false.B)
  val uJal   = RegNext(io.upd_is_jal, false.B)

  val we    = uValid
  val widx  = uPc(idxW + 1, 2)
  val wdata = pack(uPc, uTgt, uStat, uRet, uJal)

  val mem1 = Module(new fakeram45_64x96)
  val mem2 = Module(new fakeram45_64x96)
  for ((m, a) <- Seq((mem1, io.if_pc(idxW + 1, 2)), (mem2, io.if_pc2(idxW + 1, 2)))) {
    m.io.clk       := clock
    m.io.addr_in   := Mux(we, widx, a)
    m.io.wd_in     := wdata
    m.io.w_mask_in := Fill(96, true.B)
    m.io.we_in     := we
    m.io.ce_in     := true.B
  }

  val pcR1 = RegNext(io.if_pc)
  val pcR2 = RegNext(io.if_pc2)
  val r1 = mem1.io.rd_out
  val r2 = mem2.io.rd_out

  io.if_hit    := r1(0) && (r1(32, 1) === pcR1)
  io.if_tgt    := r1(64, 33)
  io.if_static := r1(65)
  io.if_return := r1(66)
  io.if_isjal  := r1(67)
  io.if_hit2    := r2(0) && (r2(32, 1) === pcR2)
  io.if_tgt2    := r2(64, 33)
  io.if_static2 := r2(65)
  io.if_return2 := r2(66)
  io.if_isjal2  := r2(67)
}

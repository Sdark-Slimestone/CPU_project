package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== 间接预测器 (RAS + IBTB) — STA SRAM 版 ========================
// 与 scala/indirect_predictor.scala (寄存器版) 接口一致。
//  - RAS: 返回地址栈 (寄存器, 深度 16)。
//  - IBTB: 表存 fakeram45_64x96 宏 (2 槽各一份, 16 项), 同步读。
// 仅用于 make sta 的面积/时序评估, 不做功能仿真。
class IndirectPredictor(val rasDepth: Int = 16, val ibtbDepth: Int = 16) extends Module {
  require(isPow2(rasDepth) && isPow2(ibtbDepth) && ibtbDepth == 16, "STA SRAM 版 IBTB 固定 16 项")
  val ptrW    = log2Ceil(rasDepth + 1)
  val rasIdxW = log2Ceil(rasDepth)
  val idxW    = 4

  val io = IO(new Bundle {
    val if_pc       = Input(UInt(32.W))
    val if_ibtb_hit = Output(Bool())
    val if_ibtb_tgt = Output(UInt(32.W))
    val ras_top     = Output(UInt(32.W))
    val ras_valid   = Output(Bool())

    val if_pc2       = Input(UInt(32.W))
    val if_ibtb_hit2 = Output(Bool())
    val if_ibtb_tgt2 = Output(UInt(32.W))

    val upd_valid       = Input(Bool())
    val upd_pc          = Input(UInt(32.W))
    val upd_target      = Input(UInt(32.W))
    val upd_is_call     = Input(Bool())
    val upd_is_return   = Input(Bool())
    val upd_is_indirect = Input(Bool())
  })

  // ---------------- RAS (寄存器) ----------------
  val ras = RegInit(VecInit(Seq.fill(rasDepth)(0.U(32.W))))
  val ptr = RegInit(0.U(ptrW.W))
  io.ras_top   := Mux(ptr === 0.U, 0.U(32.W), ras((ptr - 1.U)(rasIdxW - 1, 0)))
  io.ras_valid := ptr =/= 0.U

  // 更新输入打一拍, 切断写路径 (更新晚一拍无碍)
  val uValid    = RegNext(io.upd_valid, false.B)
  val uPc       = RegNext(io.upd_pc, 0.U(32.W))
  val uTgt      = RegNext(io.upd_target, 0.U(32.W))
  val uCall     = RegNext(io.upd_is_call, false.B)
  val uReturn   = RegNext(io.upd_is_return, false.B)
  val uIndirect = RegNext(io.upd_is_indirect, false.B)

  // ---------------- IBTB (SRAM 宏, 2 槽各一份) ----------------
  def pack(tag: UInt, tgt: UInt): UInt = Cat(tgt, tag, 1.U(1.W))  // 1+32+32 = 65b
  val we    = uValid && uIndirect
  val widx  = uPc(idxW + 1, 2)
  val wdata = pack(uPc, uTgt)

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
  io.if_ibtb_hit := r1(0) && (r1(32, 1) === pcR1)
  io.if_ibtb_tgt := r1(64, 33)
  io.if_ibtb_hit2 := r2(0) && (r2(32, 1) === pcR2)
  io.if_ibtb_tgt2 := r2(64, 33)

  // ---------------- 更新 ----------------
  when (uValid) {
    when (uReturn) {
      when (ptr =/= 0.U) { ptr := ptr - 1.U }
    } .elsewhen (uCall) {
      when (ptr =/= rasDepth.U) {
        ras(ptr(rasIdxW - 1, 0)) := uPc + 4.U
        ptr := ptr + 1.U
      }
    }
  }
}

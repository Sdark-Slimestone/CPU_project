package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== 间接预测器: RAS + IBTB ====================================
//  - RAS (返回地址栈): 调用时压入 pc+4, 返回时弹出; 取指时栈顶即返回目标。
//    深度 16; 决议时更新 (不做投机/恢复)。
//  - IBTB (间接跳转目标缓存): 按 PC 直接映射 {valid, tag=pc, target}, 训练非返回 jalr 的实际目标。
// 取指时: IBTB 命中 -> 目标取自 IBTB; 否则 BTB 命中且是返回 -> 目标取自 RAS 栈顶。
//
// 注: 试过 64 项/历史索引(GHR)/双目标 IBTB, 对非返回 jalr 目标准确率均无改善
// (目标随运行数据变化, 非 PC/历史可预测), 故维持 16 项单目标。
class IndirectPredictor(val rasDepth: Int = 16, val ibtbDepth: Int = 16) extends Module {
  require(isPow2(rasDepth) && isPow2(ibtbDepth), "深度需为 2 的幂")
  val ptrW    = log2Ceil(rasDepth + 1)
  val rasIdxW = log2Ceil(rasDepth)
  val idxW    = log2Ceil(ibtbDepth)

  val io = IO(new Bundle {
    // 查表 (槽1 / 槽2)
    val if_pc       = Input(UInt(32.W))
    val if_ibtb_hit = Output(Bool())
    val if_ibtb_tgt = Output(UInt(32.W))
    val ras_top     = Output(UInt(32.W))
    val ras_valid   = Output(Bool())

    val if_pc2       = Input(UInt(32.W))
    val if_ibtb_hit2 = Output(Bool())
    val if_ibtb_tgt2 = Output(UInt(32.W))

    // 更新 (控制指令实际决议后)
    val upd_valid       = Input(Bool())
    val upd_pc          = Input(UInt(32.W))
    val upd_target      = Input(UInt(32.W))
    val upd_is_call     = Input(Bool())
    val upd_is_return   = Input(Bool())
    val upd_is_indirect = Input(Bool())
  })

  // ---------------- RAS ----------------
  val ras = RegInit(VecInit(Seq.fill(rasDepth)(0.U(32.W))))
  val ptr = RegInit(0.U(ptrW.W))
  io.ras_top   := Mux(ptr === 0.U, 0.U(32.W), ras((ptr - 1.U)(rasIdxW - 1, 0)))
  io.ras_valid := ptr =/= 0.U

  // ---------------- IBTB ----------------
  val ibValid = RegInit(VecInit(Seq.fill(ibtbDepth)(false.B)))
  val ibTag   = RegInit(VecInit(Seq.fill(ibtbDepth)(0.U(32.W))))
  val ibTgt   = RegInit(VecInit(Seq.fill(ibtbDepth)(0.U(32.W))))

  def lookupIbtb(pc: UInt): (Bool, UInt) = {
    val i = pc(idxW + 1, 2)
    (ibValid(i) && (ibTag(i) === pc), ibTgt(i))
  }
  val (ih1, it1) = lookupIbtb(io.if_pc)
  val (ih2, it2) = lookupIbtb(io.if_pc2)
  io.if_ibtb_hit := ih1; io.if_ibtb_tgt := it1
  io.if_ibtb_hit2 := ih2; io.if_ibtb_tgt2 := it2

  when (io.upd_valid) {
    when (io.upd_is_return) {
      when (ptr =/= 0.U) { ptr := ptr - 1.U }
    } .elsewhen (io.upd_is_call) {
      when (ptr =/= rasDepth.U) {
        ras(ptr(rasIdxW - 1, 0)) := io.upd_pc + 4.U
        ptr := ptr + 1.U
      }
    }
    when (io.upd_is_indirect) {
      val i = io.upd_pc(idxW + 1, 2)
      ibValid(i) := true.B
      ibTag(i)   := io.upd_pc
      ibTgt(i)   := io.upd_target
    }
  }
}

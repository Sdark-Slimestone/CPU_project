package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== 循环计数器 (loop counter) ==================================
// 学习回边分支的固定圈数 (trip count), 精确预测循环退出。4 条目直接映射 (pc[3:2] 索引)。
//   - 取指: PC 命中且置信度高时给出方向 (count < trip => taken, 否则 not-taken)。
//   - 决议 (仅回边分支): taken 则计数 +1; not-taken (退出) 则学习本次圈数并清零计数。
//    圈数与已学一致则加置信; 变化则更新圈数、重置置信。
// 注: 用直接映射而非全相联 CAM, 是为缩短 IFU 取指路径上的组合逻辑 (频率)。
class LoopCounter(val n: Int = 4, val cntW: Int = 8, val tagW: Int = 16) extends Module {
  val idxW = log2Ceil(n)
  val io = IO(new Bundle {
    // 取指查表 (槽1 / 槽2)
    val if_pc     = Input(UInt(32.W))
    val if_hit    = Output(Bool())   // 有高置信预测
    val if_taken  = Output(Bool())
    val if_pc2    = Input(UInt(32.W))
    val if_hit2   = Output(Bool())
    val if_taken2 = Output(Bool())

    // 决议更新 (回边条件分支)
    val upd_valid = Input(Bool())
    val upd_pc    = Input(UInt(32.W))
    val upd_taken = Input(Bool())
  })

  val valid = RegInit(VecInit(Seq.fill(n)(false.B)))
  val tag   = RegInit(VecInit(Seq.fill(n)(0.U(tagW.W))))
  val trip  = RegInit(VecInit(Seq.fill(n)(0.U(cntW.W))))
  val cnt   = RegInit(VecInit(Seq.fill(n)(0.U(cntW.W))))
  val conf  = RegInit(VecInit(Seq.fill(n)(0.U(2.W))))

  def pcTag(pc: UInt): UInt = pc(tagW + 1, 2)   // 部分 tag (缩短比较器)

  def lookup(pc: UInt): (Bool, Bool) = {
    val i   = pc(idxW + 1, 2)
    val hit = valid(i) && (tag(i) === pcTag(pc)) && (conf(i) === 3.U)
    (hit, cnt(i) < trip(i))
  }
  val (h1, t1) = lookup(io.if_pc)
  val (h2, t2) = lookup(io.if_pc2)
  io.if_hit := h1; io.if_taken := t1
  io.if_hit2 := h2; io.if_taken2 := t2

  when (io.upd_valid) {
    val i   = io.upd_pc(idxW + 1, 2)
    val hit = valid(i) && (tag(i) === pcTag(io.upd_pc))
    when (!hit) {
      valid(i) := true.B
      tag(i)   := pcTag(io.upd_pc)
      trip(i)  := 0.U
      conf(i)  := 0.U
    }
    when (io.upd_taken) {
      cnt(i) := Mux(hit, cnt(i) + 1.U, 1.U)
    } .otherwise {                      // 循环退出: 学习本次圈数
      val meas = Mux(hit, cnt(i), 0.U)
      when (hit && (conf(i) =/= 0.U) && (meas === trip(i))) {
        conf(i) := Mux(conf(i) === 3.U, conf(i), conf(i) + 1.U)
      } .otherwise {
        trip(i) := meas
        conf(i) := 1.U
      }
      cnt(i) := 0.U
    }
  }
}

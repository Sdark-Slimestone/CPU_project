package R1322IAeCSR

import chisel3._
import chisel3.util._

//===================================小 BTB (分支目标缓冲)===========================================
// 直接映射, depth 项, 存 {有效, tag=分支 pc, target}。
// 命中 = "该 pc 以前是实际跳转过的(条件分支/jal)" -> 预测 taken。
// 取指级命中: 直接把下一条取指地址改到 target, 且只压入分支本身 (不走 fall-through),
// 因此正确预测的分支不产生 redirect/冲刷/空泡。
//  - 训练: EXU 实际 taken 的分支/jal 写入 {pc -> target}。
//  - 失效: 命中预测 taken 但实际 not-taken 时清除该表项, 避免长期错预测。
// 不做 jalr (目标在寄存器), 后续成熟再加间接目标缓存。
class BTB(val depth: Int = 16) extends Module {
  require(isPow2(depth) && depth >= 2, "depth 必须是 >=2 的 2 的幂")
  val idxW = log2Ceil(depth)

  val io = IO(new Bundle {
    // 取指查询 (第一槽 = 当前发出地址)
    val if_pc   = Input(UInt(32.W))
    val if_hit  = Output(Bool())
    val if_tgt  = Output(UInt(32.W))
    // 取指查询 (第二槽 = 发出地址+4)
    val if_pc2  = Input(UInt(32.W))
    val if_hit2 = Output(Bool())
    val if_tgt2 = Output(UInt(32.W))

    // 训练 (实际 taken 的条件分支/jal)
    val upd_valid = Input(Bool())
    val upd_pc    = Input(UInt(32.W))
    val upd_tgt   = Input(UInt(32.W))
    // 失效 (预测 taken 但实际 not-taken)
    val inv_valid = Input(Bool())
    val inv_pc    = Input(UInt(32.W))
  })

  val valid = RegInit(VecInit(Seq.fill(depth)(false.B)))
  val tag   = RegInit(VecInit(Seq.fill(depth)(0.U(32.W))))
  val tgt   = RegInit(VecInit(Seq.fill(depth)(0.U(32.W))))

  def lookup(pc: UInt): (Bool, UInt) = {
    val i = pc(idxW + 1, 2)
    (valid(i) && (tag(i) === pc), tgt(i))
  }

  val (h1, t1) = lookup(io.if_pc)
  val (h2, t2) = lookup(io.if_pc2)
  io.if_hit  := h1
  io.if_tgt  := t1
  io.if_hit2 := h2
  io.if_tgt2 := t2

  when (io.inv_valid) {
    val i = io.inv_pc(idxW + 1, 2)
    when (tag(i) === io.inv_pc) { valid(i) := false.B }
  }
  when (io.upd_valid) {
    val i = io.upd_pc(idxW + 1, 2)
    valid(i) := true.B
    tag(i)   := io.upd_pc
    tgt(i)   := io.upd_tgt
  }
}

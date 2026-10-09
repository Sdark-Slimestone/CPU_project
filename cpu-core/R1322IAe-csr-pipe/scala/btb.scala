package R1322IAeCSR

import chisel3._
import chisel3.util._

//===================================小 BTB (分支目标缓冲)===========================================
// 直接映射, depth 项, 存 {有效, tag=分支 pc, target}。
// 命中 = "该 pc 以前是实际跳转过的分支" -> 预测 taken。
// 取指级命中就把 pc 改到 target 并只压入分支本身, 不必等 EXU 决议, 缩短跳转取指气泡。
class BTB(val depth: Int = 16) extends Module {
  require(isPow2(depth) && depth >= 2, "depth 必须是 >=2 的 2 的幂")
  val idxW = log2Ceil(depth)

  val io = IO(new Bundle {
    // IF 查询(第一槽 = pcReg)
    val if_pc    = Input(UInt(32.W))
    val if_hit   = Output(Bool())
    val if_tgt   = Output(UInt(32.W))
    // IF 查询(第二槽 = pcReg+4, 取指对里第二条也可能是 taken 分支)
    val if_pc2   = Input(UInt(32.W))
    val if_hit2  = Output(Bool())
    val if_tgt2  = Output(UInt(32.W))
    // EXU 查询(lane1 分支 pc): 用来判预测对错
    val ex_pc    = Input(UInt(32.W))
    val ex_hit   = Output(Bool())
    // EXU 写入(实际跳转)
    val upd_valid = Input(Bool())
    val upd_pc    = Input(UInt(32.W))
    val upd_tgt   = Input(UInt(32.W))
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
  val (he, _)  = lookup(io.ex_pc)
  io.if_hit := h1; io.if_tgt := t1
  io.if_hit2 := h2; io.if_tgt2 := t2
  io.ex_hit := he

  when (io.upd_valid) {
    val i = io.upd_pc(idxW + 1, 2)
    valid(i) := true.B
    tag(i)   := io.upd_pc
    tgt(i)   := io.upd_tgt
  }
}

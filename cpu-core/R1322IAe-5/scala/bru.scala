package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== 分支解析单元 (BRU) ======================================
// 把 EXU 里的"分支/JAL/JALR 的方向判定 + 目标计算"拆成独立单元, 纯为结构清晰 (行为不变)。
// 纯组合、无状态:
//   - 方向: 复用 3 个比较器 (eq / lt 有符号 / ltu 无符号), 取反派生其余条件分支; jal/jalr 恒 taken。
//   - 目标: jal/条件分支 = pc + imm; jalr = (rs1 + imm) & ~1。
class BRU extends Module {
  val io = IO(new Bundle {
    val ctrl = Input(new CtrlSignals)
    val rs1  = Input(UInt(32.W))
    val rs2  = Input(UInt(32.W))
    val pc   = Input(UInt(32.W))
    val imm  = Input(UInt(32.W))

    val take   = Output(Bool())          // 是否跳转 (实际方向)
    val target = Output(UInt(32.W))      // 实际目标
  })

  val c = io.ctrl

  // 3 个比较器: eq / lt(有符号) / ltu(无符号)
  val cmp_eq  = io.rs1 === io.rs2
  val cmp_lt  = io.rs1.asSInt < io.rs2.asSInt
  val cmp_ltu = io.rs1 < io.rs2

  val cond = MuxCase(false.B, Seq(
    c.is_beq  -> cmp_eq,
    c.is_bne  -> !cmp_eq,
    c.is_blt  -> cmp_lt,
    c.is_bge  -> !cmp_lt,
    c.is_bltu -> cmp_ltu,
    c.is_bgeu -> !cmp_ltu
  ))

  io.take := c.is_jump || (c.is_branch && cond)

  io.target := MuxCase(0.U(32.W), Seq(
    c.is_jal    -> (io.pc + io.imm),
    c.is_jalr   -> ((io.rs1 + io.imm) & ~1.U(32.W)),
    c.is_branch -> (io.pc + io.imm)
  ))
}

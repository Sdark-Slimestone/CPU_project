package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== 地址预测器 (BTB) =========================================
// 按 PC 直接映射, 条目 {valid, tag=pc, target, static_dir, is_return, is_jal}。
//  - target    : 实际 taken 的条件分支/jal 的目标地址 (地址预测)。
//  - static_dir: 静态方向(BTFN): jal 或后向条件分支 => taken。供方向预测器在只知 PC 时取作分量。
//  - is_return : 该 PC 是函数返回 (jalr x0,0(ra)); 取指命中时目标改取 RAS 栈顶。
//  - is_jal    : 该 PC 是 jal (无条件、恒 taken)。取指命中时方向直接判 taken, 不查方向预测器,
//                避免方向表 sel 别名把 jal 方向覆盖成 not-taken。
// 只在实际 taken 时分配条目 (保证"预测 taken 时目标有效"; 否则会出现 target=0 的野跳)。
// 不做非返回 jalr (目标变化多, 交给 IBTB)。
class BTB(val depth: Int = 128) extends Module {
  require(isPow2(depth) && depth >= 2, "depth 必须是 >=2 的 2 的幂")
  val idxW = log2Ceil(depth)

  val io = IO(new Bundle {
    // 查表 (槽1 = 当前发出地址, 槽2 = 发出地址+4)
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

    // 更新 (控制指令实际决议后)
    val upd_valid     = Input(Bool())
    val upd_pc        = Input(UInt(32.W))
    val upd_taken     = Input(Bool())
    val upd_target    = Input(UInt(32.W))
    val upd_static    = Input(Bool())
    val upd_is_return = Input(Bool())
    val upd_is_jal    = Input(Bool())
  })

  val valid     = RegInit(VecInit(Seq.fill(depth)(false.B)))
  val tag       = RegInit(VecInit(Seq.fill(depth)(0.U(32.W))))
  val tgt       = RegInit(VecInit(Seq.fill(depth)(0.U(32.W))))
  val staticDir = RegInit(VecInit(Seq.fill(depth)(false.B)))
  val isReturn  = RegInit(VecInit(Seq.fill(depth)(false.B)))
  val isJal     = RegInit(VecInit(Seq.fill(depth)(false.B)))

  def lookup(pc: UInt): (Bool, UInt, Bool, Bool, Bool) = {
    val i = pc(idxW + 1, 2)
    (valid(i) && (tag(i) === pc), tgt(i), staticDir(i), isReturn(i), isJal(i))
  }

  val (h1, t1, s1, r1, j1) = lookup(io.if_pc)
  val (h2, t2, s2, r2, j2) = lookup(io.if_pc2)
  io.if_hit := h1; io.if_tgt := t1; io.if_static := s1; io.if_return := r1; io.if_isjal := j1
  io.if_hit2 := h2; io.if_tgt2 := t2; io.if_static2 := s2; io.if_return2 := r2; io.if_isjal2 := j2

  when (io.upd_valid) {
    val i    = io.upd_pc(idxW + 1, 2)
    val same = valid(i) && (tag(i) === io.upd_pc)
    when (same || io.upd_taken) {
      when (io.upd_taken) { tgt(i) := io.upd_target }
      staticDir(i) := io.upd_static
      isReturn(i)  := io.upd_is_return
      isJal(i)     := io.upd_is_jal
      valid(i) := true.B
      tag(i)   := io.upd_pc
    }
  }
}

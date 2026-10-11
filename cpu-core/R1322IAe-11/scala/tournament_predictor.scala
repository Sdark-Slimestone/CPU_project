package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== 方向预测器 (三选一锦标赛) ==================================
// 三个方向预测器竞赛, 由单一选择计数器 3 选 1:
//   - gshare : 全局历史寄存器 GHR 与 PC 异或索引的 2-bit 饱和计数器表 (gshPht)。
//   - 局部历史: 每分支的局部历史 LHT(8b) 索引的 2-bit 饱和计数器表 (locPht)。
//   - 静态   : BTFN (jal 或后向条件分支 => taken), 由 BTB 提供 static_dir。
//
// 选择器 sel (2-bit): 0=static, 1=local, 2=gshare; 初值 0 (信任静态兜底)。
//   dir = (sel==2) ? gshare : (sel==1) ? local : static
//
// 更新 (条件分支决议后, GHR 决议时更新):
//   gshare/局部计数器: 用"取指当时"携带的索引做饱和 ±1。
//   LHT: 局部历史左移并填入实际方向。
//   sel: 若当前所选预测错, 切换到某个预测正确的 (优先级 gshare>local>static); 否则不变。
//   GHR: 左移并填入实际方向。
//
// 注: 计数器更新必须用取指当时的索引 (GHR/局部历史在决议前会变), 故 gsh_idx/loc_hist 随指令携带。
// 注: 试过把 sel 改成 2-bit 饱和计数器(gshare/局部) + 静态冷启动, 以及两级锦标赛, 实测
//     alutest/microbench 一升一降或整体变差, 未稳定优于本 3 选 1, 故保留原版。
class TournamentPredictor(
  val ghrW: Int = 8,
  val gshDepth: Int = 256,
  val lhtDepth: Int = 64,
  val locDepth: Int = 64,
  val selDepth: Int = 256
) extends Module {
  val gshW = log2Ceil(gshDepth)
  val lhtW = log2Ceil(lhtDepth)
  val locW = log2Ceil(locDepth)
  val selW = log2Ceil(selDepth)
  val histW = lhtW

  val io = IO(new Bundle {
    // 查表 (槽1 / 槽2)
    val if_pc      = Input(UInt(32.W))
    val if_static  = Input(Bool())
    val if_dir     = Output(Bool())
    val if_gsh     = Output(Bool())
    val if_loc     = Output(Bool())
    val if_gshIdx  = Output(UInt(gshW.W))
    val if_locHist = Output(UInt(histW.W))

    val if_pc2      = Input(UInt(32.W))
    val if_static2  = Input(Bool())
    val if_dir2     = Output(Bool())
    val if_gsh2     = Output(Bool())
    val if_loc2     = Output(Bool())
    val if_gshIdx2  = Output(UInt(gshW.W))
    val if_locHist2 = Output(UInt(histW.W))

    // 更新 (条件分支实际决议后)
    val upd_valid       = Input(Bool())
    val upd_pc          = Input(UInt(32.W))
    val upd_taken       = Input(Bool())      // 实际方向
    val upd_dir_pred    = Input(Bool())      // 取指时方向预测器结论
    val upd_gsh_pred    = Input(Bool())
    val upd_loc_pred    = Input(Bool())
    val upd_static_pred = Input(Bool())
    val upd_gsh_idx     = Input(UInt(gshW.W))
    val upd_loc_hist    = Input(UInt(histW.W))
  })

  val ghr    = RegInit(0.U(ghrW.W))
  val gshPht = RegInit(VecInit(Seq.fill(gshDepth)(1.U(2.W))))   // 01 = 弱 not-taken
  val lht    = RegInit(VecInit(Seq.fill(lhtDepth)(0.U(histW.W))))
  val locPht = RegInit(VecInit(Seq.fill(locDepth)(1.U(2.W))))
  val sel      = RegInit(VecInit(Seq.fill(selDepth)(1.U(2.W)))) // >=2 选 gshare, <2 选局部
  val selValid = RegInit(VecInit(Seq.fill(selDepth)(false.B)))  // 0 => 静态冷启动

  def satInc(x: UInt): UInt = Mux(x === 3.U, x, x + 1.U)
  def satDec(x: UInt): UInt = Mux(x === 0.U, x, x - 1.U)

  // 查表: 返回 (方向结论, gshare分量, 局部分量, gshare索引, 局部历史)
  def lookup(pc: UInt, stat: Bool): (Bool, Bool, Bool, UInt, UInt) = {
    val gIdx   = pc(gshW + 1, 2) ^ ghr(gshW - 1, 0)
    val gTaken = gshPht(gIdx)(1)
    val lIdx   = pc(lhtW + 1, 2)
    val lHist  = lht(lIdx)
    val locIdx = lIdx ^ lHist(locW - 1, 0)
    val lTaken = locPht(locIdx)(1)
    val s      = sel(pc(selW + 1, 2))
    val sv     = selValid(pc(selW + 1, 2))
    val dir    = Mux(!sv, stat, Mux(s(1), gTaken, lTaken))   // 冷启动用静态, 之后 2-bit 选择器
    (dir, gTaken, lTaken, gIdx, lHist)
  }

  val (d1, g1, l1, gi1, lh1) = lookup(io.if_pc, io.if_static)
  val (d2, g2, l2, gi2, lh2) = lookup(io.if_pc2, io.if_static2)
  io.if_dir := d1; io.if_gsh := g1; io.if_loc := l1; io.if_gshIdx := gi1; io.if_locHist := lh1
  io.if_dir2 := d2; io.if_gsh2 := g2; io.if_loc2 := l2; io.if_gshIdx2 := gi2; io.if_locHist2 := lh2

  when (io.upd_valid) {
    val sIdx   = io.upd_pc(selW + 1, 2)
    val lIdx   = io.upd_pc(lhtW + 1, 2)
    val locIdx = lIdx ^ io.upd_loc_hist(locW - 1, 0)

    // gshare / 局部计数器: 用取指当时的索引
    gshPht(io.upd_gsh_idx) :=
      Mux(io.upd_taken, satInc(gshPht(io.upd_gsh_idx)), satDec(gshPht(io.upd_gsh_idx)))
    locPht(locIdx) :=
      Mux(io.upd_taken, satInc(locPht(locIdx)), satDec(locPht(locIdx)))

    // 局部历史移位
    lht(lIdx) := Cat(io.upd_loc_hist(histW - 2, 0), io.upd_taken)

    // 选择器: 2-bit 饱和计数器 (gshare vs 局部); 首次未训练时启用并初始化 (静态仅冷启动)
    val s  = sel(sIdx)
    val sv = selValid(sIdx)
    when (!sv) {
      selValid(sIdx) := true.B
      sel(sIdx) := Mux(io.upd_gsh_pred === io.upd_taken && io.upd_loc_pred =/= io.upd_taken, 3.U,
                   Mux(io.upd_loc_pred === io.upd_taken && io.upd_gsh_pred =/= io.upd_taken, 0.U, 1.U))
    } .elsewhen (io.upd_gsh_pred =/= io.upd_loc_pred) {
      when (io.upd_gsh_pred === io.upd_taken)      { sel(sIdx) := satInc(s) }
      .elsewhen (io.upd_loc_pred === io.upd_taken) { sel(sIdx) := satDec(s) }
    }

    // GHR
    ghr := Cat(ghr(ghrW - 2, 0), io.upd_taken)
  }
}

package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== 方向预测器 (三选一锦标赛) — STA SRAM 版 ======================
// 与 scala/tournament_predictor.scala (寄存器版) 接口一致, 但三张表存 fakeram45 宏:
//   gshare 256x2  -> fakeram45_256x34 (每槽一份)
//   局部 LHT 64x6 -> fakeram45_64x7
//   局部 PHT 64x2 -> fakeram45_64x7
//   selector 256x2-> fakeram45_256x34
// 同步读 (宏内部寄存一拍)。仅用于 make sta 的面积/时序评估, 不做功能仿真。
class TournamentPredictor(
  val ghrW: Int = 8,
  val gshDepth: Int = 256,
  val lhtDepth: Int = 64,
  val locDepth: Int = 64,
  val selDepth: Int = 256
) extends Module {
  require(gshDepth == 256 && lhtDepth == 64 && locDepth == 64 && selDepth == 256,
          "STA SRAM 版固定 gsh256/lht64/loc64/sel256")
  val gshW = 8; val lhtW = 6; val locW = 6; val selW = 8; val histW = 6

  val io = IO(new Bundle {
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

    val upd_valid       = Input(Bool())
    val upd_pc          = Input(UInt(32.W))
    val upd_taken       = Input(Bool())
    val upd_dir_pred    = Input(Bool())
    val upd_gsh_pred    = Input(Bool())
    val upd_loc_pred    = Input(Bool())
    val upd_static_pred = Input(Bool())
    val upd_gsh_idx     = Input(UInt(gshW.W))
    val upd_loc_hist    = Input(UInt(histW.W))
  })

  val ghr = RegInit(0.U(ghrW.W))

  val gsh1 = Module(new fakeram45_256x34); val gsh2 = Module(new fakeram45_256x34)
  val lht1 = Module(new fakeram45_64x7);   val lht2 = Module(new fakeram45_64x7)
  val loc1 = Module(new fakeram45_64x7);   val loc2 = Module(new fakeram45_64x7)
  // 选择器 (2-bit 饱和计数器 + valid): 饱和更新需回读当前值, 宏单读口不够, 故用寄存器。
  val sel      = RegInit(VecInit(Seq.fill(selDepth)(1.U(2.W))))
  val selValid = RegInit(VecInit(Seq.fill(selDepth)(false.B)))

  // 查表索引
  val gIdx1 = io.if_pc(gshW + 1, 2) ^ ghr(gshW - 1, 0)
  val gIdx2 = io.if_pc2(gshW + 1, 2) ^ ghr(gshW - 1, 0)
  val lIdx1 = io.if_pc(lhtW + 1, 2)
  val lIdx2 = io.if_pc2(lhtW + 1, 2)
  val lIdxR1 = RegNext(lIdx1)
  val lIdxR2 = RegNext(lIdx2)
  val locIdx1 = lIdxR1 ^ lht1.io.rd_out(histW - 1, 0)
  val locIdx2 = lIdxR2 ^ lht2.io.rd_out(histW - 1, 0)

  // 更新输入打一拍, 切断写路径 (更新晚一拍无碍)
  val uValid      = RegNext(io.upd_valid, false.B)
  val uTaken      = RegNext(io.upd_taken, false.B)
  val uPc         = RegNext(io.upd_pc, 0.U(32.W))
  val uGshIdx     = RegNext(io.upd_gsh_idx, 0.U(gshW.W))
  val uLocHist    = RegNext(io.upd_loc_hist, 0.U(histW.W))
  val uDirPred    = RegNext(io.upd_dir_pred, false.B)
  val uGshPred    = RegNext(io.upd_gsh_pred, false.B)
  val uLocPred    = RegNext(io.upd_loc_pred, false.B)
  val uStaticPred = RegNext(io.upd_static_pred, false.B)

  // 写端口 (更新)
  val we      = uValid
  val wGshIdx = uGshIdx
  val wLhtIdx = uPc(lhtW + 1, 2)
  val wLocIdx = wLhtIdx ^ uLocHist
  val wSelIdx = uPc(selW + 1, 2)
  val wGshDat = Cat(uTaken, uTaken)
  val wLhtDat = Cat(uLocHist(histW - 2, 0), uTaken)
  val wLocDat = Cat(uTaken, uTaken)

  // 选择器 (2-bit 饱和计数器, gshare vs 局部); 首次未训练时启用并初始化 (静态仅冷启动)
  val sW  = sel(wSelIdx)
  val svW = selValid(wSelIdx)
  when (uValid) {
    when (!svW) {
      selValid(wSelIdx) := true.B
      sel(wSelIdx) := Mux(uGshPred === uTaken && uLocPred =/= uTaken, 3.U,
                      Mux(uLocPred === uTaken && uGshPred =/= uTaken, 0.U, 1.U))
    } .elsewhen (uGshPred =/= uLocPred) {
      when (uGshPred === uTaken)      { sel(wSelIdx) := Mux(sW === 3.U, sW, sW + 1.U) }
      .elsewhen (uLocPred === uTaken) { sel(wSelIdx) := Mux(sW === 0.U, sW, sW - 1.U) }
    }
  }

  def drv256(m: fakeram45_256x34, raddr: UInt, waddr: UInt, wdat: UInt): Unit = {
    m.io.clk       := clock
    m.io.addr_in   := Mux(we, waddr, raddr)
    m.io.wd_in     := wdat
    m.io.w_mask_in := Fill(34, true.B)
    m.io.we_in     := we
    m.io.ce_in     := true.B
  }
  def drv64(m: fakeram45_64x7, raddr: UInt, waddr: UInt, wdat: UInt): Unit = {
    m.io.clk       := clock
    m.io.addr_in   := Mux(we, waddr, raddr)
    m.io.wd_in     := wdat
    m.io.w_mask_in := Fill(7, true.B)
    m.io.we_in     := we
    m.io.ce_in     := true.B
  }
  drv256(gsh1, gIdx1, wGshIdx, wGshDat); drv256(gsh2, gIdx2, wGshIdx, wGshDat)
  drv64(lht1, lIdx1, wLhtIdx, wLhtDat);  drv64(lht2, lIdx2, wLhtIdx, wLhtDat)
  drv64(loc1, locIdx1, wLocIdx, wLocDat); drv64(loc2, locIdx2, wLocIdx, wLocDat)

  // 读结果 (宏已寄存一拍; 选择器寄存器为组合读)
  val s1  = sel(io.if_pc(selW + 1, 2));  val sv1 = selValid(io.if_pc(selW + 1, 2))
  val s2  = sel(io.if_pc2(selW + 1, 2)); val sv2 = selValid(io.if_pc2(selW + 1, 2))
  io.if_dir := Mux(!sv1, io.if_static,  Mux(s1(1), gsh1.io.rd_out(1), loc1.io.rd_out(1)))
  io.if_gsh     := gsh1.io.rd_out(1)
  io.if_loc     := loc1.io.rd_out(1)
  io.if_gshIdx  := RegNext(gIdx1)
  io.if_locHist := lht1.io.rd_out(histW - 1, 0)
  io.if_dir2 := Mux(!sv2, io.if_static2, Mux(s2(1), gsh2.io.rd_out(1), loc2.io.rd_out(1)))
  io.if_gsh2     := gsh2.io.rd_out(1)
  io.if_loc2     := loc2.io.rd_out(1)
  io.if_gshIdx2  := RegNext(gIdx2)
  io.if_locHist2 := lht2.io.rd_out(histW - 1, 0)

  ghr := Cat(ghr(ghrW - 2, 0), uTaken)
}

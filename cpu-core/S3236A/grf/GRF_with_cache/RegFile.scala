package S3236A

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

class top extends Module {
  val logicAddrWidth = 5
  val winCodeWidth   = 3
  val datawidth      = 32

  val io = IO(new Bundle {
    val winCode      = Input(UInt(winCodeWidth.W))
    val winInc       = Input(Bool())                       // call 指令执行中
    val winDec       = Input(Bool())                       // ret  指令执行中
    val overflow     = Output(Bool())                      // 窗口上溢标志
    val underflow    = Output(Bool())                      // 窗口下溢标志
    val readAddrs    = Input(Vec(12, UInt(logicAddrWidth.W)))
    val readDatas    = Output(Vec(12, UInt(datawidth.W)))
    val writeAddrs   = Input(Vec(4, UInt(logicAddrWidth.W)))
    val writeDatas   = Input(Vec(4, UInt(datawidth.W)))
    val writeEnables = Input(Vec(4, Bool()))
    val flush        = Input(Bool())
  })

  // 窗口上溢/下溢检测
  io.overflow  := io.winInc && (io.winCode === 7.U)
  io.underflow := io.winDec && (io.winCode === 0.U)

  val gReg   = Module(new GlobalRegFile8x3212R4W)
  val cache  = Module(new WindowCache24x3212R12W)
  val body0  = Module(new WindowBody30x322R4W)   // 注意：已改为 4 写口
  val body1  = Module(new WindowBody30x322R4W)
  val body2  = Module(new WindowBody30x322R4W)
  val body3  = Module(new WindowBody30x322R4W)

  cache.io.flush := io.flush

  // 预计算 winCode * 16，供读写通路共享
  val winCodeTimes16 = io.winCode * 16.U

  // ========== 读通路：前8口全功能 ==========
  for (i <- 0 until 8) {
    val logAddr = io.readAddrs(i)
    val isGlobal = logAddr <= 7.U
    val cacheIdx = logAddr - 8.U

    gReg.io.readAddrs(i) := logAddr(2,0)
    cache.io.readAddrs(i) := cacheIdx

    val phyAddrFull = winCodeTimes16 + logAddr
    val phyAddr = phyAddrFull(6, 0)

    val winOffset = (phyAddr - 8.U) / 4.U
    val winSel    = (phyAddr - 8.U) % 4.U

    val bodyIdx = (i / 2).U(2.W)
    val local   = (i % 2).U(1.W)

    val selBody0 = winSel === 0.U
    val selBody1 = winSel === 1.U
    val selBody2 = winSel === 2.U
    val selBody3 = winSel === 3.U

    // 连接各体读地址（动态索引）
    body0.io.readAddrs(local) := Mux(bodyIdx === 0.U, winOffset(4,0), 0.U)
    body1.io.readAddrs(local) := Mux(bodyIdx === 1.U, winOffset(4,0), 0.U)
    body2.io.readAddrs(local) := Mux(bodyIdx === 2.U, winOffset(4,0), 0.U)
    body3.io.readAddrs(local) := Mux(bodyIdx === 3.U, winOffset(4,0), 0.U)

    val windowData = MuxCase(0.U, Seq(
      (selBody0 && (bodyIdx === 0.U)) -> body0.io.readDatas(local),
      (selBody1 && (bodyIdx === 1.U)) -> body1.io.readDatas(local),
      (selBody2 && (bodyIdx === 2.U)) -> body2.io.readDatas(local),
      (selBody3 && (bodyIdx === 3.U)) -> body3.io.readDatas(local)
    ))

    val hit = cache.io.readHits(i)
    io.readDatas(i) := Mux(isGlobal,
      gReg.io.readDatas(i),
      Mux(hit, cache.io.readDatas(i), windowData)
    )

    // 回填
    val refillEn = !isGlobal && !hit
    val refillWp = i + 4
    cache.io.writeAddrs(refillWp)   := cacheIdx
    cache.io.writeDatas(refillWp)   := windowData
    cache.io.writeEnables(refillWp) := refillEn
  }

  // ========== 读通路：后4口仅Cache/全局 ==========
  for (i <- 8 until 12) {
    val logAddr = io.readAddrs(i)
    val isGlobal = logAddr <= 7.U
    val cacheIdx = logAddr - 8.U

    gReg.io.readAddrs(i) := logAddr(2,0)
    cache.io.readAddrs(i) := cacheIdx

    val hit = cache.io.readHits(i)
    io.readDatas(i) := Mux(isGlobal,
      gReg.io.readDatas(i),
      Mux(hit, cache.io.readDatas(i), 0.U)
    )
  }

  // ========== 写通路（修正：每个写口独立连接各体的第 j 个写口） ==========
  for (j <- 0 until 4) {
    val logAddr = io.writeAddrs(j)
    val wData   = io.writeDatas(j)
    val wEn     = io.writeEnables(j)
    val isGlobal = logAddr <= 7.U
    val cacheIdx = logAddr - 8.U

    // 全局堆写
    gReg.io.writeAddrs(j)   := logAddr(2,0)
    gReg.io.writeDatas(j)   := wData
    gReg.io.writeEnables(j) := wEn && isGlobal

    // Cache 写穿
    cache.io.writeAddrs(j)   := cacheIdx
    cache.io.writeDatas(j)   := wData
    cache.io.writeEnables(j) := wEn && !isGlobal

    // 下层窗口堆写
    val phyAddrFull = winCodeTimes16 + logAddr
    val phyAddr = phyAddrFull(6, 0)
    val winOffset = (phyAddr - 8.U) / 4.U
    val winSel    = (phyAddr - 8.U) % 4.U

    // 使能仅由 winSel 决定，每个体独立判断
    val enBody0 = wEn && !isGlobal && (winSel === 0.U)
    val enBody1 = wEn && !isGlobal && (winSel === 1.U)
    val enBody2 = wEn && !isGlobal && (winSel === 2.U)
    val enBody3 = wEn && !isGlobal && (winSel === 3.U)

    // 连接到各体的第 j 个写口（每个体现有 4 个写口）
    body0.io.writeAddrs(j)  := winOffset(4,0)
    body0.io.writeDatas(j)  := wData
    body0.io.writeEnables(j) := enBody0

    body1.io.writeAddrs(j)  := winOffset(4,0)
    body1.io.writeDatas(j)  := wData
    body1.io.writeEnables(j) := enBody1

    body2.io.writeAddrs(j)  := winOffset(4,0)
    body2.io.writeDatas(j)  := wData
    body2.io.writeEnables(j) := enBody2

    body3.io.writeAddrs(j)  := winOffset(4,0)
    body3.io.writeDatas(j)  := wData
    body3.io.writeEnables(j) := enBody3
  }
}

object top extends App {
  ChiselStage.emitSystemVerilogFile(
    new top,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}
package S3236A

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

// 参数化的全局寄存器堆（8×32，支持可配置读/写端口数）
class GlobalRegFile extends Module {
  val regCount   = 8
  val addrwidth  = 3
  val writevec = 4
  val readvec = 12
  val datawidth  = 32

  val io = IO(new Bundle {
    val readAddrs    = Input(Vec(readvec, UInt(addrwidth.W)))
    val readDatas    = Output(Vec(readvec, UInt(datawidth.W)))
    val writeAddrs   = Input(Vec(writevec, UInt(addrwidth.W)))
    val writeDatas   = Input(Vec(writevec, UInt(datawidth.W)))
    val writeEnables = Input(Vec(writevec, Bool()))
  })

  val regs = RegInit(VecInit(Seq.fill(regCount)(0.U(datawidth.W))))

  for (wp <- 0 until writevec) {
    regs(io.writeAddrs(wp)) := Mux(io.writeEnables(wp), io.writeDatas(wp), regs(io.writeAddrs(wp)))
  }

  for (rp <- 0 until readvec) {
    io.readDatas(rp) := regs(io.readAddrs(rp))
  }
}

// 参数化的窗口体（30×32，写端口固定为1，读端口可配置）
class WindowBody extends Module {
  val regCount   = 30
  val writevec   = 4
  val readvec = 3
  val addrwidth  = 5
  val datawidth  = 32

  val io = IO(new Bundle {
    val readAddrs    = Input(Vec(readvec, UInt(addrwidth.W)))
    val readDatas    = Output(Vec(readvec, UInt(datawidth.W)))
    val writeAddrs   = Input(Vec(writevec, UInt(addrwidth.W)))
    val writeDatas   = Input(Vec(writevec, UInt(datawidth.W)))
    val writeEnables = Input(Vec(writevec, Bool()))
  })

  val regs = RegInit(VecInit(Seq.fill(regCount)(0.U(datawidth.W))))

  for (wp <- 0 until writevec) {
    regs(io.writeAddrs(wp)) := Mux(io.writeEnables(wp), io.writeDatas(wp), regs(io.writeAddrs(wp)))
  }

  for (rp <- 0 until readvec) {
    io.readDatas(rp) := regs(io.readAddrs(rp))
  }
}


// ================== 顶层模块（无Cache） ==================
class top extends Module {
  val logicAddrWidth = 5
  val winCodeWidth   = 3
  val datawidth      = 32

  val io = IO(new Bundle {
    val winCode      = Input(UInt(winCodeWidth.W))
    val readAddrs    = Input(Vec(12, UInt(logicAddrWidth.W)))
    val readDatas    = Output(Vec(12, UInt(datawidth.W)))
    val writeAddrs   = Input(Vec(4, UInt(logicAddrWidth.W)))
    val writeDatas   = Input(Vec(4, UInt(datawidth.W)))
    val writeEnables = Input(Vec(4, Bool()))
  })

  // 实例化子模块
  val gReg   = Module(new GlobalRegFile)
  val body0  = Module(new WindowBody)
  val body1  = Module(new WindowBody)
  val body2  = Module(new WindowBody)
  val body3  = Module(new WindowBody)

  // 预计算 winCode * 16
  val winCodeTimes16 = io.winCode * 16.U

  // ========== 读通路（12个读口完全对称） ==========
  // 读口绑定：body0:0,1,2 ; body1:3,4,5 ; body2:6,7,8 ; body3:9,10,11
  for (i <- 0 until 12) {
    val logAddr = io.readAddrs(i)
    val isGlobal = logAddr <= 7.U

    // 全局读地址
    gReg.io.readAddrs(i) := logAddr(2,0)

    // 窗口物理地址计算
    val phyAddrFull = winCodeTimes16 + logAddr
    val phyAddr = phyAddrFull(6, 0)
    val winOffset = (phyAddr - 8.U) / 4.U
    val winSel    = (phyAddr - 8.U) % 4.U

    // 当前读口绑定的体索引（0~3）和体内读口号（0~2）
    val bodyIdx = (i / 3).U(2.W)
    val local   = (i % 3).U(2.W)

    // 体选择信号（该地址实际属于哪个体）
    val selBody0 = winSel === 0.U
    val selBody1 = winSel === 1.U
    val selBody2 = winSel === 2.U
    val selBody3 = winSel === 3.U

    // 连接各体读地址（仅当该读口绑定到对应体时有效，否则接0）
    body0.io.readAddrs(local) := Mux(bodyIdx === 0.U, winOffset(4,0), 0.U)
    body1.io.readAddrs(local) := Mux(bodyIdx === 1.U, winOffset(4,0), 0.U)
    body2.io.readAddrs(local) := Mux(bodyIdx === 2.U, winOffset(4,0), 0.U)
    body3.io.readAddrs(local) := Mux(bodyIdx === 3.U, winOffset(4,0), 0.U)

    // 从各体读出的数据（若读口绑定体与实际体不一致，则数据为0）
    val dataFromBody0 = body0.io.readDatas(local)
    val dataFromBody1 = body1.io.readDatas(local)
    val dataFromBody2 = body2.io.readDatas(local)
    val dataFromBody3 = body3.io.readDatas(local)

    // 根据地址实际所在体选择最终窗口数据
    val windowData = MuxCase(0.U, Seq(
      (selBody0 && (bodyIdx === 0.U)) -> dataFromBody0,
      (selBody1 && (bodyIdx === 1.U)) -> dataFromBody1,
      (selBody2 && (bodyIdx === 2.U)) -> dataFromBody2,
      (selBody3 && (bodyIdx === 3.U)) -> dataFromBody3
    ))

    // 最终读数据：全局地址选全局堆，否则选窗口数据
    io.readDatas(i) := Mux(isGlobal, gReg.io.readDatas(i), windowData)
  }

  // ========== 写通路（4个写口全连接） ==========
  for (j <- 0 until 4) {
    val logAddr = io.writeAddrs(j)
    val wData   = io.writeDatas(j)
    val wEn     = io.writeEnables(j)
    val isGlobal = logAddr <= 7.U

    // 全局写
    gReg.io.writeAddrs(j)   := logAddr(2,0)
    gReg.io.writeDatas(j)   := wData
    gReg.io.writeEnables(j) := wEn && isGlobal

    // 窗口写地址计算
    val phyAddrFull = winCodeTimes16 + logAddr
    val phyAddr = phyAddrFull(6, 0)
    val winOffset = (phyAddr - 8.U) / 4.U
    val winSel    = (phyAddr - 8.U) % 4.U

    // 各体写使能（仅当写口有效、非全局、且目标体匹配时有效）
    val enBody0 = wEn && !isGlobal && (winSel === 0.U)
    val enBody1 = wEn && !isGlobal && (winSel === 1.U)
    val enBody2 = wEn && !isGlobal && (winSel === 2.U)
    val enBody3 = wEn && !isGlobal && (winSel === 3.U)

    // 连接到各体的第 j 个写口（全连接）
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

// ================== 生成 Verilog ==================
object top extends App {
  ChiselStage.emitSystemVerilogFile(
    new top,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}
package S3236A

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

// 参数化的全局寄存器堆（8×32，支持可配置读/写端口数）
class GlobalRegFile(readPorts: Int, writePorts: Int) extends Module {
  val regCount   = 8
  val addrwidth  = 3
  val datawidth  = 32

  val io = IO(new Bundle {
    val readAddrs    = Input(Vec(readPorts, UInt(addrwidth.W)))
    val readDatas    = Output(Vec(readPorts, UInt(datawidth.W)))
    val writeAddrs   = Input(Vec(writePorts, UInt(addrwidth.W)))
    val writeDatas   = Input(Vec(writePorts, UInt(datawidth.W)))
    val writeEnables = Input(Vec(writePorts, Bool()))
  })

  val regs = RegInit(VecInit(Seq.fill(regCount)(0.U(datawidth.W))))

  for (wp <- 0 until writePorts) {
    regs(io.writeAddrs(wp)) := Mux(io.writeEnables(wp), io.writeDatas(wp), regs(io.writeAddrs(wp)))
  }

  for (rp <- 0 until readPorts) {
    io.readDatas(rp) := regs(io.readAddrs(rp))
  }
}

// 参数化的窗口体（30×32，写端口固定为1，读端口可配置）
class WindowBody(readPorts: Int) extends Module {
  val regCount   = 30
  val writevec   = 1
  val addrwidth  = 5
  val datawidth  = 32

  val io = IO(new Bundle {
    val readAddrs    = Input(Vec(readPorts, UInt(addrwidth.W)))
    val readDatas    = Output(Vec(readPorts, UInt(datawidth.W)))
    val writeAddrs   = Input(Vec(writevec, UInt(addrwidth.W)))
    val writeDatas   = Input(Vec(writevec, UInt(datawidth.W)))
    val writeEnables = Input(Vec(writevec, Bool()))
  })

  val regs = RegInit(VecInit(Seq.fill(regCount)(0.U(datawidth.W))))

  for (wp <- 0 until writevec) {
    regs(io.writeAddrs(wp)) := Mux(io.writeEnables(wp), io.writeDatas(wp), regs(io.writeAddrs(wp)))
  }

  for (rp <- 0 until readPorts) {
    io.readDatas(rp) := regs(io.readAddrs(rp))
  }
}

// 顶层寄存器文件：128×32，12读4写，支持窗口寄存器窗口切换
class RegFile128_12R4W extends Module {
  val logicAddrWidth = 5   // 逻辑寄存器号 0~31
  val winCodeWidth   = 3   // 窗口指针 0~7
  val phyAddrWidth   = 7   // 物理地址 0~127
  val datawidth      = 32

  val io = IO(new Bundle {
    val winCode      = Input(UInt(winCodeWidth.W))          // 当前窗口码
    val readAddrs    = Input(Vec(12, UInt(logicAddrWidth.W))) // 12个读地址
    val readDatas    = Output(Vec(12, UInt(datawidth.W)))    // 12个读数据
    val writeAddrs   = Input(Vec(4, UInt(logicAddrWidth.W)))  // 4个写地址
    val writeDatas   = Input(Vec(4, UInt(datawidth.W)))       // 4个写数据
    val writeEnables = Input(Vec(4, Bool()))                  // 4个写使能
  })

  // 子模块实例化
  val globalReg = Module(new GlobalRegFile(readPorts = 12, writePorts = 4)) // 全局堆：12读4写
  val body0     = Module(new WindowBody(readPorts = 3))  // 窗口体0，3读1写
  val body1     = Module(new WindowBody(readPorts = 3))  // 窗口体1，3读1写
  val body2     = Module(new WindowBody(readPorts = 3))  // 窗口体2，3读1写
  val body3     = Module(new WindowBody(readPorts = 3))  // 窗口体3，3读1写

  // ---------- 读通路 ----------
  for (i <- 0 until 12) {
    val logAddr = io.readAddrs(i)
    val isGlobal = logAddr <= 7.U
    
    // 物理地址：高3位窗口码 + 低4位逻辑地址（全局寄存器直接用低3位）
    val phyAddr = Cat(io.winCode, logAddr(3,0))

    // 窗口体内偏移与体选择
    val winOffset = (phyAddr - 8.U) / 4.U       // 0~29，共30个
    val winSel    = (phyAddr - 8.U) % 4.U       // 0~3，选择哪个体

    // 读端口绑定：每3个读端口绑定一个体（0,1,2 -> body0; 3,4,5 -> body1; ...）
    val bodyIdx = (i / 3).U(2.W)                // 体索引 0..3
    val local   = (i % 3).U(2.W)                // 体内读端口号 0..2

    // 体地址匹配信号（用于数据选择）
    val selBody0 = !isGlobal && (winSel === 0.U)
    val selBody1 = !isGlobal && (winSel === 1.U)
    val selBody2 = !isGlobal && (winSel === 2.U)
    val selBody3 = !isGlobal && (winSel === 3.U)

    // 连接各体的读地址（仅当端口绑定体匹配时有效，否则给0）
    body0.io.readAddrs(local) := Mux(bodyIdx === 0.U, winOffset(4,0), 0.U)
    body1.io.readAddrs(local) := Mux(bodyIdx === 1.U, winOffset(4,0), 0.U)
    body2.io.readAddrs(local) := Mux(bodyIdx === 2.U, winOffset(4,0), 0.U)
    body3.io.readAddrs(local) := Mux(bodyIdx === 3.U, winOffset(4,0), 0.U)

    // 全局读地址（直接使用逻辑地址低3位）
    globalReg.io.readAddrs(i) := logAddr(2,0)

    // 体数据选择（从匹配的体读出对应local端口的数据）
    val bodyData = MuxCase(0.U, Seq(
      (selBody0 && (bodyIdx === 0.U)) -> body0.io.readDatas(local),
      (selBody1 && (bodyIdx === 1.U)) -> body1.io.readDatas(local),
      (selBody2 && (bodyIdx === 2.U)) -> body2.io.readDatas(local),
      (selBody3 && (bodyIdx === 3.U)) -> body3.io.readDatas(local)
    ))

    // 最终读数据：全局地址读全局堆，否则读体数据
    io.readDatas(i) := Mux(isGlobal, globalReg.io.readDatas(i), bodyData)
  }

  // ---------- 写通路 ----------
  for (j <- 0 until 4) {
    val logAddr = io.writeAddrs(j)
    val wData   = io.writeDatas(j)
    val wEn     = io.writeEnables(j)
    val isGlobal = logAddr <= 7.U

    val phyAddr = Cat(io.winCode, logAddr(3,0))

    val winOffset = (phyAddr - 8.U) / 4.U
    val winSel    = (phyAddr - 8.U) % 4.U

    // 全局写
    globalReg.io.writeAddrs(j)   := logAddr(2,0)
    globalReg.io.writeDatas(j)   := wData
    globalReg.io.writeEnables(j) := wEn && isGlobal

    // 窗口体写：写端口 j 直接绑定到 body j（0->body0, 1->body1, ...）
    val enBody0 = wEn && !isGlobal && (winSel === 0.U) && (j.U === 0.U)
    val enBody1 = wEn && !isGlobal && (winSel === 1.U) && (j.U === 1.U)
    val enBody2 = wEn && !isGlobal && (winSel === 2.U) && (j.U === 2.U)
    val enBody3 = wEn && !isGlobal && (winSel === 3.U) && (j.U === 3.U)

    body0.io.writeAddrs(0)  := winOffset(4,0)
    body0.io.writeDatas(0)  := wData
    body0.io.writeEnables(0) := enBody0

    body1.io.writeAddrs(0)  := winOffset(4,0)
    body1.io.writeDatas(0)  := wData
    body1.io.writeEnables(0) := enBody1

    body2.io.writeAddrs(0)  := winOffset(4,0)
    body2.io.writeDatas(0)  := wData
    body2.io.writeEnables(0) := enBody2

    body3.io.writeAddrs(0)  := winOffset(4,0)
    body3.io.writeDatas(0)  := wData
    body3.io.writeEnables(0) := enBody3
  }
}

// 生成 Verilog 文件
object RegFile128_12R4W extends App {
  ChiselStage.emitSystemVerilogFile(
    new RegFile128_12R4W,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}
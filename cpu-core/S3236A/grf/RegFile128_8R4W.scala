package S3236A

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

// 全局寄存器堆（8x32，8读4写），地址宽度 3 位
class GlobalRegFile8x328R4W extends Module {
  val regCount = 8
  val readvec  = 8
  val writevec = 4
  val addrwidth = 3
  val datawidth = 32

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

// 窗口体 0（30x32，2读1写），地址宽度 5 位
class GlobalRegFile30x322R1W0 extends Module {
  val regCount = 30
  val readvec  = 2
  val writevec = 1
  val addrwidth = 5
  val datawidth = 32

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

// 窗口体 1
class GlobalRegFile30x322R1W1 extends Module {
  val regCount = 30
  val readvec  = 2
  val writevec = 1
  val addrwidth = 5
  val datawidth = 32

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

// 窗口体 2
class GlobalRegFile30x322R1W2 extends Module {
  val regCount = 30
  val readvec  = 2
  val writevec = 1
  val addrwidth = 5
  val datawidth = 32

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

// 窗口体 3
class GlobalRegFile30x322R1W3 extends Module {
  val regCount = 30
  val readvec  = 2
  val writevec = 1
  val addrwidth = 5
  val datawidth = 32

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

class RegFile128_8R4W extends Module {
  val logicAddrWidth = 5   // 逻辑寄存器号 0~31
  val winCodeWidth   = 3   // 窗口指针 0~7
  val phyAddrWidth   = 7   // 物理地址 0~127
  val datawidth      = 32

  val io = IO(new Bundle {
    val winCode      = Input(UInt(winCodeWidth.W))          // 当前窗口码
    val readAddrs    = Input(Vec(8, UInt(logicAddrWidth.W)))
    val readDatas    = Output(Vec(8, UInt(datawidth.W)))
    val writeAddrs   = Input(Vec(4, UInt(logicAddrWidth.W)))
    val writeDatas   = Input(Vec(4, UInt(datawidth.W)))
    val writeEnables = Input(Vec(4, Bool()))
  })

  // 子模块实例化（实例名避免使用 "global" 等潜在关键字）
  val globalReg = Module(new GlobalRegFile8x328R4W)
  val body0     = Module(new GlobalRegFile30x322R1W0)
  val body1     = Module(new GlobalRegFile30x322R1W1)
  val body2     = Module(new GlobalRegFile30x322R1W2)
  val body3     = Module(new GlobalRegFile30x322R1W3)

  // ---------- 读通路 ----------
  for (i <- 0 until 8) {
    val logAddr = io.readAddrs(i)
    val isGlobal = logAddr <= 7.U
    
    val phyAddr = Cat(io.winCode, logAddr(3,0)) // 3位winCode + 4位低地址 = 7位

    val winOffset = (phyAddr - 8.U) / 4.U       // 5 位宽，取低 5 位
    val winSel    = (phyAddr - 8.U) % 4.U

    val bodyIdx = (i / 2).U(2.W)                 // 读端口绑定体
    val local   = (i % 2).U(1.W)                 // 体内读端口号

    // 体地址匹配信号（用于数据选择）
    val selBody0 = !isGlobal && (winSel === 0.U)
    val selBody1 = !isGlobal && (winSel === 1.U)
    val selBody2 = !isGlobal && (winSel === 2.U)
    val selBody3 = !isGlobal && (winSel === 3.U)

    // 连接各体读地址（仅当端口绑定体匹配时有效）
    body0.io.readAddrs(local) := Mux(bodyIdx === 0.U, winOffset(4,0), 0.U)
    body1.io.readAddrs(local) := Mux(bodyIdx === 1.U, winOffset(4,0), 0.U)
    body2.io.readAddrs(local) := Mux(bodyIdx === 2.U, winOffset(4,0), 0.U)
    body3.io.readAddrs(local) := Mux(bodyIdx === 3.U, winOffset(4,0), 0.U)

    // 全局读地址（直接使用逻辑地址低 3 位）
    globalReg.io.readAddrs(i) := logAddr(2,0)

    // 体数据选择
    val bodyData = MuxCase(0.U, Seq(
      (selBody0 && (bodyIdx === 0.U)) -> body0.io.readDatas(local),
      (selBody1 && (bodyIdx === 1.U)) -> body1.io.readDatas(local),
      (selBody2 && (bodyIdx === 2.U)) -> body2.io.readDatas(local),
      (selBody3 && (bodyIdx === 3.U)) -> body3.io.readDatas(local)
    ))

    // 最终读数据：全局地址选全局堆，否则选体数据
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

    // 全局写使能：逻辑地址 0~7 且写使能有效
    globalReg.io.writeAddrs(j)   := logAddr(2,0)
    globalReg.io.writeDatas(j)   := wData
    globalReg.io.writeEnables(j) := wEn && isGlobal

    // 各体写使能：非全局，体匹配，且写端口 j 与体绑定关系一致
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
//综合面积49621
object RegFile128_8R4W extends App {
  ChiselStage.emitSystemVerilogFile(
    new RegFile128_8R4W,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}
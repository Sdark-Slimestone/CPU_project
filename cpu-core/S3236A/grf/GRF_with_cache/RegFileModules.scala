package S3236A

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

//全局寄存器堆 
class GlobalRegFile8x3212R4W extends Module {
  val regCount = 8
  val readvec  = 12
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
//cache
class WindowCache24x3212R12W extends Module {
  val numEntries = 24
  val readPorts  = 12
  val writePorts = 12
  val addrWidth  = 5
  val dataWidth  = 32

  val io = IO(new Bundle {
    val flush        = Input(Bool())
    val readAddrs    = Input(Vec(readPorts, UInt(addrWidth.W)))
    val readDatas    = Output(Vec(readPorts, UInt(dataWidth.W)))
    val readHits     = Output(Vec(readPorts, Bool()))

    val writeAddrs   = Input(Vec(writePorts, UInt(addrWidth.W)))
    val writeDatas   = Input(Vec(writePorts, UInt(dataWidth.W)))
    val writeEnables = Input(Vec(writePorts, Bool()))
  })

  val dataRegs  = RegInit(VecInit(Seq.fill(numEntries)(0.U(dataWidth.W))))
  val validRegs = RegInit(VecInit(Seq.fill(numEntries)(false.B)))

  for (e <- 0 until numEntries) {
    // 收集各写口命中信号
    val hits = Wire(Vec(writePorts, Bool()))
    for (w <- 0 until writePorts) {
      hits(w) := (io.writeAddrs(w) === e.U) && io.writeEnables(w)
    }

    val writeEn   = hits.asUInt.orR
    val writeData = Mux1H(hits, io.writeDatas)   // 独热选择，面积友好

    dataRegs(e)  := Mux(writeEn, writeData, dataRegs(e))
    validRegs(e) := Mux(io.flush, false.B, Mux(writeEn, true.B, validRegs(e)))
  }

  // 读逻辑不变
  for (r <- 0 until readPorts) {
    val addr = io.readAddrs(r)
    io.readDatas(r) := dataRegs(addr)
    io.readHits(r)  := validRegs(addr)
  }
}

//窗口体
class WindowBody30x322R4W extends Module {
  val regCount = 30
  val readvec  = 2
  val writevec = 4
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


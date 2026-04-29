package S3242A

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

// 寄存器堆模板
// 写：编号越大优先级越高
class registers(val regCount: Int, val readvec: Int, val writevec: Int, val addrwidth: Int, val datawidth: Int) extends Module {
  val io = IO(new Bundle {
    val readAddrs    = Input(Vec(readvec, UInt(addrwidth.W)))
    val readDatas    = Output(Vec(readvec, UInt(datawidth.W)))
    val writeAddrs   = Input(Vec(writevec, UInt(addrwidth.W)))
    val writeDatas   = Input(Vec(writevec, UInt(datawidth.W)))
    val writeEnables = Input(Vec(writevec, Bool()))
  })
  val regs = RegInit(VecInit(Seq.fill(regCount)(0.U(datawidth.W))))

  val nextRegs = Wire(Vec(regCount, UInt(datawidth.W)))
  for (i <- 0 until regCount) {
    val matchEn = VecInit((0 until writevec).map(wp => io.writeEnables(wp) && (io.writeAddrs(wp) === i.U)))
    var higher = false.B
    val sel = Wire(Vec(writevec, Bool()))
    for (wp <- writevec-1 to 0 by -1) {
      sel(wp) := matchEn(wp) && !higher
      higher = higher || matchEn(wp)
    }
    val writeValue = Mux1H(sel, io.writeDatas)
    val writeValid = higher
    nextRegs(i) := Mux(writeValid, writeValue, regs(i))
  }
  for (i <- 0 until regCount) {
    regs(i) := nextRegs(i)
  }
  for (rp <- 0 until readvec) {
    io.readDatas(rp) := regs(io.readAddrs(rp))
  }
}




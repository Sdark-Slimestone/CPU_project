package S3236A


import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._


class top extends Module {
  val regCount = 15
  val readvec  = 1
  val writevec = 1
  val addrwidth = 4
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

object top extends App {
  ChiselStage.emitSystemVerilogFile(
    new top,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}

//120/1/1    Chip area for module '\top': 42114.800000 of which used for sequential elements: 23654.400000 (56.17%)
//15/1/1    Chip area for module '\top': 5505.640000 of which used for sequential elements: 2956.800000 (53.70%)

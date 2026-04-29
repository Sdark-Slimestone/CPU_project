package S3242A

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

class globalregfileup extends Module {
  val regCount = 8
  val readvec  = 12
  val writevec = 5
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

  // 为每个寄存器独立计算写端口覆盖后的下一个值
  val nextRegs = Wire(Vec(regCount, UInt(datawidth.W)))
  for (i <- 0 until regCount) {
    // 生成各写端口的匹配使能（地址匹配且写使能有效）
    val matchEn = VecInit((0 until writevec).map(wp => io.writeEnables(wp) && (io.writeAddrs(wp) === i.U)))

    // 优先级编码：后面的写端口优先级更高（因为原代码顺序赋值，后面的覆盖前面的）
    var higher = false.B
    val sel = Wire(Vec(writevec, Bool()))
    for (wp <- writevec-1 to 0 by -1) {
      sel(wp) := matchEn(wp) && !higher
      higher = higher || matchEn(wp)
    }

    // 使用 Mux1H 得到写端口覆盖后的值（若没有匹配则写值为 0，后面会判断）
    val writeValue = Mux1H(sel, io.writeDatas)
    val writeValid = higher

    // 最终值：如果写有效则用 writeValue，否则保持原值
    nextRegs(i) := Mux(writeValid, writeValue, regs(i))
  }

  // 更新寄存器堆
  for (i <- 0 until regCount) {
    regs(i) := nextRegs(i)
  }

  // 正常读
  for (rp <- 0 until readvec) {
    io.readDatas(rp) := regs(io.readAddrs(rp))
  }
}

object globalregfileup extends App {
  ChiselStage.emitSystemVerilogFile(
    new globalregfileup,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}

//8R4W面积7722 功耗0.006549
//12R5W面积11086 功耗0.009720
//需保证写地址互不相同




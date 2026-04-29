package S3242A

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

class wincacheup extends Module {
  val regCount = 24
  val readvec  = 12
  val writevec = 5
  val addrwidth = 5
  val datawidth = 32

  val io = IO(new Bundle {
    val readAddrs    = Input(Vec(readvec, UInt(addrwidth.W)))
    val readDatas    = Output(Vec(readvec, UInt(datawidth.W)))
    val writeAddrs   = Input(Vec(writevec, UInt(addrwidth.W)))
    val writeDatas   = Input(Vec(writevec, UInt(datawidth.W)))
    val filetocache  = Input(UInt((regCount*datawidth).W))
    val cachetofile  = Output(UInt((regCount*datawidth).W))
    val writeEnables = Input(Vec(writevec, Bool()))
    val call         = Input(Bool())
    val ret          = Input(Bool())
    val ig           = Input(Bool())
    val filetocache_enable = Input(UInt(3.W))
    val wincode      = Input(UInt(3.W))      // 新增：当前窗口号
  })

  val offset = RegInit(0.U(2.W))
  val rawNext = Mux(io.call, offset + 1.U, Mux(io.ret, offset - 1.U, offset))
  val nextOffset = Mux(rawNext === 3.U, 0.U, rawNext)
  offset := Mux(io.call || io.ret, nextOffset, offset)

  val curShift = (offset << 3)
  def mapAddr(addr: UInt): UInt = {
    (addr + curShift) % regCount.U
  }

  val regs = RegInit(VecInit(Seq.fill(regCount)(0.U(datawidth.W))))
  val fileFields = VecInit((0 until regCount).map(i => io.filetocache((i+1)*datawidth-1, i*datawidth)))

  val isExchange = io.call || io.ret

  // 正常模式下的写覆盖（地址映射）
  val afterWrites = Wire(Vec(regCount, UInt(datawidth.W)))
  for (i <- 0 until regCount) {
    val matchEn = VecInit((0 until writevec).map(wp => io.writeEnables(wp) && (mapAddr(io.writeAddrs(wp)) === i.U)))
    var higher = false.B
    val sel = Wire(Vec(writevec, Bool()))
    for (wp <- writevec-1 to 0 by -1) {
      sel(wp) := matchEn(wp) && !higher
      higher = higher || matchEn(wp)
    }
    val writeValue = Mux1H(sel, io.writeDatas)
    afterWrites(i) := Mux(higher, writeValue, regs(i))
  }

  // 交换模式下的输出覆盖（地址映射）
  val outputOverrid = Wire(Vec(regCount, UInt(datawidth.W)))
  for (i <- 0 until regCount) {
    val matchEn = VecInit((0 until writevec).map(wp => io.writeEnables(wp) && (mapAddr(io.writeAddrs(wp)) === i.U)))
    var higher = false.B
    val sel = Wire(Vec(writevec, Bool()))
    for (wp <- writevec-1 to 0 by -1) {
      sel(wp) := matchEn(wp) && !higher
      higher = higher || matchEn(wp)
    }
    val writeValue = Mux1H(sel, io.writeDatas)
    outputOverrid(i) := Mux(higher, writeValue, fileFields(i))
  }

  // 寄存器更新：优先级：直写（重叠区域）> call/ret > ig > 正常写
  for (i <- 0 until regCount) {
    val block = i / 8
    val blockEn = io.filetocache_enable(block)

    // 重新计算当前 i 的写端口匹配（优先级编码）
    val matchEn = VecInit((0 until writevec).map(wp => io.writeEnables(wp) && (mapAddr(io.writeAddrs(wp)) === i.U)))
    var higher = false.B
    val sel = Wire(Vec(writevec, Bool()))
    for (wp <- writevec-1 to 0 by -1) {
      sel(wp) := matchEn(wp) && !higher
      higher = higher || matchEn(wp)
    }
    val writeValue = Mux1H(sel, io.writeDatas)

    // 计算当前窗口下的共享物理地址（重叠区域）
    val sharedPa = Mux(io.call, (io.wincode << 1) + 2.U, Mux(io.ret, (io.wincode << 1), 0.U))
    val isOverlap = isExchange && higher && (i.U === sharedPa)

    val initValue = Mux(io.ig && blockEn, fileFields(i), regs(i))
    val exchangeValue = Mux(isExchange && blockEn, fileFields(i), Mux(isExchange, regs(i), afterWrites(i)))

    // 最终值：直写优先级最高，其次交换，然后初始化，最后正常写
    val finalValue = Mux(isOverlap, writeValue, Mux(isExchange, exchangeValue, Mux(io.ig, initValue, afterWrites(i))))
    regs(i) := finalValue
  }

  io.cachetofile := Mux(isExchange, Cat(outputOverrid.reverse), 0.U((regCount*datawidth).W))

  for (rp <- 0 until readvec) {
    io.readDatas(rp) := regs(mapAddr(io.readAddrs(rp)))
  }
}

object wincacheup extends App {
  ChiselStage.emitSystemVerilogFile(
    new wincacheup,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}
//面积40087 功耗0.03804
//需保证写地址各不相同
//加入地址变换后
//面积38711 功耗0.04238
//加了file to cache的写入块使能
//面积38134 功耗0.04923
//加了initGRF指令
//面积38603 功耗0.04001
// winreg_datapath.scala
package S3242A

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

// 支持双写端口、字段写使能（8个32位字段）的寄存器体
class winregbody extends Module {
  val regCount = 5
  val addrwidth = 3
  val datawidth = 256
  val fieldBits = 32
  val fieldNum = datawidth / fieldBits // =8

  val io = IO(new Bundle {
    // 读端口
    val readAddr = Input(UInt(addrwidth.W))
    val readData = Output(UInt(datawidth.W))

    // 写端口0（高优先级）
    val writeAddr0 = Input(UInt(addrwidth.W))
    val writeData0 = Input(UInt(datawidth.W))
    val writeEnable0 = Input(UInt(fieldNum.W)) // 每bit使能一个32位字段

    // 写端口1（低优先级）
    val writeAddr1 = Input(UInt(addrwidth.W))
    val writeData1 = Input(UInt(datawidth.W))
    val writeEnable1 = Input(UInt(fieldNum.W))
  })

  val regs = RegInit(VecInit(Seq.fill(regCount)(0.U(datawidth.W))))

  // 计算下一周期的寄存器值
  val nextRegs = Wire(Vec(regCount, UInt(datawidth.W)))
  for (i <- 0 until regCount) {
    // 收集每个字段的最终值
    val fields = for (j <- 0 until fieldNum) yield {
      val fieldLow = j * fieldBits
      val fieldHigh = fieldLow + fieldBits - 1
      // 端口0字段使能
      val en0 = io.writeEnable0(j)
      // 端口1字段使能
      val en1 = io.writeEnable1(j)
      // 端口0数据字段
      val data0Field = io.writeData0(fieldHigh, fieldLow)
      // 端口1数据字段
      val data1Field = io.writeData1(fieldHigh, fieldLow)
      // 当前寄存器字段
      val currField = regs(i)(fieldHigh, fieldLow)

      Mux(
        (io.writeAddr0 === i.U) && en0,
        data0Field,
        Mux(
          (io.writeAddr1 === i.U) && en1,
          data1Field,
          currField
        )
      )
    }
    // 拼接回256位（注意Cat第一个参数为最高位）
    nextRegs(i) := Cat(fields.reverse)
  }
  regs := nextRegs

  // 读端口（组合逻辑）
  io.readData := regs(io.readAddr)
}

class winreg_P extends Module {
  val io = IO(new Bundle {
    // 控制输入（来自状态机或外部）
    val iscall = Input(Bool())
    val isret  = Input(Bool())
    val ig     = Input(Bool())
    val cur_wincode     = Input(UInt(3.W))
    val overflowcounter = Input(UInt(16.W))
    val prev_wincode    = Input(UInt(3.W))

    // 数据接口
    val cachetofile = Input(UInt(768.W))
    val filetocache = Output(UInt(768.W))
    val filetocache_enable = Output(UInt(3.W))

    val writeback = Input(UInt(768.W))
    val writeback_enable = Input(UInt(24.W))
  })

  // 内部信号：cache_write_enable（基于状态和操作类型计算）
  val current_win_number = io.overflowcounter + io.cur_wincode
  val doExchangewhencall = io.iscall && (io.cur_wincode <= 5.U)
  val doExchangewhenret  = io.isret  && (io.cur_wincode >= 1.U) && (current_win_number <= 6.U)
  val cache_write_enable = !io.ig && (doExchangewhencall || doExchangewhenret)

  // 拆分数据
  val cachetofileVec = VecInit(
    io.cachetofile(255, 0),
    io.cachetofile(511, 256),
    io.cachetofile(767, 512)
  )
  val writebackVec = VecInit(
    io.writeback(255, 0),
    io.writeback(511, 256),
    io.writeback(767, 512)
  )
  val writebackEnVec = VecInit(
    io.writeback_enable(7, 0),
    io.writeback_enable(15, 8),
    io.writeback_enable(23, 16)
  )

  val filetocacheVec = Wire(Vec(3, UInt(256.W)))

  // 三个寄存器体实例
  val body0 = Module(new winregbody)
  val body1 = Module(new winregbody)
  val body2 = Module(new winregbody)

  // 地址映射函数
  def getWriteAddr(win: UInt): (UInt, UInt, UInt) = {
    val addr0 = MuxLookup(win, 0.U)(Seq(
      0.U -> 0.U, 1.U -> 1.U, 2.U -> 2.U, 3.U -> 2.U, 4.U -> 3.U, 5.U -> 4.U, 6.U -> 4.U
    ))
    val addr1 = MuxLookup(win, 0.U)(Seq(
      0.U -> 0.U, 1.U -> 1.U, 2.U -> 1.U, 3.U -> 2.U, 4.U -> 3.U, 5.U -> 3.U, 6.U -> 4.U
    ))
    val addr2 = MuxLookup(win, 0.U)(Seq(
      0.U -> 0.U, 1.U -> 0.U, 2.U -> 1.U, 3.U -> 2.U, 4.U -> 2.U, 5.U -> 3.U, 6.U -> 4.U
    ))
    (addr0, addr1, addr2)
  }

  def getReadAddr(win: UInt): (UInt, UInt, UInt) = {
    val addr0 = MuxLookup(win, 0.U)(Seq(
      0.U -> 1.U, 1.U -> 2.U, 2.U -> 2.U, 3.U -> 3.U, 4.U -> 4.U, 5.U -> 4.U, 6.U -> 4.U
    ))
    val addr1 = MuxLookup(win, 0.U)(Seq(
      0.U -> 1.U, 1.U -> 1.U, 2.U -> 2.U, 3.U -> 3.U, 4.U -> 3.U, 5.U -> 4.U, 6.U -> 4.U
    ))
    val addr2 = MuxLookup(win, 0.U)(Seq(
      0.U -> 0.U, 1.U -> 1.U, 2.U -> 2.U, 3.U -> 2.U, 4.U -> 3.U, 5.U -> 4.U, 6.U -> 4.U
    ))
    (addr0, addr1, addr2)
  }

  // Call 读掩码查表
  def callReadMask(win: UInt): UInt = {
    MuxLookup(win, 0.U(3.W))(Seq(
      0.U -> 3.U(3.W), 1.U -> 5.U(3.W), 2.U -> 6.U(3.W),
      3.U -> 3.U(3.W), 4.U -> 5.U(3.W), 5.U -> 6.U(3.W)
    ))
  }

  // Ret 读掩码查表
  def retReadMask(win: UInt): UInt = {
    MuxLookup(win, 0.U(3.W))(Seq(
      1.U -> 3.U(3.W), 2.U -> 5.U(3.W), 3.U -> 6.U(3.W),
      4.U -> 3.U(3.W), 5.U -> 5.U(3.W), 6.U -> 6.U(3.W)
    ))
  }

  //  写端口0（高优先级）使用 prev_wincode
  val (wbAddr0, wbAddr1, wbAddr2) = getWriteAddr(io.prev_wincode)
  body0.io.writeAddr0 := wbAddr0
  body0.io.writeData0 := writebackVec(0)
  body0.io.writeEnable0 := writebackEnVec(0)

  body1.io.writeAddr0 := wbAddr1
  body1.io.writeData0 := writebackVec(1)
  body1.io.writeEnable0 := writebackEnVec(1)

  body2.io.writeAddr0 := wbAddr2
  body2.io.writeData0 := writebackVec(2)
  body2.io.writeEnable0 := writebackEnVec(2)

  // 写端口1（低优先级）使用 cur_wincode
  val (cacheAddr0, cacheAddr1, cacheAddr2) = getWriteAddr(io.cur_wincode)
  val cacheEnVec = Fill(8, cache_write_enable)

  body0.io.writeAddr1 := cacheAddr0
  body0.io.writeData1 := cachetofileVec(0)
  body0.io.writeEnable1 := cacheEnVec

  body1.io.writeAddr1 := cacheAddr1
  body1.io.writeData1 := cachetofileVec(1)
  body1.io.writeEnable1 := cacheEnVec

  body2.io.writeAddr1 := cacheAddr2
  body2.io.writeData1 := cachetofileVec(2)
  body2.io.writeEnable1 := cacheEnVec

  // 读端口 
  val (readAddr0, readAddr1, readAddr2) = getReadAddr(io.cur_wincode)
  body0.io.readAddr := readAddr0
  body1.io.readAddr := readAddr1
  body2.io.readAddr := readAddr2

  filetocacheVec(0) := body0.io.readData
  filetocacheVec(1) := body1.io.readData
  filetocacheVec(2) := body2.io.readData

  // 输出数据拼接
  io.filetocache := Cat(filetocacheVec(2), filetocacheVec(1), filetocacheVec(0))

  // 读掩码选择（根据操作类型）
  val readMask = Mux(io.ig, 7.U(3.W),
                 Mux(io.iscall, callReadMask(io.cur_wincode),
                 Mux(io.isret, retReadMask(io.cur_wincode), 0.U(3.W))))
  io.filetocache_enable := readMask
}

object winreg_P extends App {
  ChiselStage.emitSystemVerilogFile(
    new winreg_P,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}

//面积47316 功耗0.02281w
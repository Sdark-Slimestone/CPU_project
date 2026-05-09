package R3262VSA

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

class winregcache extends Module {
  val io = IO(new Bundle {
    // 控制输入
    val iscall = Input(Bool())
    val isret  = Input(Bool())
    val cur_wincode     = Input(UInt(3.W))
    val overflowcounter = Input(UInt(16.W))
    val prev_wincode    = Input(UInt(3.W))
    val pushtrap = Input(Bool())
    val poptrap  = Input(Bool())

    // 与后端交互
    val cachetofile = Output(UInt(768.W))
    val filetocache = Input(UInt(768.W))
    val filetocache_enable = Input(UInt(3.W))
    val writeback = Output(UInt(768.W))
    val writeback_enable = Output(UInt(24.W))

    // CPU 接口（增加写使能）
    val readAddr   = Input(Vec(10, UInt(5.W)))
    val readdatas  = Output(Vec(10, UInt(32.W)))
    val writeAddr  = Input(Vec(4, UInt(5.W)))
    val writeData  = Input(Vec(4, UInt(32.W)))
    val writeEnable = Input(Vec(4, Bool()))   // 新增
  })

  val regCount = 24
  val dataWidth = 32
  val regs = RegInit(VecInit(Seq.fill(regCount)(0.U(dataWidth.W))))

  def splitFileToVec(file: UInt): Vec[UInt] = {
    VecInit((0 until regCount).map(i => file((i+1)*dataWidth-1, i*dataWidth)))
  }
  val fileFields = splitFileToVec(io.filetocache)

  def read_addr2paddr(readAddr: UInt): UInt = {
    val base = readAddr - 8.U(5.W)
    val rot = io.cur_wincode % 3.U
    Mux(rot === 0.U, base,
    Mux(rot === 1.U, (base + 8.U) % 24.U,
                    (base + 16.U) % 24.U))
  }

  def write_addr2paddr(writeAddr: UInt): UInt = {
    val base = writeAddr - 8.U(5.W)
    val rot = io.prev_wincode % 3.U
    Mux(rot === 0.U, base,
    Mux(rot === 1.U, (base + 8.U) % 24.U,
                    (base + 16.U) % 24.U))
  }

  // 读端口
  for (i <- 0 until 10) {
    io.readdatas(i) := regs(read_addr2paddr(io.readAddr(i)))
  }

  val window_diff = Mux(io.cur_wincode >= io.prev_wincode,
                        io.cur_wincode - io.prev_wincode,
                        io.prev_wincode - io.cur_wincode)

  // ---- writeback 总线生成 ----
  val writeback_data = Wire(Vec(regCount, UInt(dataWidth.W)))
  val writeback_en   = Wire(Vec(regCount, Bool()))
  for (j <- 0 until regCount) {
    writeback_data(j) := 0.U
    writeback_en(j)   := false.B
  }

  for (i <- 0 until 4) {
    val logical_addr = io.writeAddr(i)
    val data = io.writeData(i)
    val paddr = write_addr2paddr(logical_addr)
    val is_overlap = (logical_addr >= 16.U && logical_addr <= 23.U)

    val write_to_wb = Mux((io.isret) || (io.iscall), true.B,
                         Mux(window_diff === 2.U, true.B,
                         Mux((window_diff === 1.U) && (!is_overlap), true.B, false.B)))

    for (j <- 0 until regCount) {
      when (write_to_wb && io.writeEnable(i) && paddr === j.U) {
        writeback_data(j) := data
        writeback_en(j)   := true.B
      }
    }
  }

  // ---- CPU 正常写 cache（非 call/ret）----
  val cpuWriteData = Wire(Vec(regCount, UInt(dataWidth.W)))
  val cpuWriteEn   = Wire(Vec(regCount, Bool()))
  for (j <- 0 until regCount) {
    cpuWriteData(j) := 0.U
    cpuWriteEn(j)   := false.B
  }

  for (i <- 0 until 4) {
    val logical_addr = io.writeAddr(i)
    val data = io.writeData(i)
    val paddr = write_addr2paddr(logical_addr)
    val is_overlap = (logical_addr >= 16.U && logical_addr <= 23.U)

    val write_to_cache = Mux((!io.isret) && (!io.iscall),
                             Mux(window_diff === 0.U, true.B,
                             Mux((window_diff === 1.U) && is_overlap, true.B, false.B)), false.B)

    for (j <- 0 until regCount) {
      when (write_to_cache && io.writeEnable(i) && paddr === j.U) {
        cpuWriteData(j) := data
        cpuWriteEn(j)   := true.B
      }
    }
  }

  // ---- call/ret 交换逻辑 ----
  val exchangeAllowed = (io.iscall && !io.pushtrap) || (io.isret && !io.poptrap)
  val isExchange = exchangeAllowed

  io.cachetofile := Mux(isExchange, Cat(regs.reverse), 0.U(768.W))

  val nextRegs = Wire(Vec(regCount, UInt(dataWidth.W)))
  for (j <- 0 until regCount) {
    val block = j / 8
    val blockEn = io.filetocache_enable(block)
    val exchangeWrite = isExchange && blockEn
    nextRegs(j) := Mux(exchangeWrite, fileFields(j),
                       Mux(cpuWriteEn(j), cpuWriteData(j), regs(j)))
  }

  regs := nextRegs

  io.writeback := Cat(writeback_data.reverse)
  io.writeback_enable := Cat(writeback_en.reverse)
}
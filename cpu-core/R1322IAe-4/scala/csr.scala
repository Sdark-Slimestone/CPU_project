package R1322IAeCSR

import chisel3._
import chisel3.util._

// CSR 地址定义 (RV32)
object CSRAddr {
  val mstatus   = 0x300.U(12.W)
  val misa      = 0x301.U(12.W)
  val mie       = 0x304.U(12.W)
  val mtvec     = 0x305.U(12.W)
  val mscratch  = 0x340.U(12.W)
  val mepc      = 0x341.U(12.W)
  val mcause    = 0x342.U(12.W)
  val mtval     = 0x343.U(12.W)
  val mip       = 0x344.U(12.W)
  val mcycle    = 0xB00.U(12.W)
  val mcycleh   = 0xB80.U(12.W)
  val minstret  = 0xB02.U(12.W)
  val minstreth = 0xB82.U(12.W)
  val mvendorid = 0xF11.U(12.W)
  val marchid   = 0xF12.U(12.W)
  val mimpid    = 0xF13.U(12.W)
  val mhartid   = 0xF14.U(12.W)
}

// CSR 文件 (机器模式)。支持 Zicsr 读写、ecall/ebreak 异常、mret。
class CSR extends Module {
  val io = IO(new Bundle {
    val exu_to_csr = new Bundle {
      val addr      = Input(UInt(12.W))
      val wen       = Input(Bool())
      val waddr     = Input(UInt(12.W))
      val wdata     = Input(UInt(32.W))
      val op        = Input(UInt(3.W))   // 0=csrrw, 1=csrrs, 2=csrrc
      val rs1_val   = Input(UInt(32.W))
      val use_imm   = Input(Bool())
      val ecall     = Input(Bool())
      val mret      = Input(Bool())
      val is_ebreak = Input(Bool())
      val current_pc = Input(UInt(32.W))
      val inst_retire = Input(UInt(2.W))
    }

    val csr_to_exu = new Bundle {
      val rdata     = Output(UInt(32.W))
      val take_trap = Output(Bool())
      val trap_pc   = Output(UInt(32.W))
      val debug_mcycle    = Output(UInt(64.W))
      val debug_minstret  = Output(UInt(64.W))
      val debug_mstatus   = Output(UInt(32.W))
      val debug_mie       = Output(UInt(32.W))
      val debug_mtvec     = Output(UInt(32.W))
      val debug_mepc      = Output(UInt(32.W))
      val debug_mcause    = Output(UInt(32.W))
      val debug_mtval     = Output(UInt(32.W))
      val debug_mip       = Output(UInt(32.W))
      val debug_mscratch  = Output(UInt(32.W))
      val debug_mvendorid = Output(UInt(32.W))
      val debug_marchid   = Output(UInt(32.W))
      val debug_mimpid    = Output(UInt(32.W))
      val debug_mhartid   = Output(UInt(32.W))
    }
  })

  // ============== 只读常量 ==============
  val mvendorid = 0x79737978L.U(32.W)
  val marchid   = 0x018A9E3B.U(32.W)
  val mimpid    = 0.U(32.W)
  val mhartid   = 0.U(32.W)

  // ============== 计数器 ==============
  // 64 位自加拆成低/高 32 位两条并行进位链 (行为等价)。
  val mcycle_lo = RegInit(0.U(32.W))
  val mcycle_hi = RegInit(0.U(32.W))
  val cyc_sum   = mcycle_lo +& 1.U
  mcycle_lo := cyc_sum(31, 0)
  mcycle_hi := mcycle_hi + cyc_sum(32)

  val minstret_lo = RegInit(0.U(32.W))
  val minstret_hi = RegInit(0.U(32.W))
  val inst_sum    = minstret_lo +& io.exu_to_csr.inst_retire
  minstret_lo := inst_sum(31, 0)
  minstret_hi := minstret_hi + inst_sum(32)

  // ============== CSR 寄存器 ==============
  val mstatus_reg = RegInit(0x00001800L.U(32.W))
  val mtvec_reg   = RegInit(0.U(32.W))
  val mepc_reg    = RegInit(0.U(32.W))
  val mcause_reg  = RegInit(0.U(32.W))
  val mtval_reg   = RegInit(0.U(32.W))
  val mie_reg     = RegInit(0.U(32.W))
  val mip_reg     = RegInit(0.U(32.W))

  // ============== 读逻辑 ==============
  val rdata_wire = WireDefault(0.U(32.W))
  switch (io.exu_to_csr.addr) {
    is (CSRAddr.mstatus)   { rdata_wire := mstatus_reg }
    is (CSRAddr.misa)      { rdata_wire := 0.U(32.W) }
    is (CSRAddr.mie)       { rdata_wire := mie_reg }
    is (CSRAddr.mtvec)     { rdata_wire := mtvec_reg }
    is (CSRAddr.mscratch)  { rdata_wire := 0.U(32.W) }
    is (CSRAddr.mepc)      { rdata_wire := mepc_reg }
    is (CSRAddr.mcause)    { rdata_wire := mcause_reg }
    is (CSRAddr.mtval)     { rdata_wire := mtval_reg }
    is (CSRAddr.mip)       { rdata_wire := mip_reg }
    is (CSRAddr.mcycle)    { rdata_wire := mcycle_lo }
    is (CSRAddr.mcycleh)   { rdata_wire := mcycle_hi }
    is (CSRAddr.minstret)  { rdata_wire := minstret_lo }
    is (CSRAddr.minstreth) { rdata_wire := minstret_hi }
    is (CSRAddr.mvendorid) { rdata_wire := mvendorid }
    is (CSRAddr.marchid)   { rdata_wire := marchid }
    is (CSRAddr.mimpid)    { rdata_wire := mimpid }
    is (CSRAddr.mhartid)   { rdata_wire := mhartid }
  }
  io.csr_to_exu.rdata := rdata_wire

  // ============== 写逻辑 ==============
  val t_rs1 = Mux(io.exu_to_csr.use_imm, Cat(0.U(27.W), io.exu_to_csr.rs1_val(4, 0)),
                                           io.exu_to_csr.rs1_val)
  val csr_write_val = MuxCase(0.U(32.W), Seq(
    (io.exu_to_csr.op === 0.U) -> t_rs1,
    (io.exu_to_csr.op === 1.U) -> (rdata_wire | t_rs1),
    (io.exu_to_csr.op === 2.U) -> (rdata_wire & ~t_rs1)
  ))

  when (io.exu_to_csr.wen) {
    switch (io.exu_to_csr.waddr) {
      is (CSRAddr.mstatus)  { mstatus_reg := csr_write_val }
      is (CSRAddr.mie)      { mie_reg := csr_write_val }
      is (CSRAddr.mtvec)    { mtvec_reg := csr_write_val }
      is (CSRAddr.mscratch) { }
      is (CSRAddr.mepc)     { mepc_reg := csr_write_val }
      is (CSRAddr.mcause)   { mcause_reg := csr_write_val }
      is (CSRAddr.mtval)    { mtval_reg := csr_write_val }
      is (CSRAddr.mip)      { mip_reg := csr_write_val }
    }
  }

  // ============== 异常处理 ==============
  val take_trap_wire = WireDefault(false.B)
  when (io.exu_to_csr.ecall) {
    mepc_reg  := io.exu_to_csr.current_pc
    mcause_reg := 8.U(32.W)
    mtval_reg  := 0.U(32.W)
    take_trap_wire := true.B
  }
  when (io.exu_to_csr.is_ebreak) {
    mepc_reg  := io.exu_to_csr.current_pc
    mcause_reg := 3.U(32.W)
    mtval_reg  := io.exu_to_csr.current_pc
    take_trap_wire := true.B
  }

  io.csr_to_exu.take_trap := take_trap_wire
  io.csr_to_exu.trap_pc   := mtvec_reg

  // ============== 调试输出 ==============
  io.csr_to_exu.debug_mcycle    := Cat(mcycle_hi, mcycle_lo)
  io.csr_to_exu.debug_minstret  := Cat(minstret_hi, minstret_lo)
  io.csr_to_exu.debug_mstatus   := mstatus_reg
  io.csr_to_exu.debug_mie       := mie_reg
  io.csr_to_exu.debug_mtvec     := mtvec_reg
  io.csr_to_exu.debug_mepc      := mepc_reg
  io.csr_to_exu.debug_mcause    := mcause_reg
  io.csr_to_exu.debug_mtval     := mtval_reg
  io.csr_to_exu.debug_mip       := mip_reg
  io.csr_to_exu.debug_mscratch  := 0.U(32.W)
  io.csr_to_exu.debug_mvendorid := mvendorid
  io.csr_to_exu.debug_marchid   := marchid
  io.csr_to_exu.debug_mimpid    := mimpid
  io.csr_to_exu.debug_mhartid   := mhartid
}

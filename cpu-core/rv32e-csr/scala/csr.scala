package rv32ecsr

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

// CSR 文件模块
class CSR extends Module {
  val io = IO(new Bundle {
    // 读取接口
    val csr_addr   = Input(UInt(12.W))
    val csr_rdata  = Output(UInt(32.W))

    // 写入接口
    val csr_wen    = Input(Bool())
    val csr_waddr  = Input(UInt(12.W))
    val csr_wdata  = Input(UInt(32.W))

    // CSR 操作类型
    val csr_op     = Input(UInt(3.W))  // 0=csrrw, 1=csrrs, 2=csrrc
    val rs1_val    = Input(UInt(32.W))
    val use_imm    = Input(Bool())     // true: 用 uimm[4:0] 代替 rs1

    // 异常接口
    val ecall      = Input(Bool())
    val mret       = Input(Bool())
    val is_ebreak  = Input(Bool())
    val current_pc = Input(UInt(32.W))

    // 异常输出
    val take_trap  = Output(Bool())
    val trap_pc    = Output(UInt(32.W))

    // 指令退休计数使能
    val inst_retire = Input(Bool())

    // 调试输出
    val debug_mcycle  = Output(UInt(64.W))
    val debug_minstret = Output(UInt(64.W))
    val debug_mstatus = Output(UInt(32.W))
    val debug_mcause  = Output(UInt(32.W))
    val debug_mepc    = Output(UInt(32.W))
  })

  // ============== CSR 寄存器 ==============
  // 只读寄存器: mvendorid = "ysyx", marchid = 26030107
  val mvendorid = 0x79737978L.U(32.W)
  val marchid   = 0x018A9E3B.U(32.W)
  val mimpid    = 0.U(32.W)
  val mhartid   = 0.U(32.W)

  // mcycle: 每周期加1的64位计数器
  val mcycle_reg = RegInit(0.U(64.W))
  mcycle_reg := mcycle_reg + 1.U

  // minstret: 指令退休计数器
  val minstret_reg = RegInit(0.U(64.W))
  when (io.inst_retire) {
    minstret_reg := minstret_reg + 1.U
  }

  // mstatus: 机器状态寄存器, MPP 字段 [12:11] 初始化为 M-mode (11)
  val mstatus_reg = RegInit(0x00001800L.U(32.W))

  // mtvec: 异常入口基址
  val mtvec_reg = RegInit(0.U(32.W))

  // mepc: 异常 PC
  val mepc_reg = RegInit(0.U(32.W))

  // mcause: 异常原因
  val mcause_reg = RegInit(0.U(32.W))

  // mtval: 异常地址
  val mtval_reg = RegInit(0.U(32.W))

  // mie: 中断使能
  val mie_reg = RegInit(0.U(32.W))

  // mip: 中断待决
  val mip_reg = RegInit(0.U(32.W))

  // ============== CSR 读逻辑 ==============
  val csr_rdata_wire = Wire(UInt(32.W))
  csr_rdata_wire := 0.U(32.W)

  switch (io.csr_addr) {
    is (CSRAddr.mstatus)   { csr_rdata_wire := mstatus_reg }
    is (CSRAddr.misa)      { csr_rdata_wire := 0.U(32.W) }
    is (CSRAddr.mie)       { csr_rdata_wire := mie_reg }
    is (CSRAddr.mtvec)     { csr_rdata_wire := mtvec_reg }
    is (CSRAddr.mscratch)  { csr_rdata_wire := 0.U(32.W) }
    is (CSRAddr.mepc)      { csr_rdata_wire := mepc_reg }
    is (CSRAddr.mcause)    { csr_rdata_wire := mcause_reg }
    is (CSRAddr.mtval)     { csr_rdata_wire := mtval_reg }
    is (CSRAddr.mip)       { csr_rdata_wire := mip_reg }
    is (CSRAddr.mcycle)    { csr_rdata_wire := mcycle_reg(31, 0) }
    is (CSRAddr.mcycleh)   { csr_rdata_wire := mcycle_reg(63, 32) }
    is (CSRAddr.minstret)  { csr_rdata_wire := minstret_reg(31, 0) }
    is (CSRAddr.minstreth) { csr_rdata_wire := minstret_reg(63, 32) }
    is (CSRAddr.mvendorid) { csr_rdata_wire := mvendorid }
    is (CSRAddr.marchid)   { csr_rdata_wire := marchid }
    is (CSRAddr.mimpid)    { csr_rdata_wire := mimpid }
    is (CSRAddr.mhartid)   { csr_rdata_wire := mhartid }
  }

  io.csr_rdata := csr_rdata_wire

  // ============== CSR 写逻辑 ==============
  // 计算 CSR 写入值（支持 csrrw/csrrs/csrrc 原子操作）
  val csr_write_val = Wire(UInt(32.W))
  val t_rs1 = Mux(io.use_imm, Cat(0.U(27.W), io.rs1_val(4, 0)), io.rs1_val)

  csr_write_val := MuxCase(0.U(32.W), Seq(
    (io.csr_op === 0.U) -> t_rs1,                     // csrrw: 直接写入 rs1
    (io.csr_op === 1.U) -> (csr_rdata_wire | t_rs1),  // csrrs: 置位
    (io.csr_op === 2.U) -> (csr_rdata_wire & ~t_rs1), // csrrc: 清除
  ))

  when (io.csr_wen) {
    switch (io.csr_waddr) {
      is (CSRAddr.mstatus)   { mstatus_reg := csr_write_val }
      is (CSRAddr.mie)       { mie_reg := csr_write_val }
      is (CSRAddr.mtvec)     { mtvec_reg := csr_write_val }
      is (CSRAddr.mscratch)  { }
      is (CSRAddr.mepc)      { mepc_reg := csr_write_val }
      is (CSRAddr.mcause)    { mcause_reg := csr_write_val }
      is (CSRAddr.mtval)     { mtval_reg := csr_write_val }
      is (CSRAddr.mip)       { mip_reg := csr_write_val }
    }
  }

  // ============== 异常处理 ==============
  val take_trap_wire = Wire(Bool())
  take_trap_wire := false.B

  // ecall: 触发环境调用异常，mepc 设为 ecall+4（硬件跳过 ecall 指令）
  // AM 的 cte.c 中 yield 使用 ecall，但 cte_init 的异常处理没有为 ecall 添加 +4，
  // 所以必须在硬件中直接设为 pc+4
  when (io.ecall) {
    mepc_reg  := io.current_pc + 4.U
    mcause_reg := 11.U(32.W)  // ECALL from M-mode
    mtval_reg  := 0.U(32.W)
    take_trap_wire := true.B
  }

  // ebreak: 触发断点异常
  when (io.is_ebreak) {
    mepc_reg  := io.current_pc
    mcause_reg := 3.U(32.W)   // Breakpoint
    mtval_reg  := io.current_pc
    take_trap_wire := true.B
  }

  io.take_trap := take_trap_wire
  io.trap_pc   := mtvec_reg

  // ============== 调试输出 ==============
  io.debug_mcycle   := mcycle_reg
  io.debug_minstret := minstret_reg
  io.debug_mstatus  := mstatus_reg
  io.debug_mcause   := mcause_reg
  io.debug_mepc     := mepc_reg
}
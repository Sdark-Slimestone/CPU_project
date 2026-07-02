package rv32ecsr

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

class DPIMemory extends BlackBox with HasBlackBoxResource {
  val io = IO(new Bundle {
    val io_clk = Input(Clock())
    val wen    = Input(Bool())
    val waddr  = Input(UInt(32.W))
    val wdata  = Input(UInt(32.W))
    val wmask  = Input(UInt(4.W))
    val ren    = Input(Bool())
    val raddr  = Input(UInt(32.W))
    val rdata  = Output(UInt(32.W))
    val ebreak = Input(Bool())
  })
  addResource("/DPI_Memory.v")
  override def desiredName = "DPI_Memory"
}

class top extends Module {
  val io = IO(new Bundle {
    val clk   = Input(Clock())
    val reset = Input(Bool())
    // debug 输出（保持与原有兼容）
    val debug_pc   = Output(UInt(32.W))
    val debug_regs = Output(Vec(16, UInt(32.W)))
    val debug_alu_src1 = Output(UInt(32.W))
    val debug_alu_src2 = Output(UInt(32.W))
    val debug_alu_res  = Output(UInt(32.W))
    val debug_regWen = Output(Bool())
    val debug_wbData = Output(UInt(32.W))
    val debug_grf_rden   = Output(Bool())
    val debug_grf_rdaddr = Output(UInt(5.W))
    val debug_grf_input  = Output(UInt(32.W))
    val debug_is_add   = Output(Bool())
    val debug_is_addi  = Output(Bool())
    val debug_is_jalr  = Output(Bool())
    val debug_is_lui   = Output(Bool())
    val debug_is_lbu   = Output(Bool())
    val debug_is_lw    = Output(Bool())
    val debug_is_sw    = Output(Bool())
    val debug_is_sb    = Output(Bool())
    val debug_is_ebreak= Output(Bool())
    val debug_lsu_addr  = Output(UInt(32.W))
    val debug_lsu_wen   = Output(Bool())
    val debug_lsu_wdata = Output(UInt(32.W))
    val debug_lsu_wmask = Output(UInt(4.W))
    val debug_lsu_ren   = Output(Bool())
    val debug_lsu_rdata = Output(UInt(32.W))
    val debug_inst = Output(UInt(32.W))
    // CSR debug 输出（所有可读 CSR 寄存器）
    val debug_mcycle   = Output(UInt(64.W))
    val debug_minstret = Output(UInt(64.W))
    val debug_mstatus  = Output(UInt(32.W))
    val debug_mie      = Output(UInt(32.W))
    val debug_mtvec    = Output(UInt(32.W))
    val debug_mepc     = Output(UInt(32.W))
    val debug_mcause   = Output(UInt(32.W))
    val debug_mtval    = Output(UInt(32.W))
    val debug_mip      = Output(UInt(32.W))
    val debug_mscratch = Output(UInt(32.W))
    val debug_mvendorid = Output(UInt(32.W))
    val debug_marchid   = Output(UInt(32.W))
    val debug_mimpid    = Output(UInt(32.W))
    val debug_mhartid   = Output(UInt(32.W))
  })

  val ifu = Module(new IFU)
  val idu = Module(new idu)
  val exu = Module(new EXU)
  val lsu = Module(new LSU)
  val wbu = Module(new WBU)
  val grf = Module(new GRF)
  val csr = Module(new CSR)

  // 指令存储器
  val instMem = Module(new DPIMemory)
  instMem.io.io_clk := clock
  instMem.io.wen   := false.B
  instMem.io.waddr := 0.U
  instMem.io.wdata := 0.U
  instMem.io.wmask := 0.U
  instMem.io.ren   := true.B
  instMem.io.raddr := ifu.io.imemAddr
  ifu.io.inst      := instMem.io.rdata

  // 数据存储器
  val dataMem = Module(new DPIMemory)
  dataMem.io.io_clk := clock
  dataMem.io.wen   := lsu.io.dmemWen
  dataMem.io.waddr := lsu.io.dmemAddr
  dataMem.io.wdata := lsu.io.dmemWdata
  dataMem.io.wmask := lsu.io.dmemWmask
  dataMem.io.ren   := lsu.io.dmemRen
  dataMem.io.raddr := lsu.io.dmemAddr
  lsu.io.dmemRdata := dataMem.io.rdata

  instMem.io.ebreak := idu.io.is_ebreak
  dataMem.io.ebreak := idu.io.is_ebreak
  ifu.io.is_ebreak  := idu.io.is_ebreak

  // IFU <-> IDU
  idu.io.inst := ifu.io.inst

  // IDU -> EXU 指令识别
  exu.io.is_lui   := idu.io.is_lui
  exu.io.is_auipc := idu.io.is_auipc
  exu.io.is_jal   := idu.io.is_jal
  exu.io.is_jalr  := idu.io.is_jalr
  exu.io.is_beq   := idu.io.is_beq
  exu.io.is_bne   := idu.io.is_bne
  exu.io.is_blt   := idu.io.is_blt
  exu.io.is_bge   := idu.io.is_bge
  exu.io.is_bltu  := idu.io.is_bltu
  exu.io.is_bgeu  := idu.io.is_bgeu
  exu.io.is_lb    := idu.io.is_lb
  exu.io.is_lh    := idu.io.is_lh
  exu.io.is_lw    := idu.io.is_lw
  exu.io.is_lbu   := idu.io.is_lbu
  exu.io.is_lhu   := idu.io.is_lhu
  exu.io.is_sb    := idu.io.is_sb
  exu.io.is_sh    := idu.io.is_sh
  exu.io.is_sw    := idu.io.is_sw
  exu.io.is_addi  := idu.io.is_addi
  exu.io.is_slti  := idu.io.is_slti
  exu.io.is_sltiu := idu.io.is_sltiu
  exu.io.is_xori  := idu.io.is_xori
  exu.io.is_ori   := idu.io.is_ori
  exu.io.is_andi  := idu.io.is_andi
  exu.io.is_slli  := idu.io.is_slli
  exu.io.is_srli  := idu.io.is_srli
  exu.io.is_srai  := idu.io.is_srai
  exu.io.is_add   := idu.io.is_add
  exu.io.is_sub   := idu.io.is_sub
  exu.io.is_sll   := idu.io.is_sll
  exu.io.is_slt   := idu.io.is_slt
  exu.io.is_sltu  := idu.io.is_sltu
  exu.io.is_xor   := idu.io.is_xor
  exu.io.is_srl   := idu.io.is_srl
  exu.io.is_sra   := idu.io.is_sra
  exu.io.is_or    := idu.io.is_or
  exu.io.is_and   := idu.io.is_and
  // CSR 指令
  exu.io.is_csrrw := idu.io.is_csrrw
  exu.io.is_csrrs := idu.io.is_csrrs
  exu.io.is_csrrc := idu.io.is_csrrc

  exu.io.rs1_addr := idu.io.rs1
  exu.io.rs2_addr := idu.io.rs2
  exu.io.rd_addr  := idu.io.rd
  exu.io.imm_i    := idu.io.imm_i
  exu.io.imm_s    := idu.io.imm_s
  exu.io.imm_b    := idu.io.imm_b
  exu.io.imm_u    := idu.io.imm_u
  exu.io.imm_j    := idu.io.imm_j
  exu.io.current_pc := ifu.io.current_pc

  // EXU -> GRF
  grf.io.rs1en   := exu.io.rs1_en
  grf.io.rs1addr := exu.io.rs1_addr_out
  grf.io.rs2en   := exu.io.rs2_en
  grf.io.rs2addr := exu.io.rs2_addr_out
  exu.io.rs1_data := grf.io.rs1out
  exu.io.rs2_data := grf.io.rs2out

  // EXU -> LSU
  lsu.io.lb   := idu.io.is_lb
  lsu.io.lh   := idu.io.is_lh
  lsu.io.lw   := idu.io.is_lw
  lsu.io.lbu  := idu.io.is_lbu
  lsu.io.lhu  := idu.io.is_lhu
  lsu.io.sb   := idu.io.is_sb
  lsu.io.sh   := idu.io.is_sh
  lsu.io.sw   := idu.io.is_sw
  lsu.io.addr := exu.io.agu_addr
  lsu.io.wdata := exu.io.store_data

  // WBU
  wbu.io.is_lui   := idu.io.is_lui
  wbu.io.is_auipc := idu.io.is_auipc
  wbu.io.is_jal   := idu.io.is_jal
  wbu.io.is_jalr  := idu.io.is_jalr
  wbu.io.is_lb    := idu.io.is_lb
  wbu.io.is_lh    := idu.io.is_lh
  wbu.io.is_lw    := idu.io.is_lw
  wbu.io.is_lbu   := idu.io.is_lbu
  wbu.io.is_lhu   := idu.io.is_lhu
  wbu.io.is_addi  := idu.io.is_addi
  wbu.io.is_slti  := idu.io.is_slti
  wbu.io.is_sltiu := idu.io.is_sltiu
  wbu.io.is_xori  := idu.io.is_xori
  wbu.io.is_ori   := idu.io.is_ori
  wbu.io.is_andi  := idu.io.is_andi
  wbu.io.is_slli  := idu.io.is_slli
  wbu.io.is_srli  := idu.io.is_srli
  wbu.io.is_srai  := idu.io.is_srai
  wbu.io.is_add   := idu.io.is_add
  wbu.io.is_sub   := idu.io.is_sub
  wbu.io.is_sll   := idu.io.is_sll
  wbu.io.is_slt   := idu.io.is_slt
  wbu.io.is_sltu  := idu.io.is_sltu
  wbu.io.is_xor   := idu.io.is_xor
  wbu.io.is_srl   := idu.io.is_srl
  wbu.io.is_sra   := idu.io.is_sra
  wbu.io.is_or    := idu.io.is_or
  wbu.io.is_and   := idu.io.is_and
  wbu.io.is_csrrw  := idu.io.is_csrrw
  wbu.io.is_csrrs  := idu.io.is_csrrs
  wbu.io.is_csrrc  := idu.io.is_csrrc
  wbu.io.is_csrrwi := idu.io.is_csrrwi
  wbu.io.is_csrrsi := idu.io.is_csrrsi
  wbu.io.is_csrrci := idu.io.is_csrrci
  wbu.io.inputfromALU := exu.io.alu_result
  wbu.io.inputfromPC  := ifu.io.pctogrf
  wbu.io.inputfromRAM := lsu.io.rdata
  wbu.io.inputfromCSR := csr.io.csr_rdata

  // GRF 写回
  grf.io.rden   := wbu.io.regWen
  grf.io.rdaddr := exu.io.rd_out
  grf.io.input  := wbu.io.wbData

  // IFU 跳转、异常和 mret
  ifu.io.take_branch   := exu.io.take_branch
  ifu.io.branch_target := exu.io.branch_target
  ifu.io.take_trap     := csr.io.take_trap
  ifu.io.trap_pc       := csr.io.trap_pc
  ifu.io.take_mret     := idu.io.is_mret
  // mret PC: 直接从 CSR 模块取 mepc（debug_mepc 就是 mepc 寄存器的值）
  ifu.io.mret_pc       := csr.io.debug_mepc

  // ========== CSR 连接 ==========
  val csr_addr = idu.io.inst(31, 20)
  csr.io.csr_addr := csr_addr

  // CSR 操作类型判断
  val is_csr_write = idu.io.is_csrrw || idu.io.is_csrrs || idu.io.is_csrrc ||
                     idu.io.is_csrrwi || idu.io.is_csrrsi || idu.io.is_csrrci

  // csr_op: 0=csrrw(写), 1=csrrs(置位), 2=csrrc(清除)
  csr.io.csr_op := MuxCase(0.U(3.W), Seq(
    (idu.io.is_csrrs || idu.io.is_csrrsi) -> 1.U,
    (idu.io.is_csrrc || idu.io.is_csrrci) -> 2.U
  ))

  // CSR 写使能
  val use_imm_csr = idu.io.is_csrrwi || idu.io.is_csrrsi || idu.io.is_csrrci
  val uimm = idu.io.inst(19, 15)
  val rs1_csr_nonzero = idu.io.rs1 =/= 0.U
  val uimm_nonzero = uimm =/= 0.U

  csr.io.use_imm := use_imm_csr
  csr.io.rs1_val := exu.io.rs1_data

  // csrrw/csrrwi: 总是写入；csrrs/csrsi/csrrc/csrrci: 仅当 rs1/uimm != 0 时写入
  val csrrw_or_wi = idu.io.is_csrrw || idu.io.is_csrrwi
  val csr_write_cond = Mux(csrrw_or_wi, true.B,
                       Mux(use_imm_csr, uimm_nonzero, rs1_csr_nonzero))
  csr.io.csr_wen := is_csr_write && csr_write_cond
  csr.io.csr_waddr := csr_addr
  csr.io.csr_wdata := Mux(use_imm_csr, Cat(0.U(27.W), uimm), exu.io.rs1_data)

  // 异常接口
  csr.io.ecall      := idu.io.is_ecall
  csr.io.mret       := idu.io.is_mret
  csr.io.is_ebreak  := idu.io.is_ebreak
  csr.io.current_pc := ifu.io.current_pc

  // 指令退休计数
  val normal_inst = !idu.io.is_ebreak && !idu.io.is_ecall
  csr.io.inst_retire := normal_inst

  // debug 连接
  io.debug_pc   := ifu.io.debug_pc
  io.debug_regs := grf.io.debug_regs
  io.debug_alu_src1 := exu.io.alu_src1
  io.debug_alu_src2 := exu.io.alu_src2
  io.debug_alu_res  := exu.io.alu_result
  io.debug_regWen     := wbu.io.debug_regWen
  io.debug_wbData     := wbu.io.debug_wbData
  io.debug_grf_rden   := grf.io.debug_rden
  io.debug_grf_rdaddr := grf.io.debug_rdaddr
  io.debug_grf_input  := grf.io.debug_input
  io.debug_is_add   := idu.io.is_add
  io.debug_is_addi  := idu.io.is_addi
  io.debug_is_jalr  := idu.io.is_jalr
  io.debug_is_lui   := idu.io.is_lui
  io.debug_is_lbu   := idu.io.is_lbu
  io.debug_is_lw    := idu.io.is_lw
  io.debug_is_sw    := idu.io.is_sw
  io.debug_is_sb    := idu.io.is_sb
  io.debug_is_ebreak:= idu.io.is_ebreak
  io.debug_lsu_addr  := lsu.io.dmemAddr
  io.debug_lsu_wen   := lsu.io.dmemWen
  io.debug_lsu_wdata := lsu.io.dmemWdata
  io.debug_lsu_wmask := lsu.io.dmemWmask
  io.debug_lsu_ren   := lsu.io.dmemRen
  io.debug_lsu_rdata := lsu.io.dmemRdata
  io.debug_inst := idu.io.debug_inst

  // CSR debug 连接
  io.debug_mcycle    := csr.io.debug_mcycle
  io.debug_minstret  := csr.io.debug_minstret
  io.debug_mstatus   := csr.io.debug_mstatus
  io.debug_mie       := csr.io.debug_mie
  io.debug_mtvec     := csr.io.debug_mtvec
  io.debug_mepc      := csr.io.debug_mepc
  io.debug_mcause    := csr.io.debug_mcause
  io.debug_mtval     := csr.io.debug_mtval
  io.debug_mip       := csr.io.debug_mip
  io.debug_mscratch  := csr.io.debug_mscratch
  io.debug_mvendorid := csr.io.debug_mvendorid
  io.debug_marchid   := csr.io.debug_marchid
  io.debug_mimpid    := csr.io.debug_mimpid
  io.debug_mhartid   := csr.io.debug_mhartid
}

object top extends App {
  ChiselStage.emitSystemVerilogFile(
    new top,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}
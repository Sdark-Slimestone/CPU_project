package minirv

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

// 定义 DPI 内存黑盒，指定模块名
class DPIMemory extends BlackBox with HasBlackBoxResource {
  val io = IO(new Bundle {
    val wen   = Input(Bool())
    val waddr = Input(UInt(32.W))
    val wdata = Input(UInt(32.W))
    val wmask = Input(UInt(4.W))
    val ren   = Input(Bool())
    val raddr = Input(UInt(32.W))
    val rdata = Output(UInt(32.W))
    val ebreak = Input(Bool())
  })
  addResource("/DPI_Memory.v")
  override def desiredName = "DPI_Memory"   // 关键：固定模块名
}

class top extends Module {
  val io = IO(new Bundle {
    val clk   = Input(Clock())
    val reset = Input(Bool())
  })

  // 子模块实例化
  val ifu = Module(new IFU)
  val idu = Module(new idu)
  val exu = Module(new EXU)
  val lsu = Module(new LSU)
  val wbu = Module(new WBU)
  val grf = Module(new GRF)

  // 指令内存
  val instMem = Module(new DPIMemory)
  instMem.io.wen   := false.B
  instMem.io.waddr := 0.U
  instMem.io.wdata := 0.U
  instMem.io.wmask := 0.U
  instMem.io.ren   := true.B
  instMem.io.raddr := ifu.io.imemAddr
  ifu.io.inst      := instMem.io.rdata

  // 数据内存
  val dataMem = Module(new DPIMemory)
  dataMem.io.wen   := lsu.io.dmemWen
  dataMem.io.waddr := lsu.io.dmemAddr
  dataMem.io.wdata := lsu.io.dmemWdata
  dataMem.io.wmask := lsu.io.dmemWmask
  dataMem.io.ren   := lsu.io.dmemRen
  dataMem.io.raddr := lsu.io.dmemAddr
  lsu.io.dmemRdata := dataMem.io.rdata

  // ebreak 信号同时通知两个内存
  instMem.io.ebreak := idu.io.is_ebreak
  dataMem.io.ebreak := idu.io.is_ebreak

  // 其余连接保持不变
  idu.io.inst := ifu.io.inst

  exu.io.is_add  := idu.io.is_add
  exu.io.is_addi := idu.io.is_addi
  exu.io.is_jalr := idu.io.is_jalr
  exu.io.is_lui  := idu.io.is_lui
  exu.io.is_lbu  := idu.io.is_lbu
  exu.io.is_lw   := idu.io.is_lw
  exu.io.is_sw   := idu.io.is_sw
  exu.io.is_sb   := idu.io.is_sb

  exu.io.rs1_addr := idu.io.rs1
  exu.io.rs2_addr := idu.io.rs2
  exu.io.rd_addr  := idu.io.rd
  exu.io.imm12    := idu.io.imm12
  exu.io.imm20    := idu.io.imm20

  lsu.io.lw   := idu.io.is_lw
  lsu.io.lbu  := idu.io.is_lbu
  lsu.io.sw   := idu.io.is_sw
  lsu.io.sb   := idu.io.is_sb

  wbu.io.add  := idu.io.is_add
  wbu.io.addi := idu.io.is_addi
  wbu.io.jalr := idu.io.is_jalr
  wbu.io.lbu  := idu.io.is_lbu
  wbu.io.lw   := idu.io.is_lw
  wbu.io.lui  := idu.io.is_lui

  grf.io.rs1en   := exu.io.rs1_en
  grf.io.rs1addr := exu.io.rs1_addr_out
  grf.io.rs2en   := exu.io.rs2_en
  grf.io.rs2addr := exu.io.rs2_addr_out
  grf.io.rden    := wbu.io.regWen
  grf.io.rdaddr  := exu.io.rd_out
  grf.io.input   := wbu.io.wbData

  exu.io.rs1_data := grf.io.rs1out
  exu.io.rs2_data := grf.io.rs2out

  lsu.io.addr  := exu.io.agu_addr
  lsu.io.wdata := exu.io.store_data

  wbu.io.inputfromRAM := lsu.io.rdata
  wbu.io.inputfromALU := exu.io.alu_result
  wbu.io.inputfromPC  := ifu.io.pctogrf
  wbu.io.inputfromIDU := idu.io.imm20

  ifu.io.jalr         := exu.io.jalr_out
  ifu.io.inputfromBRU := exu.io.bru_target
}

object top extends App {
  ChiselStage.emitSystemVerilogFile(
    new top,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}
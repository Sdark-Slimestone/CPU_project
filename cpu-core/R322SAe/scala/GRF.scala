package rv32e

import chisel3._
import chisel3.util._

class GRF extends Module {
  val io = IO(new Bundle {
    // 读1
    val rs1en   = Input(Bool())
    val rs1addr = Input(UInt(5.W))
    val rs1out  = Output(UInt(32.W))
    // 读2
    val rs2en   = Input(Bool())
    val rs2addr = Input(UInt(5.W))
    val rs2out  = Output(UInt(32.W))
    // 读3
    val rs3en   = Input(Bool())
    val rs3addr = Input(UInt(5.W))
    val rs3out  = Output(UInt(32.W))
    // 读4
    val rs4en   = Input(Bool())
    val rs4addr = Input(UInt(5.W))
    val rs4out  = Output(UInt(32.W))
    // 写1
    val rd1en   = Input(Bool())
    val rd1addr = Input(UInt(5.W))
    val rd1data = Input(UInt(32.W))
    // 写2
    val rd2en   = Input(Bool())
    val rd2addr = Input(UInt(5.W))
    val rd2data = Input(UInt(32.W))
    // 调试
    val debug_regs   = Output(Vec(16, UInt(32.W)))
    val debug_rden   = Output(Bool())
    val debug_rdaddr = Output(UInt(5.W))
    val debug_input  = Output(UInt(32.W))
  })

  val regs = RegInit(VecInit(Seq.fill(16)(0.U(32.W))))

  
  io.rs1out := Mux(io.rs1en && (io.rs1addr(4) === 0.U) && (io.rs1addr =/= 0.U), regs(io.rs1addr(3,0)), 0.U(32.W))
  io.rs2out := Mux(io.rs2en && (io.rs2addr(4) === 0.U) && (io.rs2addr =/= 0.U), regs(io.rs2addr(3,0)), 0.U(32.W))
  io.rs3out := Mux(io.rs3en && (io.rs3addr(4) === 0.U) && (io.rs3addr =/= 0.U), regs(io.rs3addr(3,0)), 0.U(32.W))
  io.rs4out := Mux(io.rs4en && (io.rs4addr(4) === 0.U) && (io.rs4addr =/= 0.U), regs(io.rs4addr(3,0)), 0.U(32.W))

  // 写（合并两个端口，避免多驱动）
  val nextRegs = Wire(Vec(16, UInt(32.W)))
  for (i <- 0 until 16) {
    val wr1 = io.rd1en && (io.rd1addr(4) === 0.U) && (io.rd1addr(3,0) === i.U) && (io.rd1addr =/= 0.U)
    val wr2 = io.rd2en && (io.rd2addr(4) === 0.U) && (io.rd2addr(3,0) === i.U) && (io.rd2addr =/= 0.U)
    nextRegs(i) := Mux(wr1, io.rd1data, Mux(wr2, io.rd2data, regs(i)))
  }
  regs := nextRegs

  // 调试
  io.debug_regs   := regs
  io.debug_rden   := io.rd1en
  io.debug_rdaddr := io.rd1addr
  io.debug_input  := io.rd1data
}
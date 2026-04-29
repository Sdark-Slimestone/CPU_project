package minirv

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

class GRF extends Module {
  val io = IO(new Bundle {
    //出寄存器
    val rs1en   = Input(Bool())
    val rs1addr = Input(UInt(5.W))
    val rs1out  = Output(UInt(32.W))
    val rs2en   = Input(Bool())
    val rs2addr = Input(UInt(5.W))
    val rs2out  = Output(UInt(32.W))
    //入寄存器
    val rden   = Input(Bool())
    val rdaddr = Input(UInt(5.W))
    val input  = Input(UInt(32.W))
    val debug_regs = Output(Vec(16, UInt(32.W)))

    val debug_rden   = Output(Bool())
    val debug_rdaddr = Output(UInt(5.W))
    val debug_input  = Output(UInt(32.W))
  })

  val regs = RegInit(VecInit(Seq.fill(16)(0.U(32.W))))

  io.rs1out := Mux(io.rs1en && (io.rs1addr(4) === 0.U) && (io.rs1addr =/= 0.U), regs(io.rs1addr(3,0)), 0.U(32.W))
  io.rs2out := Mux(io.rs2en && (io.rs2addr(4) === 0.U) && (io.rs2addr =/= 0.U), regs(io.rs2addr(3,0)), 0.U(32.W))

  regs(io.rdaddr(3,0)) := Mux(io.rden && (io.rdaddr(4) === 0.U) && (io.rdaddr =/= 0.U), io.input, regs(io.rdaddr(3,0)))

  io.debug_regs := regs

  io.debug_rden   := io.rden
  io.debug_rdaddr := io.rdaddr
  io.debug_input  := io.input
}

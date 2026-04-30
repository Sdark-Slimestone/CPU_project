package rv32e

import chisel3._
import chisel3.util._

class LSU extends Module {
  val io = IO(new Bundle {
    val lb    = Input(Bool())
    val lh    = Input(Bool())
    val lw    = Input(Bool())
    val lbu   = Input(Bool())
    val lhu   = Input(Bool())
    val sb    = Input(Bool())
    val sh    = Input(Bool())
    val sw    = Input(Bool())

    val addr  = Input(UInt(32.W))
    val wdata = Input(UInt(32.W))      // rs2 原始数据

    val dmemAddr  = Output(UInt(32.W))
    val dmemWen   = Output(Bool())
    val dmemWdata = Output(UInt(32.W))
    val dmemWmask = Output(UInt(4.W))
    val dmemRen   = Output(Bool())
    val dmemRdata = Input(UInt(32.W))

    val rdata = Output(UInt(32.W))
  })

  val lower2 = io.addr(1, 0)

  // 读写使能：store 指令不再需要提前读内存（所有合并逻辑移入仿真）
  io.dmemWen := io.sb || io.sh || io.sw
  io.dmemRen := io.lb || io.lh || io.lw || io.lbu || io.lhu   // 仅 load 才读
  io.dmemAddr := io.addr

  // 写掩码生成（与 minirv 一致）
  io.dmemWmask := MuxCase(0.U(4.W), Seq(
    io.sw -> "b1111".U(4.W),
    io.sh -> Mux(lower2(1), "b1100".U(4.W), "b0011".U(4.W)),
    io.sb -> (1.U(4.W) << lower2)
  ))

  // 写数据：直接传递原始 rs2 数据（不移位）
  io.dmemWdata := io.wdata

  // ---------- 读数据通路（与原先相同） ----------
  val aligned = io.dmemRdata
  val byte0 = aligned(7,0)
  val byte1 = aligned(15,8)
  val byte2 = aligned(23,16)
  val byte3 = aligned(31,24)
  val half0 = Cat(byte1, byte0)
  val half1 = Cat(byte3, byte2)

  val byte_sel = Mux(lower2(1),
    Mux(lower2(0), byte3, byte2),
    Mux(lower2(0), byte1, byte0)
  )
  val half_sel = Mux(lower2(1), half1, half0)

  io.rdata := MuxCase(0.U(32.W), Seq(
    io.lw   -> aligned,
    io.lh   -> Cat(Fill(16, half_sel(15)), half_sel),
    io.lhu  -> Cat(0.U(16.W), half_sel),
    io.lb   -> Cat(Fill(24, byte_sel(7)), byte_sel),
    io.lbu  -> Cat(0.U(24.W), byte_sel)
  ))
}
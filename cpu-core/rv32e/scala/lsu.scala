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

  // 地址对齐和字节偏移
  val aligned_addr = io.addr & ~3.U(32.W)
  val byte_offset = io.addr(1, 0)

  // 读写使能
  io.dmemWen := io.sb || io.sh || io.sw
  io.dmemRen := io.lb || io.lh || io.lw || io.lbu || io.lhu

  // 对齐后的地址输出
  io.dmemAddr := aligned_addr

  // 写数据移位（小端序：将数据左移到对应的字节位置）
  val shifted_wdata = MuxCase(io.wdata, Seq(
    (io.sb) -> (io.wdata << (byte_offset << 3)),   // byte_offset * 8
    (io.sh) -> (io.wdata << (byte_offset << 3))
  ))
  io.dmemWdata := shifted_wdata

  // 写掩码生成
  io.dmemWmask := MuxCase(0.U(4.W), Seq(
    io.sw -> "b1111".U(4.W),
    io.sh -> Mux(byte_offset === 0.U, "b0011".U(4.W),
             Mux(byte_offset === 2.U, "b1100".U(4.W), 0.U(4.W))),
    io.sb -> (1.U(4.W) << byte_offset)
  ))

  // 读数据通路
  val aligned = io.dmemRdata
  val byte0 = aligned(7,0)
  val byte1 = aligned(15,8)
  val byte2 = aligned(23,16)
  val byte3 = aligned(31,24)
  val half0 = Cat(byte1, byte0)
  val half1 = Cat(byte3, byte2)

  val byte_sel = Mux(byte_offset(1),
    Mux(byte_offset(0), byte3, byte2),
    Mux(byte_offset(0), byte1, byte0)
  )
  val half_sel = Mux(byte_offset(1), half1, half0)

  io.rdata := MuxCase(0.U(32.W), Seq(
    io.lw   -> aligned,
    io.lh   -> Cat(Fill(16, half_sel(15)), half_sel),
    io.lhu  -> Cat(0.U(16.W), half_sel),
    io.lb   -> Cat(Fill(24, byte_sel(7)), byte_sel),
    io.lbu  -> Cat(0.U(24.W), byte_sel)
  ))
}
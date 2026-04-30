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

  // 读写使能：store 指令也需要读内存以获取旧值（用于字节合并）
  io.dmemWen := io.sb || io.sh || io.sw
  io.dmemRen := io.lb || io.lh || io.lw || io.lbu || io.lhu || io.sb || io.sh
  io.dmemAddr := io.addr

  // 写掩码（原有逻辑）
  io.dmemWmask := MuxCase(0.U(4.W), Seq(
    io.sw -> "b1111".U(4.W),
    io.sh -> Mux(lower2(1), "b1100".U(4.W), "b0011".U(4.W)),
    io.sb -> (1.U(4.W) << lower2)
  ))

  // ---------- 写数据生成（纯 Mux） ----------
  val data_byte = io.wdata(7, 0)
  val data_half = io.wdata(15, 0)
  val wdata_shifted = MuxCase(io.wdata, Seq(
    io.sb -> (data_byte << (lower2 * 8.U)),
    io.sh -> Mux(lower2(1), (data_half << 16.U), data_half)
  ))

  // 逐字节合并新旧数据
  val mask0 = io.dmemWmask(0)
  val mask1 = io.dmemWmask(1)
  val mask2 = io.dmemWmask(2)
  val mask3 = io.dmemWmask(3)

  val merged_byte0 = Mux(mask0, wdata_shifted(7,0),   io.dmemRdata(7,0))
  val merged_byte1 = Mux(mask1, wdata_shifted(15,8),  io.dmemRdata(15,8))
  val merged_byte2 = Mux(mask2, wdata_shifted(23,16), io.dmemRdata(23,16))
  val merged_byte3 = Mux(mask3, wdata_shifted(31,24), io.dmemRdata(31,24))

  io.dmemWdata := Cat(merged_byte3, merged_byte2, merged_byte1, merged_byte0)

  // ---------- 读数据通路 ----------
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
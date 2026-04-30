package minirv

import chisel3._
import chisel3.util._

class LSU extends Module {
  val io = IO(new Bundle {
    // 控制信号
    val lw   = Input(Bool())
    val lbu  = Input(Bool())
    val sw   = Input(Bool())
    val sb   = Input(Bool())

    // 地址和数据
    val addr  = Input(UInt(32.W))   // 来自 AGU 的计算结果
    val wdata = Input(UInt(32.W))   // 来自 rs2

    // 与数据存储器的接口（顶层连接 DMEM）
    val dmemAddr  = Output(UInt(32.W))
    val dmemWen   = Output(Bool())
    val dmemWdata = Output(UInt(32.W))
    val dmemWmask = Output(UInt(4.W))
    val dmemRen   = Output(Bool())
    val dmemRdata = Input(UInt(32.W))

    // 输出给 WBU 的读数据
    val rdata = Output(UInt(32.W))
  })

  // 默认值
  io.dmemAddr  := io.addr
  io.dmemWdata := io.wdata
  io.dmemWen   := io.sw || io.sb
  io.dmemRen   := io.lw || io.lbu

  // 写掩码生成
  val lower2 = io.addr(1, 0)
  io.dmemWmask := MuxCase(0.U(4.W), Seq(
    io.sw -> "b1111".U,
    io.sb -> (1.U << lower2)
  ))

  // 读数据处理：从 32 位对齐数据中提取需要的字节/字
  val alignedData = io.dmemRdata  
  val byteSel = io.addr(1, 0)  //取地址低两位得知需要哪个字节
  val byteData = ((alignedData >> (byteSel << 3)) & 0xFF.U)   //bytesel x 8 = 实际需要右移的位数 ，移动完就高位清0

  io.rdata := MuxCase(0.U(32.W), Seq(
    io.lw  -> alignedData,
    io.lbu -> byteData
  ))
}
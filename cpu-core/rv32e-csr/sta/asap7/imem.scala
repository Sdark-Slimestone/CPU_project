package rv32ecsr

import chisel3._
import chisel3.util._

// ASAP7 评估用的 RegisterFile 触发器阵列存储器黑盒 (PDK 无关)
class RegisterFile extends BlackBox {
  val io = IO(new Bundle {
    val io_clk = Input(Clock())
    val wen    = Input(Bool())
    val waddr  = Input(UInt(8.W))
    val wdata  = Input(UInt(32.W))
    val wmask  = Input(UInt(4.W))
    val raddr1 = Input(UInt(8.W))
    val rdata1 = Output(UInt(32.W))
    val raddr2 = Input(UInt(8.W))
    val rdata2 = Output(UInt(32.W))
  })
  override def desiredName = "RegisterFile"
}

// 指令存储器 (ASAP7 版): RegisterFile, 接口与原 DPIMemory 一致。
class imem extends Module {
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

  val register_file = Module(new RegisterFile)
  register_file.io.io_clk := io.io_clk
  val load_counter = RegInit(0.U(8.W))
  load_counter := load_counter + 1.U
  register_file.io.wen   := load_counter === "hFF".U(8.W)
  register_file.io.waddr := load_counter
  register_file.io.wdata := 0.U
  register_file.io.wmask := "b1111".U(4.W)
  register_file.io.raddr1 := io.raddr(9, 2)
  register_file.io.raddr2 := 0.U
  io.rdata := register_file.io.rdata1
}

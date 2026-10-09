package R1322IAeCSR

import chisel3._
import chisel3.util._

// ASAP7 评估用的 RegisterFile 触发器阵列存储器黑盒 (PDK 无关, Verilog 见 sta/resources/RegisterFile.v)
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

// 指令存储器 (ASAP7 版): 双端口读, RegisterFile 触发器阵列 (无 SRAM 宏)。
class imem extends Module {
  val io = IO(new Bundle {
    val ifu_to_imem = new Bundle {
      val addr1 = Input(UInt(32.W))
      val addr2 = Input(UInt(32.W))
    }
    val imem_to_ifu = new Bundle {
      val inst1 = Output(UInt(32.W))
      val inst2 = Output(UInt(32.W))
    }
  })

  val register_file = Module(new RegisterFile)
  register_file.io.io_clk := clock

  // 自由运行加载计数器占住写口, 防综合折叠掉阵列
  val load_counter = RegInit(0.U(8.W))
  load_counter := load_counter + 1.U
  register_file.io.wen   := load_counter === "hFF".U(8.W)
  register_file.io.waddr := load_counter
  register_file.io.wdata := 0.U
  register_file.io.wmask := "b1111".U(4.W)

  register_file.io.raddr1 := io.ifu_to_imem.addr1(9, 2)
  register_file.io.raddr2 := io.ifu_to_imem.addr2(9, 2)
  io.imem_to_ifu.inst1 := register_file.io.rdata1
  io.imem_to_ifu.inst2 := register_file.io.rdata2
}

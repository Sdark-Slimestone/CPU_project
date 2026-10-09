package R1322IAeCSR

import chisel3._
import chisel3.util._

// 寄存器阵列存储器黑盒 (Verilog 实现见 sta/resources/RegisterFile.v)
// 256x32b 触发器阵列, 1个写字节使能端口 + 2个组合读端口, 当前周期返回读数据
// (参考讲义"评估单周期NPC的主频": 像寄存器堆那样通过触发器实现存储器)
// 仅用于 make sta 综合评估, 不参与日常 npc 构建 (日常构建用 DPI-C 软件RAM版)
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

// 指令存储器，双端口读（支持双发射取指）
// STA 评估版: 用 RegisterFile 寄存器阵列替换 DPI-C 的 pmem_read() 访问,
// 组合读、当前周期返回数据, 以保持单周期特性; I/O 相关部分(ebreak 等)忽略
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

  // 程序加载写端口(讲义中 RegisterFile 的写口): 由一个与数据通路无关的
  // 自由运行加载计数器驱动, 综合器无法把该写使能折叠成常数, 从而保留
  // 真实的触发器阵列 (否则无写端口的存储器会被综合优化掉, 讲义也指出了这一点)
  val load_counter = RegInit(0.U(8.W))
  load_counter := load_counter + 1.U
  register_file.io.wen   := load_counter === "hFF".U(8.W)
  register_file.io.waddr := load_counter
  register_file.io.wdata := 0.U
  register_file.io.wmask := "b1111".U(4.W)

  // 组合读: 两个读端口
  register_file.io.raddr1 := io.ifu_to_imem.addr1(9, 2)
  register_file.io.raddr2 := io.ifu_to_imem.addr2(9, 2)
  io.imem_to_ifu.inst1 := register_file.io.rdata1
  io.imem_to_ifu.inst2 := register_file.io.rdata2
}

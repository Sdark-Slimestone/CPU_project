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

// 单端口同步 SRAM 宏黑盒 (时序/面积见 pdk/nangate45/lib/fakeram45_256x34.lib)
// 256x34b, 1 个共享地址端口 (读/写), 同步读 (clk 上升沿后一拍出数据)
class fakeram45_256x34 extends BlackBox {
  val io = IO(new Bundle {
    val clk       = Input(Clock())
    val addr_in   = Input(UInt(8.W))
    val wd_in     = Input(UInt(34.W))
    val w_mask_in = Input(UInt(34.W))
    val we_in     = Input(Bool())
    val ce_in     = Input(Bool())
    val rd_out    = Output(UInt(34.W))
  })
  override def desiredName = "fakeram45_256x34"
}

// 指令存储器，双端口读（支持双发射取指）
// STA 评估版: 用 fakeram45 同步 SRAM 硬宏替换触发器阵列, 以反映真实 SRAM 时序/面积。
// 注意: 宏为同步读, 数据晚一拍返回 —— 为保持 5 周期, IFU 需提前一拍发起读。
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

  val mem1 = Module(new fakeram45_256x34)
  val mem2 = Module(new fakeram45_256x34)

  for ((m, a) <- Seq((mem1, io.ifu_to_imem.addr1), (mem2, io.ifu_to_imem.addr2))) {
    m.io.clk       := clock
    m.io.addr_in   := a(9, 2)
    m.io.wd_in     := 0.U
    m.io.w_mask_in := 0.U
    m.io.we_in     := false.B
    m.io.ce_in     := true.B
  }

  io.imem_to_ifu.inst1 := mem1.io.rd_out(31, 0)
  io.imem_to_ifu.inst2 := mem2.io.rd_out(31, 0)
}

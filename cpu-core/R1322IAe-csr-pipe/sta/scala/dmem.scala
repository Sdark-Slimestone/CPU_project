package R1322IAeCSR

import chisel3._
import chisel3.util._

// 数据存储器 DMEM，支持两个 LSU 端口（读/写），内部合并写请求到唯一写端口
// STA 评估版: 用 RegisterFile 寄存器阵列替换 DPI-C 的 pmem_read()/pmem_write() 访问,
// 组合读、字节使能写; I/O 相关部分(ebreak 上报等)在主频评估中忽略
// 仅用于 make sta 综合评估, 不参与日常 npc 构建 (日常构建用 DPI-C 软件RAM版)
class dmem extends Module {
  val io = IO(new Bundle {
    val dmem_to_lsu_1 = new Bundle {
      val load_data = Output(UInt(32.W))
    }
    val dmem_to_lsu_2 = new Bundle {
      val load_data = Output(UInt(32.W))
    }

    val lsu_to_dmem_1 = new Bundle {
      val addr       = Input(UInt(32.W))
      val store_data = Input(UInt(32.W))
      val mask       = Input(UInt(4.W))
      val wen        = Input(Bool())
      val ren        = Input(Bool())
    }
    val lsu_to_dmem_2 = new Bundle {
      val addr       = Input(UInt(32.W))
      val store_data = Input(UInt(32.W))
      val mask       = Input(UInt(4.W))
      val wen        = Input(Bool())
      val ren        = Input(Bool())
    }
    val ebreak = Input(Bool())
  })

  val register_file = Module(new RegisterFile)
  register_file.io.io_clk := clock

  // 写请求合并(与原设计一致: 端口1优先)
  register_file.io.wen := io.lsu_to_dmem_1.wen || io.lsu_to_dmem_2.wen
  register_file.io.waddr := Mux(io.lsu_to_dmem_1.wen,
                                io.lsu_to_dmem_1.addr(9, 2),
                                io.lsu_to_dmem_2.addr(9, 2))
  register_file.io.wdata := Mux(io.lsu_to_dmem_1.wen,
                                io.lsu_to_dmem_1.store_data,
                                io.lsu_to_dmem_2.store_data)
  register_file.io.wmask := Mux(io.lsu_to_dmem_1.wen,
                                io.lsu_to_dmem_1.mask,
                                io.lsu_to_dmem_2.mask)

  // 组合读: 两个读端口(当前周期返回数据)
  register_file.io.raddr1 := io.lsu_to_dmem_1.addr(9, 2)
  register_file.io.raddr2 := io.lsu_to_dmem_2.addr(9, 2)
  io.dmem_to_lsu_1.load_data := register_file.io.rdata1
  io.dmem_to_lsu_2.load_data := register_file.io.rdata2
}

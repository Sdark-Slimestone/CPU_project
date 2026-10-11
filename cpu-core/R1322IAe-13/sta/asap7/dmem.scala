package R1322IAeCSR

import chisel3._
import chisel3.util._

// 数据存储器 (ASAP7 版): 只做存储, RegisterFile 触发器阵列 (无 SRAM 宏)。
class dmem extends Module {
  val io = IO(new Bundle {
    val exu_to_dmem_1 = new Bundle { val addr = Input(UInt(32.W)); val ren = Input(Bool()) }
    val exu_to_dmem_2 = new Bundle { val addr = Input(UInt(32.W)); val ren = Input(Bool()) }

    val lsu_to_dmem_1 = new Bundle {
      val addr = Input(UInt(32.W)); val store_data = Input(UInt(32.W))
      val mask = Input(UInt(4.W)); val wen = Input(Bool())
    }
    val lsu_to_dmem_2 = new Bundle {
      val addr = Input(UInt(32.W)); val store_data = Input(UInt(32.W))
      val mask = Input(UInt(4.W)); val wen = Input(Bool())
    }

    val dmem_rdata_1 = Output(UInt(32.W))
    val dmem_rdata_2 = Output(UInt(32.W))

    val ebreak = Input(Bool())
  })

  val register_file = Module(new RegisterFile)
  register_file.io.io_clk := clock

  register_file.io.wen   := io.lsu_to_dmem_1.wen || io.lsu_to_dmem_2.wen
  register_file.io.waddr := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.addr,       io.lsu_to_dmem_2.addr)(9, 2)
  register_file.io.wdata := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.store_data, io.lsu_to_dmem_2.store_data)
  register_file.io.wmask := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.mask,       io.lsu_to_dmem_2.mask)

  register_file.io.raddr1 := io.exu_to_dmem_1.addr(9, 2)
  register_file.io.raddr2 := io.exu_to_dmem_2.addr(9, 2)
  io.dmem_rdata_1 := register_file.io.rdata1
  io.dmem_rdata_2 := register_file.io.rdata2
}

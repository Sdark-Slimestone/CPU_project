package R322SAe

import chisel3._
import chisel3.util._

// 数据存储器 DMEM，支持两个 LSU 端口（读/写），内部合并写请求到唯一写端口
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

  val memory = Module(new DPIMemory)

  memory.io.io_clk := clock

  memory.io.ren1   := io.lsu_to_dmem_1.ren
  memory.io.raddr1 := io.lsu_to_dmem_1.addr
  io.dmem_to_lsu_1.load_data := memory.io.rdata1

  memory.io.ren2   := io.lsu_to_dmem_2.ren
  memory.io.raddr2 := io.lsu_to_dmem_2.addr
  io.dmem_to_lsu_2.load_data := memory.io.rdata2

  memory.io.wen := io.lsu_to_dmem_1.wen || io.lsu_to_dmem_2.wen

  memory.io.waddr := Mux(io.lsu_to_dmem_1.wen,
                         io.lsu_to_dmem_1.addr,
                         io.lsu_to_dmem_2.addr)
  memory.io.wdata := Mux(io.lsu_to_dmem_1.wen,
                         io.lsu_to_dmem_1.store_data,
                         io.lsu_to_dmem_2.store_data)
  memory.io.wmask := Mux(io.lsu_to_dmem_1.wen,
                         io.lsu_to_dmem_1.mask,
                         io.lsu_to_dmem_2.mask)

  memory.io.ebreak := io.ebreak 
}
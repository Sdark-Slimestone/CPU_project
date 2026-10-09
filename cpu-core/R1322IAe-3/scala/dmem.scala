package R1322IAeCSR

import chisel3._
import chisel3.util._

// 数据存储器 DMEM。
// 同步 SRAM 时序: 读请求在 E 级发出 (地址来自 EXU), 数据在 M 级返回;
// 写请求在 M 级发出 (来自 LSU), 在 M/W 边沿写入。单端口宏上读/写不同拍, 不冲突。
// store-to-load 转发: M 级把 lane1 的 store 数据合并进 lane2 的 load 结果。
class dmem extends Module {
  val io = IO(new Bundle {
    val dmem_to_lsu_1 = new Bundle {
      val load_data = Output(UInt(32.W))
    }
    val dmem_to_lsu_2 = new Bundle {
      val load_data = Output(UInt(32.W))
    }

    // 读请求 (EXU, E 级)
    val exu_to_dmem_1 = new Bundle {
      val addr = Input(UInt(32.W))
      val ren  = Input(Bool())
    }
    val exu_to_dmem_2 = new Bundle {
      val addr = Input(UInt(32.W))
      val ren  = Input(Bool())
    }

    // 写请求 (LSU, M 级)
    val lsu_to_dmem_1 = new Bundle {
      val addr       = Input(UInt(32.W))
      val store_data = Input(UInt(32.W))
      val mask       = Input(UInt(4.W))
      val wen        = Input(Bool())
    }
    val lsu_to_dmem_2 = new Bundle {
      val addr       = Input(UInt(32.W))
      val store_data = Input(UInt(32.W))
      val mask       = Input(UInt(4.W))
      val wen        = Input(Bool())
    }
    val ebreak = Input(Bool())
  })

  val memory = Module(new DPIMemory)

  memory.io.io_clk := clock
  memory.io.ren1   := io.exu_to_dmem_1.ren
  memory.io.raddr1 := io.exu_to_dmem_1.addr
  memory.io.ren2   := io.exu_to_dmem_2.ren
  memory.io.raddr2 := io.exu_to_dmem_2.addr

  memory.io.wen   := io.lsu_to_dmem_1.wen || io.lsu_to_dmem_2.wen
  memory.io.waddr := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.addr,       io.lsu_to_dmem_2.addr)
  memory.io.wdata := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.store_data, io.lsu_to_dmem_2.store_data)
  memory.io.wmask := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.mask,       io.lsu_to_dmem_2.mask)
  memory.io.ebreak := io.ebreak

  io.dmem_to_lsu_1.load_data := memory.io.rdata1

  // 读地址寄存一拍: E 级发出的 lane2 读地址在 M 级与 lane1 的写地址比较
  val rd2_reg = RegNext(io.exu_to_dmem_2.addr & "hFFFFFFFC".U(32.W))
  val fwd_mask = Cat(Fill(8, io.lsu_to_dmem_1.mask(3)),
                     Fill(8, io.lsu_to_dmem_1.mask(2)),
                     Fill(8, io.lsu_to_dmem_1.mask(1)),
                     Fill(8, io.lsu_to_dmem_1.mask(0)))
  val fwd = io.lsu_to_dmem_1.wen && (io.lsu_to_dmem_1.addr === rd2_reg)
  val rdata2_fwd = (memory.io.rdata2 & ~fwd_mask) | (io.lsu_to_dmem_1.store_data & fwd_mask)
  io.dmem_to_lsu_2.load_data := Mux(fwd, rdata2_fwd, memory.io.rdata2)

}

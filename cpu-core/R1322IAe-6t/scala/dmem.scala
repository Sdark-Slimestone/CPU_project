package R1322IAeCSR

import chisel3._
import chisel3.util._

// 数据存储器 DMEM (只做存储, 不做转发)。
// 同步 SRAM 时序: 读请求在 E 级发出 (地址来自 EXU), 数据在 M 级返回 (dmem_rdata_*);
// 写请求在 M 级发出 (来自 LSU)。store->load 转发由独立的 StoreBuffer 模块完成。
class dmem extends Module {
  val io = IO(new Bundle {
    // 读请求 (EXU, E 级)
    val exu_to_dmem_1 = new Bundle { val addr = Input(UInt(32.W)); val ren = Input(Bool()) }
    val exu_to_dmem_2 = new Bundle { val addr = Input(UInt(32.W)); val ren = Input(Bool()) }

    // 写请求 (LSU, M 级)
    val lsu_to_dmem_1 = new Bundle {
      val addr = Input(UInt(32.W)); val store_data = Input(UInt(32.W))
      val mask = Input(UInt(4.W)); val wen = Input(Bool())
    }
    val lsu_to_dmem_2 = new Bundle {
      val addr = Input(UInt(32.W)); val store_data = Input(UInt(32.W))
      val mask = Input(UInt(4.W)); val wen = Input(Bool())
    }

    // 原始读数据 (M 级), 交给 StoreBuffer 做转发合并
    val dmem_rdata_1 = Output(UInt(32.W))
    val dmem_rdata_2 = Output(UInt(32.W))

    val ebreak = Input(Bool())
    val flush  = Input(Bool())
  })

  val memory = Module(new DPIMemory(MemCfg.latency))
  memory.io.io_clk := clock
  memory.io.reqValid1 := io.exu_to_dmem_1.ren
  memory.io.raddr1 := io.exu_to_dmem_1.addr
  memory.io.reqValid2 := io.exu_to_dmem_2.ren
  memory.io.raddr2 := io.exu_to_dmem_2.addr
  memory.io.respReady1 := true.B
  memory.io.respReady2 := true.B
  memory.io.flush := io.flush
  memory.io.wen   := io.lsu_to_dmem_1.wen || io.lsu_to_dmem_2.wen
  memory.io.waddr := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.addr,       io.lsu_to_dmem_2.addr)
  memory.io.wdata := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.store_data, io.lsu_to_dmem_2.store_data)
  memory.io.wmask := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.mask,       io.lsu_to_dmem_2.mask)
  memory.io.ebreak := io.ebreak

  io.dmem_rdata_1 := memory.io.rdata1
  io.dmem_rdata_2 := memory.io.rdata2
}

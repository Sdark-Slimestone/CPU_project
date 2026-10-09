package R1322IAeCSR

import chisel3._
import chisel3.util._

// 数据存储器 DMEM (只做存储, 不做转发)。
// 读: E 级由 EXU 发 addr+reqValid, 存储器按 SimpleBus 在随机拍数后 respValid 返回数据;
//     load 在 E 级等到 respValid 才放行, 到 M 级由 LSU 读取数据并 respReady 消费。
// 写: M 级由 LSU 发出 (立即完成)。
// store->load 转发由独立的 StoreBuffer 模块完成。
class dmem extends Module {
  val io = IO(new Bundle {
    // 读请求 (EXU, E 级)
    val exu_to_dmem_1 = new Bundle { val addr = Input(UInt(32.W)); val ren = Input(Bool()); val tag = Input(UInt(16.W)) }
    val exu_to_dmem_2 = new Bundle { val addr = Input(UInt(32.W)); val ren = Input(Bool()); val tag = Input(UInt(16.W)) }

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
    // load 响应有效 (在 E 等到) 与消费 (到 M 时)
    val load_respValid_1 = Output(Bool())
    val load_respValid_2 = Output(Bool())
    val load_respTag_1   = Output(UInt(16.W))
    val load_respTag_2   = Output(UInt(16.W))
    val load_respReady_1 = Input(Bool())
    val load_respReady_2 = Input(Bool())

    val ebreak = Input(Bool())
    val flush  = Input(Bool())
    val dbg_a1 = Output(UInt(32.W))
    val dbg_hd1 = Output(Bool())
    val dbg_cnt1 = Output(UInt(9.W))
  })

  val memory = Module(new DPIMemory(MemCfg.latency, 1, 1))
  memory.io.io_clk := clock
  memory.io.reqValid1 := io.exu_to_dmem_1.ren
  memory.io.raddr1 := io.exu_to_dmem_1.addr
  memory.io.reqTag1 := io.exu_to_dmem_1.tag
  memory.io.reqValid2 := io.exu_to_dmem_2.ren
  memory.io.raddr2 := io.exu_to_dmem_2.addr
  memory.io.reqTag2 := io.exu_to_dmem_2.tag
  memory.io.respReady1 := io.load_respReady_1
  memory.io.respReady2 := io.load_respReady_2
  memory.io.flush := io.flush
  memory.io.wen   := io.lsu_to_dmem_1.wen || io.lsu_to_dmem_2.wen
  memory.io.waddr := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.addr,       io.lsu_to_dmem_2.addr)
  memory.io.wdata := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.store_data, io.lsu_to_dmem_2.store_data)
  memory.io.wmask := Mux(io.lsu_to_dmem_1.wen, io.lsu_to_dmem_1.mask,       io.lsu_to_dmem_2.mask)
  memory.io.ebreak := io.ebreak

  io.dmem_rdata_1 := memory.io.rdata1
  io.dmem_rdata_2 := memory.io.rdata2
  io.load_respValid_1 := memory.io.respValid1
  io.load_respValid_2 := memory.io.respValid2
  io.load_respTag_1 := memory.io.respTag1
  io.load_respTag_2 := memory.io.respTag2
  io.dbg_a1 := memory.io.dbg_a1
  io.dbg_hd1 := memory.io.dbg_hd1
  io.dbg_cnt1 := memory.io.dbg_cnt1
}

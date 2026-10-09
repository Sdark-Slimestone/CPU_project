package R1322IAeCSR

import chisel3._
import chisel3.util._

// 教科书标准转发 (forwarding/bypass) 单元, 每条 lane 一个。
// 消费者在 E 级, 操作数来源优先级:
//   1. M 级生产者 (EX/MEM 寄存器) 的 ALU 结果 -- 更年轻, 优先
//   2. W 级生产者 (MEM/WB 寄存器) 的写回值 (含 load 数据)
//   3. 都不命中 -> 用 D/E 寄存器里寄存器堆读出的值
// 同组内 lane2 (更年轻) 优先。E 级即时的 load/CSR 生产者由 IDU 停 1 拍处理, 不在这里转发。
class LaneForward extends Module {
  val io = IO(new Bundle {
    val in  = Flipped(Decoupled(new IDU_to_EXU_Lane_Message))
    val out = Decoupled(new IDU_to_EXU_Lane_Message)

    // M 级生产者 (E/M 流水寄存器输出)
    val m_rd     = Input(Vec(2, UInt(5.W)))
    val m_valid  = Input(Vec(2, Bool()))
    val m_isLoad = Input(Vec(2, Bool()))
    val m_data   = Input(Vec(2, UInt(32.W)))

    // W 级生产者 (M/W 流水寄存器输出)
    val w_rd     = Input(Vec(2, UInt(5.W)))
    val w_valid  = Input(Vec(2, Bool()))
    val w_data   = Input(Vec(2, UInt(32.W)))
  })

  io.in.ready := true.B
  io.out.valid := io.in.valid
  io.out.bits  := io.in.bits

  def forward(src: UInt, grf_val: UInt): UInt = {
    // M 级: load 的 wb_data 还是地址, 不能转发 (load-use 已由 IDU 停 1 拍)
    val hit_m = (0 until 2).map(i =>
      io.m_valid(i) && !io.m_isLoad(i) && (io.m_rd(i) =/= 0.U) && (src === io.m_rd(i)))
    val hit_w = (0 until 2).map(i =>
      io.w_valid(i) && (io.w_rd(i) =/= 0.U) && (src === io.w_rd(i)))
    val sel_m = Mux(hit_m(1), io.m_data(1), io.m_data(0))
    val sel_w = Mux(hit_w(1), io.w_data(1), io.w_data(0))
    Mux(hit_m.reduce(_ || _), sel_m,
    Mux(hit_w.reduce(_ || _), sel_w, grf_val))
  }

  io.out.bits.rs1_val := forward(io.in.bits.rs1_addr, io.in.bits.rs1_val)
  io.out.bits.rs2_val := forward(io.in.bits.rs2_addr, io.in.bits.rs2_val)
}

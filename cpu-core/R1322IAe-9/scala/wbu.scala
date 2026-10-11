package R1322IAeCSR

import chisel3._
import chisel3.util._

// 双发射写回单元: 处理两条 lane 的写回冲突并输出到双端口 GRF。
// CSR 指令的写回由 top 直接处理 (CSR 写 GRF), 不经过此模块。
class WBU extends Module {
  val io = IO(new Bundle {
    val wbu_to_grf = new Bundle {
      val wr1 = new Bundle {
        val addr = Output(UInt(5.W))
        val data = Output(UInt(32.W))
      }
      val wr2 = new Bundle {
        val addr = Output(UInt(5.W))
        val data = Output(UInt(32.W))
      }
    }

    val lsu_to_wbu_1 = Flipped(Decoupled(new LSU_to_WBU_Lane_Message))
    val lsu_to_wbu_2 = Flipped(Decoupled(new LSU_to_WBU_Lane_Message))

    val debug = new Bundle {
      val valid1   = Output(Bool())
      val valid2   = Output(Bool())
      val conflict = Output(Bool())
      val rd1      = Output(UInt(5.W))
      val rd2      = Output(UInt(5.W))
      val wr1_addr = Output(UInt(5.W))
      val wr2_addr = Output(UInt(5.W))
    }
  })

  io.lsu_to_wbu_1.ready := true.B
  io.lsu_to_wbu_2.ready := true.B

  val rd1 = io.lsu_to_wbu_1.bits.rd
  val rd2 = io.lsu_to_wbu_2.bits.rd

  val valid1 = rd1 =/= 0.U
  val valid2 = rd2 =/= 0.U
  val conflict = valid1 && valid2 && (rd1 === rd2)

  // lane2 优先 (冲突时覆盖 lane1)
  io.wbu_to_grf.wr2.addr := Mux(valid2, rd2, 0.U)
  io.wbu_to_grf.wr2.data := io.lsu_to_wbu_2.bits.grf_wb_data

  io.wbu_to_grf.wr1.addr := Mux(valid1 && !conflict, rd1, 0.U)
  io.wbu_to_grf.wr1.data := io.lsu_to_wbu_1.bits.grf_wb_data

  io.debug.valid1   := valid1
  io.debug.valid2   := valid2
  io.debug.conflict := conflict
  io.debug.rd1      := rd1
  io.debug.rd2      := rd2
  io.debug.wr1_addr := io.wbu_to_grf.wr1.addr
  io.debug.wr2_addr := io.wbu_to_grf.wr2.addr
}

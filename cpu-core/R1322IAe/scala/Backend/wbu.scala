package R1322IAe

import chisel3._
import chisel3.util._

// 双发射写回单元：处理两条指令的写回冲突并输出到双端口寄存器堆
class wbu extends Module {
  val io = IO(new Bundle {
    // 输出给 GRF 的两个写端口
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
    // LSU1 输入
    val lsu_to_wbu_1 = new Bundle {
      val rd = Input(UInt(5.W))
      val grf_wb_data = Input(UInt(32.W))
    }
    // LSU2 输入
    val lsu_to_wbu_2 = new Bundle {
      val rd = Input(UInt(5.W))
      val grf_wb_data = Input(UInt(32.W))
    }
    // 调试端口
    val debug = new Bundle {
      val valid1    = Output(Bool())
      val valid2    = Output(Bool())
      val conflict  = Output(Bool())
      val rd1       = Output(UInt(5.W))
      val rd2       = Output(UInt(5.W))
      val wr1_addr  = Output(UInt(5.W))
      val wr2_addr  = Output(UInt(5.W))
    }
  })

  // 判断写请求是否有效（rd != 0）
  val valid1 = io.lsu_to_wbu_1.rd =/= 0.U
  val valid2 = io.lsu_to_wbu_2.rd =/= 0.U

  // 检测地址冲突（两个有效且 rd 相同）
  val conflict = valid1 && valid2 && (io.lsu_to_wbu_1.rd === io.lsu_to_wbu_2.rd)

  // 分配写端口：
  // - wr2 始终来自 LSU2（如果有效且非冲突时正常，冲突时也正常写入）
  // - wr1 来自 LSU1（仅当 LSU1 有效且不与 LSU2 冲突时）
  io.wbu_to_grf.wr2.addr := Mux(valid2, io.lsu_to_wbu_2.rd, 0.U)
  io.wbu_to_grf.wr2.data := io.lsu_to_wbu_2.grf_wb_data

  io.wbu_to_grf.wr1.addr := Mux(valid1 && !conflict, io.lsu_to_wbu_1.rd, 0.U)
  io.wbu_to_grf.wr1.data := io.lsu_to_wbu_1.grf_wb_data

  // 调试输出
  io.debug.valid1    := valid1
  io.debug.valid2    := valid2
  io.debug.conflict  := conflict
  io.debug.rd1       := io.lsu_to_wbu_1.rd
  io.debug.rd2       := io.lsu_to_wbu_2.rd
  io.debug.wr1_addr  := io.wbu_to_grf.wr1.addr
  io.debug.wr2_addr  := io.wbu_to_grf.wr2.addr
}
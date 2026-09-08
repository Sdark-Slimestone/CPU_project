package R1322IAeCSRMC

import chisel3._
import chisel3.util._

// 多周期 WBU: 总线的末端 slave
// 接收 LSU 的写回消息, 在下一周期完成 GRF 写回并发出 done 完成信号,
// IFU 收到 done 后才取下一个取指包(分布式多周期的流控)
class wbu extends Module {
  val io = IO(new Bundle {
    // 总线输入: 写回消息 <- LSU
    val in = Flipped(Decoupled(new WBMsg))

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

    // 完成信号 -> IFU
    val wbu_to_ifu = new Bundle {
      val done = Output(Bool())
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

  // WBU 恰好忙碌一个周期: 收到消息的下一周期写回并发出 done
  val busy = RegNext(io.in.fire, false.B)
  val msg  = RegEnable(io.in.bits, io.in.fire)
  io.in.ready := !busy

  // 判断写请求是否有效（rd != 0）
  val valid1 = msg.rd1 =/= 0.U
  val valid2 = msg.rd2 =/= 0.U

  // 检测地址冲突（两个有效且 rd 相同）
  val conflict = valid1 && valid2 && (msg.rd1 === msg.rd2)

  // 分配写端口(仅在 busy 周期写一个周期)
  io.wbu_to_grf.wr2.addr := Mux(busy && valid2, msg.rd2, 0.U)
  io.wbu_to_grf.wr2.data := msg.wb_data2

  io.wbu_to_grf.wr1.addr := Mux(busy && valid1 && !conflict, msg.rd1, 0.U)
  io.wbu_to_grf.wr1.data := msg.wb_data1

  // 完成信号: 本包已写回
  io.wbu_to_ifu.done := busy

  // 调试输出
  io.debug.valid1    := valid1
  io.debug.valid2    := valid2
  io.debug.conflict  := conflict
  io.debug.rd1       := msg.rd1
  io.debug.rd2       := msg.rd2
  io.debug.wr1_addr  := io.wbu_to_grf.wr1.addr
  io.debug.wr2_addr  := io.wbu_to_grf.wr2.addr
}

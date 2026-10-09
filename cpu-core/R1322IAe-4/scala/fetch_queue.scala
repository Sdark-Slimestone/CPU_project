package R1322IAeCSR

import chisel3._
import chisel3.util._

// 取指队列 (FIFO): 解耦取指(IF)与译码(IDU), 每拍按条弹出 1 或 2。
//  - IFU 每拍把取回的一整对指令压入队列 (队里有 >=2 空位才收)。
//  - IDU 从队头取 1~2 条: 包内冲突只弹 1, 第二条留队头, 下拍直接当 lane1,
//    不再需要"杀掉 lane2 + 重定向重取"。
//  - flush: EXU 真控制流(分支/异常/mret)时清空队列。
class FetchQueue(val depth: Int = 4) extends Module {
  require(depth >= 2, "depth 至少 2")
  val ptrW = log2Ceil(depth)

  val io = IO(new Bundle {
    // 来自 IFU 的压入 (1 或 2 条)
    val push_valid = Input(Bool())
    val push_num   = Input(UInt(2.W))        // 本拍压入几条 (1/2)
    val push_inst  = Input(Vec(2, UInt(32.W)))
    val push_pc    = Input(Vec(2, UInt(32.W)))
    val push_pred  = Input(Vec(2, new PredInfo))  // 每条的方向预测结论
    val push_ready = Output(Bool())          // 空位 >= push_num

    // 队头 (给 IDU)
    val out_inst  = Output(Vec(2, UInt(32.W)))
    val out_pc    = Output(Vec(2, UInt(32.W)))
    val out_pred  = Output(Vec(2, new PredInfo))
    val out_valid = Output(UInt(2.W))        // 队头有效条数 0/1/2

    // 弹出 / 冲刷
    val pop_cnt = Input(UInt(2.W))
    val flush   = Input(Bool())

    val count = Output(UInt(4.W))
    val empty = Output(Bool())
  })

  val inst = RegInit(VecInit(Seq.fill(depth)(0.U(32.W))))
  val pc   = RegInit(VecInit(Seq.fill(depth)(0.U(32.W))))
  val pred = RegInit(VecInit(Seq.fill(depth)(0.U.asTypeOf(new PredInfo))))
  val head = RegInit(0.U(ptrW.W))
  val cnt  = RegInit(0.U(4.W))

  val idx0 = head
  val idx1 = (head + 1.U)(ptrW - 1, 0)
  io.out_inst(0) := inst(idx0)
  io.out_inst(1) := inst(idx1)
  io.out_pc(0)   := pc(idx0)
  io.out_pc(1)   := pc(idx1)
  io.out_pred(0) := pred(idx0)
  io.out_pred(1) := pred(idx1)

  val out_cnt  = Mux(cnt >= 2.U, 2.U(2.W), cnt(1, 0))
  io.out_valid := out_cnt
  io.count     := cnt
  io.empty     := cnt === 0.U

  val space = depth.U -& cnt
  io.push_ready := space >= io.push_num

  val do_push = io.push_valid && io.push_ready && !io.flush
  val do_pop  = Mux(io.flush, 0.U(2.W),
                Mux(io.pop_cnt >= out_cnt, out_cnt, io.pop_cnt))

  val wr = (head + cnt)(ptrW - 1, 0)
  when (do_push) {
    inst(wr) := io.push_inst(0)
    pc(wr)   := io.push_pc(0)
    pred(wr) := io.push_pred(0)
    when (io.push_num === 2.U) {
      val wr1 = (wr + 1.U)(ptrW - 1, 0)
      inst(wr1) := io.push_inst(1)
      pc(wr1)   := io.push_pc(1)
      pred(wr1) := io.push_pred(1)
    }
  }

  when (io.flush) {
    head := 0.U
    cnt  := 0.U
  } .otherwise {
    head := (head +& do_pop)(ptrW - 1, 0)
    cnt  := cnt - do_pop + Mux(do_push, io.push_num, 0.U(2.W))
  }
}

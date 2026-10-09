package R1322IAeCSR

import chisel3._
import chisel3.util._

//===================================取指队列(FIFO)===========================================
// 把取指(IF)与译码(IDU1)解耦: IF 只管顺序压入指令, IDU1 每拍从队头弹 1 或 2 条。
// 包内冲突(控制/RAW/双写...)时只弹 1 条, 第二条留在队列里, 下一拍当 lane1 再弹,
// 于是 ~不需要重取/冲刷已经超前取进来的包~ —— 这就是路线 B 的关键。
class FetchQueue(val depth: Int = 4) extends Module {
  require(isPow2(depth) && depth >= 2, "depth 必须是 >=2 的 2 的幂")
  val ptrW = log2Ceil(depth)

  val io = IO(new Bundle {
    // ---- 来自 IF 的压入 ----
    val push_inst = Input(Vec(2, UInt(32.W)))   // 本拍取到的前两条(pc, pc+4)
    val push_pc   = Input(Vec(2, UInt(32.W)))
    val push_next = Input(Vec(2, UInt(32.W)))
    val push_max  = Input(UInt(2.W))            // IF 最多想压几条(BTB 命中第一条时=1, 否则=2)
    val push_cnt  = Output(UInt(2.W))           // 本拍实际被接收几条(0/1/2), IF 据此推进 pc

    // ---- 队头(给 IDU1) ----
    val out_inst  = Output(Vec(2, UInt(32.W)))
    val out_pc    = Output(Vec(2, UInt(32.W)))
    val out_next  = Output(Vec(2, UInt(32.W)))
    val out_valid = Output(UInt(2.W))           // 队头有效条数(0/1/2)

    // ---- 弹出(由 IDU2 的冲突判决给出: 1 或 2) ----
    val pop_cnt   = Input(UInt(2.W))
    val flush     = Input(Bool())               // 重定向: 清空队列

    val count     = Output(UInt(4.W))
  })

  val inst = RegInit(VecInit(Seq.fill(depth)(0.U(32.W))))
  val pc   = RegInit(VecInit(Seq.fill(depth)(0.U(32.W))))
  val next = RegInit(VecInit(Seq.fill(depth)(0.U(32.W))))
  val head = RegInit(0.U(ptrW.W))
  val cnt  = RegInit(0.U(4.W))

  // 队头两条(head 环绕用截断实现)
  val idx0 = head
  val idx1 = (head + 1.U)(ptrW - 1, 0)
  io.out_inst(0) := inst(idx0); io.out_pc(0) := pc(idx0); io.out_next(0) := next(idx0)
  io.out_inst(1) := inst(idx1); io.out_pc(1) := pc(idx1); io.out_next(1) := next(idx1)
  val out_cnt = Mux(cnt >= 2.U, 2.U(2.W), cnt(1, 0))
  io.out_valid := out_cnt
  io.count     := cnt

  // 空间 = depth - cnt
  val space = depth.U(4.W) - cnt

  // 实际弹出(不超过有效数)
  val do_pop = Mux(io.flush, 0.U, Mux(io.pop_cnt >= out_cnt, out_cnt, io.pop_cnt))
  // 压入只看当前空间(不把刚弹出的槽算进来), 这样 IFU 的 pc 路径不依赖 D 的冲突判决
  val do_push = Mux(io.flush, 0.U,
                Mux(space >= 2.U, io.push_max,
                Mux(space >= 1.U, 1.U, 0.U)))
  io.push_cnt := do_push

  // tail = head + cnt
  val tail = (head +& cnt)(ptrW - 1, 0)

  when (io.flush) {
    head := 0.U
    cnt  := 0.U
  } .otherwise {
    head := (head +& do_pop)(ptrW - 1, 0)
    cnt  := cnt - do_pop + do_push
  }

  // 压入写(tail, tail+1)
  when (do_push >= 1.U) {
    inst(tail) := io.push_inst(0)
    pc(tail)   := io.push_pc(0)
    next(tail) := io.push_next(0)
  }
  when (do_push >= 2.U) {
    val t1 = (tail + 1.U)(ptrW - 1, 0)
    inst(t1) := io.push_inst(1)
    pc(t1)   := io.push_pc(1)
    next(t1) := io.push_next(1)
  }
}
//===================================取指队列===========================================

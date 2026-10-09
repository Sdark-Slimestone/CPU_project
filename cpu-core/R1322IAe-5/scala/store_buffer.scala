package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== store buffer (store->load 转发) ===============================
// 保存最近 N 拍 M 级 store 的 {字地址, 移位后数据, 字节掩码}; load 在 M 级拿自己的字地址
// (E 级寄存过来) 与之逐字节合并转发。
//   N = max(存储器延迟, 2): 延迟 L 时 load 到 M 比 store 晚 L 拍, 需保留 L 拍 store 才能命中。
// 优先级 (每字节取最年轻的写者): 同包 lane1 store (对 lane2) > entry0(最年轻) > ... > entryN-1 > 内存
class StoreBuffer extends Module {
  val N = math.max(MemCfg.latency, 2)
  val io = IO(new Bundle {
    val push_valid = Input(Bool())
    val push_addr  = Input(UInt(32.W))
    val push_data  = Input(UInt(32.W))
    val push_mask  = Input(UInt(4.W))

    val q1_addr = Input(UInt(32.W))
    val q1_ren  = Input(Bool())
    val q2_addr = Input(UInt(32.W))
    val q2_ren  = Input(Bool())

    val same_valid = Input(Bool())
    val same_addr  = Input(UInt(32.W))
    val same_data  = Input(UInt(32.W))
    val same_mask  = Input(UInt(4.W))

    val mem1 = Input(UInt(32.W))
    val mem2 = Input(UInt(32.W))

    val ld1 = Output(UInt(32.W))
    val ld2 = Output(UInt(32.W))
  })

  def bmask(m: UInt): UInt = Cat(Fill(8, m(3)), Fill(8, m(2)), Fill(8, m(1)), Fill(8, m(0)))

  // 最近 N 拍 M 级 store, entry0 最年轻
  val ev = RegInit(VecInit(Seq.fill(N)(false.B)))
  val ea = RegInit(VecInit(Seq.fill(N)(0.U(32.W))))
  val ed = RegInit(VecInit(Seq.fill(N)(0.U(32.W))))
  val em = RegInit(VecInit(Seq.fill(N)(0.U(4.W))))
  for (k <- N - 1 to 1 by -1) {
    ev(k) := ev(k - 1); ea(k) := ea(k - 1); ed(k) := ed(k - 1); em(k) := em(k - 1)
  }
  ev(0) := io.push_valid
  ea(0) := io.push_addr & "hFFFFFFFC".U(32.W)
  ed(0) := io.push_data
  em(0) := io.push_mask

  // E 级 load 字地址/使能寄存到 M 级
  val q1a = RegNext(io.q1_addr & "hFFFFFFFC".U(32.W))
  val q2a = RegNext(io.q2_addr & "hFFFFFFFC".U(32.W))
  val q1r = RegNext(io.q1_ren, false.B)
  val q2r = RegNext(io.q2_ren, false.B)

  // lane1: 由老到新逐条覆盖, 最年轻者最终生效
  var r1 = io.mem1
  for (k <- N - 1 to 0 by -1) {
    val m   = bmask(em(k))
    val hit = ev(k) && (ea(k) === q1a)
    r1 = Mux(q1r && hit, (r1 & ~m) | (ed(k) & m), r1)
  }
  io.ld1 := r1

  // lane2: 同包 lane1 store 最年轻优先, 再 buffer
  var r2 = io.mem2
  for (k <- N - 1 to 0 by -1) {
    val m   = bmask(em(k))
    val hit = ev(k) && (ea(k) === q2a)
    r2 = Mux(q2r && hit, (r2 & ~m) | (ed(k) & m), r2)
  }
  val sv = q2r && io.same_valid && ((io.same_addr & "hFFFFFFFC".U(32.W)) === q2a)
  val sm = bmask(io.same_mask)
  io.ld2 := Mux(sv, (r2 & ~sm) | (io.same_data & sm), r2)
}

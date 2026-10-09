package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== store buffer (store->load 转发) ===============================
// 保存最近两拍 M 级 store 的 {字地址, 移位后数据, 字节掩码}; M 级 load 拿自己的字地址
// (E 级寄存过来) 与之比较, 命中就按字节逐位合并转发。
//
// 优先级 (每字节取最年轻的写者):
//   同包 lane1 store (当前 M 级, 最年轻)  >  buffer v1 (上一拍 M store)  >  buffer v2  >  内存
// 注意: 必须"按字节合并", 不能只挑一条 store —— 不同 store 可能写同一字的不同字节。
class StoreBuffer extends Module {
  val io = IO(new Bundle {
    // ---- 压入: M 级 store (来自 LSU; lane1 优先) ----
    val push_valid = Input(Bool())
    val push_addr  = Input(UInt(32.W))
    val push_data  = Input(UInt(32.W))
    val push_mask  = Input(UInt(4.W))

    // ---- 查询: E 级 load 地址/读使能 (内部寄存一拍到 M 级比较) ----
    val q1_addr = Input(UInt(32.W))
    val q1_ren  = Input(Bool())
    val q2_addr = Input(UInt(32.W))
    val q2_ren  = Input(Bool())

    // ---- 同包 lane1 store (当前 M 级, 对 lane2 最年轻) ----
    val same_valid = Input(Bool())
    val same_addr  = Input(UInt(32.W))
    val same_data  = Input(UInt(32.W))
    val same_mask  = Input(UInt(4.W))

    // ---- 内存读值 (M 级) ----
    val mem1 = Input(UInt(32.W))
    val mem2 = Input(UInt(32.W))

    // ---- 转发后 load 值 ----
    val ld1 = Output(UInt(32.W))
    val ld2 = Output(UInt(32.W))
  })

  def bmask(m: UInt): UInt = Cat(Fill(8, m(3)), Fill(8, m(2)), Fill(8, m(1)), Fill(8, m(0)))

  // 最近两拍 M 级 store
  val v1 = RegNext(io.push_valid, false.B)
  val a1 = RegNext(io.push_addr & "hFFFFFFFC".U(32.W))
  val d1 = RegNext(io.push_data)
  val m1 = RegNext(io.push_mask)
  val v2 = RegNext(v1, false.B)
  val a2 = RegNext(a1)
  val d2 = RegNext(d1)
  val m2 = RegNext(m1)

  // E 级 load 字地址/使能寄存到 M 级
  val q1a = RegNext(io.q1_addr & "hFFFFFFFC".U(32.W))
  val q2a = RegNext(io.q2_addr & "hFFFFFFFC".U(32.W))
  val q1r = RegNext(io.q1_ren, false.B)
  val q2r = RegNext(io.q2_ren, false.B)

  // ---- lane1: 只有 buffer ----
  val h1a = v1 && (a1 === q1a)
  val h2a = v2 && (a2 === q1a)
  val la1 = Mux(h1a, bmask(m1), 0.U(32.W))
  val la2 = Mux(h2a, bmask(m2), 0.U(32.W)) & ~la1
  io.ld1 := Mux(q1r && (h1a || h2a),
                (io.mem1 & ~(la1 | la2)) | (d1 & la1) | (d2 & la2), io.mem1)

  // ---- lane2: 同包 lane1 store 最年轻优先, 再 buffer ----
  val sv  = q2r && io.same_valid && ((io.same_addr & "hFFFFFFFC".U(32.W)) === q2a)
  val h1b = v1 && (a1 === q2a)
  val h2b = v2 && (a2 === q2a)
  val l2s = Mux(sv,  bmask(io.same_mask), 0.U(32.W))
  val l21 = Mux(h1b, bmask(m1), 0.U(32.W)) & ~l2s
  val l22 = Mux(h2b, bmask(m2), 0.U(32.W)) & ~l2s & ~l21
  io.ld2 := Mux(q2r && (sv || h1b || h2b),
                (io.mem2 & ~(l2s | l21 | l22)) | (io.same_data & l2s) | (d1 & l21) | (d2 & l22), io.mem2)
}

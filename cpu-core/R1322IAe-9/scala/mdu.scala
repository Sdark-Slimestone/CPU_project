package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== 乘除法单元 (RV32M) ======================================
// 单周期组合乘/除会顶爆已贴死的 EXU 关键路径, 故做多周期迭代单元:
//   - 乘法: 32 拍移位-加, 先算 |a|*|b| (64 位无符号), 再按符号修正取高/低 32 位。
//   - 除法: 32 拍恢复余数法 (MSB 先), 先算幅值商/余, 再按符号修正; 处理除零/溢出。
// 为不把取反/比较顶到"转发→EXU"这条关键路径上, start 拍只锁存原始操作数, 下一拍 (init)
// 才在家门口把幅值装进移位寄存器; 输出也先落一拍寄存器再给写回。
// 握手: start 后 busy 置起, 经 init(1) + 迭代(32) + 结果(1) 拍后 done 拉高一拍并给出 out。
// op = 指令 funct3:
//   0 mul  1 mulh  2 mulhsu  3 mulhu  4 div  5 divu  6 rem  7 remu
class MDU extends Module {
  val N = 32

  val io = IO(new Bundle {
    val start  = Input(Bool())
    val cancel = Input(Bool())
    val is_div = Input(Bool())
    val op     = Input(UInt(3.W))
    val a      = Input(UInt(32.W))
    val b      = Input(UInt(32.W))
    val idle   = Output(Bool())
    val busy   = Output(Bool())
    val done   = Output(Bool())
    val out    = Output(UInt(32.W))
  })

  val busy  = RegInit(false.B)
  val cnt   = RegInit(0.U(6.W))
  val a_r   = RegInit(0.U(32.W))
  val b_r   = RegInit(0.U(32.W))
  val op_r  = RegInit(0.U(3.W))
  val div_r = RegInit(false.B)

  // 乘法状态
  val prod  = RegInit(0.U(64.W))
  val mcand = RegInit(0.U(64.W))
  val mplr  = RegInit(0.U(32.W))

  // 除法状态 (rem/dvsr 多留 1 位余量)
  val rem   = RegInit(0.U(33.W))
  val quot  = RegInit(0.U(32.W))
  val dvnd  = RegInit(0.U(32.W))
  val dvsr  = RegInit(0.U(33.W))

  val res_r = RegInit(0.U(32.W))

  io.idle := !busy

  // ---------------- 结果修正 (组合, 仅由寄存器得到) ----------------
  val a_sign = a_r(31) && ((op_r === 1.U) || (op_r === 2.U) || (op_r === 4.U) || (op_r === 6.U))
  val b_sign = b_r(31) && ((op_r === 1.U) || (op_r === 4.U) || (op_r === 6.U))
  val neg_mul   = a_sign ^ b_sign
  val prod_adj  = Mux(neg_mul, -prod, prod)

  val is_rem  = (op_r === 6.U) || (op_r === 7.U)
  val q_sig   = Mux(a_sign ^ b_sign, -quot, quot)
  val r_sig   = Mux(a_sign, -rem(31, 0), rem(31, 0))
  val div_norm = Mux(is_rem, r_sig, q_sig)

  val db_zero = (b_r === 0.U)
  val ovf     = (op_r === 4.U) && (a_r === "h80000000".U) && (b_r === "hFFFFFFFF".U)
  val div_res = Mux(db_zero, Mux(is_rem, a_r, "hFFFFFFFF".U),
                Mux(ovf, Mux(is_rem, 0.U, "h80000000".U), div_norm))

  val mul_res = Mux(op_r === 0.U, prod(31, 0), prod_adj(63, 32))
  val result  = Mux(div_r, div_res, mul_res)

  // 迭代拍数: 除法 32 拍 (恢复余数, 1 bit/拍); 乘法 16 拍 (radix-4, 2 bit/拍)。
  val nstep = Mux(div_r, N.U, (N / 2).U)

  // ---------------- 迭代控制 ----------------
  when (io.cancel) {
    busy := false.B
  } .elsewhen (io.start) {
    // 只锁存原始输入 (纯寄存器装载, 不在"转发→EXU"路径上做取反/比较)
    busy  := true.B
    cnt   := 0.U
    a_r   := io.a
    b_r   := io.b
    op_r  := io.op
    div_r := io.is_div
  } .elsewhen (busy && cnt === 0.U) {
    // init: 由已寄存的 a_r/b_r 取幅值, 装入移位寄存器 (寄存器→寄存器, 路径短)
    val a_signed = (op_r === 1.U) || (op_r === 2.U) || (op_r === 4.U) || (op_r === 6.U)
    val b_signed = (op_r === 1.U) || (op_r === 4.U) || (op_r === 6.U)
    val a_neg = a_r(31) && a_signed
    val b_neg = b_r(31) && b_signed
    val a_mag = Mux(a_neg, -a_r, a_r)
    val b_mag = Mux(b_neg, -b_r, b_r)
    cnt := 1.U
    when (div_r) {
      dvnd := a_mag
      dvsr := Cat(0.U(1.W), b_mag)
      rem  := 0.U
      quot := 0.U
    } .otherwise {
      mcand := Cat(0.U(32.W), a_mag)
      mplr  := b_mag
      prod  := 0.U
    }
  } .elsewhen (busy && cnt <= nstep) {
    // 迭代 (cnt = 1..nstep)
    cnt := cnt + 1.U
    when (div_r) {
      // 恢复余数法: 每拍左移一位并试减
      val shifted = Cat(rem(31, 0), dvnd(31))
      val ge      = shifted >= dvsr
      rem  := Mux(ge, shifted - dvsr, shifted)
      quot := Cat(quot(30, 0), ge)
      dvnd := dvnd << 1
    } .otherwise {
      // radix-4: 每拍吃 2 位乘数, 加 0/1/2/3 倍被乘数
      val d       = mplr(1, 0)
      val mcand3  = mcand + (mcand << 1)
      val addend  = MuxLookup(d, 0.U(64.W))(Seq(
        1.U -> mcand,
        2.U -> (mcand << 1),
        3.U -> mcand3
      ))
      prod  := prod + addend
      mcand := mcand << 2
      mplr  := mplr >> 2
    }
  } .elsewhen (busy && cnt === (nstep + 1.U)) {
    // 结果落一拍寄存器, 缩短到写回寄存器的组合路径
    res_r := result
    cnt   := cnt + 1.U
  } .otherwise {
    busy := false.B   // cnt == nstep + 2
  }

  io.busy := busy
  io.done := busy && (cnt === (nstep + 2.U))
  io.out  := res_r
}

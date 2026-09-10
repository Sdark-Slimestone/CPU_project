package R1322IAeCSR

import chisel3._
import chisel3.util._

// ==================================================================
// 【多周期新增部件】影子计数器 + 单发射保持器
// ==================================================================

// RetireCounter: 影子 mcycle/minstret 计数器
//   core/csr 内部的 minstret 按原版逻辑每拍累加 inst_retire(掩码全零
//   期间 inst_ok=1 仍会重复累计, 无法从外部阻止), 故本级维护按 EXU
//   生效拍(D/E 桥 valid, 恰好一拍)精确计数的影子计数, 供顶层 debug 显示;
//   程序经 csrr 读 minstret 读到的是 core 内部计数(已知差异);
//   mcycle 为每拍 +1 计数, 与 core 内部一致不受影响。
class RetireCounter extends Module {
  val io = IO(new Bundle {
    val dec_valid = Input(Bool())                   // EXU 生效拍(D/E 桥 valid, 恰好一拍)
    val dec_msg   = Input(new IDUToEXUMessage)      // D/E 桥当前消息(驻留内容)
    val debug_mcycle   = Output(UInt(64.W))
    val debug_minstret = Output(UInt(64.W))
  })

  val mcycle = RegInit(0.U(64.W))
  mcycle := mcycle + 1.U

  val minstret = RegInit(0.U(64.W))
  val instOk = !io.dec_msg.dec1_op.is_ebreak && !io.dec_msg.dec1_op.is_ecall
  minstret := minstret + Mux(io.dec_valid,
                 Mux(instOk, Mux(io.dec_msg.is_stall, 1.U(2.W), 2.U(2.W)), 0.U(2.W)),
                 0.U(2.W))

  io.debug_mcycle := mcycle
  io.debug_minstret := minstret
}

// StallHolder: 单发射标志保持器
//   IDU->EXU 交接拍(de_bridge.io.in.fire)锁存该包的单发射标志,
//   保持到下一包交接; 原版 idu 的 idu_to_ifu.is_stall 是组合信号,
//   多周期下 IFU 在 done 拍(晚 3 拍)读取, 必须由本级保持。
class StallHolder extends Module {
  val io = IO(new Bundle {
    val in_fire  = Input(Bool())      // IDU->EXU 交接拍
    val in_stall = Input(Bool())      // 该包的单发射标志(交接拍组合有效)
    val is_stall = Output(Bool())     // 保持输出 -> IFU
  })

  val singleIssueReg = RegInit(false.B)
  when (io.in_fire) { singleIssueReg := io.in_stall }
  io.is_stall := singleIssueReg
}

// RedirectLatch: 重定向锁存器
//   EXU 在提交拍(E 级)发出分支/异常/mret 重定向脉冲(旁路导线),
//   此处锁存为 pending, 在包完成拍(done / pc_en)经门控输出给 core/ifu,
//   core/ifu 的 pc 更新逻辑按原版优先级选择目标。
class RedirectLatch extends Module {
  val io = IO(new Bundle {
    // 来自 EXU 的重定向脉冲
    val take_trap     = Input(Bool())
    val trap_pc       = Input(UInt(32.W))
    val take_mret     = Input(Bool())
    val mret_pc       = Input(UInt(32.W))
    val take_branch   = Input(Bool())
    val branch_target = Input(UInt(32.W))
    // 包完成拍(done): 输出 pending 并清除
    val update            = Input(Bool())
    val take_trap_out     = Output(Bool())
    val trap_pc_out       = Output(UInt(32.W))
    val take_mret_out     = Output(Bool())
    val mret_pc_out       = Output(UInt(32.W))
    val take_branch_out   = Output(Bool())
    val branch_target_out = Output(UInt(32.W))
  })

  val pendingValid   = RegInit(false.B)
  val pendingTarget  = RegInit(0.U(32.W))
  val pendingIsTrap  = RegInit(false.B)
  val pendingIsMret  = RegInit(false.B)
  val pendingIsBranch = RegInit(false.B)

  val redirectNow = io.take_trap || io.take_mret || io.take_branch
  when (redirectNow) {
    pendingValid   := true.B
    pendingTarget  := Mux(io.take_trap, io.trap_pc,
                      Mux(io.take_mret, io.mret_pc, io.branch_target))
    pendingIsTrap  := io.take_trap
    pendingIsMret  := io.take_mret
    pendingIsBranch := io.take_branch
  }

  // update 拍: 输出 pending(只有一个 kind 有效)并清除
  io.take_trap_out     := pendingValid && pendingIsTrap && io.update
  io.trap_pc_out       := pendingTarget
  io.take_mret_out     := pendingValid && pendingIsMret && io.update
  io.mret_pc_out       := pendingTarget
  io.take_branch_out   := pendingValid && pendingIsBranch && io.update
  io.branch_target_out := pendingTarget

  when (io.update) {
    pendingValid    := false.B
    pendingIsTrap   := false.B
    pendingIsMret   := false.B
    pendingIsBranch := false.B
  }
}

// FetchGate: 取指门控器
//   原版 core/ifu 的取指输出是持续有效的组合信号, 多周期下每个包
//   只允许被 F/D 桥捕获一次: 被接收后到包完成(done)之前保持关闭。
class FetchGate extends Module {
  val io = IO(new Bundle {
    val accepted = Input(Bool())      // F/D 桥接收拍
    val done     = Input(Bool())      // 包完成拍(可取下一包)
    val present  = Output(Bool())     // -> F/D 桥 in.valid
  })

  val canPresent = RegInit(true.B)
  when (io.accepted) { canPresent := false.B }
  when (io.done)     { canPresent := true.B  }
  io.present := canPresent
}

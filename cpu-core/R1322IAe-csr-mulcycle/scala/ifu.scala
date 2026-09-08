package R1322IAeCSRMC

import chisel3._
import chisel3.util._

// 多周期 IFU: 作为总线 master 向 IDU 发送取指包(两条指令)
// 状态机:
//   s_wait_ready - 已取到指令, valid 有效, 等待 IDU 就绪(握手)
//   s_idle       - 本包已交给 IDU, 等待 WBU 完成信号后再取下一包
// 由于一次在飞行中最多一个取指包, 分支/异常重定向只需暂存目标地址,
// 在 WBU 完成时选择: 重定向目标 > 顺序地址(单发射 pc+4 / 双发射 pc+8)
class IFU extends Module {
  val io = IO(new Bundle {
    // 总线输出: 取指包 -> IDU
    val out = Decoupled(new IFMsg)

    // EXU 分支重定向
    val exu_to_ifu = new Bundle {
      val take_branch   = Input(Bool())
      val branch_target = Input(UInt(32.W))
    }

    // CSR 异常/mret 重定向
    val csr_to_ifu = new Bundle {
      val take_trap = Input(Bool())
      val trap_pc   = Input(UInt(32.W))
      val take_mret = Input(Bool())
      val mret_pc   = Input(UInt(32.W))
    }

    // IDU 反馈: 本包冲突只发射了一条指令(保持到包完成)
    val idu_to_ifu = new Bundle {
      val is_stall = Input(Bool())
    }

    // WBU 完成信号: 本包已写回, 可取下一包
    val wbu_to_ifu = new Bundle {
      val done = Input(Bool())
    }

    val ifu_to_imem = new Bundle {
      val addr1 = Output(UInt(32.W))
      val addr2 = Output(UInt(32.W))
    }

    val imem_to_ifu = new Bundle {
      val inst1 = Input(UInt(32.W))
      val inst2 = Input(UInt(32.W))
    }
  })

  val pcReg   = RegInit("h80000000".U(32.W))
  val pcPlus4 = pcReg + 4.U
  val pcPlus8 = pcReg + 8.U

  // 主设备通信状态机(讲义: idle / wait_ready)
  val s_idle :: s_wait_ready :: Nil = Enum(2)
  val state = RegInit(s_wait_ready)

  // 最近一次握手成功的包的 pc1, 用于计算顺序下一包地址
  val sentPc = RegInit("h80000000".U(32.W))

  // 重定向暂存: EXU 提交周期发出重定向, WBU 完成时生效
  val redValid  = RegInit(false.B)
  val redTarget = RegInit(0.U(32.W))

  val take_trap   = io.csr_to_ifu.take_trap
  val take_mret   = io.csr_to_ifu.take_mret
  val take_branch = io.exu_to_ifu.take_branch
  val redirect    = take_trap || take_mret || take_branch
  val redirectTarget = Mux(take_trap, io.csr_to_ifu.trap_pc,
                       Mux(take_mret, io.csr_to_ifu.mret_pc,
                           io.exu_to_ifu.branch_target))

  when (redirect) {
    redValid  := true.B
    redTarget := redirectTarget
  }

  // 顺序下一包地址: 单发射时 inst2 需要重新取(作为下一包的第一条), 双发射时跳过两条
  val seqNext = Mux(io.idu_to_ifu.is_stall, sentPc + 4.U, sentPc + 8.U)

  // 取指(组合读 imem), pcReg 稳定则消息稳定, 满足总线协议要求
  io.out.valid := state === s_wait_ready
  io.out.bits.inst1        := io.imem_to_ifu.inst1
  io.out.bits.inst2        := io.imem_to_ifu.inst2
  io.out.bits.inst1_pc     := pcReg
  io.out.bits.inst2_pc     := pcPlus4
  io.out.bits.inst1_nextpc := pcPlus4
  io.out.bits.inst2_nextpc := pcPlus8

  switch (state) {
    is (s_idle) {
      when (io.wbu_to_ifu.done) {
        pcReg    := Mux(redValid, redTarget, seqNext)
        redValid := false.B
        state    := s_wait_ready
      }
    }
    is (s_wait_ready) {
      when (io.out.fire) {
        sentPc := pcReg
        state  := s_idle
      }
    }
  }

  io.ifu_to_imem.addr1 := pcReg
  io.ifu_to_imem.addr2 := pcPlus4
}

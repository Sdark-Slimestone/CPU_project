// winreg_fsm.scala
package R3262VSA

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

class win_controller_P extends Module {
  val io = IO(new Bundle {
    val iscall = Input(Bool())
    val isret  = Input(Bool())
    

    // 状态输出
    val cur_wincode     = Output(UInt(3.W))
    val overflowcounter = Output(UInt(16.W))
    val prev_wincode    = Output(UInt(3.W))  // 两周期前的窗口号

    // 陷阱信号
    val winoverflow_stack_pushtrap = Output(Bool())
    val winoverflow_stack_poptrap  = Output(Bool())
    val toomuch_fuctioncall        = Output(Bool())


  })

  // 状态寄存器
  val wincode         = RegInit(0.U(3.W))
  val overflowcounter = RegInit(0.U(16.W))

  // 计算当前窗口总数
  val current_win_number = overflowcounter + wincode

  // 下一状态计算
  val wincode_plus1  = wincode + 1.U
  val wincode_minus1 = wincode - 1.U
  val overflow_plus1 = overflowcounter + 1.U
  val overflow_minus1 = overflowcounter - 1.U

  // wincode 更新逻辑（移除 ig）
  val wincode_next = Mux(io.iscall,
                         Mux(wincode === 6.U, 6.U, wincode_plus1),
                         Mux(io.isret,
                             Mux(wincode === 0.U, 0.U,
                                 Mux(overflowcounter === 0.U, wincode_minus1, wincode)),
                             wincode))

  // overflowcounter 更新逻辑（移除 ig）
  val overflow_next = Mux(io.iscall,
                          Mux(wincode === 6.U, overflow_plus1, overflowcounter),
                          Mux(io.isret,
                              Mux(overflowcounter =/= 0.U, overflow_minus1, overflowcounter),
                              overflowcounter))

  wincode         := wincode_next
  overflowcounter := overflow_next

  // 两周期延迟队列
  val wincode_delay1 = RegInit(0.U(3.W))
  val wincode_delay2 = RegInit(0.U(3.W))
  wincode_delay1 := wincode
  wincode_delay2 := wincode_delay1

  // 陷阱信号（移除 ig）
  val push_trap = io.iscall && (current_win_number >= 6.U)
  val pop_trap  = io.isret  && (current_win_number > 6.U)
  val too_much  = io.iscall && (current_win_number === 65535.U)

  io.cur_wincode     := wincode
  io.overflowcounter := overflowcounter
  io.prev_wincode    := wincode_delay2

  io.winoverflow_stack_pushtrap := push_trap
  io.winoverflow_stack_poptrap  := pop_trap
  io.toomuch_fuctioncall        := too_much
}


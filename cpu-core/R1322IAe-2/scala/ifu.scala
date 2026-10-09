package R1322IAeCSR

import chisel3._
import chisel3.util._

// 流水取指单元 (同步 imem: 地址发出后下一拍返回数据)。
//  - pcReg 保存"本拍要发出的取指地址"; 同步 imem 的数据下一拍返回,
//    本拍看到的返回数据对应上一拍发出的地址。
//  - 把取回的一整对指令压入取指队列 (FetchQueue); 队列有 >=2 空位才收。
//  - redirect 优先级最高 (来自 EXU 的分支/trap/mret): 改 pc, 丢弃在返回/待压入的对。
//  - 队列不收时保持 pc 不变, 同步 imem 重复返回同一份数据, 不会丢包。
class IFU extends Module {
  val io = IO(new Bundle {
    val redirect    = Input(Bool())
    val redirect_pc = Input(UInt(32.W))

    val ifu_to_imem = new Bundle {
      val addr1 = Output(UInt(32.W))
      val addr2 = Output(UInt(32.W))
    }
    val imem_to_ifu = new Bundle {
      val inst1 = Input(UInt(32.W))
      val inst2 = Input(UInt(32.W))
    }

    // 压入取指队列
    val push_valid = Output(Bool())
    val push_inst  = Output(Vec(2, UInt(32.W)))
    val push_pc    = Output(Vec(2, UInt(32.W)))
    val push_ready = Input(Bool())

    val debug = new Bundle {
      val inst1_pc = Output(UInt(32.W))
      val inst2_pc = Output(UInt(32.W))
    }
  })

  val pcReg    = RegInit("h80000000".U(32.W))   // 本拍发出的取指地址
  val retValid = RegInit(false.B)               // 本拍返回数据是否有效(尚未压入队列)
  val retPC    = RegInit(0.U(32.W))             // 本拍返回数据对应的包首地址

  val consumed = retValid && io.push_ready
  val issue    = !retValid || consumed

  val issueAddr = Mux(io.redirect, io.redirect_pc,
                  Mux(issue, pcReg, retPC))

  io.ifu_to_imem.addr1 := issueAddr
  io.ifu_to_imem.addr2 := issueAddr + 4.U

  io.push_valid := retValid
  io.push_inst(0) := io.imem_to_ifu.inst1
  io.push_inst(1) := io.imem_to_ifu.inst2
  io.push_pc(0)   := retPC
  io.push_pc(1)   := retPC + 4.U

  when (io.redirect) {
    retValid := false.B
    pcReg    := io.redirect_pc
  } .elsewhen (issue) {
    retValid := true.B
    retPC    := issueAddr
    pcReg    := issueAddr + 8.U
  }

  io.debug.inst1_pc := retPC
  io.debug.inst2_pc := retPC + 4.U
}

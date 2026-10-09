package R1322IAeCSR

import chisel3._
import chisel3.util._

// 流水取指(解耦前端 + BTB 预测):
//  - IF 顺序把指令压入取指队列; pc 按被接收条数推进;
//  - BTB 命中(该 pc 以前实际跳过) -> 预测 taken, 取指级直接把 pc 改到 target,
//    并只压入分支本身(第一条命中时), 后续指令直接取目标, 不必等 EXU 重定向;
//  - redirect(EXU 实际跳/异常/mret) 优先级最高。
class IFU extends Module {
  val io = IO(new Bundle {
    val redirect    = Input(Bool())
    val redirect_pc = Input(UInt(32.W))

    // BTB 查询结果(对 pcReg / pcReg+4)
    val btb_hit1 = Input(Bool())
    val btb_tgt1 = Input(UInt(32.W))
    val btb_hit2 = Input(Bool())
    val btb_tgt2 = Input(UInt(32.W))

    // 供 BTB 查询的取指地址
    val look_pc1 = Output(UInt(32.W))
    val look_pc2 = Output(UInt(32.W))

    val ifu_to_imem = new Bundle{
      val addr1      = Output(UInt(32.W))
      val addr2      = Output(UInt(32.W))
    }

    val imem_to_ifu = new Bundle{
      val inst1  = Input(UInt(32.W))
      val inst2  = Input(UInt(32.W))
    }

    // 压入取指队列
    val push_inst = Output(Vec(2, UInt(32.W)))
    val push_pc   = Output(Vec(2, UInt(32.W)))
    val push_next = Output(Vec(2, UInt(32.W)))
    val push_max  = Output(UInt(2.W))     // 本拍最多压几条(BTB 命中 pc 时只压 1 条)
    val push_cnt  = Input(UInt(2.W))      // 队列本拍接收了几条(0/1/2)

    val debug = new Bundle{
      val debug_inst1_pc = Output(UInt(32.W))
      val debug_inst2_pc = Output(UInt(32.W))
    }
  })

  val pcReg   = RegInit("h80000000".U(32.W))
  val pcPlus4 = pcReg + 4.U
  val pcPlus8 = pcReg + 8.U

  // BTB 预测: 第一槽命中 -> 只压分支本身(1 条), pc 跳 target;
  //          第二槽命中 -> 压两条(分支在第二条), pc 跳 target;
  //          都不命中 -> 顺序推进。第一槽优先。
  val pred1 = io.btb_hit1
  val pred2 = io.btb_hit2 && !io.btb_hit1
  io.push_max := Mux(pred1, 1.U, 2.U)

  when (io.redirect) {
    pcReg := io.redirect_pc
  } .elsewhen (io.push_cnt === 0.U) {
    // 队列没收下, 保持
  } .elsewhen (pred1) {
    pcReg := io.btb_tgt1
  } .elsewhen (pred2 && io.push_cnt === 2.U) {
    pcReg := io.btb_tgt2
  } .elsewhen (io.push_cnt === 2.U) {
    pcReg := pcPlus8
  } .otherwise {
    pcReg := pcPlus4
  }

  io.look_pc1 := pcReg
  io.look_pc2 := pcPlus4

  io.ifu_to_imem.addr1 := pcReg
  io.ifu_to_imem.addr2 := pcPlus4

  io.push_inst(0) := io.imem_to_ifu.inst1
  io.push_inst(1) := io.imem_to_ifu.inst2
  io.push_pc(0)   := pcReg
  io.push_pc(1)   := pcPlus4
  io.push_next(0) := pcPlus4
  io.push_next(1) := pcPlus8

  io.debug.debug_inst1_pc := pcReg
  io.debug.debug_inst2_pc := pcPlus4
}

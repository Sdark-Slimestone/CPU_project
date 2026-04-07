package minirv

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

//只有加法的丢人alu
class ALU extends Module{
  val io = IO(new Bundle {
    val source1 = Input(UInt(32.W))
    val source2 = Input(UInt(32.W))
    val output = Output(UInt(32.W)) 
  })
  io.output := io.source1 + io.source2
}

class InputMUXforALU extends Module {
  val io = IO(new Bundle {
    val inputfromRS1   = Input(UInt(32.W))
    val inputfromRS2   = Input(UInt(32.W))
    val inputfromIMM12 = Input(UInt(32.W))
    val add  = Input(Bool())
    val addi = Input(Bool())
    val jalr = Input(Bool())
    val lbu  = Input(Bool())
    val lw   = Input(Bool())
    val lui  = Input(Bool())
    val sw   = Input(Bool())
    val sb   = Input(Bool())
    val source1 = Output(UInt(32.W))
    val source2 = Output(UInt(32.W))
  })

  // source1: 仅 add/addi 使用 rs1，其余指令输出0
  io.source1 := MuxCase(0.U(32.W), Seq(
    (io.add || io.addi) -> io.inputfromRS1,
    (io.jalr || io.lbu || io.lw || io.lui || io.sw || io.sb) -> 0.U(32.W)
  ))

  // source2: add 使用 rs2，addi 使用 imm12，其余指令输出0
  io.source2 := MuxCase(0.U(32.W), Seq(
    io.add  -> io.inputfromRS2,
    io.addi -> io.inputfromIMM12,
    (io.jalr || io.lbu || io.lw || io.lui || io.sw || io.sb) -> 0.U(32.W)
  ))
}

class AGU extends Module{
  val io = IO(new Bundle {
    val source1 = Input(UInt(32.W)) //接寄存器的rs1
    val source2 = Input(UInt(32.W))
    val output = Output(UInt(32.W))  //接RAM
  })
  io.output := io.source1 + io.source2
}

class InputMUXforAGU extends Module {
  val io = IO(new Bundle {
    val inputfromRS1   = Input(UInt(32.W))
    val inputfromIMM12 = Input(UInt(32.W))
    val add  = Input(Bool())
    val addi = Input(Bool())
    val jalr = Input(Bool())
    val lbu  = Input(Bool())
    val lw   = Input(Bool())
    val lui  = Input(Bool())
    val sw   = Input(Bool())
    val sb   = Input(Bool())
    val source1 = Output(UInt(32.W))
    val source2 = Output(UInt(32.W))
  })

  // source1: 仅 add/addi 使用 rs1，其余指令输出0
  io.source1 := MuxCase(0.U(32.W), Seq(
    (io.add || io.addi || io.jalr || io.lui) -> 0.U(32.W),
    (io.lbu || io.lw || io.sw || io.sb) -> io.inputfromRS1
  ))

  // source2: add 使用 rs2，addi 使用 imm12，其余指令输出0
  io.source2 := MuxCase(0.U(32.W), Seq(
    (io.add || io.addi || io.jalr || io.lui) -> 0.U(32.W),
    (io.lbu || io.lw || io.sw || io.sb) -> io.inputfromIMM12
  ))
}

class BRU extends Module{
  val io = IO(new Bundle {
    val source1 = Input(UInt(32.W)) //接寄存器的rs1
    val source2 = Input(UInt(32.W))
    val output = Output(UInt(32.W))  //接PC的input
  })
  io.output := (io.source1 + io.source2) & (~"b11".U(32.W)) //低二位清零
}

class InputMUXforBRU extends Module {
  val io = IO(new Bundle {
    val inputfromRS1   = Input(UInt(32.W))
    val inputfromIMM12 = Input(UInt(32.W))
    val add  = Input(Bool())
    val addi = Input(Bool())
    val jalr = Input(Bool())
    val lbu  = Input(Bool())
    val lw   = Input(Bool())
    val lui  = Input(Bool())
    val sw   = Input(Bool())
    val sb   = Input(Bool())
    val source1 = Output(UInt(32.W))
    val source2 = Output(UInt(32.W))
  })

  // source1: 仅 add/addi 使用 rs1，其余指令输出0
  io.source1 := MuxCase(0.U(32.W), Seq(
    (io.add || io.addi || io.lbu || io.lw || io.sw ||io.sb) -> 0.U(32.W),
    io.jalr -> io.inputfromRS1
  ))

  // source2: add 使用 rs2，addi 使用 imm12，其余指令输出0
  io.source2 := MuxCase(0.U(32.W), Seq(
    (io.add || io.addi || io.lbu || io.lw || io.sw ||io.sb) -> 0.U(32.W),
    io.jalr -> io.inputfromIMM12
  ))
}

//读寄存器控制
class InputMUXforGRF extends Module {
  val io = IO(new Bundle {
    val add  = Input(Bool())
    val addi = Input(Bool())
    val jalr = Input(Bool())
    val lbu  = Input(Bool())
    val lw   = Input(Bool())
    val lui  = Input(Bool())
    val sw   = Input(Bool())
    val sb   = Input(Bool())
    val rs1en  = Output(Bool())
    val rs2en  = Output(Bool())
  })
  io.rs1en := io.addi || io.add || io.jalr || io.lbu || io.lw || io.sw || io.sb
  io.rs2en := io.add || io.sw || io.sb
}

//把idu解码出来的地址接受了然后传到grf
class AddrPassthrough extends Module {
  val io = IO(new Bundle {
    val rs1_addr_in = Input(UInt(5.W))
    val rs2_addr_in = Input(UInt(5.W))
    val rs1_addr_out = Output(UInt(5.W))
    val rs2_addr_out = Output(UInt(5.W))
  })
  io.rs1_addr_out := io.rs1_addr_in
  io.rs2_addr_out := io.rs2_addr_in
}

class EXU extends Module {
  val io = IO(new Bundle {
    // 指令类型（来自 IDU）
    val is_add  = Input(Bool())
    val is_addi = Input(Bool())
    val is_jalr = Input(Bool())
    val is_lui  = Input(Bool())
    val is_lbu  = Input(Bool())
    val is_lw   = Input(Bool())
    val is_sw   = Input(Bool())
    val is_sb   = Input(Bool())

    // 寄存器地址（来自 IDU）
    val rs1_addr = Input(UInt(5.W))
    val rs2_addr = Input(UInt(5.W))
    val rd_addr  = Input(UInt(5.W))

    // 来自顶层 GRF 的读数据
    val rs1_data = Input(UInt(32.W))
    val rs2_data = Input(UInt(32.W))

    // 立即数（来自 IDU）
    val imm12 = Input(UInt(32.W))   // 符号扩展后的 12 位立即数
    val imm20 = Input(UInt(32.W))   // LUI 立即数（已左移 12 位），本模块仅透传

    // 输出给顶层 GRF 的读控制信号
    val rs1_en = Output(Bool())
    val rs2_en = Output(Bool())

    // 输出给 LSU
    val agu_addr   = Output(UInt(32.W))   // 访存地址
    val store_data = Output(UInt(32.W))   // 要写入内存的数据（来自 rs2）

    // 输出给 WBU
    val alu_result = Output(UInt(32.W))   // ALU 计算结果
    val rd_out     = Output(UInt(5.W))    // 写寄存器地址（透传）

    // 输出给 IFU
    val jalr_out   = Output(Bool())
    val bru_target = Output(UInt(32.W))

    // 新增：寄存器地址透传（供 GRF 使用）
    val rs1_addr_out = Output(UInt(5.W))
    val rs2_addr_out = Output(UInt(5.W))
  })

  // 地址透传
  val addrPass = Module(new AddrPassthrough)
  addrPass.io.rs1_addr_in := io.rs1_addr
  addrPass.io.rs2_addr_in := io.rs2_addr
  io.rs1_addr_out := addrPass.io.rs1_addr_out
  io.rs2_addr_out := addrPass.io.rs2_addr_out

  // 寄存器读使能控制
  val grfCtrl = Module(new InputMUXforGRF)
  grfCtrl.io.add  := io.is_add
  grfCtrl.io.addi := io.is_addi
  grfCtrl.io.jalr := io.is_jalr
  grfCtrl.io.lbu  := io.is_lbu
  grfCtrl.io.lw   := io.is_lw
  grfCtrl.io.lui  := io.is_lui
  grfCtrl.io.sw   := io.is_sw
  grfCtrl.io.sb   := io.is_sb
  io.rs1_en := grfCtrl.io.rs1en
  io.rs2_en := grfCtrl.io.rs2en

  // ALU 通路
  val aluMux = Module(new InputMUXforALU)
  aluMux.io.inputfromRS1   := io.rs1_data
  aluMux.io.inputfromRS2   := io.rs2_data
  aluMux.io.inputfromIMM12 := io.imm12
  aluMux.io.add  := io.is_add
  aluMux.io.addi := io.is_addi
  aluMux.io.jalr := io.is_jalr
  aluMux.io.lbu  := io.is_lbu
  aluMux.io.lw   := io.is_lw
  aluMux.io.lui  := io.is_lui
  aluMux.io.sw   := io.is_sw
  aluMux.io.sb   := io.is_sb

  val alu = Module(new ALU)
  alu.io.source1 := aluMux.io.source1
  alu.io.source2 := aluMux.io.source2
  io.alu_result := alu.io.output

  // AGU 通路
  val aguMux = Module(new InputMUXforAGU)
  aguMux.io.inputfromRS1   := io.rs1_data
  aguMux.io.inputfromIMM12 := io.imm12
  aguMux.io.add  := io.is_add
  aguMux.io.addi := io.is_addi
  aguMux.io.jalr := io.is_jalr
  aguMux.io.lbu  := io.is_lbu
  aguMux.io.lw   := io.is_lw
  aguMux.io.lui  := io.is_lui
  aguMux.io.sw   := io.is_sw
  aguMux.io.sb   := io.is_sb

  val agu = Module(new AGU)
  agu.io.source1 := aguMux.io.source1
  agu.io.source2 := aguMux.io.source2
  io.agu_addr := agu.io.output

  // BRU 通路
  val bruMux = Module(new InputMUXforBRU)
  bruMux.io.inputfromRS1   := io.rs1_data
  bruMux.io.inputfromIMM12 := io.imm12
  bruMux.io.add  := io.is_add
  bruMux.io.addi := io.is_addi
  bruMux.io.jalr := io.is_jalr
  bruMux.io.lbu  := io.is_lbu
  bruMux.io.lw   := io.is_lw
  bruMux.io.lui  := io.is_lui
  bruMux.io.sw   := io.is_sw
  bruMux.io.sb   := io.is_sb

  val bru = Module(new BRU)
  bru.io.source1 := bruMux.io.source1
  bru.io.source2 := bruMux.io.source2
  io.bru_target := bru.io.output

  // 其他输出
  io.store_data := io.rs2_data
  io.jalr_out   := io.is_jalr
  io.rd_out     := io.rd_addr
}
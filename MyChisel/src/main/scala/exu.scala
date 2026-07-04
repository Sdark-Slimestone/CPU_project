package rv32e

import chisel3._
import chisel3.util._

// 多功能 ALU
class ALU extends Module {
  val io = IO(new Bundle {
    val source1 = Input(UInt(32.W))
    val source2 = Input(UInt(32.W))
    val alu_op  = Input(UInt(4.W))   // 操作选择
    val output  = Output(UInt(32.W))
  })

  val shamt = io.source2(4, 0)

  io.output := MuxCase(0.U(32.W), Seq(
    (io.alu_op === 0.U)  -> (io.source1 + io.source2),                // ADD, ADDI, JALR, 地址计算等
    (io.alu_op === 1.U)  -> (io.source1 - io.source2),                // SUB
    (io.alu_op === 2.U)  -> (io.source1 << shamt),                    // SLL, SLLI
    (io.alu_op === 3.U)  -> (io.source1.asSInt < io.source2.asSInt).asUInt, // SLT, SLTI
    (io.alu_op === 4.U)  -> (io.source1 < io.source2),                // SLTU, SLTIU
    (io.alu_op === 5.U)  -> (io.source1 ^ io.source2),                // XOR, XORI
    (io.alu_op === 6.U)  -> (io.source1 >> shamt),                    // SRL, SRLI
    (io.alu_op === 7.U)  -> (io.source1.asSInt >> shamt).asUInt,      // SRA, SRAI
    (io.alu_op === 8.U)  -> (io.source1 | io.source2),                // OR, ORI
    (io.alu_op === 9.U)  -> (io.source1 & io.source2),                // AND, ANDI
    (io.alu_op === 10.U) -> io.source2                                // 直通 source2 (LUI)
  ))
}

// ALU 输入多路选择器（支持 AUIPC 的 PC 输入）
class InputMUXforALU extends Module {
  val io = IO(new Bundle {
    val inputfromRS1   = Input(UInt(32.W))
    val inputfromRS2   = Input(UInt(32.W))
    val inputfromIMM   = Input(UInt(32.W))   // I 型立即数
    val inputfromU     = Input(UInt(32.W))   // U 型立即数（已左移12位）
    val current_pc     = Input(UInt(32.W))   // 用于 AUIPC
    val is_auipc       = Input(Bool())       // AUIPC 标识
    val use_rs1        = Input(Bool())
    val use_rs2        = Input(Bool())
    val use_imm_i      = Input(Bool())
    val use_u_imm      = Input(Bool())
    val source1 = Output(UInt(32.W))
    val source2 = Output(UInt(32.W))
  })

  // source1：AUIPC 时用 PC，否则用 rs1（或 0）
  io.source1 := MuxCase(0.U(32.W), Seq(
    io.is_auipc   -> io.current_pc,
    io.use_rs1    -> io.inputfromRS1
  ))

  // source2：按优先级选择 rs2 / I型立即数 / U型立即数
  io.source2 := MuxCase(0.U(32.W), Seq(
    io.use_rs2   -> io.inputfromRS2,
    io.use_imm_i -> io.inputfromIMM,
    io.use_u_imm -> io.inputfromU
  ))
}

// 地址生成单元（仅加法）
class AGU extends Module {
  val io = IO(new Bundle {
    val source1 = Input(UInt(32.W))
    val source2 = Input(UInt(32.W))
    val output  = Output(UInt(32.W))
  })
  io.output := io.source1 + io.source2
}

// AGU 输入选择（load/store 用 S 型立即数）
class InputMUXforAGU extends Module {
  val io = IO(new Bundle {
    val inputfromRS1   = Input(UInt(32.W))
    val inputfromIMM_I = Input(UInt(32.W))   // I 型立即数 (Load 用)
    val inputfromIMM_S = Input(UInt(32.W))   // S 型立即数 (Store 用)
    val is_load        = Input(Bool())
    val is_store       = Input(Bool())
    val source1 = Output(UInt(32.W))
    val source2 = Output(UInt(32.W))
  })

  io.source1 := Mux(io.is_load || io.is_store, io.inputfromRS1, 0.U(32.W))
  io.source2 := Mux(io.is_load, io.inputfromIMM_I,
                 Mux(io.is_store, io.inputfromIMM_S, 0.U(32.W)))
}

// 分支处理单元
class BRU extends Module {
  val io = IO(new Bundle {
    val rs1_data    = Input(UInt(32.W))
    val rs2_data    = Input(UInt(32.W))
    val branch_type = Input(UInt(3.W))      // funct3
    val jalr        = Input(Bool())         // 是否为 jalr
    val imm_b       = Input(UInt(32.W))     // B 型偏移
    val imm_i       = Input(UInt(32.W))     // JALR 偏移
    val current_pc  = Input(UInt(32.W))
    val take_branch = Output(Bool())
    val target      = Output(UInt(32.W))
  })

  val eq  = io.rs1_data === io.rs2_data
  val lt  = io.rs1_data.asSInt < io.rs2_data.asSInt
  val ltu = io.rs1_data < io.rs2_data

  val branch_cond = MuxCase(false.B, Seq(
    (io.branch_type === 0.U) -> eq,       // BEQ
    (io.branch_type === 1.U) -> !eq,      // BNE
    (io.branch_type === 4.U) -> lt,       // BLT
    (io.branch_type === 5.U) -> !lt,      // BGE
    (io.branch_type === 6.U) -> ltu,      // BLTU
    (io.branch_type === 7.U) -> !ltu      // BGEU
  ))

  // JALR 总是跳转，其他分支条件成立才跳
  io.take_branch := Mux(io.jalr, true.B, branch_cond)

  io.target := Mux(io.jalr,
    (io.rs1_data + io.imm_i) & (~"b11".U(32.W)),   // JALR 目标（低2位清零）
    io.current_pc + io.imm_b)                       // 分支目标
}

// 寄存器读使能控制
class InputMUXforGRF extends Module {
  val io = IO(new Bundle {
    val use_rs1 = Input(Bool())
    val use_rs2 = Input(Bool())
    val rs1en = Output(Bool())
    val rs2en = Output(Bool())
  })
  io.rs1en := io.use_rs1
  io.rs2en := io.use_rs2
}

// 地址直通
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

// 执行单元顶层
class EXU extends Module {
  val io = IO(new Bundle {
    // 指令类型
    val is_lui    = Input(Bool())
    val is_auipc  = Input(Bool())
    val is_jal    = Input(Bool())
    val is_jalr   = Input(Bool())
    val is_beq    = Input(Bool())
    val is_bne    = Input(Bool())
    val is_blt    = Input(Bool())
    val is_bge    = Input(Bool())
    val is_bltu   = Input(Bool())
    val is_bgeu   = Input(Bool())
    val is_lb     = Input(Bool())
    val is_lh     = Input(Bool())
    val is_lw     = Input(Bool())
    val is_lbu    = Input(Bool())
    val is_lhu    = Input(Bool())
    val is_sb     = Input(Bool())
    val is_sh     = Input(Bool())
    val is_sw     = Input(Bool())
    val is_addi   = Input(Bool())
    val is_slti   = Input(Bool())
    val is_sltiu  = Input(Bool())
    val is_xori   = Input(Bool())
    val is_ori    = Input(Bool())
    val is_andi   = Input(Bool())
    val is_slli   = Input(Bool())
    val is_srli   = Input(Bool())
    val is_srai   = Input(Bool())
    val is_add    = Input(Bool())
    val is_sub    = Input(Bool())
    val is_sll    = Input(Bool())
    val is_slt    = Input(Bool())
    val is_sltu   = Input(Bool())
    val is_xor    = Input(Bool())
    val is_srl    = Input(Bool())
    val is_sra    = Input(Bool())
    val is_or     = Input(Bool())
    val is_and    = Input(Bool())

    // 寄存器地址
    val rs1_addr = Input(UInt(5.W))
    val rs2_addr = Input(UInt(5.W))
    val rd_addr  = Input(UInt(5.W))

    // 寄存器数据
    val rs1_data = Input(UInt(32.W))
    val rs2_data = Input(UInt(32.W))

    // 立即数
    val imm_i = Input(UInt(32.W))
    val imm_s = Input(UInt(32.W))
    val imm_b = Input(UInt(32.W))
    val imm_u = Input(UInt(32.W))
    val imm_j = Input(UInt(32.W))

    // 当前 PC（来自 IFU）
    val current_pc = Input(UInt(32.W))

    // GRF 读使能
    val rs1_en = Output(Bool())
    val rs2_en = Output(Bool())

    // LSU 接口
    val agu_addr   = Output(UInt(32.W))
    val store_data = Output(UInt(32.W))

    // ALU 结果
    val alu_result = Output(UInt(32.W))
    val rd_out     = Output(UInt(5.W))

    // IFU 接口
    val take_branch   = Output(Bool())
    val branch_target = Output(UInt(32.W))

    // 地址透传
    val rs1_addr_out = Output(UInt(5.W))
    val rs2_addr_out = Output(UInt(5.W))

    // debug
    val alu_src1 = Output(UInt(32.W))
    val alu_src2 = Output(UInt(32.W))
  })

  // 地址直通
  val addrPass = Module(new AddrPassthrough)
  addrPass.io.rs1_addr_in := io.rs1_addr
  addrPass.io.rs2_addr_in := io.rs2_addr
  io.rs1_addr_out := addrPass.io.rs1_addr_out
  io.rs2_addr_out := addrPass.io.rs2_addr_out

  // 寄存器读使能条件
  val use_rs1 = io.is_jalr ||
                io.is_beq || io.is_bne || io.is_blt || io.is_bge || io.is_bltu || io.is_bgeu ||
                io.is_lb || io.is_lh || io.is_lw || io.is_lbu || io.is_lhu ||
                io.is_sb || io.is_sh || io.is_sw ||
                io.is_addi || io.is_slti || io.is_sltiu || io.is_xori ||
                io.is_ori || io.is_andi || io.is_slli || io.is_srli || io.is_srai ||
                io.is_add || io.is_sub || io.is_sll || io.is_slt || io.is_sltu ||
                io.is_xor || io.is_srl || io.is_sra || io.is_or || io.is_and

  val use_rs2 = io.is_beq || io.is_bne || io.is_blt || io.is_bge || io.is_bltu || io.is_bgeu ||
                io.is_sb || io.is_sh || io.is_sw ||
                io.is_add || io.is_sub || io.is_sll || io.is_slt || io.is_sltu ||
                io.is_xor || io.is_srl || io.is_sra || io.is_or || io.is_and

  val grfCtrl = Module(new InputMUXforGRF)
  grfCtrl.io.use_rs1 := use_rs1
  grfCtrl.io.use_rs2 := use_rs2
  io.rs1_en := grfCtrl.io.rs1en
  io.rs2_en := grfCtrl.io.rs2en

  // ALU 操作码生成
  val alu_op = Wire(UInt(4.W))
  alu_op := MuxCase(0.U(4.W), Seq(
    (io.is_add || io.is_addi || io.is_auipc || io.is_jal || io.is_jalr) -> 0.U,
    io.is_sub    -> 1.U,
    (io.is_sll || io.is_slli)   -> 2.U,
    (io.is_slt || io.is_slti)   -> 3.U,
    (io.is_sltu || io.is_sltiu) -> 4.U,
    (io.is_xor || io.is_xori)   -> 5.U,
    (io.is_srl || io.is_srli)   -> 6.U,
    (io.is_sra || io.is_srai)   -> 7.U,
    (io.is_or || io.is_ori)     -> 8.U,
    (io.is_and || io.is_andi)   -> 9.U,
    io.is_lui                   -> 10.U
  ))

  // ALU 源选择条件
  val is_r_type = io.is_add || io.is_sub || io.is_sll || io.is_slt || io.is_sltu ||
                  io.is_xor || io.is_srl || io.is_sra || io.is_or || io.is_and
  val use_imm_i = io.is_addi || io.is_slti || io.is_sltiu || io.is_xori ||
                  io.is_ori || io.is_andi || io.is_slli || io.is_srli || io.is_srai ||
                  io.is_jalr || io.is_lb || io.is_lh || io.is_lw || io.is_lbu || io.is_lhu
  val use_u_imm = io.is_lui || io.is_auipc
  val need_rs1_alu = use_rs1 && !io.is_sb && !io.is_sh && !io.is_sw  // 非存储指令都可能用 rs1 送 ALU

  val aluMux = Module(new InputMUXforALU)
  aluMux.io.inputfromRS1 := io.rs1_data
  aluMux.io.inputfromRS2 := io.rs2_data
  aluMux.io.inputfromIMM := io.imm_i
  aluMux.io.inputfromU   := io.imm_u
  aluMux.io.current_pc   := io.current_pc
  aluMux.io.is_auipc     := io.is_auipc
  aluMux.io.use_rs1      := need_rs1_alu && !io.is_auipc   // AUIPC 时不用 rs1
  aluMux.io.use_rs2      := is_r_type
  aluMux.io.use_imm_i    := use_imm_i
  aluMux.io.use_u_imm    := use_u_imm

  val alu = Module(new ALU)
  alu.io.source1 := aluMux.io.source1
  alu.io.source2 := aluMux.io.source2
  alu.io.alu_op  := alu_op
  io.alu_result  := alu.io.output

  // AGU 通路
  val aguMux = Module(new InputMUXforAGU)
  aguMux.io.inputfromRS1   := io.rs1_data
  aguMux.io.inputfromIMM_I := io.imm_i   // Load 用 I 型立即数
  aguMux.io.inputfromIMM_S := io.imm_s   // Store 用 S 型立即数
  aguMux.io.is_load        := io.is_lb || io.is_lh || io.is_lw || io.is_lbu || io.is_lhu
  aguMux.io.is_store       := io.is_sb || io.is_sh || io.is_sw

  val agu = Module(new AGU)
  agu.io.source1 := aguMux.io.source1
  agu.io.source2 := aguMux.io.source2
  io.agu_addr := agu.io.output
  // 存储数据直通
  io.store_data := io.rs2_data

  // 分支单元
  val bru = Module(new BRU)
  bru.io.rs1_data    := io.rs1_data
  bru.io.rs2_data    := io.rs2_data
  bru.io.branch_type := MuxCase(0.U(3.W), Seq(
    io.is_beq  -> 0.U,
    io.is_bne  -> 1.U,
    io.is_blt  -> 4.U,
    io.is_bge  -> 5.U,
    io.is_bltu -> 6.U,
    io.is_bgeu -> 7.U
  ))
  bru.io.jalr       := io.is_jalr
  bru.io.imm_b      := io.imm_b
  bru.io.imm_i      := io.imm_i
  bru.io.current_pc := io.current_pc

  // 分支跳转使能：仅 JAL/JALR/条件分支成立时才有效
  val is_branch = io.is_beq || io.is_bne || io.is_blt || io.is_bge || io.is_bltu || io.is_bgeu
  io.take_branch := io.is_jal || io.is_jalr || (is_branch && bru.io.take_branch)
  io.branch_target := Mux(io.is_jal, io.current_pc + io.imm_j, bru.io.target)

  // rd 地址直通
  io.rd_out := io.rd_addr

  // debug 输出
  io.alu_src1 := aluMux.io.source1
  io.alu_src2 := aluMux.io.source2
}
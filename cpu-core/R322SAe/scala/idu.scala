package R322SAe

import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._



//指令译码单元
//同时输入两条指令，检测数据冒险（RAW, WAW, WAR）以及控制冒险，
//若需停顿则 stall 拉高，并屏蔽第二条指令的译码结果。
class idu extends Module {
  val io = IO(new Bundle {
    val inst1 = Input(UInt(32.W))  // 较旧的指令
    val inst2 = Input(UInt(32.W))  // 较新的指令
    // 链接寄存器的pc值透传
    val inst1nexttoexu = Output(UInt(32.W))  
    val inst2nexttoexu = Output(UInt(32.W))  

    val inst1nexttoidu = Input(UInt(32.W))  
    val inst2nexttoidu = Input(UInt(32.W))  

    // 第一条指令的译码输出
    val dec1 = new Bundle {
      val is_lui    = Output(Bool())
      val is_auipc  = Output(Bool())
      val is_jal    = Output(Bool())
      val is_jalr   = Output(Bool())
      val is_beq    = Output(Bool())
      val is_bne    = Output(Bool())
      val is_blt    = Output(Bool())
      val is_bge    = Output(Bool())
      val is_bltu   = Output(Bool())
      val is_bgeu   = Output(Bool())
      val is_lb     = Output(Bool())
      val is_lh     = Output(Bool())
      val is_lw     = Output(Bool())
      val is_lbu    = Output(Bool())
      val is_lhu    = Output(Bool())
      val is_sb     = Output(Bool())
      val is_sh     = Output(Bool())
      val is_sw     = Output(Bool())
      val is_addi   = Output(Bool())
      val is_slti   = Output(Bool())
      val is_sltiu  = Output(Bool())
      val is_xori   = Output(Bool())
      val is_ori    = Output(Bool())
      val is_andi   = Output(Bool())
      val is_slli   = Output(Bool())
      val is_srli   = Output(Bool())
      val is_srai   = Output(Bool())
      val is_add    = Output(Bool())
      val is_sub    = Output(Bool())
      val is_sll    = Output(Bool())
      val is_slt    = Output(Bool())
      val is_sltu   = Output(Bool())
      val is_xor    = Output(Bool())
      val is_srl    = Output(Bool())
      val is_sra    = Output(Bool())
      val is_or     = Output(Bool())
      val is_and    = Output(Bool())
      val is_ebreak = Output(Bool())
      val rd        = Output(UInt(5.W))
      val rs1       = Output(UInt(5.W))
      val rs2       = Output(UInt(5.W))
      val imm_i     = Output(UInt(32.W))
      val imm_s     = Output(UInt(32.W))
      val imm_b     = Output(UInt(32.W))
      val imm_u     = Output(UInt(32.W))
      val imm_j     = Output(UInt(32.W))
      val imm12     = Output(UInt(32.W))
      val imm20     = Output(UInt(32.W))
      val debug_inst = Output(UInt(32.W))
    }

    // 第二条指令的译码输出（stall 时全为 0）
    val dec2 = new Bundle {
      val is_lui    = Output(Bool())
      val is_auipc  = Output(Bool())
      val is_jal    = Output(Bool())
      val is_jalr   = Output(Bool())
      val is_beq    = Output(Bool())
      val is_bne    = Output(Bool())
      val is_blt    = Output(Bool())
      val is_bge    = Output(Bool())
      val is_bltu   = Output(Bool())
      val is_bgeu   = Output(Bool())
      val is_lb     = Output(Bool())
      val is_lh     = Output(Bool())
      val is_lw     = Output(Bool())
      val is_lbu    = Output(Bool())
      val is_lhu    = Output(Bool())
      val is_sb     = Output(Bool())
      val is_sh     = Output(Bool())
      val is_sw     = Output(Bool())
      val is_addi   = Output(Bool())
      val is_slti   = Output(Bool())
      val is_sltiu  = Output(Bool())
      val is_xori   = Output(Bool())
      val is_ori    = Output(Bool())
      val is_andi   = Output(Bool())
      val is_slli   = Output(Bool())
      val is_srli   = Output(Bool())
      val is_srai   = Output(Bool())
      val is_add    = Output(Bool())
      val is_sub    = Output(Bool())
      val is_sll    = Output(Bool())
      val is_slt    = Output(Bool())
      val is_sltu   = Output(Bool())
      val is_xor    = Output(Bool())
      val is_srl    = Output(Bool())
      val is_sra    = Output(Bool())
      val is_or     = Output(Bool())
      val is_and    = Output(Bool())
      val is_ebreak = Output(Bool())
      val rd        = Output(UInt(5.W))
      val rs1       = Output(UInt(5.W))
      val rs2       = Output(UInt(5.W))
      val imm_i     = Output(UInt(32.W))
      val imm_s     = Output(UInt(32.W))
      val imm_b     = Output(UInt(32.W))
      val imm_u     = Output(UInt(32.W))
      val imm_j     = Output(UInt(32.W))
      val imm12     = Output(UInt(32.W))
      val imm20     = Output(UInt(32.W))
      val debug_inst = Output(UInt(32.W))
    }

    // 停顿标志（组合逻辑）
    val stall = Output(Bool())
  })

  // 实例化两个译码器
  val dec1 = Module(new decoder)
  val dec2 = Module(new decoder)
  dec1.io.inst := io.inst1
  dec2.io.inst := io.inst2

  //  冒险检测 
  val isControl1 = dec1.io.is_jal  || dec1.io.is_jalr ||
                   dec1.io.is_beq  || dec1.io.is_bne  ||
                   dec1.io.is_blt  || dec1.io.is_bge  ||
                   dec1.io.is_bltu || dec1.io.is_bgeu || dec1.io.is_ebreak
  val isControl2 = dec2.io.is_jal  || dec2.io.is_jalr ||
                   dec2.io.is_beq  || dec2.io.is_bne  ||
                   dec2.io.is_blt  || dec2.io.is_bge  ||
                   dec2.io.is_bltu || dec2.io.is_bgeu || dec2.io.is_ebreak
  val isStore1   = dec1.io.is_sb || dec1.io.is_sh || dec1.io.is_sw
  val isStore2   = dec2.io.is_sb || dec2.io.is_sh || dec2.io.is_sw

  // RAW: 指令1写寄存器 rd1 且被指令2作为源操作数使用
  val raw = (dec1.io.rd =/= 0.U) &&
            ((dec1.io.rd === dec2.io.rs1) || (dec1.io.rd === dec2.io.rs2))

  // WAW: 两条指令写同一个非零寄存器
  val waw = (dec1.io.rd =/= 0.U) && (dec2.io.rd =/= 0.U) &&
            (dec1.io.rd === dec2.io.rd)

  // WAR: 指令2写寄存器 rd2 且指令1使用了相同的源寄存器
  val war = (dec2.io.rd =/= 0.U) &&
            ((dec1.io.rs1 === dec2.io.rd) || (dec1.io.rs2 === dec2.io.rd))

  // 停顿条件组合
  io.stall := raw || waw || war ||
              (isControl1 || isControl2) ||
              (isStore1 && isStore2)

  // ---------- 第一条指令译码结果：直接输出 ----------
  io.dec1.is_lui    := dec1.io.is_lui
  io.dec1.is_auipc  := dec1.io.is_auipc
  io.dec1.is_jal    := dec1.io.is_jal
  io.dec1.is_jalr   := dec1.io.is_jalr
  io.dec1.is_beq    := dec1.io.is_beq
  io.dec1.is_bne    := dec1.io.is_bne
  io.dec1.is_blt    := dec1.io.is_blt
  io.dec1.is_bge    := dec1.io.is_bge
  io.dec1.is_bltu   := dec1.io.is_bltu
  io.dec1.is_bgeu   := dec1.io.is_bgeu
  io.dec1.is_lb     := dec1.io.is_lb
  io.dec1.is_lh     := dec1.io.is_lh
  io.dec1.is_lw     := dec1.io.is_lw
  io.dec1.is_lbu    := dec1.io.is_lbu
  io.dec1.is_lhu    := dec1.io.is_lhu
  io.dec1.is_sb     := dec1.io.is_sb
  io.dec1.is_sh     := dec1.io.is_sh
  io.dec1.is_sw     := dec1.io.is_sw
  io.dec1.is_addi   := dec1.io.is_addi
  io.dec1.is_slti   := dec1.io.is_slti
  io.dec1.is_sltiu  := dec1.io.is_sltiu
  io.dec1.is_xori   := dec1.io.is_xori
  io.dec1.is_ori    := dec1.io.is_ori
  io.dec1.is_andi   := dec1.io.is_andi
  io.dec1.is_slli   := dec1.io.is_slli
  io.dec1.is_srli   := dec1.io.is_srli
  io.dec1.is_srai   := dec1.io.is_srai
  io.dec1.is_add    := dec1.io.is_add
  io.dec1.is_sub    := dec1.io.is_sub
  io.dec1.is_sll    := dec1.io.is_sll
  io.dec1.is_slt    := dec1.io.is_slt
  io.dec1.is_sltu   := dec1.io.is_sltu
  io.dec1.is_xor    := dec1.io.is_xor
  io.dec1.is_srl    := dec1.io.is_srl
  io.dec1.is_sra    := dec1.io.is_sra
  io.dec1.is_or     := dec1.io.is_or
  io.dec1.is_and    := dec1.io.is_and
  io.dec1.is_ebreak := dec1.io.is_ebreak
  io.dec1.rd        := dec1.io.rd
  io.dec1.rs1       := dec1.io.rs1
  io.dec1.rs2       := dec1.io.rs2
  io.dec1.imm_i     := dec1.io.imm_i
  io.dec1.imm_s     := dec1.io.imm_s
  io.dec1.imm_b     := dec1.io.imm_b
  io.dec1.imm_u     := dec1.io.imm_u
  io.dec1.imm_j     := dec1.io.imm_j
  io.dec1.imm12     := dec1.io.imm12
  io.dec1.imm20     := dec1.io.imm20
  io.dec1.debug_inst := dec1.io.debug_inst

  // ---------- 第二条指令译码结果：停顿则为0 ----------
  io.dec2.is_lui    := Mux(io.stall, false.B, dec2.io.is_lui)
  io.dec2.is_auipc  := Mux(io.stall, false.B, dec2.io.is_auipc)
  io.dec2.is_jal    := Mux(io.stall, false.B, dec2.io.is_jal)
  io.dec2.is_jalr   := Mux(io.stall, false.B, dec2.io.is_jalr)
  io.dec2.is_beq    := Mux(io.stall, false.B, dec2.io.is_beq)
  io.dec2.is_bne    := Mux(io.stall, false.B, dec2.io.is_bne)
  io.dec2.is_blt    := Mux(io.stall, false.B, dec2.io.is_blt)
  io.dec2.is_bge    := Mux(io.stall, false.B, dec2.io.is_bge)
  io.dec2.is_bltu   := Mux(io.stall, false.B, dec2.io.is_bltu)
  io.dec2.is_bgeu   := Mux(io.stall, false.B, dec2.io.is_bgeu)
  io.dec2.is_lb     := Mux(io.stall, false.B, dec2.io.is_lb)
  io.dec2.is_lh     := Mux(io.stall, false.B, dec2.io.is_lh)
  io.dec2.is_lw     := Mux(io.stall, false.B, dec2.io.is_lw)
  io.dec2.is_lbu    := Mux(io.stall, false.B, dec2.io.is_lbu)
  io.dec2.is_lhu    := Mux(io.stall, false.B, dec2.io.is_lhu)
  io.dec2.is_sb     := Mux(io.stall, false.B, dec2.io.is_sb)
  io.dec2.is_sh     := Mux(io.stall, false.B, dec2.io.is_sh)
  io.dec2.is_sw     := Mux(io.stall, false.B, dec2.io.is_sw)
  io.dec2.is_addi   := Mux(io.stall, false.B, dec2.io.is_addi)
  io.dec2.is_slti   := Mux(io.stall, false.B, dec2.io.is_slti)
  io.dec2.is_sltiu  := Mux(io.stall, false.B, dec2.io.is_sltiu)
  io.dec2.is_xori   := Mux(io.stall, false.B, dec2.io.is_xori)
  io.dec2.is_ori    := Mux(io.stall, false.B, dec2.io.is_ori)
  io.dec2.is_andi   := Mux(io.stall, false.B, dec2.io.is_andi)
  io.dec2.is_slli   := Mux(io.stall, false.B, dec2.io.is_slli)
  io.dec2.is_srli   := Mux(io.stall, false.B, dec2.io.is_srli)
  io.dec2.is_srai   := Mux(io.stall, false.B, dec2.io.is_srai)
  io.dec2.is_add    := Mux(io.stall, false.B, dec2.io.is_add)
  io.dec2.is_sub    := Mux(io.stall, false.B, dec2.io.is_sub)
  io.dec2.is_sll    := Mux(io.stall, false.B, dec2.io.is_sll)
  io.dec2.is_slt    := Mux(io.stall, false.B, dec2.io.is_slt)
  io.dec2.is_sltu   := Mux(io.stall, false.B, dec2.io.is_sltu)
  io.dec2.is_xor    := Mux(io.stall, false.B, dec2.io.is_xor)
  io.dec2.is_srl    := Mux(io.stall, false.B, dec2.io.is_srl)
  io.dec2.is_sra    := Mux(io.stall, false.B, dec2.io.is_sra)
  io.dec2.is_or     := Mux(io.stall, false.B, dec2.io.is_or)
  io.dec2.is_and    := Mux(io.stall, false.B, dec2.io.is_and)
  io.dec2.is_ebreak := Mux(io.stall, false.B, dec2.io.is_ebreak)
  io.dec2.rd        := Mux(io.stall, 0.U(5.W), dec2.io.rd)
  io.dec2.rs1       := Mux(io.stall, 0.U(5.W), dec2.io.rs1)
  io.dec2.rs2       := Mux(io.stall, 0.U(5.W), dec2.io.rs2)
  io.dec2.imm_i     := Mux(io.stall, 0.U(32.W), dec2.io.imm_i)
  io.dec2.imm_s     := Mux(io.stall, 0.U(32.W), dec2.io.imm_s)
  io.dec2.imm_b     := Mux(io.stall, 0.U(32.W), dec2.io.imm_b)
  io.dec2.imm_u     := Mux(io.stall, 0.U(32.W), dec2.io.imm_u)
  io.dec2.imm_j     := Mux(io.stall, 0.U(32.W), dec2.io.imm_j)
  io.dec2.imm12     := Mux(io.stall, 0.U(32.W), dec2.io.imm12)
  io.dec2.imm20     := Mux(io.stall, 0.U(32.W), dec2.io.imm20)
  io.dec2.debug_inst := Mux(io.stall, 0.U(32.W), dec2.io.debug_inst)

  //透传接线
  io.inst1nexttoexu := io.inst1nexttoidu
  io.inst2nexttoexu := io.inst2nexttoidu
}
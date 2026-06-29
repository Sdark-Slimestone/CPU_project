package R1322IAeCSR

import chisel3._
import chisel3.util._

// 指令解码与发射单元，支持双发射，CSR指令强制单发射
class idu extends Module {
  val io = IO(new Bundle {
    // 来自 IFU
    val ifu_to_idu = new Bundle {
      val inst1        = Input(UInt(32.W))
      val inst2        = Input(UInt(32.W))
      val inst1_pc     = Input(UInt(32.W))
      val inst2_pc     = Input(UInt(32.W))
      val inst1_nextpc = Input(UInt(32.W))
      val inst2_nextpc = Input(UInt(32.W))
    }

    // 来自 GRF 的寄存器值
    val grf_to_idu = new Bundle {
      val dec1_value = new Bundle {
        val inst1rs1_value = Input(UInt(32.W))
        val inst1rs2_value = Input(UInt(32.W))
      }
      val dec2_value = new Bundle {
        val inst2rs1_value = Input(UInt(32.W))
        val inst2rs2_value = Input(UInt(32.W))
      }
    }

    // 发往 GRF 的读地址
    val idu_to_grf = new Bundle {
      val dec1_redreg = new Bundle {
        val rs1 = Output(UInt(5.W))
        val rs2 = Output(UInt(5.W))
      }
      val dec2_redreg = new Bundle {
        val rs1 = Output(UInt(5.W))
        val rs2 = Output(UInt(5.W))
      }
    }

    // 第一条指令 -> EXU1
    val idu_to_exu1 = new Bundle {
      val dec1_op = new Bundle {
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
        // CSR 信号
        val is_csrrw  = Output(Bool())
        val is_csrrs  = Output(Bool())
        val is_csrrc  = Output(Bool())
        val is_csrrwi = Output(Bool())
        val is_csrrsi = Output(Bool())
        val is_csrrci = Output(Bool())
        val is_ecall  = Output(Bool())
        val is_mret   = Output(Bool())
      }
      val dec1_imm = Output(UInt(32.W))
      val dec1_val = new Bundle {
        val rs1_val = Output(UInt(32.W))
        val rs2_val = Output(UInt(32.W))
        val nextpc  = Output(UInt(32.W))
      }
      val dec1_rd = Output(UInt(5.W))
    }

    // 第二条指令 -> EXU2
    val idu_to_exu2 = new Bundle {
      val dec2_op = new Bundle {
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
        // CSR 信号
        val is_csrrw  = Output(Bool())
        val is_csrrs  = Output(Bool())
        val is_csrrc  = Output(Bool())
        val is_csrrwi = Output(Bool())
        val is_csrrsi = Output(Bool())
        val is_csrrci = Output(Bool())
        val is_ecall  = Output(Bool())
        val is_mret   = Output(Bool())
      }
      val dec2_imm = Output(UInt(32.W))
      val dec2_val = new Bundle {
        val rs1_val = Output(UInt(32.W))
        val rs2_val = Output(UInt(32.W))
        val nextpc  = Output(UInt(32.W))
      }
      val dec2_rd = Output(UInt(5.W))
    }

    // 发往 IFU 的 stall 信号
    val idu_to_ifu = new Bundle {
      val is_stall = Output(Bool())
    }

    // CSR 相关信号直通到 top
    val idu_to_top = new Bundle {
      val is_csrrw  = Output(Bool())
      val is_csrrs  = Output(Bool())
      val is_csrrc  = Output(Bool())
      val is_csrrwi = Output(Bool())
      val is_csrrsi = Output(Bool())
      val is_csrrci = Output(Bool())
      val is_ecall  = Output(Bool())
      val is_mret   = Output(Bool())
      val is_ebreak = Output(Bool())
      val inst1_pc  = Output(UInt(32.W))
      val inst1     = Output(UInt(32.W))
      val rs1_val   = Output(UInt(32.W))
    }

    // 调试端口
    val idu_debug = new Bundle {
      val debug_inst1 = Output(UInt(32.W))
      val debug_inst2 = Output(UInt(32.W))
      val is_stall    = Output(Bool())
    }
  })

  // 实例化译码器
  val dec1 = Module(new decoder)
  val dec2 = Module(new decoder)
  dec1.io.inst := io.ifu_to_idu.inst1
  dec2.io.inst := io.ifu_to_idu.inst2

  // ---------- 冒险检测 ----------
  // 控制指令（含 CSR）强制单发射
  val isControl1 = dec1.io.is_jal || dec1.io.is_jalr ||
                   dec1.io.is_beq || dec1.io.is_bne ||
                   dec1.io.is_blt || dec1.io.is_bge ||
                   dec1.io.is_bltu || dec1.io.is_bgeu ||
                   dec1.io.is_ebreak ||
                   dec1.io.is_csrrw || dec1.io.is_csrrs || dec1.io.is_csrrc ||
                   dec1.io.is_csrrwi || dec1.io.is_csrrsi || dec1.io.is_csrrci ||
                   dec1.io.is_ecall || dec1.io.is_mret

  val isControl2 = dec2.io.is_jal || dec2.io.is_jalr ||
                   dec2.io.is_beq || dec2.io.is_bne ||
                   dec2.io.is_blt || dec2.io.is_bge ||
                   dec2.io.is_bltu || dec2.io.is_bgeu ||
                   dec2.io.is_ebreak ||
                   dec2.io.is_csrrw || dec2.io.is_csrrs || dec2.io.is_csrrc ||
                   dec2.io.is_csrrwi || dec2.io.is_csrrsi || dec2.io.is_csrrci ||
                   dec2.io.is_ecall || dec2.io.is_mret

  // 数据冒险：inst1 写入 rd，inst2 读取同一寄存器
  val data_hazard = (dec1.io.rd =/= 0.U) &&
                    (dec1.io.rd === dec2.io.rs1 || dec1.io.rd === dec2.io.rs2)

  // load-use 冒险：inst1 是 load 指令，inst2 使用其 rd
  val is_load1 = dec1.io.is_lb || dec1.io.is_lh || dec1.io.is_lw ||
                 dec1.io.is_lbu || dec1.io.is_lhu
  val load_use = is_load1 && data_hazard

  // stall 条件：控制指令、CSR 指令、load-use 冒险
  val stall_sig = isControl1 || load_use

  // 当 inst2 是控制指令但 inst1 不是时也 stall（inst2 不能单独跳转）
  val stall_sig2 = !isControl1 && isControl2
  val final_stall = stall_sig || stall_sig2

  io.idu_to_ifu.is_stall := final_stall

  // ---------- GRF 读地址 ----------
  io.idu_to_grf.dec1_redreg.rs1 := dec1.io.rs1
  io.idu_to_grf.dec1_redreg.rs2 := dec1.io.rs2
  io.idu_to_grf.dec2_redreg.rs1 := dec2.io.rs1
  io.idu_to_grf.dec2_redreg.rs2 := dec2.io.rs2

  // ---------- 第一条指令输出 ----------
  io.idu_to_exu1.dec1_op.is_lui    := dec1.io.is_lui
  io.idu_to_exu1.dec1_op.is_auipc  := dec1.io.is_auipc
  io.idu_to_exu1.dec1_op.is_jal    := dec1.io.is_jal
  io.idu_to_exu1.dec1_op.is_jalr   := dec1.io.is_jalr
  io.idu_to_exu1.dec1_op.is_beq    := dec1.io.is_beq
  io.idu_to_exu1.dec1_op.is_bne    := dec1.io.is_bne
  io.idu_to_exu1.dec1_op.is_blt    := dec1.io.is_blt
  io.idu_to_exu1.dec1_op.is_bge    := dec1.io.is_bge
  io.idu_to_exu1.dec1_op.is_bltu   := dec1.io.is_bltu
  io.idu_to_exu1.dec1_op.is_bgeu   := dec1.io.is_bgeu
  io.idu_to_exu1.dec1_op.is_lb     := dec1.io.is_lb
  io.idu_to_exu1.dec1_op.is_lh     := dec1.io.is_lh
  io.idu_to_exu1.dec1_op.is_lw     := dec1.io.is_lw
  io.idu_to_exu1.dec1_op.is_lbu    := dec1.io.is_lbu
  io.idu_to_exu1.dec1_op.is_lhu    := dec1.io.is_lhu
  io.idu_to_exu1.dec1_op.is_sb     := dec1.io.is_sb
  io.idu_to_exu1.dec1_op.is_sh     := dec1.io.is_sh
  io.idu_to_exu1.dec1_op.is_sw     := dec1.io.is_sw
  io.idu_to_exu1.dec1_op.is_addi   := dec1.io.is_addi
  io.idu_to_exu1.dec1_op.is_slti   := dec1.io.is_slti
  io.idu_to_exu1.dec1_op.is_sltiu  := dec1.io.is_sltiu
  io.idu_to_exu1.dec1_op.is_xori   := dec1.io.is_xori
  io.idu_to_exu1.dec1_op.is_ori    := dec1.io.is_ori
  io.idu_to_exu1.dec1_op.is_andi   := dec1.io.is_andi
  io.idu_to_exu1.dec1_op.is_slli   := dec1.io.is_slli
  io.idu_to_exu1.dec1_op.is_srli   := dec1.io.is_srli
  io.idu_to_exu1.dec1_op.is_srai   := dec1.io.is_srai
  io.idu_to_exu1.dec1_op.is_add    := dec1.io.is_add
  io.idu_to_exu1.dec1_op.is_sub    := dec1.io.is_sub
  io.idu_to_exu1.dec1_op.is_sll    := dec1.io.is_sll
  io.idu_to_exu1.dec1_op.is_slt    := dec1.io.is_slt
  io.idu_to_exu1.dec1_op.is_sltu   := dec1.io.is_sltu
  io.idu_to_exu1.dec1_op.is_xor    := dec1.io.is_xor
  io.idu_to_exu1.dec1_op.is_srl    := dec1.io.is_srl
  io.idu_to_exu1.dec1_op.is_sra    := dec1.io.is_sra
  io.idu_to_exu1.dec1_op.is_or     := dec1.io.is_or
  io.idu_to_exu1.dec1_op.is_and    := dec1.io.is_and
  io.idu_to_exu1.dec1_op.is_ebreak := dec1.io.is_ebreak
  io.idu_to_exu1.dec1_op.is_csrrw  := dec1.io.is_csrrw
  io.idu_to_exu1.dec1_op.is_csrrs  := dec1.io.is_csrrs
  io.idu_to_exu1.dec1_op.is_csrrc  := dec1.io.is_csrrc
  io.idu_to_exu1.dec1_op.is_csrrwi := dec1.io.is_csrrwi
  io.idu_to_exu1.dec1_op.is_csrrsi := dec1.io.is_csrrsi
  io.idu_to_exu1.dec1_op.is_csrrci := dec1.io.is_csrrci
  io.idu_to_exu1.dec1_op.is_ecall  := dec1.io.is_ecall
  io.idu_to_exu1.dec1_op.is_mret   := dec1.io.is_mret

  io.idu_to_exu1.dec1_imm := dec1.io.imm
  io.idu_to_exu1.dec1_rd  := dec1.io.rd

  io.idu_to_exu1.dec1_val.rs1_val := io.grf_to_idu.dec1_value.inst1rs1_value
  io.idu_to_exu1.dec1_val.rs2_val := io.grf_to_idu.dec1_value.inst1rs2_value
  io.idu_to_exu1.dec1_val.nextpc  := io.ifu_to_idu.inst1_nextpc

  // ---------- 第二条指令输出（stall 时全部清零）----------
  io.idu_to_exu2.dec2_op.is_lui    := Mux(final_stall, false.B, dec2.io.is_lui)
  io.idu_to_exu2.dec2_op.is_auipc  := Mux(final_stall, false.B, dec2.io.is_auipc)
  io.idu_to_exu2.dec2_op.is_jal    := Mux(final_stall, false.B, dec2.io.is_jal)
  io.idu_to_exu2.dec2_op.is_jalr   := Mux(final_stall, false.B, dec2.io.is_jalr)
  io.idu_to_exu2.dec2_op.is_beq    := Mux(final_stall, false.B, dec2.io.is_beq)
  io.idu_to_exu2.dec2_op.is_bne    := Mux(final_stall, false.B, dec2.io.is_bne)
  io.idu_to_exu2.dec2_op.is_blt    := Mux(final_stall, false.B, dec2.io.is_blt)
  io.idu_to_exu2.dec2_op.is_bge    := Mux(final_stall, false.B, dec2.io.is_bge)
  io.idu_to_exu2.dec2_op.is_bltu   := Mux(final_stall, false.B, dec2.io.is_bltu)
  io.idu_to_exu2.dec2_op.is_bgeu   := Mux(final_stall, false.B, dec2.io.is_bgeu)
  io.idu_to_exu2.dec2_op.is_lb     := Mux(final_stall, false.B, dec2.io.is_lb)
  io.idu_to_exu2.dec2_op.is_lh     := Mux(final_stall, false.B, dec2.io.is_lh)
  io.idu_to_exu2.dec2_op.is_lw     := Mux(final_stall, false.B, dec2.io.is_lw)
  io.idu_to_exu2.dec2_op.is_lbu    := Mux(final_stall, false.B, dec2.io.is_lbu)
  io.idu_to_exu2.dec2_op.is_lhu    := Mux(final_stall, false.B, dec2.io.is_lhu)
  io.idu_to_exu2.dec2_op.is_sb     := Mux(final_stall, false.B, dec2.io.is_sb)
  io.idu_to_exu2.dec2_op.is_sh     := Mux(final_stall, false.B, dec2.io.is_sh)
  io.idu_to_exu2.dec2_op.is_sw     := Mux(final_stall, false.B, dec2.io.is_sw)
  io.idu_to_exu2.dec2_op.is_addi   := Mux(final_stall, false.B, dec2.io.is_addi)
  io.idu_to_exu2.dec2_op.is_slti   := Mux(final_stall, false.B, dec2.io.is_slti)
  io.idu_to_exu2.dec2_op.is_sltiu  := Mux(final_stall, false.B, dec2.io.is_sltiu)
  io.idu_to_exu2.dec2_op.is_xori   := Mux(final_stall, false.B, dec2.io.is_xori)
  io.idu_to_exu2.dec2_op.is_ori    := Mux(final_stall, false.B, dec2.io.is_ori)
  io.idu_to_exu2.dec2_op.is_andi   := Mux(final_stall, false.B, dec2.io.is_andi)
  io.idu_to_exu2.dec2_op.is_slli   := Mux(final_stall, false.B, dec2.io.is_slli)
  io.idu_to_exu2.dec2_op.is_srli   := Mux(final_stall, false.B, dec2.io.is_srli)
  io.idu_to_exu2.dec2_op.is_srai   := Mux(final_stall, false.B, dec2.io.is_srai)
  io.idu_to_exu2.dec2_op.is_add    := Mux(final_stall, false.B, dec2.io.is_add)
  io.idu_to_exu2.dec2_op.is_sub    := Mux(final_stall, false.B, dec2.io.is_sub)
  io.idu_to_exu2.dec2_op.is_sll    := Mux(final_stall, false.B, dec2.io.is_sll)
  io.idu_to_exu2.dec2_op.is_slt    := Mux(final_stall, false.B, dec2.io.is_slt)
  io.idu_to_exu2.dec2_op.is_sltu   := Mux(final_stall, false.B, dec2.io.is_sltu)
  io.idu_to_exu2.dec2_op.is_xor    := Mux(final_stall, false.B, dec2.io.is_xor)
  io.idu_to_exu2.dec2_op.is_srl    := Mux(final_stall, false.B, dec2.io.is_srl)
  io.idu_to_exu2.dec2_op.is_sra    := Mux(final_stall, false.B, dec2.io.is_sra)
  io.idu_to_exu2.dec2_op.is_or     := Mux(final_stall, false.B, dec2.io.is_or)
  io.idu_to_exu2.dec2_op.is_and    := Mux(final_stall, false.B, dec2.io.is_and)
  io.idu_to_exu2.dec2_op.is_ebreak := Mux(final_stall, false.B, dec2.io.is_ebreak)
  io.idu_to_exu2.dec2_op.is_csrrw  := Mux(final_stall, false.B, dec2.io.is_csrrw)
  io.idu_to_exu2.dec2_op.is_csrrs  := Mux(final_stall, false.B, dec2.io.is_csrrs)
  io.idu_to_exu2.dec2_op.is_csrrc  := Mux(final_stall, false.B, dec2.io.is_csrrc)
  io.idu_to_exu2.dec2_op.is_csrrwi := Mux(final_stall, false.B, dec2.io.is_csrrwi)
  io.idu_to_exu2.dec2_op.is_csrrsi := Mux(final_stall, false.B, dec2.io.is_csrrsi)
  io.idu_to_exu2.dec2_op.is_csrrci := Mux(final_stall, false.B, dec2.io.is_csrrci)
  io.idu_to_exu2.dec2_op.is_ecall  := Mux(final_stall, false.B, dec2.io.is_ecall)
  io.idu_to_exu2.dec2_op.is_mret   := Mux(final_stall, false.B, dec2.io.is_mret)

  io.idu_to_exu2.dec2_imm := Mux(final_stall, 0.U(32.W), dec2.io.imm)
  io.idu_to_exu2.dec2_rd  := Mux(final_stall, 0.U(5.W), dec2.io.rd)

  io.idu_to_exu2.dec2_val.rs1_val := Mux(final_stall, 0.U(32.W), io.grf_to_idu.dec2_value.inst2rs1_value)
  io.idu_to_exu2.dec2_val.rs2_val := Mux(final_stall, 0.U(32.W), io.grf_to_idu.dec2_value.inst2rs2_value)
  io.idu_to_exu2.dec2_val.nextpc  := Mux(final_stall, 0.U(32.W), io.ifu_to_idu.inst2_nextpc)

  // ---------- CSR 直通到 top ----------
  io.idu_to_top.is_csrrw  := dec1.io.is_csrrw
  io.idu_to_top.is_csrrs  := dec1.io.is_csrrs
  io.idu_to_top.is_csrrc  := dec1.io.is_csrrc
  io.idu_to_top.is_csrrwi := dec1.io.is_csrrwi
  io.idu_to_top.is_csrrsi := dec1.io.is_csrrsi
  io.idu_to_top.is_csrrci := dec1.io.is_csrrci
  io.idu_to_top.is_ecall  := dec1.io.is_ecall
  io.idu_to_top.is_mret   := dec1.io.is_mret
  io.idu_to_top.is_ebreak := dec1.io.is_ebreak
  io.idu_to_top.inst1_pc  := io.ifu_to_idu.inst1_pc
  io.idu_to_top.inst1     := io.ifu_to_idu.inst1
  io.idu_to_top.rs1_val   := io.grf_to_idu.dec1_value.inst1rs1_value

  // ---------- 调试输出 ----------
  io.idu_debug.debug_inst1 := dec1.io.debug_inst
  io.idu_debug.debug_inst2 := Mux(final_stall, 0.U(32.W), dec2.io.debug_inst)
  io.idu_debug.is_stall    := final_stall
}
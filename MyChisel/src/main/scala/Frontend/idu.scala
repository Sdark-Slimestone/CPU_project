package R1322IAe

import chisel3._
import chisel3.util._

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
      }
      // 合并后的立即数
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
      }
      // 合并后的立即数
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

    // 调试端口
    val idu_debug = new Bundle {
      val debug_inst1 = Output(UInt(32.W))
      val debug_inst2 = Output(UInt(32.W))
      val is_stall    = Output(Bool())
    }
  })

  // 实例化译码器（使用简化后的 decoder）
  val dec1 = Module(new decoder)
  val dec2 = Module(new decoder)
  dec1.io.inst := io.ifu_to_idu.inst1
  dec2.io.inst := io.ifu_to_idu.inst2

  // ---------- 冒险检测 ----------
  val isControl1 = dec1.io.is_jal  || dec1.io.is_jalr ||
                   dec1.io.is_beq  || dec1.io.is_bne  ||
                   dec1.io.is_blt  || dec1.io.is_bge  ||
                   dec1.io.is_bltu || dec1.io.is_bgeu || dec1.io.is_ebreak
  val isControl2 = dec2.io.is_jal  || dec2.io.is_jalr ||
                   dec2.io.is_beq  || dec2.io.is_bne  ||
                   dec2.io.is_blt  || dec2.io.is_bge  ||
                   dec2.io.is_bltu || dec2.io.is_bgeu || dec2.io.is_ebreak

  val isStore1 = dec1.io.is_sb || dec1.io.is_sh || dec1.io.is_sw
  val isStore2 = dec2.io.is_sb || dec2.io.is_sh || dec2.io.is_sw
  val isLoad1  = dec1.io.is_lb || dec1.io.is_lbu || dec1.io.is_lh || dec1.io.is_lhu || dec1.io.is_lw
  val isLoad2  = dec2.io.is_lb || dec2.io.is_lbu || dec2.io.is_lh || dec2.io.is_lhu || dec2.io.is_lw
  
  //bank计算
  val load1imm_low = dec1.io.imm(0)
  val load2imm_low = dec2.io.imm(0)
  val load1rs1_low = io.grf_to_idu.dec1_value.inst1rs1_value (0)
  val load2rs1_low = io.grf_to_idu.dec2_value.inst2rs1_value (0)
  val load1addr_low = load1imm_low ^ load1rs1_low
  val load2addr_low = load2imm_low ^ load2rs1_low


  // RAW: 指令1写寄存器，且被指令2作为源操作数
  val raw = (dec1.io.rd =/= 0.U) &&
            ((dec1.io.rd === dec2.io.rs1) || (dec1.io.rd === dec2.io.rs2))

  // 内存冒险：store -> load 相同地址，或 store -> store
  // 注意：此时 dec1 必为 store（使用 S-type 立即数），dec2 必为 load（使用 I-type 立即数）
  // 由于 decoder 已根据指令类型输出正确的立即数，直接比较统一 imm 即可
  val ramraw = (isStore1 && isLoad2) &&
               (io.grf_to_idu.dec1_value.inst1rs1_value === io.grf_to_idu.dec2_value.inst2rs1_value) &&
               (dec1.io.imm === dec2.io.imm)
  val rambank_conflict = (isLoad1 && isLoad2) && (load1addr_low === load2addr_low)
  val ramwaw = isStore1 && isStore2

  val stall_sig = raw || isControl1 || ramraw || ramwaw || rambank_conflict
  io.idu_to_ifu.is_stall := stall_sig

  // ---------- 输出 rsaddr（给寄存器堆的读地址）----------
  io.idu_to_grf.dec1_redreg.rs1 := dec1.io.rs1
  io.idu_to_grf.dec1_redreg.rs2 := dec1.io.rs2
  io.idu_to_grf.dec2_redreg.rs1 := dec2.io.rs1
  io.idu_to_grf.dec2_redreg.rs2 := dec2.io.rs2

  // ---------- 第一条指令输出（不因 stall 清零）----------
  // dec1_op
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

  // dec1_imm（现在只有一个立即数）
  io.idu_to_exu1.dec1_imm := dec1.io.imm
  io.idu_to_exu1.dec1_rd  := dec1.io.rd

  // dec1_val：寄存器值和 nextpc
  io.idu_to_exu1.dec1_val.rs1_val := io.grf_to_idu.dec1_value.inst1rs1_value
  io.idu_to_exu1.dec1_val.rs2_val := io.grf_to_idu.dec1_value.inst1rs2_value
  io.idu_to_exu1.dec1_val.nextpc  := io.ifu_to_idu.inst1_nextpc

  // ---------- 第二条指令输出（stall 时全部清零）----------
  // dec2_op
  io.idu_to_exu2.dec2_op.is_lui    := Mux(stall_sig, false.B, dec2.io.is_lui)
  io.idu_to_exu2.dec2_op.is_auipc  := Mux(stall_sig, false.B, dec2.io.is_auipc)
  io.idu_to_exu2.dec2_op.is_jal    := Mux(stall_sig, false.B, dec2.io.is_jal)
  io.idu_to_exu2.dec2_op.is_jalr   := Mux(stall_sig, false.B, dec2.io.is_jalr)
  io.idu_to_exu2.dec2_op.is_beq    := Mux(stall_sig, false.B, dec2.io.is_beq)
  io.idu_to_exu2.dec2_op.is_bne    := Mux(stall_sig, false.B, dec2.io.is_bne)
  io.idu_to_exu2.dec2_op.is_blt    := Mux(stall_sig, false.B, dec2.io.is_blt)
  io.idu_to_exu2.dec2_op.is_bge    := Mux(stall_sig, false.B, dec2.io.is_bge)
  io.idu_to_exu2.dec2_op.is_bltu   := Mux(stall_sig, false.B, dec2.io.is_bltu)
  io.idu_to_exu2.dec2_op.is_bgeu   := Mux(stall_sig, false.B, dec2.io.is_bgeu)
  io.idu_to_exu2.dec2_op.is_lb     := Mux(stall_sig, false.B, dec2.io.is_lb)
  io.idu_to_exu2.dec2_op.is_lh     := Mux(stall_sig, false.B, dec2.io.is_lh)
  io.idu_to_exu2.dec2_op.is_lw     := Mux(stall_sig, false.B, dec2.io.is_lw)
  io.idu_to_exu2.dec2_op.is_lbu    := Mux(stall_sig, false.B, dec2.io.is_lbu)
  io.idu_to_exu2.dec2_op.is_lhu    := Mux(stall_sig, false.B, dec2.io.is_lhu)
  io.idu_to_exu2.dec2_op.is_sb     := Mux(stall_sig, false.B, dec2.io.is_sb)
  io.idu_to_exu2.dec2_op.is_sh     := Mux(stall_sig, false.B, dec2.io.is_sh)
  io.idu_to_exu2.dec2_op.is_sw     := Mux(stall_sig, false.B, dec2.io.is_sw)
  io.idu_to_exu2.dec2_op.is_addi   := Mux(stall_sig, false.B, dec2.io.is_addi)
  io.idu_to_exu2.dec2_op.is_slti   := Mux(stall_sig, false.B, dec2.io.is_slti)
  io.idu_to_exu2.dec2_op.is_sltiu  := Mux(stall_sig, false.B, dec2.io.is_sltiu)
  io.idu_to_exu2.dec2_op.is_xori   := Mux(stall_sig, false.B, dec2.io.is_xori)
  io.idu_to_exu2.dec2_op.is_ori    := Mux(stall_sig, false.B, dec2.io.is_ori)
  io.idu_to_exu2.dec2_op.is_andi   := Mux(stall_sig, false.B, dec2.io.is_andi)
  io.idu_to_exu2.dec2_op.is_slli   := Mux(stall_sig, false.B, dec2.io.is_slli)
  io.idu_to_exu2.dec2_op.is_srli   := Mux(stall_sig, false.B, dec2.io.is_srli)
  io.idu_to_exu2.dec2_op.is_srai   := Mux(stall_sig, false.B, dec2.io.is_srai)
  io.idu_to_exu2.dec2_op.is_add    := Mux(stall_sig, false.B, dec2.io.is_add)
  io.idu_to_exu2.dec2_op.is_sub    := Mux(stall_sig, false.B, dec2.io.is_sub)
  io.idu_to_exu2.dec2_op.is_sll    := Mux(stall_sig, false.B, dec2.io.is_sll)
  io.idu_to_exu2.dec2_op.is_slt    := Mux(stall_sig, false.B, dec2.io.is_slt)
  io.idu_to_exu2.dec2_op.is_sltu   := Mux(stall_sig, false.B, dec2.io.is_sltu)
  io.idu_to_exu2.dec2_op.is_xor    := Mux(stall_sig, false.B, dec2.io.is_xor)
  io.idu_to_exu2.dec2_op.is_srl    := Mux(stall_sig, false.B, dec2.io.is_srl)
  io.idu_to_exu2.dec2_op.is_sra    := Mux(stall_sig, false.B, dec2.io.is_sra)
  io.idu_to_exu2.dec2_op.is_or     := Mux(stall_sig, false.B, dec2.io.is_or)
  io.idu_to_exu2.dec2_op.is_and    := Mux(stall_sig, false.B, dec2.io.is_and)
  io.idu_to_exu2.dec2_op.is_ebreak := Mux(stall_sig, false.B, dec2.io.is_ebreak)

  // dec2_imm
  io.idu_to_exu2.dec2_imm := Mux(stall_sig, 0.U(32.W), dec2.io.imm)
  io.idu_to_exu2.dec2_rd  := Mux(stall_sig, 0.U(5.W), dec2.io.rd)

  // dec2_val：寄存器值和 nextpc（stall 时清零）
  io.idu_to_exu2.dec2_val.rs1_val := Mux(stall_sig, 0.U(32.W), io.grf_to_idu.dec2_value.inst2rs1_value)
  io.idu_to_exu2.dec2_val.rs2_val := Mux(stall_sig, 0.U(32.W), io.grf_to_idu.dec2_value.inst2rs2_value)
  io.idu_to_exu2.dec2_val.nextpc  := Mux(stall_sig, 0.U(32.W), io.ifu_to_idu.inst2_nextpc)

  // ---------- 调试输出 ----------
  io.idu_debug.debug_inst1 := dec1.io.debug_inst
  io.idu_debug.debug_inst2 := Mux(stall_sig, 0.U(32.W), dec2.io.debug_inst)
  io.idu_debug.is_stall    := stall_sig
}
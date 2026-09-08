package R1322IAeCSRMC

import chisel3._
import chisel3.util._

// 多周期 IDU: 既是 IFU 的 slave, 又是 EXU 的 master
// 接收到取指包后将其暂存于寄存器中, 下一周期完成译码/冒险检测/GRF读,
// 将译码结果消息发给 EXU; 单发射标志(is_stall)保持到下一包, 供 IFU 计算顺序地址
class idu extends Module {
  val io = IO(new Bundle {
    // 总线输入: 取指包 <- IFU
    val in = Flipped(Decoupled(new IFMsg))

    // 总线输出: 译码结果 -> EXU
    val out = Decoupled(new IDMsg)

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

    // 发往 IFU 的单发射标志(按包保持)
    val idu_to_ifu = new Bundle {
      val is_stall = Output(Bool())
    }

    // 调试端口
    val idu_debug = new Bundle {
      val debug_inst1    = Output(UInt(32.W))
      val debug_inst2    = Output(UInt(32.W))
      val debug_inst1_pc = Output(UInt(32.W))
      val debug_inst2_pc = Output(UInt(32.W))
      val is_stall       = Output(Bool())
    }
  })

  // ---------- 级寄存器: 暂存取指包 ----------
  val pkt  = RegEnable(io.in.bits, io.in.fire)
  val busy = RegInit(false.B)
  io.in.ready  := !busy
  io.out.valid := busy
  when (io.in.fire)  { busy := true.B  }
  when (io.out.fire) { busy := false.B }

  // ---------- 译码器 ----------
  val dec1 = Module(new decoder)
  val dec2 = Module(new decoder)
  dec1.io.inst := pkt.inst1
  dec2.io.inst := pkt.inst2

  // ---------- 冒险检测(与单周期版本一致) ----------
  // 控制指令（含 CSR）强制单发射
  val isControl1 = dec1.io.is_jal  || dec1.io.is_jalr ||
                   dec1.io.is_beq  || dec1.io.is_bne  ||
                   dec1.io.is_blt  || dec1.io.is_bge  ||
                   dec1.io.is_bltu || dec1.io.is_bgeu ||
                   dec1.io.is_ebreak ||
                   dec1.io.is_csrrw || dec1.io.is_csrrs || dec1.io.is_csrrc ||
                   dec1.io.is_csrrwi || dec1.io.is_csrrsi || dec1.io.is_csrrci ||
                   dec1.io.is_ecall || dec1.io.is_mret

  val isControl2 = dec2.io.is_jal  || dec2.io.is_jalr ||
                   dec2.io.is_beq  || dec2.io.is_bne  ||
                   dec2.io.is_blt  || dec2.io.is_bge  ||
                   dec2.io.is_bltu || dec2.io.is_bgeu ||
                   dec2.io.is_ebreak ||
                   dec2.io.is_csrrw || dec2.io.is_csrrs || dec2.io.is_csrrc ||
                   dec2.io.is_csrrwi || dec2.io.is_csrrsi || dec2.io.is_csrrci ||
                   dec2.io.is_ecall || dec2.io.is_mret

  val isStore1 = dec1.io.is_sb || dec1.io.is_sh || dec1.io.is_sw
  val isStore2 = dec2.io.is_sb || dec2.io.is_sh || dec2.io.is_sw
  val isLoad1  = dec1.io.is_lb || dec1.io.is_lbu || dec1.io.is_lh || dec1.io.is_lhu || dec1.io.is_lw
  val isLoad2  = dec2.io.is_lb || dec2.io.is_lbu || dec2.io.is_lh || dec2.io.is_lhu || dec2.io.is_lw

  // bank计算
  val load1imm_low = dec1.io.imm(0)
  val load2imm_low = dec2.io.imm(0)
  val load1rs1_low = io.grf_to_idu.dec1_value.inst1rs1_value(0)
  val load2rs1_low = io.grf_to_idu.dec2_value.inst2rs1_value(0)
  val load1addr_low = load1imm_low ^ load1rs1_low
  val load2addr_low = load2imm_low ^ load2rs1_low

  // RAW: 指令1写寄存器，且被指令2作为源操作数
  val raw = (dec1.io.rd =/= 0.U) &&
            ((dec1.io.rd === dec2.io.rs1) || (dec1.io.rd === dec2.io.rs2))

  // 内存冒险
  val ramraw = (isStore1 && isLoad2) &&
               (io.grf_to_idu.dec1_value.inst1rs1_value === io.grf_to_idu.dec2_value.inst2rs1_value) &&
               (dec1.io.imm === dec2.io.imm)
  val rambank_conflict = (isLoad1 && isLoad2) && (load1addr_low === load2addr_low)
  val ramwaw = isStore1 && isStore2

  // stall 条件
  val stall_sig = raw || isControl1 || ramraw || ramwaw || rambank_conflict

  // 当 inst2 是控制指令但 inst1 不是时也 stall（inst2 不能单独跳转）
  val stall_sig2 = !isControl1 && isControl2
  val final_stall = stall_sig || stall_sig2

  // 单发射标志按包保持: 握手发出译码结果时锁存, 直到下一包发出前不变,
  // IFU 在 WBU 完成时读取它来决定下一包从 pc+4 还是 pc+8 取起
  val singleIssue = RegInit(false.B)
  when (io.out.fire) { singleIssue := final_stall }
  io.idu_to_ifu.is_stall := singleIssue

  // ---------- GRF 读地址 ----------
  io.idu_to_grf.dec1_redreg.rs1 := dec1.io.rs1
  io.idu_to_grf.dec1_redreg.rs2 := dec1.io.rs2
  io.idu_to_grf.dec2_redreg.rs1 := dec2.io.rs1
  io.idu_to_grf.dec2_redreg.rs2 := dec2.io.rs2

  // ---------- 第一条指令输出 ----------
  io.out.bits.dec1_op.is_lui    := dec1.io.is_lui
  io.out.bits.dec1_op.is_auipc  := dec1.io.is_auipc
  io.out.bits.dec1_op.is_jal    := dec1.io.is_jal
  io.out.bits.dec1_op.is_jalr   := dec1.io.is_jalr
  io.out.bits.dec1_op.is_beq    := dec1.io.is_beq
  io.out.bits.dec1_op.is_bne    := dec1.io.is_bne
  io.out.bits.dec1_op.is_blt    := dec1.io.is_blt
  io.out.bits.dec1_op.is_bge    := dec1.io.is_bge
  io.out.bits.dec1_op.is_bltu   := dec1.io.is_bltu
  io.out.bits.dec1_op.is_bgeu   := dec1.io.is_bgeu
  io.out.bits.dec1_op.is_lb     := dec1.io.is_lb
  io.out.bits.dec1_op.is_lh     := dec1.io.is_lh
  io.out.bits.dec1_op.is_lw     := dec1.io.is_lw
  io.out.bits.dec1_op.is_lbu    := dec1.io.is_lbu
  io.out.bits.dec1_op.is_lhu    := dec1.io.is_lhu
  io.out.bits.dec1_op.is_sb     := dec1.io.is_sb
  io.out.bits.dec1_op.is_sh     := dec1.io.is_sh
  io.out.bits.dec1_op.is_sw     := dec1.io.is_sw
  io.out.bits.dec1_op.is_addi   := dec1.io.is_addi
  io.out.bits.dec1_op.is_slti   := dec1.io.is_slti
  io.out.bits.dec1_op.is_sltiu  := dec1.io.is_sltiu
  io.out.bits.dec1_op.is_xori   := dec1.io.is_xori
  io.out.bits.dec1_op.is_ori    := dec1.io.is_ori
  io.out.bits.dec1_op.is_andi   := dec1.io.is_andi
  io.out.bits.dec1_op.is_slli   := dec1.io.is_slli
  io.out.bits.dec1_op.is_srli   := dec1.io.is_srli
  io.out.bits.dec1_op.is_srai   := dec1.io.is_srai
  io.out.bits.dec1_op.is_add    := dec1.io.is_add
  io.out.bits.dec1_op.is_sub    := dec1.io.is_sub
  io.out.bits.dec1_op.is_sll    := dec1.io.is_sll
  io.out.bits.dec1_op.is_slt    := dec1.io.is_slt
  io.out.bits.dec1_op.is_sltu   := dec1.io.is_sltu
  io.out.bits.dec1_op.is_xor    := dec1.io.is_xor
  io.out.bits.dec1_op.is_srl    := dec1.io.is_srl
  io.out.bits.dec1_op.is_sra    := dec1.io.is_sra
  io.out.bits.dec1_op.is_or     := dec1.io.is_or
  io.out.bits.dec1_op.is_and    := dec1.io.is_and
  io.out.bits.dec1_op.is_ebreak := dec1.io.is_ebreak
  io.out.bits.dec1_op.is_csrrw  := dec1.io.is_csrrw
  io.out.bits.dec1_op.is_csrrs  := dec1.io.is_csrrs
  io.out.bits.dec1_op.is_csrrc  := dec1.io.is_csrrc
  io.out.bits.dec1_op.is_csrrwi := dec1.io.is_csrrwi
  io.out.bits.dec1_op.is_csrrsi := dec1.io.is_csrrsi
  io.out.bits.dec1_op.is_csrrci := dec1.io.is_csrrci
  io.out.bits.dec1_op.is_ecall  := dec1.io.is_ecall
  io.out.bits.dec1_op.is_mret   := dec1.io.is_mret

  io.out.bits.dec1_imm     := dec1.io.imm
  io.out.bits.dec1_rd      := dec1.io.rd
  io.out.bits.dec1_rs1_val := io.grf_to_idu.dec1_value.inst1rs1_value
  io.out.bits.dec1_rs2_val := io.grf_to_idu.dec1_value.inst1rs2_value
  io.out.bits.dec1_nextpc  := pkt.inst1_nextpc

  // ---------- 第二条指令输出（单发射时全部清零） ----------
  val dec2op = Wire(new OpSig)
  dec2op.is_lui    := dec2.io.is_lui
  dec2op.is_auipc  := dec2.io.is_auipc
  dec2op.is_jal    := dec2.io.is_jal
  dec2op.is_jalr   := dec2.io.is_jalr
  dec2op.is_beq    := dec2.io.is_beq
  dec2op.is_bne    := dec2.io.is_bne
  dec2op.is_blt    := dec2.io.is_blt
  dec2op.is_bge    := dec2.io.is_bge
  dec2op.is_bltu   := dec2.io.is_bltu
  dec2op.is_bgeu   := dec2.io.is_bgeu
  dec2op.is_lb     := dec2.io.is_lb
  dec2op.is_lh     := dec2.io.is_lh
  dec2op.is_lw     := dec2.io.is_lw
  dec2op.is_lbu    := dec2.io.is_lbu
  dec2op.is_lhu    := dec2.io.is_lhu
  dec2op.is_sb     := dec2.io.is_sb
  dec2op.is_sh     := dec2.io.is_sh
  dec2op.is_sw     := dec2.io.is_sw
  dec2op.is_addi   := dec2.io.is_addi
  dec2op.is_slti   := dec2.io.is_slti
  dec2op.is_sltiu  := dec2.io.is_sltiu
  dec2op.is_xori   := dec2.io.is_xori
  dec2op.is_ori    := dec2.io.is_ori
  dec2op.is_andi   := dec2.io.is_andi
  dec2op.is_slli   := dec2.io.is_slli
  dec2op.is_srli   := dec2.io.is_srli
  dec2op.is_srai   := dec2.io.is_srai
  dec2op.is_add    := dec2.io.is_add
  dec2op.is_sub    := dec2.io.is_sub
  dec2op.is_sll    := dec2.io.is_sll
  dec2op.is_slt    := dec2.io.is_slt
  dec2op.is_sltu   := dec2.io.is_sltu
  dec2op.is_xor    := dec2.io.is_xor
  dec2op.is_srl    := dec2.io.is_srl
  dec2op.is_sra    := dec2.io.is_sra
  dec2op.is_or     := dec2.io.is_or
  dec2op.is_and    := dec2.io.is_and
  dec2op.is_ebreak := dec2.io.is_ebreak
  dec2op.is_csrrw  := dec2.io.is_csrrw
  dec2op.is_csrrs  := dec2.io.is_csrrs
  dec2op.is_csrrc  := dec2.io.is_csrrc
  dec2op.is_csrrwi := dec2.io.is_csrrwi
  dec2op.is_csrrsi := dec2.io.is_csrrsi
  dec2op.is_csrrci := dec2.io.is_csrrci
  dec2op.is_ecall  := dec2.io.is_ecall
  dec2op.is_mret   := dec2.io.is_mret

  io.out.bits.dec2_op      := Mux(final_stall, 0.U.asTypeOf(new OpSig), dec2op)
  io.out.bits.dec2_imm     := Mux(final_stall, 0.U(32.W), dec2.io.imm)
  io.out.bits.dec2_rd      := Mux(final_stall, 0.U(5.W), dec2.io.rd)
  io.out.bits.dec2_rs1_val := Mux(final_stall, 0.U(32.W), io.grf_to_idu.dec2_value.inst2rs1_value)
  io.out.bits.dec2_rs2_val := Mux(final_stall, 0.U(32.W), io.grf_to_idu.dec2_value.inst2rs2_value)
  io.out.bits.dec2_nextpc  := Mux(final_stall, 0.U(32.W), pkt.inst2_nextpc)

  // ---------- CSR 顶传信号 ----------
  io.out.bits.is_stall  := final_stall
  io.out.bits.inst1_pc  := pkt.inst1_pc
  io.out.bits.inst1     := pkt.inst1

  // ---------- 调试输出(显示当前在飞行的取指包) ----------
  io.idu_debug.debug_inst1    := pkt.inst1
  io.idu_debug.debug_inst2    := Mux(singleIssue, 0.U(32.W), pkt.inst2)
  io.idu_debug.debug_inst1_pc := pkt.inst1_pc
  io.idu_debug.debug_inst2_pc := pkt.inst2_pc
  io.idu_debug.is_stall       := singleIssue
}

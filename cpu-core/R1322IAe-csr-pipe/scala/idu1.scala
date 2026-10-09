package R1322IAeCSR

import chisel3._
import chisel3.util._

class idu1 extends Module {
  val io = IO(new Bundle {
    // 来自取指队列(FetchQueue)队头
    //==========================================接口==============================================
    val from_fq = new Bundle {
      val inst1        = Input(UInt(32.W))
      val inst2        = Input(UInt(32.W))
      val inst1_pc     = Input(UInt(32.W))
      val inst2_pc     = Input(UInt(32.W))
      val inst1_nextpc = Input(UInt(32.W))
      val inst2_nextpc = Input(UInt(32.W))
      val valid        = Input(Bool())
    }
    //==========================================接口==============================================

    //====================================新增: IDU1 只译码, GRF 读移到 IDU2====================================
    // (GRF 读/转发/冒险 都在 idu2.scala, 这样读距 EXU 只差一级, 紧邻依赖可 0 阻塞)

    /* 原端口保留
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
      // 合并后的立即数
      val dec1_imm = Output(UInt(32.W))
      val dec1_val = new Bundle {
        val rs1_val = Output(UInt(32.W))
        val rs2_val = Output(UInt(32.W))
        val nextpc  = Output(UInt(32.W))
      }
      val dec1_rd   = Output(UInt(5.W))
      // CSR 信号
      val is_stall  = Output(Bool())
      val inst1_pc  = Output(UInt(32.W))
      val inst1     = Output(UInt(32.W))
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
      // 合并后的立即数
      val dec2_imm = Output(UInt(32.W))
      val dec2_val = new Bundle {
        val rs1_val = Output(UInt(32.W))
        val rs2_val = Output(UInt(32.W))
        val nextpc  = Output(UInt(32.W))
      }
      val dec2_rd = Output(UInt(5.W))
    }
    原端口保留结束 */

    //====================================新增: IDU1 -> IDU2 单lane总线端口====================================
    // IDU1 只负责译码, 把 op/imm/rs 地址/rd/pc 等打包成消息交给 IDU2 去读 GRF + 冒险/转发
    val idu1_to_idu2 = Decoupled(new IDU1_to_IDU2_Message)
    //====================================新增: IDU1 -> IDU2 单lane总线端口====================================

    // 调试端口
    val idu_debug = new Bundle {
      val debug_inst1 = Output(UInt(32.W))
      val debug_inst2 = Output(UInt(32.W))
    }
  })

  // 实例化译码器（使用含CSR的 decoder）
  val dec1 = Module(new decoder)
  val dec2 = Module(new decoder)
  //================================IFU输入.bits=============================================
  dec1.io.inst := io.from_fq.inst1
  dec2.io.inst := io.from_fq.inst2
  // 解耦前端: 弹出条数由 top 根据 IDU2 的冲突判决控制, IDU1 无 ready
  //================================IFU输入.bits=============================================


  // ---------- 冒险检测 ----------
  // 控制指令（含 CSR）强制单发射
  /* 原 isControl1 由 18 条一热译码信号 OR 得到, 组合链过深; 改用 opcode 级粗格式(跳转/分支/SYSTEM)
  val isControl1 = dec1.io.is_jal  || dec1.io.is_jalr ||
                   dec1.io.is_beq  || dec1.io.is_bne  ||
                   dec1.io.is_blt  || dec1.io.is_bge  ||
                   dec1.io.is_bltu || dec1.io.is_bgeu ||
                   dec1.io.is_ebreak ||
                   dec1.io.is_csrrw || dec1.io.is_csrrs || dec1.io.is_csrrc ||
                   dec1.io.is_csrrwi || dec1.io.is_csrrsi || dec1.io.is_csrrci ||
                   dec1.io.is_ecall || dec1.io.is_mret
  原 isControl1 保留结束 */
  val isControl1 = dec1.io.is_jump || dec1.io.is_branch || dec1.io.is_system

  /* 原 isControl2 由 18 条一热译码信号 OR 得到, 组合链过深
  val isControl2 = dec2.io.is_jal  || dec2.io.is_jalr ||
                   dec2.io.is_beq  || dec2.io.is_bne  ||
                   dec2.io.is_blt  || dec2.io.is_bge  ||
                   dec2.io.is_bltu || dec2.io.is_bgeu ||
                   dec2.io.is_ebreak ||
                   dec2.io.is_csrrw || dec2.io.is_csrrs || dec2.io.is_csrrc ||
                   dec2.io.is_csrrwi || dec2.io.is_csrrsi || dec2.io.is_csrrci ||
                   dec2.io.is_ecall || dec2.io.is_mret
  原 isControl2 保留结束 */
  val isControl2 = dec2.io.is_jump || dec2.io.is_branch || dec2.io.is_system

  // 访存类型直接取 opcode 级粗格式
  val isStore1 = dec1.io.is_store
  val isStore2 = dec2.io.is_store
  val isLoad1  = dec1.io.is_load
  val isLoad2  = dec2.io.is_load

  // RAW: 指令1写寄存器，且被指令2作为源操作数
  // (IDU1 只算不含 GRF 数据的这一项; 内存冒险 ramraw/rambank 需要在 IDU2 用锁存好的 GRF 值比较)
  val raw = (dec1.io.rd =/= 0.U) &&
            ((dec1.io.rd === dec2.io.rs1) || (dec1.io.rd === dec2.io.rs2))

  // (跨包 RAW 转发/阻塞 已随 GRF 读一起移到 idu2.scala)

  /* 原打包保留
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
  //================================IFU输入.bits=============================================
  io.idu_to_exu1.dec1_val.nextpc  := io.from_fq.inst1_nextpc
  //================================IFU输入.bits=============================================

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
  //================================IFU输入.bits=============================================
  io.idu_to_exu2.dec2_val.nextpc  := Mux(final_stall, 0.U(32.W), io.from_fq.inst2_nextpc)
  //================================IFU输入.bits=============================================

  // ---------- CSR 顶传信号（放入 idu_to_exu1） ----------
  io.idu_to_exu1.is_stall := io.idu_to_ifu.is_stall
  //================================IFU输入.bits=============================================
  io.idu_to_exu1.inst1_pc := io.from_fq.inst1_pc
  io.idu_to_exu1.inst1    := io.from_fq.inst1
  //================================IFU输入.bits=============================================
  原打包保留结束 */

  //====================================新增: IDU1 -> IDU2 单lane打包====================================
  // ---- lane1 ----
  io.idu1_to_idu2.bits.lane1.op.is_lui    := dec1.io.is_lui
  io.idu1_to_idu2.bits.lane1.op.is_auipc  := dec1.io.is_auipc
  io.idu1_to_idu2.bits.lane1.op.is_jal    := dec1.io.is_jal
  io.idu1_to_idu2.bits.lane1.op.is_jalr   := dec1.io.is_jalr
  io.idu1_to_idu2.bits.lane1.op.is_beq    := dec1.io.is_beq
  io.idu1_to_idu2.bits.lane1.op.is_bne    := dec1.io.is_bne
  io.idu1_to_idu2.bits.lane1.op.is_blt    := dec1.io.is_blt
  io.idu1_to_idu2.bits.lane1.op.is_bge    := dec1.io.is_bge
  io.idu1_to_idu2.bits.lane1.op.is_bltu   := dec1.io.is_bltu
  io.idu1_to_idu2.bits.lane1.op.is_bgeu   := dec1.io.is_bgeu
  io.idu1_to_idu2.bits.lane1.op.is_lb     := dec1.io.is_lb
  io.idu1_to_idu2.bits.lane1.op.is_lh     := dec1.io.is_lh
  io.idu1_to_idu2.bits.lane1.op.is_lw     := dec1.io.is_lw
  io.idu1_to_idu2.bits.lane1.op.is_lbu    := dec1.io.is_lbu
  io.idu1_to_idu2.bits.lane1.op.is_lhu    := dec1.io.is_lhu
  io.idu1_to_idu2.bits.lane1.op.is_sb     := dec1.io.is_sb
  io.idu1_to_idu2.bits.lane1.op.is_sh     := dec1.io.is_sh
  io.idu1_to_idu2.bits.lane1.op.is_sw     := dec1.io.is_sw
  io.idu1_to_idu2.bits.lane1.op.is_addi   := dec1.io.is_addi
  io.idu1_to_idu2.bits.lane1.op.is_slti   := dec1.io.is_slti
  io.idu1_to_idu2.bits.lane1.op.is_sltiu  := dec1.io.is_sltiu
  io.idu1_to_idu2.bits.lane1.op.is_xori   := dec1.io.is_xori
  io.idu1_to_idu2.bits.lane1.op.is_ori    := dec1.io.is_ori
  io.idu1_to_idu2.bits.lane1.op.is_andi   := dec1.io.is_andi
  io.idu1_to_idu2.bits.lane1.op.is_slli   := dec1.io.is_slli
  io.idu1_to_idu2.bits.lane1.op.is_srli   := dec1.io.is_srli
  io.idu1_to_idu2.bits.lane1.op.is_srai   := dec1.io.is_srai
  io.idu1_to_idu2.bits.lane1.op.is_add    := dec1.io.is_add
  io.idu1_to_idu2.bits.lane1.op.is_sub    := dec1.io.is_sub
  io.idu1_to_idu2.bits.lane1.op.is_sll    := dec1.io.is_sll
  io.idu1_to_idu2.bits.lane1.op.is_slt    := dec1.io.is_slt
  io.idu1_to_idu2.bits.lane1.op.is_sltu   := dec1.io.is_sltu
  io.idu1_to_idu2.bits.lane1.op.is_xor    := dec1.io.is_xor
  io.idu1_to_idu2.bits.lane1.op.is_srl    := dec1.io.is_srl
  io.idu1_to_idu2.bits.lane1.op.is_sra    := dec1.io.is_sra
  io.idu1_to_idu2.bits.lane1.op.is_or     := dec1.io.is_or
  io.idu1_to_idu2.bits.lane1.op.is_and    := dec1.io.is_and
  io.idu1_to_idu2.bits.lane1.op.is_ebreak := dec1.io.is_ebreak
  io.idu1_to_idu2.bits.lane1.op.is_csrrw  := dec1.io.is_csrrw
  io.idu1_to_idu2.bits.lane1.op.is_csrrs  := dec1.io.is_csrrs
  io.idu1_to_idu2.bits.lane1.op.is_csrrc  := dec1.io.is_csrrc
  io.idu1_to_idu2.bits.lane1.op.is_csrrwi := dec1.io.is_csrrwi
  io.idu1_to_idu2.bits.lane1.op.is_csrrsi := dec1.io.is_csrrsi
  io.idu1_to_idu2.bits.lane1.op.is_csrrci := dec1.io.is_csrrci
  io.idu1_to_idu2.bits.lane1.op.is_ecall  := dec1.io.is_ecall
  io.idu1_to_idu2.bits.lane1.op.is_mret   := dec1.io.is_mret
  io.idu1_to_idu2.bits.lane1.imm      := dec1.io.imm
  io.idu1_to_idu2.bits.lane1.rs1_addr := dec1.io.rs1
  io.idu1_to_idu2.bits.lane1.rs2_addr := dec1.io.rs2
  io.idu1_to_idu2.bits.lane1.rs1_val  := 0.U
  io.idu1_to_idu2.bits.lane1.rs2_val  := 0.U
  io.idu1_to_idu2.bits.lane1.pc       := io.from_fq.inst1_pc
  io.idu1_to_idu2.bits.lane1.rd       := dec1.io.rd
  io.idu1_to_idu2.bits.lane1.is_stall := false.B   // 真正的 final_stall 在 IDU2 里覆盖
  io.idu1_to_idu2.bits.lane1.inst1_pc := io.from_fq.inst1_pc
  io.idu1_to_idu2.bits.lane1.inst1    := io.from_fq.inst1
  //====================================新增: IDU1 -> IDU2 单lane打包====================================

  // ---- lane2 ----
  // IDU1 里 lane2 直接输出真实译码结果; stall 清零统一放到 IDU2(idu2) 做,
  // 这样 IDU1 只承担"译码 + GRF 读", IDU2 只承担"比较 + 选通", 两级各自变短
  io.idu1_to_idu2.bits.lane2.op.is_lui    := dec2.io.is_lui
  io.idu1_to_idu2.bits.lane2.op.is_auipc  := dec2.io.is_auipc
  io.idu1_to_idu2.bits.lane2.op.is_jal    := dec2.io.is_jal
  io.idu1_to_idu2.bits.lane2.op.is_jalr   := dec2.io.is_jalr
  io.idu1_to_idu2.bits.lane2.op.is_beq    := dec2.io.is_beq
  io.idu1_to_idu2.bits.lane2.op.is_bne    := dec2.io.is_bne
  io.idu1_to_idu2.bits.lane2.op.is_blt    := dec2.io.is_blt
  io.idu1_to_idu2.bits.lane2.op.is_bge    := dec2.io.is_bge
  io.idu1_to_idu2.bits.lane2.op.is_bltu   := dec2.io.is_bltu
  io.idu1_to_idu2.bits.lane2.op.is_bgeu   := dec2.io.is_bgeu
  io.idu1_to_idu2.bits.lane2.op.is_lb     := dec2.io.is_lb
  io.idu1_to_idu2.bits.lane2.op.is_lh     := dec2.io.is_lh
  io.idu1_to_idu2.bits.lane2.op.is_lw     := dec2.io.is_lw
  io.idu1_to_idu2.bits.lane2.op.is_lbu    := dec2.io.is_lbu
  io.idu1_to_idu2.bits.lane2.op.is_lhu    := dec2.io.is_lhu
  io.idu1_to_idu2.bits.lane2.op.is_sb     := dec2.io.is_sb
  io.idu1_to_idu2.bits.lane2.op.is_sh     := dec2.io.is_sh
  io.idu1_to_idu2.bits.lane2.op.is_sw     := dec2.io.is_sw
  io.idu1_to_idu2.bits.lane2.op.is_addi   := dec2.io.is_addi
  io.idu1_to_idu2.bits.lane2.op.is_slti   := dec2.io.is_slti
  io.idu1_to_idu2.bits.lane2.op.is_sltiu  := dec2.io.is_sltiu
  io.idu1_to_idu2.bits.lane2.op.is_xori   := dec2.io.is_xori
  io.idu1_to_idu2.bits.lane2.op.is_ori    := dec2.io.is_ori
  io.idu1_to_idu2.bits.lane2.op.is_andi   := dec2.io.is_andi
  io.idu1_to_idu2.bits.lane2.op.is_slli   := dec2.io.is_slli
  io.idu1_to_idu2.bits.lane2.op.is_srli   := dec2.io.is_srli
  io.idu1_to_idu2.bits.lane2.op.is_srai   := dec2.io.is_srai
  io.idu1_to_idu2.bits.lane2.op.is_add    := dec2.io.is_add
  io.idu1_to_idu2.bits.lane2.op.is_sub    := dec2.io.is_sub
  io.idu1_to_idu2.bits.lane2.op.is_sll    := dec2.io.is_sll
  io.idu1_to_idu2.bits.lane2.op.is_slt    := dec2.io.is_slt
  io.idu1_to_idu2.bits.lane2.op.is_sltu   := dec2.io.is_sltu
  io.idu1_to_idu2.bits.lane2.op.is_xor    := dec2.io.is_xor
  io.idu1_to_idu2.bits.lane2.op.is_srl    := dec2.io.is_srl
  io.idu1_to_idu2.bits.lane2.op.is_sra    := dec2.io.is_sra
  io.idu1_to_idu2.bits.lane2.op.is_or     := dec2.io.is_or
  io.idu1_to_idu2.bits.lane2.op.is_and    := dec2.io.is_and
  io.idu1_to_idu2.bits.lane2.op.is_ebreak := dec2.io.is_ebreak
  io.idu1_to_idu2.bits.lane2.op.is_csrrw  := dec2.io.is_csrrw
  io.idu1_to_idu2.bits.lane2.op.is_csrrs  := dec2.io.is_csrrs
  io.idu1_to_idu2.bits.lane2.op.is_csrrc  := dec2.io.is_csrrc
  io.idu1_to_idu2.bits.lane2.op.is_csrrwi := dec2.io.is_csrrwi
  io.idu1_to_idu2.bits.lane2.op.is_csrrsi := dec2.io.is_csrrsi
  io.idu1_to_idu2.bits.lane2.op.is_csrrci := dec2.io.is_csrrci
  io.idu1_to_idu2.bits.lane2.op.is_ecall  := dec2.io.is_ecall
  io.idu1_to_idu2.bits.lane2.op.is_mret   := dec2.io.is_mret
  io.idu1_to_idu2.bits.lane2.imm      := dec2.io.imm
  io.idu1_to_idu2.bits.lane2.rs1_addr := dec2.io.rs1
  io.idu1_to_idu2.bits.lane2.rs2_addr := dec2.io.rs2
  io.idu1_to_idu2.bits.lane2.rs1_val  := 0.U
  io.idu1_to_idu2.bits.lane2.rs2_val  := 0.U
  io.idu1_to_idu2.bits.lane2.pc       := io.from_fq.inst2_pc
  io.idu1_to_idu2.bits.lane2.rd       := dec2.io.rd
  io.idu1_to_idu2.bits.lane2.is_stall := false.B
  io.idu1_to_idu2.bits.lane2.inst1_pc := 0.U
  io.idu1_to_idu2.bits.lane2.inst1    := 0.U

  // 冒险检测辅助信号(IDU1 预先算好, 供 IDU2 直接使用)
  io.idu1_to_idu2.bits.is_control1 := isControl1
  io.idu1_to_idu2.bits.is_control2 := isControl2
  io.idu1_to_idu2.bits.is_store1   := isStore1
  io.idu1_to_idu2.bits.is_store2   := isStore2
  io.idu1_to_idu2.bits.is_load1    := isLoad1
  io.idu1_to_idu2.bits.is_load2    := isLoad2
  io.idu1_to_idu2.bits.raw         := raw
  io.idu1_to_idu2.bits.debug_inst2 := dec2.io.debug_inst
  //====================================新增: IDU1 -> IDU2 单lane打包====================================

  //====================================新增: valid 随输入(否则会把气泡传下去)====================================
  io.idu1_to_idu2.valid         := io.from_fq.valid
  //====================================新增: valid 随输入====================================

  // ---------- 调试输出 ----------
  io.idu_debug.debug_inst1 := dec1.io.debug_inst
  io.idu_debug.debug_inst2 := dec2.io.debug_inst
}
package R1322IAeCSRMC

import chisel3._
import chisel3.util._

// ========== 总线消息定义 ==========
// 指令识别信号集合（与 decoder 的输出一一对应）
class OpSig extends Bundle {
  val is_lui    = Bool()
  val is_auipc  = Bool()
  val is_jal    = Bool()
  val is_jalr   = Bool()
  val is_beq    = Bool()
  val is_bne    = Bool()
  val is_blt    = Bool()
  val is_bge    = Bool()
  val is_bltu   = Bool()
  val is_bgeu   = Bool()
  val is_lb     = Bool()
  val is_lh     = Bool()
  val is_lw     = Bool()
  val is_lbu    = Bool()
  val is_lhu    = Bool()
  val is_sb     = Bool()
  val is_sh     = Bool()
  val is_sw     = Bool()
  val is_addi   = Bool()
  val is_slti   = Bool()
  val is_sltiu  = Bool()
  val is_xori   = Bool()
  val is_ori    = Bool()
  val is_andi   = Bool()
  val is_slli   = Bool()
  val is_srli   = Bool()
  val is_srai   = Bool()
  val is_add    = Bool()
  val is_sub    = Bool()
  val is_sll    = Bool()
  val is_slt    = Bool()
  val is_sltu   = Bool()
  val is_xor    = Bool()
  val is_srl    = Bool()
  val is_sra    = Bool()
  val is_or     = Bool()
  val is_and    = Bool()
  val is_ebreak = Bool()
  // CSR 信号
  val is_csrrw  = Bool()
  val is_csrrs  = Bool()
  val is_csrrc  = Bool()
  val is_csrrwi = Bool()
  val is_csrrsi = Bool()
  val is_csrrci = Bool()
  val is_ecall  = Bool()
  val is_mret   = Bool()
}

// IFU -> IDU 的取指包消息（双发射：一次传递两条指令）
class IFMsg extends Bundle {
  val inst1         = UInt(32.W)
  val inst2         = UInt(32.W)
  val inst1_pc      = UInt(32.W)
  val inst2_pc      = UInt(32.W)
  val inst1_nextpc  = UInt(32.W)
  val inst2_nextpc  = UInt(32.W)
}

// IDU -> EXU 的译码结果消息（两条指令的译码信息 + GRF 读出的操作数）
class IDMsg extends Bundle {
  val dec1_op      = new OpSig
  val dec1_imm     = UInt(32.W)
  val dec1_rs1_val = UInt(32.W)
  val dec1_rs2_val = UInt(32.W)
  val dec1_nextpc  = UInt(32.W)
  val dec1_rd      = UInt(5.W)

  val dec2_op      = new OpSig
  val dec2_imm     = UInt(32.W)
  val dec2_rs1_val = UInt(32.W)
  val dec2_rs2_val = UInt(32.W)
  val dec2_nextpc  = UInt(32.W)
  val dec2_rd      = UInt(5.W)

  // 本取指包是否只发射了第一条指令（冲突时单发射）
  val is_stall = Bool()
  // CSR/异常处理所需的原始信息（只对 inst1 有效）
  val inst1_pc = UInt(32.W)
  val inst1    = UInt(32.W)
}

// 访存操作信号集合
class MemOp extends Bundle {
  val is_lb  = Bool()
  val is_lh  = Bool()
  val is_lw  = Bool()
  val is_lbu = Bool()
  val is_lhu = Bool()
  val is_sb  = Bool()
  val is_sh  = Bool()
  val is_sw  = Bool()
}

// EXU -> LSU 的执行结果消息（双 lane：运算结果 + 访存请求）
class EXMsg extends Bundle {
  // lane1: 第一条指令
  val op1          = new MemOp
  val addr1        = UInt(32.W)
  val store_data1  = UInt(32.W)
  val rd1          = UInt(5.W)
  val wb_data1     = UInt(32.W)

  // lane2: 第二条指令（单发射时全部清零）
  val op2          = new MemOp
  val addr2        = UInt(32.W)
  val store_data2  = UInt(32.W)
  val rd2          = UInt(5.W)
  val wb_data2     = UInt(32.W)

  // ebreak 恒在 lane1（控制类指令强制单发射）
  val is_ebreak = Bool()
}

// LSU -> WBU 的写回消息
class WBMsg extends Bundle {
  val rd1      = UInt(5.W)
  val wb_data1 = UInt(32.W)
  val rd2      = UInt(5.W)
  val wb_data2 = UInt(32.W)
}

// ========== 级间连接抽象 ==========
// 通过修改 arch 即可在不同微结构之间切换:
//   single   - 单周期: 消息组合逻辑直通, 不使用握手
//   multi    - 多周期(分布式): 直接握手, 模块内部实现通信状态机, 一次在飞行中一个消息
//   pipeline - 流水线: 级间插入寄存器, 每周期尝试向下游传递
//   ooo      - 乱序: 级间插入队列, 上游只要队列不满即可继续工作
object StageConnect {
  final val arch = "multi"

  def apply[T <: Data](left: DecoupledIO[T], right: DecoupledIO[T]): Unit = {
    if      (arch == "single") {
      right.bits := left.bits
      right.valid := true.B
      left.ready  := true.B
    }
    else if (arch == "multi") {
      right <> left
    }
    else if (arch == "pipeline") {
      val bitsReg = RegEnable(left.bits, left.fire)
      val validReg = RegNext(left.fire, false.B)
      right.bits  := bitsReg
      right.valid := validReg
      left.ready  := right.ready
    }
    else if (arch == "ooo") {
      right <> Queue(left, 16)
    }
    else {
      throw new IllegalArgumentException(s"unknown arch: $arch")
    }
  }
}

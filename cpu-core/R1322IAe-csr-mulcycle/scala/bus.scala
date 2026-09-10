package R1322IAeCSR

import chisel3._
import chisel3.util._

// ==================================================================
// 【多周期新增区】总线层：级间消息定义 + 总线桥
//  - core/ 目录下是 R1322IAe-csr 原版模块的逐字节镜像, 本文件开始的
//    所有文件均为多周期新增逻辑
//  - 原版模块在 top 中直接连线(与原版单周期 top 同构), 本部件
//    "总线桥 BusBridge" 插在原版组合部件之间, 构成五级流水站:
//      core/ifu(pcReg) -> [F/D桥] -> core/idu -> [D/E桥] -> core/exu
//                      -> [E/M桥] -> core/lsu -> [M/W桥] -> core/wbu
//  - 原版模块保持纯组合, 桥就是教科书上的 IF/ID、ID/EX、EX/MEM、MEM/WB 寄存器
// ==================================================================

// ========== 总线消息定义 ==========
// 指令识别信号集合（与 decoder 的输出一一对应）
class OpSignals extends Bundle {
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
class IFUToIDUMessage extends Bundle {
  val inst1         = UInt(32.W)
  val inst2         = UInt(32.W)
  val inst1_pc      = UInt(32.W)
  val inst2_pc      = UInt(32.W)
  val inst1_nextpc  = UInt(32.W)
  val inst2_nextpc  = UInt(32.W)
}

// IDU -> EXU 的译码结果消息（两条指令的译码信息 + GRF 读出的操作数）
class IDUToEXUMessage extends Bundle {
  val dec1_op      = new OpSignals
  val dec1_imm     = UInt(32.W)
  val dec1_rs1_val = UInt(32.W)
  val dec1_rs2_val = UInt(32.W)
  val dec1_nextpc  = UInt(32.W)
  val dec1_rd      = UInt(5.W)

  val dec2_op      = new OpSignals
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
class MemoryOpSignals extends Bundle {
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
class EXUToLSUMessage extends Bundle {
  // lane1: 第一条指令
  val op1          = new MemoryOpSignals
  val addr1        = UInt(32.W)
  val store_data1  = UInt(32.W)
  val rd1          = UInt(5.W)
  val wb_data1     = UInt(32.W)

  // lane2: 第二条指令（单发射时全部清零）
  val op2          = new MemoryOpSignals
  val addr2        = UInt(32.W)
  val store_data2  = UInt(32.W)
  val rd2          = UInt(5.W)
  val wb_data2     = UInt(32.W)

  // ebreak 恒在 lane1（控制类指令强制单发射）
  val is_ebreak = Bool()
}

// LSU -> WBU 的写回消息
class LSUToWBUMessage extends Bundle {
  val rd1      = UInt(5.W)
  val wb_data1 = UInt(32.W)
  val rd2      = UInt(5.W)
  val wb_data2 = UInt(32.W)
}

// ==================================================================
// 【多周期新增部件】总线桥 BusBridge：插在原版组合部件之间的通用流水站
//   1) 1 深度级寄存器(按消息类型参数化, 完全不感知消息内容)
//   2) valid/ready 两状态机(空/满)
//   3) 掩码机制(默认开启): 交出消息(out.fire 且无新消息)后寄存器清零,
//      使原版组合部件在空闲拍看到无副作用数据——原版 EXU 的 CSR 写/
//      csr_to_grf/异常/分支, 原版 LSU 的 dmem 写, 原版 wbu 的 GRF 写,
//      因此每个包的副作用恰好生效一次, 无需任何 commit 门控
//   4) 依赖不变量: 一次在飞行中最多一个取指包(IFU 等 done 才取指),
//      各桥 valid 严格错开一拍, 下游桥永远就绪
//   maskOnHandoff=false 变体(用于 F/D 桥): 交出后保持消息不掩码,
//      供顶层 debug 显示"驻留包"(idu 为纯组合无副作用, 保持安全)
// ==================================================================
class BusBridge[T <: Data](gen: T, maskOnHandoff: Boolean = true) extends Module {
  val io = IO(new Bundle {
    val in  = Flipped(Decoupled(gen))   // <- 上游消息(IFU 或原版组合部件的输出)
    val out = Decoupled(gen)            // -> 下游原版组合部件(其 fire 由下一座桥的 ready 提供)
  })

  val bridgeReg   = Reg(gen)              // 1 深度消息寄存器
  val bridgeValid = RegInit(false.B)      // 满/空状态机

  io.in.ready  := !bridgeValid
  io.out.valid := bridgeValid
  io.out.bits  := bridgeReg

  when (io.in.fire) {
    bridgeReg   := io.in.bits
    bridgeValid := true.B
  } .elsewhen (io.out.fire) {
    bridgeReg   := (if (maskOnHandoff) 0.U.asTypeOf(gen) else bridgeReg)  // 掩码: 交出后清零
    bridgeValid := false.B
  }
}

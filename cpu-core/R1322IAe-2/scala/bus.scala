package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== 译码控制信号 ===========================================
// 一条指令的一热控制信号集合, 由 Decoder 产出, 随级间消息一路携带到 EXU。
// 所有派生谓词 (isBranch/isLoad/... ) 以 def 形式集中在这里:
//   上层模块直接调用, 不必各自重复写一长串 "||" (可读性 + 单一事实来源)。
class CtrlSignals extends Bundle {
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
  val is_csrrw  = Bool()
  val is_csrrs  = Bool()
  val is_csrrc  = Bool()
  val is_csrrwi = Bool()
  val is_csrrsi = Bool()
  val is_csrrci = Bool()
  val is_ecall  = Bool()
  val is_ebreak = Bool()
  val is_mret   = Bool()

  // ---------------- 粗粒度谓词 (decoder 一级 opcode 判定) ----------------
  // 直接由 opcode 得到, 供 EXU 等下游直接使用, 避免对一热信号做多输入 OR (缩短关键路径)。
  val is_branch  = Bool()
  val is_jump    = Bool()
  val is_load    = Bool()
  val is_store   = Bool()
  val is_imm_op  = Bool()
  val is_alu_op  = Bool()
  val is_csr     = Bool()
}

// 访存操作类型 (EXU -> LSU)
class MemOp extends Bundle {
  val is_lb     = Bool()
  val is_lh     = Bool()
  val is_lw     = Bool()
  val is_lbu    = Bool()
  val is_lhu    = Bool()
  val is_sb     = Bool()
  val is_sh     = Bool()
  val is_sw     = Bool()
  val is_ebreak = Bool()

  def isLoad:  Bool = is_lb || is_lh || is_lw || is_lbu || is_lhu
  def isStore: Bool = is_sb || is_sh || is_sw
}

//=================================== 下行链路消息 ===========================================
// IFU -> IDU: 一拍取回的两条指令及其 PC
class IFU_to_IDU_Message extends Bundle {
  val inst1        = UInt(32.W)
  val inst2        = UInt(32.W)
  val inst1_pc     = UInt(32.W)
  val inst2_pc     = UInt(32.W)
}

// IDU -> EXU: 单 lane 的译码结果。
// 相比旧版把 nextpc 换成 pc (指令自身 PC): EXU 不再做 nextpc-4 的减法。
class IDU_to_EXU_Lane_Message extends Bundle {
  val ctrl     = new CtrlSignals
  val imm      = UInt(32.W)
  val rs1_addr = UInt(5.W)    // 源寄存器号 (转发比较用)
  val rs2_addr = UInt(5.W)
  val rs1_val  = UInt(32.W)
  val rs2_val  = UInt(32.W)
  val pc       = UInt(32.W)
  val rd       = UInt(5.W)
  val is_stall = Bool()
  val inst     = UInt(32.W)   // 原始指令 (CSR 地址/uimm 用)
}

// EXU -> LSU: 单 lane 的访存请求 + 穿透写回数据
class EXU_to_LSU_Lane_Message extends Bundle {
  val op         = new MemOp
  val addr       = UInt(32.W)
  val store_data = UInt(32.W)
  val rd         = UInt(5.W)
  val wb_data    = UInt(32.W)
}

// LSU -> WBU: 单 lane 的写回
class LSU_to_WBU_Lane_Message extends Bundle {
  val rd          = UInt(5.W)
  val grf_wb_data = UInt(32.W)
}

//=================================== 级间桥接 ===============================================
// 1 深度握手桥: 一次一包, 交出后清零, 保证下游在桥空时看到的位全为 0 (无副作用)。
class StageConnectBridge[T <: Data](gen: T) extends Module {
  val io = IO(new Bundle {
    val in  = Flipped(Decoupled(gen))
    val out = Decoupled(gen)
  })

  val valid = RegInit(false.B)
  val bits  = RegInit(0.U.asTypeOf(gen))

  io.in.ready  := !valid
  io.out.valid := valid
  io.out.bits  := bits

  when (io.in.fire) {
    bits  := io.in.bits
    valid := true.B
  } .elsewhen (io.out.fire) {
    bits  := 0.U.asTypeOf(gen)
    valid := false.B
  }
}

object StageConnect {
  def apply[T <: Data](left: DecoupledIO[T], right: DecoupledIO[T]): Unit = {
    val bridge = Module(new StageConnectBridge(chiselTypeOf(left.bits)))
    bridge.io.in  <> left
    bridge.io.out <> right
  }
}

//=================================== 流水级间寄存器 (pipeline) ===============================
// 与 multi 版"空才能收"的 StageConnectBridge 不同: 本寄存器每拍可进一包(输入输出同拍吞吐),
//   stall: 冻结本级(保持已存数据), 用于前端冒险阻塞;
//   flush: 清除本级有效位(冲刷错取的包); 只影响下一拍。
// 关键: 无效时把 bits 清零, 否则下游 (lsu/wbu) 会拿旧包的位产生副作用。
class StagePipeBridge[T <: Data](gen: T) extends Module {
  val io = IO(new Bundle {
    val in    = Flipped(Decoupled(gen))
    val out   = Decoupled(gen)
    val stall = Input(Bool())
    val flush = Input(Bool())
  })

  val valid = RegInit(false.B)
  val bits  = RegInit(0.U.asTypeOf(gen))

  io.in.ready  := !io.stall && !io.flush
  io.out.valid := valid
  io.out.bits  := bits

  when (io.flush) {
    valid := false.B
    bits  := 0.U.asTypeOf(gen)
  } .elsewhen (io.stall) {
    // 冻结: 保持
  } .elsewhen (io.in.valid) {
    bits  := io.in.bits
    valid := true.B
  } .otherwise {
    valid := false.B
    bits  := 0.U.asTypeOf(gen)
  }
}

object StagePipe {
  def apply[T <: Data](left: DecoupledIO[T], right: DecoupledIO[T],
                       stall: Bool, flush: Bool): Unit = {
    val bridge = Module(new StagePipeBridge(chiselTypeOf(left.bits)))
    bridge.io.in    <> left
    bridge.io.out   <> right
    bridge.io.stall := stall
    bridge.io.flush := flush
  }
}

package R1322IAeCSR

import chisel3._
import chisel3.util._

// SimpleBus 存储器读延迟 (拍)。需与 resources/DPI_Memory.v 的 LATENCY 保持一致。
object MemCfg {
  val latency: Int = 1
}

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
  // RV32M 乘除法 (通过多周期 MDU 执行)
  val is_mul    = Bool()
  val is_mulh   = Bool()
  val is_mulhsu = Bool()
  val is_mulhu  = Bool()
  val is_div    = Bool()
  val is_divu   = Bool()
  val is_rem    = Bool()
  val is_remu   = Bool()
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

  // RV32M: 是否走多周期 MDU
  def is_muldiv: Bool =
    is_mul || is_mulh || is_mulhsu || is_mulhu ||
    is_div || is_divu || is_rem || is_remu
  // 是否乘 (相对除): 决定 MDU 走乘法还是除法数据通路
  def is_mul_op: Bool = is_mul || is_mulh || is_mulhsu || is_mulhu
  // 是否写通用寄存器 rd (用于 RA 别名追踪的清除)
  def writes_rd: Bool = is_alu_op || is_jump || is_load || is_csr
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

// 方向预测随指令一路携带的信息 (IFU -> 队列 -> IDU -> EXU 校验/更新)。
//   fetch_taken: 取指是否真的按 taken 跳转 (dir_taken && BTB 命中)
//   dir_taken  : 方向预测器(锦标赛)结论
//   taken      : 最终方向 (IDU 填, EXU 用它判"实际 vs 预测")
//   gsh_pred/loc_pred: 取指时动态分量的结论 (供选择器更新)
//   gsh_idx/loc_hist : 取指时用的索引/局部历史 (供决议时更新对应计数器, GHR 已变)
class PredInfo extends Bundle {
  val fetch_taken = Bool()
  val dir_taken   = Bool()
  val taken       = Bool()
  val gsh_pred    = Bool()
  val loc_pred    = Bool()
  val gsh_idx     = UInt(8.W)
  val loc_hist    = UInt(6.W)
  val pred_target = UInt(32.W)  // 取指/译码预测 taken 时用的目标 (EXU 校验目标对错)
  val lc_hit      = Bool()      // 循环计数器给出了高置信方向 (抑制 IDU 的 BTFN 兜底)
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
  val pred     = new PredInfo // 方向预测结论 (随指令带到 EXU 校验)
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

// D/E 级寄存器 (带操作数刷新): 冻结 (stall) 时用 refresh 覆盖 rs1_val/rs2_val,
// 保证因访存延迟被冻结在 E 级的指令操作数不因 M/W 级生产者流出而变陈旧。
class ERefreshPipeBridge extends Module {
  val io = IO(new Bundle {
    val in      = Flipped(Decoupled(new IDU_to_EXU_Lane_Message))
    val out     = Decoupled(new IDU_to_EXU_Lane_Message)
    val refresh = Input(new IDU_to_EXU_Lane_Message)
    val stall   = Input(Bool())
    val flush   = Input(Bool())
  })

  val valid = RegInit(false.B)
  val bits  = RegInit(0.U.asTypeOf(new IDU_to_EXU_Lane_Message))

  io.in.ready  := !io.stall && !io.flush
  io.out.valid := valid
  io.out.bits  := bits

  when (io.flush) {
    valid := false.B
    bits  := 0.U.asTypeOf(new IDU_to_EXU_Lane_Message)
  } .elsewhen (io.stall) {
    when (valid) {
      bits.rs1_val := io.refresh.rs1_val
      bits.rs2_val := io.refresh.rs2_val
    }
  } .elsewhen (io.in.valid) {
    bits  := io.in.bits
    valid := true.B
  } .otherwise {
    valid := false.B
    bits  := 0.U.asTypeOf(new IDU_to_EXU_Lane_Message)
  }
}

object ERefreshPipe {
  def apply(in: DecoupledIO[IDU_to_EXU_Lane_Message], out: DecoupledIO[IDU_to_EXU_Lane_Message],
            refresh: IDU_to_EXU_Lane_Message, stall: Bool, flush: Bool): Unit = {
    val bridge = Module(new ERefreshPipeBridge)
    bridge.io.in      <> in
    bridge.io.out     <> out
    bridge.io.refresh := refresh
    bridge.io.stall   := stall
    bridge.io.flush   := flush
  }
}

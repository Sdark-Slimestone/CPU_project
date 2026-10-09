package R1322IAeCSR

import chisel3._
import chisel3.util._


//===================================下行链路消息===========================================
class IFU_to_IDU_Message extends Bundle {
  val inst1        = UInt(32.W)
  val inst2        = UInt(32.W)
  val inst1_pc     = UInt(32.W)
  val inst2_pc     = UInt(32.W)
  val inst1_nextpc = UInt(32.W)
  val inst2_nextpc = UInt(32.W)
}

/* 未使用的旧双lane消息，注释保留
class IDU_to_EXU_Message extends Bundle {
  val dec1_op = new Bundle {
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
  // 合并后的立即数
  val dec1_imm = UInt(32.W)
  val dec1_val = new Bundle {
    val rs1_val = UInt(32.W)
    val rs2_val = UInt(32.W)
    val nextpc  = UInt(32.W)
  }
  val dec1_rd   = UInt(5.W)

  val dec2_op = new Bundle {
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
  // 合并后的立即数
  val dec2_imm = UInt(32.W)
  val dec2_val = new Bundle {
    val rs1_val = UInt(32.W)
    val rs2_val = UInt(32.W)
    val nextpc  = UInt(32.W)
  }
  val dec2_rd   = UInt(5.W)

  // CSR 信号
  val is_stall  = Bool()
  val inst1_pc  = UInt(32.W)
  val inst1     = UInt(32.W)
}

class EXU_to_LSU_Message extends Bundle {
  val op1 = new Bundle {
    val is_lb     = Bool()
    val is_lh     = Bool()
    val is_lw     = Bool()
    val is_lbu    = Bool()
    val is_lhu    = Bool()
    val is_sb     = Bool()
    val is_sh     = Bool()
    val is_sw     = Bool()
    val is_ebreak = Bool()
  }
  val paddr1 = new Bundle {
    val addr = UInt(32.W)
  }
  val data1 = new Bundle {
    val store_data = UInt(32.W)
  }
  val exu_through_lsu_to_wbu1 = new Bundle {
    val rd = UInt(5.W)
    val grf_wb_data = UInt(32.W)
  }

  val op2 = new Bundle {
    val is_lb     = Bool()
    val is_lh     = Bool()
    val is_lw     = Bool()
    val is_lbu    = Bool()
    val is_lhu    = Bool()
    val is_sb     = Bool()
    val is_sh     = Bool()
    val is_sw     = Bool()
    val is_ebreak = Bool()
  }
  val paddr2 = new Bundle {
    val addr = UInt(32.W)
  }
  val data2 = new Bundle {
    val store_data = UInt(32.W)
  }
  val exu_through_lsu_to_wbu2 = new Bundle {
    val rd = UInt(5.W)
    val grf_wb_data = UInt(32.W)
  }
}

class LSU_to_WBU_Message extends Bundle {
  val rd1      = UInt(5.W)
  val grf_wb_data1 = UInt(32.W)
  val rd2      = UInt(5.W)
  val grf_wb_data2 = UInt(32.W)
}
未使用的旧双lane消息结束 */

//===================================新增: D/E 单lane消息===========================================
class IDU_to_EXU_Lane_Message extends Bundle {
  val op = new Bundle {
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
    val is_csrrw  = Bool()
    val is_csrrs  = Bool()
    val is_csrrc  = Bool()
    val is_csrrwi = Bool()
    val is_csrrsi = Bool()
    val is_csrrci = Bool()
    val is_ecall  = Bool()
    val is_mret   = Bool()
  }
  val imm      = UInt(32.W)
  val rs1_addr = UInt(5.W)    // 源寄存器地址(IDU1 携带给 IDU2 读 GRF; 发往 EXU 时无用)
  val rs2_addr = UInt(5.W)
  val rs1_val  = UInt(32.W)   // 源操作数(IDU1->IDU2 时无意义, 由 IDU2 读+转发后填入)
  val rs2_val  = UInt(32.W)
  val pc       = UInt(32.W)   // 指令自身 PC (原为 nextpc, 由 IDU1 直接携带 inst_pc, 省去 EXU 的 nextpc-4)
  val rd       = UInt(5.W)
  val is_stall = Bool()
  val inst1_pc = UInt(32.W)
  val inst1    = UInt(32.W)
}
//===================================新增: D/E 单lane消息===========================================

//===================================新增: IDU1(译码+GRF读) -> IDU2(冒险+打包) 消息===========================================
// IDU1 只做译码和 GRF 组合读, 把两条 lane 的完整译码结果与冒险辅助信号锁存给 IDU2;
// IDU2 再用这些已锁存的值做冒险判断与 final_stall 选通(缩短 D 级单拍组合路径)
class IDU1_to_IDU2_Message extends Bundle {
  val lane1       = new IDU_to_EXU_Lane_Message
  val lane2       = new IDU_to_EXU_Lane_Message
  // 冒险检测辅助信号(D1 预先算好, D2 直接用)
  val is_control1 = Bool()   // lane1 是控制指令(跳转/分支/SYSTEM)
  val is_control2 = Bool()   // lane2 是控制指令
  val is_store1   = Bool()   // lane1 是存储
  val is_store2   = Bool()   // lane2 是存储
  val is_load1    = Bool()   // lane1 是载入
  val is_load2    = Bool()   // lane2 是载入
  val raw         = Bool()   // lane1 写 rd 且被 lane2 读(RAW 冒险)
  val debug_inst2 = UInt(32.W)
}
//===================================新增: D1 -> D2 消息===========================================

//===================================新增: E/M 单lane消息===========================================
class EXU_to_LSU_Lane_Message extends Bundle {
  val op = new Bundle {
    val is_lb     = Bool()
    val is_lh     = Bool()
    val is_lw     = Bool()
    val is_lbu    = Bool()
    val is_lhu    = Bool()
    val is_sb     = Bool()
    val is_sh     = Bool()
    val is_sw     = Bool()
    val is_ebreak = Bool()
  }
  val addr       = UInt(32.W)
  val store_data = UInt(32.W)
  val rd         = UInt(5.W)
  val wb_data    = UInt(32.W)
}
//===================================新增: E/M 单lane消息===========================================

//===================================新增: M/W 单lane消息===========================================
class LSU_to_WBU_Lane_Message extends Bundle {
  val rd          = UInt(5.W)
  val grf_wb_data = UInt(32.W)
}
//===================================新增: M/W 单lane消息===========================================

//===================================桥接模块===========================================
class StageConnectBridge[T <: Data](gen: T) extends Module {
  val io = IO(new Bundle {
    val in  = Flipped(Decoupled(gen))
    val out = Decoupled(gen)
  })

  val valid = RegInit(false.B)
  val bits  = Reg(gen)

  io.in.ready  := !valid        
  io.out.valid := valid
  io.out.bits  := bits

  when (io.in.fire) {
    bits  := io.in.bits
    valid := true.B
  } .elsewhen (io.out.fire) {
    bits  := 0.U.asTypeOf(gen)   // 交出后清零: 下游在桥空时看到的全是 0, 不会产生副作用
    valid := false.B
  }
}
//===================================方便使用桥接模块的妙妙工具===========================================
object StageConnect {
  def apply[messagetype <: Data](left: DecoupledIO[messagetype], right: DecoupledIO[messagetype], arch: String = "multi"): Unit = { //unit类似于void，无返回类型
    require(arch == "multi", "目前只实现 multi")
    val bridge = Module(new StageConnectBridge(chiselTypeOf(left.bits)))
    bridge.io.in  <> left
    bridge.io.out <> right
  }
}

//===================================流水桥接模块(pipeline)===========================================
// 与 multi 版的区别: 输入/输出可同拍吞吐(每拍进一包), 并支持两个控制:
//   stall: 冻结本级(保持已存数据), 用于前端冒险阻塞
//   flush: 清除本级有效位(冲刷错取的包); 只影响下一拍, 本拍已在用的输出不受影响
// clearBitsOnFlush: flush 时是否组合清零 bits。
//   对喂给"会直接用 bits 产生副作用"的级(如 D2/E->EXU)必须 true;
//   对只用 valid 门控的级(IF/D、D1/D2)可 false —— 这样 redirect->flush->bits 的长路径被切断。
class StagePipeBridge[T <: Data](gen: T, clearBitsOnFlush: Boolean = true) extends Module {
  val io = IO(new Bundle {
    val in    = Flipped(Decoupled(gen))
    val out   = Decoupled(gen)
    val stall = Input(Bool())   // 冻结
    val flush = Input(Bool())   // 冲刷
  })

  val valid = RegInit(false.B)
  val bits  = RegInit(0.U.asTypeOf(gen))   // 无效时必须清零, 否则下游会拿旧包的位产生副作用

  io.in.ready  := !io.stall && !io.flush
  io.out.valid := valid
  io.out.bits  := bits

  when (io.flush) {
    valid := false.B            // 冲刷: 丢弃本拍送入的包
    if (clearBitsOnFlush) { bits := 0.U.asTypeOf(gen) }
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
//===================================方便使用流水桥接模块的妙妙工具===========================================
object StagePipe {
  def apply[messagetype <: Data](left: DecoupledIO[messagetype], right: DecoupledIO[messagetype],
                                 stall: Bool, flush: Bool, clearBitsOnFlush: Boolean = true): Unit = {
    val bridge = Module(new StagePipeBridge(chiselTypeOf(left.bits), clearBitsOnFlush))
    bridge.io.in    <> left
    bridge.io.out   <> right
    bridge.io.stall := stall
    bridge.io.flush := flush
  }
}
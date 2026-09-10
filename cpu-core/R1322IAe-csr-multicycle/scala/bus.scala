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
  val rs1_val  = UInt(32.W)
  val rs2_val  = UInt(32.W)
  val nextpc   = UInt(32.W)
  val rd       = UInt(5.W)
  val is_stall = Bool()
  val inst1_pc = UInt(32.W)
  val inst1    = UInt(32.W)
}
//===================================新增: D/E 单lane消息===========================================

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
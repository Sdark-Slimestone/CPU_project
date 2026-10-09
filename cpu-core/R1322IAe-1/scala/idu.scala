package R1322IAeCSR

import chisel3._
import chisel3.util._

// 译码/冒险单元 (双发射, 一次一包)。
// 对 IFU 取回的两条指令各自译码, 检测包内冒险, 决定是否单发射 (final_stall),
// 并把两条 lane 的译码结果打包成单 lane 消息发往两个 EXU。
class IDU extends Module {
  val io = IO(new Bundle {
    // 来自 IFU
    val ifu_to_idu = Flipped(Decoupled(new IFU_to_IDU_Message))

    // 来自/发往 GRF
    val grf_to_idu = new Bundle {
      val dec1_value = new Bundle {
        val rs1_value = Input(UInt(32.W))
        val rs2_value = Input(UInt(32.W))
      }
      val dec2_value = new Bundle {
        val rs1_value = Input(UInt(32.W))
        val rs2_value = Input(UInt(32.W))
      }
    }
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

    // 发往两个 EXU 的 D/E 单 lane 消息
    val idu_to_exu1 = Decoupled(new IDU_to_EXU_Lane_Message)
    val idu_to_exu2 = Decoupled(new IDU_to_EXU_Lane_Message)

    // 发往 IFU 的单发射信号
    val idu_to_ifu = new Bundle {
      val is_stall = Output(Bool())
    }

    val debug = new Bundle {
      val debug_inst1 = Output(UInt(32.W))
      val debug_inst2 = Output(UInt(32.W))
      val is_stall    = Output(Bool())
    }
  })

  // ---------------- 译码 ----------------
  val dec1 = Module(new Decoder)
  val dec2 = Module(new Decoder)
  dec1.io.inst := io.ifu_to_idu.bits.inst1
  dec2.io.inst := io.ifu_to_idu.bits.inst2
  io.ifu_to_idu.ready := true.B

  val c1 = dec1.io.out.ctrl
  val c2 = dec2.io.out.ctrl

  // ---------------- 冒险检测 ----------------
  // 控制/访存判定用 decoder 的粗粒度输出 (opcode 一级比较), 缩短 D 级路径
  val isControl1 = dec1.io.coarse.is_control
  val isControl2 = dec2.io.coarse.is_control
  val isStore1   = dec1.io.coarse.is_store
  val isStore2   = dec2.io.coarse.is_store

  // RAW: 指令1写寄存器且被指令2读
  val raw = (dec1.io.out.rd =/= 0.U) &&
            ((dec1.io.out.rd === dec2.io.out.rs1) || (dec1.io.out.rd === dec2.io.out.rs2))

  // 内存冒险:
  //   store1 -> load2 同字的 RAW 由 dmem 的 store-to-load 转发解决 (保持双发射);
  //   两个 store 冲突 (单写口) 仍需单发射。
  //   两条 load 不需要处理: 存储器有独立双读口 (DPI 与 RegisterFile 都是 2R)。
  val ramwaw = isStore1 && isStore2

  val stall_sig  = raw || isControl1 || ramwaw
  val stall_sig2 = !isControl1 && isControl2   // lane2 是控制指令但 lane1 不是 -> 不能双发射
  val final_stall = stall_sig || stall_sig2

  io.idu_to_ifu.is_stall := final_stall

  // ---------------- GRF 读地址 ----------------
  io.idu_to_grf.dec1_redreg.rs1 := dec1.io.out.rs1
  io.idu_to_grf.dec1_redreg.rs2 := dec1.io.out.rs2
  io.idu_to_grf.dec2_redreg.rs1 := dec2.io.out.rs1
  io.idu_to_grf.dec2_redreg.rs2 := dec2.io.out.rs2

  // ---------------- D/E 打包 ----------------
  // lane1: 直接透传译码结果
  io.idu_to_exu1.bits.ctrl     := c1
  io.idu_to_exu1.bits.imm      := dec1.io.out.imm
  io.idu_to_exu1.bits.rs1_val  := io.grf_to_idu.dec1_value.rs1_value
  io.idu_to_exu1.bits.rs2_val  := io.grf_to_idu.dec1_value.rs2_value
  io.idu_to_exu1.bits.pc       := io.ifu_to_idu.bits.inst1_pc
  io.idu_to_exu1.bits.rd       := dec1.io.out.rd
  io.idu_to_exu1.bits.is_stall := final_stall
  io.idu_to_exu1.bits.inst     := io.ifu_to_idu.bits.inst1
  io.idu_to_exu1.valid         := io.ifu_to_idu.valid

  // lane2: 直接透传译码结果; 单发射时只拉低 valid。
  // D/E 桥只在 valid 时捕获, 且交出后清零 -> 桥空时下游看到的位全为 0, 无副作用。
  // (省掉了对 ~80 位逐字段的 Mux(final_stall), 大幅降低 final_stall 扇出)
  io.idu_to_exu2.bits.ctrl     := c2
  io.idu_to_exu2.bits.imm      := dec2.io.out.imm
  io.idu_to_exu2.bits.rs1_val  := io.grf_to_idu.dec2_value.rs1_value
  io.idu_to_exu2.bits.rs2_val  := io.grf_to_idu.dec2_value.rs2_value
  io.idu_to_exu2.bits.pc       := io.ifu_to_idu.bits.inst2_pc
  io.idu_to_exu2.bits.rd       := dec2.io.out.rd
  io.idu_to_exu2.bits.is_stall := false.B
  io.idu_to_exu2.bits.inst     := io.ifu_to_idu.bits.inst2
  io.idu_to_exu2.valid         := io.ifu_to_idu.valid && !final_stall

  // ---------------- 调试 ----------------
  io.debug.debug_inst1 := dec1.io.debug_inst
  io.debug.debug_inst2 := Mux(final_stall, 0.U(32.W), dec2.io.debug_inst)
  io.debug.is_stall    := final_stall
}

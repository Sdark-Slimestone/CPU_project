package R1322IAe

import chisel3._
import chisel3.util._

class GRF extends Module {
  val io = IO(new Bundle {

    val idu_to_grf = new Bundle {
      val dec1_redreg = new Bundle {
        val rs1 = Input(UInt(5.W))
        val rs2 = Input(UInt(5.W))
      }
      val dec2_redreg = new Bundle {
        val rs1 = Input(UInt(5.W))
        val rs2 = Input(UInt(5.W))
      }
    }

    val grf_to_idu = new Bundle {
      val dec1_value = new Bundle {
        val inst1rs1_value = Output(UInt(32.W))
        val inst1rs2_value = Output(UInt(32.W))
      }
      val dec2_value = new Bundle {
        val inst2rs1_value = Output(UInt(32.W))
        val inst2rs2_value = Output(UInt(32.W))
      }
    }

    val wbu_to_grf = new Bundle {
      val wr1 = new Bundle {
        val addr = Input(UInt(5.W))
        val data = Input(UInt(32.W))
      }
      val wr2 = new Bundle {
        val addr = Input(UInt(5.W))
        val data = Input(UInt(32.W))
      }
    }

    // 调试
    val debug_regs   = Output(Vec(16, UInt(32.W)))
    val debug_rden   = Output(Bool())
    val debug_rdaddr = Output(UInt(5.W))
    val debug_input  = Output(UInt(32.W))
  })

  val regs = RegInit(VecInit(Seq.fill(16)(0.U(32.W))))

  // 读1 (inst1 rs1)
  io.grf_to_idu.dec1_value.inst1rs1_value :=
    Mux((io.idu_to_grf.dec1_redreg.rs1 =/= 0.U) && (io.idu_to_grf.dec1_redreg.rs1(4) === 0.U),
        regs(io.idu_to_grf.dec1_redreg.rs1(3,0)), 0.U(32.W))

  // 读2 (inst1 rs2)
  io.grf_to_idu.dec1_value.inst1rs2_value :=
    Mux((io.idu_to_grf.dec1_redreg.rs2 =/= 0.U) && (io.idu_to_grf.dec1_redreg.rs2(4) === 0.U),
        regs(io.idu_to_grf.dec1_redreg.rs2(3,0)), 0.U(32.W))

  // 读3 (inst2 rs1)
  io.grf_to_idu.dec2_value.inst2rs1_value :=
    Mux((io.idu_to_grf.dec2_redreg.rs1 =/= 0.U) && (io.idu_to_grf.dec2_redreg.rs1(4) === 0.U),
        regs(io.idu_to_grf.dec2_redreg.rs1(3,0)), 0.U(32.W))

  // 读4 (inst2 rs2)
  io.grf_to_idu.dec2_value.inst2rs2_value :=
    Mux((io.idu_to_grf.dec2_redreg.rs2 =/= 0.U) && (io.idu_to_grf.dec2_redreg.rs2(4) === 0.U),
        regs(io.idu_to_grf.dec2_redreg.rs2(3,0)), 0.U(32.W))

  // 写逻辑（使用 wbu_to_grf 中的两个写通道）
  val nextRegs = Wire(Vec(16, UInt(32.W)))
  for (i <- 0 until 16) {
    val wr1 = (io.wbu_to_grf.wr1.addr =/= 0.U) &&
              (io.wbu_to_grf.wr1.addr(4) === 0.U) &&
              (io.wbu_to_grf.wr1.addr(3,0) === i.U)
    val wr2 = (io.wbu_to_grf.wr2.addr =/= 0.U) &&
              (io.wbu_to_grf.wr2.addr(4) === 0.U) &&
              (io.wbu_to_grf.wr2.addr(3,0) === i.U)
    nextRegs(i) := Mux(wr1, io.wbu_to_grf.wr1.data,
                   Mux(wr2, io.wbu_to_grf.wr2.data, regs(i)))
  }
  regs := nextRegs

  // 调试信息（使用 wr1 端口作为调试观察）
  io.debug_regs   := regs
  io.debug_rden   := (io.wbu_to_grf.wr1.addr =/= 0.U) &&
                     (io.wbu_to_grf.wr1.addr(4) === 0.U)
  io.debug_rdaddr := io.wbu_to_grf.wr1.addr
  io.debug_input  := io.wbu_to_grf.wr1.data
}
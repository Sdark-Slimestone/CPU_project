package R1322IAeCSR

import chisel3._
import chisel3.util._

// 通用寄存器堆，支持双发射读和双端口写，额外CSR写端口
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

    // top 层 CSR 写回（CSR指令的旧CSR值写回rd）
    val csr_to_grf = new Bundle {
      val wen   = Input(Bool())
      val waddr = Input(UInt(5.W))
      val wdata = Input(UInt(32.W))
    }

    val debug_regs   = Output(Vec(16, UInt(32.W)))
    val debug_rden   = Output(Bool())
    val debug_rdaddr = Output(UInt(5.W))
    val debug_input  = Output(UInt(32.W))
  })

  val regs = RegInit(VecInit(Seq.fill(16)(0.U(32.W))))

  // 读端口（组合逻辑）
  io.grf_to_idu.dec1_value.inst1rs1_value :=
    Mux((io.idu_to_grf.dec1_redreg.rs1 =/= 0.U) && (io.idu_to_grf.dec1_redreg.rs1(4) === 0.U),
        regs(io.idu_to_grf.dec1_redreg.rs1(3,0)), 0.U(32.W))

  io.grf_to_idu.dec1_value.inst1rs2_value :=
    Mux((io.idu_to_grf.dec1_redreg.rs2 =/= 0.U) && (io.idu_to_grf.dec1_redreg.rs2(4) === 0.U),
        regs(io.idu_to_grf.dec1_redreg.rs2(3,0)), 0.U(32.W))

  io.grf_to_idu.dec2_value.inst2rs1_value :=
    Mux((io.idu_to_grf.dec2_redreg.rs1 =/= 0.U) && (io.idu_to_grf.dec2_redreg.rs1(4) === 0.U),
        regs(io.idu_to_grf.dec2_redreg.rs1(3,0)), 0.U(32.W))

  io.grf_to_idu.dec2_value.inst2rs2_value :=
    Mux((io.idu_to_grf.dec2_redreg.rs2 =/= 0.U) && (io.idu_to_grf.dec2_redreg.rs2(4) === 0.U),
        regs(io.idu_to_grf.dec2_redreg.rs2(3,0)), 0.U(32.W))

  // 写逻辑（三个写源：WBU的wr1/wr2 + CSR写回，优先级 CSR > WBU > WBU）
  val nextRegs = Wire(Vec(16, UInt(32.W)))
  for (i <- 0 until 16) {
    val csr_wr = (io.csr_to_grf.waddr =/= 0.U) &&
                 (io.csr_to_grf.waddr(4) === 0.U) &&
                 (io.csr_to_grf.waddr(3,0) === i.U) && io.csr_to_grf.wen
    val wr1 = (io.wbu_to_grf.wr1.addr =/= 0.U) &&
              (io.wbu_to_grf.wr1.addr(4) === 0.U) &&
              (io.wbu_to_grf.wr1.addr(3,0) === i.U)
    val wr2 = (io.wbu_to_grf.wr2.addr =/= 0.U) &&
              (io.wbu_to_grf.wr2.addr(4) === 0.U) &&
              (io.wbu_to_grf.wr2.addr(3,0) === i.U)

    nextRegs(i) := Mux(csr_wr, io.csr_to_grf.wdata,
                   Mux(wr1, io.wbu_to_grf.wr1.data,
                   Mux(wr2, io.wbu_to_grf.wr2.data, regs(i))))
  }
  regs := nextRegs

  // 调试信息
  io.debug_regs   := regs
  io.debug_rden   := (io.wbu_to_grf.wr1.addr =/= 0.U) &&
                     (io.wbu_to_grf.wr1.addr(4) === 0.U)
  io.debug_rdaddr := io.wbu_to_grf.wr1.addr
  io.debug_input  := io.wbu_to_grf.wr1.data
}
package R1322IAeCSR

import chisel3._
import chisel3.util._

// 通用寄存器堆: 4 读口 (双发射各 2 个源) + 3 写口 (WBU wr1/wr2 + CSR 写回)。
// 写优先级 CSR > wr1 > wr2。RV32E 只有 x0~x15, 且 x0 恒为 0。
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
        val rs1_value = Output(UInt(32.W))
        val rs2_value = Output(UInt(32.W))
      }
      val dec2_value = new Bundle {
        val rs1_value = Output(UInt(32.W))
        val rs2_value = Output(UInt(32.W))
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

    // top 层 CSR 写回 (CSR 指令的旧 CSR 值写回 rd)
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

  // ---------------- 组合读 (write-first: 同拍写入的值可直接读出) ----------------
  // 生产者在本拍 W 级写、消费者在同拍 D 级读时, regs 还是旧值; 这里把本拍要写的值
  // 直接旁路给读口, 消掉"读在写那拍"的 RAW。优先级与写逻辑一致: CSR > wr1 > wr2。
  def read(addr: UInt): UInt = {
    val rv      = Mux((addr =/= 0.U) && (addr(4) === 0.U), regs(addr(3, 0)), 0.U(32.W))
    val csr_hit = io.csr_to_grf.wen && (addr =/= 0.U) && (addr(4) === 0.U) &&
                  (addr === io.csr_to_grf.waddr)
    val wr1_hit = (addr =/= 0.U) && (addr(4) === 0.U) && (addr === io.wbu_to_grf.wr1.addr)
    val wr2_hit = (addr =/= 0.U) && (addr(4) === 0.U) && (addr === io.wbu_to_grf.wr2.addr)
    Mux(csr_hit, io.csr_to_grf.wdata,
    Mux(wr1_hit, io.wbu_to_grf.wr1.data,
    Mux(wr2_hit, io.wbu_to_grf.wr2.data, rv)))
  }

  io.grf_to_idu.dec1_value.rs1_value := read(io.idu_to_grf.dec1_redreg.rs1)
  io.grf_to_idu.dec1_value.rs2_value := read(io.idu_to_grf.dec1_redreg.rs2)
  io.grf_to_idu.dec2_value.rs1_value := read(io.idu_to_grf.dec2_redreg.rs1)
  io.grf_to_idu.dec2_value.rs2_value := read(io.idu_to_grf.dec2_redreg.rs2)

  // ---------------- 写逻辑 ----------------
  def wrHit(addr: UInt, i: Int): Bool =
    (addr =/= 0.U) && (addr(4) === 0.U) && (addr(3, 0) === i.U)

  val nextRegs = Wire(Vec(16, UInt(32.W)))
  for (i <- 0 until 16) {
    val csr_wr = wrHit(io.csr_to_grf.waddr, i) && io.csr_to_grf.wen
    val wr1    = wrHit(io.wbu_to_grf.wr1.addr, i)
    val wr2    = wrHit(io.wbu_to_grf.wr2.addr, i)

    nextRegs(i) := Mux(csr_wr, io.csr_to_grf.wdata,
                   Mux(wr1,    io.wbu_to_grf.wr1.data,
                   Mux(wr2,    io.wbu_to_grf.wr2.data, regs(i))))
  }
  regs := nextRegs

  // ---------------- 调试 ----------------
  io.debug_regs   := regs
  io.debug_rden   := (io.wbu_to_grf.wr1.addr =/= 0.U) && (io.wbu_to_grf.wr1.addr(4) === 0.U)
  io.debug_rdaddr := io.wbu_to_grf.wr1.addr
  io.debug_input  := io.wbu_to_grf.wr1.data
}

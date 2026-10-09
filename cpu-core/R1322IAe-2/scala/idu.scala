package R1322IAeCSR

import chisel3._
import chisel3.util._

// 译码/冒险单元: 从取指队列队头取 1~2 条指令, 各自译码, 检测包内冒险。
//
// 与旧版(整包 F/D 寄存器)的区别: 冲突时不再"杀掉 lane2 + 重定向重取",
// 而是只弹 1 条 (fq_pop=1), 第二条留在队头, 下拍直接当 lane1。
//   - 包内 RAW / lane1 控制 / 双 store / lane2 控制  -> 弹 1
//   - 跨包 interlock (load-use / CSR / 访存顺序)      -> 弹 0 (冻结)
//   - 否则                                            -> 弹 2
// 跨包 RAW 由 EX 级转发解决, 这里只停 load-use / CSR / store->load。
class IDU extends Module {
  val io = IO(new Bundle {
    // 取指队列队头 (两条指令)
    val from_fq = new Bundle {
      val inst1 = Input(UInt(32.W))
      val inst2 = Input(UInt(32.W))
      val pc1   = Input(UInt(32.W))
      val pc2   = Input(UInt(32.W))
      val valid = Input(UInt(2.W))   // 队头有效条数 0/1/2
    }
    val fq_pop = Output(UInt(2.W))   // 本拍弹出条数 0/1/2

    // BTFN 静态预测: 译码级预测 taken 的重定向
    val pred_redirect    = Output(Bool())
    val pred_redirect_pc = Output(UInt(32.W))

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

    // 跨包冒险来源 (E 级生产者 + E/M 级 store)
    val fwd = new Bundle {
      val exu_rd       = Input(Vec(2, UInt(5.W)))
      val exu_valid    = Input(Vec(2, Bool()))
      val exu_is_store = Input(Vec(2, Bool()))
      val exu_is_load  = Input(Vec(2, Bool()))
      val exu_is_csr   = Input(Vec(2, Bool()))

      val lsu_valid    = Input(Vec(2, Bool()))
      val lsu_is_store = Input(Vec(2, Bool()))
    }

    // 前端冻结 (跨包 RAW / 存储-载入)
    val stall = Output(Bool())

    // 发往两个 EXU 的 D/E 单 lane 消息
    val idu_to_exu1 = Decoupled(new IDU_to_EXU_Lane_Message)
    val idu_to_exu2 = Decoupled(new IDU_to_EXU_Lane_Message)

    val debug = new Bundle {
      val debug_inst1 = Output(UInt(32.W))
      val debug_inst2 = Output(UInt(32.W))
      val is_stall    = Output(Bool())
      // 停顿原因 (跨包 interlock)
      val stall_e_load = Output(Bool())
      val stall_e_csr  = Output(Bool())
      val stall_mem    = Output(Bool())
      // 单发射原因 (包内冲突)
      val single_raw    = Output(Bool())
      val single_ctl1   = Output(Bool())
      val single_ramwaw = Output(Bool())
      val single_ctl2   = Output(Bool())
    }
  })

  val l1v = io.from_fq.valid >= 1.U
  val l2v = io.from_fq.valid >= 2.U

  // ---------------- 译码 ----------------
  val dec1 = Module(new Decoder)
  val dec2 = Module(new Decoder)
  dec1.io.inst := io.from_fq.inst1
  dec2.io.inst := io.from_fq.inst2

  val c1 = dec1.io.out.ctrl
  val c2 = dec2.io.out.ctrl

  // ---------------- 包内冒险检测 (lane2 无效时全部屏蔽) ----------------
  val isControl1 = l1v && dec1.io.coarse.is_control
  val isControl2 = l2v && dec2.io.coarse.is_control
  val isStore1   = l1v && dec1.io.coarse.is_store
  val isStore2   = l2v && dec2.io.coarse.is_store

  val rs1_2 = Mux(l2v, dec2.io.out.rs1, 0.U)
  val rs2_2 = Mux(l2v, dec2.io.out.rs2, 0.U)

  // RAW: 指令1写寄存器且被指令2读
  val raw = l1v && l2v && (dec1.io.out.rd =/= 0.U) &&
            ((dec1.io.out.rd === rs1_2) || (dec1.io.out.rd === rs2_2))

  val ramwaw = isStore1 && isStore2

  val stall_sig  = raw || isControl1 || ramwaw
  val stall_sig2 = !isControl1 && isControl2
  val final_stall = stall_sig || stall_sig2

  // ---------------- 跨包 RAW / 存储-载入 interlock ----------------
  val rs1_1 = Mux(l1v, dec1.io.out.rs1, 0.U)
  val rs2_1 = Mux(l1v, dec1.io.out.rs2, 0.U)
  val srcRegs = VecInit(rs1_1, rs2_1, rs1_2, rs2_2)

  def hitAny(rd: UInt, valid: Bool): Bool =
    valid && (rd =/= 0.U) && srcRegs.map(_ === rd).reduce(_ || _)

  val e_load_haz = (0 until 2).map(i =>
    hitAny(io.fwd.exu_rd(i), io.fwd.exu_valid(i) && io.fwd.exu_is_load(i))).reduce(_ || _)
  val e_csr_haz  = (0 until 2).map(i =>
    hitAny(io.fwd.exu_rd(i), io.fwd.exu_valid(i) && io.fwd.exu_is_csr(i))).reduce(_ || _)

  val cur_is_load = (l1v && dec1.io.coarse.is_load) || (l2v && dec2.io.coarse.is_load)
  val older_store =
    (0 until 2).map(i => io.fwd.exu_valid(i) && io.fwd.exu_is_store(i)).reduce(_ || _) ||
    (0 until 2).map(i => io.fwd.lsu_valid(i) && io.fwd.lsu_is_store(i)).reduce(_ || _)

  io.stall := l1v && (e_load_haz || e_csr_haz || (cur_is_load && older_store))

  // ---------------- 弹出条数 ----------------
  io.fq_pop := Mux(!l1v, 0.U,
               Mux(io.stall, 0.U,
               Mux(final_stall, 1.U,
               Mux(l2v, 2.U, 1.U))))

  // ---------------- BTFN 静态分支预测 ----------------
  // 后向条件分支 (imm 为负) 和 jal 预测 taken: 目标是 pc+imm, 译码级就能算, 不需要寄存器。
  // 译码级直接改 PC 早取目标; EXU 只在"实际方向 != 预测方向"时才重定向。
  val l1_is_jal    = l1v && c1.is_jal
  val l1_is_bwd_br = l1v && c1.is_branch && dec1.io.out.imm(31)
  io.pred_redirect    := (l1_is_jal || l1_is_bwd_br) && !io.stall
  io.pred_redirect_pc := io.from_fq.pc1 + dec1.io.out.imm

  // ---------------- GRF 读地址 ----------------
  io.idu_to_grf.dec1_redreg.rs1 := rs1_1
  io.idu_to_grf.dec1_redreg.rs2 := rs2_1
  io.idu_to_grf.dec2_redreg.rs1 := rs1_2
  io.idu_to_grf.dec2_redreg.rs2 := rs2_2

  // ---------------- D/E 打包 ----------------
  io.idu_to_exu1.bits.ctrl     := c1
  io.idu_to_exu1.bits.imm      := dec1.io.out.imm
  io.idu_to_exu1.bits.rs1_addr := dec1.io.out.rs1
  io.idu_to_exu1.bits.rs2_addr := dec1.io.out.rs2
  io.idu_to_exu1.bits.rs1_val  := io.grf_to_idu.dec1_value.rs1_value
  io.idu_to_exu1.bits.rs2_val  := io.grf_to_idu.dec1_value.rs2_value
  io.idu_to_exu1.bits.pc       := io.from_fq.pc1
  io.idu_to_exu1.bits.rd       := dec1.io.out.rd
  io.idu_to_exu1.bits.is_stall := final_stall
  io.idu_to_exu1.bits.inst     := io.from_fq.inst1
  io.idu_to_exu1.valid         := l1v && !io.stall

  io.idu_to_exu2.bits.ctrl     := c2
  io.idu_to_exu2.bits.imm      := dec2.io.out.imm
  io.idu_to_exu2.bits.rs1_addr := dec2.io.out.rs1
  io.idu_to_exu2.bits.rs2_addr := dec2.io.out.rs2
  io.idu_to_exu2.bits.rs1_val  := io.grf_to_idu.dec2_value.rs1_value
  io.idu_to_exu2.bits.rs2_val  := io.grf_to_idu.dec2_value.rs2_value
  io.idu_to_exu2.bits.pc       := io.from_fq.pc2
  io.idu_to_exu2.bits.rd       := dec2.io.out.rd
  io.idu_to_exu2.bits.is_stall := false.B
  io.idu_to_exu2.bits.inst     := io.from_fq.inst2
  io.idu_to_exu2.valid         := l2v && !final_stall && !io.stall

  // ---------------- 调试 ----------------
  io.debug.debug_inst1 := dec1.io.debug_inst
  io.debug.debug_inst2 := Mux(final_stall, 0.U(32.W), dec2.io.debug_inst)
  io.debug.is_stall    := final_stall

  io.debug.stall_e_load := e_load_haz
  io.debug.stall_e_csr  := e_csr_haz
  io.debug.stall_mem    := cur_is_load && older_store
  io.debug.single_raw    := raw
  io.debug.single_ctl1   := isControl1
  io.debug.single_ramwaw := ramwaw
  io.debug.single_ctl2   := stall_sig2
}

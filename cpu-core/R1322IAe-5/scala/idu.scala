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
      val pred  = Input(Vec(2, new PredInfo))  // 方向预测结论
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

    // 前端冻结 (跨包 RAW / 存储-载入 / 多周期 MDU)
    val stall = Output(Bool())
    val mdu_stall = Input(Bool())   // 乘除法单元占用中, 冻结前端

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
  val isStore1   = l1v && dec1.io.coarse.is_store
  val isStore2   = l2v && dec2.io.coarse.is_store
  // 控制流 (分支/跳转, 不含系统指令): lane1 允许与 lane2 双发射, 但一对里不能出现两条。
  val isCtlFlow1 = l1v && (dec1.io.coarse.is_branch || dec1.io.coarse.is_jump)
  val isCtlFlow2 = l2v && (dec2.io.coarse.is_branch || dec2.io.coarse.is_jump)

  val rs1_2 = Mux(l2v, dec2.io.out.rs1, 0.U)
  val rs2_2 = Mux(l2v, dec2.io.out.rs2, 0.U)

  // RAW: 指令1写寄存器且被指令2读 -> 单发射。
  // 包内 lane1->lane2 同拍转发实测 IPC 大涨但 E 级 ALU->ALU 链把 fmax 868->605, 不用。
  val raw = l1v && l2v && (dec1.io.out.rd =/= 0.U) &&
            ((dec1.io.out.rd === rs1_2) || (dec1.io.out.rd === rs2_2))

  val ramwaw = isStore1 && isStore2

  // 系统指令 (CSR/异常/mret) 恒在 lane1 单发射 (CSR 状态在 EXU1)。
  val isSys1 = l1v && (c1.is_csr || c1.is_ecall || c1.is_ebreak || c1.is_mret)
  val isSys2 = l2v && (c2.is_csr || c2.is_ecall || c2.is_ebreak || c2.is_mret)
  // RV32M 乘除法走多周期 MDU, 恒单发射 (lane1); lane2 的 M 留到下一拍当 lane1。
  // 用 coarse (单次 opcode/funct7 比较) 而非 8 条一热信号的 OR, 避免拉长前端关键路径。
  val isMulDiv1 = l1v && dec1.io.coarse.is_muldiv
  val isMulDiv2 = l2v && dec2.io.coarse.is_muldiv
  // lane1 控制流双发射: 错路径 lane2 由 top 侧 kill (含 E 级 load 读门控);
  // 一对里两条控制流仍单发射 (重定向/预测器更新只取一条)。
  val stall_ctl   = isSys1 || isSys2 || isMulDiv1 || isMulDiv2 || (isCtlFlow1 && isCtlFlow2)
  val final_stall = raw || ramwaw || stall_ctl

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

  io.stall := io.mdu_stall || (l1v && (e_load_haz || e_csr_haz))

  // ---------------- 弹出条数 ----------------
  io.fq_pop := Mux(!l1v, 0.U,
               Mux(io.stall, 0.U,
               Mux(final_stall, 1.U,
               Mux(l2v, 2.U, 1.U))))

  // ---------------- 译码级兜底重定向 ----------------
  // 取指未按 taken 跳转时 (BTB 未命中/方向预测 not-taken), 若这是条件分支/jal 且
  // (方向预测 taken 或 静态 BTFN taken), 就用 pc+imm 在译码级早重定向。
  // 静态 BTFN: jal 或后向条件分支 (imm 为负) => taken。目标是 pc+imm, 译码级就能算。
  // lane1 / lane2 各自兜底: lane2 控制流 (ctlUse2) 此前没有兜底, 导致 lane2 的 jal
  // 方向预测错一大片 (取指只能靠 BTB, 未命中/方向被别名覆盖就漏). 这里补上。
  val l1_is_jal    = l1v && c1.is_jal
  val l1_is_bwd_br = l1v && c1.is_branch && dec1.io.out.imm(31)
  val p1           = io.from_fq.pred(0)
  // 循环计数器高置信时不再用 BTFN 兜底 (否则会把循环退出的 not-taken 又兜成 taken)。
  val idu_take1    = !p1.lc_hit && !p1.fetch_taken && (l1_is_jal || (l1v && c1.is_branch)) &&
                     (p1.dir_taken || l1_is_jal || l1_is_bwd_br)

  // lane2 的 jal: BTB 命中时由 IFU 的 is_jal 位取指级判 taken; 这里兜底 BTB 未命中的 lane2 jal。
  val l2_is_jal = l2v && c2.is_jal
  val p2        = io.from_fq.pred(1)
  val idu_take2 = !isCtlFlow1 && l2_is_jal && !final_stall && !io.stall && !p2.fetch_taken

  val idu_take        = idu_take1 || idu_take2
  io.pred_redirect    := idu_take && !io.stall
  io.pred_redirect_pc := Mux(idu_take1, io.from_fq.pc1 + dec1.io.out.imm,
                                          io.from_fq.pc2 + dec2.io.out.imm)

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
  // 最终方向 = 取指是否跳转 || 译码兜底跳转。EXU 只做"实际方向 vs 预测方向"比较。
  io.idu_to_exu1.bits.pred       := io.from_fq.pred(0)
  io.idu_to_exu1.bits.pred.taken := io.from_fq.pred(0).fetch_taken || idu_take1
  // 译码兜底重定向时, 目标用 pc+imm (取指未跳, 携带的 pred_target 无效)
  io.idu_to_exu1.bits.pred.pred_target :=
    Mux(idu_take1, io.from_fq.pc1 + dec1.io.out.imm, io.from_fq.pred(0).pred_target)
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
  io.idu_to_exu2.bits.pred       := io.from_fq.pred(1)
  io.idu_to_exu2.bits.pred.taken := io.from_fq.pred(1).fetch_taken || idu_take2
  io.idu_to_exu2.bits.pred.pred_target :=
    Mux(idu_take2, io.from_fq.pc2 + dec2.io.out.imm, io.from_fq.pred(1).pred_target)
  // lane1 兜底重定向 (idu_take1) 时 lane2 必为错路径判无效; idu_take2 时 lane2 自身要发。
  io.idu_to_exu2.valid         := l2v && !final_stall && !io.stall && !idu_take1

  // ---------------- 调试 ----------------
  io.debug.debug_inst1 := dec1.io.debug_inst
  io.debug.debug_inst2 := Mux(final_stall, 0.U(32.W), dec2.io.debug_inst)
  io.debug.is_stall    := final_stall

  io.debug.stall_e_load := e_load_haz
  io.debug.stall_e_csr  := e_csr_haz
  io.debug.stall_mem    := false.B   // store->load 由 dmem store buffer 转发
  // 单发射原因按优先级输出 (prof 里是 else-if 链, 保证只归一类)
  io.debug.single_raw    := raw
  io.debug.single_ctl1   := !raw && isCtlFlow1
  io.debug.single_ramwaw := !raw && !isCtlFlow1 && ramwaw
  io.debug.single_ctl2   := !raw && !isCtlFlow1 && !ramwaw && isSys2

}

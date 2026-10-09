package R1322IAeCSR

import chisel3._
import chisel3.util._

//====================================新增: IDU2 级(GRF 读 + 转发布塞 + 冒险检测 + lane 打包)====================================
// IDU1 只译码; GRF 组合读放在这里。这样读距 EXU 只差一级:
//   K=1 紧邻包在 EXU, ALU 结果本拍可得 -> 直接转发(0 阻塞); 若是 load 数据没好 -> 阻塞 1 拍
//   K=2 在 LSU(含 load 数据) -> 转发
//   K=3 在 WBU(M/W 寄存器写回值) -> 转发
//   K>=4 已写回, GRF 读本身就正确
class idu2 extends Module {
  val io = IO(new Bundle {
    // 来自 IDU1 的译码消息
    val idu1_to_idu2 = Flipped(Decoupled(new IDU1_to_IDU2_Message))

    // GRF 读: 地址来自消息, 值本拍组合返回
    val idu_to_grf = new Bundle {
      val dec1_redreg = new Bundle { val rs1 = Output(UInt(5.W)); val rs2 = Output(UInt(5.W)) }
      val dec2_redreg = new Bundle { val rs1 = Output(UInt(5.W)); val rs2 = Output(UInt(5.W)) }
    }
    val grf_to_idu = new Bundle {
      val dec1_value = new Bundle {
        val inst1rs1_value = Input(UInt(32.W))
        val inst1rs2_value = Input(UInt(32.W))
      }
      val dec2_value = new Bundle {
        val inst2rs1_value = Input(UInt(32.W))
        val inst2rs2_value = Input(UInt(32.W))
      }
    }

    // 跨包 RAW 转发/阻塞来源:
    //   exu = K=1 (ALU/CSR 结果本拍可得; load 则数据没好 -> 阻塞)
    //   lsu = K=2 (含 load 数据)
    //   wbu = K=3 (M/W 寄存器的写回值)
    val fwd = new Bundle {
      val exu_rd      = Input(Vec(2, UInt(5.W)))
      val exu_valid   = Input(Vec(2, Bool()))
      val exu_is_load = Input(Vec(2, Bool()))
      val exu_data    = Input(Vec(2, UInt(32.W)))

      val lsu_rd      = Input(Vec(2, UInt(5.W)))
      val lsu_valid   = Input(Vec(2, Bool()))
      val lsu_data    = Input(Vec(2, UInt(32.W)))

      val wbu_rd      = Input(Vec(2, UInt(5.W)))
      val wbu_valid   = Input(Vec(2, Bool()))
      val wbu_data    = Input(Vec(2, UInt(32.W)))
    }

    // 前端阻塞: K=1 且是 load(数据没好)
    val stall = Output(Bool())

    // 发往 EXU 的两条 lane
    val idu_to_exu1 = Decoupled(new IDU_to_EXU_Lane_Message)
    val idu_to_exu2 = Decoupled(new IDU_to_EXU_Lane_Message)

    // 发往 IFU 的 stall 信号(单发射重定向)
    val idu_to_ifu = new Bundle {
      val is_stall = Output(Bool())
    }

    // 调试端口
    val idu_debug = new Bundle {
      val debug_inst1 = Output(UInt(32.W))
      val debug_inst2 = Output(UInt(32.W))
      val is_stall    = Output(Bool())
      val reason      = Output(UInt(4.W))  // [0]raw [1]is_control1 [2]mem [3]stall_sig2
    }
  })

  // 上游(IF/D 桥)交出后本拍即可消费
  io.idu1_to_idu2.ready := true.B
  val msg = io.idu1_to_idu2.bits
  val in_valid = io.idu1_to_idu2.valid

  // ---------- GRF 读地址 ----------
  io.idu_to_grf.dec1_redreg.rs1 := msg.lane1.rs1_addr
  io.idu_to_grf.dec1_redreg.rs2 := msg.lane1.rs2_addr
  io.idu_to_grf.dec2_redreg.rs1 := msg.lane2.rs1_addr
  io.idu_to_grf.dec2_redreg.rs2 := msg.lane2.rs2_addr

  // ---------- 跨包 RAW 转发 / 阻塞 ----------
  val src_regs = VecInit(msg.lane1.rs1_addr, msg.lane1.rs2_addr,
                         msg.lane2.rs1_addr, msg.lane2.rs2_addr)
  val grf_vals = VecInit(io.grf_to_idu.dec1_value.inst1rs1_value,
                         io.grf_to_idu.dec1_value.inst1rs2_value,
                         io.grf_to_idu.dec2_value.inst2rs1_value,
                         io.grf_to_idu.dec2_value.inst2rs2_value)

  // K=1 load-use 已由 exu.scala 的 MEM→EX bypass 在操作数口解决, 不再阻塞;
  // 只有"包内依赖 rs1 值的内存冒险"(ramraw/rambank)在 IDU2 用旧值会判错, 这时才保守阻塞
  val match_exu_load = (0 until 2).map(i =>
    io.fwd.exu_valid(i) && io.fwd.exu_is_load(i) && (io.fwd.exu_rd(i) =/= 0.U) &&
    src_regs.map(_ === io.fwd.exu_rd(i)).reduce(_ || _))
  io.stall := in_valid && match_exu_load.reduce(_ || _)

  // 转发优先级: EXU(K=1, 非 load) > LSU(K=2) > WBU(K=3); 同组内 lane2(更年轻) 优先
  def forward(src: UInt, grf_val: UInt): UInt = {
    val hit_exu = (0 until 2).map(i => io.fwd.exu_valid(i) && !io.fwd.exu_is_load(i) &&
                                      (io.fwd.exu_rd(i) =/= 0.U) && (src === io.fwd.exu_rd(i)))
    val hit_lsu = (0 until 2).map(i => io.fwd.lsu_valid(i) &&
                                      (io.fwd.lsu_rd(i) =/= 0.U) && (src === io.fwd.lsu_rd(i)))
    val hit_wbu = (0 until 2).map(i => io.fwd.wbu_valid(i) &&
                                      (io.fwd.wbu_rd(i) =/= 0.U) && (src === io.fwd.wbu_rd(i)))
    val sel_exu = Mux(hit_exu(1), io.fwd.exu_data(1), io.fwd.exu_data(0))
    val sel_lsu = Mux(hit_lsu(1), io.fwd.lsu_data(1), io.fwd.lsu_data(0))
    val sel_wbu = Mux(hit_wbu(1), io.fwd.wbu_data(1), io.fwd.wbu_data(0))
    Mux(hit_exu.reduce(_ || _), sel_exu,
    Mux(hit_lsu.reduce(_ || _), sel_lsu,
    Mux(hit_wbu.reduce(_ || _), sel_wbu, grf_val)))
  }
  val v1_1 = forward(msg.lane1.rs1_addr, grf_vals(0))
  val v1_2 = forward(msg.lane1.rs2_addr, grf_vals(1))
  val v2_1 = forward(msg.lane2.rs1_addr, grf_vals(2))
  val v2_2 = forward(msg.lane2.rs2_addr, grf_vals(3))

  // ---------- 冒险检测(用转发后的值) ----------
  val ramraw = (msg.is_store1 && msg.is_load2) &&
               (v1_1 === v2_1) &&
               (msg.lane1.imm === msg.lane2.imm)
  val rambank_conflict = (msg.is_load1 && msg.is_load2) &&
               ((msg.lane1.imm(0) ^ v1_1(0)) === (msg.lane2.imm(0) ^ v2_1(0)))
  val ramwaw = msg.is_store1 && msg.is_store2

  // stall 条件(包内)
  // raw 中若 lane1 是 load(数据没好) 只能单发射; 否则包内前递(exu1 结果 -> exu2 操作数)解决, 不单发射
  val lane1_is_load = msg.lane1.op.is_lb || msg.lane1.op.is_lh || msg.lane1.op.is_lw ||
                      msg.lane1.op.is_lbu || msg.lane1.op.is_lhu
  val raw_need_stall = msg.raw && lane1_is_load
  // lane1 的"向前条件分支"预测 not-taken, lane2 在正确路径上 -> 可以并行发射
  // (若实际跳了, exu1 重定向, 同时 exu2 收到 kill 把 lane2 写回打掉)
  val l1_is_branch = msg.lane1.op.is_beq || msg.lane1.op.is_bne || msg.lane1.op.is_blt ||
                     msg.lane1.op.is_bge || msg.lane1.op.is_bltu || msg.lane1.op.is_bgeu
  val l1_forward_branch = l1_is_branch && !msg.lane1.imm(31)
  val ctl1_need_single = msg.is_control1 && !l1_forward_branch
  val stall_sig = raw_need_stall || ctl1_need_single || ramraw || ramwaw || rambank_conflict

  // lane2 的 CSR/异常/mret 仍需单发射(侧效应只在 exu1 处理);
  // lane2 的分支/跳转可以并行执行, 由 exu2 重定向(不再"先单发射再重取"浪费两次)
  val l2_is_system = msg.lane2.op.is_csrrw || msg.lane2.op.is_csrrs || msg.lane2.op.is_csrrc ||
                     msg.lane2.op.is_csrrwi || msg.lane2.op.is_csrrsi || msg.lane2.op.is_csrrci ||
                     msg.lane2.op.is_ecall || msg.lane2.op.is_mret || msg.lane2.op.is_ebreak
  val stall_sig2 = l2_is_system
  val final_stall = stall_sig || stall_sig2

  io.idu_to_ifu.is_stall := final_stall

  // ---------- lane1 输出（直通 + 转发值; is_stall 用 final_stall 覆盖）----------
  io.idu_to_exu1.bits := msg.lane1
  io.idu_to_exu1.bits.rs1_val  := v1_1
  io.idu_to_exu1.bits.rs2_val  := v1_2
  io.idu_to_exu1.bits.is_stall := final_stall
  io.idu_to_exu1.valid := in_valid && !io.stall

  // ---------- lane2 输出（stall 时全部清零）----------
  io.idu_to_exu2.bits := msg.lane2
  io.idu_to_exu2.bits.op.is_lui    := Mux(final_stall, false.B, msg.lane2.op.is_lui)
  io.idu_to_exu2.bits.op.is_auipc  := Mux(final_stall, false.B, msg.lane2.op.is_auipc)
  io.idu_to_exu2.bits.op.is_jal    := Mux(final_stall, false.B, msg.lane2.op.is_jal)
  io.idu_to_exu2.bits.op.is_jalr   := Mux(final_stall, false.B, msg.lane2.op.is_jalr)
  io.idu_to_exu2.bits.op.is_beq    := Mux(final_stall, false.B, msg.lane2.op.is_beq)
  io.idu_to_exu2.bits.op.is_bne    := Mux(final_stall, false.B, msg.lane2.op.is_bne)
  io.idu_to_exu2.bits.op.is_blt    := Mux(final_stall, false.B, msg.lane2.op.is_blt)
  io.idu_to_exu2.bits.op.is_bge    := Mux(final_stall, false.B, msg.lane2.op.is_bge)
  io.idu_to_exu2.bits.op.is_bltu   := Mux(final_stall, false.B, msg.lane2.op.is_bltu)
  io.idu_to_exu2.bits.op.is_bgeu   := Mux(final_stall, false.B, msg.lane2.op.is_bgeu)
  io.idu_to_exu2.bits.op.is_lb     := Mux(final_stall, false.B, msg.lane2.op.is_lb)
  io.idu_to_exu2.bits.op.is_lh     := Mux(final_stall, false.B, msg.lane2.op.is_lh)
  io.idu_to_exu2.bits.op.is_lw     := Mux(final_stall, false.B, msg.lane2.op.is_lw)
  io.idu_to_exu2.bits.op.is_lbu    := Mux(final_stall, false.B, msg.lane2.op.is_lbu)
  io.idu_to_exu2.bits.op.is_lhu    := Mux(final_stall, false.B, msg.lane2.op.is_lhu)
  io.idu_to_exu2.bits.op.is_sb     := Mux(final_stall, false.B, msg.lane2.op.is_sb)
  io.idu_to_exu2.bits.op.is_sh     := Mux(final_stall, false.B, msg.lane2.op.is_sh)
  io.idu_to_exu2.bits.op.is_sw     := Mux(final_stall, false.B, msg.lane2.op.is_sw)
  io.idu_to_exu2.bits.op.is_addi   := Mux(final_stall, false.B, msg.lane2.op.is_addi)
  io.idu_to_exu2.bits.op.is_slti   := Mux(final_stall, false.B, msg.lane2.op.is_slti)
  io.idu_to_exu2.bits.op.is_sltiu  := Mux(final_stall, false.B, msg.lane2.op.is_sltiu)
  io.idu_to_exu2.bits.op.is_xori   := Mux(final_stall, false.B, msg.lane2.op.is_xori)
  io.idu_to_exu2.bits.op.is_ori    := Mux(final_stall, false.B, msg.lane2.op.is_ori)
  io.idu_to_exu2.bits.op.is_andi   := Mux(final_stall, false.B, msg.lane2.op.is_andi)
  io.idu_to_exu2.bits.op.is_slli   := Mux(final_stall, false.B, msg.lane2.op.is_slli)
  io.idu_to_exu2.bits.op.is_srli   := Mux(final_stall, false.B, msg.lane2.op.is_srli)
  io.idu_to_exu2.bits.op.is_srai   := Mux(final_stall, false.B, msg.lane2.op.is_srai)
  io.idu_to_exu2.bits.op.is_add    := Mux(final_stall, false.B, msg.lane2.op.is_add)
  io.idu_to_exu2.bits.op.is_sub    := Mux(final_stall, false.B, msg.lane2.op.is_sub)
  io.idu_to_exu2.bits.op.is_sll    := Mux(final_stall, false.B, msg.lane2.op.is_sll)
  io.idu_to_exu2.bits.op.is_slt    := Mux(final_stall, false.B, msg.lane2.op.is_slt)
  io.idu_to_exu2.bits.op.is_sltu   := Mux(final_stall, false.B, msg.lane2.op.is_sltu)
  io.idu_to_exu2.bits.op.is_xor    := Mux(final_stall, false.B, msg.lane2.op.is_xor)
  io.idu_to_exu2.bits.op.is_srl    := Mux(final_stall, false.B, msg.lane2.op.is_srl)
  io.idu_to_exu2.bits.op.is_sra    := Mux(final_stall, false.B, msg.lane2.op.is_sra)
  io.idu_to_exu2.bits.op.is_or     := Mux(final_stall, false.B, msg.lane2.op.is_or)
  io.idu_to_exu2.bits.op.is_and    := Mux(final_stall, false.B, msg.lane2.op.is_and)
  io.idu_to_exu2.bits.op.is_ebreak := Mux(final_stall, false.B, msg.lane2.op.is_ebreak)
  io.idu_to_exu2.bits.op.is_csrrw  := Mux(final_stall, false.B, msg.lane2.op.is_csrrw)
  io.idu_to_exu2.bits.op.is_csrrs  := Mux(final_stall, false.B, msg.lane2.op.is_csrrs)
  io.idu_to_exu2.bits.op.is_csrrc  := Mux(final_stall, false.B, msg.lane2.op.is_csrrc)
  io.idu_to_exu2.bits.op.is_csrrwi := Mux(final_stall, false.B, msg.lane2.op.is_csrrwi)
  io.idu_to_exu2.bits.op.is_csrrsi := Mux(final_stall, false.B, msg.lane2.op.is_csrrsi)
  io.idu_to_exu2.bits.op.is_csrrci := Mux(final_stall, false.B, msg.lane2.op.is_csrrci)
  io.idu_to_exu2.bits.op.is_ecall  := Mux(final_stall, false.B, msg.lane2.op.is_ecall)
  io.idu_to_exu2.bits.op.is_mret   := Mux(final_stall, false.B, msg.lane2.op.is_mret)
  io.idu_to_exu2.bits.imm      := Mux(final_stall, 0.U(32.W), msg.lane2.imm)
  io.idu_to_exu2.bits.rs1_val  := Mux(final_stall, 0.U(32.W), v2_1)
  io.idu_to_exu2.bits.rs2_val  := Mux(final_stall, 0.U(32.W), v2_2)
  io.idu_to_exu2.bits.pc       := Mux(final_stall, 0.U(32.W), msg.lane2.pc)
  io.idu_to_exu2.bits.rd       := Mux(final_stall, 0.U(5.W), msg.lane2.rd)
  io.idu_to_exu2.bits.is_stall := false.B
  io.idu_to_exu2.bits.inst1_pc := 0.U
  io.idu_to_exu2.bits.inst1    := 0.U
  io.idu_to_exu2.valid := in_valid && !io.stall

  // ---------- 调试输出 ----------
  io.idu_debug.debug_inst1 := msg.lane1.inst1
  io.idu_debug.debug_inst2 := Mux(final_stall, 0.U(32.W), msg.debug_inst2)
  io.idu_debug.is_stall    := final_stall
  io.idu_debug.reason      := Cat(stall_sig2, (ramraw || ramwaw || rambank_conflict),
                                  msg.is_control1, msg.raw)
}
//====================================新增: IDU2 级(GRF 读 + 转发布塞 + 冒险检测 + lane 打包)====================================

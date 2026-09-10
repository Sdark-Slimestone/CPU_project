package R1322IAeCSR

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

// ==================================================================
// 【多周期新增区】顶层总装图
//  - core/ 下为 R1322IAe-csr 原版模块的逐字节镜像, 在此直接连线
//    (与原版单周期 top 的接线同构: exu1/exu2、lsu1/lsu2 双实例,
//     exu2 的 dec1 端口喂 dec2 译码字段, CSR 相关端口恒 0)
//  - 新部件 BusBridge 插在原版组合部件之间构成五级流水站:
//      core/ifu(pc_en 门控 5 拍更新一次) -> [F/D桥] -> core/idu -> [D/E桥] -> core/exu
//                                      -> [E/M桥] -> core/lsu -> [M/W桥] -> core/wbu
//    取指节奏/重定向锁存/单发射保持由 ctrl.scala 的新部件提供, 无独立 IFU stage
//    F/D 桥不掩码(交出后保持, 供 debug 显示驻留包); 其余三桥掩码
//    (交出后清零, 使原版部件空闲拍无副作用, 副作用恰好一次)
//  - 握手管线: 桥之间隔着原版组合部件, 反压/有效信号由 top 跨接
//      (如 fd.out.ready := de.in.ready)
//  - 顶层 debug 端口与原版完全一致, cppfile 无需改动
// ==================================================================

// 双端口黑盒存储器，通过 DPI-C 与 C++ 交互
class DPIMemory extends BlackBox {
  val io = IO(new Bundle {
    val io_clk = Input(Clock())
    val wen    = Input(Bool())
    val waddr  = Input(UInt(32.W))
    val wdata  = Input(UInt(32.W))
    val wmask  = Input(UInt(4.W))
    val ren1   = Input(Bool())
    val raddr1 = Input(UInt(32.W))
    val rdata1 = Output(UInt(32.W))
    val ren2   = Input(Bool())
    val raddr2 = Input(UInt(32.W))
    val rdata2 = Output(UInt(32.W))
    val ebreak = Input(Bool())
  })
  override def desiredName = "DPI_Memory"
}

class top extends Module {
  val io = IO(new Bundle {
    // ---------- IFU/IDU debug ----------
    val debug_inst1_pc = Output(UInt(32.W))
    val debug_inst2_pc = Output(UInt(32.W))

    val debug_inst1 = Output(UInt(32.W))
    val debug_inst2 = Output(UInt(32.W))
    val debug_stall = Output(Bool())

    // ---------- EXU1 debug ----------
    val debug_exu1_alu_out     = Output(UInt(32.W))
    val debug_exu1_alu_source1 = Output(UInt(32.W))
    val debug_exu1_alu_source2 = Output(UInt(32.W))
    val debug_exu1_agu_out     = Output(UInt(32.W))

    // ---------- EXU2 debug ----------
    val debug_exu2_alu_out     = Output(UInt(32.W))
    val debug_exu2_alu_source1 = Output(UInt(32.W))
    val debug_exu2_alu_source2 = Output(UInt(32.W))
    val debug_exu2_agu_out     = Output(UInt(32.W))

    // ---------- LSU1 debug ----------
    val debug_lsu1_is_load          = Output(Bool())
    val debug_lsu1_is_store         = Output(Bool())
    val debug_lsu1_addr             = Output(UInt(32.W))
    val debug_lsu1_read_origin      = Output(UInt(32.W))
    val debug_lsu1_final_wb_data    = Output(UInt(32.W))
    val debug_lsu1_store_mask       = Output(UInt(4.W))
    val debug_lsu1_store_data_shifted = Output(UInt(32.W))

    // ---------- LSU2 debug ----------
    val debug_lsu2_is_load          = Output(Bool())
    val debug_lsu2_is_store         = Output(Bool())
    val debug_lsu2_addr             = Output(UInt(32.W))
    val debug_lsu2_read_origin      = Output(UInt(32.W))
    val debug_lsu2_final_wb_data    = Output(UInt(32.W))
    val debug_lsu2_store_mask       = Output(UInt(4.W))
    val debug_lsu2_store_data_shifted = Output(UInt(32.W))

    // ---------- WBU debug ----------
    val debug_wbu_valid1   = Output(Bool())
    val debug_wbu_valid2   = Output(Bool())
    val debug_wbu_conflict = Output(Bool())
    val debug_wbu_rd1      = Output(UInt(5.W))
    val debug_wbu_rd2      = Output(UInt(5.W))
    val debug_wbu_wr1_addr = Output(UInt(5.W))
    val debug_wbu_wr2_addr = Output(UInt(5.W))

    // ---------- GRF debug ----------
    val debug_grf_regs   = Output(Vec(16, UInt(32.W)))
    val debug_grf_rden   = Output(Bool())
    val debug_grf_rdaddr = Output(UInt(5.W))
    val debug_grf_input  = Output(UInt(32.W))

    // ---------- CSR debug ----------
    val debug_mcycle    = Output(UInt(64.W))
    val debug_minstret  = Output(UInt(64.W))
    val debug_mstatus   = Output(UInt(32.W))
    val debug_mie       = Output(UInt(32.W))
    val debug_mtvec     = Output(UInt(32.W))
    val debug_mepc      = Output(UInt(32.W))
    val debug_mcause    = Output(UInt(32.W))
    val debug_mtval     = Output(UInt(32.W))
    val debug_mip       = Output(UInt(32.W))
    val debug_mscratch  = Output(UInt(32.W))
    val debug_mvendorid = Output(UInt(32.W))
    val debug_marchid   = Output(UInt(32.W))
    val debug_mimpid    = Output(UInt(32.W))
    val debug_mhartid   = Output(UInt(32.W))
  })

  // ========== 原版模块(core/ 逐字节镜像) ==========
  val imem      = Module(new imem)
  val dmem      = Module(new dmem)
  val grf       = Module(new GRF)
  val core_idu  = Module(new idu)
  val exu_core1 = Module(new EXU)
  val exu_core2 = Module(new EXU)
  val lsu_core1 = Module(new LSU)
  val lsu_core2 = Module(new LSU)
  val wbu_core  = Module(new wbu)

  // ========== 新部件(多周期) ==========
  val core_ifu    = Module(new IFU)
  val fetch_gate  = Module(new FetchGate)
  val redirect_latch = Module(new RedirectLatch)
  val fd_bridge   = Module(new BusBridge(new IFUToIDUMessage, maskOnHandoff = false))
  val de_bridge   = Module(new BusBridge(new IDUToEXUMessage))
  val em_bridge   = Module(new BusBridge(new EXUToLSUMessage))
  val mw_bridge   = Module(new BusBridge(new LSUToWBUMessage))
  val retire      = Module(new RetireCounter)
  val stall_holder = Module(new StallHolder)

  // ===================== IFU ↔ IMEM =====================
  core_ifu.io.ifu_to_imem <> imem.io.ifu_to_imem
  core_ifu.io.imem_to_ifu <> imem.io.imem_to_ifu

  // ===================== core/ifu -> [F/D桥] -> core/idu =====================
  fetch_gate.io.accepted := fd_bridge.io.in.fire
  fetch_gate.io.done     := mw_bridge.io.out.valid
  core_ifu.io.pc_en      := mw_bridge.io.out.valid          // 包完成拍: pc 更新一次

  fd_bridge.io.in.valid  := fetch_gate.io.present         // 被接收后到 done 前保持关闭
  fd_bridge.io.in.bits   := core_ifu.io.ifu_to_idu        // 取指包 -> 原版 idu 输入
  core_idu.io.ifu_to_idu := fd_bridge.io.out.bits           // 取指包 -> 原版 idu 输入
  fd_bridge.io.out.ready := de_bridge.io.in.ready         // 反压跨过组合部件
  de_bridge.io.in.valid  := fd_bridge.io.out.valid

  // ===================== core/idu -> [D/E桥] -> core/exu =====================
  de_bridge.io.in.bits.dec1_op      := core_idu.io.idu_to_exu1.dec1_op
  de_bridge.io.in.bits.dec1_imm     := core_idu.io.idu_to_exu1.dec1_imm
  de_bridge.io.in.bits.dec1_rs1_val := core_idu.io.idu_to_exu1.dec1_val.rs1_val
  de_bridge.io.in.bits.dec1_rs2_val := core_idu.io.idu_to_exu1.dec1_val.rs2_val
  de_bridge.io.in.bits.dec1_nextpc  := core_idu.io.idu_to_exu1.dec1_val.nextpc
  de_bridge.io.in.bits.dec1_rd      := core_idu.io.idu_to_exu1.dec1_rd
  de_bridge.io.in.bits.dec2_op      := core_idu.io.idu_to_exu2.dec2_op
  de_bridge.io.in.bits.dec2_imm     := core_idu.io.idu_to_exu2.dec2_imm
  de_bridge.io.in.bits.dec2_rs1_val := core_idu.io.idu_to_exu2.dec2_val.rs1_val
  de_bridge.io.in.bits.dec2_rs2_val := core_idu.io.idu_to_exu2.dec2_val.rs2_val
  de_bridge.io.in.bits.dec2_nextpc  := core_idu.io.idu_to_exu2.dec2_val.nextpc
  de_bridge.io.in.bits.dec2_rd      := core_idu.io.idu_to_exu2.dec2_rd
  de_bridge.io.in.bits.is_stall     := core_idu.io.idu_to_ifu.is_stall   // 该包的单发射标志(交接拍组合有效)
  de_bridge.io.in.bits.inst1_pc     := core_idu.io.idu_to_exu1.inst1_pc
  de_bridge.io.in.bits.inst1        := core_idu.io.idu_to_exu1.inst1
  de_bridge.io.out.ready := em_bridge.io.in.ready
  em_bridge.io.in.valid  := de_bridge.io.out.valid

  // ----- 桥消息 -> 原版 EXU×2 (与原版 top 的 exu1/exu2 接线同构) -----
  exu_core1.io.idu_to_exu1.dec1_op          := de_bridge.io.out.bits.dec1_op
  exu_core1.io.idu_to_exu1.dec1_imm         := de_bridge.io.out.bits.dec1_imm
  exu_core1.io.idu_to_exu1.dec1_val.rs1_val := de_bridge.io.out.bits.dec1_rs1_val
  exu_core1.io.idu_to_exu1.dec1_val.rs2_val := de_bridge.io.out.bits.dec1_rs2_val
  exu_core1.io.idu_to_exu1.dec1_val.nextpc  := de_bridge.io.out.bits.dec1_nextpc
  exu_core1.io.idu_to_exu1.dec1_rd          := de_bridge.io.out.bits.dec1_rd
  exu_core1.io.idu_to_exu1.is_stall         := de_bridge.io.out.bits.is_stall
  exu_core1.io.idu_to_exu1.inst1_pc         := de_bridge.io.out.bits.inst1_pc
  exu_core1.io.idu_to_exu1.inst1            := de_bridge.io.out.bits.inst1

  exu_core2.io.idu_to_exu1.dec1_op          := de_bridge.io.out.bits.dec2_op
  exu_core2.io.idu_to_exu1.dec1_imm         := de_bridge.io.out.bits.dec2_imm
  exu_core2.io.idu_to_exu1.dec1_val.rs1_val := de_bridge.io.out.bits.dec2_rs1_val
  exu_core2.io.idu_to_exu1.dec1_val.rs2_val := de_bridge.io.out.bits.dec2_rs2_val
  exu_core2.io.idu_to_exu1.dec1_val.nextpc  := de_bridge.io.out.bits.dec2_nextpc
  exu_core2.io.idu_to_exu1.dec1_rd          := de_bridge.io.out.bits.dec2_rd
  exu_core2.io.idu_to_exu1.is_stall         := false.B
  exu_core2.io.idu_to_exu1.inst1_pc         := 0.U
  exu_core2.io.idu_to_exu1.inst1            := 0.U

  // ===================== core/exu -> [E/M桥] -> core/lsu =====================
  em_bridge.io.in.bits.op1.is_lb   := exu_core1.io.exu_to_lsu.op.is_lb
  em_bridge.io.in.bits.op1.is_lh   := exu_core1.io.exu_to_lsu.op.is_lh
  em_bridge.io.in.bits.op1.is_lw   := exu_core1.io.exu_to_lsu.op.is_lw
  em_bridge.io.in.bits.op1.is_lbu  := exu_core1.io.exu_to_lsu.op.is_lbu
  em_bridge.io.in.bits.op1.is_lhu  := exu_core1.io.exu_to_lsu.op.is_lhu
  em_bridge.io.in.bits.op1.is_sb   := exu_core1.io.exu_to_lsu.op.is_sb
  em_bridge.io.in.bits.op1.is_sh   := exu_core1.io.exu_to_lsu.op.is_sh
  em_bridge.io.in.bits.op1.is_sw   := exu_core1.io.exu_to_lsu.op.is_sw
  em_bridge.io.in.bits.addr1       := exu_core1.io.exu_to_lsu.paddr.addr
  em_bridge.io.in.bits.store_data1 := exu_core1.io.exu_to_lsu.data.store_data
  em_bridge.io.in.bits.rd1         := exu_core1.io.exu_to_lsu.exu_through_lsu_to_wbu.rd
  em_bridge.io.in.bits.wb_data1    := exu_core1.io.exu_to_lsu.exu_through_lsu_to_wbu.grf_wb_data

  em_bridge.io.in.bits.op2.is_lb   := exu_core2.io.exu_to_lsu.op.is_lb
  em_bridge.io.in.bits.op2.is_lh   := exu_core2.io.exu_to_lsu.op.is_lh
  em_bridge.io.in.bits.op2.is_lw   := exu_core2.io.exu_to_lsu.op.is_lw
  em_bridge.io.in.bits.op2.is_lbu  := exu_core2.io.exu_to_lsu.op.is_lbu
  em_bridge.io.in.bits.op2.is_lhu  := exu_core2.io.exu_to_lsu.op.is_lhu
  em_bridge.io.in.bits.op2.is_sb   := exu_core2.io.exu_to_lsu.op.is_sb
  em_bridge.io.in.bits.op2.is_sh   := exu_core2.io.exu_to_lsu.op.is_sh
  em_bridge.io.in.bits.op2.is_sw   := exu_core2.io.exu_to_lsu.op.is_sw
  em_bridge.io.in.bits.addr2       := exu_core2.io.exu_to_lsu.paddr.addr
  em_bridge.io.in.bits.store_data2 := exu_core2.io.exu_to_lsu.data.store_data
  em_bridge.io.in.bits.rd2         := exu_core2.io.exu_to_lsu.exu_through_lsu_to_wbu.rd
  em_bridge.io.in.bits.wb_data2    := exu_core2.io.exu_to_lsu.exu_through_lsu_to_wbu.grf_wb_data

  // ebreak 恒在 lane1（lane2 的控制类指令已被 IDU 单发射清零）
  em_bridge.io.in.bits.is_ebreak := exu_core1.io.exu_to_lsu.op.is_ebreak

  em_bridge.io.out.ready := mw_bridge.io.in.ready
  mw_bridge.io.in.valid  := em_bridge.io.out.valid

  // ----- 桥消息 -> 原版 LSU×2 -----
  lsu_core1.io.exu_to_lsu.op.is_lb     := em_bridge.io.out.bits.op1.is_lb
  lsu_core1.io.exu_to_lsu.op.is_lh     := em_bridge.io.out.bits.op1.is_lh
  lsu_core1.io.exu_to_lsu.op.is_lw     := em_bridge.io.out.bits.op1.is_lw
  lsu_core1.io.exu_to_lsu.op.is_lbu    := em_bridge.io.out.bits.op1.is_lbu
  lsu_core1.io.exu_to_lsu.op.is_lhu    := em_bridge.io.out.bits.op1.is_lhu
  lsu_core1.io.exu_to_lsu.op.is_sb     := em_bridge.io.out.bits.op1.is_sb
  lsu_core1.io.exu_to_lsu.op.is_sh     := em_bridge.io.out.bits.op1.is_sh
  lsu_core1.io.exu_to_lsu.op.is_sw     := em_bridge.io.out.bits.op1.is_sw
  lsu_core1.io.exu_to_lsu.op.is_ebreak := em_bridge.io.out.bits.is_ebreak
  lsu_core1.io.exu_to_lsu.paddr.addr   := em_bridge.io.out.bits.addr1
  lsu_core1.io.exu_to_lsu.data.store_data := em_bridge.io.out.bits.store_data1
  lsu_core1.io.exu_to_lsu.exu_through_lsu_to_wbu.rd         := em_bridge.io.out.bits.rd1
  lsu_core1.io.exu_to_lsu.exu_through_lsu_to_wbu.grf_wb_data := em_bridge.io.out.bits.wb_data1

  lsu_core2.io.exu_to_lsu.op.is_lb     := em_bridge.io.out.bits.op2.is_lb
  lsu_core2.io.exu_to_lsu.op.is_lh     := em_bridge.io.out.bits.op2.is_lh
  lsu_core2.io.exu_to_lsu.op.is_lw     := em_bridge.io.out.bits.op2.is_lw
  lsu_core2.io.exu_to_lsu.op.is_lbu    := em_bridge.io.out.bits.op2.is_lbu
  lsu_core2.io.exu_to_lsu.op.is_lhu    := em_bridge.io.out.bits.op2.is_lhu
  lsu_core2.io.exu_to_lsu.op.is_sb     := em_bridge.io.out.bits.op2.is_sb
  lsu_core2.io.exu_to_lsu.op.is_sh     := em_bridge.io.out.bits.op2.is_sh
  lsu_core2.io.exu_to_lsu.op.is_sw     := em_bridge.io.out.bits.op2.is_sw
  lsu_core2.io.exu_to_lsu.op.is_ebreak := false.B
  lsu_core2.io.exu_to_lsu.paddr.addr   := em_bridge.io.out.bits.addr2
  lsu_core2.io.exu_to_lsu.data.store_data := em_bridge.io.out.bits.store_data2
  lsu_core2.io.exu_to_lsu.exu_through_lsu_to_wbu.rd         := em_bridge.io.out.bits.rd2
  lsu_core2.io.exu_to_lsu.exu_through_lsu_to_wbu.grf_wb_data := em_bridge.io.out.bits.wb_data2

  // ===================== core/lsu ↔ DMEM =====================
  dmem.io.lsu_to_dmem_1.addr       := lsu_core1.io.lsu_to_dmem.addr
  dmem.io.lsu_to_dmem_1.store_data := lsu_core1.io.lsu_to_dmem.store_data
  dmem.io.lsu_to_dmem_1.mask       := lsu_core1.io.lsu_to_dmem.mask
  dmem.io.lsu_to_dmem_1.wen        := lsu_core1.io.lsu_to_dmem.wen
  dmem.io.lsu_to_dmem_1.ren        := lsu_core1.io.lsu_to_dmem.ren
  dmem.io.lsu_to_dmem_2.addr       := lsu_core2.io.lsu_to_dmem.addr
  dmem.io.lsu_to_dmem_2.store_data := lsu_core2.io.lsu_to_dmem.store_data
  dmem.io.lsu_to_dmem_2.mask       := lsu_core2.io.lsu_to_dmem.mask
  dmem.io.lsu_to_dmem_2.wen        := lsu_core2.io.lsu_to_dmem.wen
  dmem.io.lsu_to_dmem_2.ren        := lsu_core2.io.lsu_to_dmem.ren
  lsu_core1.io.dmem_to_lsu.load_data := dmem.io.dmem_to_lsu_1.load_data
  lsu_core2.io.dmem_to_lsu.load_data := dmem.io.dmem_to_lsu_2.load_data
  dmem.io.ebreak := lsu_core1.io.ebreak_out || lsu_core2.io.ebreak_out

  // ===================== core/lsu -> [M/W桥] -> core/wbu =====================
  mw_bridge.io.in.bits.rd1      := lsu_core1.io.lsu_to_wbu.rd
  mw_bridge.io.in.bits.wb_data1 := lsu_core1.io.lsu_to_wbu.grf_wb_data
  mw_bridge.io.in.bits.rd2      := lsu_core2.io.lsu_to_wbu.rd
  mw_bridge.io.in.bits.wb_data2 := lsu_core2.io.lsu_to_wbu.grf_wb_data

  mw_bridge.io.out.ready := true.B               // 末端: 自动消费, valid 恰好一拍
  wbu_core.io.lsu_to_wbu_1.rd          := mw_bridge.io.out.bits.rd1
  wbu_core.io.lsu_to_wbu_1.grf_wb_data := mw_bridge.io.out.bits.wb_data1
  wbu_core.io.lsu_to_wbu_2.rd          := mw_bridge.io.out.bits.rd2
  wbu_core.io.lsu_to_wbu_2.grf_wb_data := mw_bridge.io.out.bits.wb_data2

  // ===================== WBU → GRF / IDU ↔ GRF =====================
  grf.io.wbu_to_grf <> wbu_core.io.wbu_to_grf
  grf.io.idu_to_grf <> core_idu.io.idu_to_grf
  core_idu.io.grf_to_idu <> grf.io.grf_to_idu

  // ===================== 重定向/流控回传 IFU（旁路导线, 非总线） =====================
  redirect_latch.io.take_trap     := exu_core1.io.csr_to_ifu.take_trap
  redirect_latch.io.trap_pc       := exu_core1.io.csr_to_ifu.trap_pc
  redirect_latch.io.take_mret     := exu_core1.io.csr_to_ifu.take_mret
  redirect_latch.io.mret_pc       := exu_core1.io.csr_to_ifu.mret_pc
  redirect_latch.io.take_branch   := exu_core1.io.exu_to_ifu.take_branch
  redirect_latch.io.branch_target := exu_core1.io.exu_to_ifu.branch_target
  redirect_latch.io.update        := mw_bridge.io.out.valid
  core_ifu.io.csr_to_ifu.take_trap   := redirect_latch.io.take_trap_out
  core_ifu.io.csr_to_ifu.trap_pc     := redirect_latch.io.trap_pc_out
  core_ifu.io.csr_to_ifu.take_mret   := redirect_latch.io.take_mret_out
  core_ifu.io.csr_to_ifu.mret_pc     := redirect_latch.io.mret_pc_out
  core_ifu.io.exu_to_ifu.take_branch   := redirect_latch.io.take_branch_out
  core_ifu.io.exu_to_ifu.branch_target := redirect_latch.io.branch_target_out
  stall_holder.io.in_fire  := de_bridge.io.in.fire
  stall_holder.io.in_stall := de_bridge.io.in.bits.is_stall
  core_ifu.io.idu_to_ifu.is_stall := stall_holder.io.is_stall

  // ===================== CSR 写回 GRF =====================
  grf.io.csr_to_grf.wen   := exu_core1.io.csr_to_grf.wen
  grf.io.csr_to_grf.waddr := exu_core1.io.csr_to_grf.waddr
  grf.io.csr_to_grf.wdata := exu_core1.io.csr_to_grf.wdata

  // ===================== 影子计数器 =====================
  retire.io.dec_valid := de_bridge.io.out.valid
  retire.io.dec_msg   := de_bridge.io.out.bits

  // ===================== 所有 debug 连接 =====================
  // --- IFU/IDU (显示当前在飞行的取指包; F/D 桥交出后保持, 驻留显示) ---
  io.debug_inst1_pc := core_ifu.io.debug.debug_inst1_pc
  io.debug_inst2_pc := core_ifu.io.debug.debug_inst2_pc
  io.debug_inst1    := fd_bridge.io.out.bits.inst1
  io.debug_inst2    := Mux(stall_holder.io.is_stall, 0.U(32.W), fd_bridge.io.out.bits.inst2)
  io.debug_stall    := stall_holder.io.is_stall

  // --- EXU1 ---
  io.debug_exu1_alu_out     := exu_core1.io.debug_alu_out
  io.debug_exu1_alu_source1 := exu_core1.io.debug_alu_source1
  io.debug_exu1_alu_source2 := exu_core1.io.debug_alu_source2
  io.debug_exu1_agu_out     := exu_core1.io.debug_agu_out

  // --- EXU2 ---
  io.debug_exu2_alu_out     := exu_core2.io.debug_alu_out
  io.debug_exu2_alu_source1 := exu_core2.io.debug_alu_source1
  io.debug_exu2_alu_source2 := exu_core2.io.debug_alu_source2
  io.debug_exu2_agu_out     := exu_core2.io.debug_agu_out

  // --- LSU1 ---
  io.debug_lsu1_is_load            := lsu_core1.io.debug.is_load
  io.debug_lsu1_is_store           := lsu_core1.io.debug.is_store
  io.debug_lsu1_addr               := lsu_core1.io.debug.addr
  io.debug_lsu1_read_origin        := lsu_core1.io.debug.read_origin
  io.debug_lsu1_final_wb_data      := lsu_core1.io.debug.final_wb_data
  io.debug_lsu1_store_mask         := lsu_core1.io.debug.store_mask
  io.debug_lsu1_store_data_shifted := lsu_core1.io.debug.store_data_shifted

  // --- LSU2 ---
  io.debug_lsu2_is_load            := lsu_core2.io.debug.is_load
  io.debug_lsu2_is_store           := lsu_core2.io.debug.is_store
  io.debug_lsu2_addr               := lsu_core2.io.debug.addr
  io.debug_lsu2_read_origin        := lsu_core2.io.debug.read_origin
  io.debug_lsu2_final_wb_data      := lsu_core2.io.debug.final_wb_data
  io.debug_lsu2_store_mask         := lsu_core2.io.debug.store_mask
  io.debug_lsu2_store_data_shifted := lsu_core2.io.debug.store_data_shifted

  // --- WBU ---
  io.debug_wbu_valid1   := wbu_core.io.debug.valid1
  io.debug_wbu_valid2   := wbu_core.io.debug.valid2
  io.debug_wbu_conflict := wbu_core.io.debug.conflict
  io.debug_wbu_rd1      := wbu_core.io.debug.rd1
  io.debug_wbu_rd2      := wbu_core.io.debug.rd2
  io.debug_wbu_wr1_addr := wbu_core.io.debug.wr1_addr
  io.debug_wbu_wr2_addr := wbu_core.io.debug.wr2_addr

  // --- GRF ---
  io.debug_grf_regs   := grf.io.debug_regs
  io.debug_grf_rden   := grf.io.debug_rden
  io.debug_grf_rdaddr := grf.io.debug_rdaddr
  io.debug_grf_input  := grf.io.debug_input

  // --- CSR (mcycle/minstret 取自影子计数器, 其余取自 core 原版寄存器) ---
  io.debug_mcycle    := retire.io.debug_mcycle
  io.debug_minstret  := retire.io.debug_minstret
  io.debug_mstatus   := exu_core1.io.debug_csr.mstatus
  io.debug_mie       := exu_core1.io.debug_csr.mie
  io.debug_mtvec     := exu_core1.io.debug_csr.mtvec
  io.debug_mepc      := exu_core1.io.debug_csr.mepc
  io.debug_mcause    := exu_core1.io.debug_csr.mcause
  io.debug_mtval     := exu_core1.io.debug_csr.mtval
  io.debug_mip       := exu_core1.io.debug_csr.mip
  io.debug_mscratch  := exu_core1.io.debug_csr.mscratch
  io.debug_mvendorid := exu_core1.io.debug_csr.mvendorid
  io.debug_marchid   := exu_core1.io.debug_csr.marchid
  io.debug_mimpid    := exu_core1.io.debug_csr.mimpid
  io.debug_mhartid   := exu_core1.io.debug_csr.mhartid
}

object top extends App {
  ChiselStage.emitSystemVerilogFile(
    new top,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}

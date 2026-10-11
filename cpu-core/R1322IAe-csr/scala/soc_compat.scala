package R1322IAeCSR

// ============================================================================
// R1322IAe-csr 的 SoC 兼容层 (独立文件, 不修改本体 RTL)
//
// 本体是**单周期、组合读内存**的核心 (imem/dmem 直接例化 DPI_Memory, 当拍就用)。
// ysyxSoC 的内存是多拍 AXI, 因此这里:
//   1) csr_axi_mem 把核心的"简单内存接口"翻译成 AXI4 master;
//   2) 时钟门控 gated = clock & ~busy (复位期间放行), 访存期间**冻结整个核心**;
//   3) ysyx_26030107 重新接线本体的子模块 (IFU/IDU/EXU/LSU/wbu/GRF, 均不改),
//      暴露 ysyxSoC 规范要求的 AXI4 CPU 接口。
//
// 时序要点: 核心的 dmem 请求 (含 store 数据) 是**指令与寄存器的组合逻辑**, 因此必须
//   先把本轮 im1/im2 取回并驱动给核心, 隔一拍 (等组合逻辑稳定) 再锁存 dmem 请求。
// ============================================================================

import chisel3._
import chisel3.util._
import chisel3.util.circt.ClockGate

// 复位 PC 配置 (兼容层用; 默认 0x8000_0000, 保持本体仿真/npc 行为不变)
object BootCfg {
  var resetPC: BigInt = BigInt("80000000", 16)
}

// ---- ysyxSoC/spec/cpu-interface.md 规定的 AXI4 端口 (顶层端口名 io_master_*) ----
class SocAXI4MasterIO extends Bundle {
  val awready = Input(Bool());  val awvalid = Output(Bool())
  val awid    = Output(UInt(4.W));  val awaddr = Output(UInt(32.W))
  val awlen   = Output(UInt(8.W));  val awsize = Output(UInt(3.W)); val awburst = Output(UInt(2.W))
  val wready  = Input(Bool());  val wvalid = Output(Bool())
  val wdata   = Output(UInt(32.W)); val wstrb = Output(UInt(4.W)); val wlast = Output(Bool())
  val bready  = Output(Bool()); val bvalid = Input(Bool()); val bid = Input(UInt(4.W)); val bresp = Input(UInt(2.W))
  val arready = Input(Bool());  val arvalid = Output(Bool())
  val arid    = Output(UInt(4.W)); val araddr = Output(UInt(32.W))
  val arlen   = Output(UInt(8.W)); val arsize = Output(UInt(3.W)); val arburst = Output(UInt(2.W))
  val rready  = Output(Bool()); val rvalid = Input(Bool()); val rid = Input(UInt(4.W))
  val rdata   = Input(UInt(32.W)); val rresp = Input(UInt(2.W)); val rlast = Input(Bool())
}
class SocAXI4SlaveIO extends Bundle {
  val awready = Output(Bool()); val awvalid = Input(Bool())
  val awid    = Input(UInt(4.W));  val awaddr = Input(UInt(32.W))
  val awlen   = Input(UInt(8.W));  val awsize = Input(UInt(3.W)); val awburst = Input(UInt(2.W))
  val wready  = Output(Bool()); val wvalid = Input(Bool())
  val wdata   = Input(UInt(32.W)); val wstrb = Input(UInt(4.W)); val wlast = Input(Bool())
  val bready  = Input(Bool());  val bvalid = Output(Bool()); val bid = Output(UInt(4.W)); val bresp = Output(UInt(2.W))
  val arready = Output(Bool()); val arvalid = Input(Bool())
  val arid    = Input(UInt(4.W));  val araddr = Input(UInt(32.W))
  val arlen   = Input(UInt(8.W));  val arsize = Input(UInt(3.W)); val arburst = Input(UInt(2.W))
  val rready  = Input(Bool());  val rvalid = Output(Bool()); val rid = Output(UInt(4.W))
  val rdata   = Output(UInt(32.W)); val rresp = Output(UInt(2.W)); val rlast = Output(Bool())
}

// ============================================================================
// AXI 内存桥 (面向核心=简单内存, 面向 SoC=AXI4 master)
// 每步: 取指读 a1/a2 -> 锁存 dmem 请求 -> 数据读 x2 -> (可选)写 -> CONSUME
// ============================================================================
class csr_axi_mem extends Module {
  val io = IO(new Bundle {
    val imem_a1 = Input(UInt(32.W)); val imem_a2 = Input(UInt(32.W))
    val imem_d1 = Output(UInt(32.W)); val imem_d2 = Output(UInt(32.W))
    val dr1_addr = Input(UInt(32.W)); val dr1_ren = Input(Bool()); val dr1_data = Output(UInt(32.W))
    val dr2_addr = Input(UInt(32.W)); val dr2_ren = Input(Bool()); val dr2_data = Output(UInt(32.W))
    val dr1_size = Input(UInt(2.W)); val dr2_size = Input(UInt(2.W))
    val dw_addr = Input(UInt(32.W)); val dw_wen = Input(Bool())
    val dw_data = Input(UInt(32.W)); val dw_mask = Input(UInt(4.W))
    val busy    = Output(Bool())
    val axi = new SocAXI4MasterIO
  })

  // 状态: 0=START,1=IM1,2=IM2,3=LATCH,4=DR1,5=DR2,6=DW,7=CONSUME
  val state = RegInit(0.U(3.W))
  val phase = RegInit(0.U(1.W))

  val a1 = Reg(UInt(32.W)); val a2 = Reg(UInt(32.W))
  val im1 = RegInit(0.U(32.W)); val im2 = RegInit(0.U(32.W))
  val rd1en = RegInit(false.B); val rd1a = Reg(UInt(32.W)); val rd1s = Reg(UInt(2.W))
  val rd2en = RegInit(false.B); val rd2a = Reg(UInt(32.W)); val rd2s = Reg(UInt(2.W))
  val wren  = RegInit(false.B); val wra  = Reg(UInt(32.W))
  val wrd   = Reg(UInt(32.W)); val wrm = Reg(UInt(4.W))
  val d1 = RegInit(0.U(32.W)); val d2 = RegInit(0.U(32.W))

  io.imem_d1 := im1; io.imem_d2 := im2
  io.dr1_data := d1; io.dr2_data := d2
  io.busy := state =/= 7.U

  val isRead  = state === 1.U || state === 2.U || state === 4.U || state === 5.U
  val isWrite = state === 6.U
  // 核心给出的是"字对齐地址 + byte lane 编在 mask 里"; AXI 窄写需要按字节地址:
  //   写地址 = 对齐地址 | (mask 最低有效字节的位置)
  val wrOff = MuxLookup(wrm, 0.U)(Seq(
    "b0010".U -> 1.U, "b0100".U -> 2.U, "b1000".U -> 3.U, "b1100".U -> 2.U))
  // 读: 用核心给出的**原始字节地址** + load 宽度 (arsize), 以正确访问"字节间隔的设备寄存器"
  // 写: 核心给出"字对齐地址 + byte lane 编在 mask 里", AXI 窄写需要 → 地址 = 对齐地址 | mask 偏移
  val addr = MuxLookup(state, a1)(Seq(
    1.U -> a1,
    2.U -> a2,
    4.U -> Mux(rd1en, rd1a, a1),
    5.U -> Mux(rd2en, rd2a, a1),
    6.U -> (wra | wrOff)))
  val arSize = MuxLookup(state, 2.U)(Seq(
    4.U -> Mux(rd1en, rd1s, 2.U),
    5.U -> Mux(rd2en, rd2s, 2.U)))
  val wstrb = Mux(wren, wrm, 0.U)
  // mask -> awsize: 4 字节=2, 2 字节(0x3/0xC)=1, 否则 1 字节=0
  val wrSize = Mux(wrm === "b1111".U, 2.U,
                Mux(wrm === "b0011".U || wrm === "b1100".U, 1.U, 0.U))

  // AXI 默认
  io.axi.arvalid := false.B; io.axi.araddr := addr; io.axi.arsize := arSize
  io.axi.rready := false.B
  io.axi.awvalid := false.B; io.axi.awaddr := addr; io.axi.awsize := wrSize
  io.axi.wvalid := false.B; io.axi.wdata := wrd; io.axi.wstrb := wstrb
  io.axi.bready := false.B
  io.axi.arid := 0.U; io.axi.arlen := 0.U; io.axi.arburst := 1.U
  io.axi.awid := 0.U; io.axi.awlen := 0.U; io.axi.awburst := 1.U
  io.axi.wlast := true.B

  val arFire = io.axi.arvalid && io.axi.arready
  val rFire  = io.axi.rvalid && io.axi.rready
  val awFire = io.axi.awvalid && io.axi.awready
  val wFire  = io.axi.wvalid && io.axi.wready
  val bFire  = io.axi.bvalid && io.axi.bready

  // ---- 内部 CLINT 的 mtime (AM 定时器读 0x0200_bff8/bffc), 不走 SoC ----
  val clintCnt = RegInit(0.U(64.W))
  clintCnt := clintCnt + 1.U
  val isClint = (addr & "hFFFF0000".U) === "h02000000".U
  val clintV  = Mux(addr === "h0200bff8".U, clintCnt(31, 0),
                Mux(addr === "h0200bffc".U, clintCnt(63, 32),
                Mux(addr === "h02000000".U, clintCnt(31, 0), 0.U)))  // mtimecmp 低字读作 mtime(可忽略)

  when (state === 0.U) {
    // START: 锁存取指地址 (核心此拍已冻结, pc 稳定)
    a1 := io.imem_a1; a2 := io.imem_a2
    phase := 0.U; state := 1.U
  } .elsewhen (isRead) {
    when (isClint) {
      // CLINT 本地应答 (不占用 AXI, 直接返回 mtime)
      when (state === 1.U) { im1 := clintV }
      .elsewhen (state === 2.U) { im2 := clintV }
      .elsewhen (state === 4.U) { d1 := clintV }
      .otherwise { d2 := clintV }
      phase := 0.U
      when (state === 5.U) { state := Mux(wren, 6.U, 7.U) } .otherwise { state := state + 1.U }
    } .otherwise {
      when (phase === 0.U) {
        io.axi.arvalid := true.B
        when (arFire) { phase := 1.U }
      } .otherwise {
        io.axi.rready := true.B
        when (rFire) {
          when (state === 1.U) { im1 := io.axi.rdata }
          .elsewhen (state === 2.U) { im2 := io.axi.rdata }
          .elsewhen (state === 4.U) { d1 := io.axi.rdata }
          .otherwise { d2 := io.axi.rdata }
          phase := 0.U
          when (state === 5.U) {
            state := Mux(wren, 6.U, 7.U)
          } .otherwise {
            state := state + 1.U
          }
        }
      }
    }
  } .elsewhen (state === 3.U) {
    // LATCH: im1/im2 已驱动给核心一拍, 现在锁存其组合算出的 dmem 请求
    rd1en := io.dr1_ren; rd1a := io.dr1_addr; rd1s := io.dr1_size
    rd2en := io.dr2_ren; rd2a := io.dr2_addr; rd2s := io.dr2_size
    wren := io.dw_wen; wra := io.dw_addr; wrd := io.dw_data; wrm := io.dw_mask
    state := 4.U
  } .elsewhen (isWrite) {
    when (phase === 0.U) {
      io.axi.awvalid := true.B; io.axi.wvalid := true.B
      when (awFire && wFire) { phase := 1.U }
    } .otherwise {
      io.axi.bready := true.B
      when (bFire) { phase := 0.U; state := 7.U }
    }
  } .otherwise {
    // CONSUME: busy=0, 数据有效; 下一拍重新 START
    state := 0.U
  }
}

// ============================================================================
// SoC 兼容顶层: 本体子模块 + AXI 桥 + 时钟门控
// ============================================================================
class ysyx_26030107 extends Module {
  val io = IO(new Bundle {
    val interrupt = Input(Bool())
    val master = new SocAXI4MasterIO
    val slave  = new SocAXI4SlaveIO
  })

  val bridge = Module(new csr_axi_mem)
  // 时钟门控: busy 时冻结核心; 复位期间放行时钟 (否则同步复位不生效)
  val gated = ClockGate(clock, (~bridge.io.busy) | reset.asBool)

  withClock(gated) {
    val ifu  = Module(new IFU)
    val idu  = Module(new idu)
    val exu1 = Module(new EXU)
    val exu2 = Module(new EXU)
    val lsu1 = Module(new LSU)
    val lsu2 = Module(new LSU)
    val wbu  = Module(new wbu)
    val grf  = Module(new GRF)

    bridge.io.imem_a1 := ifu.io.ifu_to_imem.addr1
    bridge.io.imem_a2 := ifu.io.ifu_to_imem.addr2
    ifu.io.imem_to_ifu.inst1 := bridge.io.imem_d1
    ifu.io.imem_to_ifu.inst2 := bridge.io.imem_d2

    ifu.io.ifu_to_idu <> idu.io.ifu_to_idu
    ifu.io.idu_to_ifu <> idu.io.idu_to_ifu
    idu.io.idu_to_grf <> grf.io.idu_to_grf
    grf.io.grf_to_idu <> idu.io.grf_to_idu
    idu.io.idu_to_exu1 <> exu1.io.idu_to_exu1
    exu2.io.idu_to_exu1.dec1_op  := idu.io.idu_to_exu2.dec2_op
    exu2.io.idu_to_exu1.dec1_imm := idu.io.idu_to_exu2.dec2_imm
    exu2.io.idu_to_exu1.dec1_val := idu.io.idu_to_exu2.dec2_val
    exu2.io.idu_to_exu1.dec1_rd  := idu.io.idu_to_exu2.dec2_rd
    exu2.io.idu_to_exu1.is_stall := false.B
    exu2.io.idu_to_exu1.inst1_pc := 0.U
    exu2.io.idu_to_exu1.inst1    := 0.U

    ifu.io.csr_to_ifu.take_trap := exu1.io.csr_to_ifu.take_trap
    ifu.io.csr_to_ifu.trap_pc   := exu1.io.csr_to_ifu.trap_pc
    ifu.io.csr_to_ifu.take_mret := exu1.io.csr_to_ifu.take_mret
    ifu.io.csr_to_ifu.mret_pc   := exu1.io.csr_to_ifu.mret_pc

    grf.io.csr_to_grf.wen   := exu1.io.csr_to_grf.wen
    grf.io.csr_to_grf.waddr := exu1.io.csr_to_grf.waddr
    grf.io.csr_to_grf.wdata := exu1.io.csr_to_grf.wdata

    exu1.io.exu_to_lsu <> lsu1.io.exu_to_lsu
    exu2.io.exu_to_lsu <> lsu2.io.exu_to_lsu

    wbu.io.lsu_to_wbu_1.rd          := lsu1.io.lsu_to_wbu.rd
    wbu.io.lsu_to_wbu_1.grf_wb_data := lsu1.io.lsu_to_wbu.grf_wb_data
    wbu.io.lsu_to_wbu_2.rd          := lsu2.io.lsu_to_wbu.rd
    wbu.io.lsu_to_wbu_2.grf_wb_data := lsu2.io.lsu_to_wbu.grf_wb_data

    bridge.io.dr1_addr := lsu1.io.debug.addr
    bridge.io.dr1_ren  := lsu1.io.lsu_to_dmem.ren
    bridge.io.dr1_size := lsu1.io.lsu_load_size
    bridge.io.dr2_addr := lsu2.io.debug.addr
    bridge.io.dr2_ren  := lsu2.io.lsu_to_dmem.ren
    bridge.io.dr2_size := lsu2.io.lsu_load_size
    bridge.io.dw_addr  := Mux(lsu1.io.lsu_to_dmem.wen, lsu1.io.lsu_to_dmem.addr, lsu2.io.lsu_to_dmem.addr)
    bridge.io.dw_wen   := lsu1.io.lsu_to_dmem.wen || lsu2.io.lsu_to_dmem.wen
    bridge.io.dw_data  := Mux(lsu1.io.lsu_to_dmem.wen, lsu1.io.lsu_to_dmem.store_data, lsu2.io.lsu_to_dmem.store_data)
    bridge.io.dw_mask  := Mux(lsu1.io.lsu_to_dmem.wen, lsu1.io.lsu_to_dmem.mask, lsu2.io.lsu_to_dmem.mask)
    lsu1.io.dmem_to_lsu.load_data := bridge.io.dr1_data
    lsu2.io.dmem_to_lsu.load_data := bridge.io.dr2_data

    wbu.io.wbu_to_grf <> grf.io.wbu_to_grf

    val e1 = exu1.io.exu_to_ifu.take_branch
    val e2 = exu2.io.exu_to_ifu.take_branch
    ifu.io.exu_to_ifu.take_branch   := e1 || e2
    ifu.io.exu_to_ifu.branch_target := Mux(e1, exu1.io.exu_to_ifu.branch_target, exu2.io.exu_to_ifu.branch_target)
  }

  io.master <> bridge.io.axi

  io.slave.awready := false.B
  io.slave.wready  := false.B
  io.slave.bvalid  := false.B
  io.slave.bid     := 0.U
  io.slave.bresp   := 0.U
  io.slave.arready := false.B
  io.slave.rvalid  := false.B
  io.slave.rid     := 0.U
  io.slave.rdata   := 0.U
  io.slave.rresp   := 0.U
  io.slave.rlast   := false.B
}

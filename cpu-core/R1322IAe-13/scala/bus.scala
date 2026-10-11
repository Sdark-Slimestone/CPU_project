package R1322IAeCSR

import chisel3._
import chisel3.util._

// SimpleBus 存储器参数。与 resources/DPI_Memory.v 的参数同步。
//   latency : 固定读延迟 (拍)
//   depth   : IMEM 未完成请求队列深度 (也限 IFU 元数据 FIFO)
//   dmemDepth: DMEM 读口在途请求深度 (非阻塞 load 允许背靠背 load 同时在飞)
// 本核 (IFU/LSU) 完全按 respValid/respReady 工作, 不假设延迟大小。
// (随机刺激已按"干净版"删除; 需要压力测试请用 -7。)
object MemCfg {
  val latency: Int = 1
  val depth:   Int = 16
  val dmemDepth: Int = 8
}

// 复位取指地址 (可在生成 Verilog 前通过环境变量 YSYX_RESET_PC 覆盖, 用于 flash 启动变体)。
object BootCfg {
  var resetPC: BigInt = BigInt("20000000", 16)   // 默认从 MROM 启动
}

//=================================== AXI4-Lite 总线 =========================================
// NPC 访存总线自 -10 起改为 AMBA AXI4-Lite。IFU/LSU 的访存接口 (imem/dmem 模块对内存
// 一侧) 是 AXI4-Lite master, 存储器 (DPI_Memory) 是 AXI4-Lite slave:
//   - 读事务: AR(地址) + R(数据) 两个独立通道;
//   - 写事务: AW(地址) + W(数据) + B(写回复) 三个独立通道。
// 每个通道各自用 valid/ready 握手 (valid 不得等 ready, 否则死锁)。
// 指令口只读, 故 IFU 侧 AW/W/B 恒为 0 (由 imem 拉低, 存储器一侧 assert)。
//
// 自 -11 起: 存储器只保留 **一个** AXI4-Lite 接口 (一组 AR/R + 一组 AW/W/B),
// IFU 与 LSU 的 master 经过一个 AXI4-Lite 仲裁器 (AXIArbiter) 共享它。
//
// 内部按功能拆成"读通道 / 写通道"两个方向正确的 Bundle, 便于 master (imem/dmem) 使用;
// 黑盒 DPIMemory 的端口则逐条展开 (保证 Verilog 端口名与 DPI_Memory.v 完全一致)。
// 自 -13 起: 从 AXI4-Lite 扩展到 **完整 AXI4** (单拍事务: len=0, burst=INCR, last=1, id=0)。
// 读/写 Bundle 按方向拆分, 便于 master (imem/dmem) 使用。去掉了 prot (SoC 不用)。
//   - arsize/awsize: 实际访问位宽 (0=1B,1=2B,2=4B), 供设备区分窄传输 (如 UART 1字节寄存器);
//   - arlen/awlen=0, arburst/awburst=INCR(01), rlast/wlast=1 (单拍);
//   - arid/awid=0。
class AXI4LiteRd extends Bundle {           // master 视角 (实为 AXI4 读通道)
  val araddr  = Output(UInt(32.W))
  val arlen   = Output(UInt(8.W))
  val arsize  = Output(UInt(3.W))
  val arburst = Output(UInt(2.W))
  val arid    = Output(UInt(4.W))
  val arvalid = Output(Bool())
  val arready = Input(Bool())
  val rdata   = Input(UInt(32.W))
  val rresp   = Input(UInt(2.W))
  val rlast   = Input(Bool())
  val rid     = Input(UInt(4.W))
  val rvalid  = Input(Bool())
  val rready  = Output(Bool())
}
class AXI4LiteWr extends Bundle {           // master 视角 (实为 AXI4 写通道)
  val awaddr  = Output(UInt(32.W))
  val awlen   = Output(UInt(8.W))
  val awsize  = Output(UInt(3.W))
  val awburst = Output(UInt(2.W))
  val awid    = Output(UInt(4.W))
  val awvalid = Output(Bool())
  val awready = Input(Bool())
  val wdata   = Output(UInt(32.W))
  val wstrb   = Output(UInt(4.W))
  val wlast   = Output(Bool())
  val wvalid  = Output(Bool())
  val wready  = Input(Bool())
  val bresp   = Input(UInt(2.W))
  val bid     = Input(UInt(4.W))
  val bvalid  = Input(Bool())
  val bready  = Output(Bool())
}

// AXI4-Lite 响应码 (AXI 规范): 2'b00 OKAY, 2'b10 SLVERR, 2'b11 DECERR。
object AXIResp {
  val OKAY   = 0.U(2.W)
  val SLVERR = 2.U(2.W)
  val DECERR = 3.U(2.W)
}

// AXI4-Lite 存储器黑盒 (DPI_Memory.v), **单个 AXI4-Lite slave 接口**。
//   - 一个 AXI4-Lite 读通道 (ar*/r*) + 一个 AXI4-Lite 写通道 (aw*/w*/b*)。
//   - IFU 与 LSU 经 AXIArbiter 共用这一组通道。
// 参数与 resources/DPI_Memory.v 同步: latency 固定读延迟, depth 读口在途深度。
class DPIMemory(val latency: Int = 1, val depth: Int = 1)
  extends BlackBox(Map(
    "LATENCY" -> IntParam(latency),
    "DEPTH"   -> IntParam(depth))) {
  val io = IO(new Bundle {
    val io_clk = Input(Clock())

    // ---- AXI4-Lite 读通道 (AR + R) ----
    val ar_valid = Input(Bool())
    val ar_ready = Output(Bool())
    val ar_addr  = Input(UInt(32.W))
    val ar_prot  = Input(UInt(3.W))
    val r_valid  = Output(Bool())
    val r_ready  = Input(Bool())
    val r_data   = Output(UInt(32.W))
    val r_resp   = Output(UInt(2.W))

    // ---- AXI4-Lite 写通道 (AW + W + B) ----
    val aw_valid = Input(Bool())
    val aw_ready = Output(Bool())
    val aw_addr  = Input(UInt(32.W))
    val aw_prot  = Input(UInt(3.W))
    val w_valid  = Input(Bool())
    val w_ready  = Output(Bool())
    val w_data   = Input(UInt(32.W))
    val w_strb   = Input(UInt(4.W))
    val b_valid  = Output(Bool())
    val b_ready  = Input(Bool())
    val b_resp   = Output(UInt(2.W))

    val flush  = Input(Bool())
    val ebreak = Input(Bool())
    val dbg_a1   = Output(UInt(32.W))
    val dbg_d1   = Output(UInt(9.W))
    val dbg_cnt1 = Output(UInt(9.W))
    val dbg_hd1  = Output(Bool())
  })
  override def desiredName = "DPI_Memory"
}

//=================================== 访存请求队列 (可冲刷) ==================================
// 保存 dmem 两个 lane 的读请求 {lane, addr}。两 lane 可能同拍发出, 需要 **原子** 接收:
//   - 只 lane1 或只 lane2 发: 需 1 个空位;
//   - 两 lane 同拍发: 需 2 个空位 (要么都收, 要么都不收, 否则 master 无法判断谁被接收,
//     会重复发射)。
// 内部环形缓冲, 支持每拍压入 0/1/2 条、弹出 1 条。flush 清空。
class LaneReqFifo(val depth: Int) extends Module {
  require(depth >= 2)
  val io = IO(new Bundle {
    val enq1_valid = Input(Bool())            // lane1 (更老)
    val enq1_addr  = Input(UInt(32.W))
    val enq1_size  = Input(UInt(2.W))         // 访问位宽 (0=1B,1=2B,2=4B)
    val enq2_valid = Input(Bool())            // lane2
    val enq2_addr  = Input(UInt(32.W))
    val enq2_size  = Input(UInt(2.W))
    val enq_ready  = Output(Bool())           // 本拍这批请求能否原子接收
    val deq_valid  = Output(Bool())
    val deq_ready  = Input(Bool())
    val deq_lane   = Output(Bool())           // false=lane1, true=lane2
    val deq_addr   = Output(UInt(32.W))
    val deq_size   = Output(UInt(2.W))
    val flush      = Input(Bool())
    val count      = Output(UInt(log2Ceil(depth + 1).W))
  })
  val AW = log2Ceil(depth)
  val lane = RegInit(VecInit(Seq.fill(depth)(false.B)))
  val addr = RegInit(VecInit(Seq.fill(depth)(0.U(32.W))))
  val size = RegInit(VecInit(Seq.fill(depth)(0.U(2.W))))
  val head = RegInit(0.U(AW.W))
  val cnt  = RegInit(0.U((AW + 1).W))

  // 注意: Chisel 的 "+" 结果宽度取操作数最大宽度 (1+1 会截成 0!)。
  // 用加宽加法得到 2 bit 的 0/1/2。
  val nReq = io.enq1_valid.asUInt +& io.enq2_valid.asUInt   // 0..2
  io.enq_ready := (depth.U -& cnt) >= nReq
  io.deq_valid := cnt > 0.U
  io.deq_lane  := lane(head)
  io.deq_addr  := addr(head)
  io.deq_size  := size(head)
  io.count     := cnt

  val doDeq = io.deq_valid && io.deq_ready
  val doEnq = (nReq > 0.U) && io.enq_ready && !io.flush
  val w0 = (head + cnt)(AW - 1, 0)
  val w1 = (head + cnt + 1.U)(AW - 1, 0)

  when (io.flush) {
    cnt  := 0.U
    head := 0.U
  } .otherwise {
    when (doEnq) {
      when (nReq === 2.U) {
        lane(w0) := false.B;     addr(w0) := io.enq1_addr;  size(w0) := io.enq1_size
        lane(w1) := true.B;      addr(w1) := io.enq2_addr;  size(w1) := io.enq2_size
      } .otherwise {
        lane(w0) := !io.enq1_valid
        addr(w0) := Mux(io.enq1_valid, io.enq1_addr, io.enq2_addr)
        size(w0) := Mux(io.enq1_valid, io.enq1_size, io.enq2_size)
      }
    }
    when (doDeq) { head := head + 1.U }
    cnt := cnt + Mux(doEnq, nReq, 0.U) - Mux(doDeq, 1.U, 0.U)
  }
}

//=================================== 译码控制信号 ===========================================
// 一条指令的一热控制信号集合, 由 Decoder 产出, 随级间消息一路携带到 EXU。
// 所有派生谓词 (isBranch/isLoad/... ) 以 def 形式集中在这里:
//   上层模块直接调用, 不必各自重复写一长串 "||" (可读性 + 单一事实来源)。
class CtrlSignals extends Bundle {
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
  // RV32M 乘除法 (通过多周期 MDU 执行)
  val is_mul    = Bool()
  val is_mulh   = Bool()
  val is_mulhsu = Bool()
  val is_mulhu  = Bool()
  val is_div    = Bool()
  val is_divu   = Bool()
  val is_rem    = Bool()
  val is_remu   = Bool()
  val is_csrrw  = Bool()
  val is_csrrs  = Bool()
  val is_csrrc  = Bool()
  val is_csrrwi = Bool()
  val is_csrrsi = Bool()
  val is_csrrci = Bool()
  val is_ecall  = Bool()
  val is_ebreak = Bool()
  val is_mret   = Bool()

  // ---------------- 粗粒度谓词 (decoder 一级 opcode 判定) ----------------
  // 直接由 opcode 得到, 供 EXU 等下游直接使用, 避免对一热信号做多输入 OR (缩短关键路径)。
  val is_branch  = Bool()
  val is_jump    = Bool()
  val is_load    = Bool()
  val is_store   = Bool()
  val is_imm_op  = Bool()
  val is_alu_op  = Bool()
  val is_csr     = Bool()

  // RV32M: 是否走多周期 MDU
  def is_muldiv: Bool =
    is_mul || is_mulh || is_mulhsu || is_mulhu ||
    is_div || is_divu || is_rem || is_remu
  // 是否乘 (相对除): 决定 MDU 走乘法还是除法数据通路
  def is_mul_op: Bool = is_mul || is_mulh || is_mulhsu || is_mulhu
  // 是否写通用寄存器 rd (用于 RA 别名追踪的清除)
  def writes_rd: Bool = is_alu_op || is_jump || is_load || is_csr
}

// 访存操作类型 (EXU -> LSU)
class MemOp extends Bundle {
  val is_lb     = Bool()
  val is_lh     = Bool()
  val is_lw     = Bool()
  val is_lbu    = Bool()
  val is_lhu    = Bool()
  val is_sb     = Bool()
  val is_sh     = Bool()
  val is_sw     = Bool()
  val is_ebreak = Bool()

  def isLoad:  Bool = is_lb || is_lh || is_lw || is_lbu || is_lhu
  def isStore: Bool = is_sb || is_sh || is_sw
}

//=================================== 下行链路消息 ===========================================
// IFU -> IDU: 一拍取回的两条指令及其 PC
class IFU_to_IDU_Message extends Bundle {
  val inst1        = UInt(32.W)
  val inst2        = UInt(32.W)
  val inst1_pc     = UInt(32.W)
  val inst2_pc     = UInt(32.W)
}

// 方向预测随指令一路携带的信息 (IFU -> 队列 -> IDU -> EXU 校验/更新)。
//   fetch_taken: 取指是否真的按 taken 跳转 (dir_taken && BTB 命中)
//   dir_taken  : 方向预测器(锦标赛)结论
//   taken      : 最终方向 (IDU 填, EXU 用它判"实际 vs 预测")
//   gsh_pred/loc_pred: 取指时动态分量的结论 (供选择器更新)
//   gsh_idx/loc_hist : 取指时用的索引/局部历史 (供决议时更新对应计数器, GHR 已变)
class PredInfo extends Bundle {
  val fetch_taken = Bool()
  val dir_taken   = Bool()
  val taken       = Bool()
  val gsh_pred    = Bool()
  val loc_pred    = Bool()
  val gsh_idx     = UInt(8.W)
  val loc_hist    = UInt(6.W)
  val pred_target = UInt(32.W)  // 取指/译码预测 taken 时用的目标 (EXU 校验目标对错)
  val lc_hit      = Bool()      // 循环计数器给出了高置信方向 (抑制 IDU 的 BTFN 兜底)
}

// IFU -> imem 的取指请求: 把该取指对的 {addr, pc, num, pred} 一起交给 imem。
// imem 取回后把这些元数据与两条指令**放在同一个响应队列里**返回, 从而 pc/指令/pred
// 永远成对流动, 不会因两个独立 FIFO 在重定向时失步而错位。
class FetchReq extends Bundle {
  val addr = UInt(32.W)
  val pc   = UInt(32.W)
  val num  = UInt(2.W)
  val pred = Vec(2, new PredInfo)
}
// imem -> IFU 的取指响应 (两条指令 + 其元数据一并带回)。
class FetchResp extends Bundle {
  val pc    = UInt(32.W)
  val num   = UInt(2.W)
  val pred  = Vec(2, new PredInfo)
  val inst0 = UInt(32.W)
  val inst1 = UInt(32.W)
}

// IDU -> EXU: 单 lane 的译码结果。
// 相比旧版把 nextpc 换成 pc (指令自身 PC): EXU 不再做 nextpc-4 的减法。
class IDU_to_EXU_Lane_Message extends Bundle {
  val ctrl     = new CtrlSignals
  val imm      = UInt(32.W)
  val rs1_addr = UInt(5.W)    // 源寄存器号 (转发比较用)
  val rs2_addr = UInt(5.W)
  val rs1_val  = UInt(32.W)
  val rs2_val  = UInt(32.W)
  val pc       = UInt(32.W)
  val rd       = UInt(5.W)
  val is_stall = Bool()
  val inst     = UInt(32.W)   // 原始指令 (CSR 地址/uimm 用)
  val pred     = new PredInfo // 方向预测结论 (随指令带到 EXU 校验)
  val ldTag    = UInt(16.W)   // load 访存标签 (IDU 按发射序分配; dmem 响应按此回配)
}

// EXU -> LSU: 单 lane 的访存请求 + 穿透写回数据
class EXU_to_LSU_Lane_Message extends Bundle {
  val op         = new MemOp
  val addr       = UInt(32.W)
  val store_data = UInt(32.W)
  val rd         = UInt(5.W)
  val wb_data    = UInt(32.W)
  val pc         = UInt(32.W)
}

// LSU -> WBU: 单 lane 的写回
class LSU_to_WBU_Lane_Message extends Bundle {
  val rd          = UInt(5.W)
  val grf_wb_data = UInt(32.W)
  val pc          = UInt(32.W)
}

// 在途 load 的元数据 (非阻塞 load: 响应到达时据此回写 GRF)
class LdMeta extends Bundle {
  val rd  = UInt(5.W)
  val op  = new MemOp
  val off = UInt(2.W)
}

//=================================== 级间桥接 ===============================================
// 1 深度握手桥: 一次一包, 交出后清零, 保证下游在桥空时看到的位全为 0 (无副作用)。
class StageConnectBridge[T <: Data](gen: T) extends Module {
  val io = IO(new Bundle {
    val in  = Flipped(Decoupled(gen))
    val out = Decoupled(gen)
  })

  val valid = RegInit(false.B)
  val bits  = RegInit(0.U.asTypeOf(gen))

  io.in.ready  := !valid
  io.out.valid := valid
  io.out.bits  := bits

  when (io.in.fire) {
    bits  := io.in.bits
    valid := true.B
  } .elsewhen (io.out.fire) {
    bits  := 0.U.asTypeOf(gen)
    valid := false.B
  }
}

object StageConnect {
  def apply[T <: Data](left: DecoupledIO[T], right: DecoupledIO[T]): Unit = {
    val bridge = Module(new StageConnectBridge(chiselTypeOf(left.bits)))
    bridge.io.in  <> left
    bridge.io.out <> right
  }
}

//=================================== 流水级间寄存器 (pipeline) ===============================
// 与 multi 版"空才能收"的 StageConnectBridge 不同: 本寄存器每拍可进一包(输入输出同拍吞吐),
//   stall: 冻结本级(保持已存数据), 用于前端冒险阻塞;
//   flush: 清除本级有效位(冲刷错取的包); 只影响下一拍。
// 关键: 无效时把 bits 清零, 否则下游 (lsu/wbu) 会拿旧包的位产生副作用。
class StagePipeBridge[T <: Data](gen: T) extends Module {
  val io = IO(new Bundle {
    val in    = Flipped(Decoupled(gen))
    val out   = Decoupled(gen)
    val stall = Input(Bool())
    val flush = Input(Bool())
  })

  val valid = RegInit(false.B)
  val bits  = RegInit(0.U.asTypeOf(gen))

  io.in.ready  := !io.stall && !io.flush
  io.out.valid := valid
  io.out.bits  := bits

  when (io.flush) {
    valid := false.B
    bits  := 0.U.asTypeOf(gen)
  } .elsewhen (io.stall) {
    // 冻结: 保持
  } .elsewhen (io.in.valid) {
    bits  := io.in.bits
    valid := true.B
  } .otherwise {
    valid := false.B
    bits  := 0.U.asTypeOf(gen)
  }
}

object StagePipe {
  def apply[T <: Data](left: DecoupledIO[T], right: DecoupledIO[T],
                       stall: Bool, flush: Bool): Unit = {
    val bridge = Module(new StagePipeBridge(chiselTypeOf(left.bits)))
    bridge.io.in    <> left
    bridge.io.out   <> right
    bridge.io.stall := stall
    bridge.io.flush := flush
  }
}

// D/E 级寄存器 (带操作数刷新): 冻结 (stall) 时用 refresh 覆盖 rs1_val/rs2_val,
// 保证因访存延迟被冻结在 E 级的指令操作数不因 M/W 级生产者流出而变陈旧。
class ERefreshPipeBridge extends Module {
  val io = IO(new Bundle {
    val in      = Flipped(Decoupled(new IDU_to_EXU_Lane_Message))
    val out     = Decoupled(new IDU_to_EXU_Lane_Message)
    val refresh = Input(new IDU_to_EXU_Lane_Message)
    val stall   = Input(Bool())
    val flush   = Input(Bool())
  })

  val valid = RegInit(false.B)
  val bits  = RegInit(0.U.asTypeOf(new IDU_to_EXU_Lane_Message))

  io.in.ready  := !io.stall && !io.flush
  io.out.valid := valid
  io.out.bits  := bits

  when (io.flush) {
    valid := false.B
    bits  := 0.U.asTypeOf(new IDU_to_EXU_Lane_Message)
  } .elsewhen (io.stall) {
    when (valid) {
      bits.rs1_val := io.refresh.rs1_val
      bits.rs2_val := io.refresh.rs2_val
    }
  } .elsewhen (io.in.valid) {
    bits  := io.in.bits
    valid := true.B
  } .otherwise {
    valid := false.B
    bits  := 0.U.asTypeOf(new IDU_to_EXU_Lane_Message)
  }
}

object ERefreshPipe {
  def apply(in: DecoupledIO[IDU_to_EXU_Lane_Message], out: DecoupledIO[IDU_to_EXU_Lane_Message],
            refresh: IDU_to_EXU_Lane_Message, stall: Bool, flush: Bool): Unit = {
    val bridge = Module(new ERefreshPipeBridge)
    bridge.io.in      <> in
    bridge.io.out     <> out
    bridge.io.refresh := refresh
    bridge.io.stall   := stall
    bridge.io.flush   := flush
  }
}

//=================================== 可冲刷元数据 FIFO =======================================
// 保存"已发出但未收到响应"的取指请求元数据 {pc, num, pred}。存储器按序返回响应,
// 因此队头即当前响应。enq 在请求被接受(reqReady)时压入; deq 在响应被消费时弹出;
// flush 清空 (重定向丢在途取指)。深度需 >= 存储器在途请求数。
class MetaFIFOBridge[T <: Data](gen: T, depth: Int) extends Module {
  require(depth >= 1)
  val io = IO(new Bundle {
    val enq   = Flipped(Decoupled(gen))
    val deq   = Decoupled(gen)
    val flush = Input(Bool())
  })

  val v = RegInit(VecInit(Seq.fill(depth)(false.B)))
  val b = RegInit(VecInit(Seq.fill(depth)(0.U.asTypeOf(gen))))
  val cnt = RegInit(0.U(log2Ceil(depth + 1).W))

  io.enq.ready := cnt < depth.U
  io.deq.valid := cnt > 0.U
  io.deq.bits  := b(0)

  val doEnq = io.enq.valid && io.enq.ready
  val doDeq = io.deq.valid && io.deq.ready

  when (io.flush) {
    cnt := 0.U
    for (i <- 0 until depth) { v(i) := false.B }
  } .otherwise {
    when (doDeq) {
      for (i <- 0 until depth - 1) { v(i) := v(i + 1); b(i) := b(i + 1) }
      v(depth - 1) := false.B
    }
    // 压入放在移位之后 (同拍既弹又压时接在末尾)
    when (doEnq) {
      val idx = Mux(doDeq, cnt - 1.U, cnt)
      v(idx) := true.B
      b(idx) := io.enq.bits
    }
    cnt := cnt + Mux(doEnq, 1.U, 0.U) - Mux(doDeq, 1.U, 0.U)
  }
}

object MetaFIFO {
  def apply[T <: Data](enq: DecoupledIO[T], deq: DecoupledIO[T], depth: Int, flush: Bool): Unit = {
    val q = Module(new MetaFIFOBridge(chiselTypeOf(enq.bits), depth))
    q.io.enq   <> enq
    q.io.deq   <> deq
    q.io.flush := flush
  }
}

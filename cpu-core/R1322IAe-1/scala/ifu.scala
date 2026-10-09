package R1322IAeCSR

import chisel3._
import chisel3.util._

// 取指单元。
// 正常每包取两条相邻指令 (pc, pc+4) 交给 IDU; IDU 若判定冲突则回 stall,
// 下周期重取 (pc+4, pc+8) 中的第一条起。所有仲裁由 IDU 做, IFU 只负责取。
// 多周期下 IFU 一次只服务一个包: 包交给 F/D 桥后关取指门, 等该包 W 拍完成再推进 pc。
class IFU extends Module {
  val io = IO(new Bundle {
    val exu_to_ifu = new Bundle {
      val take_branch   = Input(Bool())
      val branch_target = Input(UInt(32.W))
    }

    val idu_to_ifu = new Bundle {
      val is_stall = Input(Bool())
    }

    // CSR 异常/mret 接口
    val csr_to_ifu = new Bundle {
      val take_trap = Input(Bool())
      val trap_pc   = Input(UInt(32.W))
      val take_mret = Input(Bool())
      val mret_pc   = Input(UInt(32.W))
    }

    val ifu_to_imem = new Bundle {
      val addr1 = Output(UInt(32.W))
      val addr2 = Output(UInt(32.W))
    }
    val imem_to_ifu = new Bundle {
      val inst1 = Input(UInt(32.W))
      val inst2 = Input(UInt(32.W))
    }

    // 多周期控制: 各阶段"处理本包"的那一拍
    val ctrl = new Bundle {
      val capture_stall    = Input(Bool())  // IDU 处理本包 (D 拍)
      val capture_redirect = Input(Bool())  // EXU 处理本包 (E 拍)
      val done             = Input(Bool())  // WBU 处理本包 (W 拍, 最后一拍)
    }

    val ifu_to_idu = Decoupled(new IFU_to_IDU_Message)

    val debug = new Bundle {
      val inst1_pc = Output(UInt(32.W))
      val inst2_pc = Output(UInt(32.W))
    }
  })

  val pcReg   = RegInit("h80000000".U(32.W))
  val pcPlus4 = pcReg + 4.U
  val pcPlus8 = pcReg + 8.U

  // 取指门控: 包交给 F/D 桥后关闭, 包完成拍再打开
  // 同步 imem: 地址需提前一拍发起, 数据下一拍才有效 -> canFetch 在 done 的下一拍才拉高
  val canFetch = RegInit(false.B)
  // 复位后先发起一次取指 (没有前置 done 拍)
  val initFetch = RegInit(true.B)
  initFetch := false.B

  // 控制信号锁存: 产生拍写入, 包完成拍读出决定下一条 PC
  val stallReg        = RegInit(false.B)
  val branchReg       = RegInit(false.B)
  val branchTargetReg = RegInit(0.U(32.W))
  val trapReg         = RegInit(false.B)
  val trapPcReg       = RegInit(0.U(32.W))
  val mretReg         = RegInit(false.B)
  val mretPcReg       = RegInit(0.U(32.W))

  // D 拍: 锁存 IDU 的单发射判定
  when (io.ctrl.capture_stall) {
    stallReg := io.idu_to_ifu.is_stall
  }
  // E 拍: 锁存分支/异常/mret 目标
  when (io.ctrl.capture_redirect) {
    branchReg       := io.exu_to_ifu.take_branch
    branchTargetReg := io.exu_to_ifu.branch_target
    trapReg         := io.csr_to_ifu.take_trap
    trapPcReg       := io.csr_to_ifu.trap_pc
    mretReg         := io.csr_to_ifu.take_mret
    mretPcReg       := io.csr_to_ifu.mret_pc
  }
  // 下一条 PC (由锁存的控制信号决定)
  val nextPc = Mux(trapReg,   trapPcReg,
              Mux(mretReg,   mretPcReg,
              Mux(branchReg, branchTargetReg,
              Mux(stallReg,  pcPlus4, pcPlus8))))

  // W 拍: 结算下一条 PC; 同步 imem 的读地址也在这一拍提前发出
  when (io.ctrl.done) {
    pcReg := nextPc
  }

  // 取指地址: done 拍提前发下一条 PC (供同步 imem 下一拍返回), 其余拍维持当前 PC
  val fetchAddr = Mux(io.ctrl.done, nextPc, pcReg)

  // canFetch 在 done 的下一拍拉高 (此时同步 imem 数据已有效)
  when (io.ctrl.done || initFetch) {
    canFetch := true.B
  } .elsewhen (io.ifu_to_idu.fire) {
    canFetch := false.B
  }

  // 接线
  io.ifu_to_imem.addr1 := fetchAddr
  io.ifu_to_imem.addr2 := fetchAddr + 4.U

  io.ifu_to_idu.bits.inst1    := io.imem_to_ifu.inst1
  io.ifu_to_idu.bits.inst2    := io.imem_to_ifu.inst2
  io.ifu_to_idu.bits.inst1_pc := pcReg
  io.ifu_to_idu.bits.inst2_pc := pcPlus4
  io.ifu_to_idu.valid         := canFetch

  io.debug.inst1_pc := pcReg
  io.debug.inst2_pc := pcPlus4
}

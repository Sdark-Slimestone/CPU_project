package R1322IAeCSR

import chisel3._
import chisel3.util._

//正常状态+8取两条指令给idu，如果idu鉴定为冲突返回stall信号，下周期重新取上周期第二条指令和下一条指令
//所有仲裁由idu做，ifu只负责取
//冲突时idu丢弃第二条指令
//正常 pc 0 8 16。。。
//一直冲突 pc 0 4 8 。。。
class IFU extends Module {
  val io = IO(new Bundle {
    val exu_to_ifu = new Bundle{
      val take_branch    = Input(Bool())
      val branch_target  = Input(UInt(32.W))
    }

    val idu_to_ifu = new Bundle{
      val is_stall       = Input(Bool())
    }

    // CSR 异常/mret 接口
    val csr_to_ifu = new Bundle {
      val take_trap = Input(Bool())
      val trap_pc   = Input(UInt(32.W))
      val take_mret = Input(Bool())
      val mret_pc   = Input(UInt(32.W))
    }

    val ifu_to_imem = new Bundle{
      val addr1      = Output(UInt(32.W))
      val addr2      = Output(UInt(32.W))
    }

    val imem_to_ifu = new Bundle{
      val inst1  = Input(UInt(32.W))
      val inst2  = Input(UInt(32.W))
    }

    //====================================新增: 多周期控制接口====================================
    val ctrl = new Bundle {
      val capture_stall    = Input(Bool())
      val capture_redirect = Input(Bool())
      val done             = Input(Bool())
    }
    //====================================新增: 多周期控制接口====================================

    //==========================================接口==============================================
    val ifu_to_idu = Decoupled(new IFU_to_IDU_Message)
    /*val ifu_to_idu = new Bundle{
      val inst1     = Output(UInt(32.W))
      val inst2     = Output(UInt(32.W))
      val inst1_pc     = Output(UInt(32.W))
      val inst2_pc     = Output(UInt(32.W))
      val inst1_nextpc = Output(UInt(32.W))
      val inst2_nextpc = Output(UInt(32.W))
    }*/
    //==========================================接口==============================================

    val debug = new Bundle{
      val debug_inst1_pc = Output(UInt(32.W))
      val debug_inst2_pc = Output(UInt(32.W))
    }
  })

  //  jal/jalr/br 指令只能作为第一条指令进行单发射
  val pcReg   = RegInit("h80000000".U(32.W))
  val pcPlus4 = pcReg + 4.U
  val pcPlus8 = pcReg + 8.U

  /* 原 pc 更新保留
  //  pc值变化
  //  优先级: 异常跳转 > mret 返回 > 分支跳转 > stall(单步) > 正常+8
  pcReg := Mux(io.csr_to_ifu.take_trap, io.csr_to_ifu.trap_pc,
            Mux(io.csr_to_ifu.take_mret, io.csr_to_ifu.mret_pc,
            Mux(io.exu_to_ifu.take_branch, io.exu_to_ifu.branch_target,
            Mux(io.idu_to_ifu.is_stall, pcPlus4, pcPlus8))))
  原 pc 更新保留结束 */

  //====================================新增: 控制信号锁存 + 取指门控====================================
  // 取指门控: 包交给 F/D 桥后关闭, 包完成拍再打开
  val canFetch = RegInit(true.B)

  // 控制信号锁存: 产生时写入, 包完成时读出
  val stallReg        = RegInit(false.B)
  val branchReg       = RegInit(false.B)
  val branchTargetReg = RegInit(0.U(32.W))
  val trapReg         = RegInit(false.B)
  val trapPcReg       = RegInit(0.U(32.W))
  val mretReg         = RegInit(false.B)
  val mretPcReg       = RegInit(0.U(32.W))

  // stall 产生拍(IDU 处理该包): 写入
  when (io.ctrl.capture_stall) {
    stallReg := io.idu_to_ifu.is_stall
  }
  // 分支/异常/mret 产生拍(EXU 处理该包): 写入
  when (io.ctrl.capture_redirect) {
    branchReg       := io.exu_to_ifu.take_branch
    branchTargetReg := io.exu_to_ifu.branch_target
    trapReg         := io.csr_to_ifu.take_trap
    trapPcReg       := io.csr_to_ifu.trap_pc
    mretReg         := io.csr_to_ifu.take_mret
    mretPcReg       := io.csr_to_ifu.mret_pc
  }

  // 包完成拍: 用锁存值算下一条 PC, 并重新允许取指
  when (io.ctrl.done) {
    pcReg := Mux(trapReg,   trapPcReg,
            Mux(mretReg,   mretPcReg,
            Mux(branchReg, branchTargetReg,
            Mux(stallReg,  pcPlus4, pcPlus8))))
    canFetch := true.B
  } .elsewhen (io.ifu_to_idu.fire) {
    canFetch := false.B
  }
  //====================================新增: 控制信号锁存 + 取指门控====================================

  // 下一条指令地址计算
  val inst1next = pcPlus4
  val inst2next = pcPlus8

  //接线
  io.ifu_to_imem.addr1 := pcReg
  io.ifu_to_imem.addr2 := pcPlus4

  io.ifu_to_idu.bits.inst1 := io.imem_to_ifu.inst1
  io.ifu_to_idu.bits.inst2 := io.imem_to_ifu.inst2
  io.ifu_to_idu.bits.inst1_pc := pcReg
  io.ifu_to_idu.bits.inst2_pc := pcPlus4
  io.ifu_to_idu.bits.inst1_nextpc := inst1next
  io.ifu_to_idu.bits.inst2_nextpc := inst2next
  /* 原 valid 保留
  io.ifu_to_idu.valid:=true.B
  原 valid 保留结束 */

  //====================================新增: 取指门控 valid====================================
  io.ifu_to_idu.valid := canFetch
  //====================================新增: 取指门控 valid====================================

  io.debug.debug_inst1_pc := pcReg
  io.debug.debug_inst2_pc := pcPlus4
}
package R1322IAeCSRMC

import chisel3._
import chisel3.util._

// 组合访存数据通路（单个 lane）：地址对齐、写掩码生成、写数据移位、读数据提取
// 被 LSU 级实例化两次（双发射对应两个 DMEM 端口）
class MemUnit extends Module {
  val io = IO(new Bundle {
    val exu_to_lsu = new Bundle {
      val op = new Bundle {
        val is_lb     = Input(Bool())
        val is_lh     = Input(Bool())
        val is_lw     = Input(Bool())
        val is_lbu    = Input(Bool())
        val is_lhu    = Input(Bool())
        val is_sb     = Input(Bool())
        val is_sh     = Input(Bool())
        val is_sw     = Input(Bool())
        val is_ebreak = Input(Bool())
      }
      val paddr = new Bundle {
        val addr = Input(UInt(32.W))
      }
      val data = new Bundle {
        val store_data = Input(UInt(32.W))
      }
      val exu_through_lsu_to_wbu = new Bundle {
        val rd = Input(UInt(5.W))
        val grf_wb_data = Input(UInt(32.W))
      }
    }

    val lsu_to_dmem = new Bundle {
      val addr       = Output(UInt(32.W))
      val store_data = Output(UInt(32.W))
      val mask       = Output(UInt(4.W))
      val wen        = Output(Bool())
      val ren        = Output(Bool())
    }

    val dmem_to_lsu = new Bundle {
      val load_data = Input(UInt(32.W))
    }

    val lsu_to_wbu = new Bundle {
      val rd = Output(UInt(5.W))
      val grf_wb_data = Output(UInt(32.W))
    }

    val ebreak_out = Output(Bool())

    val debug = new Bundle {
      val is_load            = Output(Bool())
      val is_store           = Output(Bool())
      val addr               = Output(UInt(32.W))
      val read_origin        = Output(UInt(32.W))
      val final_wb_data      = Output(UInt(32.W))
      val store_mask         = Output(UInt(4.W))
      val store_data_shifted = Output(UInt(32.W))
    }
  })

  // 地址对齐和字节偏移
  val aligned_addr = io.exu_to_lsu.paddr.addr & ~3.U(32.W)
  val byte_offset = io.exu_to_lsu.paddr.addr(1, 0)

  val is_load = io.exu_to_lsu.op.is_lb || io.exu_to_lsu.op.is_lh || io.exu_to_lsu.op.is_lw ||
                io.exu_to_lsu.op.is_lbu || io.exu_to_lsu.op.is_lhu
  val is_store = io.exu_to_lsu.op.is_sb || io.exu_to_lsu.op.is_sh || io.exu_to_lsu.op.is_sw

  // 读写使能
  io.lsu_to_dmem.wen := is_store
  io.lsu_to_dmem.ren := is_load

  // 对齐后的地址输出
  io.lsu_to_dmem.addr := aligned_addr

  // 写数据移位（小端序）
  val wdata = io.exu_to_lsu.data.store_data
  val shifted_wdata = MuxCase(wdata, Seq(
    (io.exu_to_lsu.op.is_sb) -> (wdata << (byte_offset << 3)),
    (io.exu_to_lsu.op.is_sh) -> (wdata << (byte_offset << 3))
  ))
  io.lsu_to_dmem.store_data := shifted_wdata

  // 写掩码生成
  io.lsu_to_dmem.mask := MuxCase(0.U(4.W), Seq(
    io.exu_to_lsu.op.is_sw -> "b1111".U(4.W),
    io.exu_to_lsu.op.is_sh -> Mux(byte_offset === 0.U, "b0011".U(4.W),
                             Mux(byte_offset === 2.U, "b1100".U(4.W), 0.U(4.W))),
    io.exu_to_lsu.op.is_sb -> (1.U(4.W) << byte_offset)
  ))

  // 读数据通路
  val aligned = io.dmem_to_lsu.load_data
  val byte0 = aligned(7,0)
  val byte1 = aligned(15,8)
  val byte2 = aligned(23,16)
  val byte3 = aligned(31,24)
  val half0 = Cat(byte1, byte0)
  val half1 = Cat(byte3, byte2)

  val byte_sel = Mux(byte_offset(1),
    Mux(byte_offset(0), byte3, byte2),
    Mux(byte_offset(0), byte1, byte0)
  )
  val half_sel = Mux(byte_offset(1), half1, half0)

  val rdata = MuxCase(0.U(32.W), Seq(
    io.exu_to_lsu.op.is_lw   -> aligned,
    io.exu_to_lsu.op.is_lh   -> Cat(Fill(16, half_sel(15)), half_sel),
    io.exu_to_lsu.op.is_lhu  -> Cat(0.U(16.W), half_sel),
    io.exu_to_lsu.op.is_lb   -> Cat(Fill(24, byte_sel(7)), byte_sel),
    io.exu_to_lsu.op.is_lbu  -> Cat(0.U(24.W), byte_sel)
  ))

  // 写回数据：load指令从存储器取，其他直接用EXU传来的值
  val wb_data = Mux(is_load, rdata, io.exu_to_lsu.exu_through_lsu_to_wbu.grf_wb_data)

  io.lsu_to_wbu.rd := io.exu_to_lsu.exu_through_lsu_to_wbu.rd
  io.lsu_to_wbu.grf_wb_data := wb_data

  // ebreak 透传
  io.ebreak_out := io.exu_to_lsu.op.is_ebreak

  // debug
  io.debug.is_load            := is_load
  io.debug.is_store           := is_store
  io.debug.addr               := io.exu_to_lsu.paddr.addr
  io.debug.read_origin        := aligned
  io.debug.final_wb_data      := wb_data
  io.debug.store_mask         := io.lsu_to_dmem.mask
  io.debug.store_data_shifted := shifted_wdata
}

// 多周期 LSU 级: EXU 的 slave, WBU 的 master
// 接收 EXU 的执行结果消息(运算结果+访存请求), 暂存一个周期,
// 在 busy 周期完成访存(组合读 DMEM), 合并 load 数据后发给 WBU;
// 访存写与 ebreak 在 commit(= out.fire, 消息被 WBU 接收)那一周期生效, 恰好一次
class LSU extends Module {
  val io = IO(new Bundle {
    // 总线输入: 执行结果 <- EXU
    val in = Flipped(Decoupled(new EXMsg))

    // 总线输出: 写回消息 -> WBU
    val out = Decoupled(new WBMsg)

    // DMEM 接口（两个 lane 各一个端口）
    val lsu_to_dmem_1 = new Bundle {
      val addr       = Output(UInt(32.W))
      val store_data = Output(UInt(32.W))
      val mask       = Output(UInt(4.W))
      val wen        = Output(Bool())
      val ren        = Output(Bool())
    }
    val lsu_to_dmem_2 = new Bundle {
      val addr       = Output(UInt(32.W))
      val store_data = Output(UInt(32.W))
      val mask       = Output(UInt(4.W))
      val wen        = Output(Bool())
      val ren        = Output(Bool())
    }
    val dmem_to_lsu_1 = new Bundle {
      val load_data = Input(UInt(32.W))
    }
    val dmem_to_lsu_2 = new Bundle {
      val load_data = Input(UInt(32.W))
    }
    val ebreak_out = Output(Bool())

    // LSU debug
    val debug_lsu1 = new Bundle {
      val is_load            = Output(Bool())
      val is_store           = Output(Bool())
      val addr               = Output(UInt(32.W))
      val read_origin        = Output(UInt(32.W))
      val final_wb_data      = Output(UInt(32.W))
      val store_mask         = Output(UInt(4.W))
      val store_data_shifted = Output(UInt(32.W))
    }
    val debug_lsu2 = new Bundle {
      val is_load            = Output(Bool())
      val is_store           = Output(Bool())
      val addr               = Output(UInt(32.W))
      val read_origin        = Output(UInt(32.W))
      val final_wb_data      = Output(UInt(32.W))
      val store_mask         = Output(UInt(4.W))
      val store_data_shifted = Output(UInt(32.W))
    }
  })

  // ---------- 级寄存器: 暂存执行结果 ----------
  val msg  = RegEnable(io.in.bits, io.in.fire)
  val busy = RegInit(false.B)
  io.in.ready  := !busy
  io.out.valid := busy
  when (io.in.fire)  { busy := true.B  }
  when (io.out.fire) { busy := false.B }

  // 提交信号: 消息被 WBU 接收, 访存副作用在此周期生效
  val commit = io.out.fire

  // ---------- 两个 lane 的访存数据通路 ----------
  val u1 = Module(new MemUnit)
  val u2 = Module(new MemUnit)

  u1.io.exu_to_lsu.op.is_lb   := msg.op1.is_lb
  u1.io.exu_to_lsu.op.is_lh   := msg.op1.is_lh
  u1.io.exu_to_lsu.op.is_lw   := msg.op1.is_lw
  u1.io.exu_to_lsu.op.is_lbu  := msg.op1.is_lbu
  u1.io.exu_to_lsu.op.is_lhu  := msg.op1.is_lhu
  u1.io.exu_to_lsu.op.is_sb   := msg.op1.is_sb
  u1.io.exu_to_lsu.op.is_sh   := msg.op1.is_sh
  u1.io.exu_to_lsu.op.is_sw   := msg.op1.is_sw
  u1.io.exu_to_lsu.op.is_ebreak := msg.is_ebreak
  u1.io.exu_to_lsu.paddr.addr := msg.addr1
  u1.io.exu_to_lsu.data.store_data := msg.store_data1
  u1.io.exu_to_lsu.exu_through_lsu_to_wbu.rd := msg.rd1
  u1.io.exu_to_lsu.exu_through_lsu_to_wbu.grf_wb_data := msg.wb_data1
  u1.io.dmem_to_lsu.load_data := io.dmem_to_lsu_1.load_data

  u2.io.exu_to_lsu.op.is_lb   := msg.op2.is_lb
  u2.io.exu_to_lsu.op.is_lh   := msg.op2.is_lh
  u2.io.exu_to_lsu.op.is_lw   := msg.op2.is_lw
  u2.io.exu_to_lsu.op.is_lbu  := msg.op2.is_lbu
  u2.io.exu_to_lsu.op.is_lhu  := msg.op2.is_lhu
  u2.io.exu_to_lsu.op.is_sb   := msg.op2.is_sb
  u2.io.exu_to_lsu.op.is_sh   := msg.op2.is_sh
  u2.io.exu_to_lsu.op.is_sw   := msg.op2.is_sw
  u2.io.exu_to_lsu.op.is_ebreak := false.B
  u2.io.exu_to_lsu.paddr.addr := msg.addr2
  u2.io.exu_to_lsu.data.store_data := msg.store_data2
  u2.io.exu_to_lsu.exu_through_lsu_to_wbu.rd := msg.rd2
  u2.io.exu_to_lsu.exu_through_lsu_to_wbu.grf_wb_data := msg.wb_data2
  u2.io.dmem_to_lsu.load_data := io.dmem_to_lsu_2.load_data

  // DMEM 端口: 读使能按 busy(活动周期), 写使能按 commit(恰好写一次)
  io.lsu_to_dmem_1.addr       := u1.io.lsu_to_dmem.addr
  io.lsu_to_dmem_1.store_data := u1.io.lsu_to_dmem.store_data
  io.lsu_to_dmem_1.mask       := u1.io.lsu_to_dmem.mask
  io.lsu_to_dmem_1.wen        := u1.io.lsu_to_dmem.wen && commit
  io.lsu_to_dmem_1.ren        := u1.io.lsu_to_dmem.ren && busy

  io.lsu_to_dmem_2.addr       := u2.io.lsu_to_dmem.addr
  io.lsu_to_dmem_2.store_data := u2.io.lsu_to_dmem.store_data
  io.lsu_to_dmem_2.mask       := u2.io.lsu_to_dmem.mask
  io.lsu_to_dmem_2.wen        := u2.io.lsu_to_dmem.wen && commit
  io.lsu_to_dmem_2.ren        := u2.io.lsu_to_dmem.ren && busy

  // ebreak 只在提交周期上报一次(经 dmem 触发仿真结束)
  // (lane2 的控制类指令恒被单发射清零, 此处合并仅为与原设计保持一致)
  io.ebreak_out := (u1.io.ebreak_out || u2.io.ebreak_out) && commit

  // ---------- 总线输出: 写回消息 ----------
  io.out.bits.rd1      := u1.io.lsu_to_wbu.rd
  io.out.bits.wb_data1 := u1.io.lsu_to_wbu.grf_wb_data
  io.out.bits.rd2      := u2.io.lsu_to_wbu.rd
  io.out.bits.wb_data2 := u2.io.lsu_to_wbu.grf_wb_data

  // ---------- debug ----------
  io.debug_lsu1.is_load            := u1.io.debug.is_load
  io.debug_lsu1.is_store           := u1.io.debug.is_store
  io.debug_lsu1.addr               := u1.io.debug.addr
  io.debug_lsu1.read_origin        := u1.io.debug.read_origin
  io.debug_lsu1.final_wb_data      := u1.io.debug.final_wb_data
  io.debug_lsu1.store_mask         := u1.io.debug.store_mask
  io.debug_lsu1.store_data_shifted := u1.io.debug.store_data_shifted

  io.debug_lsu2.is_load            := u2.io.debug.is_load
  io.debug_lsu2.is_store           := u2.io.debug.is_store
  io.debug_lsu2.addr               := u2.io.debug.addr
  io.debug_lsu2.read_origin        := u2.io.debug.read_origin
  io.debug_lsu2.final_wb_data      := u2.io.debug.final_wb_data
  io.debug_lsu2.store_mask         := u2.io.debug.store_mask
  io.debug_lsu2.store_data_shifted := u2.io.debug.store_data_shifted
}

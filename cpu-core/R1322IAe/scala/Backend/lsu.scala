package R1322IAe

import chisel3._
import chisel3.util._

class LSU extends Module {
  val io = IO(new Bundle {
    val dmem_to_lsu = new Bundle {
      val load_data = Input(UInt(32.W))
    }

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

    val lsu_to_wbu = new Bundle {
      val rd = Output(UInt(5.W))
      val grf_wb_data = Output(UInt(32.W))
    }

    val lsu_to_dmem = new Bundle {
      val addr = Output(UInt(32.W))
      val store_data = Output(UInt(32.W))
      val mask = Output(UInt(4.W))   
      val wen = Output(Bool())
      val ren = Output(Bool())
    }

    // 调试端口
    val debug = new Bundle {
      val is_load        = Output(Bool())
      val is_store       = Output(Bool())
      val addr           = Output(UInt(32.W))
      val read_origin    = Output(UInt(32.W))
      val final_wb_data  = Output(UInt(32.W))
      val store_mask     = Output(UInt(4.W))
      val store_data_shifted = Output(UInt(32.W))
    }

    val ebreak_out = Output(Bool())
  })

  // 指令类型判断
  val is_load = io.exu_to_lsu.op.is_lb || io.exu_to_lsu.op.is_lh || io.exu_to_lsu.op.is_lw ||
                io.exu_to_lsu.op.is_lbu || io.exu_to_lsu.op.is_lhu
  val is_store = io.exu_to_lsu.op.is_sb || io.exu_to_lsu.op.is_sh || io.exu_to_lsu.op.is_sw
  val addr = io.exu_to_lsu.paddr.addr
  val rd = io.exu_to_lsu.exu_through_lsu_to_wbu.rd

  // ---------- 读数据提取与扩展 ----------
  val read_origin_data = Mux(is_load, io.dmem_to_lsu.load_data, 0.U(32.W))
  val byte_offset = addr(1, 0)
  // 修正 MuxLookup 语法：柯里化调用
  val byte_data = MuxLookup(byte_offset, 0.U(8.W))(
    Seq(
      0.U -> read_origin_data(7,0),
      1.U -> read_origin_data(15,8),
      2.U -> read_origin_data(23,16),
      3.U -> read_origin_data(31,24)
    )
  )
  val halfword_offset = addr(1)
  // halfword_offset 是 Bool，直接用 Mux 选择
  val halfword_data = Mux(halfword_offset, read_origin_data(31,16), read_origin_data(15,0))
  val word_data = read_origin_data(31, 0)

  val lb_data  = Cat(Fill(24, byte_data(7)), byte_data)
  val lh_data  = Cat(Fill(16, halfword_data(15)), halfword_data)
  val lbu_data = Cat(Fill(24, 0.U), byte_data)
  val lhu_data = Cat(Fill(16, 0.U), halfword_data)
  val lw_data  = word_data

  // 透传数据（非 load 指令）
  val touchuan_data = io.exu_to_lsu.exu_through_lsu_to_wbu.grf_wb_data

  // 写回通路
  val final_wb_data = Mux(is_load,
    MuxCase(touchuan_data, Seq(
      io.exu_to_lsu.op.is_lb  -> lb_data,
      io.exu_to_lsu.op.is_lh  -> lh_data,
      io.exu_to_lsu.op.is_lbu -> lbu_data,
      io.exu_to_lsu.op.is_lhu -> lhu_data,
      io.exu_to_lsu.op.is_lw  -> lw_data
    )), touchuan_data)

  io.lsu_to_wbu.rd := rd
  io.lsu_to_wbu.grf_wb_data := final_wb_data

  // ---------- store 数据处理 ----------
  val mask = MuxCase(0.U(4.W), Seq(
    io.exu_to_lsu.op.is_sb -> (1.U(4.W) << byte_offset)(3,0),
    io.exu_to_lsu.op.is_sh -> Mux(halfword_offset === 0.U, "b0011".U(4.W), "b1100".U(4.W)),
    io.exu_to_lsu.op.is_sw -> "b1111".U(4.W)
  ))

  val store_data_shifted = MuxCase(0.U(32.W), Seq(
    io.exu_to_lsu.op.is_sb -> ((io.exu_to_lsu.data.store_data(7,0) << (byte_offset * 8.U))(31,0)),
    io.exu_to_lsu.op.is_sh -> ((io.exu_to_lsu.data.store_data(15,0) << (halfword_offset * 16.U))(31,0)),
    io.exu_to_lsu.op.is_sw -> io.exu_to_lsu.data.store_data
  ))

  // 连接 DMEM 接口
  io.lsu_to_dmem.addr := addr
  io.lsu_to_dmem.store_data := store_data_shifted
  io.lsu_to_dmem.mask := mask
  io.lsu_to_dmem.wen := is_store
  io.lsu_to_dmem.ren := is_load

  // 调试输出
  io.debug.is_load        := is_load
  io.debug.is_store       := is_store
  io.debug.addr           := addr
  io.debug.read_origin    := read_origin_data
  io.debug.final_wb_data  := final_wb_data
  io.debug.store_mask     := mask
  io.debug.store_data_shifted := store_data_shifted

  io.ebreak_out := io.exu_to_lsu.op.is_ebreak
}
package R3262VSA

import chisel3._
import chisel3.util._
import _root_.circt.stage.ChiselStage

class top extends Module {
  val regCount   = 32
  val readvec    = 8
  val writevec   = 4
  val addrwidth  = 5
  val datawidth  = 32

  val io = IO(new Bundle {
    val readAddrs    = Input(Vec(readvec, UInt(addrwidth.W)))
    val readDatas    = Output(Vec(readvec, UInt(datawidth.W)))
    val writeAddrs   = Input(Vec(writevec, UInt(addrwidth.W)))
    val writeDatas   = Input(Vec(writevec, UInt(datawidth.W)))
    val writeEnables = Input(Vec(writevec, Bool()))
  })

  // 寄存器堆本体，初始为零
  val regs = RegInit(VecInit(Seq.fill(regCount)(0.U(datawidth.W))))

  // 计算每个寄存器的下一状态（组合逻辑）
  val nextRegs = Wire(Vec(regCount, UInt(datawidth.W)))
  for (i <- 0 until regCount) {
    // 生成所有写端口对该寄存器的写条件：写使能有效且地址匹配
    val conds = (0 until writevec).map { wp =>
      io.writeEnables(wp) && (io.writeAddrs(wp) === i.U)
    }

    // 为保证高索引写端口优先级更高，将条件和数据列表反转
    // (原代码中写端口索引越大优先级越高，即后面的覆盖前面的)
    val reverseConds  = conds.reverse
    val reverseDatas  = io.writeDatas.reverse

    // 优先级多路选择器：第一个有效条件对应的数据
    val writeData  = PriorityMux(reverseConds.zip(reverseDatas))
    val writeValid = reverseConds.reduce(_ || _)

    // 下一状态：若有写有效则使用写数据，否则保持原值
    nextRegs(i) := Mux(writeValid, writeData, regs(i))
  }

  // 时钟上升沿更新寄存器堆
  for (i <- 0 until regCount) {
    regs(i) := nextRegs(i)
  }

  // 读端口：组合逻辑读出当前寄存器值
  for (rp <- 0 until readvec) {
    io.readDatas(rp) := regs(io.readAddrs(rp))
  }
}

object top extends App {
  ChiselStage.emitSystemVerilogFile(
    new top,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}
// 55nm
//8R4W面积7722 功耗0.006549
//12R5W面积11086 功耗0.009720
//需保证写地址互不相同(软件保证，硬件不管)
//负责地址寄存器0-7

//改进后12r5w 10590 0.00907 / 10R4W 8896 8R4W / 7837 0.00477

//32 12r4w 41576 0.02491w
//32 10r4w 37556 0.02637w
//32 8r4w  33623 0.02404w

// 45nm
//32 8w4w  25265 0.00800w



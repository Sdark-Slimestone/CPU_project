package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== AXI4-Lite UART (设备寄存器) ===========================
// 一个设备寄存器 (地址同旧仿真串口 0xa00003f8): 写入时把 wdata 低 8 位作为字符输出
// (用 printf / $write 依赖仿真环境); 读返回 0。不是完整 UART, 只是总线练习。
class AXI4LiteUART extends Module {
  val io = IO(new Bundle {
    val axi_rd = Flipped(new AXI4LiteRd)
    val axi_wr = Flipped(new AXI4LiteWr)
  })

  // ---- 读通道: 1 深度, 返回 0 ----
  val rPend = RegInit(false.B)
  val rFire = io.axi_rd.arvalid && io.axi_rd.arready
  when (rFire) { rPend := true.B }
  .elsewhen (io.axi_rd.rvalid && io.axi_rd.rready) { rPend := false.B }
  io.axi_rd.arready := !rPend
  io.axi_rd.rvalid  := rPend
  io.axi_rd.rdata   := 0.U
  io.axi_rd.rresp   := AXIResp.OKAY

  // ---- 写通道: 每拍可接收; 输出字符; B 计数保持直到 bready ----
  io.axi_wr.awready := true.B
  io.axi_wr.wready  := true.B
  val wFire = io.axi_wr.awvalid && io.axi_wr.awready && io.axi_wr.wvalid && io.axi_wr.wready
  when (wFire) { printf("%c", io.axi_wr.wdata(7, 0)) }

  val bCnt = RegInit(0.U(4.W))
  val bGo  = io.axi_wr.bvalid && io.axi_wr.bready
  when (wFire && !bGo)     { bCnt := bCnt + 1.U }
  .elsewhen (bGo && !wFire) { bCnt := bCnt - 1.U }
  io.axi_wr.bvalid := bCnt > 0.U
  io.axi_wr.bresp  := AXIResp.OKAY
}

//=================================== AXI4-Lite CLINT ========================================
// 只读 mtime 寄存器 (64 位, 每周期 +1)。读 0xa0000048 -> 低 32 位, 0xa000004c -> 高 32 位
// (地址 bit2 选择)。写忽略但仍回 B。软件需依次读高低 32 位拼成 64 位值。
class AXI4LiteCLINT extends Module {
  val io = IO(new Bundle {
    val axi_rd = Flipped(new AXI4LiteRd)
    val axi_wr = Flipped(new AXI4LiteWr)
  })

  // mtime: 每周期加 1。仿真中用系数把它换算成真实时间 (见 AM timer.c)。
  val mtime = RegInit(0.U(64.W))
  mtime := mtime + 1.U

  // ---- 读通道: 1 深度, 地址 bit2 选低/高 32 位 ----
  val rPend = RegInit(false.B)
  val rData = RegInit(0.U(32.W))
  val rFire = io.axi_rd.arvalid && io.axi_rd.arready
  when (rFire) {
    rPend := true.B
    rData := Mux(io.axi_rd.araddr(2), mtime(63, 32), mtime(31, 0))
  } .elsewhen (io.axi_rd.rvalid && io.axi_rd.rready) { rPend := false.B }
  io.axi_rd.arready := !rPend
  io.axi_rd.rvalid  := rPend
  io.axi_rd.rdata   := rData
  io.axi_rd.rresp   := AXIResp.OKAY

  // ---- 写通道: 忽略, 回 B ----
  io.axi_wr.awready := true.B
  io.axi_wr.wready  := true.B
  val wFire = io.axi_wr.awvalid && io.axi_wr.awready && io.axi_wr.wvalid && io.axi_wr.wready
  val bCnt = RegInit(0.U(4.W))
  val bGo  = io.axi_wr.bvalid && io.axi_wr.bready
  when (wFire && !bGo)     { bCnt := bCnt + 1.U }
  .elsewhen (bGo && !wFire) { bCnt := bCnt - 1.U }
  io.axi_wr.bvalid := bCnt > 0.U
  io.axi_wr.bresp  := AXIResp.OKAY
}

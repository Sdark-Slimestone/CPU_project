package R1322IAeCSR

import chisel3._
import chisel3.util._

//=================================== AXI4-Lite CLINT ========================================
// 只读 mtime 寄存器 (64 位, 每周期 +1)。流片工程里 CLINT 是 CPU 的一个模块 (ysyxSoC 不带它)。
// 地址: CLINT 基址 0x0200_0000, mtime 在 +0xbff8, mtimeh 在 +0xbffc (标准 RISC-V CLINT 布局)。
// 读: 地址 bit2 选择低/高 32 位。写忽略但仍回 B。软件需依次读高低 32 位拼 64 位值。
class AXI4LiteCLINT extends Module {
  val io = IO(new Bundle {
    val axi_rd = Flipped(new AXI4LiteRd)
    val axi_wr = Flipped(new AXI4LiteWr)
  })

  // mtime: 每周期加 1。
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
  io.axi_rd.rlast   := true.B
  io.axi_rd.rid     := 0.U

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
  io.axi_wr.bid    := 0.U
}

class StageConnectBridge extends Module {
  val io = IO(new Bundle {
    // ---- 和上游对接（本模块是接收方）----
    val in_ready = Output(Bool())                  // 本模块驱动：我能收
    val in_valid = Input(Bool())                   // 上游驱动：有数据
    val in_bits  = Input(new IFU_to_IDU_Message)   // 上游驱动：数据

    // ---- 和下游对接（本模块是发送方）----
    val out_ready = Input(Bool())                   // 下游驱动：我能收
    val out_valid = Output(Bool())                  // 本模块驱动：有数据
    val out_bits  = Output(new IFU_to_IDU_Message)  // 本模块驱动：数据
  })

  // 1 bit 寄存器：bits 里有没有有效数据（和你的 pcReg 同款写法）
  val valid = RegInit(false.B)
  // 存消息的寄存器（和 Reg(UInt(32.W)) 同款，只是类型换成 Bundle）
  val bits  = Reg(new IFU_to_IDU_Message)

  // 组合赋值（无时钟，立即生效）
  io.in_ready  := !valid
  io.out_valid := valid
  io.out_bits  := bits

  // 把 fire 展开
  val in_fire  = io.in_valid && io.in_ready
  val out_fire = io.out_valid && io.out_ready

  // 时钟沿：条件更新寄存器（和你的 when 同款）
  when (in_fire) {
    bits  := io.in_bits
    valid := true.B
  } .elsewhen (out_fire) {
    valid := false.B
  }
}


//有语法糖
class StageConnectBridge[T <: Data](gen: T) extends Module {
  val io = IO(new Bundle {
    val in  = Flipped(Decoupled(gen))
    val out = Decoupled(gen)
  })

  val valid = RegInit(false.B)
  val bits  = Reg(gen)

  io.in.ready  := !valid        // 空才能收 -> 不重叠, 这就是多周期
  io.out.valid := valid
  io.out.bits  := bits

  when (io.in.fire) {
    bits  := io.in.bits
    valid := true.B
  } .elsewhen (io.out.fire) {
    valid := false.B
  }
}
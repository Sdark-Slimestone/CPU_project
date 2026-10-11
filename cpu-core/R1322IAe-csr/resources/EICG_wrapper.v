// 行为级集成时钟门 (ICG) 单元 (防毛刺: 时钟低电平期间锁存使能)。
// firtool 会把 chisel3.util.circt.ClockGate(clock, en) 降成对 EICG_wrapper 的例化,
// 工具链不带该技术单元库, 故这里给等价的行为模型用于 (功能) 仿真。
// 说明: 兼容层配套文件, 不涉及本体 RTL 逻辑。
module EICG_wrapper (
  input  wire in,
  input  wire en,
  input  wire test_en,
  output wire out
);
  reg en_latch;
  always @(*) begin
    if (!in) en_latch = en | test_en;   // 时钟低电平期间锁存使能, 上升沿不再有毛刺
  end
  assign out = in & en_latch;
endmodule

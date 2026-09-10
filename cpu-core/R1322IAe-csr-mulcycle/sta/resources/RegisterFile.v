// 寄存器阵列存储器: 256x32b (1KB), 1个写字节使能端口 + 2个组合读端口
// 用触发器实现, 当前周期组合读返回数据 (参考讲义"评估单周期NPC的主频":
// 像寄存器堆那样通过触发器实现存储器)
module RegisterFile (
  input         io_clk,
  input         wen,
  input  [7:0]  waddr,
  input  [31:0] wdata,
  input  [3:0]  wmask,
  input  [7:0]  raddr1,
  output [31:0] rdata1,
  input  [7:0]  raddr2,
  output [31:0] rdata2
);

  reg [31:0] mem [0:255];

  // 同步写, 字节使能
  always @(posedge io_clk) begin
    if (wen) begin
      if (wmask[0]) mem[waddr][7:0]   <= wdata[7:0];
      if (wmask[1]) mem[waddr][15:8]  <= wdata[15:8];
      if (wmask[2]) mem[waddr][23:16] <= wdata[23:16];
      if (wmask[3]) mem[waddr][31:24] <= wdata[31:24];
    end
  end

  // 组合读: 当前周期返回
  assign rdata1 = mem[raddr1];
  assign rdata2 = mem[raddr2];

endmodule

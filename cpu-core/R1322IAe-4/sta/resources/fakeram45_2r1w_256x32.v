// 双端口同步 SRAM 宏 (2R1W) 黑盒声明, 时序/面积见
//   yosys-sta/pdk/nangate45/lib/fakeram45_2r1w_256x32.lib
// 256x32b: 2 个独立读口 (raddr1/rdata1, raddr2/rdata2) + 1 个写口 (waddr_in/...),
// 同步读。用于 dmem (双发射 load + store)。
(* blackbox *)
module fakeram45_2r1w_256x32 (
  input         clk,
  input         we_in,
  input         ce_in,
  input  [7:0]  waddr_in,
  input  [31:0] wd_in,
  input  [31:0] w_mask_in,
  input  [7:0]  raddr1,
  output [31:0] rdata1,
  input  [7:0]  raddr2,
  output [31:0] rdata2
);
endmodule

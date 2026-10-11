// 单端口同步 SRAM 宏 (1R1W) 黑盒声明, 时序/面积见
//   yosys-sta/pdk/nangate45/lib/fakeram45_256x34.lib
// 综合时作为硬宏保留 (不会被 memory_map 展开成触发器阵列)。
// 仅用于 make sta 的时序/面积评估。
(* blackbox *)
module fakeram45_256x34 (
  input         clk,
  input  [7:0]  addr_in,
  input  [33:0] wd_in,
  input  [33:0] w_mask_in,
  input         we_in,
  input         ce_in,
  output [33:0] rd_out
);
endmodule

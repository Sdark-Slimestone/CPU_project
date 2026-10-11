// 单端口同步 SRAM 宏 (64x96) 黑盒声明, 时序/面积见
//   yosys-sta/pdk/nangate45/lib/fakeram45_64x96.lib
(* blackbox *)
module fakeram45_64x96 (
  input         clk,
  input  [5:0]  addr_in,
  input  [95:0] wd_in,
  input  [95:0] w_mask_in,
  input         we_in,
  input         ce_in,
  output [95:0] rd_out
);
endmodule

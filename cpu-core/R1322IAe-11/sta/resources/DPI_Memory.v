// STA 版存储器占位: 存储器的实现 (DPI-C / SRAM) **不在评估范围内**。
// 这里只对 AXI4-Lite 接口做最简终结 (组合, 无存储阵列、无时序), 使综合/STA 反映的是
// "NPC + AXI4-Lite 仲裁器" 的逻辑 (接口握手 + 仲裁), 而不是存储器内部。
// 端口与 resources/DPI_Memory.v 完全一致。
module DPI_Memory #(
    parameter integer LATENCY = 1,
    parameter integer DEPTH   = 1
) (
    input         io_clk,

    input         ar_valid,
    output        ar_ready,
    input  [31:0] ar_addr,
    input  [2:0]  ar_prot,
    output        r_valid,
    input         r_ready,
    output [31:0] r_data,
    output [1:0]  r_resp,

    input         aw_valid,
    output        aw_ready,
    input  [31:0] aw_addr,
    input  [2:0]  aw_prot,
    input         w_valid,
    output        w_ready,
    input  [31:0] w_data,
    input  [3:0]  w_strb,
    output        b_valid,
    input         b_ready,
    output [1:0]  b_resp,

    input         flush,
    input         ebreak,
    output [31:0] dbg_a1,
    output [8:0]  dbg_d1,
    output [8:0]  dbg_cnt1,
    output        dbg_hd1
);
    // 立即就绪 + 立即响应 (无存储, 无延迟): 只保留握手通路, 存储器内部不计入。
    assign ar_ready = 1'b1;
    assign r_valid  = ar_valid;
    assign r_data   = 32'd0;
    assign r_resp   = 2'b00;

    assign aw_ready = 1'b1;
    assign w_ready  = 1'b1;
    assign b_valid  = aw_valid & w_valid;
    assign b_resp   = 2'b00;

    assign dbg_a1   = 32'd0;
    assign dbg_d1   = 9'd0;
    assign dbg_cnt1 = 9'd0;
    assign dbg_hd1  = 1'b0;
endmodule

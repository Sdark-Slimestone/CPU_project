// SimpleBus 存储器: 每读口一条可停顿的请求流水线, 延迟 LATENCY 拍。
//   reqValid* : master 有读请求; raddr*/rdata* : 读地址/数据
//   respValid*: slave 响应有效(保持到 respReady*); respReady*: master 能接收响应
//   flush     : 重定向时冲掉在途读请求(防止取指流水的深度对齐被破坏)
//   wen/waddr/wdata/wmask: 写通道(立即完成, 不阻塞)
// LATENCY=1 时行为与最初的"同步读 1 拍"完全一致。
module DPI_Memory #(
    parameter integer LATENCY = 1
) (
    input         wen,
    input  [31:0] waddr,
    input  [31:0] wdata,
    input  [3:0]  wmask,
    input         reqValid1,
    input  [31:0] raddr1,
    output [31:0] rdata1,
    output        respValid1,
    input         respReady1,
    input         reqValid2,
    input  [31:0] raddr2,
    output [31:0] rdata2,
    output        respValid2,
    input         respReady2,
    input         flush,
    input         ebreak,
    input         io_clk
);
    import "DPI-C" function int  pmem_read(input int addr);
    import "DPI-C" function void pmem_write(input int addr, input int data, input byte mask);
    import "DPI-C" function void sim_finish();

    // 请求流水 (深度 LATENCY; LATENCY=1 时不用, 直接读)
    reg [31:0] a1_pipe [0:LATENCY-1];
    reg [31:0] a2_pipe [0:LATENCY-1];
    reg        v1_pipe [0:LATENCY-1];
    reg        v2_pipe [0:LATENCY-1];

    reg [31:0] rdata1_r, rdata2_r;
    reg        respValid1_r, respValid2_r;

    integer k;

    // 输出可前进: 无待接收响应, 或已被接收
    wire adv1 = !respValid1_r || respReady1;
    wire adv2 = !respValid2_r || respReady2;

    always @(posedge io_clk) begin
        if (flush) begin
            // 冲掉在途读请求与输出
            for (k = 0; k < LATENCY; k = k + 1) begin
                v1_pipe[k] <= 1'b0;
                v2_pipe[k] <= 1'b0;
            end
            respValid1_r <= 1'b0;
            respValid2_r <= 1'b0;
        end else if (LATENCY == 1) begin
            if (adv1) begin
                rdata1_r <= reqValid1 ? pmem_read(raddr1) : 32'b0;
                respValid1_r <= reqValid1;
            end
            if (adv2) begin
                rdata2_r <= reqValid2 ? pmem_read(raddr2) : 32'b0;
                respValid2_r <= reqValid2;
            end
        end else begin
            if (adv1) begin
                v1_pipe[0] <= reqValid1;  a1_pipe[0] <= raddr1;
                for (k = 1; k < LATENCY; k = k + 1) begin
                    v1_pipe[k] <= v1_pipe[k-1];  a1_pipe[k] <= a1_pipe[k-1];
                end
                respValid1_r <= v1_pipe[LATENCY-2];
                rdata1_r <= v1_pipe[LATENCY-2] ? pmem_read(a1_pipe[LATENCY-2]) : 32'b0;
            end
            if (adv2) begin
                v2_pipe[0] <= reqValid2;  a2_pipe[0] <= raddr2;
                for (k = 1; k < LATENCY; k = k + 1) begin
                    v2_pipe[k] <= v2_pipe[k-1];  a2_pipe[k] <= a2_pipe[k-1];
                end
                respValid2_r <= v2_pipe[LATENCY-2];
                rdata2_r <= v2_pipe[LATENCY-2] ? pmem_read(a2_pipe[LATENCY-2]) : 32'b0;
            end
        end
        if (wen) begin
            pmem_write(waddr, wdata, {4'b0, wmask});   // 4位掩码扩展到8位
        end
    end

    assign rdata1     = rdata1_r;
    assign rdata2     = rdata2_r;
    assign respValid1 = respValid1_r;
    assign respValid2 = respValid2_r;

    // 仿真结束检测：组合逻辑
    always @(*) begin
        if (ebreak) begin
            sim_finish();
        end
    end
endmodule

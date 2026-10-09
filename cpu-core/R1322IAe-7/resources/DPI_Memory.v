// SimpleBus 存储器 (可变/随机读延迟)。
// 每读口: reqValid/reqReady 握手接收请求; 内部 FIFO(DEPTH) 保存 {addr, delay};
//         delay 每拍减 1; 队头 delay==0 时 respValid=1、rdata=pmem_read(head.addr),
//         一直保持到 respReady 才弹出。响应按序返回, 因此支持任意/随机每请求延迟。
//   RANDOM=0: delay = LATENCY (固定)
//   RANDOM=1: delay = 1..MAXLAT (每请求由 LFSR 随机)
// flush: 清空在途请求与输出。
// 写通道: 立即完成 (组合 pmem_write), 不参与握手。
// LATENCY=1 且 RANDOM=0 且 DEPTH=1 时, 行为与最初「同步读 1 拍」一致。
module vmem_rdport #(
    parameter integer DEPTH   = 1,
    parameter integer LATENCY = 1,
    parameter integer RANDOM  = 0,
    parameter integer MAXLAT  = 1,
    parameter integer SEED    = 16'hACE1
) (
    input         clk,
    input         flush,
    input         reqValid,
    input  [31:0] raddr,
    input  [15:0] reqTag,
    output        reqReady,
    output [31:0] rdata,
    output [15:0] respTag,
    output        respValid,
    input         respReady,
    output [31:0] dbg_a0,
    output [8:0]  dbg_d0,
    output [8:0]  dbg_cnt,
    output        dbg_hd
);
    import "DPI-C" function int pmem_read(input int addr);

    reg [31:0] a  [0:DEPTH-1];
    reg [31:0] an [0:DEPTH-1];
    reg [15:0] t  [0:DEPTH-1];
    reg [15:0] tn [0:DEPTH-1];
    reg [8:0]  d  [0:DEPTH-1];
    reg [8:0]  dn [0:DEPTH-1];
    reg        v  [0:DEPTH-1];
    reg        vn [0:DEPTH-1];
    reg [8:0]  cnt;
    reg [15:0] lfsr;

    integer i;

    initial begin
        cnt  = 9'd0;
        lfsr = SEED;
        for (i = 0; i < DEPTH; i = i + 1) begin
            v[i] = 1'b0; d[i] = 9'd0; a[i] = 32'd0; t[i] = 16'd0;
        end
    end

    wire empty     = (cnt == 0);
    wire head_done = !empty && (d[0] == 0);
    assign reqReady  = (cnt < DEPTH);
    assign respValid = head_done;
    assign rdata     = head_done ? pmem_read(a[0]) : 32'b0;
    assign respTag   = t[0];

    wire do_pop  = respValid && respReady;
    wire do_push = reqValid && reqReady;

    // 压入位置: 同拍弹出则接在左移后的末尾
    localparam integer IDXW = (DEPTH <= 1) ? 1 : $clog2(DEPTH);
    wire [IDXW-1:0] push_idx = (do_push) ? (do_pop ? (cnt[IDXW-1:0] - 1'b1) : cnt[IDXW-1:0]) : {IDXW{1'b0}};

    // 随机延迟: 1..MAXLAT
    wire [8:0] rnd  = (MAXLAT <= 1) ? 9'd1 : (9'd1 + (lfsr % MAXLAT));
    wire [8:0] newd = (RANDOM == 1) ? rnd : LATENCY;
    assign dbg_a0 = a[0];
    assign dbg_d0 = d[0];
    assign dbg_cnt = cnt;
    assign dbg_hd = head_done;

    // 计算下一拍状态 (地址/延迟/有效位), 与顺序块用定长索引镜像
    always @(*) begin
        for (i = 0; i < DEPTH; i = i + 1) begin
            vn[i] = v[i];
            dn[i] = (v[i] && (d[i] != 0)) ? (d[i] - 9'd1) : d[i];
            an[i] = a[i];
            tn[i] = t[i];
        end
        // 弹出: 整体左移
        if (do_pop) begin
            for (i = 0; i < DEPTH-1; i = i + 1) begin
                vn[i] = v[i+1];
                dn[i] = (v[i+1] && (d[i+1] != 0)) ? (d[i+1] - 9'd1) : d[i+1];
                an[i] = a[i+1];
                tn[i] = t[i+1];
            end
            vn[DEPTH-1] = 1'b0;
            dn[DEPTH-1] = 9'd0;
            an[DEPTH-1] = 32'd0;
            tn[DEPTH-1] = 16'd0;
        end
        // 压入 (放在有效末尾)
        if (do_push) begin
            vn[push_idx] = 1'b1;
            dn[push_idx] = newd;
            an[push_idx] = raddr;
            tn[push_idx] = reqTag;
        end
    end

    always @(posedge clk) begin
        if (flush) begin
            cnt  <= 9'd0;
            lfsr <= SEED;
            for (i = 0; i < DEPTH; i = i + 1) v[i] <= 1'b0;
        end else begin
            cnt <= cnt - (do_pop ? 9'd1 : 9'd0) + (do_push ? 9'd1 : 9'd0);
            for (i = 0; i < DEPTH; i = i + 1) begin
                v[i] <= vn[i];
                d[i] <= dn[i];
                a[i] <= an[i];
                t[i] <= tn[i];
            end
            lfsr <= {lfsr[14:0], lfsr[15] ^ lfsr[13] ^ lfsr[12] ^ lfsr[10]};
        end
    end
endmodule

module DPI_Memory #(
    parameter integer LATENCY = 1,
    parameter integer RANDOM  = 0,
    parameter integer MAXLAT  = 1,
    parameter integer DEPTH1  = 1,
    parameter integer DEPTH2  = 1
) (
    input         wen,
    input  [31:0] waddr,
    input  [31:0] wdata,
    input  [3:0]  wmask,
    input         reqValid1,
    input  [31:0] raddr1,
    input  [15:0] reqTag1,
    output        reqReady1,
    output [31:0] rdata1,
    output [15:0] respTag1,
    output        respValid1,
    input         respReady1,
    input         reqValid2,
    input  [31:0] raddr2,
    input  [15:0] reqTag2,
    output        reqReady2,
    output [31:0] rdata2,
    output [15:0] respTag2,
    output        respValid2,
    input         respReady2,
    input         flush,
    input         ebreak,
    input         io_clk,
    output [31:0] dbg_a1,
    output [8:0]  dbg_d1,
    output [8:0]  dbg_cnt1,
    output        dbg_hd1
);
    import "DPI-C" function void pmem_write(input int addr, input int data, input byte mask);
    import "DPI-C" function void sim_finish();

    vmem_rdport #(.DEPTH(DEPTH1), .LATENCY(LATENCY), .RANDOM(RANDOM), .MAXLAT(MAXLAT), .SEED(16'hACE1))
      p1 (
        .clk(io_clk), .flush(flush),
        .reqValid(reqValid1), .raddr(raddr1), .reqTag(reqTag1), .reqReady(reqReady1),
        .rdata(rdata1), .respTag(respTag1), .respValid(respValid1), .respReady(respReady1),
        .dbg_a0(dbg_a1), .dbg_d0(dbg_d1), .dbg_cnt(dbg_cnt1), .dbg_hd(dbg_hd1)
      );

    vmem_rdport #(.DEPTH(DEPTH2), .LATENCY(LATENCY), .RANDOM(RANDOM), .MAXLAT(MAXLAT), .SEED(16'h5EED))
      p2 (
        .clk(io_clk), .flush(flush),
        .reqValid(reqValid2), .raddr(raddr2), .reqTag(reqTag2), .reqReady(reqReady2),
        .rdata(rdata2), .respTag(respTag2), .respValid(respValid2), .respReady(respReady2)
      );

    always @(posedge io_clk) begin
        if (wen) begin
            pmem_write(waddr, wdata, {4'b0, wmask});   // 4位掩码扩展到8位
        end
    end

    // 仿真结束检测: 组合逻辑
    always @(*) begin
        if (ebreak) begin
            sim_finish();
        end
    end
endmodule

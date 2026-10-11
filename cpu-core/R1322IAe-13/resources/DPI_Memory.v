// AXI4-Lite 存储器 (固定读延迟), 单个 AXI4-Lite slave 接口 (IFU/LSU 经仲裁器共享)。
// 读事务 (AR + R):
//   AR 握手 (arvalid/arready) 接收 {addr}; 内部 FIFO(DEPTH) 保存, delay 每拍减 1;
//   队头 delay==0 时 rvalid=1、rdata=pmem_read(队头addr), 保持到 rready 才弹出。
//   响应按序返回。
// 写事务 (AW + W + B): AW 与 W 独立握手, 两笔都到齐后在时钟沿写存储并回 B。
//   - aw_ready/w_ready 只在对应保持寄存器为空时拉高 (不依赖 valid, 无死锁);
//   - b_valid 为寄存器输出 (不依赖 b_ready); 一笔写未完成时不再接收新的 AW/W, 保证写按序。
// LATENCY: 固定读延迟 (拍)。flush: 清空在途读请求 (写状态不冲, 避免丢 B)。
module axi_rd_ch #(
    parameter integer DEPTH   = 1,
    parameter integer LATENCY = 1
) (
    input         clk,
    input         flush,
    // AR 通道
    input         arvalid,
    output        arready,
    input  [31:0] araddr,
    // R 通道
    output        rvalid,
    input         rready,
    output [31:0] rdata,
    output [1:0]  rresp,
    // debug
    output [31:0] dbg_a0,
    output [8:0]  dbg_d0,
    output [8:0]  dbg_cnt,
    output        dbg_hd
);
    import "DPI-C" function int pmem_read(input int addr);

    reg [31:0] a  [0:DEPTH-1];
    reg [31:0] an [0:DEPTH-1];
    reg [8:0]  d  [0:DEPTH-1];
    reg [8:0]  dn [0:DEPTH-1];
    reg        v  [0:DEPTH-1];
    reg        vn [0:DEPTH-1];
    reg [8:0]  cnt;

    integer i;

    initial begin
        cnt  = 9'd0;
        for (i = 0; i < DEPTH; i = i + 1) begin
            v[i] = 1'b0; d[i] = 9'd0; a[i] = 32'd0;
        end
    end

    wire empty     = (cnt == 0);
    wire head_done = !empty && (d[0] == 0);
    assign arready = (cnt < DEPTH);
    assign rvalid  = head_done;
    assign rdata   = head_done ? pmem_read(a[0]) : 32'b0;
    assign rresp   = 2'b00;   // OKAY

    wire do_pop  = rvalid && rready;
    wire do_push = arvalid && arready;

    localparam integer IDXW = (DEPTH <= 1) ? 1 : $clog2(DEPTH);
    wire [IDXW-1:0] push_idx = (do_push) ? (do_pop ? (cnt[IDXW-1:0] - 1'b1) : cnt[IDXW-1:0]) : {IDXW{1'b0}};

    wire [8:0] newd = LATENCY;

    assign dbg_a0 = a[0];
    assign dbg_d0 = d[0];
    assign dbg_cnt = cnt;
    assign dbg_hd = head_done;

    // 计算下一拍状态 (组合镜像, 避免 Verilator 动态下标写失效)
    always @(*) begin
        for (i = 0; i < DEPTH; i = i + 1) begin
            vn[i] = v[i];
            dn[i] = (v[i] && (d[i] != 0)) ? (d[i] - 9'd1) : d[i];
            an[i] = a[i];
        end
        if (do_pop) begin
            for (i = 0; i < DEPTH-1; i = i + 1) begin
                vn[i] = v[i+1];
                dn[i] = (v[i+1] && (d[i+1] != 0)) ? (d[i+1] - 9'd1) : d[i+1];
                an[i] = a[i+1];
            end
            vn[DEPTH-1] = 1'b0;
            dn[DEPTH-1] = 9'd0;
            an[DEPTH-1] = 32'd0;
        end
        if (do_push) begin
            vn[push_idx] = 1'b1;
            dn[push_idx] = newd;
            an[push_idx] = araddr;
        end
    end

    always @(posedge clk) begin
        if (flush) begin
            cnt  <= 9'd0;
            for (i = 0; i < DEPTH; i = i + 1) v[i] <= 1'b0;
        end else begin
            cnt <= cnt - (do_pop ? 9'd1 : 9'd0) + (do_push ? 9'd1 : 9'd0);
            for (i = 0; i < DEPTH; i = i + 1) begin
                v[i] <= vn[i];
                d[i] <= dn[i];
                a[i] <= an[i];
            end
        end
    end
endmodule

module DPI_Memory #(
    parameter integer LATENCY = 1,
    parameter integer DEPTH   = 1
) (
    input         io_clk,

    // ---- AXI4-Lite 读通道 (AR + R) ----
    input         ar_valid,
    output        ar_ready,
    input  [31:0] ar_addr,
    input  [2:0]  ar_prot,
    output        r_valid,
    input         r_ready,
    output [31:0] r_data,
    output [1:0]  r_resp,

    // ---- AXI4-Lite 写通道 (AW + W + B) ----
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
    import "DPI-C" function void pmem_write(input int addr, input int data, input byte mask);
    import "DPI-C" function void sim_finish();

    axi_rd_ch #(.DEPTH(DEPTH), .LATENCY(LATENCY))
      p1 (
        .clk(io_clk), .flush(flush),
        .arvalid(ar_valid), .arready(ar_ready), .araddr(ar_addr),
        .rvalid(r_valid), .rready(r_ready), .rdata(r_data), .rresp(r_resp),
        .dbg_a0(dbg_a1), .dbg_d0(dbg_d1), .dbg_cnt(dbg_cnt1), .dbg_hd(dbg_hd1)
      );

    // ================= AXI4-Lite 写通道 (AW + W -> 写存储 -> B) =================
    reg        aw_hold_v;
    reg [31:0] aw_hold_a;
    reg        w_hold_v;
    reg [31:0] w_hold_d;
    reg [3:0]  w_hold_s;
    reg        b_hold_v;

    assign aw_ready = !aw_hold_v;
    assign w_ready  = !w_hold_v;

    wire aw_fire = aw_valid && aw_ready;
    wire w_fire  = w_valid  && w_ready;
    wire have_aw = aw_hold_v || aw_fire;
    wire have_w  = w_hold_v  || w_fire;
    wire do_write = have_aw && have_w;

    wire [31:0] eff_a = aw_hold_v ? aw_hold_a : aw_addr;
    wire [31:0] eff_d = w_hold_v  ? w_hold_d  : w_data;
    wire [3:0]  eff_s = w_hold_v  ? w_hold_s  : w_strb;

    assign b_valid = b_hold_v;
    assign b_resp  = 2'b00;   // OKAY

    initial begin
        aw_hold_v = 1'b0; w_hold_v = 1'b0; b_hold_v = 1'b0;
        aw_hold_a = 32'd0; w_hold_d = 32'd0; w_hold_s = 4'd0;
    end

    always @(posedge io_clk) begin
        if (flush) begin
            aw_hold_v <= 1'b0; w_hold_v <= 1'b0; b_hold_v <= 1'b0;
        end else begin
            // 写事务: AW/W 齐备即写, 并置 B。
            if (do_write) begin
                pmem_write(eff_a, eff_d, {4'b0, eff_s});
                b_hold_v <= 1'b1;
            end
            if (b_hold_v && b_ready && !do_write) b_hold_v <= 1'b0;

            if (aw_hold_v) begin
                if (do_write) aw_hold_v <= 1'b0;
            end else if (aw_fire && !do_write) begin
                aw_hold_v <= 1'b1; aw_hold_a <= aw_addr;
            end
            if (w_hold_v) begin
                if (do_write) w_hold_v <= 1'b0;
            end else if (w_fire && !do_write) begin
                w_hold_v <= 1'b1; w_hold_d <= w_data; w_hold_s <= w_strb;
            end
        end
    end

    // 仿真结束检测: 组合逻辑
    always @(*) begin
        if (ebreak) begin
            sim_finish();
        end
    end
endmodule

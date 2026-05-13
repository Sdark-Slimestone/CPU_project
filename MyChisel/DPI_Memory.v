module DPI_Memory (
    input         wen,           // 写使能
    input  [31:0] waddr,         // 写地址
    input  [31:0] wdata,         // 写数据
    input  [3:0]  wmask,         // 写字节掩码
    input         ren1,          // 读端口1使能
    input  [31:0] raddr1,        // 读端口1地址
    output [31:0] rdata1,        // 读端口1数据
    input         ren2,          // 读端口2使能
    input  [31:0] raddr2,        // 读端口2地址
    output [31:0] rdata2,        // 读端口2数据
    input         ebreak,        // 仿真结束标志
    input         io_clk         // 时钟
);
    import "DPI-C" function int pmem_read(input int addr);
    import "DPI-C" function void pmem_write(input int addr, input int data, input byte mask);
    import "DPI-C" function void sim_finish();

    // 读端口1：组合逻辑
    assign rdata1 = ren1 ? pmem_read(raddr1) : 32'b0;

    // 读端口2：组合逻辑
    assign rdata2 = ren2 ? pmem_read(raddr2) : 32'b0;

    // 仿真结束检测：组合逻辑
    always @(*) begin
        if (ebreak) begin
            sim_finish();
        end
    end

    // 写操作：同步于时钟上升沿
    always @(posedge io_clk) begin
        if (wen) begin
            pmem_write(waddr, wdata, {4'b0, wmask});   // 4位掩码扩展到8位
        end
    end
endmodule

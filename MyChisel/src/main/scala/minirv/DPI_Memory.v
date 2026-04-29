module DPI_Memory (
    input        wen,
    input  [31:0] waddr,
    input  [31:0] wdata,
    input  [3:0]  wmask,
    input        ren,
    input  [31:0] raddr,
    output [31:0] rdata,
    input        ebreak,
    input        io_clk
);
    import "DPI-C" function int pmem_read(input int addr);
    import "DPI-C" function void pmem_write(input int addr, input int data, input byte mask);
    import "DPI-C" function void sim_finish();

    // 读操作：组合逻辑
    assign rdata = ren ? pmem_read(raddr) : 32'b0;

    // ebreak 检测：组合逻辑
    always @(*) begin
        if (ebreak) begin
            sim_finish();
        end
    end

    // 写操作：同步于时钟上升沿
    always @(posedge io_clk) begin
        if (wen) begin
            pmem_write(waddr, wdata, {4'b0, wmask});   // 4位扩展到8位
        end
    end
endmodule
module DPI_Memory (
    input        clk,
    input        wen,
    input  [31:0] waddr,
    input  [31:0] wdata,
    input  [3:0]  wmask,
    input        ren,
    input  [31:0] raddr,
    output [31:0] rdata,
    input        ebreak
);
    import "DPI-C" function int pmem_read(input int addr);
    import "DPI-C" function void pmem_write(input int addr, input int data, input byte mask);
    import "DPI-C" function void sim_finish();


    assign rdata = ren ? pmem_read(raddr) : 32'b0;
    always @(posedge clk) begin
        if (wen) begin
            pmem_write(waddr, wdata, wmask);
        end
    end

    always @(*) begin
        if (ebreak) begin
            sim_finish();
        end
    end
endmodule
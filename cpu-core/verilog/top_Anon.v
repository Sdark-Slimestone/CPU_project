module top_Anon (
    input        wen,
    input  [31:0] waddr,
    input  [31:0] wdata,
    input  [3:0]  wmask,
    input        ren,
    input  [31:0] raddr,
    output [31:0] rdata,
    input        ebreak
);
    DPI_Memory inst (
        .wen(wen),
        .waddr(waddr),
        .wdata(wdata),
        .wmask(wmask),
        .ren(ren),
        .raddr(raddr),
        .rdata(rdata),
        .ebreak(ebreak)
    );
endmodule
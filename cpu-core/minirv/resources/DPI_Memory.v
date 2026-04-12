module DPI_Memory (
    input        wen,      // 写使能
    input  [31:0] waddr,   // 写地址
    input  [31:0] wdata,   // 写数据
    input  [3:0]  wmask,   // 写字节掩码
    input        ren,      // 读使能
    input  [31:0] raddr,   // 读地址
    output [31:0] rdata,   // 读出的数据
    input        ebreak    // 新增：来自CPU的ebreak信号
);
    // DPI-C函数声明，全部使用无符号类型
    import "DPI-C" function int pmem_read(input int addr);
    import "DPI-C" function void pmem_write(input int addr, input int data, input byte mask);
    import "DPI-C" function void sim_finish();

    // 读操作：组合逻辑返回
    assign rdata = ren ? pmem_read(raddr) : 32'b0;

    // 写操作和ebreak检测：组合逻辑
    always @(*) begin
        if (wen) begin
            // verilator lint_off WIDTHEXPAND
            pmem_write(waddr, wdata, wmask);
            // verilator lint_on WIDTHEXPAND
        end
        if (ebreak) begin
            sim_finish();
        end
    end
endmodule
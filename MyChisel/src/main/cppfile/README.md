## 使用时可以和sim_main 互换 这个方便debug
# 等生成verilog后把这个cpp文件替换过去，在verilog里执行下列命令
# 命令改成：verilator -Wall -Wno-fatal --cc --exe --build -j 0 --top-module top top.sv DPI_Memory.v main_debug.cpp -o npc
## 使用时把整个文件夹改名为main搬到：cpu_project/MyChisel/src
# 等生成verilog后把cpp文件和resource里的.v文件复制过去，在verilog里执行下列命令
# verilator -Wall -Wno-fatal --cc --exe --build -j 0 --top-module top top.sv DPI_Memory.v sim_main.cpp -o npc
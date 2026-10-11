#!/bin/bash
# 构建 R1322IAe-csr (手写单周期核) 接入 ysyxSoC 的 Verilator 仿真。
# 核心 RTL 由 gen_core.sh 生成到 soc/rtl (SoC 兼容顶层 ysyx_26030107 + AXI 桥 + EICG)。
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
YSYXSOC=/home/sdark/ysyx-workbench/ysyxSoC
PERIP="$YSYXSOC/perip"

bash "$HERE/gen_core.sh"

TOP=ysyxSoCTop
OBJ="$HERE/obj_dir"
rm -rf "$OBJ"

verilator -cc --exe --build -j 0 \
    --top-module "$TOP" \
    --timescale "1ns/1ns" --no-timing \
    --autoflush -Wno-fatal -Wno-lint -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNOPTFLAT \
    -I"$PERIP/uart16550/rtl" -I"$PERIP/spi/rtl" \
    "$HERE"/rtl/*.sv "$HERE"/rtl/*.v \
    "$HERE"/ysyxSoCFull.v \
    $(find "$PERIP" -name '*.v') \
    "$HERE"/csrc/sim_main_soc.cpp \
    -o sim_soc \
    -Mdir "$OBJ" 2>&1 | tail -20

echo "built: $OBJ/sim_soc"

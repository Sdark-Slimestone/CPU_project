#!/bin/bash
# 构建 ysyxSoC + ysyx_26030107 的 Verilator 仿真, 顶层 ysyxSoCFull (带外设引脚)。
# 用于在没有 NVBoard GUI 的环境下通过引脚级仿真验证 GPIO/PS2/VGA/UART。
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
YSYXSOC=/home/sdark/ysyx-workbench/ysyxSoC
PERIP="$YSYXSOC/perip"

TOP=ysyxSoCFull
OBJ="$HERE/obj_dir_full_flash"
rm -rf "$OBJ"

verilator -cc --exe --build -j 0 \
    --top-module "$TOP" \
    --timescale "1ns/1ns" --no-timing \
    --autoflush -Wno-fatal -Wno-lint -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNOPTFLAT \
    -I"$PERIP/uart16550/rtl" -I"$PERIP/spi/rtl" \
    "$HERE"/rtl_flash/*.sv \
    "$HERE"/ysyxSoCFull.v \
    $(find "$PERIP" -name '*.v') \
    "$HERE"/csrc/sim_main_soc_full.cpp \
    -o sim_soc_full \
    -Mdir "$OBJ" 2>&1 | tail -40

echo "built: $OBJ/sim_soc_full"

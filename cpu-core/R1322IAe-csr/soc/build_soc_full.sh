#!/bin/bash
# 构建 csr 核 + ysyxSoCFull (带外设引脚) 的 Verilator 仿真, 用于 UART RX 注入等。
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
YSYXSOC=/home/sdark/ysyx-workbench/ysyxSoC
PERIP="$YSYXSOC/perip"
bash "$HERE/gen_core.sh"
TOP=ysyxSoCFull
OBJ="$HERE/obj_dir_full"
rm -rf "$OBJ"
verilator -cc --exe --build -j 0 --top-module "$TOP" \
  --timescale "1ns/1ns" --no-timing --autoflush \
  -Wno-fatal -Wno-lint -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNOPTFLAT \
  -I"$PERIP/uart16550/rtl" -I"$PERIP/spi/rtl" \
  "$HERE"/rtl/*.sv "$HERE"/rtl/*.v \
  "$HERE"/ysyxSoCFull.v \
  $(find "$PERIP" -name '*.v') \
  "$HERE"/csrc/sim_main_soc_full.cpp \
  -o sim_soc_full -Mdir "$OBJ" 2>&1 | tail -8
echo "built: $OBJ/sim_soc_full"

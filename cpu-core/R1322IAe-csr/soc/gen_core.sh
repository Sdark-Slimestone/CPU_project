#!/bin/bash
# 从核心源码 (../scala) 生成"接入 ysyxSoC 的兼容顶层" RTL, 安装到 soc/rtl。
# 兼容层见 ../scala/soc_compat.scala (本体子模块 + AXI 桥 + 时钟门控)。
# 本体源码基本不改; 仅 top.scala(开关)/ifu.scala(BootCfg)/lsu.scala(load 宽度) 有
# 注释包裹的小改。
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
CORE_DIR="$(cd "$HERE/.." && pwd)"
ROOT="$(cd "$CORE_DIR/../.." && pwd)"
CORE="$(basename "$CORE_DIR")"
RST_PC="${YSYX_RESET_PC:-20000000}"          # 默认 0x2000_0000 (MROM)
STAMP="$HERE/rtl/.core_stamp"

force=0; [ "$1" = "-f" ] && force=1
need=1
if [ "$force" = 0 ] && [ -f "$STAMP" ] && [ -d "$HERE/rtl" ]; then
  if [ -z "$(find "$CORE_DIR/scala" -name '*.scala' -newer "$STAMP")" ]; then need=0; fi
fi
if [ "$need" = 0 ]; then
  echo "[gen_core] 核心 RTL 已是最新, 跳过。强制: bash gen_core.sh -f"
  exit 0
fi

echo "[gen_core] 部署并生成 SoC 兼容顶层: $CORE (YSYX_SOC=1, resetPC=0x$RST_PC)"
rm -rf "$ROOT/MyChisel/verilog" "$ROOT/MyChisel/src/main"
( cd "$ROOT" && make deploy-core core="$CORE" )
( cd "$ROOT/MyChisel" && YSYX_SOC=1 YSYX_RESET_PC="$RST_PC" sbt -batch run )

echo "[gen_core] 安装到 soc/rtl ..."
rm -rf "$HERE/rtl"; mkdir -p "$HERE/rtl"
cp "$ROOT"/MyChisel/*.sv "$HERE/rtl/" 2>/dev/null || true
cp "$ROOT"/MyChisel/verilog/*.sv "$HERE/rtl/" 2>/dev/null || true
# EICG 行为模型 (ClockGate 降级出的技术单元)
cp "$CORE_DIR/resources/EICG_wrapper.v" "$HERE/rtl/" 2>/dev/null || true
touch "$STAMP"
echo "[gen_core] 完成: rtl/ ($(ls "$HERE/rtl" | wc -l) 个文件)"

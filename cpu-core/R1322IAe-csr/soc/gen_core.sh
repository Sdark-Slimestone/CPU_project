#!/bin/bash
# ============================================================================
# 从核心源码 (../scala) 生成"接入 ysyxSoC 的兼容顶层" RTL, 隔离在 soc/ 下:
#   - soc/rtl/        : MROM 启动 (复位 PC=0x2000_0000)
#   - soc/rtl_flash/  : flash 启动变体 (仅 IFU 复位 PC=0x3000_0000)
# 兼容层见 ../scala/soc_compat.scala (本体子模块 + AXI 桥 + 时钟门控)。
# 本体源码基本不改; 仅 top.scala(开关)/ifu.scala(BootCfg)/lsu.scala(load 宽度) 有注释包裹的小改。
# 用法: bash gen_core.sh [-f]
# ============================================================================
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
CORE_DIR="$(cd "$HERE/.." && pwd)"
ROOT="$(cd "$CORE_DIR/../.." && pwd)"
CORE="$(basename "$CORE_DIR")"
MROM_PC="20000000"
FLASH_PC="30000000"
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

echo "[gen_core] 部署并生成 SoC 兼容顶层: $CORE (YSYX_SOC=1)"
rm -rf "$ROOT/MyChisel/verilog" "$ROOT/MyChisel/src/main"
rm -f  "$ROOT"/MyChisel/*.sv          # 清掉散落生成物, 避免多核之间串味
( cd "$ROOT" && make deploy-core core="$CORE" )
( cd "$ROOT/MyChisel" && YSYX_SOC=1 YSYX_RESET_PC="$MROM_PC" sbt -batch run )

echo "[gen_core] 安装到 soc/rtl ..."
rm -rf "$HERE/rtl"; mkdir -p "$HERE/rtl"
cp "$ROOT"/MyChisel/*.sv "$HERE/rtl/" 2>/dev/null || true
cp "$ROOT"/MyChisel/verilog/*.sv "$HERE/rtl/" 2>/dev/null || true
cp "$CORE_DIR/resources/EICG_wrapper.v" "$HERE/rtl/" 2>/dev/null || true
touch "$STAMP"

echo "[gen_core] 派生 flash 启动变体 soc/rtl_flash (复位 PC 0x$FLASH_PC) ..."
rm -rf "$HERE/rtl_flash"; cp -r "$HERE/rtl" "$HERE/rtl_flash"
if [ -f "$HERE/rtl_flash/IFU.sv" ]; then
  sed -i "s/32'h${MROM_PC}/32'h${FLASH_PC}/" "$HERE/rtl_flash/IFU.sv"
else
  echo "[gen_core][WARN] 未找到 rtl_flash/IFU.sv, flash 变体的复位 PC 未修改"
fi
echo "[gen_core] 完成: rtl/ ($(ls "$HERE/rtl" | wc -l) 个) + rtl_flash/"

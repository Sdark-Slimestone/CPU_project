#!/bin/bash
# ============================================================================
# 核心 ↔ SoC 隔离脚本
#
# 核心源码只放在 ../scala (即 cpu-core/R1322IAe-13/scala);
# 本脚本把核心"生成物"隔离在 soc/ 下的 rtl* 目录 (可随时重新生成):
#   - soc/rtl/        : 从 ../scala 生成的核心 RTL (MROM 启动, 复位 PC=0x2000_0000)
#   - soc/rtl_flash/  : 派生变体, 仅把 IFU 复位 PC 改成 0x3000_0000 (flash 启动)
#
# 用法:
#   bash gen_core.sh        # scala 有更新才重新生成 (用时间戳判断)
#   bash gen_core.sh -f     # 强制重新生成
#
# 以后"改核心" = 只改 ../scala/*.scala, 然后 build_soc*.sh 会自动调用本脚本。
# SoC 侧的 ysyxSoCFull.v / perip / csrc / nvboard 都不用动 (只要核心顶层接口不变)。
# ============================================================================
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"      # .../R1322IAe-13/soc
CORE_DIR="$(cd "$HERE/.." && pwd)"         # .../R1322IAe-13
ROOT="$(cd "$CORE_DIR/../.." && pwd)"      # 仓库根 (cpu_project)
CORE="$(basename "$CORE_DIR")"             # 核心目录名, 如 R1322IAe-13
MROM_PC="20000000"                          # 默认复位 PC (MROM)
FLASH_PC="30000000"                         # flash 启动复位 PC
STAMP="$HERE/rtl/.core_stamp"

force=0; [ "$1" = "-f" ] && force=1
need=1
if [ "$force" = 0 ] && [ -f "$STAMP" ] && [ -d "$HERE/rtl" ]; then
  if [ -z "$(find "$CORE_DIR/scala" -name '*.scala' -newer "$STAMP")" ]; then
    need=0
  fi
fi
if [ "$need" = 0 ]; then
  echo "[gen_core] 核心 RTL 已是最新 (scala 无更新), 跳过。强制重建: bash gen_core.sh -f"
  exit 0
fi

echo "[gen_core] 部署并生成核心 Verilog: $CORE"
rm -rf "$ROOT/MyChisel/verilog" "$ROOT/MyChisel/src/main"
rm -f  "$ROOT"/MyChisel/*.sv          # 清掉散落生成物, 避免多核之间串味
( cd "$ROOT" && make deploy-core core="$CORE" && make genv )

echo "[gen_core] 安装到 soc/rtl ..."
rm -rf "$HERE/rtl"; mkdir -p "$HERE/rtl"
cp "$ROOT"/MyChisel/verilog/*.sv "$HERE/rtl/"
touch "$STAMP"

echo "[gen_core] 派生 flash 启动变体 soc/rtl_flash (复位 PC 0x$FLASH_PC) ..."
rm -rf "$HERE/rtl_flash"; cp -r "$HERE/rtl" "$HERE/rtl_flash"
if [ -f "$HERE/rtl_flash/IFU.sv" ]; then
  sed -i "s/32'h${MROM_PC}/32'h${FLASH_PC}/" "$HERE/rtl_flash/IFU.sv"
else
  echo "[gen_core][WARN] 未找到 rtl_flash/IFU.sv, flash 变体的复位 PC 未修改"
fi
echo "[gen_core] 完成: rtl/ ($(ls "$HERE/rtl" | wc -l) 个文件) + rtl_flash/"

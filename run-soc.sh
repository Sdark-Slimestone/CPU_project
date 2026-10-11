#!/bin/bash
# ============================================================================
#  傻瓜启动器 —— 一条命令把 RT-Thread 启动到 NVBoard 图形板上
#
#  用法:
#    ./run-soc.sh                 # 默认核心 R1322IAe-13
#    ./run-soc.sh R1322IAe-csr    # 手写单周期核
#
#  说明: 脚本会自动 (1) 生成该核的 flash 启动 RTL, (2) 构建 NVBoard 仿真,
#        (3) 打开 NVBoard 窗口并从 flash 启动 RT-Thread。
#        之后在窗口右上角的串口终端里点一下聚焦, 敲 `am_snake` 回车即可玩贪吃蛇。
#
#  可覆盖: RTBIN=<镜像>  NVBOARD_HOME=<目录>  DISPLAY=:0
# ============================================================================
set -e
ROOT="$(cd "$(dirname "$0")" && pwd)"
CORE="${1:-R1322IAe-13}"
SOC="$ROOT/cpu-core/$CORE/soc"
NVBOARD_HOME="${NVBOARD_HOME:-$HOME/ysyx-workbench/nvboard}"
YSYXSOC="${YSYXSOC:-$HOME/ysyx-workbench/ysyxSoC}"
RTBIN="${RTBIN:-$HOME/Templates/rt-thread-am/bsp/abstract-machine/build/rtthread-riscv32e-ysyxsoc-bootpsram.bin}"

[ -d "$SOC" ] || { echo "[错误] 没有核心目录 cpu-core/$CORE"; exit 1; }
if [ ! -f "$RTBIN" ]; then
  echo "[错误] 找不到 RT-Thread 镜像: $RTBIN"
  echo "       可先构建: cd ~/Templates/rt-thread-am/bsp/abstract-machine && \\"
  echo "         AM_HOME=~/ysyx-workbench/abstract-machine RTT_ROOT=~/Templates/rt-thread-am \\"
  echo "         make ARCH=riscv32e-ysyxsoc-bootpsram image"
  exit 1
fi

echo ">>> 核心=$CORE   RT镜像=$(basename "$RTBIN")"
echo ">>> [1/3] 生成 flash 启动核心 RTL (rtl_flash) ..."
bash "$SOC/gen_core.sh"
RTL="$SOC/rtl_flash"
[ -d "$RTL" ] || { echo "[错误] $RTL 不存在"; exit 1; }

echo ">>> [2/3] 构建 NVBoard 仿真 (核心 $CORE) ..."
NVDIR="$ROOT/cpu-core/R1322IAe-13/soc/nvboard"
if ! NVBOARD_HOME="$NVBOARD_HOME" make -C "$NVDIR" \
      RTL_DIR="$(cd "$RTL" && pwd)" BUILD_DIR="build_flash_$CORE" \
      > /tmp/run-soc-$CORE.log 2>&1; then
  echo "[错误] NVBoard 构建失败, 见 /tmp/run-soc-$CORE.log"; tail -15 /tmp/run-soc-$CORE.log; exit 1
fi
BIN="$NVDIR/build_flash_$CORE/ysyxSoCFull"

echo ">>> [3/3] 启动 NVBoard + RT-Thread (窗口在 \$DISPLAY=${DISPLAY:-:0})"
echo
echo "    操作: 等右上角串口终端出现 'msh />' → 点它聚焦 → 敲 am_snake 回车"
echo "         点 VGA 区域让终端失焦后, 用 PS/2 键盘控制; 撞死后按 Q 回 msh"
echo
exec env DISPLAY="${DISPLAY:-:0}" YSYX_BOOT_FLASH=1 "$BIN" "$RTBIN"

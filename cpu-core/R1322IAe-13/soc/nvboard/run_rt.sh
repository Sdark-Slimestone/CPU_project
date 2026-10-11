#!/bin/bash
# 在 NVBoard 上启动 ysyxSoC, 从 flash 启动 RT-Thread (ysyxsoc-bootpsram 镜像)。
# 用法:
#   ./run_rt.sh                      # 用默认 RT-Thread 镜像
#   ./run_rt.sh /path/to/other.bin   # 用别的 bootpsram 镜像
# 提示:
#   - 窗口出现后等 RT-Thread 启动到 `msh />` (约几分钟, PSRAM 较慢)。
#   - 在 NVBoard 右上角的“串口终端”里点击聚焦后键入命令, 例如:
#       am_snake        贪吃蛇 (键盘: NVBoard 的 PS/2 键盘)
#       am_typing_game  打字游戏
#       am_fceux_am     红白机 (马里奥, 很慢)
#   - 游戏画面在 VGA 区域; LED/拨码/数码管为 GPIO。
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
RTBIN="${1:-$HOME/Templates/rt-thread-am/bsp/abstract-machine/build/rtthread-riscv32e-ysyxsoc-bootpsram.bin}"
export DISPLAY="${DISPLAY:-:0}"
export YSYX_BOOT_FLASH=1
exec "$HERE/build_flash/ysyxSoCFull" "$RTBIN"

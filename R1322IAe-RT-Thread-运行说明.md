# R1322IAe 运行 RT-Thread 说明（在当前 SoC 上跑 RT-Thread 与 AM 程序）

面向：用**当前 SoC**（`R1322IAe-13/-14`：`ysyx_26030107` + ysyxSoC，
含 MROM / SRAM / PSRAM / SDRAM / flash / UART16550 / CLINT / GPIO / PS/2 / VGA）
从 **flash 启动 RT-Thread**，并在 `msh` 里启动 AM 程序（贪吃蛇、打字、红白机等）。

> 这是对 `R1322IAe-14-说明.md` 第 7 节的展开与操作手册。

---

## 0. 前提与总览

| 组件 | 位置 / 说明 |
|---|---|
| 本核 + SoC Verilog | `~/cpu_project/cpu-core/R1322IAe-13/soc/`（`ysyxSoCFull.v` + `rtl/`） |
| flash 启动变体 RTL | `soc/rtl_flash/`（仅 `IFU.sv` 复位 PC 由 `0x2000_0000` 改为 `0x3000_0000`） |
| SoC 仿真（命令行） | `soc/build_soc_full_flash.sh` → `soc/obj_dir_full_flash/sim_soc_full` |
| NVBoard | `soc/nvboard/`（`constr/top.nxdc` + `build_flash/ysyxSoCFull` + `run_rt.sh`） |
| RT-Thread 源码 | `~/Templates/rt-thread-am/`（含 AM 集成脚本） |
| AM 运行时 | `~/ysyx-workbench/abstract-machine/`（平台 `riscv32e-ysyxsoc-bootpsram`） |
| RT-Thread 镜像 | `<rt-thread-am>/bsp/abstract-machine/build/rtthread-riscv32e-ysyxsoc-bootpsram.bin` |

**启动流程**：复位 PC = `0x3000_0000`（flash, XIP）→ FSBL 把 SSBL 搬到 SRAM →
SSBL 把程序的 code/rodata/data 从 flash 搬到 **PSRAM** → 跳到 PSRAM 执行 RT-Thread →
进入 `msh` → 键入 `am_xxx` 启动 AM 程序。

**存储/外设地址**（与讲义一致）：

| 设备 | 地址 | 说明 |
|---|---|---|
| SRAM | `0x0f00_0000` | 8KB |
| CLINT | `0x0200_0000` | mtime（每周期 +1） |
| UART16550 | `0x1000_0000` | 字节间隔寄存器；除数=1 → 16 周期/bit |
| SPI master | `0x1000_1000` | |
| GPIO | `0x1000_2000` | `0x0`=LED, `0x4`=拨码开关, `0x8`=7 段 |
| PS/2 | `0x1001_1000` | `0x0`=扫描码（读后清） |
| MROM | `0x2000_0000` | 4KB |
| VGA 帧缓冲 | `0x2100_0000` | 640×480×4B |
| flash (XIP) | `0x3000_0000` | |
| PSRAM | `0x8000_0000` | 4MB |
| SDRAM rank0/1 | `0xa000_0000` / `0xa200_0000` | 各 32MB |

---

## 1. 一次性准备

### 1.1 生成 flash 启动变体 RTL
本核默认复位 PC = `0x2000_0000`（MROM 启动）。RT-Thread 走 flash 启动，需要复位 PC = `0x3000_0000`：

```bash
cd ~/cpu_project/cpu-core/R1322IAe-13/soc
rm -rf rtl_flash && cp -r rtl rtl_flash
sed -i "s/32'h20000000/32'h30000000/" rtl_flash/IFU.sv   # 仅 IFU 复位 PC 不同
```

> 也可以用 `YSYX_RESET_PC=30000000 make deploy-core core=R1322IAe-13 && YSYX_RESET_PC=30000000 make genv`
> 重新生成，再拷回 `rtl/`，本说明用上面的增量补丁方式。

### 1.2 构建命令行仿真（flash 启动，顶层 `ysyxSoCFull`）

```bash
cd ~/cpu_project/cpu-core/R1322IAe-13/soc
bash build_soc_full_flash.sh        # → ./obj_dir_full_flash/sim_soc_full
```

### 1.3 构建 NVBoard（flash 启动版）

```bash
cd ~/cpu_project/cpu-core/R1322IAe-13/soc/nvboard
NVBOARD_HOME=~/ysyx-workbench/nvboard \
  make RTL_DIR=../rtl_flash BUILD_DIR=./build_flash   # → ./build_flash/ysyxSoCFull
```

---

## 2. 构建 RT-Thread 镜像

```bash
cd ~/Templates/rt-thread-am/bsp/abstract-machine

# 首次 / 想重建集成的 AM 程序时, 强制重跑集成脚本:
rm -f build/riscv32e-ysyxsoc-bootpsram/am-apps.mk build/am-apps.c

AM_HOME=~/ysyx-workbench/abstract-machine \
RTT_ROOT=~/Templates/rt-thread-am \
  make ARCH=riscv32e-ysyxsoc-bootpsram update image
# 产物: build/rtthread-riscv32e-ysyxsoc-bootpsram.bin
```

- `integrate-am-apps.py` 会把 `am-kernels` 里的程序编译并注册为 `msh` 命令：
  `am_hello / am_microbench / am_typing_game / am_snake / am_fceux_am`。
- BSP 串口输入（`src/uart.c` 的 `_uart_getc`）已按讲义改为：**先吐一段内置命令串，
  之后经 `ioe_read(AM_UART_RX)` 从串口读字符**，从而能在 `msh` 真正键入命令。
- 改了 AM 源码（如 `ioe.c`/`input.c`）后需按上面 **强制重跑 `update`**，否则用的是旧的 AM 目标文件。

---

## 3. 运行方式 A：命令行 flash 仿真（无窗口，可抓帧/注入）

```bash
cd ~/cpu_project/cpu-core/R1322IAe-13/soc
RTBIN=~/Templates/rt-thread-am/bsp/abstract-machine/build/rtthread-riscv32e-ysyxsoc-bootpsram.bin

YSYX_BOOT_FLASH=1 \
YSYX_INJECT_UART=$'am_snake\n' YSYX_UART_DELAY=60000000 \
YSYX_VGA_CAPTURE=1 YSYX_VGA_DUMP=/tmp/snake_vga.bin \
YSYX_MAX_CYCLES=500000000 \
./obj_dir_full_flash/sim_soc_full "$RTBIN"
```

环境变量：

| 变量 | 作用 |
|---|---|
| `YSYX_BOOT_FLASH=1` | 把程序载入 **flash**（而非 MROM） |
| `YSYX_INJECT_UART="..."` | 从 UART RX 注入字符（模拟键入）；配合 `YSYX_UART_DELAY`（起始周期） |
| `YSYX_VGA_CAPTURE=1` + `YSYX_VGA_DUMP=file` | 采集最近一整帧 VGA（640×480，每像素 4B，`r<<16|g<<8|b`） |
| `YSYX_MAX_CYCLES=N` | 仿真上限周期 |

- UART 除数=1 → 16 周期/bit，注入器按此发送（`YSYX_UART_PERIOD` 可调，默认 16）。
- 内置命令串会先自动执行（`help/date/.../utest_list`），随后 msh 读你注入的命令。

---

## 4. 运行方式 B：NVBoard（可交互，推荐）

```bash
cd ~/cpu_project/cpu-core/R1322IAe-13/soc/nvboard
./run_rt.sh                                   # 默认用 RT 镜像; 也接受别的 bootpsram 镜像作参数
# 等价: DISPLAY=:0 YSYX_BOOT_FLASH=1 ./build_flash/ysyxSoCFull <rtthread.bin>
```

### 操作要点（**焦点机制**，很关键）
NVBoard 的键盘有"焦点"概念：
- **点右上角的"串口终端"** → 键盘输入送到 **UART RX**（用来敲 `msh` 命令）。
- **点别处（如左下 VGA 区域 / 中间的 PS/2 键盘图）** → 键盘事件走 **PS/2**（给游戏用）。
- 若在终端聚焦时按方向键/Q → 会被终端吞掉，游戏收不到。

### 典型流程
1. 等窗口右上角串口终端出现 `msh />`（flash+PSRAM 启动较慢，约几分钟；无 cache 阶段更慢）。
2. **点串口终端**聚焦，键入命令 + 回车，例如：`am_snake`
3. **点 VGA 区域**让终端失焦，然后按**方向键**操控（snake 只认方向键）。
4. snake 撞死 → `GAME OVER / Press Q to Exit` → 按 **Q** 回 `msh`。

---

## 5. `msh` 命令一览

| 命令 | 程序 | 输入 | 说明 |
|---|---|---|---|
| `am_hello` | hello | 无 | 打印 `Hello, AbstractMachine!` |
| `am_microbench` | microbench | 无 | 跑微基准 |
| `am_typing_game` | typing-game | 键盘 | 打字游戏 |
| `am_snake` | snake | 方向键 | 贪吃蛇（撞死后 Q 退出） |
| `am_fceux_am` | fceux（红白机） | 键盘 | 默认 ROM=`mario`；**很慢**，见下 |
| `help/list/date/version/free/ps` | RT-Thread 自带 | | |

---

## 6. 已知坑与限制（都踩过）

1. **PS/2 兼容**：NVBoard 发送器每字节前会多一个**空闲下降沿**；接收器必须用**起始位(data=0)对齐**
   （`perip/ps2/ps2_top_apb.v` 已改为在 `bitcnt==0 && data==0` 时开帧），否则整帧错位、按键无反应。
2. **UART 焦点**：见 §4；不点对地方就"按键没反应"。
3. **fceux 的 `HAS_GUI`**：`fceux-am/src/config.h` 里 `HAS_GUI` 默认被注释 → 不走 VGA 输出；
   需在 `fceux-am/Makefile` 的 `CFLAGS` 加 `-DHAS_GUI`。
4. **fceux 的 `FUNC_IDX_MAX`**：`config.h` 里 PERF_LOW 分支原为 `FUNC_IDX_MAX16`，
   在 `FCEUPPU_Power` 注册 PPU 读写处理器时会溢出/卡死；改为 **`FUNC_IDX_MAX256`**。
5. **BSP 串口输入**：`_uart_getc` 必须改成读完内置串后读 `AM_UART_RX`，否则无法键入命令。
6. **性能（无 cache）**：本核 B2 阶段无 I/D cache，fceux 这类"每指令函数指针分派 + 乱访存"
   的解释器在 PSRAM 上 **≈106M 周期/帧**；仿真 **≈0.45M 周期/秒** → **≈4 分钟/帧**，
   只能"启动"、无法实时玩。轻量游戏（snake/typing）可用，但整体受仿真速度限制约 **1 FPS** 慢放。
   要流畅玩需 B3/B4 的缓存与性能优化。
7. **SDRAM 字扩展**（`-14`）：rank 由字节地址 bit25 选择；`0xa200_0000` 起为 rank1。

---

## 7. 复现命令汇总（TL;DR）

```bash
# (0) 生成本核 flash 变体 RTL
cd ~/cpu_project/cpu-core/R1322IAe-13/soc
rm -rf rtl_flash && cp -r rtl rtl_flash && sed -i "s/32'h20000000/32'h30000000/" rtl_flash/IFU.sv

# (1) 命令行仿真器
bash build_soc_full_flash.sh

# (2) NVBoard flash 版
cd nvboard && NVBOARD_HOME=~/ysyx-workbench/nvboard make RTL_DIR=../rtl_flash BUILD_DIR=./build_flash

# (3) RT-Thread 镜像
cd ~/Templates/rt-thread-am/bsp/abstract-machine
rm -f build/riscv32e-ysyxsoc-bootpsram/am-apps.mk build/am-apps.c
AM_HOME=~/ysyx-workbench/abstract-machine RTT_ROOT=~/Templates/rt-thread-am \
  make ARCH=riscv32e-ysyxsoc-bootpsram update image

# (4a) 无窗口跑 + 自动注入 am_snake
cd ~/cpu_project/cpu-core/R1322IAe-13/soc
YSYX_BOOT_FLASH=1 YSYX_INJECT_UART=$'am_snake\n' YSYX_UART_DELAY=60000000 \
YSYX_MAX_CYCLES=500000000 ./obj_dir_full_flash/sim_soc_full \
  ~/Templates/rt-thread-am/bsp/abstract-machine/build/rtthread-riscv32e-ysyxsoc-bootpsram.bin

# (4b) NVBoard 交互
cd nvboard && ./run_rt.sh
```

---

## 8. 定位

`R1322IAe-14` 完成了讲义 B2 的「在 RT-Thread 上运行 AM 程序」：本机能 boot RT-Thread 到 `msh`，
并启动/操控 `am_snake` 等 AM 程序；`am_fceux_am`（马里奥）能启动但受无 cache 限制太慢。

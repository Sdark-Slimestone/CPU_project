# R1322IAe-13 SoC 仿真目录说明（核心 / SoC 隔离）

本目录把**核心**与**SoC 相关部分**隔离开，目标是：**以后改核心只动核心**。

## 职责划分

| 属于 | 位置 | 说明 |
|---|---|---|
| **核心源码**（唯一要改的地方） | `../scala/`（即 `cpu-core/R1322IAe-13/scala/`） | Chisel 描述：IDU/EXU/LSU/GRF/MDU/预测器/imem/dmem/arbiter/bus/top… |
| **核心生成物**（自动生成，勿手改） | `rtl/`、`rtl_flash/` | 由 `gen_core.sh` 从 `../scala` 生成 |
| **SoC 拼接**（ysyxSoC 生成，勿手改） | `ysyxSoCFull.v` | 由 `~/ysyx-workbench/ysyxSoC` 生成；只引用核心顶层模块名 `ysyx_26030107` |
| **SoC 外设** | `~/ysyx-workbench/ysyxSoC/perip/**` | UART/SPI/PSRAM/SDRAM/GPIO/PS2/VGA… |
| **仿真 TB** | `csrc/`、`tests/`、`nvboard/` | Verilator 顶层 C++ / 测试程序 / NVBoard |
| **构建脚本** | `build_soc*.sh`、`gen_core.sh` | |
| **生成物（gitignore）** | `obj_dir*/`、`rtl/`、`rtl_flash/`、`nvboard/build*/` | |

## 改核心（只动核心）
```bash
# 1) 只编辑 cpu-core/R1322IAe-13/scala/*.scala
# 2) 构建 SoC 仿真 —— 会自动重新生成核心 RTL 并重编译
cd cpu-core/R1322IAe-13/soc
bash build_soc.sh                 # 顶层 ysyxSoCTop
bash build_soc_full.sh            # 顶层 ysyxSoCFull（带外设引脚，用于无 GUI 验证）
bash build_soc_full_flash.sh      # flash 启动变体
```
- `build_soc*.sh` 会先跑 `gen_core.sh`：若 `../scala` 有更新，则
  `make deploy-core && make genv` → 装到 `rtl/`，并从 `rtl/` 派生 `rtl_flash/`
  （仅把 IFU 复位 PC 改成 `0x3000_0000`）。
- 若核心没改，`gen_core.sh` 用时间戳判断、直接跳过。强制重建：`bash gen_core.sh -f`。
- **前提**：核心顶层模块名/端口保持 `ysyxSoC/spec/cpu-interface.md` 规范不变，
  于是 `ysyxSoCFull.v` 和 SoC 侧文件**都不需要动**。

## 改 SoC（外设 / 拼接）
- 改 `~/ysyx-workbench/ysyxSoC/perip/**` 的 RTL：直接生效（构建脚本会 `find perip -name '*.v'`）。
- 改 `~/ysyx-workbench/ysyxSoC/src/**`（Chisel，如地址映射）：
  ```bash
  cd ~/ysyx-workbench/ysyxSoC && make verilog
  sed 's/ysyx_00000000/ysyx_26030107/' build/ysyxSoCFull.v \
      > ~/cpu_project/cpu-core/R1322IAe-13/soc/ysyxSoCFull.v
  ```

## 仿真运行
- 命令行：`./obj_dir/sim_soc <prog.bin>`、`./obj_dir_full/sim_soc_full <prog.bin>`
  （flash 启动加 `YSYX_BOOT_FLASH=1`，可用 `YSYX_INJECT_PS2/YSYX_INJECT_UART/YSYX_VGA_CAPTURE` 等）。
- NVBoard：见 `nvboard/`（`RTL_DIR=../rtl_flash` 构建 flash 版）。
- RT-Thread：见仓库根 `R1322IAe-RT-Thread-运行说明.md`。

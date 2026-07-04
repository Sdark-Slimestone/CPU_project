# CPU 项目

基于 Chisel + Verilator 的 RISC-V CPU 设计与验证工程，支持从 Chisel 源码生成 Verilog，并通过多种方式进行仿真验证：Verilator NPC 仿真、NVBoard FPGA 仿真、ISA 模拟器对比、以及 NEMU diff 对比测试。

所有 CPU 核心均为**单周期**设计。

## 目录结构

```
cpu_project/
├── Makefile                    # 主控 Makefile
├── autonv.py                   # 自动生成 NVBoard 引脚绑定文件
├── sv2v                        # SystemVerilog → Verilog 转换工具
│
├── cpu-core/                   # 各个 CPU 核心源码（独立子文件夹）
│   ├── rv32e/                  # RV32E 单发射
│   ├── rv32e-csr/              # RV32E + CSR (Zicsr)，单发射
│   ├── R1322IAe/               # 同构双发射 RV32E
│   ├── R1322IAe-csr/           # 同构双发射 RV32E + CSR (Zicsr)
│   ├── R1322VA/                # R1322VA 架构模块 (PPU, GRF)
│   ├── minirv/                 # 早期 RV32I 核心
│   └── scpu/                   # Verilog 简单 CPU
│
├── MyChisel/                   # Chisel 工程（deploy-core 目标目录）
│   ├── build.sbt               # sbt 构建配置
│   ├── Makefile                # genv/test/clean
│   ├── src/
│   │   └── main/
│   │       ├── scala/          # （被 deploy-core 覆盖）
│   │       ├── cppfile/        # （被 deploy-core 覆盖）
│   │       └── resources/      # DPI_Memory.v 等资源文件
│   ├── verilog/                # make genv 生成的 Verilog 文件
│   └── target/                 # sbt 编译产物（gitignored）
│
├── temper_npc/                 # NPC 模拟器临时构建目录（自动清理）
├── emu-so/                     # ISA 模拟器动态库（供 diff2 模式使用）
│   ├── rv32e/
│   │   └── rv32e.so
│   └── minirv/
│       └── minirv.so
├── nemu-so/                    # NEMU 动态库（供 diff 模式使用）
├── isa-emu/                    # ISA 模拟器源码
│   ├── rv32e-csr/              # RV32E + CSR 模拟器（生成 emu-so/rv32e/）
│   └── minirv/                 # Minirv 模拟器
├── nvboard/                    # NVBoard 仿真环境
├── test-benchmarks/            # 测试程序二进制文件
└── yosys-sta/                  # Yosys 综合 + iSTA 时序分析
```

## 各 CPU 核心一览

所有核心均为单周期设计。各核心按功能阶段划分模块（IFU/取指、IDU/译码、EXU/执行、LSU/访存、WBU/写回、CSR），各阶段组合为单周期通路。

| 核心名称 | ISA | 发射宽度 | 备注 |
|----------|-----|---------|------|
| `rv32e` | RV32E | 1 | 基础单发射单周期核心，无 CSR |
| `rv32e-csr` | RV32E + Zicsr | 1 | 增加 CSR 模块，支持 ecall/ebreak/mret |
| `R1322IAe` | RV32E | 2 (同构双发射) | 双发射单周期核心 |
| `R1322IAe-csr` | RV32E + Zicsr | 2 (同构双发射) | 双发射 + CSR |
| `R1322VA` | RV32E 变体 | — | 独立的 PPU / GRF 模块 |
| `minirv` | RV32I | 1 | 早期版本 |
| `scpu` | — | — | Verilog 简单核心 |

## 快速开始

### 0. 初始化环境

```bash
make init
```

自动检测并安装缺失的工具链（JDK、sbt、Verilator、Capstone 等）。

### 1. 部署核心 & 生成 Verilog

```bash
# 部署指定核心到 MyChisel/src/main/，并生成 Verilog
make deploy-core core=rv32e-csr
make genv
```

`make genv` 等价于在 `MyChisel/` 下执行 `sbt run`，生成的 `.sv` 文件输出到 `MyChisel/verilog/`。

### 2. NPC 仿真（Verilator）

四种仿真模式：

| sim 参数 | 功能 | 使用的 cpp 文件 | 依赖 |
|----------|------|----------------|------|
| `no` | 基础仿真 | `sim_main_io.cpp` | 无 |
| `sdb` | 带 Capstone 反汇编 | `sim_main_io_sdb.cpp` | capstone |
| `diff` | Capstone + NEMU diff 对比 | `sim_main_io_sdb_diff.cpp` | capstone, NEMU .so |
| `diff2` | Capstone + ISA 模拟器 diff 对比 | `sim_main_io_sdb_diff2.cpp` | capstone, emu-so/.so |

```bash
# 基础仿真
make npc core=rv32e-csr sim=no

# 带 sdb 仿真
make npc core=rv32e-csr sim=sdb

# 带 NEMU diff 仿真（同时对比 NEMU 执行结果）
make npc core=rv32e-csr sim=diff

# 带 ISA 模拟器 diff 仿真（对比内置 ISA 模拟器）
make npc core=rv32e-csr sim=diff2
```

构建完成后，可执行文件位于 `cpu-core/<core>/npc-core/` 下：

```bash
cpu-core/rv32e-csr/npc-core/npc         # 基础
cpu-core/rv32e-csr/npc-core/npc-sdb     # 带 sdb
cpu-core/rv32e-csr/npc-core/npc-diff    # 带 NEMU diff
cpu-core/rv32e-csr/npc-core/npc-diff2   # 带 ISA emu diff
```

### 3. 运行测试

```bash
# yield-os 测试
make npctest core=rv32e-csr test=yield-os

# microbench 测试（需要额外 test 参数）
make npctest core=rv32e-csr test=microbench

# RT-Thread 测试（需要 CSR）
make npctest core=rv32e-csr test=rtthread
```

`diff2` 模式运行时会在工作目录下通过 `dlopen` 加载 `emu-so/rv32e/rv32e.so` 作为参考模拟器。

### 4. 构建 ISA 模拟器（.so）

```bash
# 构建默认 rv32e-csr 模拟器
make emu

# 构建 minirv 模拟器
make emu emu=minirv

# 构建后 .so 自动复制到 emu-so/ 目录
```

### 5. NVBoard 仿真

```bash
# 一步完成：genv → 复制到 vsrc → sv2v 转换 → 启动 NVBoard 仿真
make all

# 或分步执行
make genv
make prepare-vsrc      # 复制 Verilog + sv2v + 生成 top.nxdc
make run-nvboard       # 仅运行
```

### 6. Yosys 综合与时序分析

```bash
make yosys-init            # 首次使用：下载 iEDA 和 PDK
make yosys-syn             # 逻辑综合，生成网表
make yosys-sta             # 静态时序 + 功耗分析
make yosys-sta CLK_FREQ_MHZ=600  # 自定义频率
```

结果输出到 `yosys-sta/result/top-500MHz/`。

## 编写新的 Chisel 电路

1. 在 `cpu-core/<your_core>/scala/` 下编写 `.scala` 文件，顶层模块命名为 `top`
2. 将 C++ 仿真文件放入 `cpu-core/<your_core>/cppfile/`
3. DPI 相关资源放入 `cpu-core/<your_core>/resources/`
4. 使用 `make deploy-core core=<your_core>` 部署
5. 使用 `make genv` 生成 Verilog

## 环境依赖

| 工具 | 版本要求 | 用途 |
|------|---------|------|
| JDK | 11+ | sbt 运行（Chisel 编译） |
| sbt | — | Chisel 构建 |
| Verilator | 5.0+ | NPC Verilog 仿真 |
| Yosys | 0.48+ | 逻辑综合 |
| Capstone | — | sdb / diff / diff2 模式反汇编 |
| NEMU | — | diff 模式对比参考（需 .so 文件） |
| Python 3 | — | autonv.py（PyYAML） |
| NVBoard | — | FPGA 仿真（需设 NVBOARD_HOME） |

## 常用命令速查

```bash
# Chisel
make genv                 # Chisel → Verilog
make rebuild              # clean + genv
make clean-chisel         # 清理 Chisel 产物

# 核心部署
make deploy-core core=rv32e-csr

# NPC 仿真
make npc core=rv32e-csr sim=sdb
make npc core=rv32e-csr sim=diff2
make npctest core=rv32e-csr test=rtthread
make clean-npc

# ISA 模拟器
make emu                  # 构建 rv32e-csr .so

# NVBoard
make all
make run-nvboard

# Yosys-STA
make yosys-init
make yosys-sta
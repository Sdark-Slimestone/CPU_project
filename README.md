# CPU 项目使用说明

## 目录结构
cpu_project/
├── Makefile # 主控 Makefile
├── autonv.py # 自动生成 NVBoard 引脚绑定文件
├── cpu-core/ # 存放不同 CPU 核心源码（子文件夹，如 minirv/）
├── MyChisel/ # Chisel 工程目录
│ ├── src/main/scala/ # Chisel 源文件（手工编写）
│ ├── src/main/cppfile/ # C++ 仿真文件（sim_main_*.cpp）
│ ├── src/main/resources/ # DPI 相关文件（DPI_Memory.v）
│ └── verilog/ # make genv 生成的 Verilog 文件
├── nvboard/myexample/ # NVBoard 仿真工程
│ ├── vsrc/ # 存放 Verilog 文件（自动从 verilog/ 复制）
│ └── constr/ # 存放引脚绑定文件 top.nxdc（autonv.py 生成）
├── isa-emu/ # 指令集模拟器（如 scpu/，minirv/ 等）
├── yosys-sta/ # 综合与时序分析工具
├── temper_npc/ # NPC 模拟器构建目录（自动生成）
└── nemu-so/ # NEMU 动态库（用于 diff 测试）

text

## 顶层模块端口命名要求

`autonv.py` 会根据端口名自动绑定 NVBoard 引脚。规则如下：

| 外设       | 端口名需包含 | 方向      | 说明                                                                 |
|------------|--------------|-----------|----------------------------------------------------------------------|
| LED        | `led`        | output    | 宽度 1~16，从 LD0 开始依次分配                                       |
| 开关       | `sw`         | input     | 宽度 1~16，从 SW0 开始依次分配                                       |
| 数码管     | `seg`        | output    | 每个 `seg0` ~ `seg7` 独立，每个端口宽度 ≤ 8，使用固定引脚            |

- 不区分大小写（`LED`、`Led`、`led` 均可识别）。
- 时钟端口请命名为 `clock` 或 `clk`。
- 复位端口请命名为 `reset`、`rst` 或 `resetn`（低有效请自行处理逻辑）。

## 常用命令

所有命令在项目根目录执行。

1. Chisel 生成 Verilog

```bash
make genv          # 编译 Chisel 并生成 verilog/ 文件夹
make test          # 运行 Chisel 测试（若工程中定义了测试）
make rebuild       # 清理后重新生成
make clean-chisel  # 仅清理 Chisel 生成物

2. NVBoard 仿真
bash
make all                   # 生成 Verilog → 复制到 vsrc → 自动绑定 → 启动仿真
make run-nvboard           # 仅启动仿真（前提：verilog/ 已生成，vsrc/ 已准备好）
make build-nvboard         # 仅构建 NVBoard 工程，不运行
make clean-nvboard         # 清理 NVBoard 的 build/ 目录

3. 指令集模拟器（ISA Emu）
使用变量 ISA_EMU_DIR 指定模拟器子目录（默认 scpu）。
修改运行程序：编辑 Makefile 中的 PATH1、PATH2 变量。

bash
make emu          # 编译并运行模拟器（使用 PATH1/PATH2 指定的内存镜像）
make run-emu      # 同 emu
make test-emu     # 仅运行单元测试
make clean-emu    # 清理编译产物
示例（使用 minirv 模拟器）：

bash
make emu ISA_EMU_DIR=minirv PATH1=path/to/rom.bin PATH2=path/to/ram.bin

4. 综合与时序分析（Yosys‑STA）
bash
make yosys-init     # 首次使用：下载脚本，克隆 PDK
make yosys-syn      # 逻辑综合，生成网表到 yosys-sta/result/
make yosys-sta      # 静态时序分析，生成时序和功耗报告
make yosys-clean    # 清理 yosys-sta 生成的结果
修改时钟频率：make yosys-sta CLK_FREQ_MHZ=600

修改顶层时钟端口名（默认 clock）：编辑 yosys-sta/Makefile 中的 CLK_PORT_NAME。

结果位置：

网表：yosys-sta/result/top-500MHz/top.netlist.v

时序报告：yosys-sta/result/top-500MHz/top.rpt

功耗报告：yosys-sta/result/top-500MHz/sta.log

5. NPC 模拟器（Verilator 仿真）
生成三种不同功能的模拟器，输出到 temper_npc/npc（可执行文件）。

bash
make npc          # 基础版本（使用 sim_main_io.cpp，无 Capstone/NEMU）
make npc-sdb      # 带 Capstone 反汇编（sim_main_io_sdb.cpp）
make npc-diff     # 带 Capstone + NEMU 动态库（sim_main_io_sdb_diff.cpp）
make clean-npc    # 清理 temper_npc/ 目录
CAP_DIR、NEMU_INC、NEMU_LIB 可在 Makefile 头部或命令行覆盖。

npc-diff 链接 NEMU 动态库，用于对比测试。

6. 部署 CPU 核心（切换 Chisel 源码）
将 cpu-core/ 下的某个核心文件夹（如 minirv）整体复制到 MyChisel/src/main，并替换原有内容。

bash
make deploy-core core=minirv
执行后请重新运行 make genv 生成新的 Verilog。

常见问题
NVBoard 窗口闪退：检查 Verilog 中 clock/reset 是否正确连接，是否存在组合逻辑环。

未识别到 led/sw/seg 信号：顶层端口命名不符合规则，请按表修改端口名。

模拟器报“打开 ROM 文件失败”：确保 PATH1/PATH2 指向存在的二进制文件。

Yosys‑STA 报 get_ports clk not found：确认 CLK_PORT_NAME 与实际时钟端口名一致（如 clock 或 io_clk）。

make npc-diff 找不到 C++ 文件：确认 MyChisel/src/main/cppfile/ 下存在 sim_main_io_sdb_diff.cpp，文件名与 Makefile 中的一致。

deploy-core 提示目录不存在：请确认 cpu-core/ 下存在指定的子文件夹，且名字拼写正确。

编写新的 Chisel 电路
将 .scala 文件放入 MyChisel/src/main/scala/。

确保顶层模块名为 top（生成 Verilog 时会自动作为顶层）。

在项目根目录执行 make genv 生成 Verilog。

根据需要运行 make all（NVBoard 仿真）或 make npc（Verilator 仿真）。

环境依赖
JDK 11 或更高版本（sbt 运行需要）

sbt

Verilator 5.0 或更高（用于 NPC）

Capstone 库（用于 npc-sdb/npc-diff）

NEMU 动态库（仅 npc-diff 需要）

Python 3 + PyYAML（用于 autonv.py）

NVBoard 已正确配置（环境变量 NVBOARD_HOME）
# CPU 项目使用说明

## 目录结构
cpu_project/
├── Makefile # 主控 Makefile
├── autonv.py # 自动生成引脚绑定
├── MyChisel/ # Chisel 源码，生成 verilog/
├── nvboard/myexample/ # NVBoard 仿真工程
│ ├── vsrc/ # 放 Verilog 文件（自动复制）
│ └── constr/ # 放引脚绑定文件 top.nxdc
├── isa-emu/ # 指令集模拟器（如 minirv）
└── yosys-sta/ # 综合与时序分析工具

## 要测试的电路文件夹位置
chisel：cpu_project/MyChisel/src/main/scala/myfile
把写好的scala放里面，然后退到根目录，用命令就行

## 顶层模块端口命名要求

`autonv.py` 会根据端口名自动绑定 NVBoard 引脚：

| 外设 | 端口名需包含 | 方向 | 说明 |
|------|--------------|------|------|
| LED | `led` | output | 宽度1~16，从 LD0 开始分配 |
| 开关 | `sw` | input | 宽度1~16，从 SW0 开始分配 |
| 数码管 | `seg` | output | 每个 seg0~seg7 独立，宽度≤8，使用固定引脚 |

不区分大小写。时钟/复位请命名为 `clock`/`clk` 和 `reset`/`rst`/`resetn`。

## 常用命令

所有命令在项目根目录执行。

### 命令


```bash
make all        # 生成 Verilog → 复制到 vsrc → 自动绑定 → 运行仿真
make run        # 仅运行仿真（前提：已经生成并复制过 Verilog）
make clean      # 清理 Chisel 生成物和 NVBoard build 目录
指令集模拟器（isa-emu/minirv）
bash
make emu        # 编译并运行模拟器（默认用 isa-emu/bin/sum.bin）
make run-emu    # 和emu是一样的
make test-emu   # 仅运行单元测试
make clean-emu  # 清理模拟器编译产物
切换模拟器目录：make emu ISA_EMU_DIR=子目录名
修改运行程序：编辑 Makefile 中的 PATH1、PATH2 变量。

综合与时序分析（yosys-sta）
bash
make yosys-init     # 首次使用：下载脚本，克隆 PDK
make yosys-syn      # 逻辑综合，生成网表到 yosys-sta/result/
make yosys-sta      # 静态时序分析，生成时序报告和功耗报告
make yosys-clean    # 清理 yosys-sta 生成的结果
结果位置：

网表：yosys-sta/result/top-500MHz/top.netlist.v

时序报告：yosys-sta/result/top-500MHz/top.rpt

功耗报告：yosys-sta/result/top-500MHz/sta.log

修改时钟频率：make yosys-sta CLK_FREQ_MHZ=600
修改时钟端口名（默认 clock）：编辑 yosys-sta/Makefile 中的 CLK_PORT_NAME。

常见问题
NVBoard 窗口闪退：检查 Verilog 中 clock/reset 连接是否正确，是否有组合逻辑环。

未识别到 led/sw/seg 信号：顶层端口名不符合命名规则，按上述要求改名。

模拟器报“打开 ROM 文件失败”：确保 PATH1/PATH2 指向存在的二进制文件。

yosys-sta 报 get_ports clk not found：确认 CLK_PORT_NAME 与实际时钟端口名一致（如 clock 或 io_clk）。
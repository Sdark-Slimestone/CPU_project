# CPU 项目一键式构建与仿真

本项目帮助你自动完成：

1. **用 Chisel 生成硬件电路**（Verilog 文件）
2. **把电路放到 NVBoard 上仿真**（使用 Verilator，可看到 LED、开关、数码管）
3. **编译并运行指令集模拟器**（例如 `minirv`），用于运行测试程序
4. **对设计进行逻辑综合与时序分析**（使用 yosys‑sta 工具链）

你只需在项目根目录执行几条 `make` 命令，其余工作由脚本自动完成。

## 目录结构（简化）
cpu_project/
├── Makefile # 根目录的“总开关”
├── autonv.py # 自动生成 NVBoard 引脚绑定的脚本
├── MyChisel/ # Chisel 源代码，生成 Verilog
│ └── verilog/ # 生成的 .sv 文件
├── nvboard/myexample/ # NVBoard 仿真工程
│ ├── vsrc/ # 存放 Verilog 文件（自动复制）
│ ├── constr/ # 存放 top.nxdc 引脚绑定文件
│ └── Makefile
├── isa-emu/ # 指令集模拟器
│ ├── minirv/ # 例如 minirv 模拟器，请阅读其中的 README
│ └── bin/ # 二进制文件
└── yosys-sta/ # 综合与时序分析工具（独立的子项目）
├── bin/ # iEDA 二进制文件
├── scripts/ # yosys 和 STA 脚本
├── example/ # 待综合的 Verilog 文件（自动复制）
└── result/ # 综合网表、时序报告、功耗报告

text

## 命名规则（让自动脚本认识你的信号）

为了让 `autonv.py` 正确将 Verilog 端口映射到 NVBoard 物理引脚，你的顶层模块 `top` 的端口名称需遵循以下规则：

- **LED**（输出）：名称包含 `led`（如 `led1`, `io_led1` 等）。宽度任意（1~16位），脚本会从 LD0 开始连续分配引脚。
- **开关 SW**（输入）：名称包含 `sw`（如 `sw1`, `io_sw1` 等）。宽度任意（1~16位），从 SW0 开始分配。
- **数码管 seg**（输出）：名称包含 `seg`（如 `seg0`, `io_seg0` 等）。每个数码管独立，宽度最多 8 位，使用固定的 8 个引脚（SEGxA … DECxP）。

> 不区分大小写，`LED1` 和 `led1` 均被识别。

**时钟与复位**：如果你的顶层有 `clock` / `clk` 和 `reset` / `rst` / `resetn`，脚本会自动修改 `nvboard/myexample/Makefile` 中的 `CPP_FILE` 设置，选择正确的 C++ 驱动文件。

## 常用命令

以下命令均在项目根目录执行。

### 1. 一条龙：生成硬件 + 仿真

```bash
make all
依次执行：

进入 MyChisel 生成 Verilog（make genv）

清空 nvboard/myexample/vsrc，复制新的 Verilog 文件

运行 autonv.py 生成 top.nxdc 并修改 nvboard/myexample/Makefile

启动 NVBoard 仿真（弹出图形窗口，显示电路运行）

2. 只运行硬件仿真（不重新生成）
bash
make run
若修改了 Verilog 但未重新生成，可能运行旧版本，建议先执行 make all。

3. 清理所有生成物
bash
make clean
同时清理 Chisel 生成的 verilog/ 目录、NVBoard 的 build/ 目录以及 yosys‑sta 的结果目录（若已集成）。

4. 指令集模拟器相关（以 minirv 为例）
命令	说明
make emu	进入 isa-emu/minirv，执行 make clean，编译可执行文件并运行单元测试，最后运行模拟器（默认使用 isa-emu/bin/sum.bin）。运行结束后自动删除模拟器生成的临时 bin/ 目录。
make run-emu	仅运行模拟器（不重新编译和测试），使用 PATH1 和 PATH2 指定的程序。
make test-emu	仅运行单元测试，不运行模拟器。
make clean-emu	清理模拟器的编译产物（build/、可执行文件等）以及 bin/ 目录。
切换模拟器：通过 ISA_EMU_DIR 变量指定子目录，例如：

bash
make emu ISA_EMU_DIR=riscv
要求该目录下有 Makefile 并能生成同名可执行文件。

修改模拟器运行的程序：编辑根目录 Makefile 中的 PATH1 和 PATH2 变量（绝对路径或相对路径）。

5. 硬件综合与时序分析（yosys‑sta）
项目集成了 yosys‑sta 工具链，可对设计进行逻辑综合和静态时序分析（STA），生成网表、时序报告和功耗估计。

初始化（首次使用）
bash
make yosys-init
该命令会下载必要的脚本并克隆 PDK（icsprout55）到 yosys-sta/pdk/。

常用命令
命令	说明
make yosys-syn	将 nvboard/myexample/vsrc 下的 Verilog 复制到 yosys-sta/example，运行 Yosys 综合，生成网表到 yosys-sta/result/top-500MHz/top.netlist.v。
make yosys-sta	在综合之后运行静态时序分析，生成时序报告 top.rpt 和功耗报告（包含在 sta.log 中）。
make yosys-clean	清理 yosys‑sta 生成的结果（result/）和示例文件（example/*.v）。
查看结果
综合网表：yosys-sta/result/top-500MHz/top.netlist.v

时序报告：yosys-sta/result/top-500MHz/top.rpt（包含路径延迟、Slack、最大频率等）

功耗报告：yosys-sta/result/top-500MHz/sta.log（包含功耗统计）

修改约束
时钟频率：通过环境变量 CLK_FREQ_MHZ 指定（默认 500MHz）。例如：

bash
make yosys-sta CLK_FREQ_MHZ=600
时钟端口名：默认使用 clock（与 Chisel 生成的顶层端口一致）。若你的设计使用其他名称（如 io_clk），请修改 yosys-sta/Makefile 中的 CLK_PORT_NAME 变量。

注意事项
yosys‑sta 目录本身是一个独立的 Git 仓库。若希望将其内容纳入版本控制，请参考根目录的 .gitignore 和子模块说明。

综合和 STA 会产生大量中间文件，建议定期执行 make yosys-clean 清理。

常见问题
1. 执行 make all 时 NVBoard 窗口闪一下就消失了？
可能是 Verilog 代码中存在组合逻辑环路，或者时钟/复位未正确连接。请检查顶层模块的 clock 和 reset 是否与 NVBoard 的驱动匹配。也可以查看终端输出的错误信息。

2. 脚本提示“未识别到任何 seg/led/sw 信号”
说明顶层模块的端口名称不符合命名规则。按照前述规则修改端口名，或手动修改 autonv.py 中的匹配模式。

3. 模拟器运行时提示“打开 ROM 文件失败”
请确保 PATH1 和 PATH2 指向正确的二进制文件（默认 isa-emu/bin/sum.bin）。若文件不存在，可以自行准备一个，或修改 Makefile 中的路径。

4. 如何更改默认的模拟器程序路径？
编辑根目录 Makefile，找到 PATH1 ?= ... 和 PATH2 ?= ... 行，修改为你的绝对路径或相对路径（相对于项目根目录）。

5. 为什么每次 make emu 都会先执行 make clean？
这是因为 emu 目标中显式调用了 $(MAKE) -C $(ISA_EMU_PATH) clean，以保证每次都是全新编译，避免旧文件干扰。如果你认为没有必要，可以删除该行。

6. yosys‑sta 运行时提示“get_ports clk was not found”？
请确认 yosys-sta/Makefile 中的 CLK_PORT_NAME 与你设计中的实际时钟端口名一致（默认为 clock）。或者通过 make yosys-sta CLK_PORT_NAME=io_clk 临时指定。

原理简述
硬件仿真：Chisel 生成 Verilog → 复制到 NVBoard 的 vsrc → autonv.py 解析端口并生成引脚约束 top.nxdc → Verilator 编译为 C++ 仿真程序 → 运行，弹出 SDL 窗口。

模拟器：直接调用子目录中的 Makefile，根目录的 make emu 仅负责执行几个步骤并清理临时目录。

综合与 STA：Yosys 读入 Verilog 进行逻辑综合，输出网表；iEDA 读入网表和 SDC 约束，进行时序分析并生成报告。
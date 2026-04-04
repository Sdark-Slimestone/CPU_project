# CPU 项目一键式构建与仿真

本项目帮你自动完成三件事：
1. **用 Chisel 生成硬件电路**（Verilog 文件）
2. **把电路放到 NVBoard 上仿真**（用 verilator 跑起来，可以看到 LED、开关、数码管）
3. **编译并运行一个指令集模拟器**（比如 minirv），可以用来跑测试程序

你只需要在项目根目录敲几条 `make` 命令，剩下的脚本自动搞定。

## 目录结构（简化版）
cpu_project/
├── Makefile # 根目录的“总开关”
├── autonv.py # 自动生成 NVBoard 引脚绑定的脚本
├── MyChisel/ # Chisel 源代码，生成 Verilog
│ └── verilog/ # 生成的 .sv 文件
├── nvboard/myexample/ # NVBoard 仿真工程
│ ├── vsrc/ # 放 Verilog 文件（自动复制过来）
│ ├── constr/ # 放 top.nxdc 引脚绑定文件
│ └── Makefile
└── isa-emu/ # 指令集模拟器
└── minirv/ # 例如一个叫 minirv 的模拟器
├── Makefile
└── ...


## 你需要知道的命名规则（让自动脚本认识你的信号）

为了让 `autonv.py` 能正确把 Verilog 端口连接到 NVBoard 的物理引脚，你的顶层模块 `top` 里的端口名字要遵守以下规则：

- **LED**（输出）：叫 `led1`, `led2`, … 或者 `io_led1`, `io_led2`, …  
  宽度随意（1~16位），脚本会从 LD0 开始连续分配引脚。
- **开关 SW**（输入）：叫 `sw1`, `sw2`, … 或者 `io_sw1`, …  
  宽度随意（1~16位），从 SW0 开始分配。
- **数码管 seg**（输出）：叫 `seg0`, `seg1`, …, `seg7` 或者 `io_seg0` …  
  每个数码管独立，宽度最多 8 位，使用固定的 8 个引脚（SEGxA … DECxP）。

不区分大小写，`LED1` 和 `led1` 都一样。

**时钟和复位**：如果你的顶层有 `clock` / `clk` 和 `reset` / `rst` / `resetn`，脚本会自动修改 `nvboard/myexample/Makefile` 里的 `CPP_FILE` 设置，选对仿真用的 C++ 驱动文件。

## 常用命令（在项目根目录执行）

### 1. 一条龙：生成硬件 + 仿真 + 跑模拟器
```bash
make all
它会依次：

进入 MyChisel 生成 Verilog（make genv）

清空 nvboard/myexample/vsrc，把新 Verilog 复制进去

运行 autonv.py 生成 top.nxdc 和修改 Makefile

启动 NVBoard 仿真（弹出图形窗口，可以看到你的电路运行）

2. 只跑硬件仿真（前提是已经生成过 Verilog）
bash
make run
如果修改了 Verilog 但没重新生成，可能会运行旧版本。建议先 make all。

3. 清理所有生成物
bash
make clean
会同时清理 Chisel 生成的 verilog/ 目录和 NVBoard 的 build/ 目录。

4. 模拟器相关（以 minirv 为例）
bash
make emu
进入 isa-emu/minirv，先 make clean 清理，然后编译可执行文件并运行单元测试，最后自动运行模拟器（用默认的 sum.bin 程序）。

运行结束后会自动删除模拟器运行时产生的 bin 文件夹（那是模拟器自己生成的临时目录）。

bash
make run-emu
只运行模拟器（不会重新编译和测试），使用默认的 PATH1 和 PATH2（在 Makefile 里设为 isa-emu/bin/sum.bin）。

bash
make test-emu
只运行单元测试，不运行模拟器。

bash
make clean-emu
清理模拟器的编译产物（build/, minirv 可执行文件等），同时删除那个讨厌的 bin 文件夹。

5. 换一个模拟器（比如你还有一个叫 riscv 的子目录）
bash
make emu ISA_EMU_DIR=riscv
只要那个目录下有 Makefile 并且能生成同名的可执行文件，就能用。

常见问题
1. 执行 make all 时 NVBoard 窗口闪一下就不见了？
可能是你的 Verilog 代码里有组合逻辑环路，或者没有正确连接时钟/复位。检查 top 模块的 clock 和 reset 是否与 NVBoard 的驱动匹配。也可以查看终端输出的错误信息。

2. 脚本说“未识别到任何 seg/led/sw 信号”
说明你的 top 模块端口名字不符合命名规则。按照上面的规则改名，或者手动修改 autonv.py 里的匹配模式。

3. 模拟器运行时报“打开 ROM 文件失败”
确保 PATH1 和 PATH2 指向正确的二进制文件（默认是 isa-emu/bin/sum.bin）。

如果文件不存在，可以自己准备一个，或者修改 Makefile 里的 PATH1 和 PATH2 路径。

4. 我想改默认的模拟器程序路径
编辑根目录 Makefile，找到 PATH1 ?= ... 和 PATH2 ?= ... 那两行，改成你自己的绝对路径或相对路径（相对于项目根目录）。

5. 为什么每次 make emu 都会先 make clean？
因为你写在 emu 目标里第一行就是 $(MAKE) -C $(ISA_EMU_PATH) clean。这样保证每次都是全新编译，避免旧文件干扰。如果你觉得没必要，可以把那一行删掉。

原理简述（给好奇的人）
Chisel 生成 Verilog → 复制到 NVBoard 的 vsrc → autonv.py 解析 top.sv 里的端口，生成 top.nxdc（告诉 NVBoard 哪个信号接哪个引脚） → verilator 编译成 C++ 仿真程序 → 运行，弹出 SDL 窗口。

模拟器：直接用你子目录里的 Makefile，根目录的 make emu 只是帮忙执行几个步骤，顺便帮你删掉讨厌的 bin 文件夹。

最后一句
这个 Makefile 和脚本的目标是让你忘记繁琐的复制、绑定、编译步骤，专心写 Chisel 代码和模拟器。如果遇到问题，先看看终端报错，八成是文件路径或命名问题。祝你玩得开心！


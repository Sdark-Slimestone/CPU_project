# R1322IAe-csr 接入 ysyxSoC 说明（兼容层）

本核是**手写的单周期、RV32E 核心**：`imem`/`dmem` 直接例化 `DPI_Memory`，
指令/数据**当拍组合读**（无停等）。要接到 ysyxSoC 的多拍 AXI 内存，需要一个兼容层。

## 设计（尽量不动本体）
- **核心本体逻辑不改**；仅 3 处**注释包裹**的小改：
  - `scala/ifu.scala`：`pcReg` 复位值改用 `BootCfg.resetPC`（默认仍 `0x8000_0000`，仿真不变）。
  - `scala/lsu.scala`：新增输出 `lsu_load_size`（load 宽度，供 AXI `arsize`）。
  - `scala/top.scala`：`object top` 加 `YSYX_SOC` 开关（默认仍生成仿真顶层 `top`）。
- **兼容层**（独立文件 `scala/soc_compat.scala`）：
  - `csr_axi_mem`：把核心的"简单内存接口"翻译成 AXI4 master；每步固定 5 笔
    （取指读 a1/a2 → 锁存 dmem 请求 → 数据读×2 →（有写才）写）。
    - 读：用**原始字节地址 + load 宽度**（`arsize`），以正确访问字节间隔的设备寄存器。
    - 写：核心给"字对齐地址 + byte lane 编在 mask 里" → AXI 窄写用 `awsize`+字节地址。
  - `ysyx_26030107`：重新接线本体子模块（IFU/IDU/EXU/LSU/wbu/GRF，不改）+ AXI 桥，
    并用**时钟门控**（`gated = clock & (~busy | reset)`）在访存期间冻结整个单周期核
    （复位期间必须放行时钟，否则同步复位不生效）。
- `resources/EICG_wrapper.v`：`ClockGate` 降级出的技术单元 `EICG_wrapper` 的行为模型
  （带锁存的 ICG，防毛刺）；工具链不带该库，故自带。

## 构建 / 运行
```bash
cd cpu-core/R1322IAe-csr/soc
bash build_soc.sh                 # 自动 gen_core.sh(YSYX_SOC=1) + verilator -> obj_dir/sim_soc
./obj_dir/sim_soc <prog.bin>      # 程序按 riscv32e-ysyxsoc 平台编译(MROM 启动, resetPC=0x2000_0000)
```
`gen_core.sh` 会 `make deploy-core core=R1322IAe-csr` 后以 `YSYX_SOC=1` 运行 sbt 生成
SoC 兼容顶层（`ysyx_26030107`），装到 `soc/rtl/`。

## 验证
`cpu-tests`（`sum/dummy/unalign/fib/quick-sort/string/to-lower-case/...`）在 ysyxSoC 上
均 `HIT GOOD TRAP`；`char-test` 输出 `A`。

## 备注
- 本核 `marchid`（学号标识）已设为 `0x018D301B`（=26030107，与 R1322IAe-13 一致）；
  `mvendorid=0x79737978`("ysyx")。
- **CLINT**：兼容层内建了 `mtime`（读 `0x0200_bff8/bffc` 本地应答，不走 SoC），供 AM 定时器使用；
  `timer2-test`（读 `AM_TIMER_UPTIME`）通过。RT-Thread 的 `date` 可运行（RTC 那条提示是 AM 的
  `__am_timer_rtc` 固定返回 1900，与 CLINT 无关）。
- 尚可扩展：flash/PSRAM 启动已支持（`YSYX_RESET_PC=30000000`）；NVBoard 集成等。

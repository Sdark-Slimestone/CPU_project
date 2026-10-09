# R1322IAe-1：把存储器改成真实 SRAM（同步接口 + 保持 5 周期）

日期：2026-10-08
工具链：Chisel 7.7 / firtool / Verilator 5.046 / yosys 0.62 + iEDA iSTA，PDK：nangate45（typical）

---

## 0. 动机

`make sta` 评估用的“物理 RAM”是**触发器阵列**（`sta/resources/RegisterFile.v`，256×32b，
组合读）。两个存储器（imem/dmem）各 8192 个触发器，共 16384 个，占全设计 DFF 的约 90%，
且 256:1 的组合读 mux 把 F/M 两级的时序拖得很重。真实芯片会用 **SRAM 宏**，因此需要：

1. 用 SRAM 硬宏替换触发器阵列；
2. 但 SRAM 是**同步读（1 拍延迟）**，而原设计在 F/M 是组合读 → 需要调整流水时序；
3. 保持 5 周期/包不变。

---

## 1. 关键结论：行为级 `reg mem[]` 会被综合成触发器

- 若用行为级 `reg [31:0] mem [0:255]` 写存储器，yosys 的 `memory_map` 会把它**展开成触发器阵列**，
  与原来的 `RegisterFile.v` 一样，主频/面积不变。
- 本 PDK 自带 **`fakeram45` SRAM 硬宏**（`yosys-sta/pdk/nangate45/lib/fakeram45_*.lib`），
  只要**实例化为黑盒**并把 `.lib` 加进流程，yosys/iEDA 就会把它当**宏单元**保留并按时序/面积计入。
  实测网表里出现 `fakeram45_256x34 \imem.mem1` 等实例，DFF 从 18212 掉到 1796。

`fakeram45_256x34`：256×34b，**单端口 1R1W，同步读**（`clk` 上升沿后一拍出 `rd_out`，
cell_rise ≈ 0.241 ns，输入 setup ≈ 0.05 ns，min_period ≈ 0.212 ns）。

---

## 2. 同步接口怎么塞进 5 级流水

### 2.1 imem：取指地址提前一拍发起

同步 imem 在**地址发出后的下一拍**才返回数据。IFU 原本在 F 拍发地址、当拍组合读、F/D 桥锁存。
改为：在**包完成的 W 拍**（`done`）就把**下一条 PC** 发给 imem（`fetchAddr = Mux(done, nextPc, pcReg)`），
下一拍（F）数据即有效，F/D 桥照常在 F 拍锁存。

- `canFetch` 改为在 `done` 的**下一拍**才拉高（此时 imem 数据已有效）。
- 复位后先来一拍“预取”（`initFetch`），解决首个包没有前置 `done` 的问题。
- 仍为 5 拍/包（启动多 1 拍）。

### 2.2 dmem：读在 E 拍发起，写在 M 拍发起

- **读**：EXU 在 E 拍就算出地址，直接发给 dmem（`exu_to_dmem_1/2`），同步 SRAM 在 E/M 边沿
  返回数据 → **M 拍的 LSU 用寄存后的读数据**（不再当拍组合读）。
- **写**：仍由 LSU 在 M 拍发出（`lsu_to_dmem_1/2`），在 M/W 边沿写入。
- 单端口宏上**读(E)与写(M)不同拍**，不冲突。
- store-to-load 转发保留：M 拍把 lane1 的 store 数据按字节掩码合并进 lane2 的读结果；
  比较用寄存后的 lane2 读地址（E 拍发出、M 拍比较）。

### 2.3 dmem 双读口：自造 2R1W 宏

PDK 自带的 fakeram45 全是**单端口 1R1W**，没有 2R1W。为让 dmem 用**一个**双端口宏，
自造了 `fakeram45_2r1w_256x32`（黑盒声明 + liberty，见
`yosys-sta/pdk/nangate45/lib/fakeram45_2r1w_256x32.lib`，面积/时序参照单端口宏按
2R1W 估 1.5×）。dmem 只需 1 个宏即可支持「2 读 + 1 写」。

> 备选：不造宏时，也可用 **2 个单端口宏**（一 lane 一个）凑双读口，面积略大
> （实测 49986 vs 47018 µm²），主频在合成噪声范围内相当。

### 2.4 E 级提频

内存换成宏后，瓶颈转到 **EXU（E 级）**：
- `op2 = Mux(isImmOp, imm, rs2)` 里的 `isImmOp` 原来是 9 项一热 OR → 改为 decoder
  一级 opcode 判定（`c.is_imm_op = op_i`），去掉关键路径上的 OR 链；
- EXU 里冗余的 `wb_data = Mux(is_load, 0, alu_out)` 删除（load 由 LSU 覆盖）；
- 一并把 `is_jump/is_branch/is_load/is_store/is_alu_op/is_csr` 都改成 decoder 预计算。

---

## 3. 结果（`make sta core=R1322IAe-1`）

| 指标 | 触发器阵列版 | **SRAM 宏版（2R1W，本文）** |
|---|---|---|
| fmax | 1160.2 MHz | **1216.5 MHz** |
| 关键路径 | 0.822 ns | **0.788 ns** |
| DFF | 18212 | **1840** |
| 总单元数 | 97905 | **15035** |
| 芯片面积 | 184723 µm² | **47018 µm²（−75%）** |
| alutest cycles | 406513 | 406514（+1，启动预取） |
| 功能 | GOOD/ABAB/msh | **GOOD/ABAB/msh** |

- 面积/DFF 大幅下降（两个 1KB 触发器阵列 → 3 个 SRAM 宏：imem×2 + dmem×1）。
- 主频较触发器阵列版 +5%，较最初 SRAM PoC（1141.8）提了约 6.5%。
- 这是**功能正确**的设计（非 PoC）：alutest `HIT GOOD TRAP`、yield-os `ABAB`、
  rtthread 进 `msh />`、定向 store→load 转发测试输出 `P`。

### 3.1 频率还能再提吗？

能，但已进入**各拍均衡**区间：D 级（译码+冒险+GRF 读）、E 级（分支/ALU）、CSR 计数器
三条路径都在 **0.78~0.80 ns** 左右，ABC 每次微调只是把瓶颈在它们之间搬来搬去
（±2~3% 属合成噪声）。要实质提频需要：
1. 把某一拍**再切一级**（如 IDU 拆 D1/D2）——但会变成 6 拍/包，非重叠下净收益有限；
2. 换更快的 SRAM 宏（当前宏读延迟 0.241 ns、min_period 0.212 ns）；
3. 把 debug 端口（1740 个）去掉再做 STA（当前口径偏乐观）。


---

## 4. 流程改动（可回退）

| 文件 | 改动 |
|---|---|
| `yosys-sta/scripts/pdk/nangate45.tcl` | `LIB_FILES` 加入 `fakeram45_256x34.lib` 与 `fakeram45_2r1w_256x32.lib` |
| `sta.sh` | 把 `sta/resources/*.v` 一起拷入 example（原来只拷 `RegisterFile.v`）；放宽生成检查 |
| `cpu-core/R1322IAe-1/sta/resources/fakeram45_256x34.v` | 单端口 SRAM 宏黑盒声明（imem） |
| `cpu-core/R1322IAe-1/sta/resources/fakeram45_2r1w_256x32.v` | **自造**双端口 SRAM 宏黑盒声明（dmem） |
| `yosys-sta/pdk/nangate45/lib/fakeram45_2r1w_256x32.lib` | **自造**双端口 SRAM 宏 liberty |
| `cpu-core/R1322IAe-1/sta/scala/{imem,dmem}.scala` | imem 用 2 个单端口宏、dmem 用 1 个 2R1W 宏（同步读） |
| `cpu-core/R1322IAe-1/scala/{ifu,lsu,dmem,top}.scala` | 同步接口 + 提前一拍发起读/写分离 |
| `cpu-core/R1322IAe-1/resources/DPI_Memory.v` | 日常仿真版也改为**同步读**（与 SRAM 时序一致） |

> 日常 `make npc` 与 `make sta` 用的是同一套同步接口；日常走 DPI 软件 RAM，STA 走 fakeram 宏。

---

## 5. 局限与后续

- fakeram45 只有单端口 1R1W；双读口靠 2 个宏 + 双写实现（面积翻倍于“理想 2R1W 宏”）。
  若换成真正的 2R1W SRAM 宏，面积还能再降。
- STA 口径仍保留全部 debug 端口（1740 个），真实流片去掉后主频会更高；布线后线延迟会进一步降低主频。
- 存储器容量（256×32b）远小于实际程序；真实设计应按程序规模选宏。

# R1322IAe-csr 单周期 vs 多周期（mulcycle / multicycle）—— STA 对比报告

日期：2026-09-09
工具链：yosys 0.62（oss-cad-suite）+ iEDA iSTA，PDK：nangate45（typical 库）
综合策略：yosys_min.tcl 精简流程（coarse synth → memory_map → techmap → 轻量 opt → dfflibmap → abc DELAY-4，目标 500MHz）

## 一、评估方法（按讲义"评估单周期NPC的主频"）

1. **存储器改造**：注释 DPI-C（pmem_read/pmem_write）访问，取指/访存各实例化一个
   **256×32b (1KB) 触发器寄存器阵列**（`RegisterFile.v`，1 个字节使能写口 + 2 个组合读口，
   当前周期返回数据，保持单周期特性）；imem 写口由自由运行计数器驱动以防止综合器
   把无写端口的存储器优化成常数。
2. **防折叠流程**：yosys 迭代 `opt` 会把"未初始化存储器驱动"的数据通路证明折叠掉
   （写使能被证明恒为假、存储器 DFF 被删除，设计只剩 156 个 DFF），因此使用精简
   流程跳过迭代 opt，保证两版设计都以完整数据通路（~1.7 万 DFF）进入时序分析。
3. **fmax 计算**：iSTA 以 100MHz 约束跑时序，fmax = 1/(约束周期 − 最差 slack)，
   与报告 Freq(MHz) 列一致。两版使用相同的综合/STA 配置，对比公平。

## 二、核心数据对比

| 指标 | 单周期 R1322IAe-csr | 多周期 R1322IAe-csr-mulcycle | 对比 |
|---|---|---|---|
| **fmax（主频）** | **444.93 MHz** | **891.43 MHz** | 多周期 **2.04×** |
| 关键路径延迟 | 2.200 ns | 1.078 ns | 多周期 **2.04×** |
| 关键路径终点 | GRF 写数据端寄存器 D | EXU 级寄存器（messageReg）D | |
| TNS（100MHz 约束） | 0（slack +7.752ns） | 0（slack +8.878ns） | 均无违例 |
| **总单元数** | **101,796** | **97,461** | 多周期少 4.3% |
| **芯片面积 (μm²)** | **181,612** | **180,801** | 基本持平（−0.4%） |
| DFF（触发器）数量 | 17,296 | 18,226 | 多周期多 930（级寄存器） |
| microbench train 周期数 | 155,251,468 | 776,270,563 | 单周期的 5.0× |
| **train 估算运行时间** | **0.349 s** | **0.871 s** | 单周期快 2.5× |

### 补充：R1322IAe-csr-multicycle（Decoupled 总线重构版）

用讲义"总线"思想重写的多周期核：各模块端口统一为 `Decoupled`，级间用 1 深度队列式的
`StageConnectBridge`（`valid`/`ready` 握手 + 交出后清零），IDU 之后按 lane 拆成单 lane 消息。

| 指标 | mulcycle | multicycle | 对比 |
|---|---|---|---|
| **fmax（主频）** | 891.43 MHz | **909.34 MHz** | +2.0% |
| 关键路径终点 | EXU 级寄存器 | 桥寄存器 `_bridge_2_io_out_bits_imm_28:D` | |
| 总单元数 | 97,461 | 100,100 | +2.7% |
| 芯片面积 (μm²) | 180,801 | 182,966.5 | +1.2% |
| DFF（触发器） | 18,226 | 18,277 | +51 |

两者微结构相同（5 周期/包、双发射），fmax 与面积基本持平；multicycle 的级间握手更规整
（每 lane 一条消息 + 通用桥），便于后续切 `arch="pipeline"` 或插入新站。

## 三、分析

### 1. 主频：多周期 2.04×
- 单周期：`imem读(256:1 mux) → 译码 → 冒险检测 → GRF读 → ALU → 写回合并 → GRF写`
  一整条组合链在 2.2ns 内完成。
- 多周期：同一链条被级寄存器切成 F→D→E→M→W 五段，最长一段（IDU→EXU：译码 +
  全部冒险检测 + GRF 组合读）仅 1.078ns。
- 结论：级寄存器切短关键路径 → 提频，这正是流水线能跑更高主频的根本原因。

### 2. 面积：基本持平
- 单周期：双发射实例化两套 EXU/LSU（组合逻辑翻倍），无级间寄存器。
- 多周期：只有一套 EXU/LSU，但每级多一个消息级寄存器（IFMsg/IDUToEXUMessage/
  EXUToLSUMessage/LSUToWBUMessage 各约 300+ bit），DFF 反而多 930 个。
- 两项相抵，面积几乎相同（180.8k vs 181.6k μm²）。

### 3. 性能：单周期反而快 2.5×
- 每包指令周期数：单周期 1 周期/包（双发射 2 条），多周期 5 周期/包。
- 时钟虽然快 2.04×，但周期数多 5×，净效果慢约 2.5×。
- 这印证了讲义的取舍：**多周期/流水线化是"用每条指令的周期数换时序余量"**，
  为后续真正的流水线（每周期都发射）铺路。

## 四、文件位置与自动化

**一键 STA（已自动化）**：
```bash
make sta core=R1322IAe-csr              # 单周期（约 25 分钟, abc 占 20 分钟）
make sta core=R1322IAe-csr-mulcycle     # 多周期（约 5 分钟）
make sta core=R1322IAe-csr-multicycle   # 多周期(总线重构版，约 5 分钟)
```
流程（`sta.sh`）：部署核心 → 自动覆盖为物理RAM版存储器（`cpu-core/<core>/sta/`）→
genv → sv2v → yosys 精简综合 → iSTA → **自动还原部署**（日常构建仍是软件RAM版）。
结果打印 fmax/关键路径/DFF/单元数/面积，并存入 `yosys-sta/result/<core>/fmax.txt`。

| 内容 | 路径 |
|---|---|
| 一键 STA 入口 | `makefile` 的 `sta` 目标 + `sta.sh` |
| 单周期 STA 结果 | `yosys-sta/result-singlecycle/top-100MHz/` |
| 多周期 STA 结果 | `yosys-sta/result/R1322IAe-csr-mulcycle/` |
| 单周期 iSTA 日志 | `yosys-sta/result-singlecycle/top-100MHz/sta.log` |
| 触发器存储器（物理RAM） | `cpu-core/<core>/sta/resources/RegisterFile.v` |
| 精简综合脚本 | `yosys-sta/scripts/yosys_min.tcl`（yosys.tcl 的去重优化版） |

## 五、注意事项

- 本评估保留了全部 1740 个调试输出端口（含 GRF debug 16×32 直通），真实流片会去掉，
  对主频影响偏乐观；布局布线后线延迟会进一步降低主频（讲义已提示）。
- 存储器为触发器阵列（各 1KB），远小于实际程序需求；真实设计应使用 SRAM 宏单元。
- 多周期核当前为"一次一包"的分布式多周期（非流水线），若切换 `StageConnect` 的
  `arch = "pipeline"` 并补齐冒险/冲刷逻辑，fmax 应保持 ~891MHz 而周期数趋近每周期一包，
  性能将反超单周期。

## 六、结论

| 场景 | 选择 |
|---|---|
| 功能仿真 / difftest / 跑 benchmark | 单周期或多周期均可（软件 RAM 版本） |
| 面积敏感 | 两者几乎相同，无差异 |
| 主频/时序余量敏感 | 多周期（891MHz vs 445MHz） |
| 程序运行性能（当前形态） | 单周期（快 2.5×） |
| 未来流水线化 | 多周期架构是必经之路 |

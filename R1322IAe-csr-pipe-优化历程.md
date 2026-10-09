# R1322IAe-csr-pipe 架构优化历程

日期：2026-09-14
工具链：yosys 0.62（oss-cad-suite）+ iEDA iSTA，PDK：nangate45（typical 库），
综合策略：`yosys-sta/scripts/yosys_min.tcl`（coarse synth → memory_map → techmap → 轻量 opt → dfflibmap → abc DELAY-4）
评估：`bash sta.sh <core>`，`CLK_FREQ_MHZ=100`，fmax 取 iSTA 报告 Freq 列。

功能验证：`make npc core=<core> sim=no` 后跑
`alutest`（看 `HIT GOOD TRAP` 与周期数）、`yield-os`（看是否输出 `ABAB`）、`rtthread`（看是否进 `msh />`）。

---

## 0. 背景

项目里有三个相关核心：

| 核心 | 架构 | 微结构 |
|---|---|---|
| `R1322IAe-csr` | RV32E + Zicsr | 同构双发射，**单周期**（1 包/拍） |
| `R1322IAe-csr-multicycle` | RV32E + Zicsr | 双发射，**多周期**（F→D→E→M→W，5 周期/包，一次一包不重叠） |
| `R1322IAe-csr-multicycle-2` | 同上 | 把 D 拆成 IDU1/IDU2，6 周期/包 |
| `R1322IAe-csr-pipe` | 同上 | **真流水线**（每拍进一包），本次优化对象 |

起点数据（`STA-对比报告.md` 与本次复现）：

| 指标 | 单周期 `R1322IAe-csr` | `multicycle` | `multicycle-2` |
|---|---|---|---|
| fmax | **444.93 MHz** | 909.34 MHz | 1163.67 MHz |
| 关键路径 | 2.200 ns | 1.053 ns | 0.814 ns |
| 周期/包 | 1 | 5 | 6 |
| alutest cycles | **81481** | ~407408 | 488890 |
| alutest 运行时间 | **183.1 µs** | ~448 µs | 420.1 µs |

结论：`multicycle` 系列主频高，但每包 5~6 拍，实际比单周期慢 2 倍多。
目标：把 `multicycle-2` 改造成真流水（每拍一包），既吃高频又吃 IPC，最终**反超单周期**。

---

## 1. 阶段一（`multicycle-2`）：逻辑优化提频

**瓶颈**：STA 显示最差路径 `_bridge_2_io_out_bits_imm_*`（IDU→EXU2 的 D/E 桥寄存器），
起点是 IFU→IDU 桥（取回的指令），即 **D 级整坨组合逻辑**：取指→译码→冒险→GRF 读→选通。

**改动**：
1. `decoder.scala`：`InstructionDecoder` 增加 opcode 级粗格式输出
   （`is_branch/is_jump/is_store/is_load/is_system/is_imm_*/wr_rd/use_rs1/use_rs2`）；
   `InformationDecoder` 的立即数选择、rd/rs 使能全改用它，去掉十几项一热 OR 与深层 PriorityMux。
2. `idu.scala`：`isControl1/2`、`isStore1/2`、`isLoad1/2` 改用粗格式；lane2 的
   `imm/rs1_val/rs2_val/nextpc` 不再过 `Mux(final_stall,0,·)`（op 全 0 时下游无副作用）。
3. `csr.scala`：`mcycle`/`minstret` 从 64 位直加拆成低/高 32 位两条并行进位链。

**结果**：fmax 909.34 → **989.59 MHz**（+8.8%），alutest 周期不变。新瓶颈变成 `mcycle` 高位进位链。

---

## 2. 阶段二（`multicycle-2`）：拆 D 级为 6 级

**改动**：把 IDU 拆成
- `idu1.scala`（原 `idu`）：只做译码 + GRF 组合读；
- `idu2.scala`：只做冒险检测 + `final_stall` 选通；
- `bus.scala` 新增 `IDU1_to_IDU2_Message`；
- `top.scala`：D1/D2 间插一个 `StageConnect`，`capture_stall` 改取 D2 输入 valid；
- `ifu.scala`：pc 从 5 拍变 6 拍。

**结果**：fmax 989.59 → **1093.35 MHz**（+10.5%），周期/包 5→6。
实测 alutest 488890 cycles，**实际运行时间反而略慢**（420 µs）——这就是“用周期数换频率”。

随后又做了两处小优化（针对 `ifu.branchTargetReg` 瓶颈）：
- `nextpc` 字段改成 `pc`（`inst_pc` 本就等于 `nextpc-4`），去掉 EXU 的减法；
- 去掉 `top.scala` 的 exu1/exu2 分支目标合并 Mux（lane2 控制被清零，永不 take_branch）。

`multicycle-2` 最终：fmax **1163.67 MHz**，0.814 ns，DFF 18623，area 187708 µm²。

此时文件结构：`idu1.scala` / `idu2.scala` 两级分离，消息进 `bus.scala`。

---

## 3. 阶段三（`R1322IAe-csr-pipe`）：真流水线

**从 6 级骨架出发**，把级间 `StageConnect`（“空才能收”的 1 深度握手，一次一包）
换成可每拍吞吐的流水桥，并补齐跨包冒险与冲刷。

**改动**：
1. `bus.scala`：新增 `StagePipeBridge` / `StagePipe`——每拍可进一包，支持
   `stall`（冻结）与 `flush`（冲刷）。**关键：寄存器无效时必须把 bits 清零**，否则下游
   （`lsu.scala`/`wbu.scala`）会拿旧包位产生副作用。
2. `ifu.scala`：重写为每拍推进（默认 pc+8），新增 `redirect`/`redirect_pc`；
   去掉 `done/capture_*`。
3. `idu1.scala`：新增跨包 RAW interlock——当前包源寄存器 vs 在飞指令（IDU2/EXU/LSU/WBU）
   的 rd，命中则 `stall` 阻塞前端直到写回。
4. `idu2.scala`：暴露 `final_stall` 作为单发射重定向（取 `pc+4`）。
5. `top.scala`：级间全改 `StagePipe`；汇总 redirect/flush/stall。

**踩坑（重要）**：第一版跑 alutest 出 `HIT BAD TRAP`。逐周期比对（用已知正确的
`multicycle-2` 做参考，打印 GRF 变化序列 diff）定位到：**`StagePipeBridge` 失效时没清零 bits**，
导致同一个 `sw` 被重复执行 5 次、一条 `lw` 用旧 a5 重复执行，最终写坏寄存器。
修复后 alutest 488890 → **351370 cycles**（−28%），`HIT GOOD TRAP`。

此后真正的流水线只跑通但没比单周期快：每包 ~1 拍，但 6 级流水“取指超前”使得每次
重定向/冒险都要冲刷掉已取的包，而 `multicycle-2` 的冒险检测只覆盖包内、跨包 RAW 全靠阻塞。

---

## 4. 阶段四：数据转发（去掉“阻塞到写回”）

**改动**：`idu1.scala` 分层转发
- 来源 `exu.scala`(K=2) / `lsu.scala`(K=3) / `wbu.scala`(K=4)，优先级 EXU > LSU > WBU，同组 lane2 优先；
- 只对 **K=1（紧邻包，结果还没出来）** 和 **K=1 load（数据没好）** 阻塞。

**结果**：alutest 351370 → **228720 cycles**（−35%）；fmax 从 1079.48 降到 1018.02 MHz
（EXU ALU → IDU1 转发 mux 成为新关键路径）。运行时间 325 → 224.7 µs。

---

## 5. 阶段五：把 GRF 读挪到 IDU2（紧邻依赖 0 阻塞）

**动机**：GRF 读在 IDU1 时，紧邻包（K=1，在 IDU2）结果没出来，只能停 1 拍。
把读放到 IDU2 后，读距 EXU 只差一级：K=1 若在 EXU，ALU 结果本拍可得，直接转发、**0 阻塞**。

**改动**：
- `bus.scala`：`IDU_to_EXU_Lane_Message` 增加 `rs1_addr/rs2_addr`；
- `idu1.scala`：只译码，不读 GRF、不转发/阻塞；
- `idu2.scala`：新增 GRF 组合读 + 转发 + 阻塞 + 冒险；
- `top.scala`：GRF、转发都接 `idu2`；`stall_d` 来自 `idu2` 并同时冻结 R_IF_D 与 D1/D2；
  单发射重定向用 `!idu2.stall` 门控。

**结果**：228720 → **197804 cycles**（−13.5%）。

---

## 6. 阶段六：BTFN 静态分支预测

**改动**：
- `exu.scala`：`pred_taken = is_branch && imm(31)`（向后分支预测 taken）；
  只有 `branch_cond =/= pred_taken` 才重定向，目标 `Mux(branch_cond, pc+imm, pc+4)`；
- `top.scala`：IDU2 侧对 lane1 向后条件分支预测 taken，重定向直接取分支目标。

**结果**：197804 → **192238 cycles**（−2.8%，alutest 循环分支少，收益有限）。

---

## 7. 阶段七：各种“砍单发射”的优化（收益最大的一轮）

用临时调试口统计 `final_stall` 原因（占 47956/186937 ≈ 25.7%）：
`ctl1`(lane1 是控制) 22723、`raw`(包内 RAW) 17146、`ctl2`(lane2 是控制) 12456、mem 2413。

### 7.1 包内前递（干掉 `raw`）
`exu.scala` 加 `intra_fwd`：exu1(lane1) 的结果直接旁路到 exu2(lane2) 的操作数；
只有 lane1 是 load（数据没好）才单发射。
**186937 → 161957**（−13.4%），raw 事件 17146 → 6915。

### 7.2 lane2 分支/跳转并行执行
`idu2.scala`：只有 lane2 的 CSR/异常/mret 才强制单发射；lane2 的分支/跳转由 exu2 自己
重定向（`top.scala` 合并 exu1/exu2 的 redirect）。
**161957 → 124541**（−23%），`ctl2` 事件归零，`ctl1` 22723 → 15410。

### 7.3 lane1 向前条件分支双发射
预测 not-taken，lane2 在正确路径上就一起发；若实际跳了，`exu.scala` 给 exu2 发 `kill`
打掉 lane2 写回。
**124541 → 117713**（−5.5%），`ctl1` 15410 → 6198。

---

## 8. 频率崩塌与回退（重要教训）

对这些激进优化跑 STA 后 **fmax 从 1018 崩到 437 MHz（2.246 ns）**。
关键路径变成：
```
dmem 256:1 读 mux → MEM→EX bypass → EXU 操作数 → 分支比较 → 重定向 → IFU pc
```
即 7.1 之前加的 **MEM→EX bypass**（为消 load-use 阻塞，把 load 数据从 LSU 旁路到 EXU）
把 dmem 的大读 mux 串进了“分支→重定向→pc”这条本来就深的路。

**回退 MEM→EX bypass**（恢复 K=1 load-use 阻塞，代价仅 +2.8% 周期）：fmax 437 → 608 MHz。
但仍有另一条 ~1.6 ns 路径：
```
exu1 移位器(ALU) → intra_fwd → exu2 分支比较 → exu2 重定向 → IFU pc
```
以及 IDU2 的 GRF 读 + 转发布塞链（`_bridge_3_io_out_bits_rs2_val`，1.627 ns）。
还试过“`raw` 且 lane2 是分支/跳转时仍单发射”来切断第一条路径，但 fmax 没涨（路径转移），
周期反而 +10%，遂回退。

**教训**：把转发/读口放进流水级、并让它们参与“分支→重定向→pc”这条链，
很容易把单个流水级拉长到 1.6 ns 以上；频率优势会被吃掉。
真正的流水线需要把重定向/分支决议单独安排（预测+寄存器化），而不是全组合一坨。

### 补充：两次无效的 fmax 实验（均已回退）

| 实验 | 想法 | 结果 |
|---|---|---|
| `clearBitsOnFlush=false`（IF/D、D1/D2 桥 flush 只清 valid 不清 bits） | 切断 `redirect→flush→bits` 长路径 | fmax 608→573 MHz（瓶颈其实是 `redirect→IFU pc`，与 bits 无关），回退 |
| `raw` 且 lane2 是分支/跳转时仍单发射 | 切断 `exu1 ALU→intra_fwd→exu2 分支比较` | 关键路径只是从 A 转移到 B（`_bridge_3_io_out_bits_rs2_val`，1.627 ns），fmax 没涨、周期 +10%，回退 |
| **寄存器化 EXU 重定向**（EXU 组合决议后打一拍再应用到 IFU/冲刷，并 kill 掉下一拍落进 EXU 的错误路径包） | 切断路径 A（EXU→redirect→pc/flush） | 功能正确，但 fmax 608→591 MHz、周期 +11.6%（每次重定向多 1 拍）；关键路径转移到 B′：`IDU2 GRF读+转发 → ramraw/rambank → final_stall → single_issue → flush → R_D_HAZ`。回退 |

说明：当前有 **两条 ~1.6 ns 的关键路径**，且互相“接力”：
- A：`exu1 ALU → intra_fwd → exu2 分支比较 → 重定向 → IFU pc / flush`；
- B：`IDU2 GRF 组合读 + 转发 → 单发射/冒险判定 → flush / R_D2_E`。

只切其中一条，另一条立刻顶上（实测 A→B→B′ 都 ~1.6 ns）。要真正提频必须**同时**：
1. 把 EXU 分支决议寄存器化（切 A，代价每次重定向 +1 拍）；
2. 把 IDU2 的 `GRF 读` 与 `转发/冒险` 拆成两级（切 B），但要注意跨级写回可见性：
   读提前一级后，本来“已退休 1 拍”的 K=4 生产者会变成读不到，需要 write-first 旁路或 WB bypass 寄存器。
两者叠加预期能把 1.6 ns 砍到 ~0.8 ns（fmax ~1.2 GHz），但周期也会增加（重定向 +1 拍、拆分可能多 load-use 阻塞），
属于较大且高风险的重构。

---

## 9. 最终数据（R1322IAe-csr-pipe）

| 指标 | 值 |
|---|---|
| fmax | **608.39 MHz** |
| 关键路径 | 1.600 ns |
| DFF | 18440 |
| 总单元数 | 98850 |
| 芯片面积 | 190990 µm² |
| alutest cycles | **122603** |
| alutest 运行时间 | **201.5 µs** |

### alutest 周期演进（同一程序）

| 版本 | cycles | 相对单周期 |
|---|---|---|
| 单周期 `R1322IAe-csr` | **81481** | 1.00× |
| pipe（interlock，无转发） | 351370 | 4.31× |
| +转发（GRF 读 IDU1） | 228720 | 2.81× |
| +GRF 读挪 IDU2 | 197804 | 2.43× |
| +BTFN | 192238 | 2.36× |
| +MEM→EX bypass | 186937 | 2.29×（已回退） |
| +包内前递（raw） | 161957 | 1.99× |
| +lane2 分支并行 | 124541 | 1.53× |
| +lane1 向前分支双发射 | 117713 | 1.44× |
| 回退 MEM→EX 后定版 | 122603 | 1.50× |

### 频率与运行时间对照

| 核心 | fmax | alutest cycles | 运行时间 |
|---|---|---|---|
| 单周期 `R1322IAe-csr` | 444.93 MHz | 81481 | **183.1 µs** |
| `multicycle` | 909.34 MHz | ~407408 | ~448 µs |
| `multicycle-2` | 1163.67 MHz | 488890 | 420.1 µs |
| `pipe`（interlock） | 1079.48 MHz | 351370 | 325.5 µs |
| `pipe`（IDU1读+转发） | 1018.02 MHz | 228720 | 224.7 µs |
| `pipe`（全部激进优化） | 437.02 MHz | 117713 | 269.3 µs |
| **`pipe`（回退 MEM→EX 后）** | **608.39 MHz** | **122603** | **201.5 µs** |

---

## 10. 结论

- 纯逻辑/结构优化（阶段一、二）把 `multicycle-2` 从 909 提到 1164 MHz，是稳赚的；
  真流水化（阶段三~七）把每包周期从 6 降到 ~1.5，**周期数砍掉 66%**。
- 但在“触发器阵列当 RAM + 共享组合逻辑”的 STA 口径下，**IPC 换来的频率损失更大**：
  最终 `pipe` 主频 608 MHz，比单周期只高 1.37×，而周期数是单周期的 1.50×，
  两者相抵后 **pipe 仍比单周期慢约 10%**（201.5 vs 183.1 µs）。
- 单周期凭“把一整条链压进 1 拍（2.2 ns）”拿到 1 拍/包，在这套口径下依然是最快的。

**要真正反超单周期，下一步应做**：
1. 把**分支决议/重定向寄存器化**（预测取指 + 下一拍才重定向），把 1.6 ns 的
   `转发→比较→重定向→pc` 链切断；或把分支决议挪到更早的级；
2. 把 IDU2 的 GRF 读+转发再拆一级，或将转发改成寄存器化旁路；
3. （可选）MEM→EX bypass 要用“寄存器化的 load 数据旁路”，而不是把 dmem 读 mux 直接串进 EXU。

---

## 附：调试手段

- **不能**用 `make ... sim=diff/diff2`：那套 diff 是给单周期准备的重试模型，不支持多周期/流水。
- 定位流水线错误：把已知正确核（`multicycle-2`）和待测核都用一个临时 testbench 打印
  `io_debug_grf_regs[0..15]` 的变化序列，`diff` 出第一处分歧，再对齐周期找指令。
- 统计单发射原因：`idu2.scala` 里临时加 `reason = Cat(stall_sig2, mem, is_control1, raw)`，
  在 testbench 里按位计数。当前 `R1322IAe-csr-pipe` 保留了 `io.debug_stall_reason` 口与
  testbench 计数打印，用于继续调优。

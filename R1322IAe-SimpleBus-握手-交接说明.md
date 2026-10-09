# R1322IAe「完整握手 SimpleBus」交接说明（任务 / 思路 / 验证 / 踩坑）

日期：2026-10-10
作者留：本文件面向**接手"支持完整握手信号的 SimpleBus 协议"这一棒**的人。
一句话结论：**这套已经做完了**，在 `R1322IAe-7`（带随机刺激）和 `R1322IAe-8`（干净版）里。
下面把任务、对应实现、还没做的那点、验证方法、以及我踩过的所有坑写清楚。

---

## 0. 任务（讲义原文要点）

在 SimpleBus 上加两个握手信号，实现**完整握手**：

```
CPU/IFU/LSU 侧（master）:
  output reqValid      input  reqReady     # 发请求握手
  output addr/wen/wdata/wmask
  input  respValid     output respReady    # 收响应握手
  input  rdata
MEM 侧（slave）:
  input  reqValid      output reqReady
  output respValid     input  respReady
```

要求：
1. 让 **IFU 和 LSU 按完整握手**（`reqValid/reqReady` + `respValid/respReady`）访问存储器。
2. 跑测试程序，**看波形**确认通信过程符合预期（master/slave 各两次握手）。
3. **对 `reqReady`、`respReady` 加随机延迟**，更充分地测试总线实现。

---

## 1. 现状：已完成，落在 -7 / -8

| 核 | 说明 |
|---|---|
| `cpu-core/R1322IAe-7/` | 完整握手 + **随机刺激**（存储器随机读延迟 LFSR + IFU/LSU 侧 step3 随机握手） |
| `cpu-core/R1322IAe-8/` | = -7 **去掉随机刺激**的干净握手核，作为做缓存的基线 |

两者在固定延迟下均全过：alutest / 39×cpu-tests / microbench / coremark / M自检 / yield-os / **rtthread(进 msh)**。
`-7` 在随机刺激下同样全过。STA：`-8` fmax ≈ **774.5 MHz**，`-7`(step3开) ≈ 614.7 MHz。

---

## 2. 实现结构（思路）

### 2.1 存储器 `resources/DPI_Memory.v` —— 每读口一个 `vmem_rdport`
- 请求 FIFO（深度 `DEPTH`）保存 `{addr, delay, tag}`；
  - `reqReady = (cnt < DEPTH)`：**队列满就拉低 reqReady 背压**（master 必须等）。
  - 收到请求（`reqValid && reqReady`）时压入。
- 队头 `delay==0` 时 `respValid=1`、`rdata=pmem_read(队头addr)`、`respTag=队头tag`；
  - **保持到 `respReady` 才弹出**（master 没准备好就一直等）。
- 响应**按序返回**；`flush` 清空在途。
- `LATENCY`：固定读延迟；`DEPTH1/DEPTH2`：各口在途深度。
- 关键实现点：**"计算下一拍状态（组合 `vn/dn/an/tn`）+ 顺序块定长展开写回"**，
  避免 Verilator 对"动态下标数组写"失效（见 §6 坑 1）。

### 2.2 IFU `scala/ifu.scala` —— 解耦取指
- 不再用"深度=延迟的移位流水"，改成**未完成请求元数据 FIFO**（`MetaFIFO`，深度 `MemCfg.depth`）。
- 发请求：`ifu_reqValid = !redirect && metaFIFO.enq.ready`，`!reset` 门控；
  `reqReady` 有效才把 `{pc,num,pred}` 压入并推进 `pcIssue`。
- 收响应：两读口 `respValid1&&respValid2` 都有效才把这一对压入取指队列 FQ；
  `respReady = fq.push_ready`；同时弹元数据（存储器按序返回，队头即当前响应）。
- `redirect` 时清元数据 FIFO 并让存储器 `flush`。

### 2.3 LSU / load 通路 `scala/top.scala` + `scala/idu.scala` —— **tag 回配**
- **IDU 按发射序给每条指令分配 `ldTag`**（16bit 递增），随 D/E 消息携带。
- dmem 读请求带 `tag`；存储器响应带 `respTag`。
- load 在 E 级等到 **`respValid && respTag==ldTag`** 才放行，并 `RegEnable` **锁存数据**；
  `respReady` 把该响应消费（或对孤儿响应排空）。
- **`mem_stall = (E 是 load && 还没等到本 load 的响应)`**；StoreBuffer 用 M 级地址直比。
- **`m_load_haz`（M 级 load-use 互锁）常开**（-7/-8 的 load 有握手等待）。
- **`flush_de` 一律用"延后冲刷"**（重定向源可能因访存冻结滞留在 E）。
- 写通道：M 级由 LSU 发 `wen/waddr/wdata/wmask`（立即完成，不参与握手）。

### 2.4 为什么需要 tag？（孤儿响应）
错路径 load 在 E 级已把请求发给了存储器，随后被重定向冲掉 → 它的响应回来时"没人要"。
若端口是单笔在途（DEPTH=1），这笔孤儿会把端口卡死 → 后面的 load 读到 0。
**tag 回配**让"不是当前这条 load 的响应"被安全丢弃（排空），不误伤正确 load，也无需按重定向乱冲 dmem。

---

## 3. 讲义第 3 点（reqReady/respReady 随机延迟）—— 已完成

| 信号 | 现状 |
|---|---|
| 存储器读延迟（`respValid` 时机）| ✅ 已随机：`DPI_Memory.v` 每口独立 LFSR，`delay=1..MAXLAT`（`-7` 的 `MemCfg.random/maxLat`）|
| **`respReady` 随机延迟**（master 侧）| ✅ 已随机：`-7` 的 **step3**——top 里 16bit LFSR 产生 `randOk`(≈3/4 为 1)，`randOk=0` 那拍 IFU/LSU 不消费响应（`respReady` 拉低），逼存储器保持 `respValid` |
| **`reqReady` 随机延迟**（slave 侧）| ✅ 已随机：`-7` 的 `MemCfg.randReq`——`DPI_Memory.v` 里一个**共享** 16bit LFSR（`RANDREQ` 参数）随机把两读口的 `reqReady` 一起拉低（"随机忙碌"，逼 master 重试）|

- **共享同一位是关键**：imem 两读口（成对取指）/ dmem 两口（lane1/lane2）每拍一致地接收或拒绝，
  否则两口 `reqReady` 不一致会导致请求错位。
- 三开（随机访存延迟 + `respReady` 随机 + `reqReady` 随机）下全部负载通过。
- 相关代码：`cpu-core/R1322IAe-7/scala/top.scala`（搜 `randLfsr/randOk`，master 侧）、
  `cpu-core/R1322IAe-7/resources/DPI_Memory.v`（搜 `RANDREQ/req_lfsr/req_block`，slave 侧）。

---

## 4. 验证方法

### 4.1 跑测试程序（日常）
```bash
make npc core=R1322IAe-8 sim=prof
cpu-core/R1322IAe-8/npc-core/npc-prof test-benchmarks/alutest/alutest-riscv32im-npc.bin   # PASS
cpu-core/R1322IAe-8/npc-core/npc-prof test-benchmarks/coremark/coremark-riscv32im-npc.bin   # PASS
cpu-core/R1322IAe-8/npc-core/npc-prof test-benchmarks/microbench/microbench-riscv32im-npc.bin test
timeout 20 cpu-core/R1322IAe-8/npc-core/npc-prof test-benchmarks/rtthread/rtthread-riscv32im-npc.bin  # 进 msh
# cpu-tests 基线合计(rv32e)：见 R1322IAe-5 文档
```

### 4.2 看波形确认握手（讲义要求）
当前 `make npc` 的 verilator **没开 `--trace`**。两种做法：
1. **开 VCD/FST**：在 `makefile` 的 npc 配方 verilator 命令行加 `--trace-fst`（或 `--trace`），
   `sim_main_io_*.cpp` 里在循环内加 `top->trace(Verilated::time(), 5);`，`main` 前后 `Verilated::traceEverOn(true);`，
   跑几拍后用 `gtkwave` 看。重点看 IFU/LSU 与存储器的：`reqValid/reqReady`（一次握手）、
   `respValid/respReady`（再一次握手）、`addr/rdata`。
2. **不开波形也能查**：`-7` 留了一堆 `debug_*` 输出 + `sim_main_io_prof.cpp` 的 `TRACE` 分支，
   可打印每拍 ifu/load 地址、`respValid`、`dmem` 端口状态（`debug_dmem_*`）。设 `TRACE=1` 跑 `npc-prof`。

### 4.3 关键观察点（"是否符合预期"）
- 读事务：master 先等 `reqReady` → slave 收请求 → 若干拍后 `respValid` → master `respReady` 收数据。**两次握手**。
- 写事务：只有请求侧一次握手（`respValid` 也会例行给一拍）。
- 背压时：`reqReady` 拉低期间，master 的 `addr/reqValid` **保持不动**；`respReady` 拉低期间，`respValid` 保持。

---

## 5. 文件 / 开关地图

| 想改的东西 | 位置 |
|---|---|
| 握手协议主体 | `resources/DPI_Memory.v`（`vmem_rdport` + `DPI_Memory`）|
| 取指解耦 | `scala/ifu.scala` + `scala/bus.scala` 的 `MetaFIFO` |
| load tag / load 等待 / mem_stall | `scala/top.scala`（搜 `ld_for_me`、`load_wait`、`mem_stall`）、`scala/idu.scala`（搜 `ldTag`、`tagCnt`）|
| 固定延迟 / 深度 | `scala/bus.scala` 的 `MemCfg{latency, depth}`；`-7` 另有 `random/maxLat/randValid` |
| ~~随机刺激~~ | `-7` 有：`DPI_Memory.v` 的 `RANDOM/MAXLAT/lfsr`；`top.scala` 的 `randLfsr/randOk`。`-8` 已删 |
| STA 评估 | `sta.sh <core>`（SRAM 版在 `sta/scala/imem.scala`、`sta/scala/dmem.scala`，注意接口要与 top 对齐）|

---

## 6. 踩过的坑（务必逐条避开）

1. **Verilator 里"动态下标数组写"永不生效**：`always @(posedge)` 内 `a[idx]<=x`（idx 是 wire/变量）
   会被优化掉，`a[]` 恒为 0 → 取指数据全 0。
   **解法**：像 `v/d` 那样用**组合 `an[]` + 顺序块定长展开 `for i: a[i]<=an[i]`**（见 `vmem_rdport`）。
2. **复位期 IFU 发请求**：元数据 FIFO 被 Chisel 复位清零，而 `DPI_Memory` 黑盒**无复位**，两边错位
   （取指 pc/inst 差一位，程序跑飞）。**解法**：`ifu_reqValid` 加 `!reset.asBool`；并保证存储器侧初始为 0。
3. **dmem 端口被"孤儿响应"卡死**：错路径 load 的请求没人消费，`DEPTH=1` 的端口被占死，
   后续 load 读到 0（现象：coremark `get_seed_32` 跳转表读 0 → `jr` 跳到 `.rodata`）。
   **解法**：**请求带 tag、响应按 tag 回配**，不是本 load 的响应就排空丢弃。
4. **M 级 load-use 互锁被 `latency>1` 门控关掉**（从 -6 抄来时的坑）：-7 的 load 握手等待 ≥1 拍，
   消费者可能在 load 停在 M（`wb_data` 还是地址、不能转发）时进 E → 读到陈旧值
   （现象：rtthread 里 `rt_thread_self()` 后隔几条的 `beqz a0` 拿到旧 a0 → 误判调度器可用 → 断言）。
   **解法**：`m_load_haz` **常开**。
5. **重定向源被冻结在 E 时被寄存化 flush 冲掉**：-7 的 load 会握手等待，重定向源可能被 mem_stall
   冻在 E；`flush_de` 打一拍后把它（及同拍更老的 lane）冲掉（现象：`rt_thread_self` 里 `lw a0`/`ret` 被误杀）。
   **解法**：-7/-8 的 `flush_de` **一律"延后冲刷"** `(flush_pending||tgt_mis)&&!e_holds_src`。
6. **StoreBuffer 深度别乱放大**：从 -6 抄来的 `N=max(latency,2)` 是对的。
   我一度按"随机延迟"放大到 44，**STA 从 758→580 MHz、DFF 翻倍**。
   其实 -7 的 load 读发生在 store 写回内存**之后**，转发基本用不上 → **N=2 就够**（全负载验证过）。
7. **`LATENCY[8:0]` 这种"对整数参数位选"不合法**（Verilator 告警/可能错）→ 直接赋 `LATENCY`。
8. **单 lane 探针会骗人**：只看 lane1 的 retire 会把 lane2（双发射）的指令漏掉，导致对比出现假差异。
   对比 -6/-7 时要**两个 lane 都打**；且注意"双发射交织"会造成同一条回边循环的相位差，不是真分歧。
9. **STA 版存储器要与 top 接口对齐**：`make sta` 会用 `sta/scala/{imem,dmem}.scala` 覆盖主版本。
   改了 top 的握手端口后，别忘了同步 sta 版（本轮给 `-7/-8` 的 sta imem/dmem 补过 `reqReady/respValid/respReady/reqTag/respTag` 和 `dbg_*`）。
10. **给存储器两读口加随机 `reqReady` 时，两口必须用"同一个"随机位**：imem 是成对取指、dmem 是
    lane1/lane2，若两口 `reqReady` 各自随机就会不一致 → 一个口接收、另一个不接收 → 请求错位。
   `-7` 的做法：LFSR 放在 `DPI_Memory` 顶层产生一个共享 `req_block`，两口都用它（见 `DPI_Memory.v`）。

---

## 7. 参考
- `R1322IAe-7-优化历程.md`：完整握手 + 随机刺激的实现与 STA 对比（任务 2、3 的落点）。
- `R1322IAe-8-说明.md`：干净握手核（任务 1 的落点，作为缓存基线）。
- `R1322IAe-6-交接说明.md`：上一棒（固定 L 档）留下的状态与背景。

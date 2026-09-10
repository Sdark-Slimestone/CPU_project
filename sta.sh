#!/bin/bash
# 一键 STA 评估: make sta core=<核心名>
# 流程: 部署核心 -> 覆盖为物理RAM版(RegisterFile)存储器 -> genv -> sv2v
#       -> yosys 精简综合 -> iSTA -> 还原部署(软件RAM版)
# 产物: yosys-sta/result/<核心名>/{top.netlist.v, synth_stat.txt, sta.log, fmax.txt}
set -u

CORE="${1:-}"
PROJ="$(cd "$(dirname "$0")" && pwd)"
STA_DIR="$PROJ/yosys-sta"
MYCHISEL="$PROJ/MyChisel"
CORE_DIR="$PROJ/cpu-core/$CORE"
export PATH="$HOME/oss-cad-suite/oss-cad-suite/bin:$PATH"

die() { echo "错误: $*"; exit 1; }

# ---------- 前置检查 ----------
[ -n "$CORE" ] || die "用法: bash sta.sh <核心名>"
[ -d "$CORE_DIR" ] || die "核心目录 $CORE_DIR 不存在"
[ -f "$CORE_DIR/sta/scala/imem.scala" ] && [ -f "$CORE_DIR/sta/scala/dmem.scala" ] \
  || die "核心 $CORE 缺少 sta/scala/{imem,dmem}.scala (STA物理RAM版存储器)"
[ -f "$CORE_DIR/sta/resources/RegisterFile.v" ] || die "核心 $CORE 缺少 sta/resources/RegisterFile.v"
[ -x "$PROJ/sv2v" ] || die "sv2v 工具 $PROJ/sv2v 不存在"
[ -f "$STA_DIR/bin/iEDA" ] || die "iEDA 不存在, 请先运行 make -C yosys-sta init"
[ -f "$STA_DIR/scripts/yosys_min.tcl" ] || die "精简综合脚本 yosys_min.tcl 不存在"

# 无论成功失败, 退出前都还原部署(日常构建用软件RAM版)
restore() {
  echo "=== STA: 还原部署(软件RAM版) ==="
  make -C "$PROJ" deploy-core core="$CORE" >/dev/null 2>&1
}
trap restore EXIT

# ---------- 1) 部署核心 ----------
echo "=== STA: 部署核心 $CORE ==="
make -C "$PROJ" deploy-core core="$CORE" >/dev/null 2>&1 || die "部署核心失败"

# ---------- 2) 覆盖为物理RAM版存储器 ----------
echo "=== STA: 覆盖为物理RAM版存储器(RegisterFile) ==="
# 兼容两种核心目录结构: mulcycle 用 scala/core/, multicycle 用扁平 scala/
if [ -d "$MYCHISEL/src/main/scala/core" ]; then
  MEM_SCALA_DIR="$MYCHISEL/src/main/scala/core"
else
  MEM_SCALA_DIR="$MYCHISEL/src/main/scala"
fi
echo "    存储器源文件目标: $MEM_SCALA_DIR"
cp "$CORE_DIR/sta/scala/imem.scala" "$MEM_SCALA_DIR/imem.scala"
cp "$CORE_DIR/sta/scala/dmem.scala" "$MEM_SCALA_DIR/dmem.scala"
cp "$CORE_DIR/sta/resources/RegisterFile.v" "$MYCHISEL/src/main/resources/RegisterFile.v"
# firtool 降低选项: 避免 always 块内 automatic 声明 (幂等, 还原部署时自动恢复)
sed -i 's|firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")|firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable", "--lowering-options=disallowLocalVariables")|' \
  "$MYCHISEL/src/main/scala/top.scala" 2>/dev/null || true

# ---------- 3) 生成 Verilog ----------
echo "=== STA: 生成 Verilog (sbt+firtool) ==="
rm -f "$MYCHISEL"/verilog/*.sv
make -C "$PROJ" genv >/dev/null 2>&1 || true
grep -q "RegisterFile" "$MYCHISEL/verilog/imem.sv" || die "Verilog 生成失败(imem.sv 中未找到 RegisterFile), 检查 sbt 日志"
[ -f "$MYCHISEL/verilog/top.sv" ] || die "Verilog 生成失败(top.sv 不存在)"

# ---------- 4) sv2v 转换 ----------
echo "=== STA: sv2v 转换 ==="
rm -f "$STA_DIR"/example/*.v "$STA_DIR"/example/*.sv
cp "$MYCHISEL"/verilog/*.sv "$STA_DIR/example/"
cp "$MYCHISEL"/verilog/RegisterFile.v "$STA_DIR/example/"
for f in "$STA_DIR"/example/*.sv; do
  "$PROJ/sv2v" "$f" > "${f%.sv}.v" && rm -f "$f"
done

# ---------- 5) yosys 精简综合 ----------
RESULT="$STA_DIR/result/$CORE"
rm -rf "$RESULT"; mkdir -p "$RESULT"
echo "=== STA: yosys 精简综合 (abc 阶段可能需要 5~20 分钟) ==="
cd "$STA_DIR"
echo "tcl scripts/yosys_min.tcl top nangate45 \"$(echo "$STA_DIR"/example/*.v)\" $RESULT/top.netlist.v" \
  | yosys -g -l "$RESULT/yosys.log" -s - > "$RESULT/yosys-stdout.log" 2>&1
[ -f "$RESULT/top.netlist.v" ] || die "综合失败, 见 $RESULT/yosys.log"

# ---------- 6) iSTA 时序分析 ----------
echo "=== STA: iSTA 时序分析 (检测到时序报告后自动结束 iEDA, 防卡死) ==="
CLK_FREQ_MHZ=100 CLK_PORT_NAME=clock "$STA_DIR/bin/iEDA" -script "$STA_DIR/scripts/sta.tcl" \
  "$STA_DIR/scripts/default.sdc" "$RESULT/top.netlist.v" top nangate45 \
  > "$RESULT/sta.log" 2>&1 &
IEDA_PID=$!

# 监视 sta.log: 时序汇总表出现(含 "| core_clock")后杀掉 iEDA, 避免 report_power 阶段卡住
FOUND=0
while kill -0 "$IEDA_PID" 2>/dev/null; do
  if grep -q '| core_clock' "$RESULT/sta.log" 2>/dev/null; then
    FOUND=1
    break
  fi
  sleep 2
done
if [ "$FOUND" -eq 1 ]; then
  sleep 2
  echo "=== STA: 检测到时序报告, 结束 iEDA (pid $IEDA_PID) ==="
  kill -TERM "$IEDA_PID" 2>/dev/null || true
  sleep 3
  kill -KILL "$IEDA_PID" 2>/dev/null || true
fi
wait "$IEDA_PID" 2>/dev/null || true

# ---------- 7) 提取结果 ----------
# 时序汇总表列: | Endpoint | ClockGroup | DelayType | PathDelay | Required | CPPR | Slack | Freq(MHz) |
# 按竖线分列后: $5=PathDelay, $8=Slack, $9=Freq
FMAX=$(grep -E '\| core_clock' "$RESULT/sta.log" | grep ' max ' | head -1 | awk -F'|' '{gsub(/ /,"",$9); print $9}')
PATHDELAY=$(grep -E '\| core_clock' "$RESULT/sta.log" | grep ' max ' | head -1 | awk -F'|' '{gsub(/ /,"",$5); print $5}')
DFFCOUNT=$(grep 'DFF_X1' "$RESULT/synth_stat.txt" | awk '{print $1}')
CELLCOUNT=$(grep -E '^\s+[0-9]+\s+[0-9.eE+]+\s+cells$' "$RESULT/synth_stat.txt" | awk '{print $1}')
CHIPAREA=$(grep 'Chip area' "$RESULT/synth_stat.txt" | awk '{print $NF}')

echo "$FMAX" > "$RESULT/fmax.txt"
echo ""
echo "==================== STA 完成: $CORE ===================="
echo "  fmax        = ${FMAX:-未提取到} MHz"
echo "  关键路径延迟 = ${PATHDELAY:-?} ns"
echo "  触发器数量   = ${DFFCOUNT:-?}"
echo "  总单元数     = ${CELLCOUNT:-?}"
echo "  芯片面积     = ${CHIPAREA:-?} um2"
echo "  结果目录     = $RESULT"
echo "=========================================================="

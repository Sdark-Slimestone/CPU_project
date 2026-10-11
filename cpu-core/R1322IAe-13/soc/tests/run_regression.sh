#!/bin/bash
# 回归: 在 ysyxSoC 仿真器上跑全部 cpu-tests + mem-test + hello, 检查 HIT GOOD TRAP。
AM=/home/sdark/ysyx-workbench/abstract-machine
SIM=/home/sdark/cpu_project/cpu-core/R1322IAe-13/soc/obj_dir/sim_soc
CT=/home/sdark/ysyx-workbench/am-kernels/tests/cpu-tests
export AM_HOME=$AM

run_one() {  # $1=label $2=bin
  local out
  out=$(timeout 120 "$SIM" "$2" 2>/dev/null)
  if echo "$out" | grep -q "HIT GOOD TRAP"; then echo "[$1] PASS"; return 0
  else echo "[$1] FAIL"; echo "$out" | tail -2; return 1; fi
}

cd "$CT" || exit 1
pass=0; fail=0
for c in tests/*.c; do
  t=$(basename "$c" .c)
  printf 'NAME = %s\nSRCS = %s\nCFLAGS += -I %s/include\ninclude $(AM_HOME)/Makefile\n' \
    "$t" "tests/$t.c" "$CT" > Makefile.$t
  if ! make -s -f Makefile.$t ARCH=riscv32e-ysyxsoc image >/tmp/opencode/ct_$t.build 2>&1; then
    echo "[$t] BUILD FAIL"; fail=$((fail+1)); rm -f Makefile.$t; continue
  fi
  if run_one "$t" "build/$t-riscv32e-ysyxsoc.bin"; then pass=$((pass+1)); else fail=$((fail+1)); fi
  rm -f Makefile.$t
done
echo "=== cpu-tests: $pass pass, $fail fail ==="

for d in /home/sdark/ysyx-workbench/am-kernels/tests/mem-test \
         /home/sdark/ysyx-workbench/am-kernels/tests/str-test \
         /home/sdark/ysyx-workbench/am-kernels/tests/flash-test \
         /home/sdark/ysyx-workbench/am-kernels/tests/flash-read-test \
         /home/sdark/ysyx-workbench/am-kernels/tests/bitrev-test \
         /home/sdark/ysyx-workbench/am-kernels/kernels/hello; do
  n=$(basename "$d")
  ( cd "$d" && make -s ARCH=riscv32e-ysyxsoc image >/tmp/opencode/rg_$n.build 2>&1 )
  if run_one "$n" "$d/build/$n-riscv32e-ysyxsoc.bin"; then pass=$((pass+1)); else fail=$((fail+1)); fi
done
echo "=== TOTAL: $pass pass, $fail fail ==="

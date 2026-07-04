#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define __EXPORT __attribute__((visibility("default")))

// 导出 difftest 接口给 NPC 调用

void __EXPORT difftest_init() {
  cpu_reset();
}

void __EXPORT difftest_memcpy(uint32_t addr, void *buf, size_t n, bool direction) {
  // direction: DIFFTEST_TO_REF (1) = 从 DUT 复制到 REF
  if (direction == 1 && buf != NULL) {
    uint32_t offset = addr - 0x80000000;
    if (offset + n <= MEM_SIZE) {
      memcpy(&mem[offset], buf, n);
    }
  }
}

void __EXPORT difftest_regcpy(void *dut, bool direction) {
  uint32_t (*regs)[17] = (uint32_t (*)[17])dut;
  if (direction == 0) {
    // DIFFTEST_TO_DUT: copy FROM ref TO dut - 写入外部数组
    for (int i = 0; i < 16; i++) (*regs)[i] = cpu.gpr[i];
    (*regs)[16] = cpu.pc;
  } else {
    // DIFFTEST_TO_REF: copy FROM dut TO ref - 从外部读入 emu
    for (int i = 0; i < 16; i++) cpu.gpr[i] = (*regs)[i];
    cpu.pc = (*regs)[16];
  }
}

void __EXPORT difftest_exec(uint64_t n) {
  // 执行 n 条指令
  for (uint64_t i = 0; i < n; i++) {
    if (isa_exec_once() != 0) break;
  }
}

// 读取全部 CSR 寄存器：csrs[0]=mstatus, [1]=mtvec, [2]=mepc, [3]=mcause
void __EXPORT difftest_csr_rd_all(uint32_t *csrs) {
  csrs[0] = cpu.csr.mstatus;
  csrs[1] = cpu.csr.mtvec;
  csrs[2] = cpu.csr.mepc;
  csrs[3] = cpu.csr.mcause;
}

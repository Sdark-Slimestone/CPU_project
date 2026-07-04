#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t mem[MEM_SIZE];
CPU_State cpu;

void cpu_reset(void) {
  memset(mem, 0, MEM_SIZE);
  memset(&cpu, 0, sizeof(cpu));
  cpu.csr.mstatus = 0x1800;  // NPC 初始值：MPP=11
}

uint32_t csr_read(uint32_t addr) {
  switch (addr) {
    case CSR_MSTATUS: return cpu.csr.mstatus;
    case CSR_MTVEC:   return cpu.csr.mtvec;
    case CSR_MEPC:    return cpu.csr.mepc;
    case CSR_MCAUSE:  return cpu.csr.mcause;
    default: return 0;
  }
}

void csr_write(uint32_t addr, uint32_t val) {
  switch (addr) {
    case CSR_MSTATUS: cpu.csr.mstatus = val; break;
    case CSR_MTVEC:   cpu.csr.mtvec   = val; break;
    case CSR_MEPC:    cpu.csr.mepc    = val; break;
    case CSR_MCAUSE:  cpu.csr.mcause  = val; break;
    default: break;
  }
}

#define MEM_BASE 0x80000000

int load_program(const char *filename, uint32_t base_addr, const char *mainargs) {
  FILE *f = fopen(filename, "rb");
  if (!f) {
    printf("[ERROR] Cannot open %s\n", filename);
    return -1;
  }
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);
  uint32_t offset = base_addr - MEM_BASE;
  if (offset + size > MEM_SIZE) {
    printf("[ERROR] Program too large (%ld bytes, max %lu)\n", size, (unsigned long)(MEM_SIZE - offset));
    fclose(f);
    return -1;
  }
  size_t n = fread(&mem[offset], 1, size, f);
  if (n != (size_t)size) {
    printf("[ERROR] Failed to read program file\n");
    fclose(f);
    return -1;
  }
  fclose(f);
  printf("[INFO] Loaded %ld bytes to 0x%08x\n", size, base_addr);

  // mainargs placeholder replacement
  if (mainargs != NULL && mainargs[0] != '\0') {
    const char *ph = "the_insert-arg_rule_in_Makefile_will_insert_mainargs_here";
    int ph_len = 57;
    char buf[58];
    memset(buf, 0, sizeof(buf));
    strncpy(buf, mainargs, 57);
    buf[57] = '\0';
    int replaced = 0;
    for (long i = 0; i <= (long)(size - ph_len); i++) {
      if (memcmp(&mem[offset + i], ph, ph_len) == 0) {
        memcpy(&mem[offset + i], buf, ph_len);
        printf("[INFO] Replaced mainargs placeholder with \"%s\"\n", mainargs);
        replaced = 1;
        break;
      }
    }
    if (!replaced) {
      printf("[INFO] mainargs placeholder not found\n");
    }
  }
  return 0;
}

int32_t diff_get_regs(uint32_t *regs) {
  // 写入所有 GPR + PC + CSR 到 regs 数组，用于 diff 对比
  for (int i = 0; i < NR_REG; i++) regs[i] = cpu.gpr[i];
  regs[16] = cpu.pc;
  regs[17] = cpu.csr.mstatus;
  regs[18] = cpu.csr.mtvec;
  regs[19] = cpu.csr.mepc;
  regs[20] = cpu.csr.mcause;
  return 0;
}

// 内存访问接口（地址从 MEM_BASE 开始映射）
uint32_t mem_read(uint32_t addr, int len) {
  uint32_t offset = addr - MEM_BASE;
  if (addr < MEM_BASE || offset + len > MEM_SIZE) return 0;
  uint32_t val = 0;
  for (int i = 0; i < len; i++) {
    val |= ((uint32_t)mem[offset + i]) << (i * 8);
  }
  return val;
}

void mem_write(uint32_t addr, int len, uint32_t data) {
  uint32_t offset = addr - MEM_BASE;
  if (addr < MEM_BASE || offset + len > MEM_SIZE) return;
  for (int i = 0; i < len; i++) {
    mem[offset + i] = (data >> (i * 8)) & 0xFF;
  }
}

#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define NR_REG 16        // RV32E: 16 GPRs (x0-x15)
#define MEM_SIZE (128 * 1024 * 1024)  // 128MB

// CSR 寄存器地址
#define CSR_MSTATUS 0x300
#define CSR_MTVEC   0x305
#define CSR_MEPC    0x341
#define CSR_MCAUSE  0x342

// CSR 寄存器值
typedef struct {
  uint32_t mstatus;
  uint32_t mtvec;
  uint32_t mepc;
  uint32_t mcause;
} CSR;

// CPU 状态
typedef struct {
  uint32_t gpr[NR_REG];  // 通用寄存器 (x0-x15)
  uint32_t pc;            // 程序计数器
  CSR csr;                // CSR 寄存器
  uint32_t inst;          // 当前指令
} CPU_State;

// 内存
extern uint8_t mem[MEM_SIZE];
extern CPU_State cpu;

// 初始化
void cpu_reset(void);
int load_program(const char *filename, uint32_t base_addr, const char *mainargs);
int isa_exec_once(void);

// 读取/写入 GPR
static inline uint32_t gpr(int idx) {
  return cpu.gpr[idx];
}
static inline void gpr_write(int idx, uint32_t val) {
  if (idx != 0) cpu.gpr[idx] = val;
}

// CSR 读写
uint32_t csr_read(uint32_t addr);
void csr_write(uint32_t addr, uint32_t val);

// Diff-test 接口（给 NPC diff 功能调用）
int32_t diff_get_regs(uint32_t *regs);

#endif
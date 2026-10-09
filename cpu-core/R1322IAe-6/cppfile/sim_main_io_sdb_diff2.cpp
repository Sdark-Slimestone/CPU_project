#include <verilated.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dlfcn.h>
#include "Vtop.h"

#define panic(...) do { fprintf(stderr, __VA_ARGS__); exit(1); } while(0)
#define MEM_BASE 0x80000000
#define MEM_SIZE (128 * 1024 * 1024)

static unsigned char mem[MEM_SIZE];
static int simulation_finished = 0;
static int good_trap = 0;
static unsigned long long cycle = 0;
static int ebreak_triggered = 0;
static int drain_cycles = 0;
static Vtop *top = nullptr;

// 声明 difftest 接口
typedef void (*difftest_init_t)();
typedef void (*difftest_memcpy_t)(unsigned int, void *, size_t, int);
typedef void (*difftest_regcpy_t)(void *, int);
typedef void (*difftest_exec_t)(unsigned long long);
typedef void (*difftest_csr_rd_all_t)(uint32_t *);

static difftest_init_t       ref_init = NULL;
static difftest_memcpy_t     ref_memcpy = NULL;
static difftest_regcpy_t     ref_regcpy = NULL;
static difftest_exec_t       ref_exec = NULL;
static difftest_csr_rd_all_t ref_csr_rd_all = NULL;

#define DIFFTEST_TO_REF 1
#define DIFFTEST_TO_DUT 0

// 装载程序到内存
void load_program(const char *filename, unsigned int base) {
    FILE *f = fopen(filename, "rb");
    if (!f) panic("Cannot open %s\n", filename);
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
    unsigned int off = base - MEM_BASE;
    if (off + sz > MEM_SIZE) panic("Program too large\n");
    if (fread(&mem[off], 1, sz, f) != (size_t)sz) panic("Failed to read\n");
    fclose(f); printf("[INFO] Loaded %ld bytes\n", sz);
}

// DPI-C 内存读
extern "C" unsigned int pmem_read(unsigned int addr) {
    if (addr < MEM_BASE) return 0;
    unsigned int base = addr & ~3, off = base - MEM_BASE;
    if (off + 3 >= MEM_SIZE) { simulation_finished = 1; good_trap = 0; return 0; }
    return (mem[off+3]<<24)|(mem[off+2]<<16)|(mem[off+1]<<8)|mem[off];
}

// DPI-C 内存写
extern "C" void pmem_write(unsigned int addr, unsigned int data, unsigned char mask) {
    if (addr == 0xa00003f8) { if (mask & 0x1) putchar(data&0xFF); fflush(stdout); return; }
    if (addr < MEM_BASE) return;
    unsigned int base = addr & ~3, off = base - MEM_BASE;
    if (off + 3 >= MEM_SIZE) { simulation_finished = 1; good_trap = 0; return; }
    for (int i = 0; i < 4; i++) if (mask & (1 << i)) mem[off + i] = (data >> (i * 8)) & 0xFF;
}

// ebreak 回调
extern "C" void sim_finish(void) {
    if (!ebreak_triggered) { ebreak_triggered = 1; drain_cycles = 20; }
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    memset(mem, 0, MEM_SIZE);
    if (argc < 2) return printf("Usage: %s <program.bin>\n", argv[0]), 1;
    load_program(argv[1], MEM_BASE);

    // 加载 emu .so
    void *so = dlopen("/home/sdark/cpu_project/emu-so/rv32e/rv32e.so", RTLD_LAZY | RTLD_LOCAL);
    if (!so) panic("[ERROR] dlopen: %s\n", dlerror());
    ref_init       = (difftest_init_t)dlsym(so, "difftest_init");
    ref_memcpy     = (difftest_memcpy_t)dlsym(so, "difftest_memcpy");
    ref_regcpy     = (difftest_regcpy_t)dlsym(so, "difftest_regcpy");
    ref_exec       = (difftest_exec_t)dlsym(so, "difftest_exec");
    ref_csr_rd_all = (difftest_csr_rd_all_t)dlsym(so, "difftest_csr_rd_all");
    if (!ref_init || !ref_exec) panic("[ERROR] dlsym\n");
    printf("[INFO] Loaded emu.so\n");

    // 复位 NPC 并初始化 emu
    top = new Vtop;
    top->reset = 1; top->clock = 0; top->eval();
    top->clock = 1; top->eval();
    top->reset = 0; top->clock = 0; top->eval();

    uint32_t init_regs[17] = {0}; init_regs[16] = MEM_BASE;
    ref_init();
    ref_memcpy(MEM_BASE, mem, MEM_SIZE, DIFFTEST_TO_REF);
    ref_regcpy(init_regs, DIFFTEST_TO_REF);
    printf("[INFO] Start simulation\n");

    // 主循环：每个时钟周期 NPC 执行后对比 NEMU
    while (!simulation_finished && !Verilated::gotFinish()) {
        int retired = top->io_debug_stall ? 1 : 2;

        top->clock = 0; top->eval();
        top->clock = 1; top->eval();
        cycle++;

        if (ebreak_triggered) {
            if (drain_cycles > 0) drain_cycles--;
            else { simulation_finished = 1; good_trap = 1; }
        }

        // 双发射：根据 stall 信号决定 NEMU 执行 1 或 2 条
        if (!simulation_finished)
            for (int i = 0; i < retired; i++) ref_exec(1);

        // 对比寄存器状态
        if (!ebreak_triggered && !simulation_finished) {
            uint32_t nemu[17]; ref_regcpy(nemu, DIFFTEST_TO_DUT);
            uint32_t npc[16] = {
                top->io_debug_grf_regs_0,  top->io_debug_grf_regs_1,
                top->io_debug_grf_regs_2,  top->io_debug_grf_regs_3,
                top->io_debug_grf_regs_4,  top->io_debug_grf_regs_5,
                top->io_debug_grf_regs_6,  top->io_debug_grf_regs_7,
                top->io_debug_grf_regs_8,  top->io_debug_grf_regs_9,
                top->io_debug_grf_regs_10, top->io_debug_grf_regs_11,
                top->io_debug_grf_regs_12, top->io_debug_grf_regs_13,
                top->io_debug_grf_regs_14, top->io_debug_grf_regs_15,
            };
            int diff = 0;
            for (int i = 0; i < 16; i++) {
                if (npc[i] != nemu[i]) {
                    if (!diff) printf("\n[DIFF] Mismatch at cycle %llu\n", cycle);
                    printf("  Reg[%d]: NPC=0x%08x, NEMU=0x%08x\n", i, npc[i], nemu[i]);
                    diff = 1;
                }
            }
            if (!diff && ref_csr_rd_all) {
                uint32_t ec[4]; ref_csr_rd_all(ec);
                struct {const char *n; uint32_t npc, emu;} cc[] = {
                    {"mstatus", top->io_debug_mstatus, ec[0]},
                    {"mtvec",   top->io_debug_mtvec,   ec[1]},
                    {"mepc",    top->io_debug_mepc,    ec[2]},
                    {"mcause",  top->io_debug_mcause,  ec[3]},
                };
                for (int i = 0; i < 4; i++) {
                    if (cc[i].npc != cc[i].emu) {
                        if (!diff) printf("\n[DIFF] Mismatch at cycle %llu\n", cycle);
                        printf("  CSR %s: NPC=0x%08x, NEMU=0x%08x\n", cc[i].n, cc[i].npc, cc[i].emu);
                        diff = 1;
                    }
                }
            }
            if (diff) { simulation_finished = 1; good_trap = 0; }
        }
    }

    printf("[INFO] Simulation finished after %llu cycles\n", cycle);
    if (good_trap) printf("HIT GOOD TRAP\n"); else printf("HIT BAD TRAP\n");
    if (so) dlclose(so); delete top;
    return good_trap ? 0 : 1;
}
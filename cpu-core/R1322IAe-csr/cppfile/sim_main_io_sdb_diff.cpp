#include <verilated.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include "Vtop.h"
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "/home/sdark/ysyx-workbench/nemu/include/difftest-def.h"

#define panic(...) do { fprintf(stderr, __VA_ARGS__); exit(1); } while(0)
typedef uint32_t word_t;
#define MEM_BASE 0x80000000
#define MEM_SIZE (32 * 1024 * 1024)

static unsigned char mem[MEM_SIZE];
static int simulation_finished = 0;
static int good_trap = 0;
static int user_quit = 0;

#define MAX_WRITES 64
static uint32_t written_addrs[MAX_WRITES];
static int written_count = 0;

extern "C" {
    void difftest_init();
    void difftest_memcpy(unsigned int addr, void *buf, size_t n, int direction);
    void difftest_regcpy(void *dut, int direction);
    void difftest_exec(unsigned long long n);
}

#define DIFFTEST_TO_REF   1
#define DIFFTEST_TO_DUT   0

static auto boot_time = std::chrono::steady_clock::now();
static Vtop *top = nullptr;
static unsigned long long cycle = 0;

static uint64_t get_uptime_us() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now - boot_time).count();
}

#ifdef __cplusplus
extern "C" {
#endif

unsigned int pmem_read(unsigned int addr) {
    if (addr == 0xa0000048) {
        uint64_t us = get_uptime_us();
        return (unsigned int)us;
    }
    if (addr == 0xa000004c) {
        uint64_t us = get_uptime_us();
        return (unsigned int)(us >> 32);
    }
    if (addr < MEM_BASE) return 0;
    unsigned int base = addr & ~3;
    unsigned int offset = base - MEM_BASE;
    if (offset + 3 >= MEM_SIZE) {
        simulation_finished = 1;
        good_trap = 0;
        return 0;
    }
    return (mem[offset+3] << 24) |
           (mem[offset+2] << 16) |
           (mem[offset+1] << 8)  |
           mem[offset];
}

void pmem_write(unsigned int addr, unsigned int data, unsigned char mask) {
    if (addr == 0x10000000) {
        if (mask & 0x1) {
            putchar((char)(data & 0xFF));
            fflush(stdout);
        }
        return;
    }
    if (addr < MEM_BASE) return;
    unsigned int base = addr & ~3;
    unsigned int offset = base - MEM_BASE;
    if (offset + 3 >= MEM_SIZE) {
        simulation_finished = 1;
        good_trap = 0;
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            mem[offset + i] = (data >> (i * 8)) & 0xFF;
        }
    }
    // 记录写入地址用于内存一致性检查
    if (written_count < MAX_WRITES) {
        written_addrs[written_count++] = addr;
    }
}

void sim_finish(void) {
    simulation_finished = 1;
    good_trap = 1;
}

#ifdef __cplusplus
}
#endif

void load_program(const char *filename, unsigned int base_addr) {
    FILE *f = fopen(filename, "rb");
    if (!f) { printf("[ERROR] Cannot open %s\n", filename); exit(1); }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned int offset = base_addr - MEM_BASE;
    if (offset + size > MEM_SIZE) {
        printf("[ERROR] Program too large\n"); exit(1);
    }
    size_t n = fread(&mem[offset], 1, size, f);
    if (n != (size_t)size) { printf("[ERROR] Failed to read\n"); exit(1); }
    fclose(f);
    printf("[INFO] Loaded %ld bytes to 0x%08x\n", size, base_addr);
}

static void print_debug_info() {
    printf("--- NPC Debug Info ---\n");
    printf("Cycle: %llu\n", cycle);
    printf("Inst1 PC: 0x%08x, Inst1: 0x%08x\n", top->io_debug_inst1_pc, top->io_debug_inst1);
    printf("Inst2 PC: 0x%08x, Inst2: 0x%08x\n", top->io_debug_inst2_pc, top->io_debug_inst2);
    printf("Stall: %d\n", top->io_debug_stall);
    printf("EXU1 alu_out=0x%08x agu_out=0x%08x\n",
           top->io_debug_exu1_alu_out, top->io_debug_exu1_agu_out);
    printf("EXU2 alu_out=0x%08x agu_out=0x%08x\n",
           top->io_debug_exu2_alu_out, top->io_debug_exu2_agu_out);
    printf("LSU1 load=%d store=%d addr=0x%08x\n",
           top->io_debug_lsu1_is_load, top->io_debug_lsu1_is_store,
           top->io_debug_lsu1_addr);
    printf("LSU2 load=%d store=%d addr=0x%08x\n",
           top->io_debug_lsu2_is_load, top->io_debug_lsu2_is_store,
           top->io_debug_lsu2_addr);
    printf("WBU valid1=%d valid2=%d conflict=%d\n",
           top->io_debug_wbu_valid1, top->io_debug_wbu_valid2,
           top->io_debug_wbu_conflict);
    printf("Regs: ");
    uint32_t debug_regs[16] = {
        top->io_debug_grf_regs_0,  top->io_debug_grf_regs_1,
        top->io_debug_grf_regs_2,  top->io_debug_grf_regs_3,
        top->io_debug_grf_regs_4,  top->io_debug_grf_regs_5,
        top->io_debug_grf_regs_6,  top->io_debug_grf_regs_7,
        top->io_debug_grf_regs_8,  top->io_debug_grf_regs_9,
        top->io_debug_grf_regs_10, top->io_debug_grf_regs_11,
        top->io_debug_grf_regs_12, top->io_debug_grf_regs_13,
        top->io_debug_grf_regs_14, top->io_debug_grf_regs_15
    };
    for (int i = 0; i < 16; i++) {
        printf("x%d=0x%08x ", i, debug_regs[i]);
    }
    printf("\n");
    printf("CSR: mcycle=%llu minstret=%llu mstatus=0x%08x mcause=0x%08x mepc=0x%08x\n",
           top->io_debug_mcycle, top->io_debug_minstret,
           top->io_debug_mstatus, top->io_debug_mcause, top->io_debug_mepc);
    printf("----------------------\n");
}

// Dual-issue: 每周期最多执行2条指令
// 读寄存器顺序：
//   1. NPC时钟上升沿（regs := nextRegs）-> regs更新
//   2. C++读取io_debug_grf_regs（此时regs已包含刚执行完的结果）
//   3. difftest_exec让NEMU执行同样数量的指令
//   4. 对比
static int exec_one_cycle() {
    if (simulation_finished || user_quit) return 0;

    // 执行NPC一个周期（时钟上升沿时GRF写入新值）
    top->clock = 0; top->eval();
    top->clock = 1; top->eval();
    cycle++;

    // 读取NPC刚更新后的寄存器值
    uint32_t npc_regs[16];
    npc_regs[0]  = top->io_debug_grf_regs_0;
    npc_regs[1]  = top->io_debug_grf_regs_1;
    npc_regs[2]  = top->io_debug_grf_regs_2;
    npc_regs[3]  = top->io_debug_grf_regs_3;
    npc_regs[4]  = top->io_debug_grf_regs_4;
    npc_regs[5]  = top->io_debug_grf_regs_5;
    npc_regs[6]  = top->io_debug_grf_regs_6;
    npc_regs[7]  = top->io_debug_grf_regs_7;
    npc_regs[8]  = top->io_debug_grf_regs_8;
    npc_regs[9]  = top->io_debug_grf_regs_9;
    npc_regs[10] = top->io_debug_grf_regs_10;
    npc_regs[11] = top->io_debug_grf_regs_11;
    npc_regs[12] = top->io_debug_grf_regs_12;
    npc_regs[13] = top->io_debug_grf_regs_13;
    npc_regs[14] = top->io_debug_grf_regs_14;
    npc_regs[15] = top->io_debug_grf_regs_15;

    // 检查stall: stall=true仅发1条，否则发2条
    int inst_count = top->io_debug_stall ? 1 : 2;

    // 通知NEMU执行对应数量的指令
    difftest_exec(inst_count);

    // 获取NEMU执行后的寄存器状态
    uint32_t nemu_state[17];
    difftest_regcpy(nemu_state, DIFFTEST_TO_DUT);

    // 对比：NPC刚执行完的结果 vs NEMU执行对应数量的结果
    for (int i = 0; i < 16; i++) {
        if (npc_regs[i] != nemu_state[i]) {
            printf("\n[DIFFTEST] Register mismatch at cycle %llu\n", cycle);
            printf("Reg[%d]: NPC = 0x%08x, NEMU = 0x%08x\n", i, npc_regs[i], nemu_state[i]);
            print_debug_info();
        }
    }

    // 内存对比（本周期写入过的地址）
    for (int i = 0; i < written_count; i++) {
        uint32_t addr = written_addrs[i];
        uint32_t off = addr - MEM_BASE;
        if (off < MEM_SIZE) {
            uint8_t npc_val = mem[off];
            uint8_t nemu_val = 0;
            difftest_memcpy(addr, &nemu_val, 1, DIFFTEST_TO_DUT);
            if (npc_val != nemu_val) {
                printf("\n[MEM MISMATCH] cycle %llu, addr 0x%08x, NPC 0x%02x, NEMU 0x%02x\n",
                       cycle, addr, npc_val, nemu_val);
                print_debug_info();
            }
        }
    }
    written_count = 0;

    return 0;
}

static void exec_continue() {
    printf("Continuing execution...\n");
    while (!simulation_finished && !Verilated::gotFinish() && !user_quit) {
        if (exec_one_cycle()) break;
    }
    if (simulation_finished) {
        printf("[INFO] Simulation finished after %llu cycles\n", cycle);
        printf(good_trap ? "HIT GOOD TRAP\n" : "HIT BAD TRAP\n");
    }
}

// ============== 简化的交互式调试（支持 si 单步命令） ==============
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    memset(mem, 0, MEM_SIZE);
    if (argc < 2) {
        printf("Usage: %s <program.bin>\n", argv[0]);
        return 1;
    }
    load_program(argv[1], MEM_BASE);

    difftest_init();
    difftest_memcpy(MEM_BASE, mem, MEM_SIZE, DIFFTEST_TO_REF);
    unsigned int init_regs[17] = {0};
    init_regs[16] = MEM_BASE;
    difftest_regcpy(init_regs, DIFFTEST_TO_REF);

    top = new Vtop;
    // 复位
    top->reset = 1;
    top->clock = 0; top->eval();
    top->clock = 1; top->eval();
    top->reset = 0;
    top->clock = 0; top->eval();
    // 复位后NEMU同步
    difftest_exec(1);

    printf("[INFO] Reset complete. Running...\n");

    // 直接跑完
    exec_continue();

    delete top;
    return 0;
}
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

    printf("[INFO] Reset complete. Running...\n");

    // 主循环：让NEMU先执行，然后NPC执行，然后对比
    while (!simulation_finished && !Verilated::gotFinish() && !user_quit) {
        // 先让NEMU执行（NEMU的difftest_exec是从当前PC执行）
        int inst_count = top->io_debug_stall ? 1 : 2;
        for (int i = 0; i < inst_count; i++) {
            difftest_exec(1);
        }

        // NPC 执行一个周期
        top->clock = 0; top->eval();
        top->clock = 1; top->eval();
        cycle++;

        // 读取NPC寄存器（时钟上升沿后已更新）
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

        // 读取NEMU寄存器并对比
        uint32_t nemu_state[17];
        difftest_regcpy(nemu_state, DIFFTEST_TO_DUT);

        for (int i = 0; i < 16; i++) {
            if (npc_regs[i] != nemu_state[i]) {
                printf("[DIFFTEST] MISMATCH cycle=%llu reg[%d]: NPC=0x%08x NEMU=0x%08x\n",
                       cycle, i, npc_regs[i], nemu_state[i]);
            }
        }

        // 对比内存
        for (int i = 0; i < written_count; i++) {
            uint32_t addr = written_addrs[i];
            uint32_t off = addr - MEM_BASE;
            if (off < MEM_SIZE) {
                uint8_t npc_val = mem[off];
                uint8_t nemu_val = 0;
                difftest_memcpy(addr, &nemu_val, 1, DIFFTEST_TO_DUT);
                if (npc_val != nemu_val) {
                    printf("[MEM MISMATCH] cycle=%llu addr=0x%08x NPC=0x%02x NEMU=0x%02x\n",
                           cycle, addr, npc_val, nemu_val);
                }
            }
        }
        written_count = 0;
    }

    if (simulation_finished) {
        printf("[INFO] Simulation finished after %llu cycles\n", cycle);
        printf(good_trap ? "HIT GOOD TRAP\n" : "HIT BAD TRAP\n");
    }

    delete top;
    return 0;
}
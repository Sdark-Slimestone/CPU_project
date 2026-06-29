#include <verilated.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include "Vtop.h"

#define MEM_BASE 0x80000000
#define MEM_SIZE (128 * 1024 * 1024)

static unsigned char mem[MEM_SIZE];
static int simulation_finished = 0;
static int good_trap = 0;

static auto boot_time = std::chrono::steady_clock::now();

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
        printf("[ERROR] Out of bounds read at addr=0x%08x\n", addr);
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
        printf("[ERROR] Out of bounds write at addr=0x%08x\n", addr);
        simulation_finished = 1;
        good_trap = 0;
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            mem[offset + i] = (data >> (i * 8)) & 0xFF;
        }
    }
}

static int ebreak_triggered = 0;
static int drain_cycles = 0;

void sim_finish(void) {
    if (!ebreak_triggered) {
        ebreak_triggered = 1;
        drain_cycles = 20;
    }
}

#ifdef __cplusplus
}
#endif

void load_program(const char *filename, unsigned int base_addr) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("[ERROR] Cannot open %s\n", filename);
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned int offset = base_addr - MEM_BASE;
    if (offset + size > MEM_SIZE) {
        printf("[ERROR] Program too large\n");
        fclose(f);
        exit(1);
    }
    size_t n = fread(&mem[offset], 1, size, f);
    if (n != (size_t)size) {
        printf("[ERROR] Failed to read\n");
        fclose(f);
        exit(1);
    }
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

    Vtop *top = new Vtop;

    // 复位序列
    top->reset = 1;
    top->clock = 0; top->eval();
    top->clock = 1; top->eval();
    top->reset = 0;
    top->clock = 0; top->eval();

    unsigned long long cycle = 0;
    while (!simulation_finished && !Verilated::gotFinish()) {
        top->clock = 0; top->eval();
        top->clock = 1; top->eval();
        cycle++;
        if (ebreak_triggered) {
            if (drain_cycles > 0) {
                drain_cycles--;
            } else {
                simulation_finished = 1;
                good_trap = 1;
            }
        }
    }

    printf("[INFO] Simulation finished after %llu cycles\n", cycle);
    if (good_trap)
        printf("HIT GOOD TRAP\n");
    else
        printf("HIT BAD TRAP\n");

    delete top;
    return 0;
}
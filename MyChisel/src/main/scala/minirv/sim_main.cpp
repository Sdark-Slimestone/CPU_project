#include <verilated.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vtop.h"

#define MEM_BASE 0x80000000
#define MEM_SIZE (32 * 1024 * 1024)  // 32MB

static unsigned char mem[MEM_SIZE];
static int simulation_finished = 0;
static int good_trap = 0;   // 1: 正常ebreak结束, 0: 异常结束

#ifdef __cplusplus
extern "C" {
#endif

unsigned int pmem_read(unsigned int addr) {
    // 可选：打印前几次读地址（调试用）
    static int read_cnt = 0;
    if (read_cnt < 114514) {
        printf("[READ] addr=0x%08x\n", addr);
        read_cnt++;
    }
    if (addr < MEM_BASE) {
        return 0;
    }
    unsigned int offset = addr - MEM_BASE;
    unsigned int aligned = offset & 0xFFFFFFFC;
    if (aligned + 3 >= MEM_SIZE) {
        printf("\n[ERROR] pmem_read: address 0x%08x out of bounds!\n", addr);
        simulation_finished = 1;
        good_trap = 0;          // 异常结束
        return 0;
    }
    unsigned int word = (mem[aligned + 3] << 24) |
                        (mem[aligned + 2] << 16) |
                        (mem[aligned + 1] << 8)  |
                        mem[aligned];
    return word;
}

void pmem_write(unsigned int addr, unsigned int data, unsigned char mask) {
    if (addr < MEM_BASE) {
        return;
    }
    unsigned int offset = addr - MEM_BASE;
    if (mask & 0xF0) {
        printf("\n[WARNING] pmem_write: mask 0x%02x has invalid high bits, ignored\n", mask);
        mask &= 0x0F;
    }
    if (offset + 3 >= MEM_SIZE) {
        printf("\n[ERROR] pmem_write: address 0x%08x out of bounds!\n", addr);
        simulation_finished = 1;
        good_trap = 0;          // 异常结束
        return;
    }
    if (mask & 0x01) mem[offset]     = (data >> 0) & 0xFF;
    if (mask & 0x02) mem[offset + 1] = (data >> 8) & 0xFF;
    if (mask & 0x04) mem[offset + 2] = (data >> 16) & 0xFF;
    if (mask & 0x08) mem[offset + 3] = (data >> 24) & 0xFF;
}

void sim_finish(void) {
    printf("\n[INFO] ebreak instruction executed, finishing simulation.\n");
    simulation_finished = 1;
    good_trap = 1;              // 正常结束
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
        printf("[ERROR] Program too large for memory (need %ld bytes from 0x%08x)\n", 
               size, base_addr);
        fclose(f);
        exit(1);
    }
    size_t n = fread(&mem[offset], 1, size, f);
    if (n != (size_t)size) {
        printf("[ERROR] Failed to read program file\n");
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
    top->reset = 1;
    top->clock = 0;
    top->eval();
    top->clock = 1;   // 上升沿
    top->eval();
    top->reset = 0;
    top->clock = 0;
    top->eval();

    unsigned long long cycle = 0;
    while (!simulation_finished && !Verilated::gotFinish()) {
        top->clock = 0;
        top->eval();
        top->clock = 1;
        top->eval();
        cycle++;
        if (cycle % 100000 == 0) {
            printf("[INFO] Cycle %llu ...\n", cycle);
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
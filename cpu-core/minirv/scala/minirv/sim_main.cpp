#include <verilated.h>
#include <verilated_vcd_c.h>  // 若不需要波形可删除此行
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vtop.h"

#define MEM_BASE 0x80000000
#define MEM_SIZE (32 * 1024 * 1024)  // 32MB

static unsigned char mem[MEM_SIZE];
static int simulation_finished = 0;
static int good_trap = 0;   // 1: 正常 ebreak 结束, 0: 异常结束

#ifdef __cplusplus
extern "C" {
#endif

// 读内存：逻辑完全保持不变
unsigned int pmem_read(unsigned int addr) {
    if (addr < MEM_BASE) return 0;
    unsigned int base = addr & ~3;
    unsigned int offset = base - MEM_BASE;
    if (offset + 3 >= MEM_SIZE) {
        simulation_finished = 1;
        good_trap = 0;
        return 0;
    }
    unsigned int word = (mem[offset+3] << 24) |
                        (mem[offset+2] << 16) |
                        (mem[offset+1] << 8)  |
                        mem[offset];
    return word;
}

// 写内存：逻辑完全保持不变
void pmem_write(unsigned int addr, unsigned int data, unsigned char mask) {
    if (addr < MEM_BASE) return;
    unsigned int base = addr & ~3;
    unsigned int offset = base - MEM_BASE;
    int is_byte = (mask == 0x1 || mask == 0x2 || mask == 0x4 || mask == 0x8);

    for (int i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            if (is_byte) {
                mem[offset + i] = data & 0xFF;
            } else {
                mem[offset + i] = (data >> (i * 8)) & 0xFF;
            }
        }
    }
}

// ebreak 回调，由 Verilog 模块通过 DPI-C 调用
void sim_finish(void) {
    simulation_finished = 1;
    good_trap = 1;
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

    // 复位序列
    top->reset = 1;
    top->clock = 0;
    top->eval();
    top->clock = 1;
    top->eval();
    top->reset = 0;
    top->clock = 0;
    top->eval();

    unsigned long long cycle = 0;
    while (!simulation_finished && !Verilated::gotFinish()) {
        // 标准时钟周期：低 → 高
        top->clock = 0;
        top->eval();
        top->clock = 1;
        top->eval();
        cycle++;
    }

    printf("[INFO] Simulation finished after %llu cycles\n", cycle);
    if (good_trap)
        printf("HIT GOOD TRAP\n");
    else
        printf("HIT BAD TRAP\n");

    delete top;
    return 0;
}
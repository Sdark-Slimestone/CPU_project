#include <verilated.h>
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
    printf("[PMEM_READ] addr=0x%08x -> value=0x%08x\n", addr, word);
    return word;
}

void pmem_write(unsigned int addr, unsigned int data, unsigned char mask) {
    printf("[PMEM_WRITE_CALL] addr=0x%08x, data=0x%08x, mask=0x%x\n", addr, data, mask);
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

    // 打印写入后的内存值（按字对齐的32位值）
    unsigned int word_after = (mem[offset+3] << 24) |
                              (mem[offset+2] << 16) |
                              (mem[offset+1] << 8)  |
                              mem[offset];
    printf("[PMEM_WRITE] after_write: mem[0x%08x]=0x%08x\n", base, word_after);
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

// 打印所有 debug 信息（适配 Verilator 生成的 io_ 信号）
void print_debug(Vtop *top, unsigned long long cycle) {
    printf("\n========== Cycle %llu ==========\n", cycle);

    // PC
    printf("[PC] 0x%08x\n", top->io_debug_pc);

    // 指令：由于无法直接访问 ifu 实例，可选择通过内存读取（会增加一次 pmem_read 打印）
    // 若需要，取消注释下面两行（注意会多输出一条 PMEM_READ 信息）
    // uint32_t inst = pmem_read(top->io_debug_pc);
    // printf("[INST] 0x%08x\n", inst);

    // 寄存器 (16个) - Verilator 展开为独立信号
    printf("[REGS] ");
    for (int i = 0; i < 16; i++) {
        // 根据 Verilator 命名规则，debug_regs 向量展开为 io_debug_regs_0..15
        uint32_t reg_val;
        switch(i) {
            case 0:  reg_val = top->io_debug_regs_0; break;
            case 1:  reg_val = top->io_debug_regs_1; break;
            case 2:  reg_val = top->io_debug_regs_2; break;
            case 3:  reg_val = top->io_debug_regs_3; break;
            case 4:  reg_val = top->io_debug_regs_4; break;
            case 5:  reg_val = top->io_debug_regs_5; break;
            case 6:  reg_val = top->io_debug_regs_6; break;
            case 7:  reg_val = top->io_debug_regs_7; break;
            case 8:  reg_val = top->io_debug_regs_8; break;
            case 9:  reg_val = top->io_debug_regs_9; break;
            case 10: reg_val = top->io_debug_regs_10; break;
            case 11: reg_val = top->io_debug_regs_11; break;
            case 12: reg_val = top->io_debug_regs_12; break;
            case 13: reg_val = top->io_debug_regs_13; break;
            case 14: reg_val = top->io_debug_regs_14; break;
            case 15: reg_val = top->io_debug_regs_15; break;
            default: reg_val = 0;
        }
        printf("[REG x%d] = 0x%08x\n", i, reg_val);
    }
    printf("\n");

    // 控制信号
    printf("[CTRL] is_add=%d is_addi=%d is_jalr=%d is_lui=%d is_lbu=%d is_lw=%d is_sw=%d is_sb=%d is_ebreak=%d\n",
           top->io_debug_is_add, top->io_debug_is_addi, top->io_debug_is_jalr, top->io_debug_is_lui,
           top->io_debug_is_lbu, top->io_debug_is_lw, top->io_debug_is_sw, top->io_debug_is_sb,
           top->io_debug_is_ebreak);

    // ALU 信息
    printf("[ALU] src1=0x%08x src2=0x%08x result=0x%08x\n",
           top->io_debug_alu_src1, top->io_debug_alu_src2, top->io_debug_alu_res);

    // GRF 写回信息
    printf("[WB] regWen=%d wbData=0x%08x\n", top->io_debug_regWen, top->io_debug_wbData);

    // GRF 读端口信息
    printf("[GRF_RD] rden=%d rdaddr=0x%02x input=0x%08x\n",
           top->io_debug_grf_rden, top->io_debug_grf_rdaddr, top->io_debug_grf_input);

    // LSU 信息
    printf("[LSU] addr=0x%08x wen=%d wdata=0x%08x wmask=0x%x ren=%d rdata=0x%08x\n",
           top->io_debug_lsu_addr, top->io_debug_lsu_wen, top->io_debug_lsu_wdata,
           top->io_debug_lsu_wmask, top->io_debug_lsu_ren, top->io_debug_lsu_rdata);

    // 函数调用检测 (jalr)
    if (top->io_debug_is_jalr) {
        printf("[CALL] jalr detected: from PC 0x%08x -> target 0x%08x\n",
               top->io_debug_pc, top->io_debug_alu_res);
    }
}

// 等待用户输入：返回 0 表示继续，1 表示退出
int wait_for_step() {
    printf("\n--- Press ENTER to step, 'q' to quit --- ");
    fflush(stdout);
    int c = getchar();
    // 清空输入缓冲区（直到换行）
    while (getchar() != '\n');
    if (c == 'q' || c == 'Q')
        return 1; // 退出
    return 0;     // 继续
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    memset(mem, 0, MEM_SIZE);

    // 解析单步模式标志
    int single_step = 1;   // 默认开启单步
    const char *bin_file = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--auto") == 0 || strcmp(argv[i], "-a") == 0) {
            single_step = 0;
        } else if (strcmp(argv[i], "--single-step") == 0 || strcmp(argv[i], "-s") == 0) {
            single_step = 1;
        } else if (bin_file == NULL) {
            bin_file = argv[i];
        } else {
            printf("[ERROR] Unknown argument: %s\n", argv[i]);
            return 1;
        }
    }
    if (bin_file == NULL) {
        printf("Usage: %s [--auto|-a] <program.bin>\n", argv[0]);
        printf("       --auto, -a : run without single-step (auto mode)\n");
        printf("       (default is single-step mode)\n");
        return 1;
    }

    load_program(bin_file, MEM_BASE);

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

    printf("[INFO] Reset complete, starting simulation\n");
    print_debug(top, 0);  // 打印复位后初始状态

    unsigned long long cycle = 0;
    while (!simulation_finished && !Verilated::gotFinish()) {
        // 时钟低半周
        top->clock = 0;
        top->eval();

        // 时钟高半周
        top->clock = 1;
        top->eval();

        cycle++;
        print_debug(top, cycle);

        // 单步控制
        if (single_step) {
            if (wait_for_step()) {
                printf("[INFO] User requested quit.\n");
                break;
            }
        }

        // 如果 ebreak 触发，simulation_finished 会在 DPI 回调中置 1
        if (simulation_finished) break;
    }

    printf("\n[INFO] Simulation finished after %llu cycles\n", cycle);
    if (good_trap)
        printf("HIT GOOD TRAP\n");
    else
        printf("HIT BAD TRAP\n");

    // 打印最终寄存器状态
    printf("\nFinal register state:\n");
    for (int i = 0; i < 16; i++) {
        uint32_t reg_val;
        switch(i) {
            case 0:  reg_val = top->io_debug_regs_0; break;
            case 1:  reg_val = top->io_debug_regs_1; break;
            case 2:  reg_val = top->io_debug_regs_2; break;
            case 3:  reg_val = top->io_debug_regs_3; break;
            case 4:  reg_val = top->io_debug_regs_4; break;
            case 5:  reg_val = top->io_debug_regs_5; break;
            case 6:  reg_val = top->io_debug_regs_6; break;
            case 7:  reg_val = top->io_debug_regs_7; break;
            case 8:  reg_val = top->io_debug_regs_8; break;
            case 9:  reg_val = top->io_debug_regs_9; break;
            case 10: reg_val = top->io_debug_regs_10; break;
            case 11: reg_val = top->io_debug_regs_11; break;
            case 12: reg_val = top->io_debug_regs_12; break;
            case 13: reg_val = top->io_debug_regs_13; break;
            case 14: reg_val = top->io_debug_regs_14; break;
            case 15: reg_val = top->io_debug_regs_15; break;
            default: reg_val = 0;
        }
        printf("x%d = 0x%08x\n", i, reg_val);
    }

    delete top;
    return 0;
}
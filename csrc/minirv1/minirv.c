#include "lcthw/architecture.h"
#include "lcthw/isa.h"
#include "lcthw/dbg.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // 检查命令行参数数量
    if (argc < 3) {
        fprintf(stderr, "用法: %s <rom文件> <ram文件>\n", argv[0]);
        return 1;
    }

    const char *rom_file = argv[1];
    const char *ram_file = argv[2];

    if (init_rom(rom_file) != 0) {
        fprintf(stderr, "错误：无法初始化 ROM (文件: %s)\n", rom_file);
        return 1;
    }
    if (init_ram(ram_file) != 0) {
        fprintf(stderr, "错误：无法初始化 RAM (文件: %s)\n", ram_file);
        return 1;
    }

    // 确保内存指针非空
    if (ROM == NULL || RAM == NULL) {
        fprintf(stderr, "错误：内存分配失败\n");
        return 1;
    }

    GFR[PC] = 0;

    while (1) {
        int pc = GFR[PC];
        int instr = read_rom(pc / 4);
        if (instr == -1) {
            fprintf(stderr, "错误：无法读取 PC=0x%x 处的指令\n", pc);
            break;
        }
        decode(instr);
        if (simulation_exit) {
            break;
        }
    }

    if (simulation_exit) {
        printf("程序结束，退出码: %d，显示图形...\n", simulation_exit_code);
        for (int i = 0; i < 17; i++) {
        if (i == 16) {
            printf("  PC = %d (0x%x)\n", GFR[i], GFR[i]);
        } else {
            printf("  x%d = %d (0x%x)\n", i, GFR[i], GFR[i]);
        }
    }
    printf("显示图形...\n");
    vga_show_framebuffer();
}

    free(RAM);
    free(ROM);
    return 0;
}
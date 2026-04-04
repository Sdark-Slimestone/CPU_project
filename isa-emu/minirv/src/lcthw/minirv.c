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

    GFR[PC] = 0;

    while (1) {
        int pc = GFR[PC];
        int instr = read_rom(pc / 4);
        if (instr == -1) {
            fprintf(stderr, "错误：无法读取 PC=0x%x 处的指令\n", pc);
            break;
        }
        decode(instr);
    }

    free(RAM);
    free(ROM);
    return 0;
}
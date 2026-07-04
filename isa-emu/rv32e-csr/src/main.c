#include "lcthw/cpu.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "用法: %s <program.bin> [mainargs]\n", argv[0]);
        return 1;
    }

    const char *prog_file = argv[1];
    const char *mainargs = (argc >= 3) ? argv[2] : NULL;

    cpu_reset();

    if (load_program(prog_file, 0x80000000, mainargs) != 0) {
        fprintf(stderr, "错误：无法加载程序 (文件: %s)\n", prog_file);
        return 1;
    }

    cpu.pc = 0x80000000;
    cpu.csr.mtvec = 0x80000000;  // 默认异常入口

    unsigned long long cycle = 0;
    while (1) {
        int ret = isa_exec_once();
        if (ret != 0) break;
        cycle++;
    }

    printf("[INFO] Simulation finished after %llu cycles\n", cycle);
    return 0;
}
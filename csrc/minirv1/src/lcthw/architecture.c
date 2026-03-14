#include "architecture.h"
#include "dbg.h"
#include "isa.h"

#include <sys/stat.h>  
#include <string.h>  
#include <stdlib.h>
#include <stdio.h>

int *RAM = NULL;
int *ROM = NULL;
int simulation_exit = 0;
int simulation_exit_code = 0;

//初始化ram, 单位是字
int init_ram(const char *file_path) {
    FILE *fp = NULL;
    int i = 0;
    size_t read_count = 0;

    if (!file_path || !*file_path) {
        log_err("RAM 文件路径为空");
        return -1;
    }

    RAM = malloc(RAM_SIZE * sizeof(int));
    check_mem(RAM);
    memset(RAM, 0, RAM_SIZE * sizeof(int));//初始化为全0

    fp = fopen(file_path, "rb");
    check(fp, "打开 RAM 文件失败: %s", file_path);

    while(1){
        read_count = fread(&RAM[i], sizeof(int), 1, fp); //fp→ram
        if(read_count != 1){
            break;
        }
        check(read_count == 1, "读取第%d个地址失败", i);
        i++;
    }

    fclose(fp);
    log_info("RAM 加载完成");
    return 0;

error:
    if (fp) fclose(fp);
    if (RAM) {
        free(RAM);
        RAM = NULL;
    }
    return -1;
}


//初始化rom，单位也是字
int init_rom(const char *file_path) {
    FILE *fp = NULL;
    int i = 0;
    size_t read_count = 0;

    if (!file_path || !*file_path) {
        log_err("ROM 文件路径为空");
        return -1;
    }

    ROM = malloc(ROM_SIZE * sizeof(int));
    check_mem(ROM);
    memset(ROM, 0, ROM_SIZE * sizeof(int));

    fp = fopen(file_path, "rb");
    check(fp, "打开 ROM 文件失败: %s", file_path);

    while(1){
        read_count = fread(&ROM[i], sizeof(int), 1, fp);
        if(read_count != 1){
            break;
        }
        check(read_count == 1, "读取第%d个地址失败", i);
        i++;
    }

    fclose(fp);
    log_info("ROM 加载完成");

    // 写入 ebreak 指令
    int ebreak_word_addr = 103;
    int ebreak_instr = 0x00100073;
    if (ebreak_word_addr < ROM_SIZE) {
        ROM[ebreak_word_addr] = ebreak_instr;
        log_info("已在 ROM 字地址 %d（字节地址 0x%x）写入 ebreak 指令",
                 ebreak_word_addr, ebreak_word_addr * 4);
    } else {
        log_err("ebreak 写入地址 %d 越界", ebreak_word_addr);
        goto error;
    }

    return 0;

error:
    if (fp) fclose(fp);
    if (ROM) {
        free(ROM);
        ROM = NULL;
    }
    return -1;
}

//读ram
int read_ram(int addr){
    if (RAM == NULL) {
        log_err("RAM 未初始化");
        return -1;
    }
    if (addr < 0 || addr >= RAM_SIZE) {
        log_err("RAM 地址越界: %d (最大: %d)", addr, RAM_SIZE-1);
        return -1;
    }
    return RAM[addr];
}

//写ram
void write_ram(int addr, int value){
    if (RAM == NULL) {
        log_err("RAM 未初始化");
        return;
    }
    if (addr < 0 || addr >= RAM_SIZE) {
        log_err("RAM 地址越界: %d (最大: %d)", addr, RAM_SIZE-1);
        return;
    }
    RAM[addr] = value;
}

//读rom
int read_rom(int addr){
    if (ROM == NULL) {
        log_err("ROM 未初始化");
        return -1;
    }
    if (addr < 0 || addr >= ROM_SIZE) {
        log_err("ROM 地址越界: %d (最大: %d)", addr, ROM_SIZE-1);
        return -1;
    }
    return ROM[addr];
}

//解码
int decode(int instruction){
    int opcode = ((unsigned int)instruction << 25) >> 25;
    int funct7 = ((unsigned int)instruction) >> 25;
    int funct3 = ((unsigned int)(instruction << 17)) >> 29;
    int rs1 = ((instruction << 12) >> 27) & 0x1F;
    int rs2 = ((instruction << 7) >> 27) & 0x1F;
    int rd = ((instruction << 20) >> 27) & 0x1F;
    int imm20 = ((unsigned int)instruction >> 12) & 0x000FFFFF;
    int imm12 = ((unsigned int)instruction >> 20) & 0x00000FFF;
    int imm12_merge = ((((unsigned int)instruction >> 25) & 0x7F) << 5 | (((unsigned int)instruction >> 7) & 0x1F)) & 0x00000FFF;

    switch(opcode){
        case 115: {
            if(funct3 == 0) {
                int exit_code = GFR[10];
                fprintf(stderr, "HIT %s TRAP (exit code: %d)\n",
                        exit_code == 0 ? "GOOD" : "BAD", exit_code);
                simulation_exit = 1;
                simulation_exit_code = exit_code;
            } else {
                printf("未知系统指令，地址是%d\n", GFR[PC]);
            }
            break;
        }
        case 51: {
            if(funct3 == 0 && funct7 == 0) {
                add(GFR, rs2, rs1, rd);
            } else {
                printf("指令无法识别，地址是%d\n", GFR[PC]);
            }
            break; 
        }
        case 19: {
            if(funct3 == 0) {
                addi(GFR, imm12, rs1, rd);
            } else {
                printf("指令无法识别，地址是%d\n", GFR[PC]);
            }
            break;
        }
        case 55: {
            lui(GFR, imm20, rd);
            break;
        }
        case 3: {
            if(funct3 == 2) {
                lw(RAM, GFR, imm12, rs1, rd);
            } else if(funct3 == 4) {
                lbu(RAM, GFR, imm12, rs1, rd);
            } else {
                printf("指令无法识别，地址是%d\n", GFR[PC]);
            }
            break;
        }
        case 35: {
            if(funct3 == 2) {
                sw(RAM, GFR, rs2, rs1, imm12_merge);
            } else if(funct3 == 0) {
                sb(RAM, GFR, rs2, rs1, imm12_merge);
            } else {
                printf("指令无法识别，地址是%d\n", GFR[PC]);
            }
            break;
        }
        case 103: {
            if(funct3 == 0) {
                jalr(GFR, imm12, rs1, rd);
            } else {
                printf("指令无法识别，地址是%d\n", GFR[PC]);
            }
            break;
        }
        default: {
            printf("未知opcode，指令无法识别，地址是%d\n", GFR[PC]);
            break;
        }
    }
    return 0;
}
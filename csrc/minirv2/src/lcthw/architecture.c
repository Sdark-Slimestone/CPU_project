#include "architecture.h"
#include "dbg.h"
#include "isa.h"

#include <sys/stat.h>  
#include <string.h>  
#include <stdlib.h>
#include <stdio.h>

int *RAM = NULL;
int *ROM = NULL;

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
    if (fp) fclose(fp);  // 如果文件打开过，就关闭
    if (RAM) {           // 如果内存分配过，就释放
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
    memset(ROM, 0, ROM_SIZE * sizeof(int));//初始化为全0

    fp = fopen(file_path, "rb");
    check(fp, "打开 ROM 文件失败: %s", file_path);

    while(1){
        read_count = fread(&ROM[i], sizeof(int), 1, fp); //fp→rom
        if(read_count != 1){
            break;
        }
        check(read_count == 1, "读取第%d个地址失败", i);
        i++;
    }

    fclose(fp);
    log_info("ROM 加载完成");

    // 1. 换算后的字地址：0xdb8（字节地址） /4 = 878（十进制）
    //137：sum 1158：mem 876：vga
    int ebreak_word_addr = 878;
    // 2. ebreak 指令的机器码：0x00100073
    int ebreak_instr = 0x00100073;
    // 3. 写入 ROM（覆盖原来的 jalr 死循环指令）
    if (ebreak_word_addr < ROM_SIZE) { // 安全检查：地址不越界
        ROM[ebreak_word_addr] = ebreak_instr;
        log_info("已在 ROM 字地址 %d（字节地址 0x%x）写入 ebreak 指令（机器码：0x%x）",
                 ebreak_word_addr, ebreak_word_addr * 4, ebreak_instr);
    } else {
        log_err("ebreak 写入地址 %d 越界（ROM 最大字地址：%d）", ebreak_word_addr, ROM_SIZE-1);
        goto error;
    }


    return 0;

error:
    if (fp) fclose(fp);  // 如果文件打开过，就关闭
    if (ROM) {           // 如果内存分配过，就释放
        free(ROM);
        ROM = NULL;
    }
    return -1;
}

//读ram
int read_ram(int addr){
    if (addr < 0 || addr >= RAM_SIZE) {
        log_err("RAM 地址越界: %d (最大: %d)", addr, RAM_SIZE-1);
        return -1;
    }
    int data = RAM[addr];
    return data;    
}

//写ram
void write_ram(int addr, int value){
    if (addr < 0 || addr >= RAM_SIZE) {
        log_err("RAM 地址越界: %d (最大: %d)", addr, RAM_SIZE-1);
        return;
    }
    RAM[addr] = value;
}

//读rom
int read_rom(int addr){
    if (addr < 0 || addr >= ROM_SIZE) {
        log_err("ROM 地址越界: %d (最大: %d)", addr, ROM_SIZE-1);
        return -1;
    }
    int instruction = ROM[addr];
    return instruction;
}

//解码
//decoder
int decode(int instruction){
    int opcode = ((unsigned int)instruction << 25) >> 25;
    int funct7 = ((unsigned int)instruction) >> 25;
    int funct3 = ((unsigned int)(instruction << 17)) >> 29;
    int rs1 = ((instruction << 12) >> 27) & 0x1F;
    int rs2 = ((instruction << 7) >> 27) & 0x1F;
    int rd = ((instruction << 20) >> 27) & 0x1F;
    int imm20 = ((unsigned int)instruction >> 12) & 0x000FFFFF;//低20位
    int imm12 = ((unsigned int)instruction >> 20) & 0x00000FFF;
    int imm12_merge = ((((unsigned int)instruction >> 25) & 0x7F) << 5 | (((unsigned int)instruction >> 7) & 0x1F)) & 0x00000FFF;
    switch(opcode){
        case 115: {
        // funct3=0 是 ebreak 指令（其他 funct3 是其他系统指令，暂不处理）
            if(funct3 == 0) {
                //获取 a0 寄存器的值（a0 对应 x10，即 GFR[10]）
                int exit_code = GFR[10];
                
                //根据 exit_code 输出结果
                if(exit_code == 0) {
                    fprintf(stderr, "HIT GOOD TRAP (exit code: %d)\n", exit_code);
                } else {
                    fprintf(stderr, "HIT BAD TRAP (exit code: %d)\n", exit_code);
                }
                
                // 停止仿真（直接退出程序，或设置全局标志位）
                // 这里简单处理：释放内存 + 退出程序
                if(RAM) free(RAM);
                if(ROM) free(ROM);
                exit(exit_code); // 用 exit_code 作为程序退出码
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
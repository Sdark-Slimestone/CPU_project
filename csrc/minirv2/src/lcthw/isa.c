#include "isa.h"
#include "dbg.h"
#include "architecture.h" 

int GFR[17] = {0};

// 另外一个文件里生成的rom和ram都是字为单位，所以寻址是按照高30位来
void add(int *gfr, int rs2, int rs1, int rd){
    if(rd !=0){
        gfr[rd] = gfr[rs2] + gfr[rs1];
    }
    else {
        gfr[rd] = gfr[rd];
    }
    gfr[PC] = gfr[PC] + 4;
}

void addi(int *gfr, int imm12, int rs1, int rd){
    int imm12_signed = (imm12 & 0x800) ? (imm12 | 0xFFFFF000) : imm12;
    if(rd !=0){
        gfr[rd] = gfr[rs1] + imm12_signed; 
    }
    else{
        gfr[rd] = gfr[rd];
    }
    gfr[PC] = gfr[PC] + 4;
}

void lui(int *gfr, int imm20, int rd){ //传入的imm20是被放在低20位的
    if(rd != 0){
        gfr[rd] = (unsigned int)imm20 << 12;
    }
    else{
        gfr[rd] = gfr[rd];
    }
    gfr[PC] = gfr[PC] + 4;
}

void lw(int *ram, int *gfr, int imm12, int rs1, int rd){
    int imm12_signed = (imm12 & 0x800) ? (imm12 | 0xFFFFF000) : imm12;
    int addr = ((unsigned int)(gfr[rs1] + imm12_signed)) >> 2;
    if(rd != 0){
        gfr[rd] = ram[addr]; 
    }
    else{
        gfr[rd] = gfr[rd];
    }
    gfr[PC] = gfr[PC] + 4;
}

void lbu(int *ram, int *gfr, int imm12, int rs1, int rd){
    int imm12_signed = (imm12 & 0x800) ? (imm12 | 0xFFFFF000) : imm12;
    int addr_low2 = (((unsigned int)((gfr[rs1] + imm12_signed) << 30)) >> 30) & 0x00000003;
    int addr = ((unsigned int)(gfr[rs1] + imm12_signed)) >> 2;
    if(rd != 0){
        switch(addr_low2){
            case 0: gfr[rd] = ((unsigned int)(ram[addr] << 24)) >> 24;
                    break;
            case 1: gfr[rd] = ((unsigned int)(ram[addr] << 16)) >> 24;
                    break;
            case 2: gfr[rd] = ((unsigned int)(ram[addr] << 8)) >> 24;
                    break;
            case 3: gfr[rd] = ((unsigned int)ram[addr]) >> 24;
                    break;
        }
    }
    else{
        gfr[rd] = gfr[rd];
    }
    gfr[PC] = gfr[PC] + 4;
}

void sw(int *ram, int *gfr, int rs2, int rs1, int imm12_merge){
    int imm12_merge_signed = (imm12_merge & 0x800) ? (imm12_merge | 0xFFFFF000) : imm12_merge;
    int byte_addr = gfr[rs1] + imm12_merge_signed;   // 实际字节地址
    int word_addr = byte_addr >> 2;                   // 字地址

    // 检查字地址是否越界（有效范围 0 ~ RAM_SIZE-1）
    if (word_addr >= RAM_SIZE) {
        // 可选：打印警告
        // log_warn("sw 忽略对地址 0x%x 的写入 (PC=0x%x)", byte_addr, gfr[PC]);
        gfr[PC] += 4;          // 必须更新 PC，否则程序卡死
        return;
    }

    // 如果要求字对齐，可在此处检查 byte_addr 低2位是否为0
    // if (byte_addr & 3) { /* 未对齐处理 */ }

    ram[word_addr] = gfr[rs2];
    gfr[PC] += 4;
}

void sb(int *ram, int *gfr, int rs2, int rs1, int imm12_merge){
    int imm12_merge_signed = (imm12_merge & 0x800) ? (imm12_merge | 0xFFFFF000) : imm12_merge;
    int addr_low2 = (((unsigned int)((gfr[rs1] + imm12_merge_signed) << 30)) >> 30) & 0x00000003;
    int addr = ((unsigned int)(gfr[rs1] + imm12_merge_signed)) >> 2;

    // 检查字地址是否越界（RAM_SIZE 在 architecture.h 中定义为 262144）
    if (addr >= RAM_SIZE) {
        // 忽略越界写入，仅更新 PC 并返回
        gfr[PC] += 4;
        return;
    }

    int content = (((unsigned int)(gfr[rs2] << 24)) >> 24) & 0x000000FF;
    switch(addr_low2){
        case 0: { 
            int ram_waiting_change = ((unsigned int)ram[addr]) & 0xFFFFFF00;
            ram[addr] = ram_waiting_change | content;  
            break;
        }
        case 1: {
            int ram_waiting_change = ((unsigned int)ram[addr]) & 0xFFFF00FF;
            ram[addr] = ram_waiting_change | (content << 8);  
            break;
        }
        case 2: {
            int ram_waiting_change = ((unsigned int)ram[addr]) & 0xFF00FFFF;
            ram[addr] = ram_waiting_change | (content << 16);  
            break;
        }
        case 3: {
            int ram_waiting_change = ((unsigned int)ram[addr]) & 0x00FFFFFF;
            ram[addr] = ram_waiting_change | (content << 24);  
            break;
        }
    }
    gfr[PC] += 4;
}

void jalr(int *gfr, int imm12, int rs1, int rd){
    int imm12_signed = (imm12 & 0x800) ? (imm12 | 0xFFFFF000) : imm12;
    unsigned int addr = (unsigned int)(gfr[rs1] + imm12_signed);
    if(rd != 0){
        gfr[rd] = gfr[PC] + 4;
    }
    else{
        gfr[rd] = gfr[rd];
    }
    //jalr强制清零最低位
    addr = addr & 0xFFFFFFFE;
    gfr[PC] = (int)addr;
}
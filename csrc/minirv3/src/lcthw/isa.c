#include "isa.h"
#include "dbg.h"
#include "architecture.h"

int GFR[17] = {0};
int VRAM[0x10000] = {0};   // 显存，大小 0x10000 字（对应 256KB）

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

void lui(int *gfr, int imm20, int rd){
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

    // 显存读取
    if (addr >= 0x8000000 && addr < 0x8010000) {
        if (rd != 0) gfr[rd] = VRAM[addr - 0x8000000];
        gfr[PC] += 4;
        return;
    }

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

    // 显存读取字节
    if (addr >= 0x8000000 && addr < 0x8010000) {
        if (rd != 0) {
            int vram_word = VRAM[addr - 0x8000000];
            switch(addr_low2){
                case 0: gfr[rd] = ((unsigned int)(vram_word << 24)) >> 24; break;
                case 1: gfr[rd] = ((unsigned int)(vram_word << 16)) >> 24; break;
                case 2: gfr[rd] = ((unsigned int)(vram_word << 8)) >> 24; break;
                case 3: gfr[rd] = ((unsigned int)vram_word) >> 24; break;
            }
        }
        gfr[PC] += 4;
        return;
    }

    if(rd != 0){
        switch(addr_low2){
            case 0: gfr[rd] = ((unsigned int)(ram[addr] << 24)) >> 24; break;
            case 1: gfr[rd] = ((unsigned int)(ram[addr] << 16)) >> 24; break;
            case 2: gfr[rd] = ((unsigned int)(ram[addr] << 8)) >> 24; break;
            case 3: gfr[rd] = ((unsigned int)ram[addr]) >> 24; break;
        }
    }
    else{
        gfr[rd] = gfr[rd];
    }
    gfr[PC] = gfr[PC] + 4;
}

void sw(int *ram, int *gfr, int rs2, int rs1, int imm12_merge){
    int imm12_merge_signed = (imm12_merge & 0x800) ? (imm12_merge | 0xFFFFF000) : imm12_merge;
    int addr = ((unsigned int)(gfr[rs1] + imm12_merge_signed)) >> 2;

    //  显存写入（字） 
    if (addr >= 0x8000000 && addr < 0x8010000) {
        VRAM[addr - 0x8000000] = gfr[rs2];
        gfr[PC] += 4;
        return;
    }

    if(addr > 262144){
        return;   
    }
    ram[addr] = gfr[rs2];
    gfr[PC] = gfr[PC] + 4;
}

void sb(int *ram, int *gfr, int rs2, int rs1, int imm12_merge){
    int imm12_merge_signed = (imm12_merge & 0x800) ? (imm12_merge | 0xFFFFF000) : imm12_merge;
    int addr_low2 = (((unsigned int)((gfr[rs1] + imm12_merge_signed) << 30)) >> 30) & 0x00000003;
    int addr = ((unsigned int)(gfr[rs1] + imm12_merge_signed)) >> 2;
    int content = (((unsigned int)(gfr[rs2] << 24)) >> 24) & 0x000000FF;

    // 显存写入 字节
    if (addr >= 0x8000000 && addr < 0x8010000) {
        int vram_idx = addr - 0x8000000;
        unsigned int old = (unsigned int)VRAM[vram_idx];
        unsigned int mask = 0xFFU << (addr_low2 * 8);
        VRAM[vram_idx] = (int)((old & ~mask) | (content << (addr_low2 * 8)));
        gfr[PC] += 4;
        return;
    }

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
    gfr[PC] = gfr[PC] + 4;
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
    addr = addr & 0xFFFFFFFE;
    gfr[PC] = (int)addr;
}
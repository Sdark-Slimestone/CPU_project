#ifndef isa_h
#define isa_h

#include <stdio.h>

extern int GFR[17];

extern int *RAM;
extern int *ROM;

#define PC  16

void add(int *gfr, int rs2, int rs1, int rd);
void addi(int *gfr, int imm12, int rs1, int rd);
void lui(int *gfr, int imm20, int rd);
void lw(int *ram, int *gfr, int imm12, int rs1, int rd);
void lbu(int *ram, int *gfr, int imm12, int rs1, int rd);
void sw(int *ram, int *gfr, int rs2, int rs1, int imm12_merge);
void sb(int *ram, int *gfr, int rs2, int rs1, int imm12_merge);
void jalr(int *gfr, int imm12, int rs1, int rd);
void vga_show_framebuffer(void);

#endif
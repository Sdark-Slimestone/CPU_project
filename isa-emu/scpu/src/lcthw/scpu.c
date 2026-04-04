#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

uint8_t PC = 0;
uint8_t R[4];
// 新的指令序列：从 R[0] 读取末项 n，计算 1+...+n
// 地址0: li R1,0   (0x90)
// 地址1: li R2,0   (0xA0)
// 地址2: li R3,1   (0xB1)
// 地址3: add R1,R1,R3 (0x17)
// 地址4: add R2,R2,R1 (0x29)
// 地址5: bne R0,R1,3  (0xCD)   // 若 R0 != R1 则跳转到地址3
// 地址6: out R2      (0x42)
uint8_t M[16] = {0x90, 0xA0, 0xB1, 0x17, 0x29, 0xCD, 0x42, 0x00};

uint8_t fetch_bin(uint8_t *M, uint8_t *PC_p) {
    if (M == NULL) {
        printf("取指：指令地址无效\n");
        return 0x7F;
    }
    return M[*PC_p];
}

void add(uint8_t *R, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    if (R == NULL) {
        printf("add指令：寄存器指针为空\n");
        return;
    }
    R[rd] = R[rs1] + R[rs2];
}

void li(uint8_t *R, uint8_t rd, uint8_t imm) {
    if (R == NULL) {
        printf("li指令：寄存器指针为空\n");
        return;
    }
    R[rd] = imm;
}

void bner0(uint8_t *R, uint8_t addr, uint8_t rs2) {
    if (R == NULL) {
        printf("bner0指令：寄存器指针为空\n");
        return;
    }
    if (R[0] != R[rs2]) {
        PC = addr;
    } else {
        PC++;
    }
}

void decoder(uint8_t instruction) {
    uint8_t op = instruction >> 6;                    
    uint8_t rd  = ((uint8_t)(instruction << 2)) >> 6; 
    uint8_t rs1 = ((uint8_t)(instruction << 4)) >> 6; 
    uint8_t rs2 = ((uint8_t)(instruction << 6)) >> 6; 
    uint8_t imm = ((uint8_t)(instruction << 4)) >> 4;  
    uint8_t addr= ((uint8_t)(instruction << 2)) >> 4; 

    switch (op) {
        case 0: // add
            add(R, rd, rs1, rs2);
            PC++;
            break;
        case 2: // li
            li(R, rd, imm);
            PC++;
            break;
        case 3: // bner0
            bner0(R, addr, rs2);
            break;
        case 1: // out（停机）
            printf("结果为：%d\n", R[rs2]);
            exit(0);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("用法: %s <n>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    if (n < 0 || n > 255) {
        printf("n 必须在 0~255 之间\n");
        return 1;
    }
    R[0] = (uint8_t)n;  // 将命令行参数存入 R0

    uint8_t instemp;
    int turn = 0;
    while (turn < 50) {
        instemp = fetch_bin(M, &PC);
        if (instemp == 0x7F) {
            printf("取指失败，pc地址=%d\n", PC);
            break;
        }
        decoder(instemp);
        turn++;
    }
    printf("寄存器值：%d, %d, %d, %d\n", R[0], R[1], R[2], R[3]);
    return 0;
}
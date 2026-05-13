#include "Vtop.h"
#include "verilated.h"
#include <stdio.h>

vluint64_t main_time = 0;
double sc_time_stamp() { return main_time; }

// 你的指令内存
uint32_t imem[] = {
    0xfac10113,  // addi sp, sp, -84
    0x04112823,  // sw ra, 80(sp)
    0x02c12023,  // sw a2, 32(sp)
    0x00a12023,  // sw a0, 0(sp)
    0x1ec5d663,  // bge a1, a2, 800002e8
    0x04812623,  // sw s0, 76(sp)
    0x04912423,  // sw s1, 72(sp)
    0x00058413,  // mv s0, a1
    0x02012603,  // lw a2, 32(sp)
    0x00012503,  // lw a0, 0(sp)
    0x00040593,  // mv a1, s0
    0xf11ff0ef,  // jal 80000028 <partition>
    0xfff50613,  // addi a2, a0, -1
    0x00050493,  // mv s1, a0
    0x1ac45863,  // bge s0, a2, 800002d4
    0x02a12423,  // sw a0, 40(sp)
    0x00060493,  // mv s1, a2
    0x00012503,  // lw a0, 0(sp)
    0x00048613,  // mv a2, s1
    0x00040593,  // mv a1, s0
    0xeedff0ef,  // jal 80000028 <partition>
    0xfff50793,  // addi a5, a0, -1
    0x00f12c23,  // sw a5, 24(sp)
    0x18f45063,  // bge s0, a5, 800002c8
    0x02912623,  // sw s1, 44(sp)
    0x02a12823,  // sw a0, 48(sp)
    0x00040493,  // mv s1, s0
    0x01812603,  // lw a2, 24(sp)
    0x00012503,  // lw a0, 0(sp)
    0x00048593,  // mv a1, s1
    0xec5ff0ef,  // jal 80000028 <partition>
    0xfff50793,  // addi a5, a0, -1
    0x00f12e23,  // sw a5, 28(sp)
    0x14f4d263,  // bge s1, a5, 800002b4
    0x02a12a23,  // sw a0, 52(sp)
    0x00048413,  // mv s0, s1
    0x01c12603,  // lw a2, 28(sp)
    0x00012503,  // lw a0, 0(sp)
    0x00040593,  // mv a1, s0
    0xea1ff0ef,  // jal 80000028 <partition>
    0xfff50793,  // addi a5, a0, -1
    0x00f12223,  // sw a5, 4(sp)
    0x00050493,  // mv s1, a0
    0x10f45663,  // bge s0, a5, 800002a4
    0x02a12c23,  // sw a0, 56(sp)
    0x00040493,  // mv s1, s0
    0x00412603,  // lw a2, 4(sp)
    0x00012503,  // lw a0, 0(sp)
    0x00048593,  // mv a1, s1
    0xe79ff0ef,  // jal 80000028 <partition>
    0xfff50413,  // addi s0, a0, -1
    0x0c84de63,  // bge s1, s0, 80000294
    0x00a12623,  // sw a0, 12(sp)
    0x00812423,  // sw s0, 8(sp)
    0x00812603,  // lw a2, 8(sp)
    0x00012503,  // lw a0, 0(sp)
    0x00048593,  // mv a1, s1
    0xe59ff0ef,  // jal 80000028 <partition>
    0xfff50713,  // addi a4, a0, -1
    0x00e12823,  // sw a4, 16(sp)
    0x00050793,  // mv a5, a0
    0x0ae4d263,  // bge s1, a4, 80000284
    0x00048413,  // mv s0, s1
    0x01012603,  // lw a2, 16(sp)
    0x00012503,  // lw a0, 0(sp)
    0x00040593,  // mv a1, s0
    0x00f12a23,  // sw a5, 20(sp)
    0xe31ff0ef,  // jal 80000028 <partition>
    0x02a12223,  // sw a0, 36(sp)
    0xfff50493,  // addi s1, a0, -1
    0x01412783,  // lw a5, 20(sp)
    0x06945663,  // bge s0, s1, 80000274
    0x00012503,  // lw a0, 0(sp)
    0x00048613,  // mv a2, s1
    0x00040593,  // mv a1, s0
    0xe11ff0ef,  // jal 80000028 <partition>
    0xfff50713,  // addi a4, a0, -1
    0x00050793,  // mv a5, a0
    0x04e45263,  // bge s0, a4, 80000268
    0x00040593,  // mv a1, s0
    0x00012503,  // lw a0, 0(sp)
    0x00070613,  // mv a2, a4
    0x04f12223,  // sw a5, 68(sp)
    0x04e12023,  // sw a4, 64(sp)
    0x02b12e23,  // sw a1, 60(sp)
    0xde9ff0ef,  // jal 80000028 <partition>
    0x03c12583,  // lw a1, 60(sp)
    0x00050413,  // mv s0, a0
    0xfff50613,  // addi a2, a0, -1
    0x00012503,  // lw a0, 0(sp)
    0xe99ff0ef,  // jal 800000ec <quick_sort>
    0x04012703,  // lw a4, 64(sp)
    0x00140593,  // addi a1, s0, 1
    0x04412783,  // lw a5, 68(sp)
    0xfce5c4e3,  // blt a1, a4, 8000022c
    0x00178413,  // addi s0, a5, 1
    0xfa9440e3,  // blt s0, s1, 8000020c
    0x01412783,  // lw a5, 20(sp)
    0x02412703,  // lw a4, 36(sp)
    0x00170413,  // addi s0, a4, 1
    0x01012703,  // lw a4, 16(sp)
    0xf6e444e3,  // blt s0, a4, 800001e8
    0x00178493,  // addi s1, a5, 1
    0x00812783,  // lw a5, 8(sp)
    0xf2f4cce3,  // blt s1, a5, 800001c4
    0x00c12503,  // lw a0, 12(sp)
    0x00412783,  // lw a5, 4(sp)
    0x00150493,  // addi s1, a0, 1
    0xf0f4c4e3,  // blt s1, a5, 800001a4
    0x03812483,  // lw s1, 56(sp)
    0x01c12783,  // lw a5, 28(sp)
    0x00148413,  // addi s0, s1, 1
    0xecf448e3,  // blt s0, a5, 8000017c
    0x03412503,  // lw a0, 52(sp)
    0x01812783,  // lw a5, 24(sp)
    0x00150493,  // addi s1, a0, 1
    0xe8f4cee3,  // blt s1, a5, 80000158
    0x02c12483,  // lw s1, 44(sp)
    0x03012503,  // lw a0, 48(sp)
    0x00150413,  // addi s0, a0, 1
    0xe69442e3,  // blt s0, s1, 80000130
    0x02812483,  // lw s1, 40(sp)
    0x02012783,  // lw a5, 32(sp)
    0x00148413,  // addi s0, s1, 1
    0xe2f448e3,  // blt s0, a5, 8000010c
    0x04c12403,  // lw s0, 76(sp)
    0x04812483,  // lw s1, 72(sp)
    0x05012083,  // lw ra, 80(sp)
    0x05410113,  // addi sp, sp, 84
    0x00008067,  // ret
};
#define MEM_SIZE (sizeof(imem)/sizeof(imem[0]))

uint32_t fetch_inst(uint32_t pc) {
    int idx = (pc - 0x80000000) / 4;
    if (idx < 0 || idx >= (int)MEM_SIZE) return 0x00000013; // nop
    return imem[idx];
}

void fetch_pair(uint32_t pc, uint32_t *i1, uint32_t *i2) {
    *i1 = fetch_inst(pc);
    *i2 = fetch_inst(pc + 4);
}

int main(int argc, char **argv) {
    VerilatedContext* ctx = new VerilatedContext;
    ctx->commandArgs(argc, argv);
    Vtop* top = new Vtop(ctx);

    top->clock = 0;
    top->reset = 1;
    top->io_exu_to_ifu_take_branch = 0;
    top->io_exu_to_ifu_branch_target = 0;
    top->io_imem_to_ifu_inst1 = 0;
    top->io_imem_to_ifu_inst2 = 0;

    // 复位几个周期
    for (int i = 0; i < 5; i++) {
        top->clock = 1;
        top->eval();
        top->clock = 0;
        top->eval();
    }
    top->reset = 0;

    int cycles = 0;
    while (cycles < 200) {
        uint32_t addr1 = top->io_ifu_to_imem_addr1;
        uint32_t addr2 = top->io_ifu_to_imem_addr2;
        uint32_t i1, i2;
        fetch_pair(addr1, &i1, &i2);
        top->io_imem_to_ifu_inst1 = i1;
        top->io_imem_to_ifu_inst2 = i2;

        // 时钟上升沿
        top->clock = 1;
        top->eval();
        main_time++;

        printf("[%2d] PC=%08x\n", cycles, (unsigned int)top->io_debug_ifu_inst1_pc);
        printf("     addr1=%08x inst1=0x%08x -> is_addi=%d is_auipc=%d rd=%d rs1=%d\n",
               addr1, i1, top->io_idu_to_exu1_dec1_op_is_addi,
               top->io_idu_to_exu1_dec1_op_is_auipc,
               top->io_idu_to_exu1_dec1_rd,
               top->io_debug_grf_addr_dec1_redreg_rs1);
        printf("     addr2=%08x inst2=0x%08x -> is_addi=%d is_auipc=%d rd=%d rs1=%d\n",
               addr2, i2, top->io_idu_to_exu2_dec2_op_is_addi,
               top->io_idu_to_exu2_dec2_op_is_auipc,
               top->io_idu_to_exu2_dec2_rd,
               top->io_debug_grf_addr_dec2_redreg_rs1);

        top->clock = 0;
        top->eval();
        main_time++;
        cycles++;

        // 当取指地址超出内存范围时停止（可选）
        if (addr1 >= 0x80000000 + MEM_SIZE * 4) break;
    }

    // IPC 计算：总静态指令数 / 总周期数
    float ipc = (float)MEM_SIZE / (float)cycles;
    printf("\nSimulation finished.\n");
    printf("Total cycles: %d\n", cycles);
    printf("Static instruction count: %d\n", MEM_SIZE);
    printf("IPC = %.2f\n", ipc);

    delete top;
    delete ctx;
    return 0;
}
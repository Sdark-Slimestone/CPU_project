#include "minunit.h"
#include <lcthw/architecture.h>
#include <lcthw/isa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 辅助函数：创建临时文件并写入指定内容（以字为单位）
static int create_temp_file(const char *filename, int *data, int word_count) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return -1;
    size_t written = fwrite(data, sizeof(int), word_count, fp);
    fclose(fp);
    return (written == word_count) ? 0 : -1;
}

// 清理全局状态：释放 RAM 和 ROM，并重置指针
static void cleanup_memory() {
    if (RAM) {
        free(RAM);
        RAM = NULL;
    }
    if (ROM) {
        free(ROM);
        ROM = NULL;
    }
}

// 每个测试之前运行：清空 GFR
static void setup() {
    memset(GFR, 0, sizeof(GFR));
    // PC 通常从 0 开始，但测试时可以自行设置
}

// 每个测试之后运行：释放内存
static void teardown() {
    cleanup_memory();
}

// 测试 init_ram 从文件加载数据
char *test_init_ram() {
    setup();
    
    // 准备测试数据
    int test_data[] = {0x12345678, 0x9ABCDEF0, 0xDEADBEEF};
    const char *tmp_file = "ram_test.bin";
    mu_assert(create_temp_file(tmp_file, test_data, 3) == 0, "Failed to create test RAM file");
    
    // 调用 init_ram
    int rc = init_ram(tmp_file);
    mu_assert(rc == 0, "init_ram failed");
    
    // 验证 RAM 内容
    mu_assert(RAM != NULL, "RAM is NULL");
    mu_assert(read_ram(0) == 0x12345678, "RAM[0] mismatch");
    mu_assert(read_ram(1) == 0x9ABCDEF0, "RAM[1] mismatch");
    mu_assert(read_ram(2) == 0xDEADBEEF, "RAM[2] mismatch");
    // 未初始化的位置应为 0
    mu_assert(read_ram(3) == 0, "RAM[3] should be 0");
    
    // 清理
    remove(tmp_file);
    teardown();
    return NULL;
}

// 测试 init_rom 从文件加载数据，并验证 ebreak 覆盖
char *test_init_rom() {
    setup();
    
    // 准备测试数据，注意 ebreak 会覆盖地址 1158，所以我们写入一些其他值
    int test_data[ROM_SIZE];
    for (int i = 0; i < ROM_SIZE; i++) {
        test_data[i] = i * 2;  // 随便填
    }
    const char *tmp_file = "rom_test.bin";
    mu_assert(create_temp_file(tmp_file, test_data, ROM_SIZE) == 0, "Failed to create test ROM file");
    
    int rc = init_rom(tmp_file);
    mu_assert(rc == 0, "init_rom failed");
    
    // 验证 ROM 内容：除 1158 外，其他应与 test_data 一致
    mu_assert(ROM != NULL, "ROM is NULL");
    for (int i = 0; i < ROM_SIZE; i++) {
        if (i == 1158) {
            mu_assert(ROM[i] == 0x00100073, "EBREAK not written at address 1158");
        } else {
            mu_assert(ROM[i] == i * 2, "ROM content mismatch");
        }
    }
    
    remove(tmp_file);
    teardown();
    return NULL;
}

// 测试 write_ram / read_ram 基本功能
char *test_ram_read_write() {
    setup();
    
    // 先初始化 RAM（使用空文件，实际上我们直接 malloc，但为了简单，调用 init_ram 传 NULL 会失败，所以直接手动分配）
    // 实际上 init_ram 需要有效文件，但测试时我们可以直接 malloc 来测试读写
    RAM = malloc(RAM_SIZE * sizeof(int));
    mu_assert(RAM != NULL, "malloc RAM failed");
    memset(RAM, 0, RAM_SIZE * sizeof(int));
    
    write_ram(10, 0xAABBCCDD);
    mu_assert(read_ram(10) == 0xAABBCCDD, "read/write mismatch");
    
    // 边界测试（应打印错误但不崩溃）
    write_ram(RAM_SIZE, 0x1234);      // 越界
    mu_assert(read_ram(RAM_SIZE) == -1, "越界读应该返回 -1"); // read_ram 越界返回 -1
    // 注意 read_ram 越界返回 -1，但 write_ram 无返回值，我们只需确保程序不崩
    
    free(RAM);
    RAM = NULL;
    teardown();
    return NULL;
}

// 测试 read_rom 基本功能
char *test_rom_read() {
    setup();
    
    // 手动分配 ROM 并写入测试值，因为 init_rom 需要文件，这里直接操作全局 ROM
    ROM = malloc(ROM_SIZE * sizeof(int));
    mu_assert(ROM != NULL, "malloc ROM failed");
    for (int i = 0; i < ROM_SIZE; i++) {
        ROM[i] = i + 100;
    }
    
    mu_assert(read_rom(0) == 100, "ROM[0] mismatch");
    mu_assert(read_rom(10) == 110, "ROM[10] mismatch");
    mu_assert(read_rom(ROM_SIZE) == -1, "越界读应该返回 -1");
    
    free(ROM);
    ROM = NULL;
    teardown();
    return NULL;
}

// 辅助函数：设置 GFR 初始值，调用 decode，并检查结果
static void test_insn(int insn, int expected_pc, int check_reg, int expected_val) {
    // 注意：decode 内部会修改 GFR[PC]，所以先设置好当前 PC
    GFR[PC] = 0x1000;  // 随便设个起始 PC
    decode(insn);
    mu_assert(GFR[PC] == expected_pc, "PC update mismatch");
    if (check_reg >= 0) {
        mu_assert(GFR[check_reg] == expected_val, "Register value mismatch");
    }
}

// 测试 add 指令 (opcode=51, funct3=0, funct7=0)
char *test_decode_add() {
    setup();
    // add x3, x1, x2   (rd=3, rs1=1, rs2=2)
    // 机器码: 0000000 00010 00001 000 00011 0110011
    // funct7=0x00, rs2=2, rs1=1, funct3=0, rd=3, opcode=51
    int insn = (0 << 25) | (2 << 20) | (1 << 15) | (0 << 12) | (3 << 7) | 51;
    GFR[1] = 10;
    GFR[2] = 20;
    test_insn(insn, 0x1004, 3, 30);  // rd 不应为 0
    // 测试 rd=0 的情况：add x0, x1, x2
    insn = (0 << 25) | (2 << 20) | (1 << 15) | (0 << 12) | (0 << 7) | 51;
    GFR[1] = 10;
    GFR[2] = 20;
    GFR[PC] = 0x1000;
    decode(insn);
    mu_assert(GFR[0] == 0, "x0 should remain 0");
    mu_assert(GFR[PC] == 0x1004, "PC should increment");
    teardown();
    return NULL;
}

// 测试 addi 指令 (opcode=19, funct3=0)
char *test_decode_addi() {
    setup();
    // addi x4, x1, -100  (rd=4, rs1=1, imm12=-100 符号扩展)
    // imm12 = -100 的 12 位补码: 0xF9C (即 0xF9C 在 12 位中是 -100)
    int imm12 = 0xF9C;
    int insn = (imm12 << 20) | (1 << 15) | (0 << 12) | (4 << 7) | 19;
    GFR[1] = 200;
    test_insn(insn, 0x1004, 4, 100);  // 200 + (-100) = 100
    teardown();
    return NULL;
}

// 测试 lui 指令 (opcode=55)
char *test_decode_lui() {
    setup();
    // lui x5, 0x12345  (rd=5, imm20=0x12345)
    int imm20 = 0x12345;
    int insn = (imm20 << 12) | (5 << 7) | 55;
    test_insn(insn, 0x1004, 5, 0x12345000);  // 左移 12 位
    teardown();
    return NULL;
}

// 测试 lw 指令 (opcode=3, funct3=2)
char *test_decode_lw() {
    setup();
    // 需要先初始化 RAM
    RAM = malloc(RAM_SIZE * sizeof(int));
    mu_assert(RAM != NULL, "malloc RAM failed");
    memset(RAM, 0, RAM_SIZE * sizeof(int));
    int addr = 0x100;  // 字节地址
    int word_addr = addr / 4;  // 100/4=25
    RAM[word_addr] = 0xDEADBEEF;
    
    // lw x6, 0x100(x1)  (rd=6, rs1=1, imm12=0x100)
    int imm12 = 0x100;
    int insn = (imm12 << 20) | (1 << 15) | (2 << 12) | (6 << 7) | 3;
    GFR[1] = 0;  // 基址 0，加上 0x100 得到 0x100
    GFR[PC] = 0x1000;
    decode(insn);
    mu_assert(GFR[6] == 0xDEADBEEF, "lw loaded wrong value");
    mu_assert(GFR[PC] == 0x1004, "PC update failed");
    
    free(RAM);
    RAM = NULL;
    teardown();
    return NULL;
}

// 测试 sw 指令 (opcode=35, funct3=2)
char *test_decode_sw() {
    setup();
    RAM = malloc(RAM_SIZE * sizeof(int));
    mu_assert(RAM != NULL, "malloc RAM failed");
    memset(RAM, 0, RAM_SIZE * sizeof(int));
    
    // sw x7, 0x200(x2)  (rs2=7, rs1=2, imm12_merge=0x200)
    // 注意 imm12_merge 的编码方式：低5位在 [11:7]，高7位在 [31:25]
    int imm12 = 0x200;  // 512 字节，字地址 128
    int imm_lo = imm12 & 0x1F;
    int imm_hi = (imm12 >> 5) & 0x7F;
    int insn = (imm_hi << 25) | (7 << 20) | (2 << 15) | (2 << 12) | (imm_lo << 7) | 35;
    GFR[2] = 0;
    GFR[7] = 0xA5A5A5A5;
    GFR[PC] = 0x1000;
    decode(insn);
    int word_addr = 0x200 / 4;  // 128
    mu_assert(RAM[word_addr] == 0xA5A5A5A5, "sw stored wrong value");
    mu_assert(GFR[PC] == 0x1004, "PC update failed");
    
    free(RAM);
    RAM = NULL;
    teardown();
    return NULL;
}

// 测试 jalr 指令 (opcode=103, funct3=0)
char *test_decode_jalr() {
    setup();
    // jalr x1, x2, 0x100  (rd=1, rs1=2, imm12=0x100)
    int imm12 = 0x100;
    int insn = (imm12 << 20) | (2 << 15) | (0 << 12) | (1 << 7) | 103;
    GFR[2] = 0x2000;
    GFR[PC] = 0x1000;
    decode(insn);
    // 目标地址 = (0x2000 + 0x100) & ~1 = 0x2100 & ~1 = 0x2100（最低位清零）
    mu_assert(GFR[PC] == 0x2100, "jalr PC wrong");
    mu_assert(GFR[1] == 0x1004, "jalr return address wrong");
    teardown();
    return NULL;
}

// 测试未知 opcode
char *test_decode_unknown() {
    setup();
    int insn = 0xFFFFFFFF;  // 未知指令
    // 应该输出错误信息，但不会崩溃，返回 0
    int rc = decode(insn);
    mu_assert(rc == 0, "decode should return 0 even for unknown");
    // 由于 PC 未定义行为，我们不做断言，只是确保不崩溃
    teardown();
    return NULL;
}

// 运行所有测试
char *all_tests() {
    mu_suite_start();
    
    mu_run_test(test_init_ram);
    mu_run_test(test_init_rom);
    mu_run_test(test_ram_read_write);
    mu_run_test(test_rom_read);
    mu_run_test(test_decode_add);
    mu_run_test(test_decode_addi);
    mu_run_test(test_decode_lui);
    mu_run_test(test_decode_lw);
    mu_run_test(test_decode_sw);
    mu_run_test(test_decode_jalr);
    mu_run_test(test_decode_unknown);
    
    return NULL;
}

RUN_TESTS(all_tests);
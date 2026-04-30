#include <verilated.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include "Vtop.h"
#include <stdbool.h>
#include <regex.h> 
#include <stdint.h> 
#include <assert.h>
#include <capstone/capstone.h>   // 新增 Capstone 反汇编库
#include "/home/sdark/ysyx-workbench/nemu/include/difftest-def.h"

#define panic(...) do { fprintf(stderr, __VA_ARGS__); exit(1); } while(0)
typedef uint32_t word_t;
#define MEM_BASE 0x80000000
#define MEM_SIZE (32 * 1024 * 1024)

static unsigned char mem[MEM_SIZE];
static int simulation_finished = 0;
static int good_trap = 0;
static int user_quit = 0;           // 用户主动退出标志


// 声明 difftest 接口
extern "C" {
    void difftest_init();
    void difftest_memcpy(unsigned int addr, void *buf, size_t n, int direction);
    void difftest_regcpy(void *dut, int direction);
    void difftest_exec(unsigned long long n);
}


#define DIFFTEST_TO_REF   1   // NPC -> NEMU
#define DIFFTEST_TO_DUT   0   // NEMU -> NPC

// 仿真启动时刻
static auto boot_time = std::chrono::steady_clock::now();

// 全局顶层模块指针，供命令处理函数访问
static Vtop *top = nullptr;

// 当前执行的时钟周期计数
static unsigned long long cycle = 0;

// 保存执行前的 PC 和指令（用于输出反汇编和监视点触发时的正确地址）
static uint32_t pc_before = 0;
static uint32_t inst_before = 0;
static uint32_t lw_before = 0;
static uint32_t lbu_before = 0;
static uint32_t add_before = 0;
static uint32_t addi_before = 0;
static uint32_t jalr_before = 0;
static uint32_t sw_before = 0;
static uint32_t sb_before = 0;
static uint32_t lui_before = 0;

static uint32_t writeback_before = 0; 
static uint32_t io_debug_lsu_rdata_before = 0; 
static uint32_t io_debug_lsu_addr_before = 0;
static uint32_t io_debug_lsu_wdata_before = 0;

// Capstone 反汇编句柄
static csh capstone_handle = 0;

// 获取系统运行微秒数（用于 RTC）
static uint64_t get_uptime_us() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now - boot_time).count();
}

#ifdef __cplusplus
extern "C" {
#endif

// DPI-C 可调用函数：内存读
unsigned int pmem_read(unsigned int addr) {
    if (addr == 0xa0000048) {  // RTC 低32位
        uint64_t us = get_uptime_us();
        return (unsigned int)us;
    }
    if (addr == 0xa000004c) {  // RTC 高32位
        uint64_t us = get_uptime_us();
        return (unsigned int)(us >> 32);
    }
    if (addr < MEM_BASE) return 0;
    unsigned int base = addr & ~3;
    unsigned int offset = base - MEM_BASE;
    if (offset + 3 >= MEM_SIZE) {
        simulation_finished = 1;
        good_trap = 0;
        return 0;
    }
    return (mem[offset+3] << 24) |
           (mem[offset+2] << 16) |
           (mem[offset+1] << 8)  |
            mem[offset];
}

// DPI-C 可调用函数：内存写
void pmem_write(unsigned int addr, unsigned int data, unsigned char mask) {

    //printf("[PMEM_WRITE] cycle=%llu addr=0x%08x data=0x%08x mask=0x%02x\n", cycle, addr, data, mask);

    if (addr == 0x10000000) {  // 串口输出
        if (mask & 0x1) {
            putchar((char)(data & 0xFF));
            fflush(stdout);
        }
        return;
    }
    if (addr < MEM_BASE) return;
    unsigned int base = addr & ~3;
    unsigned int offset = base - MEM_BASE;
    int is_byte = (mask == 0x1 || mask == 0x2 || mask == 0x4 || mask == 0x8);
    for (int i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            mem[offset + i] = is_byte ? (data & 0xFF) : ((data >> (i * 8)) & 0xFF);
        }
    }
}

// ebreak 回调
void sim_finish(void) {
    simulation_finished = 1;
    good_trap = 1;
}

#ifdef __cplusplus
}
#endif

// 装载二进制程序到内存
void load_program(const char *filename, unsigned int base_addr) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("[ERROR] Cannot open %s\n", filename);
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned int offset = base_addr - MEM_BASE;
    if (offset + size > MEM_SIZE) {
        printf("[ERROR] Program too large\n");
        fclose(f);
        exit(1);
    }
    size_t n = fread(&mem[offset], 1, size, f);
    if (n != (size_t)size) {
        printf("[ERROR] Failed to read program file\n");
        fclose(f);
        exit(1);
    }
    fclose(f);
    printf("[INFO] Loaded %ld bytes to 0x%08x\n", size, base_addr);
}

// ========================================================调试部分======================================================================
bool check_watchpoints(void);
void info_watchpoints(void);
static int watchpoint_hit = 0;

// 反汇编辅助函数
static void disassemble_instruction(uint32_t pc, uint32_t inst, char *buffer, size_t buf_size) {
    if (!capstone_handle) {
        snprintf(buffer, buf_size, "(disasm unavailable)");
        return;
    }
    cs_insn *insn = NULL;
    size_t count = cs_disasm(capstone_handle, (uint8_t*)&inst, sizeof(inst), pc, 1, &insn);
    if (count > 0) {
        snprintf(buffer, buf_size, "%s %s", insn[0].mnemonic, insn[0].op_str);
        cs_free(insn, count);
    } else {
        snprintf(buffer, buf_size, ".word 0x%08x", inst);
    }
}

static void print_debug_info() {
    printf("---- Cycle %llu ----\n", cycle);
    printf("PC = 0x%08x (before execution)\n", pc_before);
    printf("INST = 0x%08x\n", inst_before);
    char disasm[128];
    disassemble_instruction(pc_before, inst_before, disasm, sizeof(disasm));
    printf("Instruction: %s\n", disasm);
    printf("lw=%d,lbu=%d,add=%d,addi=%d,sw=%d,sb=%d,jalr=%d,lui=%d\n", lw_before, lbu_before,add_before,addi_before,sw_before,sb_before,jalr_before,lui_before);
    printf("Writebackdata= 0x%08x\n",writeback_before);
    printf("lsu_read_data = 0x%08x\n", io_debug_lsu_rdata_before);
    printf("lsu_addr = 0x%08x\n", io_debug_lsu_addr_before);
    printf("lsu_write_data = 0x%08x\n", io_debug_lsu_wdata_before);
    printf("\n");
}

// 执行一个完整的时钟周期（低->高->低），并更新全局 cycle
static int exec_one_cycle() {
    if (simulation_finished || user_quit) return 0;

    // 记录执行前的状态
    pc_before = top->io_debug_pc;
    inst_before = top->io_debug_inst;

    lw_before = top->io_debug_is_lw;
    lbu_before = top->io_debug_is_lbu;
    add_before = top->io_debug_is_add;
    addi_before = top->io_debug_is_addi;
    jalr_before = top->io_debug_is_jalr;
    sw_before = top->io_debug_is_sw;
    sb_before = top->io_debug_is_sb;
    lui_before = top->io_debug_is_lui;

    writeback_before = top->io_debug_wbData;
    io_debug_lsu_rdata_before = top->io_debug_lsu_rdata;
    io_debug_lsu_addr_before = top->io_debug_lsu_addr;
    io_debug_lsu_wdata_before = top->io_debug_lsu_wdata;



    top->clock = 0; top->eval();
    top->clock = 1; top->eval();
    cycle++;

    //========================状态对比=========================
    difftest_exec(1);   

    uint32_t nemu_state[17];   // 前16个是GPR
    difftest_regcpy(nemu_state, DIFFTEST_TO_DUT);

    uint32_t npc_regs[16] = {
        top->io_debug_regs_0,  top->io_debug_regs_1,
        top->io_debug_regs_2,  top->io_debug_regs_3,
        top->io_debug_regs_4,  top->io_debug_regs_5,
        top->io_debug_regs_6,  top->io_debug_regs_7,
        top->io_debug_regs_8,  top->io_debug_regs_9,
        top->io_debug_regs_10, top->io_debug_regs_11,
        top->io_debug_regs_12, top->io_debug_regs_13,
        top->io_debug_regs_14, top->io_debug_regs_15
    };

    for (int i = 0; i < 16; i++) {
        if (npc_regs[i] != nemu_state[i]) {
            printf("\n[DIFFTEST] Register mismatch at cycle %llu\n", cycle);
            printf("Reg[%d]: NPC = 0x%08x, NEMU = 0x%08x\n", i, npc_regs[i], nemu_state[i]);
            print_debug_info();
            assert(0);
        }
    }
    
    //============================状态对比======================

    if (check_watchpoints()) {
        watchpoint_hit = 1;
        return 1;   // 监视点触发，停止后续执行
    }
    return 0;
}

// 连续执行直到结束或用户中断（用于 c 命令）
static void exec_continue() {
    printf("Continuing execution...\n");
    watchpoint_hit = 0;
    while (!simulation_finished && !Verilated::gotFinish() && !user_quit) {
        if (exec_one_cycle()) break;
    }
    if (watchpoint_hit) {
        printf("Watchpoint triggered, stopping at cycle %llu\n", cycle);
        //打印当前状态
        print_debug_info();
    }
}

// =====================================功能函数==============================================
//==============寄存器访问===============
const char *regs[] = {
    "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5"
};

static void isa_reg_display() {
    for (int i = 0; i < 16; i++) {
        uint32_t val;
        switch(i) {
            case 0: val = top->io_debug_regs_0; break;
            case 1: val = top->io_debug_regs_1; break;
            case 2: val = top->io_debug_regs_2; break;
            case 3: val = top->io_debug_regs_3; break;
            case 4: val = top->io_debug_regs_4; break;
            case 5: val = top->io_debug_regs_5; break;
            case 6: val = top->io_debug_regs_6; break;
            case 7: val = top->io_debug_regs_7; break;
            case 8: val = top->io_debug_regs_8; break;
            case 9: val = top->io_debug_regs_9; break;
            case 10: val = top->io_debug_regs_10; break;
            case 11: val = top->io_debug_regs_11; break;
            case 12: val = top->io_debug_regs_12; break;
            case 13: val = top->io_debug_regs_13; break;
            case 14: val = top->io_debug_regs_14; break;
            case 15: val = top->io_debug_regs_15; break;
            default: val = 0;
        }
        printf("%s = 0x%x (%d)\n", regs[i], val, val);  
    }
}

static uint32_t isa_reg_str2val(const char *s, bool *success) {
    if (strcmp(s, "pc") == 0 || strcmp(s, "PC") == 0) {
        *success = true;
        return top->io_debug_pc;
    }
    for (int i = 0; i < 16; i++) {
        if (strcmp(regs[i], s) == 0) {
            *success = true;
            uint32_t val;
            switch(i) {
                case 0: val = top->io_debug_regs_0; break;
                case 1: val = top->io_debug_regs_1; break;
                case 2: val = top->io_debug_regs_2; break;
                case 3: val = top->io_debug_regs_3; break;
                case 4: val = top->io_debug_regs_4; break;
                case 5: val = top->io_debug_regs_5; break;
                case 6: val = top->io_debug_regs_6; break;
                case 7: val = top->io_debug_regs_7; break;
                case 8: val = top->io_debug_regs_8; break;
                case 9: val = top->io_debug_regs_9; break;
                case 10: val = top->io_debug_regs_10; break;
                case 11: val = top->io_debug_regs_11; break;
                case 12: val = top->io_debug_regs_12; break;
                case 13: val = top->io_debug_regs_13; break;
                case 14: val = top->io_debug_regs_14; break;
                case 15: val = top->io_debug_regs_15; break;
                default: val = 0;
            }
            return val;
        }
    }
    printf("寄存器不存在\n");
    *success = false;
    return 0;
}

//==============表达式求值===================
enum {
    TK_NOTYPE = 256,  // 
    TK_EQ,            // == 双等号 257
    TK_DEC,           // 十进制整数 258
    TK_ADD,           // 加号 + 259
    TK_SUB,           // 减号 - 260
    TK_NEG,           // 负号 -
    TK_MUL,           // 乘号 * 
    TK_DIV,           // 除号 / 
    TK_LPAREN,        // 左括号 ( 
    TK_RPAREN,        // 右括号 ) 
    TK_DEREF,         // 解引用 *
    TK_NEQ,           // !=
    TK_AND,           // &&
    TK_HEX,           // 十六进制数 (以0x开头)
    TK_REG            // 寄存器 (以$开头)
};

static struct rule {
    const char *regex;
    int token_type;
} rules[] = {
    // 多字符运算符和特殊 token 优先匹配
    {"\\(unsigned\\)", TK_NOTYPE},      // 忽略类型转换标记  <-- 新增
    {"0[xX][0-9a-fA-F]+u", TK_HEX},      // 带u后缀的十六进制数
    {"0[xX][0-9a-fA-F]+", TK_HEX},      // 十六进制数
    {"[0-9]+u", TK_DEC},                 // 带u后缀的十进制数
    {"[0-9]+", TK_DEC},                   // 普通十进制数
    {"\\$[a-zA-Z0-9_]+", TK_REG},        // 寄存器，如 $eax
    {"==", TK_EQ},                        // 等于
    {"!=", TK_NEQ},                       // 不等于
    {"&&", TK_AND},                       // 逻辑与
    {"\\+", TK_ADD},                      // 加号
    {"\\-", TK_SUB},                      // 减号
    {"\\*", TK_MUL},                      // 乘号（也可能是解引用，后面处理）
    {"\\/", TK_DIV},                      // 除号
    {"\\(", TK_LPAREN},                   // 左括号
    {"\\)", TK_RPAREN},                   // 右括号
    {" +", TK_NOTYPE},                    // 空格（忽略）
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

void init_regex() {
    int i;
    char error_msg[128];
    int ret;
    for (i = 0; i < NR_REGEX; i ++) {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0) {
            regerror(ret, &re[i], error_msg, 128);
            panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

typedef struct token {
    int type;
    char str[128];
} Token;

static Token tokens[1024] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;
int stage = 0;

static bool make_token(char *e) {                                           
    int position = 0;                                                            
    int i;                                                                         
    regmatch_t pmatch;                                                                  
    nr_token = 0;                                                                           
    while (e[position] != '\0') {                                                               
        for (i = 0; i < NR_REGEX; i ++) {                                                             
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {             
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;
                position += substr_len;
                if (rules[i].token_type != TK_NOTYPE) {
                    int copy_len = substr_len;
                    if (nr_token >= 1023) { printf("token数组越界，保证输入小于1023个字符\n"); return false; }
                    
                    //数字去掉末尾的 'u'，寄存器去掉开头的 '$'
                    if (rules[i].token_type == TK_DEC && copy_len > 0 && substr_start[copy_len-1] == 'u') {
                        copy_len--;
                    } else if (rules[i].token_type == TK_HEX && copy_len > 0 && substr_start[copy_len-1] == 'u') {
                        copy_len--;
                    } else if (rules[i].token_type == TK_REG && copy_len > 0 && substr_start[0] == '$') {
                        substr_start++;
                        copy_len--;
                    }
                    if (copy_len >= 127) { printf("单个字符过长，保证单字符串小于128字符\n"); return false; }
                    strncpy(tokens[nr_token].str, substr_start, copy_len);                 
                    tokens[nr_token].str[copy_len] = '\0';
                    tokens[nr_token].type = rules[i].token_type;
                    nr_token++;
                }
                break;
            }
        }
        if (i == NR_REGEX) {
            printf("匹配不到符号\n");
            return false;
        }
    }
    /* 重新遍历一遍tokens，区分负号和减号，上面是统一识别为减号的，同时区分乘号和解引用 */
    for (int j = 0; j < nr_token; j++) {
        if (tokens[j].type == TK_SUB) {
            if ((j == 0) ||
                (tokens[j-1].type == TK_ADD || tokens[j-1].type == TK_SUB || 
                 tokens[j-1].type == TK_MUL || tokens[j-1].type == TK_DIV ||
                 tokens[j-1].type == TK_EQ || tokens[j-1].type == TK_NEQ ||
                 tokens[j-1].type == TK_AND) || 
                (tokens[j-1].type == TK_LPAREN) ||
                (tokens[j-1].type == TK_NEG)) {
                tokens[j].type = TK_NEG;
            }
        } else if (tokens[j].type == TK_MUL) {
            if (j == 0 ||
                tokens[j-1].type == TK_ADD || tokens[j-1].type == TK_SUB ||
                tokens[j-1].type == TK_MUL || tokens[j-1].type == TK_DIV ||
                tokens[j-1].type == TK_EQ || tokens[j-1].type == TK_NEQ ||
                tokens[j-1].type == TK_AND || 
                tokens[j-1].type == TK_LPAREN ||
                tokens[j-1].type == TK_NEG || tokens[j-1].type == TK_DEREF) {
                tokens[j].type = TK_DEREF;
            }
        }
    }
    return true;
}

static word_t eval(int l, int r, bool *success) {
    if (l > r) {
        *success = false;
        return 0;
    }
    // 单个 token
    if (l == r) {
        Token *t = &tokens[l];
        switch (t->type) {
            case TK_DEC: {
                char *endptr;
                word_t val = strtoul(t->str, &endptr, 10);
                if (*endptr != '\0') {
                    *success = false;
                    return 0;
                }
                return val;
            }
            case TK_HEX: {
                char *endptr;
                word_t val = strtoul(t->str, &endptr, 16);
                if (*endptr != '\0') {
                    *success = false;
                    return 0;
                }
                return val;
            }
            case TK_REG: {
                bool reg_success = true;
                word_t val = isa_reg_str2val(t->str, &reg_success);
                if (!reg_success) {
                    *success = false;
                    return 0;
                }
                return val;
            }
            default:
                *success = false;
                return 0;
        }
    }
    // 检查是否被一对括号完全包裹
    if (tokens[l].type == TK_LPAREN && tokens[r].type == TK_RPAREN) {
        int level = 0;
        int i;
        for (i = l; i <= r; i++) {
            if (tokens[i].type == TK_LPAREN) level++;
            else if (tokens[i].type == TK_RPAREN) level--;
            if (level == 0 && i < r) break;
            if (i == r && level == 0) {
                return eval(l + 1, r - 1, success);
            }
        }
    }
    // 查找不在括号内的最低优先级二元运算符
    int op_pos = -1;
    int op_type = -1;
    int prio_groups[][4] = {
        {TK_AND, 0},
        {TK_EQ, TK_NEQ, 0},
        {TK_ADD, TK_SUB, 0},
        {TK_MUL, TK_DIV, 0},
    };
    int group_count = sizeof(prio_groups) / sizeof(prio_groups[0]);
    for (int g = 0; g < group_count; g++) {
        int level = 0;
        int last_op = -1;
        for (int i = l; i <= r; i++) {
            if (tokens[i].type == TK_LPAREN) {
                level++;
            } else if (tokens[i].type == TK_RPAREN) {
                level--;
            } else if (level == 0) {
                for (int k = 0; prio_groups[g][k] != 0; k++) {
                    if (tokens[i].type == prio_groups[g][k]) {
                        last_op = i;
                        break;
                    }
                }
            }
        }
        if (last_op != -1) {
            op_pos = last_op;
            op_type = tokens[last_op].type;
            break;
        }
    }
    if (op_pos != -1) {
        word_t left_val = eval(l, op_pos - 1, success);
        if (!*success) return 0;
        switch (op_type) {
            case TK_ADD: {
                word_t right_val = eval(op_pos + 1, r, success);
                if (!*success) return 0;
                word_t result = left_val + right_val;
                return result;
            }
            case TK_SUB: {
                word_t right_val = eval(op_pos + 1, r, success);
                if (!*success) return 0;
                word_t result = left_val - right_val;
                return result;
            }
            case TK_MUL: {
                word_t right_val = eval(op_pos + 1, r, success);
                if (!*success) return 0;
                word_t result = left_val * right_val;
                return result;
            }
            case TK_DIV: {
                word_t right_val = eval(op_pos + 1, r, success);
                if (!*success) return 0;
                if (right_val == 0) {
                    printf("除法错误：除数为零\n");
                    *success = false;
                    return 0;
                }
                word_t result = left_val / right_val;
                return result;
            }
            case TK_EQ: {
                word_t right_val = eval(op_pos + 1, r, success);
                if (!*success) return 0;
                word_t result = (left_val == right_val);
                return result;
            }
            case TK_NEQ: {
                word_t right_val = eval(op_pos + 1, r, success);
                if (!*success) return 0;
                word_t result = (left_val != right_val);
                return result;
            }
            case TK_AND: {
                if (left_val == 0) {
                    return 0;
                }
                word_t right_val = eval(op_pos + 1, r, success);
                if (!*success) return 0;
                word_t result = (right_val != 0) ? 1 : 0;
                return result;
            }
            default:
                *success = false;
                return 0;
        }
    }
    // 处理一元运算符（负号、解引用）
    if (tokens[l].type == TK_NEG || tokens[l].type == TK_DEREF) {
        word_t sub_val = eval(l + 1, r, success);
        if (!*success) return 0;
        if (tokens[l].type == TK_NEG) {
            word_t result = 0 - sub_val;
            return result;
        } else { // TK_DEREF
            if ((sub_val % 4) != 0) {
                printf("非法解引用, 但也不是不能解（\n");
            }
            word_t result = pmem_read(sub_val);
            return result;
        }
    }
    *success = false;
    return 0;
}

word_t expr(char *e, bool *success) {
    if (!make_token(e)) {
        *success = false;
        return 0;
    }
    *success = true;
    return eval(0, nr_token - 1, success);
}

static void cleanup_regex() {
    for (int i = 0; i < NR_REGEX; i++) {
        regfree(&re[i]);
    }
}

//=====================================监视点==========================================
typedef struct watchpoint {
    int NO;
    uint32_t result;
    bool result_en;
    struct watchpoint *next;
    char exp[514];
} WP;

#define NR_WP 32

static WP wp_pool[NR_WP] = {};          
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
    int i;
    for (i = 0; i < NR_WP; i ++) {
        wp_pool[i].NO = i;
        wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
    }
    head = NULL;
    free_ = wp_pool;
}

WP* new_wp(const char* exp) {
    //参数检查
    if (exp == NULL) {
        printf("错误：表达式为空\n");
        return NULL;
    }
    //检查空闲节点
    if (free_ == NULL) {
        printf("错误：无空闲监视点，请先删除一些\n");
        return NULL;
    }
    //检查表达式长度
    size_t exp_len = strlen(exp);
    if (exp_len >= sizeof(wp_pool[0].exp)) {
        printf("错误：表达式过长（最大 %zu 字符）\n", sizeof(wp_pool[0].exp) - 1);
        return NULL;
    }
    //表达式求值
    bool success = false;
    uint32_t val = (uint32_t)expr((char *)exp, &success);
    if (!success) {
        printf("错误：表达式求值失败，表达式 = \"%s\"\n", exp);
        return NULL;
    }
    //从空闲链表取下节点
    WP *gained = free_;
    free_ = free_->next;
    gained->next = NULL;
    //初始化节点内容
    gained->result = val;
    gained->result_en = true;
    strncpy(gained->exp, exp, sizeof(gained->exp) - 1);
    gained->exp[sizeof(gained->exp) - 1] = '\0';
    //将节点插入head链表
    if (head == NULL) {
        head = gained;
    } else {
        WP *tail = head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = gained;
    }
    return gained;
}

void free_wp(WP *wp){
    if (wp == NULL) {
        printf("监视点呢？\n");
        assert(0);
    }
    /*从head里删链表*/
    if (head == wp) {
        head = wp->next;
        wp->next = NULL;
    } else {
        WP *check_next = head;
        while (check_next->next != wp && check_next->next != NULL) {
            check_next = check_next->next;
        }
        if (check_next->next == NULL) {
            printf("不存在该监视点\n");
            assert(0);
        }
        if (wp->next == NULL) {
            wp->next = NULL;
            check_next->next = NULL;
        } else {
            WP *node_tobe_connected = wp->next;
            wp->next = NULL;
            check_next->next = node_tobe_connected;
        }
    }
    /*清空内容*/
    wp->result = 0;
    wp->result_en = false;
    memset(wp->exp, 0, sizeof(wp->exp));
    /*把链表接回free，头插*/
    if (free_ == NULL) {
        free_ = wp;
    } else {
        WP *node_tobe_connected = free_;
        free_ = wp;
        wp->next = node_tobe_connected;
    }
}

bool check_watchpoints() {
    if (head == NULL) {
        return false;
    }
    
    WP *wp = head;
    int triggered = 0;
    while (wp != NULL) {
        bool success;
        uint32_t new_val = expr(wp->exp, &success);
        if (!success) {
            printf("警告：监视点 %d 表达式求值失败，后续不会触发监视点\n", wp->NO);
            wp = wp->next;
            continue;
        }
        if (new_val != wp->result) {
            // 使用保存的 before PC 和指令进行输出，并加上反汇编
            char disasm[128];
            disassemble_instruction(pc_before, inst_before, disasm, sizeof(disasm));
            printf("Watchpoint %d hit at PC = 0x%08x\n", wp->NO, pc_before);
            printf("Instruction: %s\n", disasm);
            printf("Expression: %s\n", wp->exp);
            printf("Old value: %u (0x%x)\n", wp->result, wp->result);
            printf("New value: %u (0x%x)\n", new_val, new_val);
            wp->result = new_val;
            triggered = 1;
        }
        wp = wp->next;
    }
    return triggered;
}

/* 打印所有使用中的监视点信息 */
void info_watchpoints() {
    if (head == NULL) {
        printf("没有监视点\n");
        return;
    }
    printf("编号\t表达式\t\t当前值\n");
    WP *wp = head;
    while (wp != NULL) {
        printf("%d\t%s\t\t%u (0x%x)\n", wp->NO, wp->exp, wp->result, wp->result);
        wp = wp->next;
    }
}

/* 根据编号删除监视点 */
void delete_watchpoint(int no) {
    WP *wp = head;
    while (wp != NULL) {
        if (wp->NO == no) {
            free_wp(wp);
            return;
        }
        wp = wp->next;
    }
    printf("未找到编号为 %d 的监视点\n", no);
}

// =====================================命令处理========================================
static int cmd_si(char *args) {
    unsigned long long n = 1;
    if (args != NULL) {
        char *end;
        unsigned long long val = strtoull(args, &end, 0);
        if (*end == '\0') n = val;
        else printf("Invalid step count, using 1\n");
    }
    printf("Executing %llu cycle(s)...\n", n);
    for (unsigned long long i = 0; i < n; i++) {
        if (simulation_finished || user_quit) break;
        exec_one_cycle();
        print_debug_info();
    }
    return 0;
}

static int cmd_c(char *args) {
    exec_continue();
    return 0;
}

static int cmd_q(char *args) {
    printf("Quit simulation.\n");
    user_quit = 1;
    return -1;
}

static int cmd_info(char *args) {
    char arg = '\0';
    if (args == NULL) {
        printf("参数呢？？？？\n");
        return 0;
    } else {
        if (sscanf(args, "%c", &arg) != 1) {
            printf("参数格式错误，请输入 r 或者 w\n");
            return 0;
        } else {
            if (arg == 'r') {
                isa_reg_display();
                return 0;
            } else if (arg == 'w') {
                info_watchpoints(); 
                return 0;
            } else {
                printf("未知命令，请输入r或者w\n");
                return 0;
            }
        }
    }
}

static int cmd_p(char *args) {
    if (args == NULL || strlen(args) == 0) {
        printf("请输入表达式\n");
        return 0;
    }
    bool success;
    word_t val = expr(args, &success);
    if (success) {
        printf("0x%x (%u)\n", val, val);
    } else {
        printf("求值失败.\n");
    }
    return 0;
}

static int cmd_w(char *args) {
    if (args == NULL || strlen(args) == 0) {
        printf("Usage: w <expression>\n");
        return 0;
    }
    WP *wp = new_wp(args);
    if (wp != NULL) {
        printf("Watchpoint %d added: %s\n", wp->NO, wp->exp);
    }
    return 0;
}

static int cmd_d(char *args) {
    if (args == NULL || strlen(args) == 0) {
        printf("Usage: d <watchpoint number>\n");
        return 0;
    }
    int no = atoi(args);
    delete_watchpoint(no);
    return 0;
}

static int cmd_help(char *args);
static int cmd_x(char *args) {
    int n;          
    uint32_t addr;  
    if (sscanf(args, "%d %x", &n, &addr) != 2) {
        printf("命令格式错误，正确格式：x 个数 十六进制地址（例：x 10 0x80000000）\n");
        return 0;
    }
    if (n <= 0) {
        printf("个数必须是正整数！\n");
        return 0;
    }
    printf("内存扫描：起始地址 0x%x，共%d个字(个4字节)\n", addr, n);
    printf("地址          数值（16进制）\n");
    printf("-----------------------------\n");
    for (int i = 0; i < n; i++) {
        word_t data = pmem_read(addr + i*4);
        printf("0x%08x    0x%08x\n", addr + i*4, data);
    }
    return 0;
}

// 命令表
static struct {
    const char *name;
    const char *desc;
    int (*handler)(char *);
} cmd_table[] = {
    { "help", "Show this help", cmd_help },
    { "si",   "Step N cycles (default 1)", cmd_si },
    { "c",    "Continue execution", cmd_c },
    { "q",    "Quit", cmd_q },
    { "info", "Show info (r:regs, w:watchpoints) [placeholder]", cmd_info },
    { "w",    "设置监视点 w <expr>", cmd_w },
    { "d",    "删除对应的监视点 d N", cmd_d },
    { "x",    "Examine memory [placeholder]", cmd_x },
    { "p",    "Evaluate expression [placeholder]", cmd_p },
};
#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
    if (args == NULL) {
        for (unsigned i = 0; i < NR_CMD; i++) {
            printf("%s - %s\n", cmd_table[i].name, cmd_table[i].desc);
        }
    } else {
        for (unsigned i = 0; i < NR_CMD; i++) {
            if (strcmp(args, cmd_table[i].name) == 0) {
                printf("%s - %s\n", cmd_table[i].name, cmd_table[i].desc);
                return 0;
            }
        }
        printf("Unknown command '%s'\n", args);
    }
    return 0;
}

// ==========================================交互主循环============================================== 
static void interactive_loop() {
    char line[256];
    printf("Entering interactive debugger. Type 'help' for commands.\n");
    while (!simulation_finished && !user_quit && !Verilated::gotFinish()) {
        printf("(sim) ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        line[strcspn(line, "\n")] = '\0';

        char *cmd = strtok(line, " ");
        if (cmd == NULL) continue;
        char *args = strtok(NULL, "");   
        // 注意：args 可能为 NULL

        int i;
        for (i = 0; i < NR_CMD; i++) {
            if (strcmp(cmd, cmd_table[i].name) == 0) {
                if (cmd_table[i].handler(args) < 0)
                    return;
                break;
            }
        }
        if (i == NR_CMD) {
            printf("Unknown command '%s'\n", cmd);
        }
    }
}

// ---------- 主函数 ----------
int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    init_regex();
    init_wp_pool();
    memset(mem, 0, MEM_SIZE);
    if (argc < 2) {
        printf("Usage: %s <program.bin>\n", argv[0]);
        return 1;
    }
    load_program(argv[1], MEM_BASE);

    difftest_init();                                            // 初始化 NEMU

    difftest_memcpy(MEM_BASE, mem, MEM_SIZE, DIFFTEST_TO_REF);  // 同步内存
    unsigned int init_regs[17] = {0};                           // 16个GPR + PC
    init_regs[16] = MEM_BASE;                                   // 初始 PC
    difftest_regcpy(init_regs, DIFFTEST_TO_REF);                // 同步寄存器

    // 初始化 Capstone 反汇编引擎
    if (cs_open(CS_ARCH_RISCV, CS_MODE_RISCV32, &capstone_handle) != CS_ERR_OK) {
        printf("Warning: Capstone initialization failed, disassembly disabled.\n");
        capstone_handle = 0;
    }

    top = new Vtop;
    // 复位序列
    top->reset = 1;
    top->clock = 0; top->eval();
    top->clock = 1; top->eval();
    top->reset = 0;
    top->clock = 0; top->eval();
    printf("[INFO] Reset complete.\n");

    interactive_loop();

    if (simulation_finished) {
        printf("[INFO] Simulation finished after %llu cycles\n", cycle);
        printf(good_trap ? "HIT GOOD TRAP\n" : "HIT BAD TRAP\n");
    } else {
        printf("[INFO] Simulation stopped by user after %llu cycles\n", cycle);
    }

    // 清理 Capstone
    if (capstone_handle) cs_close(&capstone_handle);

    delete top;
    cleanup_regex();
    return 0;
}
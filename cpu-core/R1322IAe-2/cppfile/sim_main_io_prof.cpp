#include <verilated.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include "Vtop.h"

#define MEM_BASE 0x80000000
#define MEM_SIZE (128*1024*1024)

static unsigned char mem[MEM_SIZE];
static int sim_finished = 0, good_trap = 0;
static auto boot = std::chrono::steady_clock::now();

static uint64_t get_us() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - boot).count();
}

#ifdef __cplusplus
extern "C" {
#endif
unsigned int pmem_read(unsigned int addr) {
    if (addr == 0xa0000048) return (unsigned int)get_us();
    if (addr == 0xa000004c) return (unsigned int)(get_us() >> 32);
    if (addr < MEM_BASE) return 0;
    unsigned int base = addr & ~3, off = base - MEM_BASE;
    if (off + 3 >= MEM_SIZE) { sim_finished = 1; good_trap = 0; return 0; }
    return (mem[off+3]<<24)|(mem[off+2]<<16)|(mem[off+1]<<8)|mem[off];
}
void pmem_write(unsigned int addr, unsigned int data, unsigned char mask) {
    if (addr == 0xa00003f8) { if (mask & 1) putchar(data & 0xFF); fflush(stdout); return; }
    if (addr < MEM_BASE) return;
    unsigned int base = addr & ~3, off = base - MEM_BASE;
    if (off + 3 >= MEM_SIZE) { sim_finished = 1; good_trap = 0; return; }
    for (int i = 0; i < 4; i++) if (mask & (1<<i)) mem[off + i] = (data >> (i*8)) & 0xFF;
}
void sim_finish(void) { sim_finished = 1; good_trap = 1; }
#ifdef __cplusplus
}
#endif

struct Prof {
    unsigned long long cycles = 0;
    unsigned long long d_valid = 0;
    unsigned long long d_stall = 0;
    unsigned long long st_load = 0, st_csr = 0, st_mem = 0, st_other = 0;
    unsigned long long single = 0;
    unsigned long long sg_raw = 0, sg_ctl1 = 0, sg_mem = 0, sg_ctl2 = 0;
    unsigned long long redirect = 0;
    unsigned long long pred_redirect = 0;
    unsigned long long e_valid = 0, m_valid = 0, w_valid = 0;
    unsigned long long retire1 = 0, retire2 = 0;
};

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    memset(mem, 0, MEM_SIZE);
    if (argc < 2) { printf("Usage: %s <program.bin>\n", argv[0]); return 1; }
    FILE *f = fopen(argv[1], "rb");
    if (!f) { printf("[ERROR] Cannot open %s\n", argv[1]); return 1; }
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
    size_t _r = fread(&mem[0], 1, sz, f); (void)_r; fclose(f);

    Vtop *top = new Vtop;
    top->reset = 1;
    top->clock = 0; top->eval();
    top->clock = 1; top->eval();
    top->reset = 0;
    top->clock = 0; top->eval();

    Prof p;
    while (!sim_finished && !Verilated::gotFinish()) {
        top->clock = 0; top->eval();
        top->clock = 1; top->eval();
        p.cycles++;

        if (top->io_debug_d_valid) p.d_valid++;
        if (top->io_debug_d_stall) {
            p.d_stall++;
            if (top->io_debug_stall_load)      p.st_load++;
            else if (top->io_debug_stall_csr)  p.st_csr++;
            else if (top->io_debug_stall_mem)  p.st_mem++;
            else                               p.st_other++;
        }
        if (top->io_debug_single_issue) {
            p.single++;
            if (top->io_debug_single_raw)       p.sg_raw++;
            else if (top->io_debug_single_ctl1) p.sg_ctl1++;
            else if (top->io_debug_single_mem)  p.sg_mem++;
            else if (top->io_debug_single_ctl2) p.sg_ctl2++;
        }
        if (top->io_debug_exu_redirect) p.redirect++;
        if (top->io_debug_pred_redirect) p.pred_redirect++;
        if (top->io_debug_e_valid) p.e_valid++;
        if (top->io_debug_m_valid) p.m_valid++;
        if (top->io_debug_w_valid) p.w_valid++;
        if (top->io_debug_retire1) p.retire1++;
        if (top->io_debug_retire2) p.retire2++;
    }

    unsigned long long instr = p.retire1 + p.retire2;
    printf("\n================== R1322IAe-2 alutest 瓶颈计数 ==================\n");
    printf("total cycles            = %llu\n", p.cycles);
    printf("D 有包 (packets in D)   = %llu  (%.1f%%)\n", p.d_valid, 100.0*p.d_valid/p.cycles);
    printf("D 空泡 (front bubble)   = %llu  (%.1f%%)\n", p.cycles - p.d_valid, 100.0*(p.cycles-p.d_valid)/p.cycles);
    printf("  E 有效 / M 有效 / W 有效 = %llu / %llu / %llu\n", p.e_valid, p.m_valid, p.w_valid);
    printf("retire: lane1=%llu lane2=%llu  指令数=%llu\n", p.retire1, p.retire2, instr);
    if (p.cycles) printf("IPC = %.3f   CPI = %.3f   每包 = %.2f cycles\n",
                          (double)instr/p.cycles, (double)p.cycles/instr,
                          p.d_valid ? (double)p.cycles/p.d_valid : 0.0);
    printf("\n---- D 停顿 (interlock) 共 %llu 拍 ----\n", p.d_stall);
    printf("  load-use    = %5llu (%.1f%%)\n", p.st_load, p.d_stall?100.0*p.st_load/p.d_stall:0);
    printf("  CSR         = %5llu (%.1f%%)\n", p.st_csr,  p.d_stall?100.0*p.st_csr/p.d_stall:0);
    printf("  store->load = %5llu (%.1f%%)\n", p.st_mem,  p.d_stall?100.0*p.st_mem/p.d_stall:0);
    printf("  其他        = %5llu\n", p.st_other);
    printf("\n---- 单发射 (包内冲突) 共 %llu 次 ----\n", p.single);
    printf("  包内 RAW      = %5llu (%.1f%%)\n", p.sg_raw,  p.single?100.0*p.sg_raw/p.single:0);
    printf("  lane1 控制    = %5llu (%.1f%%)\n", p.sg_ctl1, p.single?100.0*p.sg_ctl1/p.single:0);
    printf("  双 store      = %5llu (%.1f%%)\n", p.sg_mem,  p.single?100.0*p.sg_mem/p.single:0);
    printf("  lane2 控制    = %5llu (%.1f%%)\n", p.sg_ctl2, p.single?100.0*p.sg_ctl2/p.single:0);
    printf("\n---- 控制重定向: EXU(预测错/trap) = %llu 次, 译码级预测重定向 = %llu 次 ----\n",
           p.redirect, p.pred_redirect);
    printf("=================================================================\n");

    printf("[INFO] Finished %llu cycles  %s\n", p.cycles, good_trap?"HIT GOOD TRAP":"HIT BAD TRAP");
    delete top;
    return 0;
}

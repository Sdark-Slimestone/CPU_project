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

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    memset(mem, 0, MEM_SIZE);
    if (argc < 2) { printf("Usage: %s <program.bin>\n", argv[0]); return 1; }
    FILE *f = fopen(argv[1], "rb");
    if (!f) { printf("[ERROR] Cannot open %s\n", argv[1]); return 1; }
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
    fread(&mem[0], 1, sz, f); fclose(f);

    Vtop *top = new Vtop;
    top->reset = 1;
    top->clock = 0; top->eval();
    top->clock = 1; top->eval();
    top->reset = 0;
    top->clock = 0; top->eval();

    unsigned long long cycle = 0;
    unsigned long long stall_ev = 0, stall_cyc = 0; unsigned prev = 0;
    unsigned long long r_raw = 0, r_ctl1 = 0, r_mem = 0, r_ctl2 = 0;
    unsigned long long occ_sum = 0, occ_max = 0, occ_ge2 = 0, occ_ge3 = 0, occ_hist[8] = {0};
    unsigned long long retired = 0;
    unsigned long long d_stall = 0, redirects = 0, qempty = 0, pop1 = 0;
    while (!sim_finished && !Verilated::gotFinish()) {
        top->clock = 0; top->eval();
        top->clock = 1; top->eval();
        unsigned ds = top->io_debug_stall;
        if (ds) {
            stall_cyc++;
            unsigned rs = top->io_debug_stall_reason;
            if (rs & 1) r_raw++;
            if (rs & 2) r_ctl1++;
            if (rs & 4) r_mem++;
            if (rs & 8) r_ctl2++;
        }
        if (ds && !prev) stall_ev++;
        prev = ds;
        unsigned occ = top->io_debug_occupancy;
        occ_sum += occ; if (occ > occ_max) occ_max = occ; if (occ >= 2) occ_ge2++; if (occ >= 3) occ_ge3++;
        if (occ < 8) occ_hist[occ]++;
        if (top->io_debug_retire) retired++;
        if (top->io_debug_d_stall) d_stall++;
        if (top->io_debug_redirect) redirects++;
        if (top->io_debug_qempty) qempty++;
        if (top->io_debug_pop1) pop1++;
        cycle++;
    }
    printf("[INFO] Finished %llu cycles  %s  final_stall_events=%llu  raw=%llu ctl1=%llu mem=%llu ctl2=%llu\n",
           cycle, good_trap?"HIT GOOD TRAP":"HIT BAD TRAP", stall_ev, r_raw, r_ctl1, r_mem, r_ctl2);
    printf("[OCC] avg=%.2f max=%llu  cycles_ge2=%llu(%.1f%%) cycles_ge3=%llu(%.1f%%)\n",
           (double)occ_sum/cycle, occ_max, occ_ge2, 100.0*occ_ge2/cycle, occ_ge3, 100.0*occ_ge3/cycle);
    printf("[RET] retired_packets=%llu  cycles/packet=%.3f\n", retired, retired?(double)cycle/retired:0.0);
    printf("[IPC] d_stall=%llu redirect=%llu qempty=%llu single_issue(pop1)=%llu  (of %llu cycles)\n",
           d_stall, redirects, qempty, pop1, cycle);
    printf("[OCC] hist: 0=%llu 1=%llu 2=%llu 3=%llu 4=%llu 5=%llu\n",
           occ_hist[0],occ_hist[1],occ_hist[2],occ_hist[3],occ_hist[4],occ_hist[5]);
    delete top;
    return 0;
}
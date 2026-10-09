#include <verilated.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <algorithm>
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
    unsigned long long ctl = 0, mispred = 0, njalr = 0, njal = 0, nbr = 0;
    // 控制流分类: 0=无 1=条件分支 2=jal 3=jalr返回 4=jalr非返回
    unsigned long long cls[5] = {0};
    unsigned long long cls_misp[5] = {0};    // 方向预测错
    unsigned long long cls_tgtmis[5] = {0};  // 方向对但目标错
    unsigned long long cls_l2[5] = {0};      // 其中 lane2 的条数
    unsigned long long cls_misp_l2[5] = {0}; // 其中 lane2 的方向错
    // 条件分支方向细分
    unsigned long long cb_taken = 0, cb_taken_misp = 0;
    unsigned long long cb_nt = 0, cb_nt_misp = 0;
    unsigned long long gsh_ok = 0, loc_ok = 0, stat_ok = 0;  // 三分量各自对 (条件分支)
    // 条件分支每 PC 热点: pc -> {次数, 方向错}
    std::unordered_map<unsigned, std::pair<unsigned, unsigned>> pcstat;
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

    if (argc >= 3 && argv[2][0]) {
        char buf[58]; memset(buf, 0, sizeof(buf));
        strncpy(buf, argv[2], 57);
        for (long i = 0; i <= (long)(sz - 57); i++) {
            if (memcmp(&mem[i], "the_insert-arg_rule_in_Makefile_will_insert_mainargs_here", 57) == 0) {
                memcpy(&mem[i], buf, 57);
                printf("[INFO] mainargs=%s\n", argv[2]);
                break;
            }
        }
    }

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
        if (top->io_debug_exu_ctl) p.ctl++;
        if (top->io_debug_exu_mispred) p.mispred++;
        if (top->io_debug_exu_jalr) p.njalr++;
        if (top->io_debug_exu_jal) p.njal++;
        if (top->io_debug_exu_br) p.nbr++;
        if (top->io_debug_exu_ctl) {
            unsigned c = top->io_debug_ctl_class;
            bool mis = top->io_debug_exu_mispred;
            if (c < 5) {
                p.cls[c]++;
                if (mis) p.cls_misp[c]++;
                if (top->io_debug_ctl_lane2) {
                    p.cls_l2[c]++;
                    if (mis) p.cls_misp_l2[c]++;
                }
            }
            if (c == 1) {  // 条件分支方向诊断
                bool act = top->io_debug_exu_actual;
                if (act) { p.cb_taken++;    if (mis) p.cb_taken_misp++; }
                else     { p.cb_nt++;       if (mis) p.cb_nt_misp++; }
                if (top->io_debug_exu_gsh_pred    == act) p.gsh_ok++;
                if (top->io_debug_exu_loc_pred    == act) p.loc_ok++;
                if (top->io_debug_exu_static_pred == act) p.stat_ok++;
                unsigned pc = (unsigned)top->io_debug_exu_pc;
                auto &e = p.pcstat[pc];
                e.first++; if (mis) e.second++;
            }
        }
        if (top->io_debug_exu_tgt_mis) {
            unsigned c = top->io_debug_tgtmis_class;
            if (c < 5) p.cls_tgtmis[c]++;
        }
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
    printf("---- 分支预测: 控制流指令=%llu (jal=%llu jalr=%llu 条件分支=%llu)\n", p.ctl, p.njal, p.njalr, p.nbr);
    printf("     方向预测错=%llu, 准确率=%.2f%%;  排除 jalr 后准确率=%.2f%% ----\n",
           p.mispred, p.ctl?100.0*(p.ctl-p.mispred)/p.ctl:0.0,
           (p.ctl-p.njalr)?100.0*((p.ctl-p.njalr)-(p.mispred-p.njalr))/(p.ctl-p.njalr):0.0);

    printf("\n---- 控制流分类准确率 ----\n");
    const char *cn[5] = {"", "条件分支", "jal", "jalr-返回", "jalr-非返回"};
    for (int c = 1; c <= 4; c++) {
        unsigned long long n = p.cls[c], m = p.cls_misp[c], t = p.cls_tgtmis[c];
        double dir_acc = n ? 100.0 * (double)(n - m) / n : 0.0;
        double all_acc = n ? 100.0 * (double)(n - m - t) / n : 0.0;
        unsigned long long n2 = p.cls_l2[c], m2 = p.cls_misp_l2[c];
        unsigned long long n1 = n - n2, m1 = m - m2;
        printf("  %-11s n=%6llu 方向错=%5llu 目标错=%5llu 方向准确率=%6.2f%% 综合=%6.2f%% | lane1 n=%6llu 错=%5llu %6.2f%% | lane2 n=%6llu 错=%5llu %6.2f%%\n",
               cn[c], n, m, t, dir_acc, all_acc,
               n1, m1, n1 ? 100.0*(double)(n1-m1)/n1 : 0.0,
               n2, m2, n2 ? 100.0*(double)(n2-m2)/n2 : 0.0);
    }
    {
        unsigned long long n = p.cls[3] + p.cls[4], m = p.cls_misp[3] + p.cls_misp[4],
                           t = p.cls_tgtmis[3] + p.cls_tgtmis[4];
        double dir_acc = n ? 100.0 * (double)(n - m) / n : 0.0;
        double all_acc = n ? 100.0 * (double)(n - m - t) / n : 0.0;
        printf("  %-11s n=%6llu  方向错=%5llu  目标错=%5llu  方向准确率=%6.2f%%  综合(含目标)=%6.2f%%\n",
               "jalr合计", n, m, t, dir_acc, all_acc);
    }
    printf("=================================================================\n");

    // ---- 条件分支方向细分 ----
    unsigned long long cb = p.cls[1];
    if (cb) {
        printf("\n---- 条件分支方向细分 (n=%llu) ----\n", cb);
        printf("  taken     n=%6llu 方向错=%6llu 准确率=%6.2f%%\n",
               p.cb_taken, p.cb_taken_misp,
               p.cb_taken ? 100.0*(double)(p.cb_taken-p.cb_taken_misp)/p.cb_taken : 0.0);
        printf("  not-taken n=%6llu 方向错=%6llu 准确率=%6.2f%%\n",
               p.cb_nt, p.cb_nt_misp,
               p.cb_nt ? 100.0*(double)(p.cb_nt-p.cb_nt_misp)/p.cb_nt : 0.0);
        printf("  分量准确率: gshare=%6.2f%%  局部=%6.2f%%  静态=%6.2f%%  (选择器输出=%6.2f%%)\n",
               100.0*(double)p.gsh_ok/cb, 100.0*(double)p.loc_ok/cb,
               100.0*(double)p.stat_ok/cb,
               100.0*(double)(cb-p.cls_misp[1])/cb);

        // 每 PC 热点 (按方向错数降序取前 15)
        std::vector<std::pair<unsigned, std::pair<unsigned,unsigned>>> v(p.pcstat.begin(), p.pcstat.end());
        std::sort(v.begin(), v.end(), [](const auto &a, const auto &b){
            return a.second.second > b.second.second;
        });
        printf("  --- 方向错最多的 PC (前15) ---\n");
        printf("  %-12s %8s %8s %8s\n", "PC", "命中", "错", "准确率");
        for (size_t i = 0; i < v.size() && i < 15; i++) {
            unsigned pc = v[i].first, n = v[i].second.first, m = v[i].second.second;
            if (m == 0) break;
            printf("  0x%08x %8u %8u %7.2f%%\n", pc, n, m, 100.0*(double)(n-m)/n);
        }
        printf("  (不同 PC 总数 = %zu)\n", p.pcstat.size());
    }

    printf("[INFO] Finished %llu cycles  %s\n", p.cycles, good_trap?"HIT GOOD TRAP":"HIT BAD TRAP");
    delete top;
    return 0;
}

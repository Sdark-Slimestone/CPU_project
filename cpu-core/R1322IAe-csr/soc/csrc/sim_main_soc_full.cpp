// ysyxSoC 仿真 testbench (Verilator), 顶层 ysyxSoCFull —— 带外设引脚。
// 相比 sim_main_soc.cpp (顶层 ysyxSoCTop, 仅 clock/reset), 本 TB 直接驱动/观测
// GPIO / PS2 / VGA / UART 引脚, 用于在没有 NVBoard GUI 的环境下验证外设控制器。
//   - PS2: 用 YS2_INJECT_PS2 注入逗号分隔的十六进制扫描码字节, 模拟键盘。
//   - VGA: 统计有效像素/帧, 可与 YS2_VGA_DUMP 指定的文件比对。
//   - UART RX: 用 YS2_INJECT_UART 注入字符串 (按控制器波特率发串行位)。
#include <verilated.h>
#include "VysyxSoCFull.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>

#define MROM_BASE 0x20000000
#define MROM_SIZE 0x1000
#define FLASH_BASE 0x30000000
#define FLASH_SIZE 0x100000

static uint8_t mrom[MROM_SIZE];
static uint8_t flash[FLASH_SIZE];
static volatile int sim_finished = 0;
static volatile int sim_haltcode = -1;

extern "C" void flash_read(int32_t addr, int32_t *data) {
    uint32_t off = (uint32_t)addr & (FLASH_SIZE - 1); off &= ~3u;
    *data = (int32_t)(flash[off] | (flash[off + 1] << 8) |
                      (flash[off + 2] << 16) | (flash[off + 3] << 24));
}
extern "C" void mrom_read(int32_t raddr, int32_t *rdata) {
    uint32_t a = (uint32_t)raddr & 0xfffffffcU;
    if (a >= 0x20000ff0U && a <= 0x20000ffcU) {
        sim_finished = 1; sim_haltcode = (int)((a >> 2) & 0x3U); *rdata = 0; return;
    }
    uint32_t off = (uint32_t)raddr & (MROM_SIZE - 1); off &= ~3u;
    *rdata = mrom[off] | (mrom[off + 1] << 8) | (mrom[off + 2] << 16) | (mrom[off + 3] << 24);
}

// ---------------- PS/2 键盘发送器 ----------------
struct Ps2Sender {
    std::vector<uint8_t> q; size_t idx = 0;
    bool enabled = false; long cyc = 0;
    long start_delay = 0, gap = 0;
    bool active = false; int bit = 0, sub = 0; uint16_t bits = 0;
    int clk = 1, data = 1;
    void load(const char *s) {
        if (!s) return;
        enabled = true;
        const char *p = s;
        while (*p) {
            while (*p == ',' || *p == ' ') p++;
            if (!*p) break;
            q.push_back((uint8_t)strtol(p, (char **)&p, 16));
        }
    }
    void setdelay(const char *s) { if (s) start_delay = atol(s); }
    void tick() {
        cyc++;
        if (!enabled || cyc < start_delay) { clk = 1; data = 1; return; }
        if (gap > 0) { gap--; clk = 1; data = 1; return; }
        if (!active) {
            if (idx >= q.size()) { clk = 1; data = 1; return; }
            uint8_t b = q[idx++]; int par = 0;
            // 模仿 NVBoard 发送器: 每字节前有一个空闲(1)下降沿, 帧 = 空闲(1)+起始(0)+8数据+校验+停止
            bits = 1;                                  // bit0 = 前导空闲(1)
            for (int i = 0; i < 8; i++) { int v = (b >> i) & 1; bits |= (v << (2 + i)); par ^= v; }
            bits |= (par << 10); bits |= (1 << 11);    // bit1=起始(0), bit2..9=数据, bit10=校验, bit11=停止
            active = true; bit = 0; sub = 0;
        }
        data = (bits >> bit) & 1;
        clk = (sub < 8) ? 1 : 0;
        if (++sub >= 16) { sub = 0; if (++bit >= 12) { active = false; gap = 20000; } }
    }
};

// ---------------- 串口 RX 发送器 ----------------
// 从 uart16550 控制器角度看, 需要在 rx 上按位发送。这里的位速率取一个固定值,
// 由 YS2_UART_BIT_CYCLES 指定 (默认 16), 与控制器内部采样率配合。
struct UartSender {
    std::string s; size_t idx = 0; int bit = -1;
    bool active = false; uint16_t frame = 0; int clk = 1; int sub = 0;
    bool enabled = false; long cyc = 0, start_delay = 0;
    int period = 16;
    void load(const char *str) {
        if (!str) return;
        enabled = true;
        for (const char *p = str; *p; p++) s.push_back(*p);
    }
    void tick() {
        cyc++;
        if (!enabled || cyc < start_delay) { clk = 1; return; }
        if (!active) {
            if (idx >= s.size()) { clk = 1; return; }
            uint8_t b = (uint8_t)s[idx++]; int par = 0; frame = 0;
            for (int i = 0; i < 8; i++) { int v = (b >> i) & 1; frame |= (v << (1 + i)); par ^= v; }
            frame |= (par << 9); frame |= (1 << 10);
            active = true; bit = 0; sub = 0;
        }
        clk = (frame >> bit) & 1;
        if (++sub >= period) { sub = 0; if (++bit >= 11) { active = false; } }
    }
};

// ---------------- VGA 采集 ----------------
// 采集"最近一帧完整画面": 每次 vsync 下降沿若上一帧已收满 640*480 个有效像素, 则保存为
// vga_frame; 随后清空重新采集。仿真结束时 vga_frame 保存最近一帧完整画面。
static std::vector<uint32_t> vga_frame, vga_cur;
static long vga_valid_cnt = 0;
static bool vga_capture = false;
static int  vga_vsync_prev = 1;
static long vga_start_cycle = 0;
static long vga_cyc = 0;
static long vga_frames = 0;      // 完成的 VGA 帧数
static long vga_changes = 0;     // 与上一帧不同的帧数 (画面真正变化)
static uint64_t vga_last_hash = 0;
static long vga_last_change_cyc = 0;

static uint64_t hash_frame(const std::vector<uint32_t> &v) {
    uint64_t h = 1469598103934665603ULL;
    for (size_t i = 0; i < v.size(); i += 64) { h ^= (uint64_t)(v[i] & 0xffffff); h *= 1099511628211ULL; }
    return h ^ v.size();
}

static void vga_sample(VysyxSoCFull *top) {
    if (!vga_capture) return;
    vga_cyc++;
    int vs = top->externalPins_vga_vsync;
    bool newframe = (vga_vsync_prev == 1 && vs == 0);
    vga_vsync_prev = vs;
    if (newframe && vga_cyc >= vga_start_cycle) {
        if (vga_cur.size() >= 307200) {
            uint64_t h = hash_frame(vga_cur);
            vga_frames++;
            if (h != vga_last_hash) { vga_changes++; vga_last_change_cyc = vga_cyc; }
            vga_last_hash = h;
            vga_frame = vga_cur;              // 保存最近完整帧
        }
        vga_cur.clear();
    }
    if (top->externalPins_vga_valid) {
        uint32_t col = (top->externalPins_vga_r << 16) |
                       (top->externalPins_vga_g << 8)  | top->externalPins_vga_b;
        vga_valid_cnt++;
        if (vga_cyc >= vga_start_cycle) vga_cur.push_back(col);
    }
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    setbuf(stdout, NULL);

    memset(mrom, 0, sizeof(mrom));
    memset(flash, 0, sizeof(flash));
    int boot_flash = getenv("YSYX_BOOT_FLASH") != NULL;
    if (argc >= 2) {
        FILE *f = fopen(argv[1], "rb");
        if (!f) { printf("[ERROR] cannot open %s\n", argv[1]); return 1; }
        if (boot_flash) { size_t n = fread(flash, 1, FLASH_SIZE, f); printf("[INFO] loaded %zu bytes into FLASH\n", n); }
        else            { size_t n = fread(mrom, 1, MROM_SIZE, f);   printf("[INFO] loaded %zu bytes into MROM\n", n); }
        fclose(f);
    }
    if (!boot_flash) {
        for (uint32_t off = 0; off + 4 <= FLASH_SIZE; off += 4) {
            uint32_t v = FLASH_BASE + off;
            flash[off] = v & 0xff; flash[off+1] = (v>>8)&0xff; flash[off+2] = (v>>16)&0xff; flash[off+3] = (v>>24)&0xff;
        }
    }
    if (const char *ff = getenv("YSYX_FLASH_FILE")) {
        FILE *f = fopen(ff, "rb");
        if (f) { size_t n = fread(flash, 1, FLASH_SIZE, f); fclose(f); printf("[INFO] loaded %zu bytes into FLASH\n", n); }
    }

    Ps2Sender ps2;   ps2.load(getenv("YSYX_INJECT_PS2"));   ps2.setdelay(getenv("YSYX_PS2_DELAY"));
    UartSender urx;  urx.load(getenv("YSYX_INJECT_UART"));
    if (const char *d = getenv("YSYX_UART_DELAY")) urx.start_delay = atol(d);
    if (const char *d = getenv("YSYX_UART_PERIOD")) urx.period = atoi(d);
    vga_capture = getenv("YSYX_VGA_CAPTURE") != NULL;
    if (const char *s = getenv("YSYX_VGA_START")) vga_start_cycle = atol(s);

    VysyxSoCFull *top = new VysyxSoCFull;
    // 复位保持 >10 拍 (ChipLink/SoC 要求)。
    top->clock = 0; top->reset = 1;
    top->externalPins_gpio_in = 0;
    top->externalPins_ps2_clk = 1; top->externalPins_ps2_data = 1;
    top->externalPins_uart_rx = 1;
    top->eval();
    for (int i = 0; i < 20; i++) { top->clock = 1; top->eval(); top->clock = 0; top->eval(); }
    top->reset = 0; top->eval();

    const long MAX_CYCLES = getenv("YSYX_MAX_CYCLES") ? atol(getenv("YSYX_MAX_CYCLES")) : 20000000;
    long c = 0;
    for (; c < MAX_CYCLES && !Verilated::gotFinish() && !sim_finished; c++) {
        ps2.tick(); urx.tick();
        top->externalPins_ps2_clk = ps2.clk;
        top->externalPins_ps2_data = ps2.data;
        top->externalPins_uart_rx = urx.clk;
        top->externalPins_gpio_in = 0;
        top->clock = 0; top->eval();
        vga_sample(top);
        top->clock = 1; top->eval();
        if ((c % 20000000L) == 0 && c != 0) {
            long nz = 0; for (uint32_t p : vga_cur) if (p & 0xffffff) nz++;
            printf("[PROG] %ld cycles, frames=%ld changes=%ld lastchange=%ld cur_nz=%ld\n",
                   c, vga_frames, vga_changes, vga_last_change_cyc, nz);
        }
    }
    printf("[INFO] finished %ld cycles, vga_valid=%ld\n", c, vga_valid_cnt);
    if (sim_haltcode == 0)      printf("[HIT GOOD TRAP]\n");
    else if (sim_haltcode > 0)  printf("[HIT BAD TRAP] code=%d\n", sim_haltcode);
    else                        printf("[INFO] timeout (no halt)\n");

    const char *dump = getenv("YSYX_VGA_DUMP");
    if (vga_capture && dump) {
        FILE *f = fopen(dump, "wb");
        if (f) { fwrite(vga_frame.data(), 4, vga_frame.size(), f); fclose(f); printf("[INFO] dumped %zu vga pixels\n", vga_frame.size()); }
    }
    top->final();
    delete top;
    return 0;
}

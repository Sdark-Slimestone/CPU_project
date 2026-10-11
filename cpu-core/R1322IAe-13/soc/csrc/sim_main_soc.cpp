// ysyxSoC 仿真 testbench (Verilator)。
// CPU = ysyx_26030107 (接入在 ysyxSoCFull.v 内), 顶层 = ysyxSoCTop。
// 程序通过 MROM (0x2000_0000) 存取: 用 DPI-C mrom_read 提供 MROM 内容 (从文件加载)。
#include <verilated.h>
#include "VysyxSoCTop.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#define MROM_BASE 0x20000000
#define MROM_SIZE 0x1000   // 4 KiB
#define FLASH_BASE 0x30000000
#define FLASH_SIZE 0x100000  // 1 MiB (模拟 flash 颗粒内容)

static uint8_t mrom[MROM_SIZE];
static uint8_t flash[FLASH_SIZE];
static volatile int sim_finished = 0;
static volatile int sim_haltcode = -1;

// 讲义要求: 提供 flash_read / mrom_read 供链接。
// flash_read: FAST_FLASH 路径 (spi_top_apb.v) 直接调用它读 flash 内容。
extern "C" void flash_read(int32_t addr, int32_t *data) {
    uint32_t off = (uint32_t)addr & (FLASH_SIZE - 1);
    off &= ~3u;
    *data = (int32_t)(flash[off] | (flash[off + 1] << 8) |
                      (flash[off + 2] << 16) | (flash[off + 3] << 24));
}
extern "C" void mrom_read(int32_t raddr, int32_t *rdata) {
    // MROM 末尾的 magic 字: 程序 halt() 读它, 用于结束仿真。
    // 地址 [0x20000ff0, 0x20000ffc] 低 2 位编码 halt code (字对齐)。
    uint32_t a = (uint32_t)raddr & 0xfffffffcU;
    if (a >= 0x20000ff0U && a <= 0x20000ffcU) {
        sim_finished = 1; sim_haltcode = (int)((a >> 2) & 0x3U); *rdata = 0; return;
    }
    uint32_t off = (uint32_t)raddr & (MROM_SIZE - 1);
    off &= ~3u;   // 返回字对齐数据; 窄读的字节 lane 由 CPU 用 addr[1:0] 选择
    *rdata = mrom[off] | (mrom[off + 1] << 8) | (mrom[off + 2] << 16) | (mrom[off + 3] << 24);
}

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);   // 解决 plusargs 报错
    setbuf(stdout, NULL);                 // 串口 $write 实时输出 (不缓冲)

    memset(mrom, 0, sizeof(mrom));
    memset(flash, 0, sizeof(flash));
    int boot_flash = getenv("YSYX_BOOT_FLASH") != NULL;
    if (argc >= 2) {
        FILE *f = fopen(argv[1], "rb");
        if (!f) { printf("[ERROR] cannot open %s\n", argv[1]); return 1; }
        if (boot_flash) {
            size_t n = fread(flash, 1, FLASH_SIZE, f);
            printf("[INFO] loaded %zu bytes into FLASH\n", n);
        } else {
            size_t n = fread(mrom, 1, MROM_SIZE, f);
            printf("[INFO] loaded %zu bytes into MROM\n", n);
        }
        fclose(f);
    }

    if (!boot_flash) {
        // 模拟往 flash 颗粒烧录: flash 中 CPU 地址 A 处的字 = A。
        for (uint32_t off = 0; off + 4 <= FLASH_SIZE; off += 4) {
            uint32_t v = FLASH_BASE + off;
            flash[off]     = (uint8_t)(v & 0xff);
            flash[off + 1] = (uint8_t)((v >> 8) & 0xff);
            flash[off + 2] = (uint8_t)((v >> 16) & 0xff);
            flash[off + 3] = (uint8_t)((v >> 24) & 0xff);
        }
    }
    // 可选: 把一个文件烧录到 flash (模拟往 flash 颗粒中写入程序)
    if (const char *ff = getenv("YSYX_FLASH_FILE")) {
        FILE *f = fopen(ff, "rb");
        if (f) {
            size_t n = fread(flash, 1, FLASH_SIZE, f);
            fclose(f);
            printf("[INFO] loaded %zu bytes into FLASH (from %s)\n", n, ff);
        }
    }

    VysyxSoCTop *top = new VysyxSoCTop;
    // reset 需保持 >10 拍: SoC 里 CPU 复位是 reset || shift(reset,10), 太短会多出一个延迟复位脉冲。
    top->reset = 1; top->clock = 0; top->eval();
    for (int i = 0; i < 20; i++) { top->clock = 1; top->eval(); top->clock = 0; top->eval(); }
    top->reset = 0; top->clock = 0; top->eval();

    const long MAX_CYCLES = getenv("YSYX_MAX_CYCLES") ? atol(getenv("YSYX_MAX_CYCLES")) : 20000000;
    long c = 0;
    for (; c < MAX_CYCLES && !Verilated::gotFinish() && !sim_finished; c++) {
        top->clock = 0; top->eval();
        top->clock = 1; top->eval();
    }
    printf("[INFO] finished %ld cycles\n", c);
    if (sim_haltcode == 0)        printf("[HIT GOOD TRAP]\n");
    else if (sim_haltcode > 0)    printf("[HIT BAD TRAP] code=%d\n", sim_haltcode);
    else                          printf("[INFO] timeout (no halt)\n");
    top->final();
    delete top;
    return 0;
}

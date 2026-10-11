#include <nvboard.h>
#include <VysyxSoCFull.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

static VysyxSoCFull dut;

void nvboard_bind_all_pins(VysyxSoCFull *top);

#define MROM_SIZE  0x1000
#define FLASH_SIZE 0x100000
static uint8_t mrom[MROM_SIZE];
static uint8_t flash[FLASH_SIZE];

// ysyxSoC 的 MROM/SPI 控制器通过 DPI-C 读取存储器内容。
extern "C" void mrom_read(int32_t raddr, int32_t *rdata) {
    uint32_t off = (uint32_t)raddr & (MROM_SIZE - 1); off &= ~3u;
    *rdata = mrom[off] | (mrom[off + 1] << 8) | (mrom[off + 2] << 16) | (mrom[off + 3] << 24);
}
extern "C" void flash_read(int32_t addr, int32_t *data) {
    uint32_t off = (uint32_t)addr & (FLASH_SIZE - 1); off &= ~3u;
    *data = flash[off] | (flash[off + 1] << 8) | (flash[off + 2] << 16) | (flash[off + 3] << 24);
}

static void single_cycle() {
    dut.clock = 0; dut.eval();
    dut.clock = 1; dut.eval();
}

int main(int argc, char **argv) {
    memset(mrom, 0, sizeof(mrom));
    memset(flash, 0, sizeof(flash));
    const char *prog = (argc >= 2) ? argv[1] : getenv("YSYX_MROM");
    int boot_flash = getenv("YSYX_BOOT_FLASH") != NULL;
    if (prog) {
        FILE *f = fopen(prog, "rb");
        if (f) {
            if (boot_flash) {
                size_t n = fread(flash, 1, FLASH_SIZE, f);
                printf("[INFO] loaded %zu bytes into FLASH\n", n);
            } else {
                size_t n = fread(mrom, 1, MROM_SIZE, f);
                printf("[INFO] loaded %zu bytes into MROM\n", n);
            }
            fclose(f);
        }
    }

    nvboard_bind_all_pins(&dut);
    nvboard_init();

    dut.clock = 0; dut.reset = 1;
    dut.externalPins_gpio_in = 0;
    dut.externalPins_ps2_clk = 1; dut.externalPins_ps2_data = 1;
    dut.externalPins_uart_rx = 1;
    dut.eval();
    for (int i = 0; i < 20; i++) single_cycle();
    dut.reset = 0;

    while (1) {
        nvboard_update();
        single_cycle();
    }
    return 0;
}

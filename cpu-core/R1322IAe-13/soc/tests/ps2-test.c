// PS/2 键盘直接测试 (MROM 启动, 链接到地址 0)。
// 轮询 PS/2 数据寄存器 (0x1001_1000), 每读到一个扫描码就通过 UART 打印 <xx>。
#define UART_BASE 0x10000000L
#define PS2_BASE  0x10011000L

static void putch(char c) {
  volatile unsigned char *u = (volatile unsigned char *)UART_BASE;
  while ((u[5] & 0x20) == 0) ;
  u[0] = c;
}
static void phex(unsigned char v) {
  unsigned char hi = v >> 4, lo = v & 0xf;
  putch(hi < 10 ? '0' + hi : 'a' + hi - 10);
  putch(lo < 10 ? '0' + lo : 'a' + lo - 10);
}

void _start() {
  volatile unsigned char *u = (volatile unsigned char *)UART_BASE;
  u[3] = 0x80; u[0] = 1; u[1] = 0; u[3] = 0x03; u[2] = 0x07;  // UART 初始化
  volatile unsigned char *ps2 = (volatile unsigned char *)PS2_BASE;
  for (;;) {
    unsigned char c = *ps2;
    if (c) { putch('<'); phex(c); putch('>'); }
  }
}

// VGA 帧缓冲直接测试 (MROM 启动, 链接到地址 0)。
// 往帧缓冲 (0x2100_0000, 每像素 4 字节) 写几个已知像素, 然后空转。
// TB 采集 VGA 有效像素流并与期望比较。
#define FB 0x21000000L
void _start() {
  volatile unsigned int *fb = (volatile unsigned int *)FB;
  fb[0]                   = 0x00ff0000;   // (0,0)     红
  fb[320]                 = 0x00ff00ff;   // (320,0)   品红
  fb[640 * 240 + 320]     = 0x0000ff00;   // (320,240) 绿
  fb[640 * 479 + 639]     = 0x000000ff;   // (639,479) 蓝
  while (1);
}

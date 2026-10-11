void _start() {
  *(volatile int  *)0x0f000000L = 0x12345678;   // SRAM (AXI4 direct)
  *(volatile char *)0x10000000L = 'A';          // UART (APB)
  *(volatile char *)0x10000000L = '\n';
  while (1);
}

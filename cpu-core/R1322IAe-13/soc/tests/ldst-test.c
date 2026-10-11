void _start() {
  volatile int  *s = (volatile int  *)0x0f000000L;   // SRAM (可写)
  volatile char *u = (volatile char *)0x10000000L;   // UART
  s[0] = 0x12345678; s[1] = 0x0abcdef0;
  int a = s[0], b = s[1];
  *u = (a == 0x12345678 && b == (int)0x0abcdef0) ? 'A' : 'X';
  *u = '\n';
  while (1);
}

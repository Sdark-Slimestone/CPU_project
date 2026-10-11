void _start() {
  volatile char *u = (volatile char *)0x10000000L;
  int s;
  asm volatile(
    "li t0, 1\n"
    "li t1, 0\n"
    "1:\n"
    "add t1, t1, t0\n"
    "addi t0, t0, 1\n"
    "li t2, 11\n"
    "blt t0, t2, 1b\n"
    "mv %0, t1\n"
    : "=r"(s) :: "t0", "t1", "t2");
  *u = (s == 55) ? 'A' : 'X';
  *u = '\n';
  while (1);
}

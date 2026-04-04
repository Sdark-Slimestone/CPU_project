#ifndef architecture_h
#define architecture_h

#include <stdio.h>

#define RAM_SIZE_BYTES 1048576
#define ROM_SIZE_BYTES 1048576
#define RAM_SIZE 262144
#define ROM_SIZE 262144

extern int *RAM;
extern int *ROM;

//ram 32位
int init_ram(const char *file_path);
int read_ram(int addr);
void write_ram(int addr, int value);

//rom 32位
int init_rom(const char *file_path);
int read_rom(int addr);

//decoder
int decode(int instruction);


#endif  
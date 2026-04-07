// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Prototypes for DPI import and export functions.
//
// Verilator includes this file in all generated .cpp files that use DPI functions.
// Manually include this file where DPI .c import functions are declared to ensure
// the C functions match the expectations of the DPI imports.

#ifndef VERILATED_VTOP__DPI_H_
#define VERILATED_VTOP__DPI_H_  // guard

#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif


    // DPI IMPORTS
    // DPI import at DPI_Memory.v:12:33
    extern int pmem_read(int addr);
    // DPI import at DPI_Memory.v:13:34
    extern void pmem_write(int addr, int data, char mask);
    // DPI import at DPI_Memory.v:14:34
    extern void sim_finish();

#ifdef __cplusplus
}
#endif

#endif  // guard

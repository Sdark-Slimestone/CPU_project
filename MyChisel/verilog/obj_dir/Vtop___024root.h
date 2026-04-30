// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtop.h for the primary calling header

#ifndef VERILATED_VTOP___024ROOT_H_
#define VERILATED_VTOP___024ROOT_H_  // guard

#include "verilated.h"


class Vtop__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtop___024root final {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(clock,0,0);
        VL_IN8(reset,0,0);
        VL_IN8(io_clk,0,0);
        VL_IN8(io_reset,0,0);
        VL_OUT8(io_debug_regWen,0,0);
        VL_OUT8(io_debug_grf_rden,0,0);
        VL_OUT8(io_debug_grf_rdaddr,4,0);
        VL_OUT8(io_debug_is_add,0,0);
        VL_OUT8(io_debug_is_addi,0,0);
        VL_OUT8(io_debug_is_jalr,0,0);
        VL_OUT8(io_debug_is_lui,0,0);
        VL_OUT8(io_debug_is_lbu,0,0);
        VL_OUT8(io_debug_is_lw,0,0);
        VL_OUT8(io_debug_is_sw,0,0);
        VL_OUT8(io_debug_is_sb,0,0);
        VL_OUT8(io_debug_is_ebreak,0,0);
        VL_OUT8(io_debug_lsu_wen,0,0);
        VL_OUT8(io_debug_lsu_wmask,3,0);
        VL_OUT8(io_debug_lsu_ren,0,0);
        CData/*0:0*/ top__DOT___wbu_io_regWen;
        CData/*0:0*/ top__DOT___lsu_io_dmemWen;
        CData/*0:0*/ top__DOT___lsu_io_dmemRen;
        CData/*0:0*/ top__DOT___exu_io_rs2_en;
        CData/*4:0*/ top__DOT___exu_io_rd_out;
        CData/*0:0*/ top__DOT___exu_io_jalr_out;
        CData/*4:0*/ top__DOT___exu_io_rs1_addr_out;
        CData/*4:0*/ top__DOT___exu_io_rs2_addr_out;
        CData/*0:0*/ top__DOT___idu_io_is_addi;
        CData/*0:0*/ top__DOT___idu_io_is_add;
        CData/*0:0*/ top__DOT___idu_io_is_lbu;
        CData/*0:0*/ top__DOT___idu_io_is_lw;
        CData/*0:0*/ top__DOT___idu_io_is_sb;
        CData/*0:0*/ top__DOT___idu_io_is_sw;
        CData/*0:0*/ top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_0;
        CData/*0:0*/ top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_0;
        CData/*0:0*/ top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3;
        CData/*0:0*/ top__DOT__exu__DOT__aguMux__DOT____VdfgRegularize_h679c4643_0_0;
        CData/*0:0*/ top__DOT__exu__DOT__bruMux__DOT___GEN;
        CData/*0:0*/ top__DOT__wbu__DOT___io_regWen_T;
        CData/*0:0*/ top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4;
        CData/*0:0*/ __VdfgRegularize_he50b618e_0_0;
        CData/*0:0*/ __VdfgRegularize_he50b618e_0_1;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VstlPhaseResult;
        CData/*0:0*/ __Vtrigprevexpr___TOP__clock__0;
        CData/*0:0*/ __VactPhaseResult;
        CData/*0:0*/ __VnbaPhaseResult;
        SData/*11:0*/ top__DOT__idu__DOT__infoDecoder__DOT__imm12_pre_extended;
        VL_OUT(io_debug_pc,31,0);
        VL_OUT(io_debug_regs_0,31,0);
        VL_OUT(io_debug_regs_1,31,0);
        VL_OUT(io_debug_regs_2,31,0);
        VL_OUT(io_debug_regs_3,31,0);
        VL_OUT(io_debug_regs_4,31,0);
        VL_OUT(io_debug_regs_5,31,0);
        VL_OUT(io_debug_regs_6,31,0);
        VL_OUT(io_debug_regs_7,31,0);
        VL_OUT(io_debug_regs_8,31,0);
        VL_OUT(io_debug_regs_9,31,0);
        VL_OUT(io_debug_regs_10,31,0);
        VL_OUT(io_debug_regs_11,31,0);
        VL_OUT(io_debug_regs_12,31,0);
        VL_OUT(io_debug_regs_13,31,0);
        VL_OUT(io_debug_regs_14,31,0);
    };
    struct {
        VL_OUT(io_debug_regs_15,31,0);
        VL_OUT(io_debug_alu_src1,31,0);
        VL_OUT(io_debug_alu_src2,31,0);
        VL_OUT(io_debug_alu_res,31,0);
        VL_OUT(io_debug_wbData,31,0);
        VL_OUT(io_debug_grf_input,31,0);
        VL_OUT(io_debug_lsu_addr,31,0);
        VL_OUT(io_debug_lsu_wdata,31,0);
        VL_OUT(io_debug_lsu_rdata,31,0);
        VL_OUT(io_debug_inst,31,0);
        IData/*31:0*/ top__DOT___dataMem_rdata;
        IData/*31:0*/ top__DOT___instMem_rdata;
        IData/*31:0*/ top__DOT___grf_io_rs1out;
        IData/*31:0*/ top__DOT___grf_io_rs2out;
        IData/*31:0*/ top__DOT___wbu_io_wbData;
        IData/*31:0*/ top__DOT___lsu_io_dmemAddr;
        IData/*31:0*/ top__DOT___exu_io_alu_result;
        IData/*31:0*/ top__DOT___idu_io_imm12;
        IData/*31:0*/ top__DOT___ifu_io_pctogrf;
        IData/*31:0*/ top__DOT__ifu__DOT__pcReg;
        IData/*31:0*/ top__DOT__exu__DOT___aluMux_io_source1;
        IData/*31:0*/ top__DOT__exu__DOT___aluMux_io_source2;
        IData/*31:0*/ top__DOT__grf__DOT__regs_0;
        IData/*31:0*/ top__DOT__grf__DOT__regs_1;
        IData/*31:0*/ top__DOT__grf__DOT__regs_2;
        IData/*31:0*/ top__DOT__grf__DOT__regs_3;
        IData/*31:0*/ top__DOT__grf__DOT__regs_4;
        IData/*31:0*/ top__DOT__grf__DOT__regs_5;
        IData/*31:0*/ top__DOT__grf__DOT__regs_6;
        IData/*31:0*/ top__DOT__grf__DOT__regs_7;
        IData/*31:0*/ top__DOT__grf__DOT__regs_8;
        IData/*31:0*/ top__DOT__grf__DOT__regs_9;
        IData/*31:0*/ top__DOT__grf__DOT__regs_10;
        IData/*31:0*/ top__DOT__grf__DOT__regs_11;
        IData/*31:0*/ top__DOT__grf__DOT__regs_12;
        IData/*31:0*/ top__DOT__grf__DOT__regs_13;
        IData/*31:0*/ top__DOT__grf__DOT__regs_14;
        IData/*31:0*/ top__DOT__grf__DOT__regs_15;
        VlWide<16>/*511:0*/ top__DOT__grf__DOT___GEN;
        IData/*31:0*/ top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0;
        IData/*31:0*/ __Vfunc_top__DOT__instMem__DOT__pmem_read__0__Vfuncout;
        IData/*31:0*/ __Vfunc_top__DOT__dataMem__DOT__pmem_read__2__Vfuncout;
        IData/*31:0*/ __VactIterCount;
        VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    };

    // INTERNAL VARIABLES
    Vtop__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vtop___024root(Vtop__Syms* symsp, const char* namep);
    ~Vtop___024root();
    VL_UNCOPYABLE(Vtop___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard

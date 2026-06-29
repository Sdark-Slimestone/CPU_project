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
        CData/*4:0*/ top__DOT___exu_io_rd_out;
        CData/*4:0*/ top__DOT___exu_io_rs1_addr_out;
        CData/*4:0*/ top__DOT___exu_io_rs2_addr_out;
        CData/*0:0*/ top__DOT___idu_io_is_ecall;
        CData/*0:0*/ top__DOT___idu_io_is_ebreak;
        CData/*0:0*/ top__DOT__use_imm_csr;
        CData/*2:0*/ top__DOT____Vcellinp__csr__io_csr_op;
        CData/*0:0*/ top__DOT____Vcellinp__csr__io_csr_wen;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_jalr;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_beq;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_bne;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_blt;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_bge;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_bltu;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_bgeu;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_lb;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_lh;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_lw;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_lbu;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_lhu;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_sb;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_sh;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_sw;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_addi;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_slti;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_sltiu;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_xori;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_ori;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_andi;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_slli;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_srli;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_srai;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_add;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_sub;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_sll;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_slt;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_sltu;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_xor;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_srl;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_sra;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_or;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_and;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_csrrw;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_csrrs;
    };
    struct {
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_csrrc;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_csrrwi;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_csrrsi;
        CData/*0:0*/ top__DOT__idu__DOT___instDecoder_io_is_csrrci;
        CData/*0:0*/ top__DOT__idu__DOT__instDecoder__DOT__is_csr;
        CData/*0:0*/ top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1;
        CData/*0:0*/ top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2;
        CData/*0:0*/ top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3;
        CData/*0:0*/ top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4;
        CData/*0:0*/ top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6;
        CData/*0:0*/ top__DOT__idu__DOT__infoDecoder__DOT___csr_use_rs1_T;
        CData/*0:0*/ top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2;
        CData/*0:0*/ top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3;
        CData/*0:0*/ top__DOT__exu__DOT__use_rs1;
        CData/*0:0*/ top__DOT__exu__DOT____Vcellinp__grfCtrl__io_use_rs2;
        CData/*0:0*/ top__DOT__exu__DOT____Vcellinp__aluMux__io_use_u_imm;
        CData/*0:0*/ top__DOT__exu__DOT____Vcellinp__aluMux__io_use_rs2;
        CData/*3:0*/ top__DOT__exu__DOT____Vcellinp__alu__io_alu_op;
        CData/*0:0*/ top__DOT__exu__DOT____Vcellinp__aguMux__io_is_store;
        CData/*0:0*/ top__DOT__exu__DOT____Vcellinp__aguMux__io_is_load;
        CData/*0:0*/ top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_0;
        CData/*0:0*/ top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_1;
        CData/*0:0*/ top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_2;
        CData/*0:0*/ top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_3;
        CData/*0:0*/ top__DOT__exu__DOT__bru__DOT___branch_cond_T_9;
        CData/*7:0*/ top__DOT__lsu__DOT__byte_sel;
        CData/*0:0*/ top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_0;
        CData/*0:0*/ top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_3;
        CData/*0:0*/ top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4;
        CData/*0:0*/ top__DOT__csr__DOT__unnamedblk1__DOT___GEN;
        CData/*0:0*/ top__DOT__csr__DOT__unnamedblk1__DOT___GEN_0;
        CData/*0:0*/ top__DOT__csr__DOT__unnamedblk1__DOT___GEN_1;
        CData/*0:0*/ top__DOT__csr__DOT__unnamedblk1__DOT___GEN_2;
        CData/*0:0*/ top__DOT__csr__DOT__unnamedblk1__DOT___GEN_3;
        CData/*0:0*/ top__DOT__csr__DOT__unnamedblk1__DOT___GEN_4;
        CData/*0:0*/ top__DOT__csr__DOT__unnamedblk1__DOT___GEN_5;
        CData/*0:0*/ __VdfgRegularize_he50b618e_0_0;
        CData/*0:0*/ __VdfgRegularize_he50b618e_0_2;
        CData/*0:0*/ __VdfgRegularize_he50b618e_0_3;
        CData/*0:0*/ __VdfgRegularize_he50b618e_0_5;
        CData/*0:0*/ __VdfgRegularize_he50b618e_0_6;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VstlPhaseResult;
        CData/*0:0*/ __Vtrigprevexpr___TOP__clock__0;
        CData/*0:0*/ __VactPhaseResult;
        CData/*0:0*/ __VnbaPhaseResult;
        SData/*15:0*/ top__DOT__lsu__DOT__half_sel;
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
        VL_OUT(io_debug_regs_15,31,0);
    };
    struct {
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
        IData/*31:0*/ top__DOT___csr_io_csr_rdata;
        IData/*31:0*/ top__DOT___grf_io_rs1out;
        IData/*31:0*/ top__DOT___grf_io_rs2out;
        IData/*31:0*/ top__DOT___wbu_io_wbData;
        IData/*31:0*/ top__DOT___exu_io_agu_addr;
        IData/*31:0*/ top__DOT___exu_io_alu_result;
        IData/*31:0*/ top__DOT___exu_io_branch_target;
        IData/*31:0*/ top__DOT___idu_io_imm_i;
        IData/*31:0*/ top__DOT___ifu_io_pctogrf;
        IData/*31:0*/ top__DOT__ifu__DOT__pcReg;
        IData/*31:0*/ top__DOT__exu__DOT___aluMux_io_source1;
        IData/*31:0*/ top__DOT__exu__DOT___aluMux_io_source2;
        IData/*31:0*/ top__DOT__lsu__DOT____VdfgRegularize_hac146698_0_0;
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
        IData/*31:0*/ top__DOT__csr__DOT__mstatus_reg;
        IData/*31:0*/ top__DOT__csr__DOT__mtvec_reg;
        IData/*31:0*/ top__DOT__csr__DOT__mepc_reg;
        IData/*31:0*/ top__DOT__csr__DOT__mcause_reg;
        IData/*31:0*/ top__DOT__csr__DOT__mtval_reg;
        IData/*31:0*/ top__DOT__csr__DOT__mie_reg;
        IData/*31:0*/ top__DOT__csr__DOT__mip_reg;
        IData/*31:0*/ top__DOT__csr__DOT__unnamedblk1__DOT__t_rs1;
        IData/*31:0*/ top__DOT__csr__DOT__unnamedblk1__DOT__csr_write_val;
        IData/*31:0*/ __Vfunc_top__DOT__instMem__DOT__pmem_read__0__Vfuncout;
        IData/*31:0*/ __Vfunc_top__DOT__dataMem__DOT__pmem_read__2__Vfuncout;
        IData/*31:0*/ __VactIterCount;
        QData/*63:0*/ top__DOT__csr__DOT__mcycle_reg;
        QData/*63:0*/ top__DOT__csr__DOT__minstret_reg;
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

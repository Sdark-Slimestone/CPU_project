// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

extern "C" int pmem_read(int addr);

void Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_read_TOP(IData/*31:0*/ addr, IData/*31:0*/ &pmem_read__Vfuncrtn) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_read_TOP\n"); );
    // Body
    int addr__Vcvt;
    addr__Vcvt = addr;
    int pmem_read__Vfuncrtn__Vcvt;
    pmem_read__Vfuncrtn__Vcvt = pmem_read(addr__Vcvt);
    pmem_read__Vfuncrtn = (pmem_read__Vfuncrtn__Vcvt);
}

extern "C" void pmem_write(int addr, int data, char mask);

void Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_write_TOP(IData/*31:0*/ addr, IData/*31:0*/ data, CData/*7:0*/ mask) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_write_TOP\n"); );
    // Body
    int addr__Vcvt;
    addr__Vcvt = addr;
    int data__Vcvt;
    data__Vcvt = data;
    char mask__Vcvt;
    mask__Vcvt = mask;
    pmem_write(addr__Vcvt, data__Vcvt, mask__Vcvt);
}

extern "C" void sim_finish();

void Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__sim_finish_TOP() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__sim_finish_TOP\n"); );
    // Body
    sim_finish();
}

void Vtop___024root___eval_triggers_vec__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers_vec__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((IData)(vlSelfRef.clock) 
                                                     & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clock__0)))));
    vlSelfRef.__Vtrigprevexpr___TOP__clock__0 = vlSelfRef.clock;
}

bool Vtop___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __Vdly__top__DOT__ifu__DOT__pcReg;
    __Vdly__top__DOT__ifu__DOT__pcReg = 0;
    QData/*63:0*/ __Vdly__top__DOT__csr__DOT__minstret_reg;
    __Vdly__top__DOT__csr__DOT__minstret_reg = 0;
    VlWide<16>/*511:0*/ __Vtemp_19;
    // Body
    __Vdly__top__DOT__csr__DOT__minstret_reg = vlSelfRef.top__DOT__csr__DOT__minstret_reg;
    if (vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_store) {
        Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_write_TOP(
                                                                            (0xfffffffcU 
                                                                             & vlSelfRef.top__DOT___exu_io_agu_addr), 
                                                                            ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sb)
                                                                              ? vlSelfRef.top__DOT__lsu__DOT____VdfgRegularize_hac146698_0_0
                                                                              : 
                                                                             ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sh)
                                                                               ? vlSelfRef.top__DOT__lsu__DOT____VdfgRegularize_hac146698_0_0
                                                                               : vlSelfRef.top__DOT___grf_io_rs2out)), 
                                                                            ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sw)
                                                                              ? 0x0000000fU
                                                                              : 
                                                                             (0x0000000fU 
                                                                              & ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sh)
                                                                                 ? 
                                                                                ((0U 
                                                                                == 
                                                                                (3U 
                                                                                & vlSelfRef.top__DOT___exu_io_agu_addr))
                                                                                 ? 3U
                                                                                 : 
                                                                                ((2U 
                                                                                == 
                                                                                (3U 
                                                                                & vlSelfRef.top__DOT___exu_io_agu_addr))
                                                                                 ? 0x0cU
                                                                                 : 0U))
                                                                                 : 
                                                                                ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sb)
                                                                                 ? 
                                                                                ((IData)(1U) 
                                                                                << 
                                                                                (3U 
                                                                                & vlSelfRef.top__DOT___exu_io_agu_addr))
                                                                                 : 0U)))));
    }
    __Vdly__top__DOT__ifu__DOT__pcReg = vlSelfRef.top__DOT__ifu__DOT__pcReg;
    if (vlSelfRef.reset) {
        vlSelfRef.top__DOT__csr__DOT__mcycle_reg = 0ULL;
        __Vdly__top__DOT__csr__DOT__minstret_reg = 0ULL;
        __Vdly__top__DOT__ifu__DOT__pcReg = 0x80000000U;
        vlSelfRef.top__DOT__grf__DOT__regs_0 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_1 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_2 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_3 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_4 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_5 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_6 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_7 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_8 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_9 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_10 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_11 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_12 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_13 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_14 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_15 = 0U;
        vlSelfRef.top__DOT__csr__DOT__mstatus_reg = 0x00001800U;
        vlSelfRef.top__DOT__csr__DOT__mtvec_reg = 0U;
        vlSelfRef.top__DOT__csr__DOT__mepc_reg = 0U;
        vlSelfRef.top__DOT__csr__DOT__mcause_reg = 0U;
        vlSelfRef.top__DOT__csr__DOT__mtval_reg = 0U;
        vlSelfRef.top__DOT__csr__DOT__mie_reg = 0U;
        vlSelfRef.top__DOT__csr__DOT__mip_reg = 0U;
    } else {
        vlSelfRef.top__DOT__csr__DOT__mcycle_reg = 
            (1ULL + vlSelfRef.top__DOT__csr__DOT__mcycle_reg);
        if ((1U & (~ ((IData)(vlSelfRef.top__DOT___idu_io_is_ebreak) 
                      | (IData)(vlSelfRef.top__DOT___idu_io_is_ecall))))) {
            __Vdly__top__DOT__csr__DOT__minstret_reg 
                = (1ULL + vlSelfRef.top__DOT__csr__DOT__minstret_reg);
        }
        if ((1U & (~ (IData)(vlSelfRef.top__DOT___idu_io_is_ebreak)))) {
            __Vdly__top__DOT__ifu__DOT__pcReg = (((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4) 
                                                  & ((0x00c0U 
                                                      == 
                                                      (vlSelfRef.top__DOT___instMem_rdata 
                                                       >> 0x00000014U)) 
                                                     & (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_5)))
                                                  ? vlSelfRef.top__DOT__csr__DOT__mepc_reg
                                                  : 
                                                 (((IData)(vlSelfRef.top__DOT___idu_io_is_ecall) 
                                                   | (IData)(vlSelfRef.top__DOT___idu_io_is_ebreak))
                                                   ? vlSelfRef.top__DOT__csr__DOT__mtvec_reg
                                                   : 
                                                  (((0x6fU 
                                                     == 
                                                     (0x0000007fU 
                                                      & vlSelfRef.top__DOT___instMem_rdata)) 
                                                    | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_jalr) 
                                                       | ((IData)(vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_3) 
                                                          & ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_jalr) 
                                                             | (((((((IData)(vlSelfRef.top__DOT__exu__DOT__bru__DOT___branch_cond_T_9) 
                                                                     << 3U) 
                                                                    | ((vlSelfRef.top__DOT___grf_io_rs1out 
                                                                        < vlSelfRef.top__DOT___grf_io_rs2out) 
                                                                       << 2U)) 
                                                                   | ((VL_GTES_III(32, vlSelfRef.top__DOT___grf_io_rs1out, vlSelfRef.top__DOT___grf_io_rs2out) 
                                                                       << 1U) 
                                                                      | VL_LTS_III(32, vlSelfRef.top__DOT___grf_io_rs1out, vlSelfRef.top__DOT___grf_io_rs2out))) 
                                                                  << 4U) 
                                                                 | ((0x0000000cU 
                                                                     & ((- (IData)((IData)(vlSelfRef.top__DOT__exu__DOT__bru__DOT___branch_cond_T_9))) 
                                                                        << 2U)) 
                                                                    | (((vlSelfRef.top__DOT___grf_io_rs1out 
                                                                         != vlSelfRef.top__DOT___grf_io_rs2out) 
                                                                        << 1U) 
                                                                       | (vlSelfRef.top__DOT___grf_io_rs1out 
                                                                          == vlSelfRef.top__DOT___grf_io_rs2out)))) 
                                                                >> 
                                                                ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_beq)
                                                                  ? 0U
                                                                  : 
                                                                 ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bne)
                                                                   ? 1U
                                                                   : 
                                                                  ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_blt)
                                                                    ? 4U
                                                                    : 
                                                                   ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bge)
                                                                     ? 5U
                                                                     : 
                                                                    ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bltu)
                                                                      ? 6U
                                                                      : 
                                                                     (7U 
                                                                      & (- (IData)((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bgeu))))))))))))))
                                                    ? vlSelfRef.top__DOT___exu_io_branch_target
                                                    : vlSelfRef.top__DOT___ifu_io_pctogrf)));
        }
        vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4 
            = (((IData)(vlSelfRef.top__DOT___wbu_io_regWen) 
                & (~ ((IData)(vlSelfRef.top__DOT___exu_io_rd_out) 
                      >> 4U))) & (0U != (IData)(vlSelfRef.top__DOT___exu_io_rd_out)));
        vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0 
            = (((0U == (0x0000001fU & VL_SHIFTL_III(9,32,32, 
                                                    (0x0000000fU 
                                                     & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)), 5U)))
                 ? 0U : (vlSelfRef.top__DOT__grf__DOT___GEN
                         [(((IData)(0x0000001fU) + 
                            (0x000001ffU & VL_SHIFTL_III(9,32,32, 
                                                         (0x0000000fU 
                                                          & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)), 5U))) 
                           >> 5U)] << ((IData)(0x00000020U) 
                                       - (0x0000001fU 
                                          & VL_SHIFTL_III(9,32,32, 
                                                          (0x0000000fU 
                                                           & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)), 5U))))) 
               | (vlSelfRef.top__DOT__grf__DOT___GEN
                  [(0x0000000fU & (VL_SHIFTL_III(9,32,32, 
                                                 (0x0000000fU 
                                                  & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)), 5U) 
                                   >> 5U))] >> (0x0000001fU 
                                                & VL_SHIFTL_III(9,32,32, 
                                                                (0x0000000fU 
                                                                 & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)), 5U))));
        if ((0U == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_0 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((1U == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_1 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((2U == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_2 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((3U == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_3 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((4U == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_4 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((5U == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_5 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((6U == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_6 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((7U == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_7 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((8U == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_8 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((9U == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_9 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((0x0aU == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_10 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((0x0bU == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_11 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((0x0cU == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_12 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((0x0dU == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_13 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((0x0eU == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_14 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        if ((0x0000000fU == (0x0000000fU & (IData)(vlSelfRef.top__DOT___exu_io_rd_out)))) {
            vlSelfRef.top__DOT__grf__DOT__regs_15 = 
                ((IData)(vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4)
                  ? vlSelfRef.top__DOT___wbu_io_wbData
                  : vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___GEN_0);
        }
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__t_rs1 
            = ((IData)(vlSelfRef.top__DOT__use_imm_csr)
                ? (0x0000001fU & vlSelfRef.top__DOT___grf_io_rs1out)
                : vlSelfRef.top__DOT___grf_io_rs1out);
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__csr_write_val 
            = ((0U == (IData)(vlSelfRef.top__DOT____Vcellinp__csr__io_csr_op))
                ? vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__t_rs1
                : ((1U == (IData)(vlSelfRef.top__DOT____Vcellinp__csr__io_csr_op))
                    ? (vlSelfRef.top__DOT___csr_io_csr_rdata 
                       | vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__t_rs1)
                    : ((2U == (IData)(vlSelfRef.top__DOT____Vcellinp__csr__io_csr_op))
                        ? (vlSelfRef.top__DOT___csr_io_csr_rdata 
                           & (~ vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__t_rs1))
                        : 0U)));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN 
            = (0x0300U == (vlSelfRef.top__DOT___instMem_rdata 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_0 
            = (0x0304U == (vlSelfRef.top__DOT___instMem_rdata 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_1 
            = (0x0305U == (vlSelfRef.top__DOT___instMem_rdata 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_2 
            = (0x0340U == (vlSelfRef.top__DOT___instMem_rdata 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_3 
            = (0x0341U == (vlSelfRef.top__DOT___instMem_rdata 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_4 
            = (0x0342U == (vlSelfRef.top__DOT___instMem_rdata 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_5 
            = (0x0343U == (vlSelfRef.top__DOT___instMem_rdata 
                           >> 0x00000014U));
        if (((IData)(vlSelfRef.top__DOT____Vcellinp__csr__io_csr_wen) 
             & (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN))) {
            vlSelfRef.top__DOT__csr__DOT__mstatus_reg 
                = vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__csr_write_val;
        }
        if ((1U & (~ ((((~ (IData)(vlSelfRef.top__DOT____Vcellinp__csr__io_csr_wen)) 
                        | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN)) 
                       | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_0)) 
                      | (~ (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_1)))))) {
            vlSelfRef.top__DOT__csr__DOT__mtvec_reg 
                = vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__csr_write_val;
        }
        if (vlSelfRef.top__DOT___idu_io_is_ebreak) {
            vlSelfRef.top__DOT__csr__DOT__mepc_reg 
                = vlSelfRef.top__DOT__ifu__DOT__pcReg;
            vlSelfRef.top__DOT__csr__DOT__mcause_reg = 3U;
            vlSelfRef.top__DOT__csr__DOT__mtval_reg 
                = vlSelfRef.top__DOT__ifu__DOT__pcReg;
        } else if (vlSelfRef.top__DOT___idu_io_is_ecall) {
            vlSelfRef.top__DOT__csr__DOT__mepc_reg 
                = ((IData)(4U) + vlSelfRef.top__DOT__ifu__DOT__pcReg);
            vlSelfRef.top__DOT__csr__DOT__mcause_reg = 0x0000000bU;
            vlSelfRef.top__DOT__csr__DOT__mtval_reg = 0U;
        } else {
            if ((1U & (~ ((((((~ (IData)(vlSelfRef.top__DOT____Vcellinp__csr__io_csr_wen)) 
                              | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN)) 
                             | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_0)) 
                            | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_1)) 
                           | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_2)) 
                          | (~ (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_3)))))) {
                vlSelfRef.top__DOT__csr__DOT__mepc_reg 
                    = vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__csr_write_val;
            }
            if ((1U & (~ (((((((~ (IData)(vlSelfRef.top__DOT____Vcellinp__csr__io_csr_wen)) 
                               | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN)) 
                              | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_0)) 
                             | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_1)) 
                            | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_2)) 
                           | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_3)) 
                          | (~ (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_4)))))) {
                vlSelfRef.top__DOT__csr__DOT__mcause_reg 
                    = vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__csr_write_val;
            }
            if ((1U & (~ ((((((((~ (IData)(vlSelfRef.top__DOT____Vcellinp__csr__io_csr_wen)) 
                                | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN)) 
                               | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_0)) 
                              | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_1)) 
                             | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_2)) 
                            | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_3)) 
                           | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_4)) 
                          | (~ (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_5)))))) {
                vlSelfRef.top__DOT__csr__DOT__mtval_reg 
                    = vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__csr_write_val;
            }
        }
        if ((1U & (~ (((~ (IData)(vlSelfRef.top__DOT____Vcellinp__csr__io_csr_wen)) 
                       | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN)) 
                      | (~ (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_0)))))) {
            vlSelfRef.top__DOT__csr__DOT__mie_reg = vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__csr_write_val;
        }
        if ((1U & (~ (((((((((~ (IData)(vlSelfRef.top__DOT____Vcellinp__csr__io_csr_wen)) 
                             | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN)) 
                            | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_0)) 
                           | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_1)) 
                          | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_2)) 
                         | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_3)) 
                        | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_4)) 
                       | (IData)(vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_5)) 
                      | (0x0344U != (vlSelfRef.top__DOT___instMem_rdata 
                                     >> 0x00000014U)))))) {
            vlSelfRef.top__DOT__csr__DOT__mip_reg = vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__csr_write_val;
        }
    }
    vlSelfRef.top__DOT__csr__DOT__minstret_reg = __Vdly__top__DOT__csr__DOT__minstret_reg;
    vlSelfRef.io_debug_regs_0 = vlSelfRef.top__DOT__grf__DOT__regs_0;
    vlSelfRef.io_debug_regs_1 = vlSelfRef.top__DOT__grf__DOT__regs_1;
    vlSelfRef.io_debug_regs_2 = vlSelfRef.top__DOT__grf__DOT__regs_2;
    vlSelfRef.io_debug_regs_3 = vlSelfRef.top__DOT__grf__DOT__regs_3;
    vlSelfRef.io_debug_regs_4 = vlSelfRef.top__DOT__grf__DOT__regs_4;
    vlSelfRef.io_debug_regs_5 = vlSelfRef.top__DOT__grf__DOT__regs_5;
    vlSelfRef.io_debug_regs_6 = vlSelfRef.top__DOT__grf__DOT__regs_6;
    vlSelfRef.io_debug_regs_7 = vlSelfRef.top__DOT__grf__DOT__regs_7;
    vlSelfRef.io_debug_regs_8 = vlSelfRef.top__DOT__grf__DOT__regs_8;
    vlSelfRef.io_debug_regs_9 = vlSelfRef.top__DOT__grf__DOT__regs_9;
    vlSelfRef.io_debug_regs_10 = vlSelfRef.top__DOT__grf__DOT__regs_10;
    vlSelfRef.io_debug_regs_11 = vlSelfRef.top__DOT__grf__DOT__regs_11;
    vlSelfRef.io_debug_regs_12 = vlSelfRef.top__DOT__grf__DOT__regs_12;
    vlSelfRef.io_debug_regs_13 = vlSelfRef.top__DOT__grf__DOT__regs_13;
    vlSelfRef.io_debug_regs_14 = vlSelfRef.top__DOT__grf__DOT__regs_14;
    vlSelfRef.io_debug_regs_15 = vlSelfRef.top__DOT__grf__DOT__regs_15;
    vlSelfRef.top__DOT__grf__DOT___GEN[0U] = vlSelfRef.top__DOT__grf__DOT__regs_0;
    vlSelfRef.top__DOT__grf__DOT___GEN[1U] = vlSelfRef.top__DOT__grf__DOT__regs_1;
    vlSelfRef.top__DOT__grf__DOT___GEN[2U] = (IData)(
                                                     (((QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_3)) 
                                                       << 0x00000020U) 
                                                      | (QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_2))));
    vlSelfRef.top__DOT__grf__DOT___GEN[3U] = (IData)(
                                                     ((((QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_3)) 
                                                        << 0x00000020U) 
                                                       | (QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_2))) 
                                                      >> 0x00000020U));
    vlSelfRef.top__DOT__grf__DOT___GEN[4U] = vlSelfRef.top__DOT__grf__DOT__regs_4;
    vlSelfRef.top__DOT__grf__DOT___GEN[5U] = vlSelfRef.top__DOT__grf__DOT__regs_5;
    vlSelfRef.top__DOT__grf__DOT___GEN[6U] = (IData)(
                                                     (((QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_7)) 
                                                       << 0x00000020U) 
                                                      | (QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_6))));
    vlSelfRef.top__DOT__grf__DOT___GEN[7U] = (IData)(
                                                     ((((QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_7)) 
                                                        << 0x00000020U) 
                                                       | (QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_6))) 
                                                      >> 0x00000020U));
    vlSelfRef.top__DOT__grf__DOT___GEN[8U] = vlSelfRef.top__DOT__grf__DOT__regs_8;
    vlSelfRef.top__DOT__grf__DOT___GEN[9U] = vlSelfRef.top__DOT__grf__DOT__regs_9;
    vlSelfRef.top__DOT__grf__DOT___GEN[10U] = (IData)(
                                                      (((QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_11)) 
                                                        << 0x00000020U) 
                                                       | (QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_10))));
    vlSelfRef.top__DOT__grf__DOT___GEN[11U] = (IData)(
                                                      ((((QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_11)) 
                                                         << 0x00000020U) 
                                                        | (QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_10))) 
                                                       >> 0x00000020U));
    vlSelfRef.top__DOT__grf__DOT___GEN[12U] = vlSelfRef.top__DOT__grf__DOT__regs_12;
    vlSelfRef.top__DOT__grf__DOT___GEN[13U] = vlSelfRef.top__DOT__grf__DOT__regs_13;
    vlSelfRef.top__DOT__grf__DOT___GEN[14U] = (IData)(
                                                      (((QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_15)) 
                                                        << 0x00000020U) 
                                                       | (QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_14))));
    vlSelfRef.top__DOT__grf__DOT___GEN[15U] = (IData)(
                                                      ((((QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_15)) 
                                                         << 0x00000020U) 
                                                        | (QData)((IData)(vlSelfRef.top__DOT__grf__DOT__regs_14))) 
                                                       >> 0x00000020U));
    vlSelfRef.top__DOT__ifu__DOT__pcReg = __Vdly__top__DOT__ifu__DOT__pcReg;
    vlSelfRef.io_debug_pc = vlSelfRef.top__DOT__ifu__DOT__pcReg;
    vlSelfRef.top__DOT___ifu_io_pctogrf = ((IData)(4U) 
                                           + vlSelfRef.top__DOT__ifu__DOT__pcReg);
    Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_read_TOP(vlSelfRef.top__DOT__ifu__DOT__pcReg, vlSelfRef.__Vfunc_top__DOT__instMem__DOT__pmem_read__0__Vfuncout);
    vlSelfRef.top__DOT___instMem_rdata = vlSelfRef.__Vfunc_top__DOT__instMem__DOT__pmem_read__0__Vfuncout;
    vlSelfRef.io_debug_is_lui = (0x37U == (0x0000007fU 
                                           & vlSelfRef.top__DOT___instMem_rdata));
    vlSelfRef.io_debug_inst = vlSelfRef.top__DOT___instMem_rdata;
    vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4 
        = (IData)((0x00000073U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.__VdfgRegularize_he50b618e_0_5 = (IData)(
                                                       (0U 
                                                        == 
                                                        (0xfe0f8f80U 
                                                         & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT___csr_io_csr_rdata = ((0x0300U 
                                              == (vlSelfRef.top__DOT___instMem_rdata 
                                                  >> 0x00000014U))
                                              ? vlSelfRef.top__DOT__csr__DOT__mstatus_reg
                                              : ((0x0301U 
                                                  == 
                                                  (vlSelfRef.top__DOT___instMem_rdata 
                                                   >> 0x00000014U))
                                                  ? 0U
                                                  : 
                                                 ((0x0304U 
                                                   == 
                                                   (vlSelfRef.top__DOT___instMem_rdata 
                                                    >> 0x00000014U))
                                                   ? vlSelfRef.top__DOT__csr__DOT__mie_reg
                                                   : 
                                                  ((0x0305U 
                                                    == 
                                                    (vlSelfRef.top__DOT___instMem_rdata 
                                                     >> 0x00000014U))
                                                    ? vlSelfRef.top__DOT__csr__DOT__mtvec_reg
                                                    : 
                                                   ((0x0340U 
                                                     == 
                                                     (vlSelfRef.top__DOT___instMem_rdata 
                                                      >> 0x00000014U))
                                                     ? 0U
                                                     : 
                                                    ((0x0341U 
                                                      == 
                                                      (vlSelfRef.top__DOT___instMem_rdata 
                                                       >> 0x00000014U))
                                                      ? vlSelfRef.top__DOT__csr__DOT__mepc_reg
                                                      : 
                                                     ((0x0342U 
                                                       == 
                                                       (vlSelfRef.top__DOT___instMem_rdata 
                                                        >> 0x00000014U))
                                                       ? vlSelfRef.top__DOT__csr__DOT__mcause_reg
                                                       : 
                                                      ((0x0343U 
                                                        == 
                                                        (vlSelfRef.top__DOT___instMem_rdata 
                                                         >> 0x00000014U))
                                                        ? vlSelfRef.top__DOT__csr__DOT__mtval_reg
                                                        : 
                                                       ((0x0344U 
                                                         == 
                                                         (vlSelfRef.top__DOT___instMem_rdata 
                                                          >> 0x00000014U))
                                                         ? vlSelfRef.top__DOT__csr__DOT__mip_reg
                                                         : 
                                                        ((0x0b00U 
                                                          == 
                                                          (vlSelfRef.top__DOT___instMem_rdata 
                                                           >> 0x00000014U))
                                                          ? (IData)(vlSelfRef.top__DOT__csr__DOT__mcycle_reg)
                                                          : 
                                                         ((0x0b80U 
                                                           == 
                                                           (vlSelfRef.top__DOT___instMem_rdata 
                                                            >> 0x00000014U))
                                                           ? (IData)(
                                                                     (vlSelfRef.top__DOT__csr__DOT__mcycle_reg 
                                                                      >> 0x00000020U))
                                                           : 
                                                          ((0x0b02U 
                                                            == 
                                                            (vlSelfRef.top__DOT___instMem_rdata 
                                                             >> 0x00000014U))
                                                            ? (IData)(vlSelfRef.top__DOT__csr__DOT__minstret_reg)
                                                            : 
                                                           ((0x0b82U 
                                                             == 
                                                             (vlSelfRef.top__DOT___instMem_rdata 
                                                              >> 0x00000014U))
                                                             ? (IData)(
                                                                       (vlSelfRef.top__DOT__csr__DOT__minstret_reg 
                                                                        >> 0x00000020U))
                                                             : 
                                                            ((0x0f11U 
                                                              == 
                                                              (vlSelfRef.top__DOT___instMem_rdata 
                                                               >> 0x00000014U))
                                                              ? 0x79737978U
                                                              : 
                                                             ((0x0f12U 
                                                               == 
                                                               (vlSelfRef.top__DOT___instMem_rdata 
                                                                >> 0x00000014U))
                                                               ? 0x018a9e3bU
                                                               : 0U)))))))))))))));
    vlSelfRef.top__DOT__exu__DOT____Vcellinp__aluMux__io_use_u_imm 
        = ((0x17U == (0x0000007fU & vlSelfRef.top__DOT___instMem_rdata)) 
           | (0x37U == (0x0000007fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_beq 
        = (IData)((0x00000063U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bne 
        = (IData)((0x00001063U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_blt 
        = (IData)((0x00004063U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bge 
        = (IData)((0x00005063U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bltu 
        = (IData)((0x00006063U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bgeu 
        = (IData)((0x00007063U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slt 
        = (IData)((0x00002033U == (0xfe00707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltu 
        = (IData)((0x00003033U == (0xfe00707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xor 
        = (IData)((0x00004033U == (0xfe00707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_or 
        = (IData)((0x00006033U == (0xfe00707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_and 
        = (IData)((0x00007033U == (0xfe00707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sw 
        = (IData)((0x00002023U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sb 
        = (IData)((0x00000023U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sh 
        = (IData)((0x00001023U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_addi 
        = (IData)((0x00000013U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xori 
        = (IData)((0x00004013U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_ori 
        = (IData)((0x00006013U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_andi 
        = (IData)((0x00007013U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slti 
        = (IData)((0x00002013U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltiu 
        = (IData)((0x00003013U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_jalr 
        = (IData)((0x00000067U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lb 
        = (IData)((3U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lh 
        = (IData)((0x00001003U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lw 
        = (IData)((0x00002003U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lhu 
        = (IData)((0x00005003U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lbu 
        = (IData)((0x00004003U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT__is_csr 
        = ((0x73U == (0x0000007fU & vlSelfRef.top__DOT___instMem_rdata)) 
           & (0U != (7U & (vlSelfRef.top__DOT___instMem_rdata 
                           >> 0x0000000cU))));
    vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3 
        = (IData)((0x00005033U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2 
        = (IData)((0x00000033U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6 
        = (IData)((0x00001000U == (0xfe007000U & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1 
        = (IData)((0x00005013U == (0x0000707fU & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT___idu_io_is_ecall = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4) 
                                            & ((0U 
                                                == 
                                                (vlSelfRef.top__DOT___instMem_rdata 
                                                 >> 0x00000014U)) 
                                               & (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_5)));
    vlSelfRef.top__DOT___idu_io_is_ebreak = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4) 
                                             & ((1U 
                                                 == 
                                                 (vlSelfRef.top__DOT___instMem_rdata 
                                                  >> 0x00000014U)) 
                                                & (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_5)));
    vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_3 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_beq) 
           | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bne) 
              | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bge) 
                 | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_blt) 
                    | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bgeu) 
                       | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bltu))))));
    vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_and) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_or));
    vlSelfRef.io_debug_is_sw = vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sw;
    vlSelfRef.io_debug_is_sb = vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sb;
    vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_1 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sh) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sw));
    vlSelfRef.io_debug_is_addi = vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_addi;
    vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_2 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slti) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltiu));
    vlSelfRef.io_debug_is_jalr = vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_jalr;
    vlSelfRef.top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_0 
        = ((0x6fU == (0x0000007fU & vlSelfRef.top__DOT___instMem_rdata)) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_jalr));
    vlSelfRef.io_debug_is_lw = vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lw;
    vlSelfRef.io_debug_is_lbu = vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lbu;
    vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_0 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lbu) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lhu));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrsi 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT__is_csr) 
           & (0x00006000U == (0x00007000U & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrci 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT__is_csr) 
           & (0x00007000U == (0x00007000U & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrwi 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT__is_csr) 
           & (0x00005000U == (0x00007000U & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrs 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT__is_csr) 
           & (0x00002000U == (0x00007000U & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrc 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT__is_csr) 
           & (0x00003000U == (0x00007000U & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrw 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT__is_csr) 
           & (0x00001000U == (0x00007000U & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srl 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3) 
           & (0U == (vlSelfRef.top__DOT___instMem_rdata 
                     >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sra 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3) 
           & (0x20U == (vlSelfRef.top__DOT___instMem_rdata 
                        >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sub 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2) 
           & (0x20U == (vlSelfRef.top__DOT___instMem_rdata 
                        >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_add 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2) 
           & (0U == (vlSelfRef.top__DOT___instMem_rdata 
                     >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sll 
        = ((0x33U == (0x0000007fU & vlSelfRef.top__DOT___instMem_rdata)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slli 
        = ((0x13U == (0x0000007fU & vlSelfRef.top__DOT___instMem_rdata)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srli 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1) 
           & (0U == (vlSelfRef.top__DOT___instMem_rdata 
                     >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srai 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1) 
           & (0x20U == (vlSelfRef.top__DOT___instMem_rdata 
                        >> 0x00000019U)));
    if (vlSelfRef.top__DOT___idu_io_is_ebreak) {
        sim_finish();
    }
    if (vlSelfRef.top__DOT___idu_io_is_ebreak) {
        sim_finish();
    }
    vlSelfRef.io_debug_is_ebreak = vlSelfRef.top__DOT___idu_io_is_ebreak;
    vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_store 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sb) 
           | (IData)(vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_1));
    vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_load 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lb) 
           | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lh) 
              | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lw) 
                 | (IData)(vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_0))));
    vlSelfRef.top__DOT__use_imm_csr = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrwi) 
                                       | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrci) 
                                          | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrsi)));
    vlSelfRef.top__DOT____Vcellinp__csr__io_csr_op 
        = (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrs) 
            | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrsi))
            ? 1U : (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrc) 
                     | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrci)) 
                    << 1U));
    vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT___csr_use_rs1_T 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrs) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrw));
    vlSelfRef.io_debug_is_add = vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_add;
    vlSelfRef.top__DOT__exu__DOT____Vcellinp__aluMux__io_use_rs2 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_add) 
           | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sub) 
              | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sll) 
                 | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slt) 
                    | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltu) 
                       | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xor) 
                          | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sra) 
                             | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srl) 
                                | (IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3)))))))));
    vlSelfRef.top__DOT__exu__DOT____Vcellinp__alu__io_alu_op 
        = (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_add) 
            | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_addi) 
               | ((0x17U == (0x0000007fU & vlSelfRef.top__DOT___instMem_rdata)) 
                  | (IData)(vlSelfRef.top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_0))))
            ? 0U : ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sub)
                     ? 1U : (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sll) 
                              | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slli))
                              ? 2U : (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slt) 
                                       | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slti))
                                       ? 3U : (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltiu) 
                                                | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltu))
                                                ? 4U
                                                : (
                                                   ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xor) 
                                                    | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xori))
                                                    ? 5U
                                                    : 
                                                   (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srl) 
                                                     | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srli))
                                                     ? 6U
                                                     : 
                                                    (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sra) 
                                                      | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srai))
                                                      ? 7U
                                                      : 
                                                     (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_or) 
                                                       | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_ori))
                                                       ? 8U
                                                       : 
                                                      (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_and) 
                                                        | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_andi))
                                                        ? 9U
                                                        : 
                                                       ((0x37U 
                                                         == 
                                                         (0x0000007fU 
                                                          & vlSelfRef.top__DOT___instMem_rdata))
                                                         ? 0x0aU
                                                         : 0U)))))))))));
    vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srai) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srli));
    vlSelfRef.io_debug_lsu_wen = vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_store;
    vlSelfRef.io_debug_lsu_ren = vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_load;
    vlSelfRef.__VdfgRegularize_he50b618e_0_0 = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrc) 
                                                | (IData)(vlSelfRef.top__DOT__use_imm_csr));
    vlSelfRef.top__DOT__exu__DOT____Vcellinp__grfCtrl__io_use_rs2 
        = ((IData)(vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_3) 
           | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sb) 
              | ((IData)(vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_1) 
                 | (IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aluMux__io_use_rs2))));
    vlSelfRef.__VdfgRegularize_he50b618e_0_3 = ((IData)(vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_2) 
                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_ori) 
                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xori) 
                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_andi) 
                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slli) 
                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srai) 
                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srli) 
                                                                  | (IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aluMux__io_use_rs2))))))));
    vlSelfRef.top__DOT___idu_io_imm_i = (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_jalr) 
                                          | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lb) 
                                             | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lh) 
                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lbu) 
                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lw) 
                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_addi) 
                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lhu) 
                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slti) 
                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltiu) 
                                                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_ori) 
                                                                     | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xori) 
                                                                        | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_andi) 
                                                                           | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slli) 
                                                                              | (IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2))))))))))))))
                                          ? (((- (IData)(
                                                         (vlSelfRef.top__DOT___instMem_rdata 
                                                          >> 0x0000001fU))) 
                                              << 0x0000000cU) 
                                             | (vlSelfRef.top__DOT___instMem_rdata 
                                                >> 0x00000014U))
                                          : 0U);
    vlSelfRef.__VdfgRegularize_he50b618e_0_2 = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_beq) 
                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_jalr) 
                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_blt) 
                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bne) 
                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bge) 
                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bltu) 
                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bgeu) 
                                                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lb) 
                                                                     | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lh) 
                                                                        | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lw) 
                                                                           | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lbu) 
                                                                              | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lhu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sb) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sh) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_addi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sw) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slti) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltiu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_ori) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xori) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_andi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slli) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_add) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sub) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sll) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slt) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xor) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sra) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srl) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT___csr_use_rs1_T) 
                                                                                | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrc))))))))))))))))))))))))))))))))));
    vlSelfRef.top__DOT___exu_io_rd_out = (((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aluMux__io_use_u_imm) 
                                           | ((0x6fU 
                                               == (0x0000007fU 
                                                   & vlSelfRef.top__DOT___instMem_rdata)) 
                                              | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_jalr) 
                                                 | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lb) 
                                                    | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lh) 
                                                       | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lw) 
                                                          | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lbu) 
                                                             | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lhu) 
                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_addi) 
                                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slti) 
                                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltiu) 
                                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xori) 
                                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_andi) 
                                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_ori) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slli) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srli) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_add) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srai) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sll) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sub) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slt) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srl) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xor) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_or) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sra) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_and) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT___csr_use_rs1_T) 
                                                                                | (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_0)))))))))))))))))))))))))))))
                                           ? (0x0000001fU 
                                              & (vlSelfRef.top__DOT___instMem_rdata 
                                                 >> 7U))
                                           : 0U);
    vlSelfRef.top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_3 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrs) 
           | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrw) 
              | (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_0)));
    vlSelfRef.top__DOT___exu_io_rs2_addr_out = ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__grfCtrl__io_use_rs2)
                                                 ? 
                                                (0x0000001fU 
                                                 & (vlSelfRef.top__DOT___instMem_rdata 
                                                    >> 0x00000014U))
                                                 : 0U);
    vlSelfRef.top__DOT__exu__DOT__use_rs1 = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_beq) 
                                             | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_jalr) 
                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_blt) 
                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bne) 
                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bge) 
                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bltu) 
                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bgeu) 
                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lb) 
                                                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lh) 
                                                                     | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lw) 
                                                                        | ((IData)(vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_0) 
                                                                           | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sb) 
                                                                              | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sh) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_addi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sw) 
                                                                                | (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_3))))))))))))))));
    if (vlSelfRef.__VdfgRegularize_he50b618e_0_2) {
        vlSelfRef.top__DOT___exu_io_rs1_addr_out = 
            (0x0000001fU & (vlSelfRef.top__DOT___instMem_rdata 
                            >> 0x0000000fU));
        vlSelfRef.__VdfgRegularize_he50b618e_0_6 = 
            (0U != (0x0000001fU & (vlSelfRef.top__DOT___instMem_rdata 
                                   >> 0x0000000fU)));
    } else {
        vlSelfRef.top__DOT___exu_io_rs1_addr_out = 0U;
        vlSelfRef.__VdfgRegularize_he50b618e_0_6 = 0U;
    }
    vlSelfRef.io_debug_grf_rdaddr = vlSelfRef.top__DOT___exu_io_rd_out;
    vlSelfRef.top__DOT___wbu_io_regWen = ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aluMux__io_use_u_imm) 
                                          | ((IData)(vlSelfRef.top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_0) 
                                             | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lb) 
                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lh) 
                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lbu) 
                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lw) 
                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_addi) 
                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lhu) 
                                                               | ((IData)(vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_2) 
                                                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_ori) 
                                                                     | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xori) 
                                                                        | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_andi) 
                                                                           | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slli) 
                                                                              | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srai) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srli) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_add) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sub) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sll) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slt) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sltu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xor) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sra) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srl) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3) 
                                                                                | (IData)(vlSelfRef.top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_3)))))))))))))))))))))))));
    vlSelfRef.top__DOT___grf_io_rs2out = ((((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__grfCtrl__io_use_rs2) 
                                            & (~ ((IData)(vlSelfRef.top__DOT___exu_io_rs2_addr_out) 
                                                  >> 4U))) 
                                           & (0U != 
                                              (0x0000001fU 
                                               & (vlSelfRef.top__DOT___instMem_rdata 
                                                  >> 0x00000014U))))
                                           ? vlSelfRef.top__DOT__grf__DOT___GEN
                                          [(0x0000000fU 
                                            & (IData)(vlSelfRef.top__DOT___exu_io_rs2_addr_out))]
                                           : 0U);
    vlSelfRef.top__DOT____Vcellinp__csr__io_csr_wen 
        = ((IData)(vlSelfRef.top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_3) 
           & ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrw) 
              | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_csrrwi) 
                 | ((IData)(vlSelfRef.top__DOT__use_imm_csr)
                     ? (0U != (0x0000001fU & (vlSelfRef.top__DOT___instMem_rdata 
                                              >> 0x0000000fU)))
                     : (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_6)))));
    vlSelfRef.top__DOT___grf_io_rs1out = (((IData)(vlSelfRef.top__DOT__exu__DOT__use_rs1) 
                                           & ((~ ((IData)(vlSelfRef.top__DOT___exu_io_rs1_addr_out) 
                                                  >> 4U)) 
                                              & (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_6)))
                                           ? vlSelfRef.top__DOT__grf__DOT___GEN
                                          [(0x0000000fU 
                                            & (IData)(vlSelfRef.top__DOT___exu_io_rs1_addr_out))]
                                           : 0U);
    vlSelfRef.io_debug_regWen = vlSelfRef.top__DOT___wbu_io_regWen;
    vlSelfRef.io_debug_grf_rden = vlSelfRef.top__DOT___wbu_io_regWen;
    vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2 
        = ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aluMux__io_use_rs2)
            ? vlSelfRef.top__DOT___grf_io_rs2out : 
           (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_addi) 
             | ((IData)(vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_2) 
                | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_ori) 
                   | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_xori) 
                      | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_andi) 
                         | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_slli) 
                            | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srai) 
                               | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_srli) 
                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_jalr) 
                                     | (IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_load))))))))))
             ? vlSelfRef.top__DOT___idu_io_imm_i : 
            ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aluMux__io_use_u_imm)
              ? (0xfffff000U & vlSelfRef.top__DOT___instMem_rdata)
              : 0U)));
    vlSelfRef.top__DOT___exu_io_branch_target = ((0x6fU 
                                                  == 
                                                  (0x0000007fU 
                                                   & vlSelfRef.top__DOT___instMem_rdata))
                                                  ? 
                                                 (vlSelfRef.top__DOT__ifu__DOT__pcReg 
                                                  + 
                                                  ((0x6fU 
                                                    == 
                                                    (0x0000007fU 
                                                     & vlSelfRef.top__DOT___instMem_rdata))
                                                    ? 
                                                   (((- (IData)(
                                                                (vlSelfRef.top__DOT___instMem_rdata 
                                                                 >> 0x0000001fU))) 
                                                     << 0x00000014U) 
                                                    | ((((0x000001feU 
                                                          & (vlSelfRef.top__DOT___instMem_rdata 
                                                             >> 0x0000000bU)) 
                                                         | (1U 
                                                            & (vlSelfRef.top__DOT___instMem_rdata 
                                                               >> 0x00000014U))) 
                                                        << 0x0000000bU) 
                                                       | (0x000007feU 
                                                          & (vlSelfRef.top__DOT___instMem_rdata 
                                                             >> 0x00000014U))))
                                                    : 0U))
                                                  : 
                                                 ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_jalr)
                                                   ? 
                                                  (0xfffffffcU 
                                                   & (vlSelfRef.top__DOT___idu_io_imm_i 
                                                      + vlSelfRef.top__DOT___grf_io_rs1out))
                                                   : 
                                                  (vlSelfRef.top__DOT__ifu__DOT__pcReg 
                                                   + 
                                                   ((IData)(vlSelfRef.top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_3)
                                                     ? 
                                                    (((- (IData)(
                                                                 (vlSelfRef.top__DOT___instMem_rdata 
                                                                  >> 0x0000001fU))) 
                                                      << 0x0000000cU) 
                                                     | ((0x00000800U 
                                                         & (vlSelfRef.top__DOT___instMem_rdata 
                                                            << 4U)) 
                                                        | ((0x000007e0U 
                                                            & (vlSelfRef.top__DOT___instMem_rdata 
                                                               >> 0x00000014U)) 
                                                           | (0x0000001eU 
                                                              & (vlSelfRef.top__DOT___instMem_rdata 
                                                                 >> 7U)))))
                                                     : 0U))));
    vlSelfRef.top__DOT__exu__DOT__bru__DOT___branch_cond_T_9 
        = ((~ (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_beq)) 
           & ((~ (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bne)) 
              & (((~ (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_blt)) 
                  & ((~ (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bge)) 
                     & ((~ (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bltu)) 
                        & (7U == (7U & (- (IData)((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_bgeu)))))))) 
                 & (vlSelfRef.top__DOT___grf_io_rs1out 
                    >= vlSelfRef.top__DOT___grf_io_rs2out))));
    vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1 
        = ((0x17U == (0x0000007fU & vlSelfRef.top__DOT___instMem_rdata))
            ? vlSelfRef.top__DOT__ifu__DOT__pcReg : 
           (((~ ((0x17U == (0x0000007fU & vlSelfRef.top__DOT___instMem_rdata)) 
                 | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sw) 
                    | ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sb) 
                       | (IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sh))))) 
             & (IData)(vlSelfRef.top__DOT__exu__DOT__use_rs1))
             ? vlSelfRef.top__DOT___grf_io_rs1out : 0U));
    vlSelfRef.top__DOT___exu_io_agu_addr = ((((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_load) 
                                              | (IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_store))
                                              ? vlSelfRef.top__DOT___grf_io_rs1out
                                              : 0U) 
                                            + ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_load)
                                                ? vlSelfRef.top__DOT___idu_io_imm_i
                                                : ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_store)
                                                    ? 
                                                   (((- (IData)(
                                                                (vlSelfRef.top__DOT___instMem_rdata 
                                                                 >> 0x0000001fU))) 
                                                     << 0x0000000cU) 
                                                    | ((0x00000fe0U 
                                                        & (vlSelfRef.top__DOT___instMem_rdata 
                                                           >> 0x00000014U)) 
                                                       | (0x0000001fU 
                                                          & (vlSelfRef.top__DOT___instMem_rdata 
                                                             >> 7U))))
                                                    : 0U)));
    vlSelfRef.io_debug_alu_src2 = vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2;
    vlSelfRef.io_debug_alu_src1 = vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1;
    __Vtemp_19[0U] = 0U;
    __Vtemp_19[1U] = (vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1 
                      - vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2);
    __Vtemp_19[2U] = (vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1 
                      << (0x0000001fU & vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2));
    __Vtemp_19[3U] = VL_LTS_III(32, vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1, vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2);
    __Vtemp_19[4U] = (vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1 
                      < vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2);
    __Vtemp_19[5U] = (vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1 
                      ^ vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2);
    __Vtemp_19[6U] = (vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1 
                      >> (0x0000001fU & vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2));
    __Vtemp_19[7U] = VL_SHIFTRS_III(32,32,5, vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1, 
                                    (0x0000001fU & vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2));
    __Vtemp_19[8U] = (vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1 
                      | vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2);
    __Vtemp_19[9U] = (vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1 
                      & vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2);
    __Vtemp_19[10U] = vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2;
    __Vtemp_19[11U] = 0U;
    __Vtemp_19[12U] = 0U;
    __Vtemp_19[13U] = 0U;
    __Vtemp_19[14U] = 0U;
    __Vtemp_19[15U] = 0U;
    vlSelfRef.top__DOT___exu_io_alu_result = ((0U == (IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__alu__io_alu_op))
                                               ? (vlSelfRef.top__DOT__exu__DOT___aluMux_io_source1 
                                                  + vlSelfRef.top__DOT__exu__DOT___aluMux_io_source2)
                                               : ((
                                                   (0U 
                                                    == 
                                                    (0x0000001fU 
                                                     & ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__alu__io_alu_op) 
                                                        << 5U)))
                                                    ? 0U
                                                    : 
                                                   (__Vtemp_19
                                                    [
                                                    (((IData)(0x0000001fU) 
                                                      + 
                                                      ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__alu__io_alu_op) 
                                                       << 5U)) 
                                                     >> 5U)] 
                                                    << 
                                                    ((IData)(0x00000020U) 
                                                     - 
                                                     (0x0000001fU 
                                                      & ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__alu__io_alu_op) 
                                                         << 5U))))) 
                                                  | (__Vtemp_19
                                                     [
                                                     (0x07ffffffU 
                                                      & (IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__alu__io_alu_op))] 
                                                     >> 
                                                     (0x0000001fU 
                                                      & ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__alu__io_alu_op) 
                                                         << 5U)))));
    vlSelfRef.io_debug_lsu_addr = (0xfffffffcU & vlSelfRef.top__DOT___exu_io_agu_addr);
    vlSelfRef.io_debug_lsu_wmask = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sw)
                                     ? 0x0000000fU : 
                                    (0x0000000fU & 
                                     ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sh)
                                       ? ((0U == (3U 
                                                  & vlSelfRef.top__DOT___exu_io_agu_addr))
                                           ? 3U : (
                                                   (2U 
                                                    == 
                                                    (3U 
                                                     & vlSelfRef.top__DOT___exu_io_agu_addr))
                                                    ? 0x0cU
                                                    : 0U))
                                       : ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sb)
                                           ? ((IData)(1U) 
                                              << (3U 
                                                  & vlSelfRef.top__DOT___exu_io_agu_addr))
                                           : 0U))));
    vlSelfRef.top__DOT__lsu__DOT____VdfgRegularize_hac146698_0_0 
        = (vlSelfRef.top__DOT___grf_io_rs2out << (0x00000018U 
                                                  & (vlSelfRef.top__DOT___exu_io_agu_addr 
                                                     << 3U)));
    vlSelfRef.top__DOT___dataMem_rdata = ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_load)
                                           ? ([&]() {
                Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_read_TOP(
                                                                                (0xfffffffcU 
                                                                                & vlSelfRef.top__DOT___exu_io_agu_addr), vlSelfRef.__Vfunc_top__DOT__dataMem__DOT__pmem_read__2__Vfuncout);
            }(), vlSelfRef.__Vfunc_top__DOT__dataMem__DOT__pmem_read__2__Vfuncout)
                                           : 0U);
    vlSelfRef.io_debug_alu_res = vlSelfRef.top__DOT___exu_io_alu_result;
    vlSelfRef.io_debug_lsu_wdata = ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sb)
                                     ? vlSelfRef.top__DOT__lsu__DOT____VdfgRegularize_hac146698_0_0
                                     : ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_sh)
                                         ? vlSelfRef.top__DOT__lsu__DOT____VdfgRegularize_hac146698_0_0
                                         : vlSelfRef.top__DOT___grf_io_rs2out));
    vlSelfRef.io_debug_lsu_rdata = vlSelfRef.top__DOT___dataMem_rdata;
    if ((2U & vlSelfRef.top__DOT___exu_io_agu_addr)) {
        vlSelfRef.top__DOT__lsu__DOT__byte_sel = (0x000000ffU 
                                                  & ((1U 
                                                      & vlSelfRef.top__DOT___exu_io_agu_addr)
                                                      ? 
                                                     (vlSelfRef.top__DOT___dataMem_rdata 
                                                      >> 0x00000018U)
                                                      : 
                                                     (vlSelfRef.top__DOT___dataMem_rdata 
                                                      >> 0x00000010U)));
        vlSelfRef.top__DOT__lsu__DOT__half_sel = (0x0000ffffU 
                                                  & (vlSelfRef.top__DOT___dataMem_rdata 
                                                     >> 0x00000010U));
    } else {
        vlSelfRef.top__DOT__lsu__DOT__byte_sel = (0x000000ffU 
                                                  & ((1U 
                                                      & vlSelfRef.top__DOT___exu_io_agu_addr)
                                                      ? 
                                                     (vlSelfRef.top__DOT___dataMem_rdata 
                                                      >> 8U)
                                                      : vlSelfRef.top__DOT___dataMem_rdata));
        vlSelfRef.top__DOT__lsu__DOT__half_sel = (0x0000ffffU 
                                                  & vlSelfRef.top__DOT___dataMem_rdata);
    }
    vlSelfRef.top__DOT___wbu_io_wbData = ((IData)(vlSelfRef.top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_3)
                                           ? vlSelfRef.top__DOT___csr_io_csr_rdata
                                           : ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aluMux__io_use_u_imm)
                                               ? vlSelfRef.top__DOT___exu_io_alu_result
                                               : ((IData)(vlSelfRef.top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_0)
                                                   ? 
                                                  ((IData)(4U) 
                                                   + vlSelfRef.top__DOT__ifu__DOT__pcReg)
                                                   : 
                                                  ((IData)(vlSelfRef.top__DOT__exu__DOT____Vcellinp__aguMux__io_is_load)
                                                    ? 
                                                   ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lw)
                                                     ? vlSelfRef.top__DOT___dataMem_rdata
                                                     : 
                                                    ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lh)
                                                      ? 
                                                     (((- (IData)(
                                                                  (1U 
                                                                   & ((IData)(vlSelfRef.top__DOT__lsu__DOT__half_sel) 
                                                                      >> 0x0000000fU)))) 
                                                       << 0x00000010U) 
                                                      | (IData)(vlSelfRef.top__DOT__lsu__DOT__half_sel))
                                                      : 
                                                     ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lhu)
                                                       ? (IData)(vlSelfRef.top__DOT__lsu__DOT__half_sel)
                                                       : 
                                                      ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lb)
                                                        ? 
                                                       (((- (IData)(
                                                                    (1U 
                                                                     & ((IData)(vlSelfRef.top__DOT__lsu__DOT__byte_sel) 
                                                                        >> 7U)))) 
                                                         << 8U) 
                                                        | (IData)(vlSelfRef.top__DOT__lsu__DOT__byte_sel))
                                                        : 
                                                       ((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_lbu)
                                                         ? (IData)(vlSelfRef.top__DOT__lsu__DOT__byte_sel)
                                                         : 0U)))))
                                                    : 
                                                   (((IData)(vlSelfRef.top__DOT__idu__DOT___instDecoder_io_is_addi) 
                                                     | (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_3))
                                                     ? vlSelfRef.top__DOT___exu_io_alu_result
                                                     : 0U)))));
    vlSelfRef.io_debug_wbData = vlSelfRef.top__DOT___wbu_io_wbData;
    vlSelfRef.io_debug_grf_input = vlSelfRef.top__DOT___wbu_io_wbData;
}

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtop___024root___nba_sequent__TOP__0(vlSelf);
    }
}

void Vtop___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((0U >= n));
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vtop___024root___eval_phase__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtop___024root___eval_triggers_vec__act(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vtop___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    return (0U);
}

void Vtop___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtop___024root___eval_phase__nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtop___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtop___024root___eval_nba(vlSelf);
        Vtop___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vtop___024root___eval(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("../MyChisel/verilog/top.sv", 2, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("../MyChisel/verilog/top.sv", 2, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactPhaseResult = Vtop___024root___eval_phase__act(vlSelf);
        } while (vlSelfRef.__VactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = Vtop___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

#ifdef VL_DEBUG
void Vtop___024root___eval_debug_assertions(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_debug_assertions\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clock & 0xfeU)))) {
        Verilated::overWidthError("clock");
    }
    if (VL_UNLIKELY(((vlSelfRef.reset & 0xfeU)))) {
        Verilated::overWidthError("reset");
    }
    if (VL_UNLIKELY(((vlSelfRef.io_clk & 0xfeU)))) {
        Verilated::overWidthError("io_clk");
    }
    if (VL_UNLIKELY(((vlSelfRef.io_reset & 0xfeU)))) {
        Verilated::overWidthError("io_reset");
    }
}
#endif  // VL_DEBUG

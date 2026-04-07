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
    // Body
    if (vlSelfRef.reset) {
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
        vlSelfRef.top__DOT__ifu__DOT__pcReg = 0x80000000U;
    } else {
        vlSelfRef.top__DOT__grf__DOT__unnamedblk1__DOT___regs_T_4 
            = ((((IData)(vlSelfRef.top__DOT__wbu__DOT___io_regWen_T) 
                 | ((IData)(vlSelfRef.top__DOT___exu_io_jalr_out) 
                    | ((IData)(vlSelfRef.top__DOT___idu_io_is_lbu) 
                       | ((0x37U == (0x0000007fU & vlSelfRef.top__DOT___instMem_rdata)) 
                          | (IData)(vlSelfRef.top__DOT___idu_io_is_lw))))) 
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
        vlSelfRef.top__DOT__ifu__DOT__pcReg = ((IData)(vlSelfRef.top__DOT___exu_io_jalr_out)
                                                ? (0xfffffffcU 
                                                   & (((IData)(vlSelfRef.top__DOT__exu__DOT__bruMux__DOT___GEN)
                                                        ? 0U
                                                        : vlSelfRef.top__DOT___grf_io_rs1out) 
                                                      + 
                                                      ((IData)(vlSelfRef.top__DOT__exu__DOT__bruMux__DOT___GEN)
                                                        ? 0U
                                                        : vlSelfRef.top__DOT___idu_io_imm12)))
                                                : vlSelfRef.top__DOT___ifu_io_pctogrf);
    }
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
    vlSelfRef.top__DOT___ifu_io_pctogrf = ((IData)(4U) 
                                           + vlSelfRef.top__DOT__ifu__DOT__pcReg);
    Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_read_TOP(vlSelfRef.top__DOT__ifu__DOT__pcReg, vlSelfRef.__Vfunc_top__DOT__instMem__DOT__pmem_read__0__Vfuncout);
    vlSelfRef.top__DOT___instMem_rdata = vlSelfRef.__Vfunc_top__DOT__instMem__DOT__pmem_read__0__Vfuncout;
    if ((IData)((0x00000073U == (0xfe0fffffU & vlSelfRef.top__DOT___instMem_rdata)))) {
        sim_finish();
    }
    vlSelfRef.top__DOT___idu_io_is_add = (IData)((0x00000033U 
                                                  == 
                                                  (0xfe00707fU 
                                                   & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT___exu_io_jalr_out = (IData)(
                                                   (0x00000067U 
                                                    == 
                                                    (0x0000707fU 
                                                     & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT___idu_io_is_addi = (IData)((0x00000013U 
                                                   == 
                                                   (0x0000707fU 
                                                    & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT___idu_io_is_lbu = (IData)((0x00004003U 
                                                  == 
                                                  (0x0000707fU 
                                                   & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT___idu_io_is_lw = (IData)((0x00002003U 
                                                 == 
                                                 (0x0000707fU 
                                                  & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT___idu_io_is_sb = (IData)((0x00000023U 
                                                 == 
                                                 (0x0000707fU 
                                                  & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT___idu_io_is_sw = (IData)((0x00002023U 
                                                 == 
                                                 (0x0000707fU 
                                                  & vlSelfRef.top__DOT___instMem_rdata)));
    vlSelfRef.top__DOT__wbu__DOT___io_regWen_T = ((IData)(vlSelfRef.top__DOT___idu_io_is_add) 
                                                  | (IData)(vlSelfRef.top__DOT___idu_io_is_addi));
    vlSelfRef.top__DOT___lsu_io_dmemRen = ((IData)(vlSelfRef.top__DOT___idu_io_is_lbu) 
                                           | (IData)(vlSelfRef.top__DOT___idu_io_is_lw));
    vlSelfRef.top__DOT__exu__DOT__bruMux__DOT___GEN 
        = (1U & ((IData)(vlSelfRef.top__DOT___idu_io_is_add) 
                 | ((IData)(vlSelfRef.top__DOT___idu_io_is_addi) 
                    | ((IData)(vlSelfRef.top__DOT___idu_io_is_lbu) 
                       | ((IData)(vlSelfRef.top__DOT___idu_io_is_lw) 
                          | ((IData)(vlSelfRef.top__DOT___idu_io_is_sw) 
                             | ((~ (IData)(vlSelfRef.top__DOT___exu_io_jalr_out)) 
                                | (IData)(vlSelfRef.top__DOT___idu_io_is_sb))))))));
    vlSelfRef.top__DOT___lsu_io_dmemWen = ((IData)(vlSelfRef.top__DOT___idu_io_is_sb) 
                                           | (IData)(vlSelfRef.top__DOT___idu_io_is_sw));
    vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_0 
        = ((IData)(vlSelfRef.top__DOT__wbu__DOT___io_regWen_T) 
           | (IData)(vlSelfRef.top__DOT___exu_io_jalr_out));
    vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3 
        = ((IData)(vlSelfRef.top__DOT___idu_io_is_addi) 
           | ((IData)(vlSelfRef.top__DOT___exu_io_jalr_out) 
              | (IData)(vlSelfRef.top__DOT___lsu_io_dmemRen)));
    vlSelfRef.top__DOT___exu_io_rs2_en = ((IData)(vlSelfRef.top__DOT___idu_io_is_add) 
                                          | (IData)(vlSelfRef.top__DOT___lsu_io_dmemWen));
    vlSelfRef.__VdfgRegularize_he50b618e_0_0 = ((IData)(vlSelfRef.top__DOT___lsu_io_dmemRen) 
                                                | (IData)(vlSelfRef.top__DOT___lsu_io_dmemWen));
    vlSelfRef.top__DOT___exu_io_rd_out = (((IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_0) 
                                           | ((0x37U 
                                               == (0x0000007fU 
                                                   & vlSelfRef.top__DOT___instMem_rdata)) 
                                              | (IData)(vlSelfRef.top__DOT___lsu_io_dmemRen)))
                                           ? (0x0000001fU 
                                              & (vlSelfRef.top__DOT___instMem_rdata 
                                                 >> 7U))
                                           : 0U);
    vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT__imm12_pre_extended 
        = (0x00000fffU & ((IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3)
                           ? (vlSelfRef.top__DOT___instMem_rdata 
                              >> 0x00000014U) : ((IData)(vlSelfRef.top__DOT___lsu_io_dmemWen)
                                                  ? 
                                                 ((0x00000fe0U 
                                                   & (vlSelfRef.top__DOT___instMem_rdata 
                                                      >> 0x00000014U)) 
                                                  | (0x0000001fU 
                                                     & (vlSelfRef.top__DOT___instMem_rdata 
                                                        >> 7U)))
                                                  : 0U)));
    vlSelfRef.top__DOT___exu_io_rs2_addr_out = ((IData)(vlSelfRef.top__DOT___exu_io_rs2_en)
                                                 ? 
                                                (0x0000001fU 
                                                 & (vlSelfRef.top__DOT___instMem_rdata 
                                                    >> 0x00000014U))
                                                 : 0U);
    vlSelfRef.top__DOT__exu__DOT__aguMux__DOT____VdfgRegularize_h679c4643_0_0 
        = (1U & ((IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_0) 
                 | ((~ (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_0)) 
                    | (0x37U == (0x0000007fU & vlSelfRef.top__DOT___instMem_rdata)))));
    vlSelfRef.__VdfgRegularize_he50b618e_0_1 = ((IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_0) 
                                                | (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_0));
    vlSelfRef.top__DOT___idu_io_imm12 = (((IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3) 
                                          | (IData)(vlSelfRef.top__DOT___lsu_io_dmemWen))
                                          ? (((- (IData)(
                                                         (1U 
                                                          & ((IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT__imm12_pre_extended) 
                                                             >> 0x0000000bU)))) 
                                              << 0x0000000cU) 
                                             | (IData)(vlSelfRef.top__DOT__idu__DOT__infoDecoder__DOT__imm12_pre_extended))
                                          : 0U);
    vlSelfRef.top__DOT___exu_io_rs1_addr_out = ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_1)
                                                 ? 
                                                (0x0000001fU 
                                                 & (vlSelfRef.top__DOT___instMem_rdata 
                                                    >> 0x0000000fU))
                                                 : 0U);
    vlSelfRef.top__DOT___grf_io_rs1out = ((((IData)(vlSelfRef.top__DOT___idu_io_is_addi) 
                                            | ((IData)(vlSelfRef.top__DOT___idu_io_is_add) 
                                               | ((IData)(vlSelfRef.top__DOT___exu_io_jalr_out) 
                                                  | ((IData)(vlSelfRef.top__DOT___idu_io_is_lbu) 
                                                     | ((IData)(vlSelfRef.top__DOT___idu_io_is_lw) 
                                                        | (IData)(vlSelfRef.top__DOT___lsu_io_dmemWen)))))) 
                                           & ((~ ((IData)(vlSelfRef.top__DOT___exu_io_rs1_addr_out) 
                                                  >> 4U)) 
                                              & ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_1) 
                                                 & (0U 
                                                    != 
                                                    (0x0000001fU 
                                                     & (vlSelfRef.top__DOT___instMem_rdata 
                                                        >> 0x0000000fU))))))
                                           ? vlSelfRef.top__DOT__grf__DOT___GEN
                                          [(0x0000000fU 
                                            & (IData)(vlSelfRef.top__DOT___exu_io_rs1_addr_out))]
                                           : 0U);
    vlSelfRef.top__DOT___lsu_io_dmemAddr = (((IData)(vlSelfRef.top__DOT__exu__DOT__aguMux__DOT____VdfgRegularize_h679c4643_0_0)
                                              ? 0U : vlSelfRef.top__DOT___grf_io_rs1out) 
                                            + ((IData)(vlSelfRef.top__DOT__exu__DOT__aguMux__DOT____VdfgRegularize_h679c4643_0_0)
                                                ? 0U
                                                : vlSelfRef.top__DOT___idu_io_imm12));
    if (vlSelfRef.top__DOT___lsu_io_dmemWen) {
        Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_write_TOP(vlSelfRef.top__DOT___lsu_io_dmemAddr, 
                                                                            ((((IData)(vlSelfRef.top__DOT___exu_io_rs2_en) 
                                                                               & (~ 
                                                                                ((IData)(vlSelfRef.top__DOT___exu_io_rs2_addr_out) 
                                                                                >> 4U))) 
                                                                              & (0U 
                                                                                != 
                                                                                (0x0000001fU 
                                                                                & (vlSelfRef.top__DOT___instMem_rdata 
                                                                                >> 0x00000014U))))
                                                                              ? vlSelfRef.top__DOT__grf__DOT___GEN
                                                                             [
                                                                             (0x0000000fU 
                                                                              & (IData)(vlSelfRef.top__DOT___exu_io_rs2_addr_out))]
                                                                              : 0U), 
                                                                            ((IData)(vlSelfRef.top__DOT___idu_io_is_sw)
                                                                              ? 0x0000000fU
                                                                              : 
                                                                             ((IData)(vlSelfRef.top__DOT___idu_io_is_sb)
                                                                               ? 
                                                                              (0x0000000fU 
                                                                               & ((IData)(1U) 
                                                                                << 
                                                                                (3U 
                                                                                & vlSelfRef.top__DOT___lsu_io_dmemAddr)))
                                                                               : 0U)));
    }
    if ((IData)((0x00000073U == (0xfe0fffffU & vlSelfRef.top__DOT___instMem_rdata)))) {
        sim_finish();
    }
    vlSelfRef.top__DOT___dataMem_rdata = ((IData)(vlSelfRef.top__DOT___lsu_io_dmemRen)
                                           ? ([&]() {
                Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_read_TOP(vlSelfRef.top__DOT___lsu_io_dmemAddr, vlSelfRef.__Vfunc_top__DOT__dataMem__DOT__pmem_read__2__Vfuncout);
            }(), vlSelfRef.__Vfunc_top__DOT__dataMem__DOT__pmem_read__2__Vfuncout)
                                           : 0U);
    vlSelfRef.top__DOT___wbu_io_wbData = ((IData)(vlSelfRef.top__DOT__wbu__DOT___io_regWen_T)
                                           ? (((IData)(vlSelfRef.top__DOT__wbu__DOT___io_regWen_T)
                                                ? vlSelfRef.top__DOT___grf_io_rs1out
                                                : 0U) 
                                              + ((IData)(vlSelfRef.top__DOT___idu_io_is_add)
                                                  ? 
                                                 ((((IData)(vlSelfRef.top__DOT___exu_io_rs2_en) 
                                                    & (~ 
                                                       ((IData)(vlSelfRef.top__DOT___exu_io_rs2_addr_out) 
                                                        >> 4U))) 
                                                   & (0U 
                                                      != 
                                                      (0x0000001fU 
                                                       & (vlSelfRef.top__DOT___instMem_rdata 
                                                          >> 0x00000014U))))
                                                   ? vlSelfRef.top__DOT__grf__DOT___GEN
                                                  [
                                                  (0x0000000fU 
                                                   & (IData)(vlSelfRef.top__DOT___exu_io_rs2_addr_out))]
                                                   : 0U)
                                                  : 
                                                 ((IData)(vlSelfRef.top__DOT___idu_io_is_addi)
                                                   ? vlSelfRef.top__DOT___idu_io_imm12
                                                   : 0U)))
                                           : ((IData)(vlSelfRef.top__DOT___exu_io_jalr_out)
                                               ? ((IData)(4U) 
                                                  + vlSelfRef.top__DOT__ifu__DOT__pcReg)
                                               : ((IData)(vlSelfRef.top__DOT___lsu_io_dmemRen)
                                                   ? 
                                                  ((IData)(vlSelfRef.top__DOT___idu_io_is_lw)
                                                    ? vlSelfRef.top__DOT___dataMem_rdata
                                                    : 
                                                   ((IData)(vlSelfRef.top__DOT___idu_io_is_lbu)
                                                     ? 
                                                    (0x000000ffU 
                                                     & (vlSelfRef.top__DOT___dataMem_rdata 
                                                        >> 
                                                        (0x00000018U 
                                                         & (vlSelfRef.top__DOT___lsu_io_dmemAddr 
                                                            << 3U))))
                                                     : 0U))
                                                   : 
                                                  ((0x37U 
                                                    == 
                                                    (0x0000007fU 
                                                     & vlSelfRef.top__DOT___instMem_rdata))
                                                    ? 
                                                   ((0x37U 
                                                     == 
                                                     (0x0000007fU 
                                                      & vlSelfRef.top__DOT___instMem_rdata))
                                                     ? 
                                                    (0xfffff000U 
                                                     & vlSelfRef.top__DOT___instMem_rdata)
                                                     : 0U)
                                                    : 0U))));
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
            VL_FATAL_MT("top.sv", 2, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("top.sv", 2, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 100 tries");
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

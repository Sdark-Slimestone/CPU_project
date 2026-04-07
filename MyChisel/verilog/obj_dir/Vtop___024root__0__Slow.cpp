// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

VL_ATTR_COLD void Vtop___024root___eval_static(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_static\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clock__0 = vlSelfRef.clock;
}

VL_ATTR_COLD void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vtop___024root___eval_final(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_final\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_settle(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_settle\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("top.sv", 2, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 100 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        vlSelfRef.__VstlPhaseResult = Vtop___024root___eval_phase__stl(vlSelf);
        vlSelfRef.__VstlFirstIteration = 0U;
    } while (vlSelfRef.__VstlPhaseResult);
}

VL_ATTR_COLD void Vtop___024root___eval_triggers_vec__stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers_vec__stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
}

VL_ATTR_COLD bool Vtop___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vtop___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vtop___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___trigger_anySet__stl\n"); );
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

void Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_read_TOP(IData/*31:0*/ addr, IData/*31:0*/ &pmem_read__Vfuncrtn);
extern "C" void sim_finish();
void Vtop___024root____Vdpiimwrap_top__DOT__instMem__DOT__pmem_write_TOP(IData/*31:0*/ addr, IData/*31:0*/ data, CData/*7:0*/ mask);

VL_ATTR_COLD void Vtop___024root___stl_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___stl_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.top__DOT___ifu_io_pctogrf = ((IData)(4U) 
                                           + vlSelfRef.top__DOT__ifu__DOT__pcReg);
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

VL_ATTR_COLD void Vtop___024root___eval_stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        Vtop___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtop___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Vtop___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vtop___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vtop___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vtop___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge clock)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->clock = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5452235342940299466ull);
    vlSelf->reset = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9928399931838511862ull);
    vlSelf->io_clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10937873780045651504ull);
    vlSelf->io_reset = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6055738782511889183ull);
    vlSelf->top__DOT___dataMem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14059355147320882350ull);
    vlSelf->top__DOT___instMem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6965546483730634457ull);
    vlSelf->top__DOT___grf_io_rs1out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16335525794916807298ull);
    vlSelf->top__DOT___wbu_io_wbData = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3200796912473519894ull);
    vlSelf->top__DOT___lsu_io_dmemAddr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10784632259440815732ull);
    vlSelf->top__DOT___lsu_io_dmemWen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5916052640535906217ull);
    vlSelf->top__DOT___lsu_io_dmemRen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10873617612392818161ull);
    vlSelf->top__DOT___exu_io_rs2_en = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8452204196625430270ull);
    vlSelf->top__DOT___exu_io_rd_out = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 1790477245112467482ull);
    vlSelf->top__DOT___exu_io_jalr_out = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15638720456775885284ull);
    vlSelf->top__DOT___exu_io_rs1_addr_out = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 1179211574241633726ull);
    vlSelf->top__DOT___exu_io_rs2_addr_out = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 16127636897836788153ull);
    vlSelf->top__DOT___idu_io_is_addi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15213149625525327952ull);
    vlSelf->top__DOT___idu_io_is_add = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14865633967580470890ull);
    vlSelf->top__DOT___idu_io_is_lbu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1525873088433727309ull);
    vlSelf->top__DOT___idu_io_is_lw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2481355422362183801ull);
    vlSelf->top__DOT___idu_io_is_sb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8967295290823338344ull);
    vlSelf->top__DOT___idu_io_is_sw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16756864205483778809ull);
    vlSelf->top__DOT___idu_io_imm12 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4460370805014620328ull);
    vlSelf->top__DOT___ifu_io_pctogrf = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16416068941535248747ull);
    vlSelf->top__DOT__ifu__DOT__pcReg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18029227762866665639ull);
    vlSelf->top__DOT__idu__DOT__infoDecoder__DOT__imm12_pre_extended = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 9326665995559762156ull);
    vlSelf->top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_0 = 0;
    vlSelf->top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3 = 0;
    vlSelf->top__DOT__exu__DOT__aguMux__DOT____VdfgRegularize_h679c4643_0_0 = 0;
    vlSelf->top__DOT__exu__DOT__bruMux__DOT___GEN = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9275470072028175266ull);
    vlSelf->top__DOT__wbu__DOT___io_regWen_T = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13110169665348737045ull);
    vlSelf->top__DOT__grf__DOT__regs_0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5961025797839999775ull);
    vlSelf->top__DOT__grf__DOT__regs_1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5647333497618765059ull);
    vlSelf->top__DOT__grf__DOT__regs_2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11829087322106235373ull);
    vlSelf->top__DOT__grf__DOT__regs_3 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17008360697114964483ull);
    vlSelf->top__DOT__grf__DOT__regs_4 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9820947936515600678ull);
    vlSelf->top__DOT__grf__DOT__regs_5 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13087362804273446046ull);
    vlSelf->top__DOT__grf__DOT__regs_6 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16025194271456955079ull);
    vlSelf->top__DOT__grf__DOT__regs_7 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16716770544542430884ull);
    vlSelf->top__DOT__grf__DOT__regs_8 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10034218277475433086ull);
    vlSelf->top__DOT__grf__DOT__regs_9 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10725794550560905699ull);
    vlSelf->top__DOT__grf__DOT__regs_10 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8522602560987684441ull);
    vlSelf->top__DOT__grf__DOT__regs_11 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14110300475461219300ull);
    vlSelf->top__DOT__grf__DOT__regs_12 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8430552049700039551ull);
    vlSelf->top__DOT__grf__DOT__regs_13 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4423423991892881941ull);
    vlSelf->top__DOT__grf__DOT__regs_14 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9977372024966996416ull);
    vlSelf->top__DOT__grf__DOT__regs_15 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5970243967159869130ull);
    VL_SCOPED_RAND_RESET_W(512, vlSelf->top__DOT__grf__DOT___GEN, __VscopeHash, 1429410694585668247ull);
    vlSelf->__VdfgRegularize_he50b618e_0_0 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_1 = 0;
    vlSelf->__Vfunc_top__DOT__instMem__DOT__pmem_read__0__Vfuncout = 0;
    vlSelf->__Vfunc_top__DOT__dataMem__DOT__pmem_read__2__Vfuncout = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__clock__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}

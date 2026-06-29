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
            VL_FATAL_MT("../MyChisel/verilog/top.sv", 2, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 100 tries");
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

VL_ATTR_COLD void Vtop___024root___stl_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___stl_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    VlWide<16>/*511:0*/ __Vtemp_19;
    // Body
    vlSelfRef.io_debug_pc = vlSelfRef.top__DOT__ifu__DOT__pcReg;
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
    vlSelf->io_debug_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10350072731982309968ull);
    vlSelf->io_debug_regs_0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 999351465680123889ull);
    vlSelf->io_debug_regs_1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18033834768616077237ull);
    vlSelf->io_debug_regs_2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4321877655614352741ull);
    vlSelf->io_debug_regs_3 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7301261264927283813ull);
    vlSelf->io_debug_regs_4 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1356722703946994541ull);
    vlSelf->io_debug_regs_5 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1904604732382904874ull);
    vlSelf->io_debug_regs_6 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9624663976985056298ull);
    vlSelf->io_debug_regs_7 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12855735764104389610ull);
    vlSelf->io_debug_regs_8 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4228007444440641636ull);
    vlSelf->io_debug_regs_9 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1841011536386463862ull);
    vlSelf->io_debug_regs_10 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8455783693901063416ull);
    vlSelf->io_debug_regs_11 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4208828854362199439ull);
    vlSelf->io_debug_regs_12 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18045287202430769341ull);
    vlSelf->io_debug_regs_13 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2531229752547710709ull);
    vlSelf->io_debug_regs_14 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6902458778072304788ull);
    vlSelf->io_debug_regs_15 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13856698560832336442ull);
    vlSelf->io_debug_alu_src1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1546633362621520098ull);
    vlSelf->io_debug_alu_src2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17476564100633228304ull);
    vlSelf->io_debug_alu_res = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6794195812305466440ull);
    vlSelf->io_debug_regWen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 627919454197584206ull);
    vlSelf->io_debug_wbData = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6007019661007117638ull);
    vlSelf->io_debug_grf_rden = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8199932863445055972ull);
    vlSelf->io_debug_grf_rdaddr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 5812366255494292560ull);
    vlSelf->io_debug_grf_input = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12600662880647615683ull);
    vlSelf->io_debug_is_add = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4567567042938143928ull);
    vlSelf->io_debug_is_addi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12306242529361300291ull);
    vlSelf->io_debug_is_jalr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1292213968276675657ull);
    vlSelf->io_debug_is_lui = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7478528140644774545ull);
    vlSelf->io_debug_is_lbu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15597307205094917569ull);
    vlSelf->io_debug_is_lw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4227069855583408923ull);
    vlSelf->io_debug_is_sw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13989922604447886077ull);
    vlSelf->io_debug_is_sb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16029438352032163306ull);
    vlSelf->io_debug_is_ebreak = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2073741396114314498ull);
    vlSelf->io_debug_lsu_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7986055355180814067ull);
    vlSelf->io_debug_lsu_wen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4275202726133674258ull);
    vlSelf->io_debug_lsu_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12304424435085210550ull);
    vlSelf->io_debug_lsu_wmask = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 12769064339541744809ull);
    vlSelf->io_debug_lsu_ren = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17513047851208078687ull);
    vlSelf->io_debug_lsu_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13663671333615066852ull);
    vlSelf->io_debug_inst = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15041432708164082798ull);
    vlSelf->top__DOT___dataMem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14059355147320882350ull);
    vlSelf->top__DOT___instMem_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6965546483730634457ull);
    vlSelf->top__DOT___csr_io_csr_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16809869104273706259ull);
    vlSelf->top__DOT___grf_io_rs1out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16335525794916807298ull);
    vlSelf->top__DOT___grf_io_rs2out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7232741988184477766ull);
    vlSelf->top__DOT___wbu_io_wbData = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3200796912473519894ull);
    vlSelf->top__DOT___wbu_io_regWen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4295660534264252955ull);
    vlSelf->top__DOT___exu_io_agu_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11235543217695879816ull);
    vlSelf->top__DOT___exu_io_alu_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13187835157142772264ull);
    vlSelf->top__DOT___exu_io_rd_out = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 1790477245112467482ull);
    vlSelf->top__DOT___exu_io_branch_target = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16147852551026733481ull);
    vlSelf->top__DOT___exu_io_rs1_addr_out = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 1179211574241633726ull);
    vlSelf->top__DOT___exu_io_rs2_addr_out = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 16127636897836788153ull);
    vlSelf->top__DOT___idu_io_is_ecall = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9330184391737262621ull);
    vlSelf->top__DOT___idu_io_is_ebreak = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17976071435424928153ull);
    vlSelf->top__DOT___idu_io_imm_i = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1119935281213623945ull);
    vlSelf->top__DOT___ifu_io_pctogrf = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16416068941535248747ull);
    vlSelf->top__DOT__use_imm_csr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9140753468636641465ull);
    vlSelf->top__DOT____Vcellinp__csr__io_csr_op = 0;
    vlSelf->top__DOT____Vcellinp__csr__io_csr_wen = 0;
    vlSelf->top__DOT__ifu__DOT__pcReg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18029227762866665639ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_jalr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12202300127143806659ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_beq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14153526989969698998ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_bne = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18333152744186729178ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_blt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2745374746749879884ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_bge = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16975661286960903936ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_bltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4551695261358405391ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_bgeu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 214068391482772877ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_lb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15854779978633708999ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_lh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1636148382491712210ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_lw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4849296773356747449ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_lbu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7114952500344686034ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_lhu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11112403698028169037ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_sb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6166779215807383013ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_sh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7825247865666938424ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_sw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10337471860216355531ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_addi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7606132131379623638ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_slti = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3954202075402012744ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_sltiu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1974680378237287857ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_xori = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5211178789563840598ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_ori = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8385447202183233964ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_andi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13170309887203399136ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_slli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2530075892314789984ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_srli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5459384051473313034ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_srai = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4477311400929362536ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_add = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15071810710442224163ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_sub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8376256026394796604ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_sll = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15820363767769833732ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_slt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16006843250654252326ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_sltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13867032022656530195ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_xor = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9909660415082618027ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_srl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4556930381982552811ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_sra = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 312531000507594223ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_or = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17459511574430254017ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_and = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4479099326107318679ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_csrrw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16785397115524567848ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_csrrs = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1591366027882424866ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_csrrc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11848048936387091773ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_csrrwi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10710386636271063889ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_csrrsi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16372654192711167584ull);
    vlSelf->top__DOT__idu__DOT___instDecoder_io_is_csrrci = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2885015125806573396ull);
    vlSelf->top__DOT__idu__DOT__instDecoder__DOT__is_csr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12874501650785869201ull);
    vlSelf->top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1 = 0;
    vlSelf->top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2 = 0;
    vlSelf->top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3 = 0;
    vlSelf->top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4 = 0;
    vlSelf->top__DOT__idu__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6 = 0;
    vlSelf->top__DOT__idu__DOT__infoDecoder__DOT___csr_use_rs1_T = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1960285744801894858ull);
    vlSelf->top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2 = 0;
    vlSelf->top__DOT__idu__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3 = 0;
    vlSelf->top__DOT__exu__DOT___aluMux_io_source1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16388293643041485213ull);
    vlSelf->top__DOT__exu__DOT___aluMux_io_source2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5882601844344551203ull);
    vlSelf->top__DOT__exu__DOT__use_rs1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7623681854430551834ull);
    vlSelf->top__DOT__exu__DOT____Vcellinp__grfCtrl__io_use_rs2 = 0;
    vlSelf->top__DOT__exu__DOT____Vcellinp__aluMux__io_use_u_imm = 0;
    vlSelf->top__DOT__exu__DOT____Vcellinp__aluMux__io_use_rs2 = 0;
    vlSelf->top__DOT__exu__DOT____Vcellinp__alu__io_alu_op = 0;
    vlSelf->top__DOT__exu__DOT____Vcellinp__aguMux__io_is_store = 0;
    vlSelf->top__DOT__exu__DOT____Vcellinp__aguMux__io_is_load = 0;
    vlSelf->top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_0 = 0;
    vlSelf->top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_1 = 0;
    vlSelf->top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_2 = 0;
    vlSelf->top__DOT__exu__DOT____VdfgRegularize_haeb391f3_0_3 = 0;
    vlSelf->top__DOT__exu__DOT__bru__DOT___branch_cond_T_9 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11987883306207304368ull);
    vlSelf->top__DOT__lsu__DOT__byte_sel = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 6496859475714271287ull);
    vlSelf->top__DOT__lsu__DOT__half_sel = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16940672790174789684ull);
    vlSelf->top__DOT__lsu__DOT____VdfgRegularize_hac146698_0_0 = 0;
    vlSelf->top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_0 = 0;
    vlSelf->top__DOT__wbu__DOT____VdfgRegularize_h44ea2a3e_0_3 = 0;
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
    vlSelf->top__DOT__csr__DOT__mcycle_reg = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 8673800831865773840ull);
    vlSelf->top__DOT__csr__DOT__minstret_reg = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 841833537967629442ull);
    vlSelf->top__DOT__csr__DOT__mstatus_reg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1349995296528247700ull);
    vlSelf->top__DOT__csr__DOT__mtvec_reg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3753128838859354722ull);
    vlSelf->top__DOT__csr__DOT__mepc_reg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4254039594750412532ull);
    vlSelf->top__DOT__csr__DOT__mcause_reg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1141333297342614583ull);
    vlSelf->top__DOT__csr__DOT__mtval_reg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13025972608341843230ull);
    vlSelf->top__DOT__csr__DOT__mie_reg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12030320287472266994ull);
    vlSelf->top__DOT__csr__DOT__mip_reg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8488695250963029448ull);
    vlSelf->__VdfgRegularize_he50b618e_0_0 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_2 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_3 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_5 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_6 = 0;
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

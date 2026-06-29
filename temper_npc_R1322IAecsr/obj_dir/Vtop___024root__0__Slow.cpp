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

void Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__pmem_read_TOP(IData/*31:0*/ addr, IData/*31:0*/ &pmem_read__Vfuncrtn);
extern "C" void sim_finish();

VL_ATTR_COLD void Vtop___024root___stl_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___stl_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.io_debug_inst1_pc = vlSelfRef.top__DOT__ifu__DOT__pcReg;
    vlSelfRef.io_debug_inst2_pc = ((IData)(4U) + vlSelfRef.top__DOT__ifu__DOT__pcReg);
    vlSelfRef.io_debug_grf_regs_0 = vlSelfRef.top__DOT__grf__DOT__regs_0;
    vlSelfRef.io_debug_grf_regs_1 = vlSelfRef.top__DOT__grf__DOT__regs_1;
    vlSelfRef.io_debug_grf_regs_2 = vlSelfRef.top__DOT__grf__DOT__regs_2;
    vlSelfRef.io_debug_grf_regs_3 = vlSelfRef.top__DOT__grf__DOT__regs_3;
    vlSelfRef.io_debug_grf_regs_4 = vlSelfRef.top__DOT__grf__DOT__regs_4;
    vlSelfRef.io_debug_grf_regs_5 = vlSelfRef.top__DOT__grf__DOT__regs_5;
    vlSelfRef.io_debug_grf_regs_6 = vlSelfRef.top__DOT__grf__DOT__regs_6;
    vlSelfRef.io_debug_grf_regs_7 = vlSelfRef.top__DOT__grf__DOT__regs_7;
    vlSelfRef.io_debug_grf_regs_8 = vlSelfRef.top__DOT__grf__DOT__regs_8;
    vlSelfRef.io_debug_grf_regs_9 = vlSelfRef.top__DOT__grf__DOT__regs_9;
    vlSelfRef.io_debug_grf_regs_10 = vlSelfRef.top__DOT__grf__DOT__regs_10;
    vlSelfRef.io_debug_grf_regs_11 = vlSelfRef.top__DOT__grf__DOT__regs_11;
    vlSelfRef.io_debug_grf_regs_12 = vlSelfRef.top__DOT__grf__DOT__regs_12;
    vlSelfRef.io_debug_grf_regs_13 = vlSelfRef.top__DOT__grf__DOT__regs_13;
    vlSelfRef.io_debug_grf_regs_14 = vlSelfRef.top__DOT__grf__DOT__regs_14;
    vlSelfRef.io_debug_grf_regs_15 = vlSelfRef.top__DOT__grf__DOT__regs_15;
    vlSelfRef.io_debug_mcycle = vlSelfRef.top__DOT__csr__DOT__mcycle_reg;
    vlSelfRef.io_debug_minstret = vlSelfRef.top__DOT__csr__DOT__minstret_reg;
    vlSelfRef.io_debug_mstatus = vlSelfRef.top__DOT__csr__DOT__mstatus_reg;
    vlSelfRef.io_debug_mcause = vlSelfRef.top__DOT__csr__DOT__mcause_reg;
    vlSelfRef.io_debug_mepc = vlSelfRef.top__DOT__csr__DOT__mepc_reg;
    vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_val_nextpc 
        = ((IData)(4U) + vlSelfRef.top__DOT__ifu__DOT__pcReg);
    vlSelfRef.top__DOT___ifu_io_ifu_to_idu_inst2_nextpc 
        = ((IData)(8U) + vlSelfRef.top__DOT__ifu__DOT__pcReg);
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
    Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__pmem_read_TOP(
                                                                                ((IData)(4U) 
                                                                                + vlSelfRef.top__DOT__ifu__DOT__pcReg), vlSelfRef.__Vfunc_top__DOT__imem__DOT__memory__DOT__pmem_read__1__Vfuncout);
    vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
        = vlSelfRef.__Vfunc_top__DOT__imem__DOT__memory__DOT__pmem_read__1__Vfuncout;
    Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__pmem_read_TOP(vlSelfRef.top__DOT__ifu__DOT__pcReg, vlSelfRef.__Vfunc_top__DOT__imem__DOT__memory__DOT__pmem_read__0__Vfuncout);
    vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
        = vlSelfRef.__Vfunc_top__DOT__imem__DOT__memory__DOT__pmem_read__0__Vfuncout;
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT__need_imm_u 
        = ((0x17U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)) 
           | (0x37U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_addi 
        = (IData)((0x00000013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slti 
        = (IData)((0x00002013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltiu 
        = (IData)((0x00003013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xori 
        = (IData)((0x00004013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_ori 
        = (IData)((0x00006013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_andi 
        = (IData)((0x00007013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lb 
        = (IData)((3U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lh 
        = (IData)((0x00001003U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lw 
        = (IData)((0x00002003U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lbu 
        = (IData)((0x00004003U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lhu 
        = (IData)((0x00005003U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4 
        = (IData)((0x00000073U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.__VdfgRegularize_he50b618e_0_13 = (IData)(
                                                        (0U 
                                                         == 
                                                         (0xfe0f8f80U 
                                                          & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slt 
        = (IData)((0x00002033U == (0xfe00707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltu 
        = (IData)((0x00003033U == (0xfe00707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xor 
        = (IData)((0x00004033U == (0xfe00707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_or 
        = (IData)((0x00006033U == (0xfe00707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_and 
        = (IData)((0x00007033U == (0xfe00707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_jalr 
        = (IData)((0x00000067U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sw 
        = (IData)((0x00002023U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sb 
        = (IData)((0x00000023U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sh 
        = (IData)((0x00001023U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1 
        = (IData)((0x00005013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_beq 
        = (IData)((0x00000063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bne 
        = (IData)((0x00001063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_blt 
        = (IData)((0x00004063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bge 
        = (IData)((0x00005063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bltu 
        = (IData)((0x00006063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bgeu 
        = (IData)((0x00007063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6 
        = (IData)((0x00001000U == (0xfe007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2 
        = (IData)((0x00000033U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3 
        = (IData)((0x00005033U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT__is_csr 
        = ((0x73U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)) 
           & (0U != (7U & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                           >> 0x0000000cU))));
    vlSelfRef.io_debug_inst1 = vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1;
    vlSelfRef.top__DOT___csr_io_csr_rdata = ((0x0300U 
                                              == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                  >> 0x00000014U))
                                              ? vlSelfRef.top__DOT__csr__DOT__mstatus_reg
                                              : ((0x0301U 
                                                  == 
                                                  (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                   >> 0x00000014U))
                                                  ? 0U
                                                  : 
                                                 ((0x0304U 
                                                   == 
                                                   (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                    >> 0x00000014U))
                                                   ? vlSelfRef.top__DOT__csr__DOT__mie_reg
                                                   : 
                                                  ((0x0305U 
                                                    == 
                                                    (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                     >> 0x00000014U))
                                                    ? vlSelfRef.top__DOT__csr__DOT__mtvec_reg
                                                    : 
                                                   ((0x0340U 
                                                     == 
                                                     (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                      >> 0x00000014U))
                                                     ? 0U
                                                     : 
                                                    ((0x0341U 
                                                      == 
                                                      (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                       >> 0x00000014U))
                                                      ? vlSelfRef.top__DOT__csr__DOT__mepc_reg
                                                      : 
                                                     ((0x0342U 
                                                       == 
                                                       (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                        >> 0x00000014U))
                                                       ? vlSelfRef.top__DOT__csr__DOT__mcause_reg
                                                       : 
                                                      ((0x0343U 
                                                        == 
                                                        (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                         >> 0x00000014U))
                                                        ? vlSelfRef.top__DOT__csr__DOT__mtval_reg
                                                        : 
                                                       ((0x0344U 
                                                         == 
                                                         (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                          >> 0x00000014U))
                                                         ? vlSelfRef.top__DOT__csr__DOT__mip_reg
                                                         : 
                                                        ((0x0b00U 
                                                          == 
                                                          (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                           >> 0x00000014U))
                                                          ? (IData)(vlSelfRef.top__DOT__csr__DOT__mcycle_reg)
                                                          : 
                                                         ((0x0b80U 
                                                           == 
                                                           (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                            >> 0x00000014U))
                                                           ? (IData)(
                                                                     (vlSelfRef.top__DOT__csr__DOT__mcycle_reg 
                                                                      >> 0x00000020U))
                                                           : 
                                                          ((0x0b02U 
                                                            == 
                                                            (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                             >> 0x00000014U))
                                                            ? (IData)(vlSelfRef.top__DOT__csr__DOT__minstret_reg)
                                                            : 
                                                           ((0x0b82U 
                                                             == 
                                                             (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                              >> 0x00000014U))
                                                             ? (IData)(
                                                                       (vlSelfRef.top__DOT__csr__DOT__minstret_reg 
                                                                        >> 0x00000020U))
                                                             : 
                                                            ((0x0f11U 
                                                              == 
                                                              (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                               >> 0x00000014U))
                                                              ? 0x79737978U
                                                              : 
                                                             ((0x0f12U 
                                                               == 
                                                               (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                                >> 0x00000014U))
                                                               ? 0x018a9e3bU
                                                               : 0U)))))))))))))));
    vlSelfRef.top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_2 
        = ((0x17U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
           | (0x6fU == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sw 
        = (IData)((0x00002023U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sb 
        = (IData)((0x00000023U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sh 
        = (IData)((0x00001023U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_beq 
        = (IData)((0x00000063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bne 
        = (IData)((0x00001063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_blt 
        = (IData)((0x00004063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bge 
        = (IData)((0x00005063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bltu 
        = (IData)((0x00006063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bgeu 
        = (IData)((0x00007063U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT__need_imm_u 
        = ((0x17U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
           | (0x37U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slt 
        = (IData)((0x00002033U == (0xfe00707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltu 
        = (IData)((0x00003033U == (0xfe00707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xor 
        = (IData)((0x00004033U == (0xfe00707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_or 
        = (IData)((0x00006033U == (0xfe00707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_and 
        = (IData)((0x00007033U == (0xfe00707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_addi 
        = (IData)((0x00000013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slti 
        = (IData)((0x00002013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltiu 
        = (IData)((0x00003013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xori 
        = (IData)((0x00004013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_ori 
        = (IData)((0x00006013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_andi 
        = (IData)((0x00007013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4 
        = (IData)((0x00000073U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.__VdfgRegularize_he50b618e_0_16 = (IData)(
                                                        (0U 
                                                         == 
                                                         (0xfe0f8f80U 
                                                          & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_jalr 
        = (IData)((0x00000067U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lb 
        = (IData)((3U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lh 
        = (IData)((0x00001003U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lw 
        = (IData)((0x00002003U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lbu 
        = (IData)((0x00004003U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lhu 
        = (IData)((0x00005003U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2 
        = (IData)((0x00000033U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3 
        = (IData)((0x00005033U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6 
        = (IData)((0x00001000U == (0xfe007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1 
        = (IData)((0x00005013U == (0x0000707fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT__is_csr 
        = ((0x73U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
           & (0U != (7U & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                           >> 0x0000000cU))));
    vlSelfRef.top__DOT__idu__DOT___dec2_io_is_ebreak 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4) 
           & ((1U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                      >> 0x00000014U)) & (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_13)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_and) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_or));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_1 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sh) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sw));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srai 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1) 
           & (0x20U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                        >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srli 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1) 
           & (0U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                     >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_4 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_beq) 
           | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bne) 
              | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bge) 
                 | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_blt) 
                    | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bgeu) 
                       | (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bltu))))));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slli 
        = ((0x13U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sll 
        = ((0x33U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_add 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2) 
           & (0U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                     >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sub 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2) 
           & (0x20U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                        >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srl 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3) 
           & (0U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                     >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sra 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3) 
           & (0x20U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                        >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrwi 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT__is_csr) 
           & (0x00005000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrsi 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT__is_csr) 
           & (0x00006000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrci 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT__is_csr) 
           & (0x00007000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrc 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT__is_csr) 
           & (0x00003000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrw 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT__is_csr) 
           & (0x00001000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrs 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT__is_csr) 
           & (0x00002000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_1 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sh) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sw));
    vlSelfRef.top__DOT__exu1__DOT__is_branch = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_beq) 
                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bne) 
                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bge) 
                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_blt) 
                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bgeu) 
                                                            | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bltu))))));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_and) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_or));
    vlSelfRef.top__DOT__idu__DOT___dec1_io_is_mret 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4) 
           & ((0x00c0U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                           >> 0x00000014U)) & (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_16)));
    vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ecall 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4) 
           & ((0U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                      >> 0x00000014U)) & (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_16)));
    vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ebreak 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4) 
           & ((1U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                      >> 0x00000014U)) & (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_16)));
    vlSelfRef.top__DOT__exu1__DOT__is_jump = ((0x6fU 
                                               == (0x0000007fU 
                                                   & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                                              | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_jalr));
    vlSelfRef.top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_1 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lb) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lh));
    vlSelfRef.__VdfgRegularize_he50b618e_0_10 = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lw) 
                                                 | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lbu) 
                                                    | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lhu)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_add 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2) 
           & (0U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                     >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sub 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2) 
           & (0x20U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                        >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srl 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3) 
           & (0U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                     >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sra 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3) 
           & (0x20U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                        >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sll 
        = ((0x33U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slli 
        = ((0x13U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srai 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1) 
           & (0x20U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                        >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srli 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1) 
           & (0U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                     >> 0x00000019U)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrwi 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT__is_csr) 
           & (0x00005000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrsi 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT__is_csr) 
           & (0x00006000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrci 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT__is_csr) 
           & (0x00007000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrc 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT__is_csr) 
           & (0x00003000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrw 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT__is_csr) 
           & (0x00001000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrs 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT__is_csr) 
           & (0x00002000U == (0x00007000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srai) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srli));
    vlSelfRef.__VdfgRegularize_he50b618e_0_11 = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_4) 
                                                 | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sb) 
                                                    | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_1) 
                                                       | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_add) 
                                                          | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sub) 
                                                             | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sll) 
                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slt) 
                                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltu) 
                                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xor) 
                                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sra) 
                                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srl) 
                                                                               | (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3))))))))))));
    vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT___csr_use_rs1_T 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrs) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrw));
    vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_wen = 
        ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sb) 
         | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_1));
    vlSelfRef.top__DOT__exu1__DOT__is_load = ((IData)(vlSelfRef.top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_1) 
                                              | (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_10));
    vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_ren = 
        ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lb) 
         | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lh) 
            | (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_10)));
    vlSelfRef.__VdfgRegularize_he50b618e_0_3 = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_add) 
                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sub) 
                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sll) 
                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slt) 
                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltu) 
                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xor) 
                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sra) 
                                                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srl) 
                                                                     | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3)))))))));
    vlSelfRef.top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_0 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slli) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srli));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srai) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srli));
    vlSelfRef.top__DOT__use_imm_csr = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrwi) 
                                       | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrci) 
                                          | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrsi)));
    vlSelfRef.top__DOT____Vcellinp__csr__io_csr_op 
        = (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrs) 
            | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrsi))
            ? 1U : (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrc) 
                     | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrci)) 
                    << 1U));
    vlSelfRef.top__DOT__idu__DOT__isControl1 = ((0x6fU 
                                                 == 
                                                 (0x0000007fU 
                                                  & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_beq) 
                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_jalr) 
                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_blt) 
                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bne) 
                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bge) 
                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bltu) 
                                                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bgeu) 
                                                                     | ((IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ebreak) 
                                                                        | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrs) 
                                                                           | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrw) 
                                                                              | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrc) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrwi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrci) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrsi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ecall) 
                                                                                | (IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_is_mret)))))))))))))))));
    vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT___csr_use_rs1_T 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrs) 
           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrw));
    vlSelfRef.top__DOT__idu__DOT___dec2_io_rs2 = ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_11)
                                                   ? 
                                                  (0x0000001fU 
                                                   & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                      >> 0x00000014U))
                                                   : 0U);
    vlSelfRef.__VdfgRegularize_he50b618e_0_12 = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT__need_imm_u) 
                                                 | ((0x6fU 
                                                     == 
                                                     (0x0000007fU 
                                                      & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)) 
                                                    | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_jalr) 
                                                       | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lb) 
                                                          | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lh) 
                                                             | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lw) 
                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lbu) 
                                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lhu) 
                                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_addi) 
                                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slti) 
                                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltiu) 
                                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xori) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_andi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_ori) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slli) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srli) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_add) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srai) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sll) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sub) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slt) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srl) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xor) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_or) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sra) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_and) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT___csr_use_rs1_T) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrc) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrwi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrci) 
                                                                                | (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrsi))))))))))))))))))))))))))))))));
    vlSelfRef.__VdfgRegularize_he50b618e_0_4 = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_beq) 
                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_jalr) 
                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_blt) 
                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bne) 
                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bge) 
                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bltu) 
                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bgeu) 
                                                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lb) 
                                                                     | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lh) 
                                                                        | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lw) 
                                                                           | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lbu) 
                                                                              | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lhu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sb) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sh) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_addi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sw) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slti) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltiu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_ori) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xori) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_andi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slli) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_add) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sub) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sll) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slt) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xor) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sra) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srl) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT___csr_use_rs1_T) 
                                                                                | (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrc))))))))))))))))))))))))))))))))));
    vlSelfRef.io_debug_lsu1_is_store = vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_wen;
    vlSelfRef.io_debug_lsu1_is_load = vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_ren;
    vlSelfRef.__VdfgRegularize_he50b618e_0_5 = (((~ (IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ebreak)) 
                                                 & ((0x37U 
                                                     == 
                                                     (0x0000007fU 
                                                      & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                                                    | ((IData)(vlSelfRef.top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_2) 
                                                       | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_addi) 
                                                          | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_jalr) 
                                                             | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slti) 
                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltiu) 
                                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_ori) 
                                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xori) 
                                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_andi) 
                                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slli) 
                                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srai) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srli) 
                                                                                | (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_3)))))))))))))) 
                                                | (IData)(vlSelfRef.top__DOT__exu1__DOT__is_load));
    vlSelfRef.__VdfgRegularize_he50b618e_0_14 = ((IData)(vlSelfRef.top__DOT__exu1__DOT__is_branch) 
                                                 | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sb) 
                                                    | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_1) 
                                                       | (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_3))));
    vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm 
        = (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_jalr) 
            | ((IData)(vlSelfRef.top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_1) 
               | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lbu) 
                  | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lw) 
                     | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_addi) 
                        | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lhu) 
                           | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slti) 
                              | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltiu) 
                                 | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_ori) 
                                    | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xori) 
                                       | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_andi) 
                                          | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slli) 
                                             | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2)))))))))))))
            ? (((- (IData)((vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                            >> 0x0000001fU))) << 0x0000000cU) 
               | (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                  >> 0x00000014U)) : ((IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_wen)
                                       ? (((- (IData)(
                                                      (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                       >> 0x0000001fU))) 
                                           << 0x0000000cU) 
                                          | ((0x00000fe0U 
                                              & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                 >> 0x00000014U)) 
                                             | (0x0000001fU 
                                                & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                   >> 7U))))
                                       : ((IData)(vlSelfRef.top__DOT__exu1__DOT__is_branch)
                                           ? (((- (IData)(
                                                          (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                           >> 0x0000001fU))) 
                                               << 0x0000000cU) 
                                              | ((0x00000800U 
                                                  & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                     << 4U)) 
                                                 | ((0x000007e0U 
                                                     & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                        >> 0x00000014U)) 
                                                    | (0x0000001eU 
                                                       & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                          >> 7U)))))
                                           : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT__need_imm_u)
                                               ? (0xfffff000U 
                                                  & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)
                                               : ((0x6fU 
                                                   == 
                                                   (0x0000007fU 
                                                    & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1))
                                                   ? 
                                                  (((- (IData)(
                                                               (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                                >> 0x0000001fU))) 
                                                    << 0x00000014U) 
                                                   | ((((0x000001feU 
                                                         & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                            >> 0x0000000bU)) 
                                                        | (1U 
                                                           & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                              >> 0x00000014U))) 
                                                       << 0x0000000bU) 
                                                      | (0x000007feU 
                                                         & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                                            >> 0x00000014U))))
                                                   : 0U)))));
    vlSelfRef.__VdfgRegularize_he50b618e_0_2 = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_beq) 
                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_jalr) 
                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_blt) 
                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bne) 
                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bge) 
                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bltu) 
                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bgeu) 
                                                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lb) 
                                                                     | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lh) 
                                                                        | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lw) 
                                                                           | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lbu) 
                                                                              | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lhu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sb) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sh) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_addi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sw) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slti) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltiu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_ori) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xori) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_andi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slli) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_add) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sub) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sll) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slt) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xor) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sra) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srl) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT___csr_use_rs1_T) 
                                                                                | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrc))))))))))))))))))))))))))))))))));
    vlSelfRef.top__DOT____VdfgRegularize_he2b63832_0_1 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT___csr_use_rs1_T) 
           | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrc) 
              | (IData)(vlSelfRef.top__DOT__use_imm_csr)));
    vlSelfRef.top__DOT__idu__DOT___dec2_io_rs1 = ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_4)
                                                   ? 
                                                  (0x0000001fU 
                                                   & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                      >> 0x0000000fU))
                                                   : 0U);
    vlSelfRef.top__DOT___idu_io_idu_to_grf_dec1_redreg_rs2 
        = ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_14)
            ? (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                              >> 0x00000014U)) : 0U);
    vlSelfRef.top__DOT__exu1__DOT___target_branch_T 
        = (vlSelfRef.top__DOT__ifu__DOT__pcReg + vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm);
    vlSelfRef.top__DOT___idu_io_idu_to_grf_dec1_redreg_rs1 
        = ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_2)
            ? (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                              >> 0x0000000fU)) : 0U);
    vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen 
        = ((IData)(vlSelfRef.top__DOT____VdfgRegularize_he2b63832_0_1) 
           & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                    >> 7U))));
    vlSelfRef.top__DOT____Vcellinp__csr__io_csr_wen 
        = ((IData)(vlSelfRef.top__DOT____VdfgRegularize_he2b63832_0_1) 
           & (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrw) 
               | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrwi)) 
              | (0U != (0x000f8000U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1))));
    vlSelfRef.__VdfgRegularize_he50b618e_0_15 = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT__need_imm_u) 
                                                 | ((0x6fU 
                                                     == 
                                                     (0x0000007fU 
                                                      & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                                                    | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_jalr) 
                                                       | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lb) 
                                                          | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lh) 
                                                             | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lw) 
                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lbu) 
                                                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lhu) 
                                                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_addi) 
                                                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slti) 
                                                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltiu) 
                                                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xori) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_andi) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_ori) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slli) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srli) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_add) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srai) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sll) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sub) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slt) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltu) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srl) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xor) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_or) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sra) 
                                                                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_and) 
                                                                                | (IData)(vlSelfRef.top__DOT____VdfgRegularize_he2b63832_0_1))))))))))))))))))))))))))));
    vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value 
        = (((~ ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_grf_dec1_redreg_rs2) 
                >> 4U)) & ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_14) 
                           & (0U != (0x0000001fU & 
                                     (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                      >> 0x00000014U)))))
            ? vlSelfRef.top__DOT__grf__DOT___GEN[(0x0000000fU 
                                                  & (IData)(vlSelfRef.top__DOT___idu_io_idu_to_grf_dec1_redreg_rs2))]
            : 0U);
    vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
        = (((~ ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_grf_dec1_redreg_rs1) 
                >> 4U)) & ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_2) 
                           & (0U != (0x0000001fU & 
                                     (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                      >> 0x0000000fU)))))
            ? vlSelfRef.top__DOT__grf__DOT___GEN[(0x0000000fU 
                                                  & (IData)(vlSelfRef.top__DOT___idu_io_idu_to_grf_dec1_redreg_rs1))]
            : 0U);
    if (vlSelfRef.__VdfgRegularize_he50b618e_0_15) {
        vlSelfRef.top__DOT__idu__DOT___dec1_io_rd = 
            (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                            >> 7U));
        vlSelfRef.__VdfgRegularize_he50b618e_0_17 = 
            (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                   >> 7U)));
    } else {
        vlSelfRef.top__DOT__idu__DOT___dec1_io_rd = 0U;
        vlSelfRef.__VdfgRegularize_he50b618e_0_17 = 0U;
    }
    vlSelfRef.io_debug_exu1_alu_source2 = (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_addi) 
                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slti) 
                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltiu) 
                                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xori) 
                                                     | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_andi) 
                                                        | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_ori) 
                                                           | ((IData)(vlSelfRef.top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_0) 
                                                              | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_jalr) 
                                                                 | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srai) 
                                                                    | ((IData)(vlSelfRef.top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_1) 
                                                                       | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lbu) 
                                                                          | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lw) 
                                                                             | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lhu) 
                                                                                | (IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_wen))))))))))))))
                                            ? vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm
                                            : vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value);
    vlSelfRef.top__DOT__exu1__DOT__shamt = (0x0000001fU 
                                            & (((IData)(vlSelfRef.top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_0) 
                                                | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srai))
                                                ? vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm
                                                : vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value));
    vlSelfRef.io_debug_exu1_alu_source1 = ((IData)(vlSelfRef.top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_2)
                                            ? vlSelfRef.top__DOT__ifu__DOT__pcReg
                                            : ((0x37U 
                                                == 
                                                (0x0000007fU 
                                                 & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1))
                                                ? 0U
                                                : vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value));
    vlSelfRef.top__DOT___exu1_io_exu_to_ifu_take_branch 
        = ((IData)(vlSelfRef.top__DOT__exu1__DOT__is_jump) 
           | ((IData)(vlSelfRef.top__DOT__exu1__DOT__is_branch) 
              & ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_beq)
                  ? (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                     == vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value)
                  : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bne)
                      ? (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                         != vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value)
                      : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_blt)
                          ? VL_LTS_III(32, vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value, vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value)
                          : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bge)
                              ? VL_GTES_III(32, vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value, vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value)
                              : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bltu)
                                  ? (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                                     < vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value)
                                  : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bgeu) 
                                     & (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                                        >= vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value)))))))));
    vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr 
        = (vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm 
           + vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value);
    if (vlSelfRef.__VdfgRegularize_he50b618e_0_5) {
        vlSelfRef.top__DOT___lsu1_io_lsu_to_wbu_rd 
            = vlSelfRef.top__DOT__idu__DOT___dec1_io_rd;
        vlSelfRef.__VdfgRegularize_he50b618e_0_8 = vlSelfRef.__VdfgRegularize_he50b618e_0_17;
    } else {
        vlSelfRef.top__DOT___lsu1_io_lsu_to_wbu_rd = 0U;
        vlSelfRef.__VdfgRegularize_he50b618e_0_8 = 0U;
    }
    vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__isControl1) 
           | (((IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_ren) 
               & ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_17) 
                  & (((IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_rd) 
                      == (IData)(vlSelfRef.top__DOT__idu__DOT___dec2_io_rs1)) 
                     | ((IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_rd) 
                        == (IData)(vlSelfRef.top__DOT__idu__DOT___dec2_io_rs2))))) 
              | ((~ (IData)(vlSelfRef.top__DOT__idu__DOT__isControl1)) 
                 & ((0x6fU == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)) 
                    | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_beq) 
                       | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_jalr) 
                          | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_blt) 
                             | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bne) 
                                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bge) 
                                   | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bltu) 
                                      | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bgeu) 
                                         | ((IData)(vlSelfRef.top__DOT__idu__DOT___dec2_io_is_ebreak) 
                                            | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrs) 
                                               | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrw) 
                                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrc) 
                                                     | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrwi) 
                                                        | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrci) 
                                                           | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrsi) 
                                                              | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4) 
                                                                 & (((0U 
                                                                      == 
                                                                      (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                                       >> 0x00000014U)) 
                                                                     | (0x00c0U 
                                                                        == 
                                                                        (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                                         >> 0x00000014U))) 
                                                                    & (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_13)))))))))))))))))))));
    vlSelfRef.io_debug_exu1_alu_out = ((0x37U == (0x0000007fU 
                                                  & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1))
                                        ? vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm
                                        : ((0x17U == 
                                            (0x0000007fU 
                                             & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1))
                                            ? vlSelfRef.top__DOT__exu1__DOT___target_branch_T
                                            : (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_add) 
                                                | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_addi))
                                                ? (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                                                   + 
                                                   ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_addi)
                                                     ? vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm
                                                     : vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value))
                                                : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sub)
                                                    ? 
                                                   (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                                                    - vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value)
                                                    : 
                                                   (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slt) 
                                                     | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slti))
                                                     ? 
                                                    VL_LTS_III(32, vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value, 
                                                               ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slti)
                                                                 ? vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm
                                                                 : vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value))
                                                     : 
                                                    (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltiu) 
                                                      | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltu))
                                                      ? 
                                                     (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                                                      < 
                                                      ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltiu)
                                                        ? vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm
                                                        : vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value))
                                                      : 
                                                     (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xor) 
                                                       | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xori))
                                                       ? 
                                                      (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                                                       ^ 
                                                       ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xori)
                                                         ? vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm
                                                         : vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value))
                                                       : 
                                                      (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_or) 
                                                        | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_ori))
                                                        ? 
                                                       (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                                                        | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_ori)
                                                            ? vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm
                                                            : vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value))
                                                        : 
                                                       (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_and) 
                                                         | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_andi))
                                                         ? 
                                                        (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                                                         & ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_andi)
                                                             ? vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_imm
                                                             : vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value))
                                                         : 
                                                        (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sll) 
                                                          | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slli))
                                                          ? 
                                                         (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                                                          << (IData)(vlSelfRef.top__DOT__exu1__DOT__shamt))
                                                          : 
                                                         (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srl) 
                                                           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srli))
                                                           ? 
                                                          (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value 
                                                           >> (IData)(vlSelfRef.top__DOT__exu1__DOT__shamt))
                                                           : 
                                                          (((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sra) 
                                                            | (IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srai))
                                                            ? 
                                                           VL_SHIFTRS_III(32,32,5, vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value, (IData)(vlSelfRef.top__DOT__exu1__DOT__shamt))
                                                            : 
                                                           ((IData)(vlSelfRef.top__DOT__exu1__DOT__is_jump)
                                                             ? 
                                                            ((IData)(4U) 
                                                             + vlSelfRef.top__DOT__ifu__DOT__pcReg)
                                                             : 0U)))))))))))));
    vlSelfRef.io_debug_exu1_agu_out = vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr;
    vlSelfRef.io_debug_lsu1_addr = vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr;
    vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_mask = 
        ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sw)
          ? 0x0000000fU : (0x0000000fU & ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sh)
                                           ? ((0U == 
                                               (3U 
                                                & vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr))
                                               ? 3U
                                               : ((2U 
                                                   == 
                                                   (3U 
                                                    & vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr))
                                                   ? 0x0cU
                                                   : 0U))
                                           : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sb)
                                               ? ((IData)(1U) 
                                                  << 
                                                  (3U 
                                                   & vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr))
                                               : 0U))));
    vlSelfRef.top__DOT__lsu1__DOT____VdfgRegularize_hac146698_0_0 
        = (vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value 
           << (0x00000018U & (vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr 
                              << 3U)));
    vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_1_load_data 
        = ((IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_ren)
            ? ([&]() {
                Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__pmem_read_TOP(
                                                                                (0xfffffffcU 
                                                                                & vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr), vlSelfRef.__Vfunc_top__DOT__dmem__DOT__memory__DOT__pmem_read__2__Vfuncout);
            }(), vlSelfRef.__Vfunc_top__DOT__dmem__DOT__memory__DOT__pmem_read__2__Vfuncout)
            : 0U);
    vlSelfRef.io_debug_wbu_rd1 = vlSelfRef.top__DOT___lsu1_io_lsu_to_wbu_rd;
    vlSelfRef.io_debug_wbu_valid1 = vlSelfRef.__VdfgRegularize_he50b618e_0_8;
    if (((IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ebreak) 
         | ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
            & (IData)(vlSelfRef.top__DOT__idu__DOT___dec2_io_is_ebreak)))) {
        sim_finish();
    }
    if (vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall) {
        vlSelfRef.io_debug_stall = 1U;
        vlSelfRef.io_debug_inst2 = 0U;
        vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_nextpc = 0U;
        vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data = 0U;
        vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val = 0U;
        vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm = 0U;
    } else {
        vlSelfRef.io_debug_stall = 0U;
        vlSelfRef.io_debug_inst2 = vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2;
        vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_nextpc 
            = ((IData)(8U) + vlSelfRef.top__DOT__ifu__DOT__pcReg);
        vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data 
            = (((~ ((IData)(vlSelfRef.top__DOT__idu__DOT___dec2_io_rs2) 
                    >> 4U)) & ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_11) 
                               & (0U != (0x0000001fU 
                                         & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                            >> 0x00000014U)))))
                ? vlSelfRef.top__DOT__grf__DOT___GEN
               [(0x0000000fU & (IData)(vlSelfRef.top__DOT__idu__DOT___dec2_io_rs2))]
                : 0U);
        vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
            = (((~ ((IData)(vlSelfRef.top__DOT__idu__DOT___dec2_io_rs1) 
                    >> 4U)) & ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_4) 
                               & (0U != (0x0000001fU 
                                         & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                            >> 0x0000000fU)))))
                ? vlSelfRef.top__DOT__grf__DOT___GEN
               [(0x0000000fU & (IData)(vlSelfRef.top__DOT__idu__DOT___dec2_io_rs1))]
                : 0U);
        vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm 
            = (((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_jalr) 
                | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lb) 
                   | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lh) 
                      | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lbu) 
                         | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lw) 
                            | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_addi) 
                               | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lhu) 
                                  | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slti) 
                                     | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltiu) 
                                        | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_ori) 
                                           | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xori) 
                                              | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_andi) 
                                                 | ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slli) 
                                                    | (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2))))))))))))))
                ? (((- (IData)((vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                >> 0x0000001fU))) << 0x0000000cU) 
                   | (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                      >> 0x00000014U)) : (((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sb) 
                                           | (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_1))
                                           ? (((- (IData)(
                                                          (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                           >> 0x0000001fU))) 
                                               << 0x0000000cU) 
                                              | ((0x00000fe0U 
                                                  & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                     >> 0x00000014U)) 
                                                 | (0x0000001fU 
                                                    & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                       >> 7U))))
                                           : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_4)
                                               ? ((
                                                   (- (IData)(
                                                              (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                               >> 0x0000001fU))) 
                                                   << 0x0000000cU) 
                                                  | ((0x00000800U 
                                                      & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                         << 4U)) 
                                                     | ((0x000007e0U 
                                                         & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                            >> 0x00000014U)) 
                                                        | (0x0000001eU 
                                                           & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                              >> 7U)))))
                                               : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT__need_imm_u)
                                                   ? 
                                                  (0xfffff000U 
                                                   & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)
                                                   : 
                                                  ((0x6fU 
                                                    == 
                                                    (0x0000007fU 
                                                     & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2))
                                                    ? 
                                                   (((- (IData)(
                                                                (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                                 >> 0x0000001fU))) 
                                                     << 0x00000014U) 
                                                    | ((((0x000001feU 
                                                          & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                             >> 0x0000000bU)) 
                                                         | (1U 
                                                            & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                               >> 0x00000014U))) 
                                                        << 0x0000000bU) 
                                                       | (0x000007feU 
                                                          & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                             >> 0x00000014U))))
                                                    : 0U)))));
    }
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_beq 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_beq));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bne 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bne));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_blt 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_blt));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bge 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bge));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bltu 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bltu));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bgeu 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bgeu));
    vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_sw 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sw));
    vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_sb 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sb));
    vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_sh 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sh));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_add 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_add));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sub 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sub));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sll 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sll));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slt 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slt));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sltu 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltu));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_xor 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xor));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srl 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srl));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sra 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sra));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_or 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_or));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_and 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_and));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_lui 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (0x37U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_auipc 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (0x17U == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_jal 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (0x6fU == (0x0000007fU & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2)));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_addi 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_addi));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slti 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slti));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sltiu 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltiu));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_xori 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xori));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_ori 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_ori));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_andi 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_andi));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_jalr 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_jalr));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srai 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srai));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slli 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slli));
    vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srli 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srli));
    vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lb 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lb));
    vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lh 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lh));
    vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lw 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lw));
    vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lbu 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lbu));
    vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lhu 
        = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
           & (IData)(vlSelfRef.top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lhu));
    vlSelfRef.io_debug_lsu1_store_mask = vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_mask;
    vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_store_data 
        = ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sb)
            ? vlSelfRef.top__DOT__lsu1__DOT____VdfgRegularize_hac146698_0_0
            : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sh)
                ? vlSelfRef.top__DOT__lsu1__DOT____VdfgRegularize_hac146698_0_0
                : vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value));
    vlSelfRef.io_debug_lsu1_read_origin = vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_1_load_data;
    if ((2U & vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr)) {
        vlSelfRef.top__DOT__lsu1__DOT__byte_sel = (0x000000ffU 
                                                   & ((1U 
                                                       & vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr)
                                                       ? 
                                                      (vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_1_load_data 
                                                       >> 0x00000018U)
                                                       : 
                                                      (vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_1_load_data 
                                                       >> 0x00000010U)));
        vlSelfRef.top__DOT__lsu1__DOT__half_sel = (0x0000ffffU 
                                                   & (vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_1_load_data 
                                                      >> 0x00000010U));
    } else {
        vlSelfRef.top__DOT__lsu1__DOT__byte_sel = (0x000000ffU 
                                                   & ((1U 
                                                       & vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr)
                                                       ? 
                                                      (vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_1_load_data 
                                                       >> 8U)
                                                       : vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_1_load_data));
        vlSelfRef.top__DOT__lsu1__DOT__half_sel = (0x0000ffffU 
                                                   & vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_1_load_data);
    }
    vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data 
        = ((IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_ren)
            ? ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lw)
                ? vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_1_load_data
                : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lh)
                    ? (((- (IData)((1U & ((IData)(vlSelfRef.top__DOT__lsu1__DOT__half_sel) 
                                          >> 0x0000000fU)))) 
                        << 0x00000010U) | (IData)(vlSelfRef.top__DOT__lsu1__DOT__half_sel))
                    : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lhu)
                        ? (IData)(vlSelfRef.top__DOT__lsu1__DOT__half_sel)
                        : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lb)
                            ? (((- (IData)((1U & ((IData)(vlSelfRef.top__DOT__lsu1__DOT__byte_sel) 
                                                  >> 7U)))) 
                                << 8U) | (IData)(vlSelfRef.top__DOT__lsu1__DOT__byte_sel))
                            : ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lbu)
                                ? (IData)(vlSelfRef.top__DOT__lsu1__DOT__byte_sel)
                                : 0U))))) : ((IData)(vlSelfRef.top__DOT__exu1__DOT__is_load)
                                              ? 0U : vlSelfRef.io_debug_exu1_alu_out));
    vlSelfRef.top__DOT__exu2__DOT__is_branch = ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_beq) 
                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bne) 
                                                   | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bge) 
                                                      | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_blt) 
                                                         | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bgeu) 
                                                            | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bltu))))));
    vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_wen = 
        ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_sb) 
         | ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_sh) 
            | (IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_sw)));
    vlSelfRef.top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_2 
        = ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_auipc) 
           | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_jal));
    vlSelfRef.top__DOT__exu2__DOT__is_jump = ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_jal) 
                                              | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_jalr));
    vlSelfRef.top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_0 
        = ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slli) 
           | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srli));
    vlSelfRef.top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_1 
        = ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lb) 
           | (IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lh));
    vlSelfRef.__VdfgRegularize_he50b618e_0_9 = ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lw) 
                                                | ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lbu) 
                                                   | (IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lhu)));
    vlSelfRef.top__DOT__exu2__DOT___target_branch_T 
        = ((vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_nextpc 
            - (IData)(4U)) + vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm);
    vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr 
        = (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm 
           + vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val);
    vlSelfRef.io_debug_lsu1_store_data_shifted = vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_store_data;
    vlSelfRef.io_debug_lsu2_is_store = vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_wen;
    vlSelfRef.io_debug_exu2_alu_source1 = ((IData)(vlSelfRef.top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_2)
                                            ? (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_nextpc 
                                               - (IData)(4U))
                                            : ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_lui)
                                                ? 0U
                                                : vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val));
    vlSelfRef.top__DOT__exu2__DOT__shamt = (0x0000001fU 
                                            & (((IData)(vlSelfRef.top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_0) 
                                                | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srai))
                                                ? vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm
                                                : vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data));
    vlSelfRef.io_debug_exu2_alu_source2 = (((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_addi) 
                                            | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slti) 
                                               | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sltiu) 
                                                  | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_xori) 
                                                     | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_andi) 
                                                        | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_ori) 
                                                           | ((IData)(vlSelfRef.top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_0) 
                                                              | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_jalr) 
                                                                 | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srai) 
                                                                    | ((IData)(vlSelfRef.top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_1) 
                                                                       | ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lbu) 
                                                                          | ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lw) 
                                                                             | ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lhu) 
                                                                                | (IData)(vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_wen))))))))))))))
                                            ? vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm
                                            : vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data);
    vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_ren = 
        ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lb) 
         | ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lh) 
            | (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_9)));
    vlSelfRef.top__DOT__exu2__DOT__is_load = ((IData)(vlSelfRef.top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_1) 
                                              | (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_9));
    vlSelfRef.io_debug_exu2_agu_out = vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr;
    vlSelfRef.io_debug_lsu2_addr = vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr;
    vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_mask = 
        ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_sw)
          ? 0x0000000fU : (0x0000000fU & ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_sh)
                                           ? ((0U == 
                                               (3U 
                                                & vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr))
                                               ? 3U
                                               : ((2U 
                                                   == 
                                                   (3U 
                                                    & vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr))
                                                   ? 0x0cU
                                                   : 0U))
                                           : ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_sb)
                                               ? ((IData)(1U) 
                                                  << 
                                                  (3U 
                                                   & vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr))
                                               : 0U))));
    vlSelfRef.top__DOT__lsu2__DOT____VdfgRegularize_hac146698_0_0 
        = (vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data 
           << (0x00000018U & (vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr 
                              << 3U)));
    vlSelfRef.io_debug_lsu1_final_wb_data = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
    vlSelfRef.io_debug_grf_input = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
    vlSelfRef.io_debug_exu2_alu_out = ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_lui)
                                        ? vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm
                                        : ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_auipc)
                                            ? vlSelfRef.top__DOT__exu2__DOT___target_branch_T
                                            : (((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_add) 
                                                | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_addi))
                                                ? (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                   + 
                                                   ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_addi)
                                                     ? vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm
                                                     : vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data))
                                                : ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sub)
                                                    ? 
                                                   (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                    - vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data)
                                                    : 
                                                   (((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slt) 
                                                     | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slti))
                                                     ? 
                                                    VL_LTS_III(32, vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val, 
                                                               ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slti)
                                                                 ? vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm
                                                                 : vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data))
                                                     : 
                                                    (((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sltiu) 
                                                      | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sltu))
                                                      ? 
                                                     (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                      < 
                                                      ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sltiu)
                                                        ? vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm
                                                        : vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data))
                                                      : 
                                                     (((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_xor) 
                                                       | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_xori))
                                                       ? 
                                                      (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                       ^ 
                                                       ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_xori)
                                                         ? vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm
                                                         : vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data))
                                                       : 
                                                      (((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_or) 
                                                        | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_ori))
                                                        ? 
                                                       (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                        | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_ori)
                                                            ? vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm
                                                            : vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data))
                                                        : 
                                                       (((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_and) 
                                                         | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_andi))
                                                         ? 
                                                        (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                         & ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_andi)
                                                             ? vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_imm
                                                             : vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data))
                                                         : 
                                                        (((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sll) 
                                                          | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slli))
                                                          ? 
                                                         (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                          << (IData)(vlSelfRef.top__DOT__exu2__DOT__shamt))
                                                          : 
                                                         (((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srl) 
                                                           | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srli))
                                                           ? 
                                                          (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                           >> (IData)(vlSelfRef.top__DOT__exu2__DOT__shamt))
                                                           : 
                                                          (((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sra) 
                                                            | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srai))
                                                            ? 
                                                           VL_SHIFTRS_III(32,32,5, vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val, (IData)(vlSelfRef.top__DOT__exu2__DOT__shamt))
                                                            : 
                                                           ((IData)(vlSelfRef.top__DOT__exu2__DOT__is_jump)
                                                             ? vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_nextpc
                                                             : 0U)))))))))))));
    vlSelfRef.io_debug_lsu2_is_load = vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_ren;
    vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_2_load_data 
        = ((IData)(vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_ren)
            ? ([&]() {
                Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__pmem_read_TOP(
                                                                                (0xfffffffcU 
                                                                                & vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr), vlSelfRef.__Vfunc_top__DOT__dmem__DOT__memory__DOT__pmem_read__3__Vfuncout);
            }(), vlSelfRef.__Vfunc_top__DOT__dmem__DOT__memory__DOT__pmem_read__3__Vfuncout)
            : 0U);
    vlSelfRef.__VdfgRegularize_he50b618e_0_6 = (((~ 
                                                  ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
                                                   & (IData)(vlSelfRef.top__DOT__idu__DOT___dec2_io_is_ebreak))) 
                                                 & ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_lui) 
                                                    | ((IData)(vlSelfRef.top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_2) 
                                                       | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_addi) 
                                                          | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_jalr) 
                                                             | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slti) 
                                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sltiu) 
                                                                   | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_ori) 
                                                                      | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_xori) 
                                                                         | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_andi) 
                                                                            | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slli) 
                                                                               | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srai) 
                                                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srli) 
                                                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_add) 
                                                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sub) 
                                                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sll) 
                                                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_slt) 
                                                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sltu) 
                                                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_xor) 
                                                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_sra) 
                                                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_srl) 
                                                                                | ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_and) 
                                                                                | (IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_or))))))))))))))))))))))) 
                                                | (IData)(vlSelfRef.top__DOT__exu2__DOT__is_load));
    vlSelfRef.io_debug_lsu2_store_mask = vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_mask;
    vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_store_data 
        = ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_sb)
            ? vlSelfRef.top__DOT__lsu2__DOT____VdfgRegularize_hac146698_0_0
            : ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_sh)
                ? vlSelfRef.top__DOT__lsu2__DOT____VdfgRegularize_hac146698_0_0
                : vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data));
    vlSelfRef.io_debug_lsu2_read_origin = vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_2_load_data;
    if ((2U & vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr)) {
        vlSelfRef.top__DOT__lsu2__DOT__byte_sel = (0x000000ffU 
                                                   & ((1U 
                                                       & vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr)
                                                       ? 
                                                      (vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_2_load_data 
                                                       >> 0x00000018U)
                                                       : 
                                                      (vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_2_load_data 
                                                       >> 0x00000010U)));
        vlSelfRef.top__DOT__lsu2__DOT__half_sel = (0x0000ffffU 
                                                   & (vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_2_load_data 
                                                      >> 0x00000010U));
    } else {
        vlSelfRef.top__DOT__lsu2__DOT__byte_sel = (0x000000ffU 
                                                   & ((1U 
                                                       & vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr)
                                                       ? 
                                                      (vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_2_load_data 
                                                       >> 8U)
                                                       : vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_2_load_data));
        vlSelfRef.top__DOT__lsu2__DOT__half_sel = (0x0000ffffU 
                                                   & vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_2_load_data);
    }
    vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data 
        = ((IData)(vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_ren)
            ? ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lw)
                ? vlSelfRef.top__DOT___dmem_io_dmem_to_lsu_2_load_data
                : ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lh)
                    ? (((- (IData)((1U & ((IData)(vlSelfRef.top__DOT__lsu2__DOT__half_sel) 
                                          >> 0x0000000fU)))) 
                        << 0x00000010U) | (IData)(vlSelfRef.top__DOT__lsu2__DOT__half_sel))
                    : ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lhu)
                        ? (IData)(vlSelfRef.top__DOT__lsu2__DOT__half_sel)
                        : ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lb)
                            ? (((- (IData)((1U & ((IData)(vlSelfRef.top__DOT__lsu2__DOT__byte_sel) 
                                                  >> 7U)))) 
                                << 8U) | (IData)(vlSelfRef.top__DOT__lsu2__DOT__byte_sel))
                            : ((IData)(vlSelfRef.top__DOT___exu2_io_exu_to_lsu_op_is_lbu)
                                ? (IData)(vlSelfRef.top__DOT__lsu2__DOT__byte_sel)
                                : 0U))))) : ((IData)(vlSelfRef.top__DOT__exu2__DOT__is_load)
                                              ? 0U : vlSelfRef.io_debug_exu2_alu_out));
    if (vlSelfRef.__VdfgRegularize_he50b618e_0_6) {
        vlSelfRef.io_debug_wbu_valid2 = ((~ (IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)) 
                                         & ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_12) 
                                            & (0U != 
                                               (0x0000001fU 
                                                & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                                   >> 7U)))));
        vlSelfRef.top__DOT___lsu2_io_lsu_to_wbu_rd 
            = ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)
                ? 0U : ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_12)
                         ? (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst2 
                                           >> 7U)) : 0U));
    } else {
        vlSelfRef.io_debug_wbu_valid2 = 0U;
        vlSelfRef.top__DOT___lsu2_io_lsu_to_wbu_rd = 0U;
    }
    vlSelfRef.io_debug_lsu2_store_data_shifted = vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_store_data;
    vlSelfRef.io_debug_wbu_rd2 = vlSelfRef.top__DOT___lsu2_io_lsu_to_wbu_rd;
    vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr 
        = ((IData)(vlSelfRef.io_debug_wbu_valid2) ? (IData)(vlSelfRef.top__DOT___lsu2_io_lsu_to_wbu_rd)
            : 0U);
    vlSelfRef.io_debug_wbu_conflict = ((IData)(vlSelfRef.io_debug_wbu_valid1) 
                                       & ((IData)(vlSelfRef.io_debug_wbu_valid2) 
                                          & ((IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_wbu_rd) 
                                             == (IData)(vlSelfRef.top__DOT___lsu2_io_lsu_to_wbu_rd))));
    vlSelfRef.io_debug_lsu2_final_wb_data = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
    vlSelfRef.io_debug_wbu_wr2_addr = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr;
    vlSelfRef.__VdfgRegularize_he50b618e_0_7 = ((~ (IData)(vlSelfRef.io_debug_wbu_conflict)) 
                                                & (IData)(vlSelfRef.io_debug_wbu_valid1));
    vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr 
        = ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_7)
            ? (IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_wbu_rd)
            : 0U);
    vlSelfRef.io_debug_wbu_wr1_addr = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr;
    vlSelfRef.io_debug_grf_rdaddr = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr;
    vlSelfRef.io_debug_grf_rden = ((~ ((IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr) 
                                       >> 4U)) & ((IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_7) 
                                                  & (IData)(vlSelfRef.__VdfgRegularize_he50b618e_0_8)));
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
    vlSelf->io_debug_inst1_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9107053074397004913ull);
    vlSelf->io_debug_inst2_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10695329756797406567ull);
    vlSelf->io_debug_inst1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2721256934299681963ull);
    vlSelf->io_debug_inst2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13823546535315925620ull);
    vlSelf->io_debug_stall = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15886880210754119587ull);
    vlSelf->io_debug_exu1_alu_out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9615170713812400826ull);
    vlSelf->io_debug_exu1_alu_source1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3289991509520712083ull);
    vlSelf->io_debug_exu1_alu_source2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12876413382748008024ull);
    vlSelf->io_debug_exu1_agu_out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14508390187293525080ull);
    vlSelf->io_debug_exu2_alu_out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15539544996718795053ull);
    vlSelf->io_debug_exu2_alu_source1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13311625009575781485ull);
    vlSelf->io_debug_exu2_alu_source2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7387311571473739392ull);
    vlSelf->io_debug_exu2_agu_out = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8444871669640078003ull);
    vlSelf->io_debug_lsu1_is_load = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9906307063127600900ull);
    vlSelf->io_debug_lsu1_is_store = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1887600688963087265ull);
    vlSelf->io_debug_lsu1_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12551663550018288521ull);
    vlSelf->io_debug_lsu1_read_origin = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11433403018472709080ull);
    vlSelf->io_debug_lsu1_final_wb_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12737099535197315540ull);
    vlSelf->io_debug_lsu1_store_mask = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17504018763875324879ull);
    vlSelf->io_debug_lsu1_store_data_shifted = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6672545066130875028ull);
    vlSelf->io_debug_lsu2_is_load = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2612308073332184925ull);
    vlSelf->io_debug_lsu2_is_store = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7709210522720190584ull);
    vlSelf->io_debug_lsu2_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11529293144755027829ull);
    vlSelf->io_debug_lsu2_read_origin = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12382081491136934302ull);
    vlSelf->io_debug_lsu2_final_wb_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3653423307048292448ull);
    vlSelf->io_debug_lsu2_store_mask = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 12262413538657935272ull);
    vlSelf->io_debug_lsu2_store_data_shifted = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15780686345721909880ull);
    vlSelf->io_debug_wbu_valid1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14642878606062235268ull);
    vlSelf->io_debug_wbu_valid2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15913676564120441417ull);
    vlSelf->io_debug_wbu_conflict = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5758191379168973117ull);
    vlSelf->io_debug_wbu_rd1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 15809178709128638457ull);
    vlSelf->io_debug_wbu_rd2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 1806793417684910572ull);
    vlSelf->io_debug_wbu_wr1_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 5333028140329355548ull);
    vlSelf->io_debug_wbu_wr2_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 2891703492105647404ull);
    vlSelf->io_debug_grf_regs_0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1516429540739288524ull);
    vlSelf->io_debug_grf_regs_1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9136399714284272369ull);
    vlSelf->io_debug_grf_regs_2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15648840098972985212ull);
    vlSelf->io_debug_grf_regs_3 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15106013216865910818ull);
    vlSelf->io_debug_grf_regs_4 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15484653282076681002ull);
    vlSelf->io_debug_grf_regs_5 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14941826399969608612ull);
    vlSelf->io_debug_grf_regs_6 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14297020724538661234ull);
    vlSelf->io_debug_grf_regs_7 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1827135117014064608ull);
    vlSelf->io_debug_grf_regs_8 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7875615312263704079ull);
    vlSelf->io_debug_grf_regs_9 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9247555277159049614ull);
    vlSelf->io_debug_grf_regs_10 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5914776880081635200ull);
    vlSelf->io_debug_grf_regs_11 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4114774657794762545ull);
    vlSelf->io_debug_grf_regs_12 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6740909982866754082ull);
    vlSelf->io_debug_grf_regs_13 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15766291878825425097ull);
    vlSelf->io_debug_grf_regs_14 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15917843370482773358ull);
    vlSelf->io_debug_grf_regs_15 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18026994859479429600ull);
    vlSelf->io_debug_grf_rden = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8199932863445055972ull);
    vlSelf->io_debug_grf_rdaddr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 5812366255494292560ull);
    vlSelf->io_debug_grf_input = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12600662880647615683ull);
    vlSelf->io_debug_mcycle = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 12733171097262851409ull);
    vlSelf->io_debug_minstret = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 5336748499956231459ull);
    vlSelf->io_debug_mstatus = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14092641832076603985ull);
    vlSelf->io_debug_mcause = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17428707543328560330ull);
    vlSelf->io_debug_mepc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8460049848738239835ull);
    vlSelf->top__DOT___dmem_io_dmem_to_lsu_1_load_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7099132119293077361ull);
    vlSelf->top__DOT___dmem_io_dmem_to_lsu_2_load_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 846505442593818901ull);
    vlSelf->top__DOT___imem_io_imem_to_ifu_inst1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1333393006428345424ull);
    vlSelf->top__DOT___imem_io_imem_to_ifu_inst2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17256732353447063804ull);
    vlSelf->top__DOT___csr_io_csr_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16809869104273706259ull);
    vlSelf->top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16048339296414377887ull);
    vlSelf->top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs2_value = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8331018650512975574ull);
    vlSelf->top__DOT___wbu_io_wbu_to_grf_wr1_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 7945928234260746396ull);
    vlSelf->top__DOT___wbu_io_wbu_to_grf_wr1_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3914608940616754998ull);
    vlSelf->top__DOT___wbu_io_wbu_to_grf_wr2_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 16606776202189328124ull);
    vlSelf->top__DOT___wbu_io_wbu_to_grf_wr2_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3140118232392425095ull);
    vlSelf->top__DOT___lsu2_io_lsu_to_dmem_store_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 816454224148702335ull);
    vlSelf->top__DOT___lsu2_io_lsu_to_dmem_mask = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6288904090483669818ull);
    vlSelf->top__DOT___lsu2_io_lsu_to_dmem_wen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8603869755261894205ull);
    vlSelf->top__DOT___lsu2_io_lsu_to_dmem_ren = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9798130728857672131ull);
    vlSelf->top__DOT___lsu2_io_lsu_to_wbu_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 9216956149399172284ull);
    vlSelf->top__DOT___lsu1_io_lsu_to_dmem_store_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2455751665809530651ull);
    vlSelf->top__DOT___lsu1_io_lsu_to_dmem_mask = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 8193274981415345101ull);
    vlSelf->top__DOT___lsu1_io_lsu_to_dmem_wen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8926727715611081585ull);
    vlSelf->top__DOT___lsu1_io_lsu_to_dmem_ren = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13231369537416218992ull);
    vlSelf->top__DOT___lsu1_io_lsu_to_wbu_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 3270950236330297309ull);
    vlSelf->top__DOT___exu2_io_exu_to_lsu_op_is_lb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10646537577567896756ull);
    vlSelf->top__DOT___exu2_io_exu_to_lsu_op_is_lh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17654318459179904653ull);
    vlSelf->top__DOT___exu2_io_exu_to_lsu_op_is_lw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8806210894144314456ull);
    vlSelf->top__DOT___exu2_io_exu_to_lsu_op_is_lbu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6252851555793322416ull);
    vlSelf->top__DOT___exu2_io_exu_to_lsu_op_is_lhu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8177050864748093898ull);
    vlSelf->top__DOT___exu2_io_exu_to_lsu_op_is_sb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7837891674915811694ull);
    vlSelf->top__DOT___exu2_io_exu_to_lsu_op_is_sh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1097805071780075184ull);
    vlSelf->top__DOT___exu2_io_exu_to_lsu_op_is_sw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4101600306696691735ull);
    vlSelf->top__DOT___exu2_io_exu_to_lsu_paddr_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15507269109541970408ull);
    vlSelf->top__DOT___exu2_io_exu_to_lsu_data_store_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3878320749058055939ull);
    vlSelf->top__DOT___exu1_io_exu_to_lsu_paddr_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4726828300274083517ull);
    vlSelf->top__DOT___exu1_io_exu_to_ifu_take_branch = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7777441091878023229ull);
    vlSelf->top__DOT___idu_io_idu_to_grf_dec1_redreg_rs1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 7613322276048061892ull);
    vlSelf->top__DOT___idu_io_idu_to_grf_dec1_redreg_rs2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 2218299137018177667ull);
    vlSelf->top__DOT___idu_io_idu_to_exu1_dec1_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15082996721080534940ull);
    vlSelf->top__DOT___idu_io_idu_to_exu1_dec1_val_nextpc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4099640637804584297ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_lui = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15636432308757975953ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_auipc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8657593959696232542ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_jal = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9776691404098359015ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_jalr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16632740454037630715ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_beq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9240305835892414885ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_bne = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 476896606081988883ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_blt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12079710720370292622ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_bge = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17560565414423222241ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_bltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 54952652652915534ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_bgeu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2549780960838419421ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_addi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4966658322360183447ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_slti = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 309602744818425622ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_sltiu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11380303176696646217ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_xori = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11987011702854439624ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_ori = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3468716821294890446ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_andi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14439225165394663262ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_slli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2357974918905757455ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_srli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15143424501662780874ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_srai = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16946031258303874179ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_add = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10422503396421256984ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_sub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7174008493926819460ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_sll = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7685836021061202246ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_slt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4487211734168897173ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_sltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2941355970073166362ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_xor = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9997791866602363192ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_srl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6258872977544986485ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_sra = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16631367714521842366ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_or = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16427182627088087885ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_op_is_and = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4982968298054041571ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_imm = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11796641571217869186ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3331587062148320673ull);
    vlSelf->top__DOT___idu_io_idu_to_exu2_dec2_val_nextpc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1483415545272382717ull);
    vlSelf->top__DOT___idu_io_idu_to_ifu_is_stall = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13480541304058387765ull);
    vlSelf->top__DOT___ifu_io_ifu_to_idu_inst2_nextpc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17571421658788627289ull);
    vlSelf->top__DOT__use_imm_csr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9140753468636641465ull);
    vlSelf->top__DOT____Vcellinp__grf__io_csr_to_grf_wen = 0;
    vlSelf->top__DOT____Vcellinp__csr__io_csr_op = 0;
    vlSelf->top__DOT____Vcellinp__csr__io_csr_wen = 0;
    vlSelf->top__DOT____VdfgRegularize_he2b63832_0_1 = 0;
    vlSelf->top__DOT__ifu__DOT__pcReg = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18029227762866665639ull);
    vlSelf->top__DOT__idu__DOT___dec2_io_is_ebreak = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17737109400454189468ull);
    vlSelf->top__DOT__idu__DOT___dec2_io_rs1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 7168008560236031421ull);
    vlSelf->top__DOT__idu__DOT___dec2_io_rs2 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 12206914744629841273ull);
    vlSelf->top__DOT__idu__DOT___dec1_io_is_ecall = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5298268118289387765ull);
    vlSelf->top__DOT__idu__DOT___dec1_io_is_mret = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2912111791438918538ull);
    vlSelf->top__DOT__idu__DOT___dec1_io_is_ebreak = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8821006871004487576ull);
    vlSelf->top__DOT__idu__DOT___dec1_io_rd = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 4274068076931290354ull);
    vlSelf->top__DOT__idu__DOT__isControl1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1823423364747707874ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_jalr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9498082658573050537ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_beq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14332556054284737708ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bne = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8444736122742741030ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_blt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2096263325453974670ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bge = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16178748912553054417ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6660873813491679710ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_bgeu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13111796618336745262ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11808598567829702269ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9236197562769191743ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16166849840512198381ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lbu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10998558742853476593ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_lhu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8116512743379334139ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 498499149732093977ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15901589487171711215ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14663330648829981811ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_addi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13621579958596610372ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slti = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6485512412278501552ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltiu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12907442772671311931ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xori = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4111136476406462627ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_ori = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7279814971991337216ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_andi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12123494959495538944ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3215991629549248270ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14902774616218603227ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srai = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15893200027570347490ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_add = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5355872343448328289ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8257812768531954861ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sll = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17459808182195253204ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_slt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9430597232881299909ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15758329580057372551ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_xor = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14716524939755286091ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_srl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6993991388473838177ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_sra = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7836345104607392450ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_or = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2148535068585086790ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_and = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8158763798566445838ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14882842790422235012ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrs = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16880811962424104009ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14880893417649389550ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrwi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 269348787710461595ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrsi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8700774707151457497ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_csrrci = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8011566373071501301ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT__is_csr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14726936938386262792ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1 = 0;
    vlSelf->top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2 = 0;
    vlSelf->top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3 = 0;
    vlSelf->top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4 = 0;
    vlSelf->top__DOT__idu__DOT__dec1__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6 = 0;
    vlSelf->top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT___csr_use_rs1_T = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12133478564757013244ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT__need_imm_u = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12453711737182516443ull);
    vlSelf->top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_1 = 0;
    vlSelf->top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2 = 0;
    vlSelf->top__DOT__idu__DOT__dec1__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3 = 0;
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_jalr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3178274526288790895ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_beq = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12570759017794991980ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bne = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8555923360679093353ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_blt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8184693809707101549ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bge = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17300554793046816605ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13949517933023029836ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_bgeu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12395909629535680264ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17764049093095412471ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3369372545062657670ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2050031623434676860ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lbu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12355590618412527809ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_lhu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11426054059141447044ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13960601451960133923ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sh = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15360012654979804916ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 142691633618007114ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_addi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9956743194674360309ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slti = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7352021670218499199ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltiu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6193390129553283921ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xori = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8351033786491046193ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_ori = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13573582019915207625ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_andi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11874873903064751134ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13507273089451689940ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srli = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4637451668224747295ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srai = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4968229533739352369ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_add = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6762708484948919851ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sub = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3208061536004178497ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sll = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6904661641947297805ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_slt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8237312788160490002ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sltu = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1583826484017995089ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_xor = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11460138102700105658ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_srl = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17416571431468697095ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_sra = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7887323217289216686ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_or = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13298961088095050394ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_and = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16133453144637088874ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrw = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7449418292085931097ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrs = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3937750635312380098ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6163804792754391028ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrwi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16140090920107988823ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrsi = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12477664991448921973ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT___instDecoder_io_is_csrrci = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15658630899103602764ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT__is_csr = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4565262025897708948ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_1 = 0;
    vlSelf->top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_2 = 0;
    vlSelf->top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_3 = 0;
    vlSelf->top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_4 = 0;
    vlSelf->top__DOT__idu__DOT__dec2__DOT__instDecoder__DOT____VdfgRegularize_h2df762c6_0_6 = 0;
    vlSelf->top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT___csr_use_rs1_T = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16628163023703552632ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT__need_imm_u = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12518614613394029648ull);
    vlSelf->top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_1 = 0;
    vlSelf->top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_2 = 0;
    vlSelf->top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_3 = 0;
    vlSelf->top__DOT__idu__DOT__dec2__DOT__infoDecoder__DOT____VdfgRegularize_he37144c4_0_4 = 0;
    vlSelf->top__DOT__exu1__DOT__shamt = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 2095108458103126356ull);
    vlSelf->top__DOT__exu1__DOT___target_branch_T = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16329925379917507332ull);
    vlSelf->top__DOT__exu1__DOT__is_jump = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13459496533800667716ull);
    vlSelf->top__DOT__exu1__DOT__is_branch = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6324595610223509941ull);
    vlSelf->top__DOT__exu1__DOT__is_load = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8428629204389500254ull);
    vlSelf->top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_0 = 0;
    vlSelf->top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_1 = 0;
    vlSelf->top__DOT__exu1__DOT____VdfgRegularize_haeb391f3_0_2 = 0;
    vlSelf->top__DOT__exu2__DOT__shamt = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 6271198150644576078ull);
    vlSelf->top__DOT__exu2__DOT___target_branch_T = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3811055462112214391ull);
    vlSelf->top__DOT__exu2__DOT__is_jump = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6134882675260987833ull);
    vlSelf->top__DOT__exu2__DOT__is_branch = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11201511515112722372ull);
    vlSelf->top__DOT__exu2__DOT__is_load = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2448386613667843245ull);
    vlSelf->top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_0 = 0;
    vlSelf->top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_1 = 0;
    vlSelf->top__DOT__exu2__DOT____VdfgRegularize_haeb391f3_0_2 = 0;
    vlSelf->top__DOT__lsu1__DOT__byte_sel = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5491267390267257675ull);
    vlSelf->top__DOT__lsu1__DOT__half_sel = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 11617117616913305906ull);
    vlSelf->top__DOT__lsu1__DOT____VdfgRegularize_hac146698_0_0 = 0;
    vlSelf->top__DOT__lsu2__DOT__byte_sel = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2672629385049579777ull);
    vlSelf->top__DOT__lsu2__DOT__half_sel = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3325183261152892825ull);
    vlSelf->top__DOT__lsu2__DOT____VdfgRegularize_hac146698_0_0 = 0;
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
    vlSelf->__VdfgRegularize_he50b618e_0_2 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_3 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_4 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_5 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_6 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_7 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_8 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_9 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_10 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_11 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_12 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_13 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_14 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_15 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_16 = 0;
    vlSelf->__VdfgRegularize_he50b618e_0_17 = 0;
    vlSelf->__Vfunc_top__DOT__imem__DOT__memory__DOT__pmem_read__0__Vfuncout = 0;
    vlSelf->__Vfunc_top__DOT__imem__DOT__memory__DOT__pmem_read__1__Vfuncout = 0;
    vlSelf->__Vfunc_top__DOT__dmem__DOT__memory__DOT__pmem_read__2__Vfuncout = 0;
    vlSelf->__Vfunc_top__DOT__dmem__DOT__memory__DOT__pmem_read__3__Vfuncout = 0;
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

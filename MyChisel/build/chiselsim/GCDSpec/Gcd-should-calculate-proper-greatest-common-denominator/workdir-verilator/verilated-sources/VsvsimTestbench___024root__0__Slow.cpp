// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VsvsimTestbench.h for the primary calling header

#include "VsvsimTestbench__pch.h"

VL_ATTR_COLD void VsvsimTestbench___024root___eval_static(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_static\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.svsimTestbench__DOT__clock = 0U;
    vlSelfRef.svsimTestbench__DOT__reset = 0U;
    vlSelfRef.svsimTestbench__DOT__input_bits_value2 = 0U;
    vlSelfRef.svsimTestbench__DOT__input_bits_value1 = 0U;
    vlSelfRef.svsimTestbench__DOT__input_valid = 0U;
    vlSelfRef.svsimTestbench__DOT__output_ready = 0U;
    vlSelfRef.svsimTestbench__DOT__simulationState = 0U;
    vlSelfRef.__Vtrigprevexpr___TOP__svsimTestbench__DOT__simulationState__0 = 0U;
    vlSelfRef.__Vtrigprevexpr___TOP__svsimTestbench__DOT__clock__0 
        = vlSelfRef.svsimTestbench__DOT__clock;
}

VL_ATTR_COLD void VsvsimTestbench___024root___eval_static__TOP(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_static__TOP\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.svsimTestbench__DOT__clock = 0U;
    vlSelfRef.svsimTestbench__DOT__reset = 0U;
    vlSelfRef.svsimTestbench__DOT__input_bits_value2 = 0U;
    vlSelfRef.svsimTestbench__DOT__input_bits_value1 = 0U;
    vlSelfRef.svsimTestbench__DOT__input_valid = 0U;
    vlSelfRef.svsimTestbench__DOT__output_ready = 0U;
    vlSelfRef.svsimTestbench__DOT__simulationState = 0U;
}

VL_ATTR_COLD void VsvsimTestbench___024root___eval_initial__TOP(VsvsimTestbench___024root* vlSelf);

VL_ATTR_COLD void VsvsimTestbench___024root___eval_initial(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_initial\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    VsvsimTestbench___024root___eval_initial__TOP(vlSelf);
}

void VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__initTestBenchScope_TOP(const VerilatedScope* __Vscopep, const char* __Vfilenamep, IData/*31:0*/ __Vlineno);

VL_ATTR_COLD void VsvsimTestbench___024root___eval_initial__TOP(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_initial__TOP\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ svsimTestbench__DOT__dut__DOT____Vlvbound_hf5eeebd0__0;
    svsimTestbench__DOT__dut__DOT____Vlvbound_hf5eeebd0__0 = 0;
    VlUnpacked<IData/*31:0*/, 3> svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM;
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM[__Vi0] = 0;
    }
    // Body
    VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__initTestBenchScope_TOP(
                                                                                (vlSymsp->__Vscopep_svsimTestbench), 
                                                                                "/home/sdark/MyChisel/build/chiselsim/GCDSpec/Gcd-should-calculate-proper-greatest-common-denominator/workdir-verilator/../generated-sources/testbench.sv", 0x0000001eU);
    vlSelfRef.svsimTestbench__DOT__simulationState = 1U;
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM[__Vi0] = 0;
    }
    svsimTestbench__DOT__dut__DOT____Vlvbound_hf5eeebd0__0 
        = VL_RANDOM_I();
    svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM[0U] 
        = svsimTestbench__DOT__dut__DOT____Vlvbound_hf5eeebd0__0;
    svsimTestbench__DOT__dut__DOT____Vlvbound_hf5eeebd0__0 
        = VL_RANDOM_I();
    svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM[1U] 
        = svsimTestbench__DOT__dut__DOT____Vlvbound_hf5eeebd0__0;
    svsimTestbench__DOT__dut__DOT____Vlvbound_hf5eeebd0__0 
        = VL_RANDOM_I();
    svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM[2U] 
        = svsimTestbench__DOT__dut__DOT____Vlvbound_hf5eeebd0__0;
    vlSelfRef.svsimTestbench__DOT__dut__DOT__xInitial 
        = (0x0000ffffU & svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM[0U]);
    vlSelfRef.svsimTestbench__DOT__dut__DOT__yInitial 
        = (svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM[0U] 
           >> 0x10U);
    vlSelfRef.svsimTestbench__DOT__dut__DOT__x = (0x0000ffffU 
                                                  & svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM[1U]);
    vlSelfRef.svsimTestbench__DOT__dut__DOT__y = (svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM[1U] 
                                                  >> 0x10U);
    vlSelfRef.svsimTestbench__DOT__dut__DOT__busy = 
        (1U & svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM[2U]);
    vlSelfRef.svsimTestbench__DOT__dut__DOT__resultValid 
        = (1U & (svsimTestbench__DOT__dut__DOT__unnamedblk2__DOT___RANDOM[2U] 
                 >> 1U));
}

VL_ATTR_COLD void VsvsimTestbench___024root___eval_final__TOP(VsvsimTestbench___024root* vlSelf);

VL_ATTR_COLD void VsvsimTestbench___024root___eval_final(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_final\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    VsvsimTestbench___024root___eval_final__TOP(vlSelf);
}

void VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__simulation_final_TOP(const VerilatedScope* __Vscopep, const char* __Vfilenamep, IData/*31:0*/ __Vlineno);

VL_ATTR_COLD void VsvsimTestbench___024root___eval_final__TOP(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_final__TOP\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__simulation_final_TOP(
                                                                                (vlSymsp->__Vscopep_svsimTestbench), 
                                                                                "/home/sdark/MyChisel/build/chiselsim/GCDSpec/Gcd-should-calculate-proper-greatest-common-denominator/workdir-verilator/../generated-sources/testbench.sv", 0x000000ceU);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void VsvsimTestbench___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool VsvsimTestbench___024root___eval_phase__stl(VsvsimTestbench___024root* vlSelf);

VL_ATTR_COLD void VsvsimTestbench___024root___eval_settle(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_settle\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            VsvsimTestbench___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("/home/sdark/MyChisel/build/chiselsim/GCDSpec/Gcd-should-calculate-proper-greatest-common-denominator/workdir-verilator/../generated-sources/testbench.sv", 1, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 100 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        vlSelfRef.__VstlPhaseResult = VsvsimTestbench___024root___eval_phase__stl(vlSelf);
        vlSelfRef.__VstlFirstIteration = 0U;
    } while (vlSelfRef.__VstlPhaseResult);
}

VL_ATTR_COLD void VsvsimTestbench___024root___eval_triggers_vec__stl(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_triggers_vec__stl\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
}

VL_ATTR_COLD bool VsvsimTestbench___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void VsvsimTestbench___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(VsvsimTestbench___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool VsvsimTestbench___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___trigger_anySet__stl\n"); );
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

VL_ATTR_COLD void VsvsimTestbench___024root___stl_sequent__TOP__0(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___stl_sequent__TOP__0\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.svsimTestbench__DOT__dut__DOT___GEN = 
        (0U == (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__x));
}

VL_ATTR_COLD void VsvsimTestbench___024root___eval_stl(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_stl\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        vlSelfRef.svsimTestbench__DOT__dut__DOT___GEN 
            = (0U == (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__x));
    }
}

VL_ATTR_COLD bool VsvsimTestbench___024root___eval_phase__stl(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_phase__stl\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    VsvsimTestbench___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        VsvsimTestbench___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = VsvsimTestbench___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        VsvsimTestbench___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool VsvsimTestbench___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 2> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void VsvsimTestbench___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 2> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(VsvsimTestbench___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @( svsimTestbench.simulationState)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(posedge svsimTestbench.clock)\n");
    }
    if ((1U & (IData)(triggers[1U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 64 is active: Internal 'act' trigger - DPI export trigger\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void VsvsimTestbench___024root___ctor_var_reset(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___ctor_var_reset\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->svsimTestbench__DOT__dut__DOT__xInitial = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7365778499980238109ull);
    vlSelf->svsimTestbench__DOT__dut__DOT__yInitial = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7638352793987222757ull);
    vlSelf->svsimTestbench__DOT__dut__DOT__x = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16161556372027329353ull);
    vlSelf->svsimTestbench__DOT__dut__DOT__y = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 269454852191556596ull);
    vlSelf->svsimTestbench__DOT__dut__DOT__busy = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17262404433777352611ull);
    vlSelf->svsimTestbench__DOT__dut__DOT__resultValid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15206042952107023065ull);
    vlSelf->svsimTestbench__DOT__dut__DOT___GEN = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2459972863847317586ull);
    vlSelf->__Vdpi_export_trigger = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__svsimTestbench__DOT__simulationState__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__svsimTestbench__DOT__clock__0 = 0;
    vlSelf->__VactDidInit = 0;
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}

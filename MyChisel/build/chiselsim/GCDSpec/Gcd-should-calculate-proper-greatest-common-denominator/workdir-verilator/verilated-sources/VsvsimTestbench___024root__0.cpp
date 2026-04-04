// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VsvsimTestbench.h for the primary calling header

#include "VsvsimTestbench__pch.h"

extern "C" void initTestBenchScope();

void VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__initTestBenchScope_TOP(const VerilatedScope* __Vscopep, const char* __Vfilenamep, IData/*31:0*/ __Vlineno) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__initTestBenchScope_TOP\n"); );
    // Body
    Verilated::dpiContext(__Vscopep, __Vfilenamep, __Vlineno);
    initTestBenchScope();
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_clock_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &value) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_clock_TOP\n"); );
    // Body
    value = 1U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_clock_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, CData/*0:0*/ value_clock) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_clock_TOP\n"); );
    // Body
    vlSymsp->TOP.__Vdpi_export_trigger = 1U;
    vlSymsp->TOP.svsimTestbench__DOT__clock = value_clock;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_clock_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, CData/*0:0*/ &value_clock) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_clock_TOP\n"); );
    // Body
    value_clock = vlSymsp->TOP.svsimTestbench__DOT__clock;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_reset_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &value) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_reset_TOP\n"); );
    // Body
    value = 1U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_reset_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, CData/*0:0*/ value_reset) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_reset_TOP\n"); );
    // Body
    vlSymsp->TOP.__Vdpi_export_trigger = 1U;
    vlSymsp->TOP.svsimTestbench__DOT__reset = value_reset;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_reset_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, CData/*0:0*/ &value_reset) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_reset_TOP\n"); );
    // Body
    value_reset = vlSymsp->TOP.svsimTestbench__DOT__reset;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_input_bits_value2_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &value) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_input_bits_value2_TOP\n"); );
    // Body
    value = 0x00000010U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_input_bits_value2_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, SData/*15:0*/ value_input_bits_value2) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_input_bits_value2_TOP\n"); );
    // Body
    vlSymsp->TOP.__Vdpi_export_trigger = 1U;
    vlSymsp->TOP.svsimTestbench__DOT__input_bits_value2 
        = value_input_bits_value2;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_input_bits_value2_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, SData/*15:0*/ &value_input_bits_value2) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_input_bits_value2_TOP\n"); );
    // Body
    value_input_bits_value2 = vlSymsp->TOP.svsimTestbench__DOT__input_bits_value2;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_input_bits_value1_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &value) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_input_bits_value1_TOP\n"); );
    // Body
    value = 0x00000010U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_input_bits_value1_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, SData/*15:0*/ value_input_bits_value1) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_input_bits_value1_TOP\n"); );
    // Body
    vlSymsp->TOP.__Vdpi_export_trigger = 1U;
    vlSymsp->TOP.svsimTestbench__DOT__input_bits_value1 
        = value_input_bits_value1;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_input_bits_value1_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, SData/*15:0*/ &value_input_bits_value1) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_input_bits_value1_TOP\n"); );
    // Body
    value_input_bits_value1 = vlSymsp->TOP.svsimTestbench__DOT__input_bits_value1;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_input_valid_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &value) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_input_valid_TOP\n"); );
    // Body
    value = 1U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_input_valid_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, CData/*0:0*/ value_input_valid) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_input_valid_TOP\n"); );
    // Body
    vlSymsp->TOP.__Vdpi_export_trigger = 1U;
    vlSymsp->TOP.svsimTestbench__DOT__input_valid = value_input_valid;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_input_valid_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, CData/*0:0*/ &value_input_valid) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_input_valid_TOP\n"); );
    // Body
    value_input_valid = vlSymsp->TOP.svsimTestbench__DOT__input_valid;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_input_ready_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &value) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_input_ready_TOP\n"); );
    // Body
    value = 1U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_input_ready_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, CData/*0:0*/ &value_input_ready) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_input_ready_TOP\n"); );
    // Body
    value_input_ready = (1U & ((~ (IData)(vlSymsp->TOP.svsimTestbench__DOT__dut__DOT__busy)) 
                               | ((~ (IData)(vlSymsp->TOP.svsimTestbench__DOT__dut__DOT__busy)) 
                                  & (IData)(vlSymsp->TOP.svsimTestbench__DOT__input_valid))));
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_output_bits_gcd_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &value) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_output_bits_gcd_TOP\n"); );
    // Body
    value = 0x00000010U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_output_bits_gcd_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, SData/*15:0*/ &value_output_bits_gcd) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_output_bits_gcd_TOP\n"); );
    // Body
    value_output_bits_gcd = ((0U == (IData)(vlSymsp->TOP.svsimTestbench__DOT__dut__DOT__x))
                              ? (IData)(vlSymsp->TOP.svsimTestbench__DOT__dut__DOT__y)
                              : (IData)(vlSymsp->TOP.svsimTestbench__DOT__dut__DOT__x));
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_output_bits_value2_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &value) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_output_bits_value2_TOP\n"); );
    // Body
    value = 0x00000010U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_output_bits_value2_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, SData/*15:0*/ &value_output_bits_value2) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_output_bits_value2_TOP\n"); );
    // Body
    value_output_bits_value2 = vlSymsp->TOP.svsimTestbench__DOT__dut__DOT__yInitial;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_output_bits_value1_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &value) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_output_bits_value1_TOP\n"); );
    // Body
    value = 0x00000010U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_output_bits_value1_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, SData/*15:0*/ &value_output_bits_value1) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_output_bits_value1_TOP\n"); );
    // Body
    value_output_bits_value1 = vlSymsp->TOP.svsimTestbench__DOT__dut__DOT__xInitial;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_output_valid_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &value) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_output_valid_TOP\n"); );
    // Body
    value = 1U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_output_valid_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, CData/*0:0*/ &value_output_valid) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_output_valid_TOP\n"); );
    // Body
    value_output_valid = vlSymsp->TOP.svsimTestbench__DOT__dut__DOT__resultValid;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_output_ready_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &value) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitWidthImpl_output_ready_TOP\n"); );
    // Body
    value = 1U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_output_ready_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, CData/*0:0*/ value_output_ready) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__setBitsImpl_output_ready_TOP\n"); );
    // Body
    vlSymsp->TOP.__Vdpi_export_trigger = 1U;
    vlSymsp->TOP.svsimTestbench__DOT__output_ready 
        = value_output_ready;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_output_ready_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, CData/*0:0*/ &value_output_ready) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__getBitsImpl_output_ready_TOP\n"); );
    // Body
    value_output_ready = vlSymsp->TOP.svsimTestbench__DOT__output_ready;
}

extern "C" int simulation_body();

void VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__simulation_body_TOP(const VerilatedScope* __Vscopep, const char* __Vfilenamep, IData/*31:0*/ __Vlineno) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__simulation_body_TOP\n"); );
    // Body
    Verilated::dpiContext(__Vscopep, __Vfilenamep, __Vlineno);
    simulation_body();
}

extern "C" int simulation_final();

void VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__simulation_final_TOP(const VerilatedScope* __Vscopep, const char* __Vfilenamep, IData/*31:0*/ __Vlineno) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__simulation_final_TOP\n"); );
    // Body
    Verilated::dpiContext(__Vscopep, __Vfilenamep, __Vlineno);
    simulation_final();
}

extern "C" void run_simulation(int timesteps, int* done);

void VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__run_simulation_TOP(IData/*31:0*/ timesteps, IData/*31:0*/ &done) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__run_simulation_TOP\n"); );
    // Body
    done = 0U;
    int timesteps__Vcvt;
    timesteps__Vcvt = timesteps;
    int done__Vcvt;
    run_simulation(timesteps__Vcvt, &done__Vcvt);
    done = (done__Vcvt);
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__simulation_initializeTrace_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, std::string traceFilePath) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__simulation_initializeTrace_TOP\n"); );
    // Body
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__simulation_enableTrace_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &success) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__simulation_enableTrace_TOP\n"); );
    // Body
    success = 0U;
}

void VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__simulation_disableTrace_TOP(VsvsimTestbench__Syms* __restrict vlSymsp, IData/*31:0*/ &success) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root____Vdpiexp_svsimTestbench__DOT__simulation_disableTrace_TOP\n"); );
    // Body
    success = 0U;
}

void VsvsimTestbench___024root___eval_triggers_vec__act(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_triggers_vec__act\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[1U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VactTriggered[1U]) 
                                     | (IData)((IData)(vlSelfRef.__Vdpi_export_trigger)));
    vlSelfRef.__Vdpi_export_trigger = 0U;
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((((IData)(vlSelfRef.svsimTestbench__DOT__clock) 
                                                       & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__svsimTestbench__DOT__clock__0))) 
                                                      << 1U) 
                                                     | (vlSelfRef.svsimTestbench__DOT__simulationState 
                                                        != vlSelfRef.__Vtrigprevexpr___TOP__svsimTestbench__DOT__simulationState__0))));
    vlSelfRef.__Vtrigprevexpr___TOP__svsimTestbench__DOT__simulationState__0 
        = vlSelfRef.svsimTestbench__DOT__simulationState;
    vlSelfRef.__Vtrigprevexpr___TOP__svsimTestbench__DOT__clock__0 
        = vlSelfRef.svsimTestbench__DOT__clock;
    if (VL_UNLIKELY(((1U & (~ (IData)(vlSelfRef.__VactDidInit)))))) {
        vlSelfRef.__VactDidInit = 1U;
        vlSelfRef.__VactTriggered[0U] = (1ULL | vlSelfRef.__VactTriggered[0U]);
    }
}

bool VsvsimTestbench___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 2> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((2U > n));
    return (0U);
}

void VsvsimTestbench___024root___act_sequent__TOP__0(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___act_sequent__TOP__0\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U == vlSelfRef.svsimTestbench__DOT__simulationState)) {
        VsvsimTestbench___024root____Vdpiimwrap_svsimTestbench__DOT__simulation_body_TOP(
                                                                                (vlSymsp->__Vscopep_svsimTestbench), 
                                                                                "/home/sdark/MyChisel/build/chiselsim/GCDSpec/Gcd-should-calculate-proper-greatest-common-denominator/workdir-verilator/../generated-sources/testbench.sv", 0x000000c8U);
        vlSelfRef.svsimTestbench__DOT__simulationState = 2U;
    }
}

void VsvsimTestbench___024root___eval_act(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_act\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered[0U])) {
        VsvsimTestbench___024root___act_sequent__TOP__0(vlSelf);
    }
}

void VsvsimTestbench___024root___nba_sequent__TOP__0(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___nba_sequent__TOP__0\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    SData/*15:0*/ __Vdly__svsimTestbench__DOT__dut__DOT__x;
    __Vdly__svsimTestbench__DOT__dut__DOT__x = 0;
    SData/*15:0*/ __Vdly__svsimTestbench__DOT__dut__DOT__y;
    __Vdly__svsimTestbench__DOT__dut__DOT__y = 0;
    CData/*0:0*/ __Vdly__svsimTestbench__DOT__dut__DOT__busy;
    __Vdly__svsimTestbench__DOT__dut__DOT__busy = 0;
    CData/*0:0*/ __Vdly__svsimTestbench__DOT__dut__DOT__resultValid;
    __Vdly__svsimTestbench__DOT__dut__DOT__resultValid = 0;
    // Body
    __Vdly__svsimTestbench__DOT__dut__DOT__y = vlSelfRef.svsimTestbench__DOT__dut__DOT__y;
    __Vdly__svsimTestbench__DOT__dut__DOT__busy = vlSelfRef.svsimTestbench__DOT__dut__DOT__busy;
    __Vdly__svsimTestbench__DOT__dut__DOT__resultValid 
        = vlSelfRef.svsimTestbench__DOT__dut__DOT__resultValid;
    __Vdly__svsimTestbench__DOT__dut__DOT__x = vlSelfRef.svsimTestbench__DOT__dut__DOT__x;
    if ((1U & (~ ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__busy) 
                  | (~ (IData)(vlSelfRef.svsimTestbench__DOT__input_valid)))))) {
        vlSelfRef.svsimTestbench__DOT__dut__DOT__xInitial 
            = vlSelfRef.svsimTestbench__DOT__input_bits_value1;
        vlSelfRef.svsimTestbench__DOT__dut__DOT__yInitial 
            = vlSelfRef.svsimTestbench__DOT__input_bits_value2;
    }
    if (vlSelfRef.svsimTestbench__DOT__dut__DOT__busy) {
        if (((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__x) 
             > (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__y))) {
            __Vdly__svsimTestbench__DOT__dut__DOT__x 
                = (0x0000ffffU & ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__x) 
                                  - (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__y)));
        } else {
            __Vdly__svsimTestbench__DOT__dut__DOT__y 
                = (0x0000ffffU & ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__y) 
                                  - (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__x)));
        }
    } else if (vlSelfRef.svsimTestbench__DOT__input_valid) {
        __Vdly__svsimTestbench__DOT__dut__DOT__x = vlSelfRef.svsimTestbench__DOT__input_bits_value1;
        __Vdly__svsimTestbench__DOT__dut__DOT__y = vlSelfRef.svsimTestbench__DOT__input_bits_value2;
    }
    if (vlSelfRef.svsimTestbench__DOT__reset) {
        __Vdly__svsimTestbench__DOT__dut__DOT__busy = 0U;
        __Vdly__svsimTestbench__DOT__dut__DOT__resultValid = 0U;
    } else {
        vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_0 
            = ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT___GEN) 
               | (0U == (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__y)));
        vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_1 
            = ((IData)(vlSelfRef.svsimTestbench__DOT__output_ready) 
               & (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__resultValid));
        __Vdly__svsimTestbench__DOT__dut__DOT__busy 
            = (1U & ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__busy)
                      ? (~ ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_0) 
                            & (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_1)))
                      : (IData)(vlSelfRef.svsimTestbench__DOT__input_valid)));
        if (((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__busy) 
             & (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_0))) {
            __Vdly__svsimTestbench__DOT__dut__DOT__resultValid 
                = (1U & (~ (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_1)));
        }
    }
    vlSelfRef.svsimTestbench__DOT__dut__DOT__y = __Vdly__svsimTestbench__DOT__dut__DOT__y;
    vlSelfRef.svsimTestbench__DOT__dut__DOT__busy = __Vdly__svsimTestbench__DOT__dut__DOT__busy;
    vlSelfRef.svsimTestbench__DOT__dut__DOT__resultValid 
        = __Vdly__svsimTestbench__DOT__dut__DOT__resultValid;
    vlSelfRef.svsimTestbench__DOT__dut__DOT__x = __Vdly__svsimTestbench__DOT__dut__DOT__x;
    vlSelfRef.svsimTestbench__DOT__dut__DOT___GEN = 
        (0U == (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__x));
}

void VsvsimTestbench___024root___eval_nba(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_nba\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    SData/*15:0*/ __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__x;
    __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__x = 0;
    SData/*15:0*/ __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__y;
    __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__y = 0;
    CData/*0:0*/ __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__busy;
    __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__busy = 0;
    CData/*0:0*/ __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__resultValid;
    __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__resultValid = 0;
    // Body
    if ((2ULL & vlSelfRef.__VnbaTriggered[0U])) {
        __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__y 
            = vlSelfRef.svsimTestbench__DOT__dut__DOT__y;
        __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__busy 
            = vlSelfRef.svsimTestbench__DOT__dut__DOT__busy;
        __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__resultValid 
            = vlSelfRef.svsimTestbench__DOT__dut__DOT__resultValid;
        __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__x 
            = vlSelfRef.svsimTestbench__DOT__dut__DOT__x;
        if ((1U & (~ ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__busy) 
                      | (~ (IData)(vlSelfRef.svsimTestbench__DOT__input_valid)))))) {
            vlSelfRef.svsimTestbench__DOT__dut__DOT__xInitial 
                = vlSelfRef.svsimTestbench__DOT__input_bits_value1;
            vlSelfRef.svsimTestbench__DOT__dut__DOT__yInitial 
                = vlSelfRef.svsimTestbench__DOT__input_bits_value2;
        }
        if (vlSelfRef.svsimTestbench__DOT__dut__DOT__busy) {
            if (((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__x) 
                 > (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__y))) {
                __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__x 
                    = (0x0000ffffU & ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__x) 
                                      - (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__y)));
            } else {
                __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__y 
                    = (0x0000ffffU & ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__y) 
                                      - (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__x)));
            }
        } else if (vlSelfRef.svsimTestbench__DOT__input_valid) {
            __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__x 
                = vlSelfRef.svsimTestbench__DOT__input_bits_value1;
            __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__y 
                = vlSelfRef.svsimTestbench__DOT__input_bits_value2;
        }
        if (vlSelfRef.svsimTestbench__DOT__reset) {
            __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__busy = 0U;
            __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__resultValid = 0U;
        } else {
            vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_0 
                = ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT___GEN) 
                   | (0U == (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__y)));
            vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_1 
                = ((IData)(vlSelfRef.svsimTestbench__DOT__output_ready) 
                   & (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__resultValid));
            __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__busy 
                = (1U & ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__busy)
                          ? (~ ((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_0) 
                                & (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_1)))
                          : (IData)(vlSelfRef.svsimTestbench__DOT__input_valid)));
            if (((IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__busy) 
                 & (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_0))) {
                __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__resultValid 
                    = (1U & (~ (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__unnamedblk1__DOT___GEN_1)));
            }
        }
        vlSelfRef.svsimTestbench__DOT__dut__DOT__y 
            = __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__y;
        vlSelfRef.svsimTestbench__DOT__dut__DOT__busy 
            = __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__busy;
        vlSelfRef.svsimTestbench__DOT__dut__DOT__resultValid 
            = __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__resultValid;
        vlSelfRef.svsimTestbench__DOT__dut__DOT__x 
            = __Vinline__nba_sequent__TOP__0___Vdly__svsimTestbench__DOT__dut__DOT__x;
        vlSelfRef.svsimTestbench__DOT__dut__DOT___GEN 
            = (0U == (IData)(vlSelfRef.svsimTestbench__DOT__dut__DOT__x));
    }
}

void VsvsimTestbench___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 2> &out, const VlUnpacked<QData/*63:0*/, 2> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((1U >= n));
}

#ifdef VL_DEBUG
VL_ATTR_COLD void VsvsimTestbench___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 2> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool VsvsimTestbench___024root___eval_phase__act(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_phase__act\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    VsvsimTestbench___024root___eval_triggers_vec__act(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        VsvsimTestbench___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    VsvsimTestbench___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = VsvsimTestbench___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        VsvsimTestbench___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

void VsvsimTestbench___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 2> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((2U > n));
}

bool VsvsimTestbench___024root___eval_phase__nba(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_phase__nba\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = VsvsimTestbench___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        VsvsimTestbench___024root___eval_nba(vlSelf);
        VsvsimTestbench___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void VsvsimTestbench___024root___eval(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            VsvsimTestbench___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("/home/sdark/MyChisel/build/chiselsim/GCDSpec/Gcd-should-calculate-proper-greatest-common-denominator/workdir-verilator/../generated-sources/testbench.sv", 1, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                VsvsimTestbench___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("/home/sdark/MyChisel/build/chiselsim/GCDSpec/Gcd-should-calculate-proper-greatest-common-denominator/workdir-verilator/../generated-sources/testbench.sv", 1, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactPhaseResult = VsvsimTestbench___024root___eval_phase__act(vlSelf);
        } while (vlSelfRef.__VactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = VsvsimTestbench___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

#ifdef VL_DEBUG
void VsvsimTestbench___024root___eval_debug_assertions(VsvsimTestbench___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VsvsimTestbench___024root___eval_debug_assertions\n"); );
    VsvsimTestbench__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}
#endif  // VL_DEBUG

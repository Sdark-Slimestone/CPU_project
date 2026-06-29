// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtop__pch.h"

//============================================================
// Constructors

Vtop::Vtop(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtop__Syms(contextp(), _vcname__, this)}
    , clock{vlSymsp->TOP.clock}
    , reset{vlSymsp->TOP.reset}
    , io_debug_stall{vlSymsp->TOP.io_debug_stall}
    , io_debug_lsu1_is_load{vlSymsp->TOP.io_debug_lsu1_is_load}
    , io_debug_lsu1_is_store{vlSymsp->TOP.io_debug_lsu1_is_store}
    , io_debug_lsu1_store_mask{vlSymsp->TOP.io_debug_lsu1_store_mask}
    , io_debug_lsu2_is_load{vlSymsp->TOP.io_debug_lsu2_is_load}
    , io_debug_lsu2_is_store{vlSymsp->TOP.io_debug_lsu2_is_store}
    , io_debug_lsu2_store_mask{vlSymsp->TOP.io_debug_lsu2_store_mask}
    , io_debug_wbu_valid1{vlSymsp->TOP.io_debug_wbu_valid1}
    , io_debug_wbu_valid2{vlSymsp->TOP.io_debug_wbu_valid2}
    , io_debug_wbu_conflict{vlSymsp->TOP.io_debug_wbu_conflict}
    , io_debug_wbu_rd1{vlSymsp->TOP.io_debug_wbu_rd1}
    , io_debug_wbu_rd2{vlSymsp->TOP.io_debug_wbu_rd2}
    , io_debug_wbu_wr1_addr{vlSymsp->TOP.io_debug_wbu_wr1_addr}
    , io_debug_wbu_wr2_addr{vlSymsp->TOP.io_debug_wbu_wr2_addr}
    , io_debug_grf_rden{vlSymsp->TOP.io_debug_grf_rden}
    , io_debug_grf_rdaddr{vlSymsp->TOP.io_debug_grf_rdaddr}
    , io_debug_inst1_pc{vlSymsp->TOP.io_debug_inst1_pc}
    , io_debug_inst2_pc{vlSymsp->TOP.io_debug_inst2_pc}
    , io_debug_inst1{vlSymsp->TOP.io_debug_inst1}
    , io_debug_inst2{vlSymsp->TOP.io_debug_inst2}
    , io_debug_exu1_alu_out{vlSymsp->TOP.io_debug_exu1_alu_out}
    , io_debug_exu1_alu_source1{vlSymsp->TOP.io_debug_exu1_alu_source1}
    , io_debug_exu1_alu_source2{vlSymsp->TOP.io_debug_exu1_alu_source2}
    , io_debug_exu1_agu_out{vlSymsp->TOP.io_debug_exu1_agu_out}
    , io_debug_exu2_alu_out{vlSymsp->TOP.io_debug_exu2_alu_out}
    , io_debug_exu2_alu_source1{vlSymsp->TOP.io_debug_exu2_alu_source1}
    , io_debug_exu2_alu_source2{vlSymsp->TOP.io_debug_exu2_alu_source2}
    , io_debug_exu2_agu_out{vlSymsp->TOP.io_debug_exu2_agu_out}
    , io_debug_lsu1_addr{vlSymsp->TOP.io_debug_lsu1_addr}
    , io_debug_lsu1_read_origin{vlSymsp->TOP.io_debug_lsu1_read_origin}
    , io_debug_lsu1_final_wb_data{vlSymsp->TOP.io_debug_lsu1_final_wb_data}
    , io_debug_lsu1_store_data_shifted{vlSymsp->TOP.io_debug_lsu1_store_data_shifted}
    , io_debug_lsu2_addr{vlSymsp->TOP.io_debug_lsu2_addr}
    , io_debug_lsu2_read_origin{vlSymsp->TOP.io_debug_lsu2_read_origin}
    , io_debug_lsu2_final_wb_data{vlSymsp->TOP.io_debug_lsu2_final_wb_data}
    , io_debug_lsu2_store_data_shifted{vlSymsp->TOP.io_debug_lsu2_store_data_shifted}
    , io_debug_grf_regs_0{vlSymsp->TOP.io_debug_grf_regs_0}
    , io_debug_grf_regs_1{vlSymsp->TOP.io_debug_grf_regs_1}
    , io_debug_grf_regs_2{vlSymsp->TOP.io_debug_grf_regs_2}
    , io_debug_grf_regs_3{vlSymsp->TOP.io_debug_grf_regs_3}
    , io_debug_grf_regs_4{vlSymsp->TOP.io_debug_grf_regs_4}
    , io_debug_grf_regs_5{vlSymsp->TOP.io_debug_grf_regs_5}
    , io_debug_grf_regs_6{vlSymsp->TOP.io_debug_grf_regs_6}
    , io_debug_grf_regs_7{vlSymsp->TOP.io_debug_grf_regs_7}
    , io_debug_grf_regs_8{vlSymsp->TOP.io_debug_grf_regs_8}
    , io_debug_grf_regs_9{vlSymsp->TOP.io_debug_grf_regs_9}
    , io_debug_grf_regs_10{vlSymsp->TOP.io_debug_grf_regs_10}
    , io_debug_grf_regs_11{vlSymsp->TOP.io_debug_grf_regs_11}
    , io_debug_grf_regs_12{vlSymsp->TOP.io_debug_grf_regs_12}
    , io_debug_grf_regs_13{vlSymsp->TOP.io_debug_grf_regs_13}
    , io_debug_grf_regs_14{vlSymsp->TOP.io_debug_grf_regs_14}
    , io_debug_grf_regs_15{vlSymsp->TOP.io_debug_grf_regs_15}
    , io_debug_grf_input{vlSymsp->TOP.io_debug_grf_input}
    , io_debug_mstatus{vlSymsp->TOP.io_debug_mstatus}
    , io_debug_mcause{vlSymsp->TOP.io_debug_mcause}
    , io_debug_mepc{vlSymsp->TOP.io_debug_mepc}
    , io_debug_mcycle{vlSymsp->TOP.io_debug_mcycle}
    , io_debug_minstret{vlSymsp->TOP.io_debug_minstret}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtop::Vtop(const char* _vcname__)
    : Vtop(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtop::~Vtop() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtop___024root___eval_debug_assertions(Vtop___024root* vlSelf);
#endif  // VL_DEBUG
void Vtop___024root___eval_static(Vtop___024root* vlSelf);
void Vtop___024root___eval_initial(Vtop___024root* vlSelf);
void Vtop___024root___eval_settle(Vtop___024root* vlSelf);
void Vtop___024root___eval(Vtop___024root* vlSelf);

void Vtop::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtop::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtop___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtop___024root___eval_static(&(vlSymsp->TOP));
        Vtop___024root___eval_initial(&(vlSymsp->TOP));
        Vtop___024root___eval_settle(&(vlSymsp->TOP));
        vlSymsp->__Vm_didInit = true;
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtop___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtop::eventsPending() { return false; }

uint64_t Vtop::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vtop::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtop___024root___eval_final(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop::final() {
    Vtop___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtop::hierName() const { return vlSymsp->name(); }
const char* Vtop::modelName() const { return "Vtop"; }
unsigned Vtop::threads() const { return 1; }
void Vtop::prepareClone() const { contextp()->prepareClone(); }
void Vtop::atClone() const {
    contextp()->threadPoolpOnClone();
}

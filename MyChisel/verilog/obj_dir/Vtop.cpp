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
    , io_clk{vlSymsp->TOP.io_clk}
    , io_reset{vlSymsp->TOP.io_reset}
    , io_debug_regWen{vlSymsp->TOP.io_debug_regWen}
    , io_debug_grf_rden{vlSymsp->TOP.io_debug_grf_rden}
    , io_debug_grf_rdaddr{vlSymsp->TOP.io_debug_grf_rdaddr}
    , io_debug_is_add{vlSymsp->TOP.io_debug_is_add}
    , io_debug_is_addi{vlSymsp->TOP.io_debug_is_addi}
    , io_debug_is_jalr{vlSymsp->TOP.io_debug_is_jalr}
    , io_debug_is_lui{vlSymsp->TOP.io_debug_is_lui}
    , io_debug_is_lbu{vlSymsp->TOP.io_debug_is_lbu}
    , io_debug_is_lw{vlSymsp->TOP.io_debug_is_lw}
    , io_debug_is_sw{vlSymsp->TOP.io_debug_is_sw}
    , io_debug_is_sb{vlSymsp->TOP.io_debug_is_sb}
    , io_debug_is_ebreak{vlSymsp->TOP.io_debug_is_ebreak}
    , io_debug_lsu_wen{vlSymsp->TOP.io_debug_lsu_wen}
    , io_debug_lsu_wmask{vlSymsp->TOP.io_debug_lsu_wmask}
    , io_debug_lsu_ren{vlSymsp->TOP.io_debug_lsu_ren}
    , io_debug_pc{vlSymsp->TOP.io_debug_pc}
    , io_debug_regs_0{vlSymsp->TOP.io_debug_regs_0}
    , io_debug_regs_1{vlSymsp->TOP.io_debug_regs_1}
    , io_debug_regs_2{vlSymsp->TOP.io_debug_regs_2}
    , io_debug_regs_3{vlSymsp->TOP.io_debug_regs_3}
    , io_debug_regs_4{vlSymsp->TOP.io_debug_regs_4}
    , io_debug_regs_5{vlSymsp->TOP.io_debug_regs_5}
    , io_debug_regs_6{vlSymsp->TOP.io_debug_regs_6}
    , io_debug_regs_7{vlSymsp->TOP.io_debug_regs_7}
    , io_debug_regs_8{vlSymsp->TOP.io_debug_regs_8}
    , io_debug_regs_9{vlSymsp->TOP.io_debug_regs_9}
    , io_debug_regs_10{vlSymsp->TOP.io_debug_regs_10}
    , io_debug_regs_11{vlSymsp->TOP.io_debug_regs_11}
    , io_debug_regs_12{vlSymsp->TOP.io_debug_regs_12}
    , io_debug_regs_13{vlSymsp->TOP.io_debug_regs_13}
    , io_debug_regs_14{vlSymsp->TOP.io_debug_regs_14}
    , io_debug_regs_15{vlSymsp->TOP.io_debug_regs_15}
    , io_debug_alu_src1{vlSymsp->TOP.io_debug_alu_src1}
    , io_debug_alu_src2{vlSymsp->TOP.io_debug_alu_src2}
    , io_debug_alu_res{vlSymsp->TOP.io_debug_alu_res}
    , io_debug_wbData{vlSymsp->TOP.io_debug_wbData}
    , io_debug_grf_input{vlSymsp->TOP.io_debug_grf_input}
    , io_debug_lsu_addr{vlSymsp->TOP.io_debug_lsu_addr}
    , io_debug_lsu_wdata{vlSymsp->TOP.io_debug_lsu_wdata}
    , io_debug_lsu_rdata{vlSymsp->TOP.io_debug_lsu_rdata}
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

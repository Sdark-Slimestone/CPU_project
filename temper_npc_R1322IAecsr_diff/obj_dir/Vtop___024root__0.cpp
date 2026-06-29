// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"

extern "C" int pmem_read(int addr);

void Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__pmem_read_TOP(IData/*31:0*/ addr, IData/*31:0*/ &pmem_read__Vfuncrtn) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__pmem_read_TOP\n"); );
    // Body
    int addr__Vcvt;
    addr__Vcvt = addr;
    int pmem_read__Vfuncrtn__Vcvt;
    pmem_read__Vfuncrtn__Vcvt = pmem_read(addr__Vcvt);
    pmem_read__Vfuncrtn = (pmem_read__Vfuncrtn__Vcvt);
}

extern "C" void pmem_write(int addr, int data, char mask);

void Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__pmem_write_TOP(IData/*31:0*/ addr, IData/*31:0*/ data, CData/*7:0*/ mask) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__pmem_write_TOP\n"); );
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

void Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__sim_finish_TOP() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__sim_finish_TOP\n"); );
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
    // Body
    if (((IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_wen) 
         | (IData)(vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_wen))) {
        Vtop___024root____Vdpiimwrap_top__DOT__imem__DOT__memory__DOT__pmem_write_TOP(
                                                                                ((IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_wen)
                                                                                 ? 
                                                                                (0xfffffffcU 
                                                                                & vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr)
                                                                                 : 
                                                                                (0xfffffffcU 
                                                                                & vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr)), 
                                                                                ((IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_wen)
                                                                                 ? vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_store_data
                                                                                 : vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_store_data), 
                                                                                ((IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_wen)
                                                                                 ? (IData)(vlSelfRef.top__DOT___lsu1_io_lsu_to_dmem_mask)
                                                                                 : (IData)(vlSelfRef.top__DOT___lsu2_io_lsu_to_dmem_mask)));
    }
    __Vdly__top__DOT__csr__DOT__minstret_reg = vlSelfRef.top__DOT__csr__DOT__minstret_reg;
    __Vdly__top__DOT__ifu__DOT__pcReg = vlSelfRef.top__DOT__ifu__DOT__pcReg;
    if (vlSelfRef.reset) {
        vlSelfRef.top__DOT__csr__DOT__mcycle_reg = 0ULL;
        __Vdly__top__DOT__csr__DOT__minstret_reg = 0ULL;
        __Vdly__top__DOT__ifu__DOT__pcReg = 0x80000000U;
        vlSelfRef.top__DOT__grf__DOT__regs_11 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_10 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_9 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_7 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_2 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_5 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_0 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_3 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_1 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_8 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_12 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_13 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_14 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_4 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_15 = 0U;
        vlSelfRef.top__DOT__grf__DOT__regs_6 = 0U;
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
        if ((1U & (~ ((IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ebreak) 
                      | (IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ecall))))) {
            __Vdly__top__DOT__csr__DOT__minstret_reg 
                = (1ULL + vlSelfRef.top__DOT__csr__DOT__minstret_reg);
        }
        __Vdly__top__DOT__ifu__DOT__pcReg = (((IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ecall) 
                                              | (IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ebreak))
                                              ? vlSelfRef.top__DOT__csr__DOT__mtvec_reg
                                              : ((IData)(vlSelfRef.top__DOT__idu__DOT___dec1_io_is_mret)
                                                  ? vlSelfRef.top__DOT__csr__DOT__mepc_reg
                                                  : 
                                                 (((IData)(vlSelfRef.top__DOT___exu1_io_exu_to_ifu_take_branch) 
                                                   | ((IData)(vlSelfRef.top__DOT__exu2__DOT__is_jump) 
                                                      | ((IData)(vlSelfRef.top__DOT__exu2__DOT__is_branch) 
                                                         & ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_beq)
                                                             ? 
                                                            (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                             == vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data)
                                                             : 
                                                            ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bne)
                                                              ? 
                                                             (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                              != vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data)
                                                              : 
                                                             ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_blt)
                                                               ? 
                                                              VL_LTS_III(32, vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val, vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data)
                                                               : 
                                                              ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bge)
                                                                ? 
                                                               VL_GTES_III(32, vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val, vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data)
                                                                : 
                                                               ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bltu)
                                                                 ? 
                                                                (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                                 < vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data)
                                                                 : 
                                                                ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_bgeu) 
                                                                 & (vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_val_rs1_val 
                                                                    >= vlSelfRef.top__DOT___exu2_io_exu_to_lsu_data_store_data))))))))))
                                                   ? 
                                                  ((IData)(vlSelfRef.top__DOT___exu1_io_exu_to_ifu_take_branch)
                                                    ? 
                                                   ((0x6fU 
                                                     == 
                                                     (0x0000007fU 
                                                      & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1))
                                                     ? vlSelfRef.top__DOT__exu1__DOT___target_branch_T
                                                     : 
                                                    ((IData)(vlSelfRef.top__DOT__idu__DOT__dec1__DOT___instDecoder_io_is_jalr)
                                                      ? 
                                                     (0xfffffffeU 
                                                      & vlSelfRef.top__DOT___exu1_io_exu_to_lsu_paddr_addr)
                                                      : 
                                                     ((IData)(vlSelfRef.top__DOT__exu1__DOT__is_branch)
                                                       ? vlSelfRef.top__DOT__exu1__DOT___target_branch_T
                                                       : 0U)))
                                                    : 
                                                   ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_jal)
                                                     ? vlSelfRef.top__DOT__exu2__DOT___target_branch_T
                                                     : 
                                                    ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_exu2_dec2_op_is_jalr)
                                                      ? 
                                                     (0xfffffffeU 
                                                      & vlSelfRef.top__DOT___exu2_io_exu_to_lsu_paddr_addr)
                                                      : 
                                                     ((IData)(vlSelfRef.top__DOT__exu2__DOT__is_branch)
                                                       ? vlSelfRef.top__DOT__exu2__DOT___target_branch_T
                                                       : 0U))))
                                                   : 
                                                  ((IData)(vlSelfRef.top__DOT___idu_io_idu_to_ifu_is_stall)
                                                    ? vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_val_nextpc
                                                    : vlSelfRef.top__DOT___ifu_io_ifu_to_idu_inst2_nextpc))));
        if ((IData)((((0x00000580U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_11 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((0x0bU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_11 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((0x0bU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_11 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000500U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_10 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((0x0aU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_10 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((0x0aU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_10 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000480U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_9 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((9U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_9 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((9U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_9 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000380U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_7 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((7U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_7 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((7U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_7 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000100U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_2 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((2U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_2 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((2U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_2 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000280U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_5 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((5U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_5 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((5U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_5 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0U == (0x00000f80U & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_0 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((0U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_0 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((0U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_0 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000180U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_3 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((3U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_3 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((3U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_3 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000080U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_1 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((1U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_1 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((1U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_1 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000400U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_8 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((8U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_8 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((8U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_8 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000600U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_12 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((0x0cU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_12 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((0x0cU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_12 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000680U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_13 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((0x0dU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_13 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((0x0dU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_13 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000700U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_14 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((0x0eU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_14 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((0x0eU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_14 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000200U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_4 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((4U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_4 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((4U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_4 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000780U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_15 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((0x0fU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_15 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((0x0fU == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_15 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        if ((IData)((((0x00000300U == (0x00000f80U 
                                       & vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1)) 
                      & (IData)(vlSelfRef.top__DOT____Vcellinp__grf__io_csr_to_grf_wen)) 
                     & (0U != (0x0000001fU & (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                              >> 7U)))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_6 = vlSelfRef.top__DOT___csr_io_csr_rdata;
        } else if ((IData)(((6U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_6 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr1_data;
        } else if ((IData)(((6U == (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr)) 
                            & (0U != (IData)(vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_addr))))) {
            vlSelfRef.top__DOT__grf__DOT__regs_6 = vlSelfRef.top__DOT___wbu_io_wbu_to_grf_wr2_data;
        }
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__t_rs1 
            = ((IData)(vlSelfRef.top__DOT__use_imm_csr)
                ? (0x0000001fU & vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value)
                : vlSelfRef.top__DOT___grf_io_grf_to_idu_dec1_value_inst1rs1_value);
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
            = (0x0300U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_0 
            = (0x0304U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_1 
            = (0x0305U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_2 
            = (0x0340U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_3 
            = (0x0341U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_4 
            = (0x0342U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                           >> 0x00000014U));
        vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT___GEN_5 
            = (0x0343U == (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
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
        if (vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ebreak) {
            vlSelfRef.top__DOT__csr__DOT__mepc_reg 
                = vlSelfRef.top__DOT__ifu__DOT__pcReg;
            vlSelfRef.top__DOT__csr__DOT__mcause_reg = 3U;
            vlSelfRef.top__DOT__csr__DOT__mtval_reg 
                = vlSelfRef.top__DOT__ifu__DOT__pcReg;
        } else if (vlSelfRef.top__DOT__idu__DOT___dec1_io_is_ecall) {
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
                      | (0x0344U != (vlSelfRef.top__DOT___imem_io_imem_to_ifu_inst1 
                                     >> 0x00000014U)))))) {
            vlSelfRef.top__DOT__csr__DOT__mip_reg = vlSelfRef.top__DOT__csr__DOT__unnamedblk1__DOT__csr_write_val;
        }
    }
    vlSelfRef.top__DOT__csr__DOT__minstret_reg = __Vdly__top__DOT__csr__DOT__minstret_reg;
    vlSelfRef.io_debug_mcycle = vlSelfRef.top__DOT__csr__DOT__mcycle_reg;
    vlSelfRef.io_debug_minstret = vlSelfRef.top__DOT__csr__DOT__minstret_reg;
    vlSelfRef.io_debug_grf_regs_11 = vlSelfRef.top__DOT__grf__DOT__regs_11;
    vlSelfRef.io_debug_grf_regs_10 = vlSelfRef.top__DOT__grf__DOT__regs_10;
    vlSelfRef.io_debug_grf_regs_9 = vlSelfRef.top__DOT__grf__DOT__regs_9;
    vlSelfRef.io_debug_grf_regs_7 = vlSelfRef.top__DOT__grf__DOT__regs_7;
    vlSelfRef.io_debug_grf_regs_2 = vlSelfRef.top__DOT__grf__DOT__regs_2;
    vlSelfRef.io_debug_grf_regs_5 = vlSelfRef.top__DOT__grf__DOT__regs_5;
    vlSelfRef.io_debug_grf_regs_0 = vlSelfRef.top__DOT__grf__DOT__regs_0;
    vlSelfRef.io_debug_grf_regs_3 = vlSelfRef.top__DOT__grf__DOT__regs_3;
    vlSelfRef.io_debug_grf_regs_1 = vlSelfRef.top__DOT__grf__DOT__regs_1;
    vlSelfRef.io_debug_grf_regs_8 = vlSelfRef.top__DOT__grf__DOT__regs_8;
    vlSelfRef.io_debug_grf_regs_12 = vlSelfRef.top__DOT__grf__DOT__regs_12;
    vlSelfRef.io_debug_grf_regs_13 = vlSelfRef.top__DOT__grf__DOT__regs_13;
    vlSelfRef.io_debug_grf_regs_14 = vlSelfRef.top__DOT__grf__DOT__regs_14;
    vlSelfRef.io_debug_grf_regs_4 = vlSelfRef.top__DOT__grf__DOT__regs_4;
    vlSelfRef.io_debug_grf_regs_15 = vlSelfRef.top__DOT__grf__DOT__regs_15;
    vlSelfRef.io_debug_grf_regs_6 = vlSelfRef.top__DOT__grf__DOT__regs_6;
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
    vlSelfRef.io_debug_mstatus = vlSelfRef.top__DOT__csr__DOT__mstatus_reg;
    vlSelfRef.io_debug_mepc = vlSelfRef.top__DOT__csr__DOT__mepc_reg;
    vlSelfRef.io_debug_mcause = vlSelfRef.top__DOT__csr__DOT__mcause_reg;
    vlSelfRef.io_debug_inst1_pc = vlSelfRef.top__DOT__ifu__DOT__pcReg;
    vlSelfRef.io_debug_inst2_pc = ((IData)(4U) + vlSelfRef.top__DOT__ifu__DOT__pcReg);
    vlSelfRef.top__DOT___idu_io_idu_to_exu1_dec1_val_nextpc 
        = ((IData)(4U) + vlSelfRef.top__DOT__ifu__DOT__pcReg);
    vlSelfRef.top__DOT___ifu_io_ifu_to_idu_inst2_nextpc 
        = ((IData)(8U) + vlSelfRef.top__DOT__ifu__DOT__pcReg);
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
}
#endif  // VL_DEBUG

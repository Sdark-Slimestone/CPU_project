// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary model header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef VERILATED_VTOP_H_
#define VERILATED_VTOP_H_  // guard

#include "verilated.h"
#include "svdpi.h"

class Vtop__Syms;
class Vtop___024root;

// This class is the main interface to the Verilated model
class alignas(VL_CACHE_LINE_BYTES) Vtop VL_NOT_FINAL : public VerilatedModel {
  private:
    // Symbol table holding complete model state (owned by this class)
    Vtop__Syms* const vlSymsp;

  public:

    // CONSTEXPR CAPABILITIES
    // Verilated with --trace?
    static constexpr bool traceCapable = false;

    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(&clock,0,0);
    VL_IN8(&reset,0,0);
    VL_OUT8(&io_debug_stall,0,0);
    VL_OUT8(&io_debug_lsu1_is_load,0,0);
    VL_OUT8(&io_debug_lsu1_is_store,0,0);
    VL_OUT8(&io_debug_lsu1_store_mask,3,0);
    VL_OUT8(&io_debug_lsu2_is_load,0,0);
    VL_OUT8(&io_debug_lsu2_is_store,0,0);
    VL_OUT8(&io_debug_lsu2_store_mask,3,0);
    VL_OUT8(&io_debug_wbu_valid1,0,0);
    VL_OUT8(&io_debug_wbu_valid2,0,0);
    VL_OUT8(&io_debug_wbu_conflict,0,0);
    VL_OUT8(&io_debug_wbu_rd1,4,0);
    VL_OUT8(&io_debug_wbu_rd2,4,0);
    VL_OUT8(&io_debug_wbu_wr1_addr,4,0);
    VL_OUT8(&io_debug_wbu_wr2_addr,4,0);
    VL_OUT8(&io_debug_grf_rden,0,0);
    VL_OUT8(&io_debug_grf_rdaddr,4,0);
    VL_OUT(&io_debug_inst1_pc,31,0);
    VL_OUT(&io_debug_inst2_pc,31,0);
    VL_OUT(&io_debug_inst1,31,0);
    VL_OUT(&io_debug_inst2,31,0);
    VL_OUT(&io_debug_exu1_alu_out,31,0);
    VL_OUT(&io_debug_exu1_alu_source1,31,0);
    VL_OUT(&io_debug_exu1_alu_source2,31,0);
    VL_OUT(&io_debug_exu1_agu_out,31,0);
    VL_OUT(&io_debug_exu2_alu_out,31,0);
    VL_OUT(&io_debug_exu2_alu_source1,31,0);
    VL_OUT(&io_debug_exu2_alu_source2,31,0);
    VL_OUT(&io_debug_exu2_agu_out,31,0);
    VL_OUT(&io_debug_lsu1_addr,31,0);
    VL_OUT(&io_debug_lsu1_read_origin,31,0);
    VL_OUT(&io_debug_lsu1_final_wb_data,31,0);
    VL_OUT(&io_debug_lsu1_store_data_shifted,31,0);
    VL_OUT(&io_debug_lsu2_addr,31,0);
    VL_OUT(&io_debug_lsu2_read_origin,31,0);
    VL_OUT(&io_debug_lsu2_final_wb_data,31,0);
    VL_OUT(&io_debug_lsu2_store_data_shifted,31,0);
    VL_OUT(&io_debug_grf_regs_0,31,0);
    VL_OUT(&io_debug_grf_regs_1,31,0);
    VL_OUT(&io_debug_grf_regs_2,31,0);
    VL_OUT(&io_debug_grf_regs_3,31,0);
    VL_OUT(&io_debug_grf_regs_4,31,0);
    VL_OUT(&io_debug_grf_regs_5,31,0);
    VL_OUT(&io_debug_grf_regs_6,31,0);
    VL_OUT(&io_debug_grf_regs_7,31,0);
    VL_OUT(&io_debug_grf_regs_8,31,0);
    VL_OUT(&io_debug_grf_regs_9,31,0);
    VL_OUT(&io_debug_grf_regs_10,31,0);
    VL_OUT(&io_debug_grf_regs_11,31,0);
    VL_OUT(&io_debug_grf_regs_12,31,0);
    VL_OUT(&io_debug_grf_regs_13,31,0);
    VL_OUT(&io_debug_grf_regs_14,31,0);
    VL_OUT(&io_debug_grf_regs_15,31,0);
    VL_OUT(&io_debug_grf_input,31,0);

    // CELLS
    // Public to allow access to /* verilator public */ items.
    // Otherwise the application code can consider these internals.

    // Root instance pointer to allow access to model internals,
    // including inlined /* verilator public_flat_* */ items.
    Vtop___024root* const rootp;

    // CONSTRUCTORS
    /// Construct the model; called by application code
    /// If contextp is null, then the model will use the default global context
    /// If name is "", then makes a wrapper with a
    /// single model invisible with respect to DPI scope names.
    explicit Vtop(VerilatedContext* contextp, const char* name = "TOP");
    explicit Vtop(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    virtual ~Vtop();
  private:
    VL_UNCOPYABLE(Vtop);  ///< Copying not allowed

  public:
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    /// Are there scheduled events to handle?
    bool eventsPending();
    /// Returns time at next time slot. Aborts if !eventsPending()
    uint64_t nextTimeSlot();
    /// Trace signals in the model; called by application code
    void trace(VerilatedTraceBaseC* tfp, int levels, int options = 0) { contextp()->trace(tfp, levels, options); }
    /// Retrieve name of this model instance (as passed to constructor).
    const char* name() const;

    // Abstract methods from VerilatedModel
    const char* hierName() const override final;
    const char* modelName() const override final;
    unsigned threads() const override final;
    /// Prepare for cloning the model at the process level (e.g. fork in Linux)
    /// Release necessary resources. Called before cloning.
    void prepareClone() const;
    /// Re-init after cloning the model at the process level (e.g. fork in Linux)
    /// Re-allocate necessary resources. Called after cloning.
    void atClone() const;
  private:
    // Internal functions - trace registration
    void traceBaseModel(VerilatedTraceBaseC* tfp, int levels, int options);
};

#endif  // guard

module svsimTestbench;
  reg  [$bits(dut.clock)-1:0] clock = '0;
  reg  [$bits(dut.reset)-1:0] reset = '0;
  reg  [$bits(dut.input_bits_value2)-1:0] input_bits_value2 = '0;
  reg  [$bits(dut.input_bits_value1)-1:0] input_bits_value1 = '0;
  reg  [$bits(dut.input_valid)-1:0] input_valid = '0;
  wire [$bits(dut.input_ready)-1:0] input_ready;
  wire [$bits(dut.output_bits_gcd)-1:0] output_bits_gcd;
  wire [$bits(dut.output_bits_value2)-1:0] output_bits_value2;
  wire [$bits(dut.output_bits_value1)-1:0] output_bits_value1;
  wire [$bits(dut.output_valid)-1:0] output_valid;
  reg  [$bits(dut.output_ready)-1:0] output_ready = '0;

DecoupledGcd dut (
    .clock(clock),
    .reset(reset),
    .input_bits_value2(input_bits_value2),
    .input_bits_value1(input_bits_value1),
    .input_valid(input_valid),
    .input_ready(input_ready),
    .output_bits_gcd(output_bits_gcd),
    .output_bits_value2(output_bits_value2),
    .output_bits_value1(output_bits_value1),
    .output_valid(output_valid),
    .output_ready(output_ready)
);

  import "DPI-C" context function void initTestBenchScope();
  initial
    initTestBenchScope();
  // Port 0: clock
  export "DPI-C" function getBitWidthImpl_clock;
  function void getBitWidthImpl_clock;
    output int value;
    value = $bits(dut.clock);
  endfunction
  export "DPI-C" function setBitsImpl_clock;
  function void setBitsImpl_clock;
    input bit [$bits(dut.clock)-1:0] value_clock;
    clock = value_clock;
  endfunction
  export "DPI-C" function getBitsImpl_clock;
  function void getBitsImpl_clock;
    output bit [$bits(dut.clock)-1:0] value_clock;
    value_clock = clock;
  endfunction

  // Port 1: reset
  export "DPI-C" function getBitWidthImpl_reset;
  function void getBitWidthImpl_reset;
    output int value;
    value = $bits(dut.reset);
  endfunction
  export "DPI-C" function setBitsImpl_reset;
  function void setBitsImpl_reset;
    input bit [$bits(dut.reset)-1:0] value_reset;
    reset = value_reset;
  endfunction
  export "DPI-C" function getBitsImpl_reset;
  function void getBitsImpl_reset;
    output bit [$bits(dut.reset)-1:0] value_reset;
    value_reset = reset;
  endfunction

  // Port 2: input_bits_value2
  export "DPI-C" function getBitWidthImpl_input_bits_value2;
  function void getBitWidthImpl_input_bits_value2;
    output int value;
    value = $bits(dut.input_bits_value2);
  endfunction
  export "DPI-C" function setBitsImpl_input_bits_value2;
  function void setBitsImpl_input_bits_value2;
    input bit [$bits(dut.input_bits_value2)-1:0] value_input_bits_value2;
    input_bits_value2 = value_input_bits_value2;
  endfunction
  export "DPI-C" function getBitsImpl_input_bits_value2;
  function void getBitsImpl_input_bits_value2;
    output bit [$bits(dut.input_bits_value2)-1:0] value_input_bits_value2;
    value_input_bits_value2 = input_bits_value2;
  endfunction

  // Port 3: input_bits_value1
  export "DPI-C" function getBitWidthImpl_input_bits_value1;
  function void getBitWidthImpl_input_bits_value1;
    output int value;
    value = $bits(dut.input_bits_value1);
  endfunction
  export "DPI-C" function setBitsImpl_input_bits_value1;
  function void setBitsImpl_input_bits_value1;
    input bit [$bits(dut.input_bits_value1)-1:0] value_input_bits_value1;
    input_bits_value1 = value_input_bits_value1;
  endfunction
  export "DPI-C" function getBitsImpl_input_bits_value1;
  function void getBitsImpl_input_bits_value1;
    output bit [$bits(dut.input_bits_value1)-1:0] value_input_bits_value1;
    value_input_bits_value1 = input_bits_value1;
  endfunction

  // Port 4: input_valid
  export "DPI-C" function getBitWidthImpl_input_valid;
  function void getBitWidthImpl_input_valid;
    output int value;
    value = $bits(dut.input_valid);
  endfunction
  export "DPI-C" function setBitsImpl_input_valid;
  function void setBitsImpl_input_valid;
    input bit [$bits(dut.input_valid)-1:0] value_input_valid;
    input_valid = value_input_valid;
  endfunction
  export "DPI-C" function getBitsImpl_input_valid;
  function void getBitsImpl_input_valid;
    output bit [$bits(dut.input_valid)-1:0] value_input_valid;
    value_input_valid = input_valid;
  endfunction

  // Port 5: input_ready
  export "DPI-C" function getBitWidthImpl_input_ready;
  function void getBitWidthImpl_input_ready;
    output int value;
    value = $bits(dut.input_ready);
  endfunction
  export "DPI-C" function getBitsImpl_input_ready;
  function void getBitsImpl_input_ready;
    output bit [$bits(dut.input_ready)-1:0] value_input_ready;
    value_input_ready = input_ready;
  endfunction

  // Port 6: output_bits_gcd
  export "DPI-C" function getBitWidthImpl_output_bits_gcd;
  function void getBitWidthImpl_output_bits_gcd;
    output int value;
    value = $bits(dut.output_bits_gcd);
  endfunction
  export "DPI-C" function getBitsImpl_output_bits_gcd;
  function void getBitsImpl_output_bits_gcd;
    output bit [$bits(dut.output_bits_gcd)-1:0] value_output_bits_gcd;
    value_output_bits_gcd = output_bits_gcd;
  endfunction

  // Port 7: output_bits_value2
  export "DPI-C" function getBitWidthImpl_output_bits_value2;
  function void getBitWidthImpl_output_bits_value2;
    output int value;
    value = $bits(dut.output_bits_value2);
  endfunction
  export "DPI-C" function getBitsImpl_output_bits_value2;
  function void getBitsImpl_output_bits_value2;
    output bit [$bits(dut.output_bits_value2)-1:0] value_output_bits_value2;
    value_output_bits_value2 = output_bits_value2;
  endfunction

  // Port 8: output_bits_value1
  export "DPI-C" function getBitWidthImpl_output_bits_value1;
  function void getBitWidthImpl_output_bits_value1;
    output int value;
    value = $bits(dut.output_bits_value1);
  endfunction
  export "DPI-C" function getBitsImpl_output_bits_value1;
  function void getBitsImpl_output_bits_value1;
    output bit [$bits(dut.output_bits_value1)-1:0] value_output_bits_value1;
    value_output_bits_value1 = output_bits_value1;
  endfunction

  // Port 9: output_valid
  export "DPI-C" function getBitWidthImpl_output_valid;
  function void getBitWidthImpl_output_valid;
    output int value;
    value = $bits(dut.output_valid);
  endfunction
  export "DPI-C" function getBitsImpl_output_valid;
  function void getBitsImpl_output_valid;
    output bit [$bits(dut.output_valid)-1:0] value_output_valid;
    value_output_valid = output_valid;
  endfunction

  // Port a: output_ready
  export "DPI-C" function getBitWidthImpl_output_ready;
  function void getBitWidthImpl_output_ready;
    output int value;
    value = $bits(dut.output_ready);
  endfunction
  export "DPI-C" function setBitsImpl_output_ready;
  function void setBitsImpl_output_ready;
    input bit [$bits(dut.output_ready)-1:0] value_output_ready;
    output_ready = value_output_ready;
  endfunction
  export "DPI-C" function getBitsImpl_output_ready;
  function void getBitsImpl_output_ready;
    output bit [$bits(dut.output_ready)-1:0] value_output_ready;
    value_output_ready = output_ready;
  endfunction

  // Simulation
  import "DPI-C" context task simulation_body();
  enum {INIT, RUN, DONE} simulationState = INIT;
  initial
    simulationState = RUN;
  always @(simulationState) begin
    if (simulationState == RUN) begin
      simulation_body();
      simulationState = DONE;
    end
  end
  import "DPI-C" context task simulation_final();
  final
    simulation_final();
  `ifdef SVSIM_BACKEND_SUPPORTS_DELAY_IN_PUBLIC_FUNCTIONS
  export "DPI-C" task run_simulation;
  task run_simulation;
    input int timesteps;
    output int finish;
    #(timesteps*0.1);
    finish = 0;
  endtask
  `else
  import "DPI-C" function void run_simulation(input int timesteps, output int done);
  `endif

  // Tracing
  int traceSupported = 0;
  export "DPI-C" function simulation_initializeTrace;
  function void simulation_initializeTrace;
    input string traceFilePath;
    `ifdef SVSIM_ENABLE_FST_TRACING_SUPPORT
      $dumpfile({traceFilePath,".fst"});
      $dumpvars(0, dut);
      traceSupported = 1;
    `elsif SVSIM_ENABLE_VCD_TRACING_SUPPORT
      $dumpfile({traceFilePath,".vcd"});
      $dumpvars(0, dut);
      traceSupported = 1;
    `endif
    `ifdef SVSIM_ENABLE_VPD_TRACING_SUPPORT
      $vcdplusfile({traceFilePath,".vpd"});
      $dumpvars(0, dut);
      $vcdpluson(0, dut);
      traceSupported = 1;
    `endif
    `ifdef SVSIM_ENABLE_FSDB_TRACING_SUPPORT
      $fsdbDumpfile({traceFilePath,".fsdb"});
      $fsdbDumpvars(0, dut, "+all");
      traceSupported = 1;
    `endif
  endfunction
  export "DPI-C" function simulation_enableTrace;
  function void simulation_enableTrace;
    output int success;
    success = traceSupported;
    `ifdef SVSIM_ENABLE_VCD_TRACING_SUPPORT
    $dumpon;
    `elsif SVSIM_ENABLE_FST_TRACING_SUPPORT
    $dumpon;
    `elsif SVSIM_ENABLE_VPD_TRACING_SUPPORT
    $dumpon;
    `endif
    `ifdef SVSIM_ENABLE_FSDB_TRACING_SUPPORT
    $fsdbDumpon;
    `endif
  endfunction
  export "DPI-C" function simulation_disableTrace;
  function void simulation_disableTrace;
    output int success;
    success = traceSupported;
    `ifdef SVSIM_ENABLE_VCD_TRACING_SUPPORT
    $dumpoff;
    `elsif SVSIM_ENABLE_FST_TRACING_SUPPORT
    $dumpoff;
    `elsif SVSIM_ENABLE_VPD_TRACING_SUPPORT
    $dumpoff;
    `endif
    `ifdef SVSIM_ENABLE_FSDB_TRACING_SUPPORT
    $fsdbDumpoff;
    `endif
  endfunction

endmodule

# ASAP7 7nm 预测性 PDK (虚拟/学术库, BSD-3) — 仅用于评估, 非真实工艺, 不可制造。
# 来源: https://github.com/The-OpenROAD-Project/asap7 (7.5T 标准单元库, RVT/TT/NLDM)
# 注: 库的 time_unit 为 1ps (nangate45 是 1ns); iEDA 按 ns 解读, 故报出的 fmax 需 ×1000。
# 注: 已从库/LEF 剔除 ICG 时钟门控单元 (iEDA liberty 解析器不认 statetable), 并 DONT_USE ICG*。
set ASAP7_HOME "$PROJ_HOME/pdk/asap7"
set LIB_FILES [list \
  "$ASAP7_HOME/lib/asap7sc7p5t_SIMPLE_RVT_TT_nldm_211120.lib" \
  "$ASAP7_HOME/lib/asap7sc7p5t_INVBUF_RVT_TT_nldm_220122.lib" \
  "$ASAP7_HOME/lib/asap7sc7p5t_AO_RVT_TT_nldm_211120.lib" \
  "$ASAP7_HOME/lib/asap7sc7p5t_OA_RVT_TT_nldm_211120.lib" \
  "$ASAP7_HOME/lib/asap7sc7p5t_SEQ_RVT_TT_nldm_clean.lib" \
]
set STDCELL_LEF_FILES [list "$ASAP7_HOME/lef/asap7sc7p5t_28_R_1x_clean.lef"]
set TECH_LEF_FILE     "$ASAP7_HOME/lef/asap7_tech_1x_260907.lef"

set TIEHI_CELL_AND_PORT "TIEHIx1_ASAP7_75t_R H"
set TIELO_CELL_AND_PORT "TIELOx1_ASAP7_75t_R L"
set BUF_CELL            "BUFx2_ASAP7_75t_R"
set DONT_USE_CELLS      [list "ICG*"]

# NVBoard 集成: 把 ysyxSoCFull 的 GPIO/PS2/UART/VGA 引脚绑定到 NVBoard 虚拟开发板。
# 需要设置 NVBOARD_HOME=/home/sdark/ysyx-workbench/nvboard
TOPNAME  = ysyxSoCFull
NXDC_FILES = constr/top.nxdc
CPP_FILE = main

YSYXSOC = /home/sdark/ysyx-workbench/ysyxSoC
PERIP   = $(YSYXSOC)/perip
ROOT    = $(abspath ..)

VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build -cc -O3 --x-assign fast --x-initial fast --noassert \
	--timescale "1ns/1ns" --no-timing --autoflush \
	-Wno-fatal -Wno-lint -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-UNOPTFLAT \
	-I$(PERIP)/uart16550/rtl -I$(PERIP)/spi/rtl

BUILD_DIR ?= ./build
OBJ_DIR   = $(BUILD_DIR)/obj_dir
BIN       = $(BUILD_DIR)/$(TOPNAME)
RTL_DIR  ?= $(ROOT)/rtl

default: $(BIN)

$(shell mkdir -p $(BUILD_DIR))

# 约束文件 -> 自动引脚绑定
SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)
$(SRC_AUTO_BIND): $(NXDC_FILES)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

# 本核 RTL + ysyxSoC 外设
VSRCS  = $(RTL_DIR)/*.sv $(ROOT)/ysyxSoCFull.v $(shell find $(PERIP) -name '*.v')
CSRCS  = $(wildcard csrc/*.cpp)
CSRCS += $(SRC_AUTO_BIND)

include $(NVBOARD_HOME)/scripts/nvboard.mk

INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\""

$(BIN): $(VSRCS) $(CSRCS) $(NVBOARD_ARCHIVE)
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) --top-module $(TOPNAME) \
		$(VSRCS) $(CSRCS) $(NVBOARD_ARCHIVE) \
		$(addprefix -CFLAGS , $(CXXFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))

all: default
run: $(BIN)
	@$^ $(IMG)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: default all run clean

# 定义 sv2v 
SV2V := ./sv2v                    

# 子目录路径（相对于当前目录）
CHISEL_DIR = MyChisel
NVBOARD_DIR = nvboard/myexample

# 派生路径
CHISEL_VERILOG_DIR = $(CHISEL_DIR)/verilog
NVBOARD_VSRC_DIR = $(NVBOARD_DIR)/vsrc

# 默认目标：生成 Verilog 并构建仿真程序
.DEFAULT_GOAL := all


# 代理 MyChisel 的目标
.PHONY: genv test rebuild clean-chisel

genv test rebuild:
	$(MAKE) -C $(CHISEL_DIR) $@

clean-chisel:
	$(MAKE) -C $(CHISEL_DIR) clean


# 代理 nvboard/myexample 的目标
.PHONY: build-nvboard run-nvboard clean-nvboard

build-nvboard:
	$(MAKE) -C $(NVBOARD_DIR) default

run-nvboard:
	$(MAKE) -C $(NVBOARD_DIR) run

clean-nvboard:
	$(MAKE) -C $(NVBOARD_DIR) clean


# 自定义操作：复制 Verilog 并运行自动绑定脚本
.PHONY: prepare-vsrc

prepare-vsrc:
	@echo "清空 $(NVBOARD_VSRC_DIR) ..."
	rm -rf $(NVBOARD_VSRC_DIR)/*
	@echo "复制 Verilog 文件从 $(CHISEL_VERILOG_DIR) 到 $(NVBOARD_VSRC_DIR) ..."
	cp -r $(CHISEL_VERILOG_DIR)/* $(NVBOARD_VSRC_DIR)/
	@echo "运行 autonv.py 生成 top.nxdc ..."
	python3 autonv.py
	@echo "使用 sv2v 转换 $(NVBOARD_VSRC_DIR) 中的 .sv 文件并替换原文件 ..."
	for f in $(NVBOARD_VSRC_DIR)/*.sv; do \
		if [ -f "$$f" ]; then \
			./sv2v "$$f" > "$${f%.sv}.v"; \
			echo "转换: $$f -> $${f%.sv}.v"; \
			rm -f "$$f"; \
			echo "删除原文件: $$f"; \
		fi; \
	done


# 聚合目标（推荐使用）

.PHONY: all clean run

# 完整流程：生成 Verilog -> 复制文件 -> 自动绑定 -> 运行仿真
all: genv prepare-vsrc run-nvboard

# 同时清理两个子模块
clean: clean-chisel clean-nvboard yosys-clean

# 直接运行仿真（前提是已经准备好 vsrc 并构建过）
run: run-nvboard




# ISA 模拟器集成（isa-emu 下的子项目）

ISA_EMU_DIR ?= minirv
ISA_EMU_PATH = isa-emu/$(ISA_EMU_DIR)
EXECUTABLE ?= $(ISA_EMU_DIR)      
PATH1 ?= /home/sdark/cpu_project/isa-emu/bin/mem.bin
PATH2 ?= /home/sdark/cpu_project/isa-emu/bin/mem.bin

.PHONY: emu clean-emu run-emu test-emu

emu:
	$(MAKE) -C $(ISA_EMU_PATH) clean
	$(MAKE) -C $(ISA_EMU_PATH) $(EXECUTABLE) tests
	@cd $(ISA_EMU_PATH) && ./$(EXECUTABLE) $(PATH1) $(PATH2)
	rm -rf $(ISA_EMU_PATH)/bin

clean-emu:
	$(MAKE) -C $(ISA_EMU_PATH) clean
	rm -rf $(ISA_EMU_PATH)/bin

run-emu: emu
	@cd $(ISA_EMU_PATH) && ./$(EXECUTABLE) $(PATH1) $(PATH2)
	rm -rf $(ISA_EMU_PATH)/bin

test-emu: emu
	$(MAKE) -C $(ISA_EMU_PATH) tests   
	rm -rf $(ISA_EMU_PATH)/bin



# yosys-sta 集成（位于 yosys-sta copy 目录）
YOSYS_STA_DIR = yosys-sta

.PHONY: yosys-init yosys-syn yosys-sta yosys-clean

yosys-init:
	$(MAKE) -C "$(YOSYS_STA_DIR)" init

yosys-syn:
	cp -r $(NVBOARD_VSRC_DIR)/* "$(YOSYS_STA_DIR)/example"/
	$(MAKE) -C "$(YOSYS_STA_DIR)" syn

yosys-sta:
	cp -r $(NVBOARD_VSRC_DIR)/* "$(YOSYS_STA_DIR)/example"/
	$(MAKE) -C "$(YOSYS_STA_DIR)" sta

yosys-clean:
	$(MAKE) -C "$(YOSYS_STA_DIR)" clean
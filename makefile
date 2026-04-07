# 可配置选项
ISA_EMU_DIR ?= scpu
PATH1 ?= /home/sdark/cpu_project/isa-emu/bin/mem.bin
PATH2 ?= /home/sdark/cpu_project/isa-emu/bin/mem.bin

# 固定路径（一般无需修改）
SV2V := ./sv2v
CHISEL_DIR = MyChisel
NVBOARD_DIR = nvboard/myexample
YOSYS_STA_DIR = yosys-sta

# 派生路径
CHISEL_VERILOG_DIR = $(CHISEL_DIR)/verilog
NVBOARD_VSRC_DIR = $(NVBOARD_DIR)/vsrc
ISA_EMU_PATH = isa-emu/$(ISA_EMU_DIR)
EXECUTABLE ?= $(ISA_EMU_DIR)

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

# 自定义操作：复制 Verilog 并运行自动绑定脚本（用于 nvboard）
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
			$(SV2V) "$$f" > "$${f%.sv}.v"; \
			echo "转换: $$f -> $${f%.sv}.v"; \
			rm -f "$$f"; \
			echo "删除原文件: $$f"; \
		fi; \
	done

# 新增：准备 STA 需要的源文件（直接从 Chisel 目录复制并转换，不依赖 nvboard）
.PHONY: prepare-sta-src
prepare-sta-src: genv
	@echo "准备 STA 源文件从 $(CHISEL_VERILOG_DIR) 到 $(YOSYS_STA_DIR)/example ..."
	find $(YOSYS_STA_DIR)/example -maxdepth 1 -type f \( -name "*.v" -o -name "*.sv" \) -delete
	@echo "复制 Verilog 文件..."
	cp -r $(CHISEL_VERILOG_DIR)/* $(YOSYS_STA_DIR)/example/
	@echo "转换 $(YOSYS_STA_DIR)/example 中的 .sv 文件为 .v ..."
	for f in $(YOSYS_STA_DIR)/example/*.sv; do \
		if [ -f "$$f" ]; then \
			$(SV2V) "$$f" > "$${f%.sv}.v"; \
			echo "转换: $$f -> $${f%.sv}.v"; \
			rm -f "$$f"; \
		fi; \
	done

# 聚合目标
.PHONY: all clean run
all: genv prepare-vsrc run-nvboard yosys-sta
clean: clean-chisel clean-nvboard yosys-clean
run: run-nvboard

# ISA 模拟器集成
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
test-emu: emu
	$(MAKE) -C $(ISA_EMU_PATH) tests
	rm -rf $(ISA_EMU_PATH)/bin

# yosys-sta 集成
.PHONY: yosys-init yosys-syn yosys-sta yosys-clean
yosys-init:
	$(MAKE) -C "$(YOSYS_STA_DIR)" init

yosys-syn: prepare-sta-src
	$(MAKE) -C "$(YOSYS_STA_DIR)" syn

yosys-sta:  prepare-sta-src
	$(MAKE) -C "$(YOSYS_STA_DIR)" sta

yosys-clean:
	$(MAKE) -C "$(YOSYS_STA_DIR)" clean
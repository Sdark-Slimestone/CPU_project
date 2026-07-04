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
.PHONY: check-nvboard build-nvboard run-nvboard clean-nvboard
check-nvboard:
	@if [ ! -d "$(NVBOARD_DIR)" ]; then \
		echo "错误：NVBoard 目录 $(NVBOARD_DIR) 不存在"; \
		exit 1; \
	fi
build-nvboard: check-nvboard
	$(MAKE) -C $(NVBOARD_DIR) default
run-nvboard: check-nvboard
	$(MAKE) -C $(NVBOARD_DIR) run
clean-nvboard: check-nvboard
	$(MAKE) -C $(NVBOARD_DIR) clean

# 自定义操作：复制 Verilog 并运行自动绑定脚本（用于 nvboard）
.PHONY: prepare-vsrc
prepare-vsrc: check-nvboard
	@if [ ! -d "$(CHISEL_VERILOG_DIR)" ]; then \
		echo "错误：Chisel Verilog 目录 $(CHISEL_VERILOG_DIR) 不存在"; \
		echo "请先执行 make genv 生成 Verilog"; \
		exit 1; \
	fi
	@echo "清空 $(NVBOARD_VSRC_DIR) ..."
	rm -rf $(NVBOARD_VSRC_DIR)/*
	@echo "复制 Verilog 文件从 $(CHISEL_VERILOG_DIR) 到 $(NVBOARD_VSRC_DIR) ..."
	cp -r $(CHISEL_VERILOG_DIR)/* $(NVBOARD_VSRC_DIR)/
	@if [ ! -f "autonv.py" ]; then \
		echo "错误：autonv.py 不存在于当前目录"; \
		exit 1; \
	fi
	@echo "运行 autonv.py 生成 top.nxdc ..."
	python3 autonv.py
	@if [ ! -f "$(SV2V)" ]; then \
		echo "错误：sv2v 工具 $(SV2V) 不存在"; \
		exit 1; \
	fi
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
	@if [ ! -d "$(CHISEL_VERILOG_DIR)" ]; then \
		echo "错误：Chisel Verilog 目录 $(CHISEL_VERILOG_DIR) 不存在"; \
		echo "请先执行 make genv 生成 Verilog"; \
		exit 1; \
	fi
	@if [ ! -d "$(YOSYS_STA_DIR)/example" ]; then \
		echo "错误：STA example 目录 $(YOSYS_STA_DIR)/example 不存在"; \
		exit 1; \
	fi
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
clean: clean-chisel clean-nvboard yosys-clean clean-npc
run: run-nvboard

# ISA 模拟器集成（分别作为独立程序运行，如 scpu, minirv）
# 不适用于 npc 内部用的 rv32e-csr.so（使用下方 make emu-so）
.PHONY: check-emu emu-run emu-clean emu-test
check-emu:
	@if [ ! -d "$(ISA_EMU_PATH)" ]; then \
		echo "错误：ISA 模拟器目录 $(ISA_EMU_PATH) 不存在"; \
		echo "提示：可用的模拟器有 scpu, minirv"; \
		echo "用法：make emu-run ISA_EMU_DIR=minirv"; \
		exit 1; \
	fi
emu-run: check-emu
	$(MAKE) -C $(ISA_EMU_PATH) clean
	$(MAKE) -C $(ISA_EMU_PATH) $(EXECUTABLE) tests
	@if [ ! -f "$(PATH1)" ]; then \
		echo "错误：程序文件 $(PATH1) 不存在"; \
		echo "请修改 Makefile 中的 PATH1/PATH2 变量"; \
		exit 1; \
	fi
	@cd $(ISA_EMU_PATH) && ./$(EXECUTABLE) $(PATH1) $(PATH2)
	rm -rf $(ISA_EMU_PATH)/bin
emu-clean: check-emu
	$(MAKE) -C $(ISA_EMU_PATH) clean
	rm -rf $(ISA_EMU_PATH)/bin
emu-test: check-emu
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


# ========== NPC 模拟器构建（重构版） ==========
# 用法:
#   make npc core=rv32e-csr sim=no     # 干净版本，什么都不带
#   make npc core=rv32e-csr sim=sdb    # 带 sdb/capstone
#   make npc core=rv32e-csr sim=diff   # 带 sdb + diff/NEMU
#
# core: cpu-core 下的核心文件夹名（如 rv32e-csr, R1322IAe-csr, minirv 等）
# sim:  cpp 文件类型
#   no   = 基础版本，使用 sim_main_io.cpp
#   sdb  = 带 sdb/capstone，使用 sim_main_io_sdb.cpp
#   diff = 带 sdb + diff/NEMU，使用 sim_main_io_sdb_diff.cpp
#
# 构建后 npc 二进制放入 cpu-core/<core>/npc-core/ 并自动清理

NPC_TEMP_DIR = temper_npc
CAP_DIR ?= /home/sdark/ysyx-workbench/nemu/tools/capstone/repo
NEMU_INC ?= /home/sdark/ysyx-workbench/nemu/include
NEMU_LIB ?= /home/sdark/cpu_project/nemu-so/riscv32-nemu-interpreter-so

# sim 参数 -> cpp 文件名映射
SIM_CPP_no    = sim_main_io.cpp
SIM_CPP_sdb   = sim_main_io_sdb.cpp
SIM_CPP_diff  = sim_main_io_sdb_diff.cpp
SIM_CPP_diff2 = sim_main_io_sdb_diff2.cpp

# sim 参数 -> 输出二进制名
SIM_BIN_no    = npc
SIM_BIN_sdb   = npc-sdb
SIM_BIN_diff  = npc-diff
SIM_BIN_diff2 = npc-diff2

# sim 参数 -> 额外的 Verilator 编译/链接 flags
SIM_EXTRA_FLAGS_no    =
SIM_EXTRA_FLAGS_sdb   = --CFLAGS "-I$(CAP_DIR)/include" --LDFLAGS "-L$(CAP_DIR) -lcapstone -Wl,-rpath,$(CAP_DIR)"
SIM_EXTRA_FLAGS_diff  = --CFLAGS "-I$(CAP_DIR)/include -I$(NEMU_INC)" \
	--LDFLAGS "-L$(CAP_DIR) -lcapstone -Wl,-rpath,$(CAP_DIR) $(NEMU_LIB) -lreadline -Wl,-rpath,/home/sdark/ysyx-workbench/nemu/build"
SIM_EXTRA_FLAGS_diff2 = --CFLAGS "-I$(CAP_DIR)/include" \
	--LDFLAGS "-L$(CAP_DIR) -lcapstone -Wl,-rpath,$(CAP_DIR) -ldl"

# ========== ISA 模拟器（emu）构建 ==========
# 用法:
#   make emu              # 构建默认 emu (rv32e-csr)
#   make emu EMU_DIR=rv32e-csr   # 指定 emu 目录
# 构建后 .so 自动复制到 emu-so/ 目录供 npc 使用
.PHONY: emu
emu:
	@emu_dir="rv32e-csr"; \
	if [ -n "$(emu)" ]; then \
		case "$(emu)" in \
			rv32e) emu_dir="rv32e-csr" ;; \
			minirv) emu_dir="minirv" ;; \
			scpu) emu_dir="scpu" ;; \
			*) echo "错误：不支持的 emu \"$(emu)\" (支持: rv32e/minirv/scpu)"; exit 1 ;; \
		esac; \
	fi; \
	emu_name="$${emu_dir%%-csr}"; \
	emu_out="emu-so/$$emu_name"; \
	if [ ! -d "isa-emu/$$emu_dir" ]; then \
		echo "错误：ISA 模拟器目录 isa-emu/$$emu_dir 不存在"; \
		exit 1; \
	fi; \
	$(MAKE) -C isa-emu/$$emu_dir clean; \
	$(MAKE) -C isa-emu/$$emu_dir; \
	mkdir -p $$emu_out; \
	cp isa-emu/$$emu_dir/build/$$emu_dir.so $$emu_out/$$emu_name.so; \
	echo "=== 完成 ==="; \
	echo "emu 已构建: $$emu_out/$$emu_name.so"

# 基础准备：复制 DPI 文件，依赖 genv 生成 Verilog
.PHONY: prepare-npc
prepare-npc: genv
	@mkdir -p $(NPC_TEMP_DIR)
	@echo "复制 DPI_Memory.v 到 $(CHISEL_VERILOG_DIR)"
	cp $(CHISEL_DIR)/src/main/resources/DPI_Memory.v $(CHISEL_VERILOG_DIR)/

# 统一 npc 构建入口
.PHONY: npc
npc:
ifndef core
	$(error 错误：请指定 core 变量，例如 make npc core=rv32e-csr sim=no)
endif
ifndef sim
	$(error 错误：请指定 sim 变量（no/sdb/diff），例如 make npc core=rv32e-csr sim=no)
endif
	@sim_cpp="$(SIM_CPP_$(sim))"; \
	sim_cpp_file="cpu-core/$(core)/cppfile/$$sim_cpp"; \
	sim_bin="$(SIM_BIN_$(sim))"; \
	if [ ! -d "cpu-core/$(core)" ]; then \
		echo "错误：目录 cpu-core/$(core) 不存在"; \
		exit 1; \
	fi; \
	if [ ! -f "$$sim_cpp_file" ]; then \
		echo "错误：cppfile $$sim_cpp 在 cpu-core/$(core)/cppfile/ 中不存在"; \
		exit 1; \
	fi; \
	echo "=== 部署核心: $(core) ==="; \
	$(MAKE) deploy-core core=$(core); \
	echo "=== 生成 Verilog ==="; \
	$(MAKE) genv; \
	echo "=== 准备 NPC 构建（sim=$(sim)） ==="; \
	mkdir -p $(NPC_TEMP_DIR); \
	cp $(CHISEL_DIR)/src/main/resources/DPI_Memory.v $(CHISEL_VERILOG_DIR)/; \
	echo "清理旧的构建产物..."; \
	rm -rf $(NPC_TEMP_DIR)/obj_dir $(NPC_TEMP_DIR)/$$sim_bin; \
	echo "使用 cppfile: $$sim_cpp"; \
	cp "$$sim_cpp_file" $(CHISEL_VERILOG_DIR)/; \
	echo "=== 运行 Verilator ==="; \
	cd $(NPC_TEMP_DIR) && \
	verilator -Wall -Wno-fatal --cc --exe --build -j 0 --top-module top \
		../$(CHISEL_VERILOG_DIR)/*.sv \
		../$(CHISEL_VERILOG_DIR)/DPI_Memory.v \
		../$(CHISEL_VERILOG_DIR)/$$sim_cpp \
		-o $$sim_bin \
		$(SIM_EXTRA_FLAGS_$(sim)) && \
	cd .. && \
	echo "=== 清空 cpu-core/$(core)/npc-core/ ==="; \
	rm -rf cpu-core/$(core)/npc-core; \
	mkdir -p cpu-core/$(core)/npc-core; \
	echo "=== 将新二进制放入 cpu-core/$(core)/npc-core/ ==="; \
	mv $(NPC_TEMP_DIR)/obj_dir/$$sim_bin cpu-core/$(core)/npc-core/$$sim_bin; \
	echo "=== 清理构建临时文件 ==="; \
	rm -rf $(NPC_TEMP_DIR)/obj_dir; \
	rm -f $(CHISEL_VERILOG_DIR)/$$sim_cpp; \
	echo "=== 完成 ==="; \
	echo "npc 已构建: cpu-core/$(core)/npc-core/$$sim_bin"

# npctest：运行已构建的 npc 模拟器执行测试
# 用法: make npctest core=rv32e-csr test=yield-os
#       make npctest core=rv32e-csr test=microbench
#       make npctest core=rv32e-csr test=alutest
# 可选手动指定 npcbin（默认自动在 npc-core 目录查找含 npc 的可执行文件）
# 注意: microbench 会自动传入 test 参数进行 mainargs 占位符替换
# 注意: yield-os 和 rtthread 需要 CSR 支持（核心名须含 -csr）
.PHONY: npctest
npctest:
ifndef core
	$(error 错误：请指定 core 变量，例如 make npctest core=rv32e-csr test=yield-os)
endif
ifndef test
	$(error 错误：请指定 test 变量，例如 make npctest core=rv32e-csr test=yield-os)
endif
	@npc_dir="cpu-core/$(core)/npc-core"; \
	needs_csr=0; \
	for t in yield-os rtthread; do \
		if [ "$$t" = "$(test)" ]; then needs_csr=1; break; fi; \
	done; \
	has_csr=0; \
	case "$(core)" in *-csr) has_csr=1;; esac; \
	if [ "$$needs_csr" -eq 1 ] && [ "$$has_csr" -eq 0 ]; then \
		echo "错误：测试 \"$(test)\" 需要 CSR 支持，核心 \"$(core)\" 不含 -csr 后缀"; \
		echo "请使用带 csr 的核心，如 rv32e-csr 或 R1322IAe-csr"; \
		exit 1; \
	fi; \
	core_ok=0; \
	for c in rv32e rv32e-csr R1322IAe R1322IAe-csr; do \
		if [ "$$c" = "$(core)" ]; then core_ok=1; break; fi; \
	done; \
	if [ "$$core_ok" -eq 0 ]; then \
		echo "错误：核心 \"$(core)\" 不支持 npc 测试。支持的 core: rv32e, rv32e-csr, R1322IAe, R1322IAe-csr"; \
		exit 1; \
	fi; \
$(if $(npcbin),npc_path="$$npc_dir/$(npcbin)",npc_path=$$(ls $$npc_dir/npc* 2>/dev/null | head -1)); \
	test_path="test-benchmarks/$(test)/$(test)-riscv32e-npc.bin"; \
	mainargs=""; \
	if [ "$(test)" = "microbench" ]; then \
		mainargs="test"; \
	fi; \
	if [ -z "$$npc_path" ] || [ ! -f "$$npc_path" ]; then \
		echo "错误：cpu-core/$(core)/npc-core/ 中未找到 npc 可执行文件"; \
		echo "请先执行 make npc core=$(core) sim=... 构建 npc"; \
		exit 1; \
	fi; \
	if [ ! -f "$$test_path" ]; then \
		echo "错误：测试程序 $$test_path 不存在"; \
		exit 1; \
	fi; \
	echo "=== 运行测试: $(test) ==="; \
	echo "npc : $$npc_path"; \
	echo "bin : $$test_path"; \
	if [ -n "$$mainargs" ]; then \
		echo "args: mainargs=$$mainargs"; \
	fi; \
	echo ""; \
	$$npc_path $$test_path $$mainargs

# 清理 NPC 构建目录及核心文件夹内的 npc-core 产物
.PHONY: clean-npc
clean-npc:
	rm -rf $(NPC_TEMP_DIR)
	@echo "已清理临时目录 $(NPC_TEMP_DIR)"
ifdef core
	@if [ ! -d "cpu-core/$(core)" ]; then \
		echo "警告：目录 cpu-core/$(core) 不存在，跳过清理"; \
	else \
		if [ -d "cpu-core/$(core)/npc-core" ]; then \
			echo "清理 cpu-core/$(core)/npc-core/ ..."; \
			rm -f cpu-core/$(core)/npc-core/npc*; \
		else \
			echo "cpu-core/$(core)/npc-core/ 目录不存在，无需清理"; \
		fi; \
	fi
else
	@echo "提示：未指定 core，如需清理特定核心请使用 make clean-npc core=xxx"
endif


# ========== 部署 CPU 核心 ==========
# 用法: make deploy-core core=<文件夹名>
# 例如: make deploy-core core=minirv
.PHONY: deploy-core
deploy-core:
ifndef core
	$(error 错误：请指定 core 变量，例如 make deploy-core core=minirv)
endif
	@if [ ! -d "cpu-core/$(core)" ]; then \
		echo "错误：目录 cpu-core/$(core) 不存在"; \
		exit 1; \
	fi
	@echo "准备部署 CPU 核心: $(core)"
	@echo "删除现有的 MyChisel/src/main ..."
	@rm -rf MyChisel/src/main
	@echo "复制 cpu-core/$(core) 到 MyChisel/src/main ..."
	@cp -r "cpu-core/$(core)" MyChisel/src/main
	@echo "部署完成。现在可以运行 make genv 重新生成 Verilog。"
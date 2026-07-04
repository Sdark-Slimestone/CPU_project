#!/bin/bash
set -e

# =========================================
# CPU Project - One-Command Setup
# =========================================
# 用法: cd cpu_project && bash setup.sh
#
# 会自动检查每个依赖是否已安装，
# 只下载缺失的部分。
#
# 测试系统: Ubuntu 22.04+ / Debian 12+
# =========================================

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_DIR"

NEED_APT=""

# ---------- 检查函数 ----------
check_cmd() {
    if command -v "$1" &>/dev/null; then
        echo "  [已安装] $1"
        return 0
    else
        echo "  [缺失]   $1"
        NEED_APT="$NEED_APT $2"
        return 1
    fi
}

check_pkg() {
    if dpkg -l "$1" &>/dev/null 2>&1; then
        echo "  [已安装] $1"
        return 0
    else
        echo "  [缺失]   $1"
        NEED_APT="$NEED_APT $1"
        return 1
    fi
}

check_file() {
    if [ -f "$1" ]; then
        echo "  [已安装] $2"
        return 0
    else
        echo "  [缺失]   $2"
        return 1
    fi
}

echo ""
echo "========================================"
echo "  CPU Project - 依赖检查 & 安装"
echo "========================================"
echo ""

# ─────────────────────────────────────────
# 1. 系统工具 (APT)
# ─────────────────────────────────────────
echo "--- 系统工具 ---"
echo ""
check_cmd java    openjdk-21-jdk
check_cmd make    make
check_cmd git     git
check_cmd curl    curl
check_cmd wget    wget
check_cmd g++     g++
check_cmd ccache  ccache
check_cmd python3 python3

# capstone
check_pkg libcapstone-dev
# capstone-tool is optional (cstool CLI debugger), not required for compilation
if ! dpkg -l capstone-tool &>/dev/null 2>&1; then
    echo "  [可选缺失] capstone-tool (仅 cstool 命令行调试工具，不影响编译)"
fi

# readline (for nemu diff)
check_pkg libreadline-dev

# Verilator
if ! command -v verilator &>/dev/null; then
    echo "  [缺失]   verilator (需要从源码编译)"
    NEED_APT="$NEED_APT help2man perl libfl-dev libgoogle-perftools-dev numactl"
fi

if [ -n "$NEED_APT" ]; then
    echo ""
    echo "--- 安装缺失的系统包 ---"
    sudo apt update
    sudo apt install -y $NEED_APT
fi

# 编译 Verilator（如果没有）
if ! command -v verilator &>/dev/null; then
    echo ""
    echo "--- 编译 Verilator 5.046 ---"
    if [ ! -d /tmp/verilator ]; then
        git clone --depth 1 --branch v5.046 https://github.com/verilator/verilator /tmp/verilator
    fi
    cd /tmp/verilator
    autoconf
    ./configure
    make -j$(nproc)
    sudo make install
    cd "$PROJECT_DIR"
    echo "  Verilator 安装完成: $(verilator --version)"
fi

# ─────────────────────────────────────────
# 2. sbt (通过 coursier 安装)
# ─────────────────────────────────────────
echo ""
echo "--- Scala / sbt ---"
echo ""
if ! command -v sbt &>/dev/null; then
    echo "  [缺失]   sbt，正在安装..."
    curl -fL https://github.com/coursier/coursier/releases/latest/download/cs-x86_64-pc-linux.gz | gzip -d > /tmp/cs
    chmod +x /tmp/cs
    /tmp/cs setup --yes
    rm -f /tmp/cs
else
    echo "  [已安装] sbt"
fi

# ─────────────────────────────────────────
# 3. ISA 模拟器 (.so, 供 diff2 模式用)
# ─────────────────────────────────────────
echo ""
echo "--- ISA 模拟器 (emu) ---"
echo ""
if [ -d "isa-emu/rv32e-csr" ]; then
    if [ -f "emu-so/rv32e/rv32e.so" ]; then
        echo "  [已安装] emu-so/rv32e/rv32e.so"
    else
        echo "  [缺失]   正在构建 ISA 模拟器..."
        make emu 2>&1 | tail -3
        echo "  emu 构建完成"
    fi
else
    echo "  [跳过]   isa-emu/rv32e-csr 不存在"
fi

# ─────────────────────────────────────────
# 4. Yosys-STA 工具链 (可选)
# ─────────────────────────────────────────
echo ""
echo "--- Yosys-STA (可选) ---"
echo ""
if [ -d "yosys-sta" ]; then
    if [ -f "yosys-sta/bin/iEDA" ]; then
        echo "  [已安装] iEDA"
    else
        echo "  [缺失]   正在下载 iEDA + PDK..."
        make -C yosys-sta init 2>&1 | tail -5 || echo "  (下载需要 SSH key 访问 GitHub)"
    fi
    if [ -d "yosys-sta/pdk/nangate45" ]; then
        echo "  [已安装] PDK (nangate45)"
    fi
else
    echo "  [跳过]   yosys-sta 不存在"
fi

echo ""
echo "========================================"
echo "  依赖检查完成"
echo "========================================"
echo ""
echo "快速开始:"
echo "  make deploy-core core=rv32e-csr"
echo "  make npc core=rv32e-csr sim=no"
echo "  make npctest core=rv32e-csr test=yield-os"
echo ""
echo "如需 diff2 模式:  make emu"
echo "如需 NVBoard:     设置 NVBOARD_HOME 环境变量"
echo "如需 NEMU diff:   从 ysyx-workbench 编译 nemu"
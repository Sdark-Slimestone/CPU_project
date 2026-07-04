#!/bin/bash
set -e

# =========================================
# CPU Project - One-Command Setup
# =========================================
# This script installs all dependencies needed
# to build and simulate CPU cores.
#
# Tested on Ubuntu 22.04+ / Debian 12+
# =========================================

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_DIR"

echo "=== CPU Project Setup ==="
echo ""

# ─────────────────────────────────────────
# 1. System packages (APT)
# ─────────────────────────────────────────
echo "[1/5] Installing system packages..."

# Check for APT
if command -v apt &>/dev/null; then
    sudo apt update
    sudo apt install -y \
        openjdk-21-jdk \
        curl wget git make \
        libcapstone-dev capstone-tool \
        g++ ccache \
        python3 python3-pip \
        libreadline-dev

    # Verilator 5.x is not in most distro repos, build from source if needed
    if ! command -v verilator &>/dev/null; then
        echo "Verilator not found. Building from source..."
        sudo apt install -y help2man perl python3 make g++ ccache \
            libfl-dev libgoogle-perftools-dev numactl perl-doc
        git clone --depth 1 --branch v5.046 https://github.com/verilator/verilator /tmp/verilator
        cd /tmp/verilator
        autoconf && ./configure && make -j$(nproc) && sudo make install
        cd "$PROJECT_DIR"
    fi
else
    echo "WARNING: APT not found. Install the following manually:"
    echo "  - JDK 21+ (or 11+ for older Chisel)"
    echo "  - sbt (https://www.scala-sbt.org/)"
    echo "  - Verilator 5.0+ (https://verilator.org/)"
    echo "  - Capstone 4.x+ (libcapstone-dev)"
    echo "  - make, g++, ccache"
fi

# ─────────────────────────────────────────
# 2. sbt (via coursier if not installed)
# ─────────────────────────────────────────
echo "[2/5] Checking sbt..."
if ! command -v sbt &>/dev/null; then
    echo "Installing sbt via coursier..."
    curl -fL https://github.com/coursier/coursier/releases/latest/download/cs-x86_64-pc-linux.gz | gzip -d > /tmp/cs
    chmod +x /tmp/cs
    /tmp/cs setup --yes
    rm -f /tmp/cs
fi
echo "  sbt: OK ($(sbt --version 2>&1 | head -1))"

# ─────────────────────────────────────────
# 3. Capstone (needed for sdb/diff modes)
# ─────────────────────────────────────────
echo "[3/5] Checking Capstone..."
if [ -f /usr/lib/x86_64-linux-gnu/libcapstone.so ] || \
   [ -f /usr/lib/libcapstone.so ] || \
   dpkg -l libcapstone-dev &>/dev/null 2>&1; then
    echo "  Capstone: OK"
else
    echo "  WARNING: Capstone not found. Install libcapstone-dev."
fi

# ─────────────────────────────────────────
# 4. Yosys-STA tools & PDK
# ─────────────────────────────────────────
echo "[4/5] Setting up Yosys-STA..."
if [ -d "yosys-sta" ]; then
    cd yosys-sta
    make init 2>&1 | tail -5 || echo "  NOTE: yosys-sta init skipped (may need SSH keys)"
    cd "$PROJECT_DIR"
    echo "  Yosys-STA: done"
else
    echo "  No yosys-sta/ directory found, skipping"
fi

# ─────────────────────────────────────────
# 5. Build ISA emulator .so (for diff2 mode)
# ─────────────────────────────────────────
echo "[5/5] Building ISA emulator..."
if [ -d "isa-emu/rv32e-csr" ]; then
    make emu 2>&1 | tail -5
    echo "  ISA emulator: done"
else
    echo "  No isa-emu/rv32e-csr found, skipping"
fi

echo ""
echo "=== Setup Complete ==="
echo ""
echo "Quick test:"
echo "  make deploy-core core=rv32e-csr"
echo "  make npc core=rv32e-csr sim=no"
echo "  make npctest core=rv32e-csr test=yield-os"
echo ""

# Show what's still needed
echo "Optional dependencies (not checked):"
echo "  - Yosys (for ASIC synthesis): apt install yosys"
echo "  - NEMU (for diff mode): set up from ysyx-workbench"
echo "  - NVBoard (for FPGA sim): set NVBOARD_HOME"
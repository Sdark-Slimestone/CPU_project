#!/bin/bash
set -e

cd "$(cd "$(dirname "$0")" && pwd)"

echo "=== CPU Project Setup ==="
echo ""

NEED_INSTALL=""

check() {
    if command -v "$1" &>/dev/null; then
        echo "  [OK] $1"
    else
        echo "  [MISS] $1"
        NEED_INSTALL="$NEED_INSTALL $2"
    fi
}

check_pkg() {
    if dpkg -l "$1" &>/dev/null 2>&1; then
        echo "  [OK] $1"
    else
        echo "  [MISS] $1"
        NEED_INSTALL="$NEED_INSTALL $1"
    fi
}

echo "--- System Tools ---"
check java    openjdk-21-jdk
check make    make
check g++     g++
check ccache  ccache
check yosys   yosys
check_pkg libcapstone-dev
check_pkg libreadline-dev

if ! command -v verilator &>/dev/null; then
    echo "  [MISS] verilator (will build from source)"
    NEED_INSTALL="$NEED_INSTALL help2man perl libfl-dev libgoogle-perftools-dev numactl"
fi

if ! command -v sbt &>/dev/null; then
    echo "  [MISS] sbt (will install via coursier)"
fi

if [ -n "$NEED_INSTALL" ]; then
    echo ""
    echo "--- Installing system packages ---"
    sudo apt update
    sudo apt install -y $NEED_INSTALL
fi

# Verilator
if ! command -v verilator &>/dev/null; then
    echo ""
    echo "--- Building Verilator 5.046 ---"
    [ -d /tmp/verilator ] || git clone --depth 1 --branch v5.046 https://github.com/verilator/verilator /tmp/verilator
    cd /tmp/verilator
    autoconf && ./configure && make -j$(nproc) && sudo make install
    cd "$OLDPWD"
fi

# sbt
if ! command -v sbt &>/dev/null; then
    echo ""
    echo "--- Installing sbt ---"
    curl -fL https://github.com/coursier/coursier/releases/latest/download/cs-x86_64-pc-linux.gz | gzip -d > /tmp/cs
    chmod +x /tmp/cs && /tmp/cs setup --yes && rm /tmp/cs
fi

# Yosys-STA: download iEDA + PDK if missing
echo ""
echo "--- Yosys-STA ---"
if [ -d "yosys-sta" ]; then
    if [ ! -f "yosys-sta/bin/iEDA" ]; then
        echo "  [MISS] iEDA tool, downloading..."
        make -C yosys-sta init 2>&1 | tail -5 || echo "  (skipped, may need SSH key)"
    else
        echo "  [OK] iEDA"
    fi
    if [ ! -d "yosys-sta/pdk/nangate45" ]; then
        echo "  [MISS] PDK (nangate45), downloading..."
        make -C yosys-sta init 2>&1 | tail -5 || echo "  (skipped, may need SSH key)"
    else
        echo "  [OK] PDK (nangate45)"
    fi
else
    echo "  (no yosys-sta directory, skipped)"
fi

echo ""
echo "=== Done ==="
echo ""
echo "Next:"
echo "  make deploy-core core=rv32e-csr"
echo "  make npc core=rv32e-csr sim=no"
echo "  make npctest core=rv32e-csr test=yield-os"
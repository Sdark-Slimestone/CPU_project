#!/bin/bash
set -e

cd "$(cd "$(dirname "$0")" && pwd)"

echo "=== CPU Project Setup ==="
echo ""

NEED_INSTALL=""
NEED_VERILATOR=0
NEED_SBT=0

check_ver() {
    local name="$1" cmd="$2" want="$3"
    if ! command -v "$cmd" &>/dev/null; then
        echo "  [MISS] $name"
        NEED_INSTALL="$NEED_INSTALL $4"
        [ -n "$5" ] && eval "$5=1"
        return
    fi
    local ver=$($cmd --version 2>&1 | head -1 | grep -oP '(\d+\.\d+)' | head -1)
    if [ -z "$ver" ]; then
        echo "  [?]    $name (installed, version unknown)"
        return
    fi
    # Compare major.minor
    local major=$(echo "$ver" | cut -d. -f1)
    local want_major=$(echo "$want" | cut -d. -f1)
    if [ "$major" -lt "$want_major" ] 2>/dev/null; then
        echo "  [OLD]  $name $ver (need $want+)"
        NEED_INSTALL="$NEED_INSTALL $4"
        [ -n "$5" ] && eval "$5=1"
    else
        echo "  [OK]   $name $ver"
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
check_ver "java"    java    11  openjdk-21-jdk
check_ver "make"    make    ""  make
check_ver "g++"     g++     ""  g++
check_ver "yosys"   yosys   0.48 yosys
check_ver "verilator" verilator 5.0 "help2man perl libfl-dev libgoogle-perftools-dev numactl" NEED_VERILATOR
check_ver "sbt"     sbt     ""  "" NEED_SBT

check_pkg libcapstone-dev
check_pkg libreadline-dev

# Install missing APT packages
if [ -n "$NEED_INSTALL" ]; then
    echo ""
    echo "--- Installing system packages ---"
    sudo apt update
    sudo apt install -y $NEED_INSTALL 2>&1 | tail -3
fi

# Build verilator 5.x if missing or too old
if [ "$NEED_VERILATOR" = "1" ]; then
    echo ""
    echo "--- Building Verilator 5.046 ---"
    if [ ! -d /tmp/verilator ]; then
        git clone --depth 1 --branch v5.046 https://github.com/verilator/verilator /tmp/verilator
    fi
    cd /tmp/verilator
    autoconf
    ./configure
    make -j$(nproc)
    sudo make install
    cd "$OLDPWD"
    echo "  verilator $(verilator --version 2>&1 | head -1)"
fi

# Install sbt if missing
if [ "$NEED_SBT" = "1" ]; then
    echo ""
    echo "--- Installing sbt ---"
    curl -fL https://github.com/coursier/coursier/releases/latest/download/cs-x86_64-pc-linux.gz | gzip -d > /tmp/cs
    chmod +x /tmp/cs && /tmp/cs setup --yes && rm /tmp/cs
fi

echo ""
echo "=== Done ==="
echo ""
echo "Next:"
echo "  make deploy-core core=rv32e-csr"
echo "  make npc core=rv32e-csr sim=no"
echo "  make npctest core=rv32e-csr test=yield-os"
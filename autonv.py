#!/usr/bin/env python3
"""
autonv.py - 从 top 模块的 Verilog 文件自动生成 NVBoard 约束文件 (top.nxdc)
并修改 nvboard/myexample/Makefile 中的 CPP_FILE 配置（根据时钟/复位信号）
功能：
1. 查找 top.sv 或 top.v
2. 解析所有端口，收集 seg/led/sw 信号，生成 top.nxdc（信号名使用原始端口名，包括 io_ 前缀）
3. 识别时钟（clk/clock）和复位（rst/reset/resetn）信号，修改 Makefile 中的 CPP_FILE 行
"""

import os
import re
import sys
from typing import Dict, List, Optional, Tuple

# ========== 配置 ==========
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
VSRC_DIR = os.path.join(SCRIPT_DIR, "nvboard/myexample/vsrc")
OUTPUT_DIR = os.path.join(SCRIPT_DIR, "nvboard/myexample/constr")
OUTPUT_FILE = os.path.join(OUTPUT_DIR, "top.nxdc")
MAKEFILE_PATH = os.path.join(SCRIPT_DIR, "nvboard/myexample/Makefile")

# 每个数码管的固定引脚（高位到低位）
SEG_PINS_TEMPLATE = {
    0: ["SEG0A", "SEG0B", "SEG0C", "SEG0D", "SEG0E", "SEG0F", "SEG0G", "DEC0P"],
    1: ["SEG1A", "SEG1B", "SEG1C", "SEG1D", "SEG1E", "SEG1F", "SEG1G", "DEC1P"],
    2: ["SEG2A", "SEG2B", "SEG2C", "SEG2D", "SEG2E", "SEG2F", "SEG2G", "DEC2P"],
    3: ["SEG3A", "SEG3B", "SEG3C", "SEG3D", "SEG3E", "SEG3F", "SEG3G", "DEC3P"],
    4: ["SEG4A", "SEG4B", "SEG4C", "SEG4D", "SEG4E", "SEG4F", "SEG4G", "DEC4P"],
    5: ["SEG5A", "SEG5B", "SEG5C", "SEG5D", "SEG5E", "SEG5F", "SEG5G", "DEC5P"],
    6: ["SEG6A", "SEG6B", "SEG6C", "SEG6D", "SEG6E", "SEG6F", "SEG6G", "DEC6P"],
    7: ["SEG7A", "SEG7B", "SEG7C", "SEG7D", "SEG7E", "SEG7F", "SEG7G", "DEC7P"],
}
LED_PINS_LOW_TO_HIGH = [f"LD{i}" for i in range(16)]   # LD0 .. LD15
SW_PINS_LOW_TO_HIGH = [f"SW{i}" for i in range(16)]    # SW0 .. SW15

MAX_SEG_COUNT = 8
MAX_SEG_WIDTH = 8
MAX_LED_TOTAL = 16
MAX_SW_TOTAL = 16


def remove_comments(text: str) -> str:
    lines = []
    for line in text.splitlines():
        idx = line.find("//")
        if idx != -1:
            line = line[:idx]
        lines.append(line)
    text = "\n".join(lines)
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    return text


def find_top_file() -> Optional[str]:
    if not os.path.isdir(VSRC_DIR):
        print(f"错误：目录不存在 {VSRC_DIR}")
        return None
    for f in os.listdir(VSRC_DIR):
        lower_f = f.lower()
        if lower_f in ("top.sv", "top.v"):
            return os.path.join(VSRC_DIR, f)
    print(f"错误：在 {VSRC_DIR} 中未找到 top.sv 或 top.v")
    return None


def parse_width(width_str: str) -> int:
    width_str = width_str.strip()
    if not width_str:
        return 1
    m = re.match(r"\[(\d+)\s*:\s*(\d+)\]", width_str)
    if not m:
        raise ValueError(f"无法解析位宽: {width_str}")
    msb = int(m.group(1))
    lsb = int(m.group(2))
    return abs(msb - lsb) + 1


def parse_port_decl(decl: str, default_dir: Optional[str] = None, default_width: int = 1) -> Tuple[Optional[str], int, str]:
    decl = decl.strip()
    if not decl:
        return None, 1, ""

    dir_match = re.match(r"^\s*(input|output|inout)\b", decl)
    if dir_match:
        direction = dir_match.group(1)
        rest = decl[dir_match.end():].strip()
        rest = re.sub(r"^\s*(wire|reg)\s+", "", rest)
        width = 1
        width_match = re.match(r"^\[\s*([^\]]+)\s*\]", rest)
        if width_match:
            width_str = width_match.group(1)
            width = parse_width(f"[{width_str}]")
            rest = rest[width_match.end():].strip()
        name_match = re.match(r"^(\w+)", rest)
        if not name_match:
            raise ValueError(f"无法解析信号名: {decl}")
        name = name_match.group(1)
        return direction, width, name
    else:
        name_match = re.match(r"^(\w+)", decl)
        if not name_match:
            raise ValueError(f"无法解析信号名: {decl}")
        name = name_match.group(1)
        width = default_width if default_width is not None else 1
        return default_dir, width, name


def parse_top_ports(filepath: str) -> Dict[str, Tuple[str, int]]:
    with open(filepath, "r") as f:
        content = f.read()
    content = remove_comments(content)

    module_start = re.search(r"module\s+top\s*\(", content, re.IGNORECASE)
    if not module_start:
        print("错误：未找到 'module top (' 定义")
        return {}
    start_pos = module_start.end()
    paren_count = 1
    end_pos = start_pos
    while end_pos < len(content) and paren_count > 0:
        ch = content[end_pos]
        if ch == '(':
            paren_count += 1
        elif ch == ')':
            paren_count -= 1
        end_pos += 1
    if paren_count != 0:
        print("错误：端口列表括号不匹配")
        return {}
    port_body = content[start_pos:end_pos-1]
    semicolon_pos = content.find(';', end_pos-1)
    if semicolon_pos == -1:
        print("错误：模块定义缺少结尾分号")
        return {}

    parts = port_body.split(',')
    ports = {}
    current_dir = None
    current_width = 1
    for part in parts:
        if not part.strip():
            continue
        try:
            dir_, width, name = parse_port_decl(part, current_dir, current_width)
            if name:
                if dir_ is None:
                    dir_ = current_dir
                if dir_ is None:
                    print(f"警告：端口 {name} 无法确定方向，跳过")
                    continue
                ports[name] = (dir_, width)
                current_dir = dir_
                current_width = width
        except Exception as e:
            print(f"解析端口项失败: {part}, 错误: {e}")
            continue
    return ports


def extract_signals(ports: Dict[str, Tuple[str, int]]):
    seg_signals = []   # (sort_key, original_name, width)
    led_signals = []   # (sort_key, original_name, width)
    sw_signals = []    # (sort_key, original_name, width)
    has_clk = False      # 端口名为 clk
    has_clock = False    # 端口名为 clock
    has_rst = False      # 端口名为 rst 或 reset
    has_resetn = False   # 端口名为 resetn

    for name, (dir_, width) in ports.items():
        base = name.lower()
        # 时钟检测
        if base == "clk":
            has_clk = True
        elif base == "clock":
            has_clock = True
        # 复位检测
        if base == "rst" or base == "reset":
            has_rst = True
        elif base == "resetn":
            has_resetn = True

        # 去掉 io_ 前缀用于匹配 seg/led/sw
        if base.startswith("io_"):
            base = base[3:]

        seg_match = re.match(r"^seg([0-7]?)$", base)
        if seg_match:
            if dir_ != "output":
                print(f"错误：信号 {name} 是 {dir_}, 但 seg 必须是 output")
                sys.exit(1)
            num_str = seg_match.group(1)
            index = int(num_str) if num_str else 0
            seg_signals.append((index, name, width))
            continue

        led_match = re.match(r"^led([0-9]*)$", base)
        if led_match:
            if dir_ != "output":
                print(f"错误：信号 {name} 是 {dir_}, 但 led 必须是 output")
                sys.exit(1)
            num_str = led_match.group(1)
            sort_key = int(num_str) if num_str else 0
            led_signals.append((sort_key, name, width))
            continue

        sw_match = re.match(r"^sw([0-9]*)$", base)
        if sw_match:
            if dir_ != "input":
                print(f"错误：信号 {name} 是 {dir_}, 但 sw 必须是 input")
                sys.exit(1)
            num_str = sw_match.group(1)
            sort_key = int(num_str) if num_str else 0
            sw_signals.append((sort_key, name, width))
            continue

    seg_signals.sort(key=lambda x: x[0])
    led_signals.sort(key=lambda x: x[0])
    sw_signals.sort(key=lambda x: x[0])

    return seg_signals, led_signals, sw_signals, (has_clk, has_clock, has_rst, has_resetn)


def update_makefile_cpp_file(has_clk, has_clock, has_rst, has_resetn):
    if not os.path.exists(MAKEFILE_PATH):
        print(f"警告：Makefile 不存在 {MAKEFILE_PATH}，跳过修改")
        return

    new_value = None
    if has_rst and has_clk:
        new_value = "clk"
    elif has_clk and has_resetn:
        new_value = "clkresetn"
    elif has_clock and has_rst:
        new_value = "clkforchisel"
    elif has_clock and has_resetn:
        new_value = "clkresetnforchisel"
    else:
        print("信息：未识别到匹配的时钟/复位组合，不修改 Makefile")
        return

    with open(MAKEFILE_PATH, "r") as f:
        content = f.read()

    pattern = r"^(CPP_FILE\s*=\s*).*$"
    new_line = f"CPP_FILE = {new_value}"
    new_content, count = re.subn(pattern, new_line, content, flags=re.MULTILINE)

    if count == 0:
        print(f"警告：Makefile 中未找到 'CPP_FILE =' 行，未修改")
        return

    with open(MAKEFILE_PATH, "w") as f:
        f.write(new_content)
    print(f"已将 Makefile 中的 CPP_FILE 修改为: {new_value}")


def generate_nxdc(seg_signals, led_signals, sw_signals) -> List[str]:
    lines = ["top=top", ""]

    seen_indices = set()
    for idx, name, width in seg_signals:
        if idx in seen_indices:
            print(f"错误：数码管 seg{idx} 重复定义（信号 {name}）")
            sys.exit(1)
        seen_indices.add(idx)
        if idx >= MAX_SEG_COUNT:
            print(f"错误：seg{idx} 索引超出最大允许值 {MAX_SEG_COUNT-1}")
            sys.exit(1)
        if width > MAX_SEG_WIDTH:
            print(f"错误：{name} 位宽 {width} 超过最大值 {MAX_SEG_WIDTH}")
            sys.exit(1)
        pins = SEG_PINS_TEMPLATE[idx][:width]
        lines.append(f"{name} ({', '.join(pins)})")

    led_offset = 0
    total_led = sum(w for (_, _, w) in led_signals)
    if total_led > MAX_LED_TOTAL:
        print(f"错误：led 总位宽 {total_led} 超过最大值 {MAX_LED_TOTAL}")
        sys.exit(1)
    for _, name, width in led_signals:
        low_to_high = LED_PINS_LOW_TO_HIGH[led_offset:led_offset+width]
        if len(low_to_high) != width:
            print(f"错误：{name} 需要的引脚不足（偏移 {led_offset}，宽度 {width}）")
            sys.exit(1)
        high_to_low = list(reversed(low_to_high))
        lines.append(f"{name} ({', '.join(high_to_low)})")
        led_offset += width

    sw_offset = 0
    total_sw = sum(w for (_, _, w) in sw_signals)
    if total_sw > MAX_SW_TOTAL:
        print(f"错误：sw 总位宽 {total_sw} 超过最大值 {MAX_SW_TOTAL}")
        sys.exit(1)
    for _, name, width in sw_signals:
        low_to_high = SW_PINS_LOW_TO_HIGH[sw_offset:sw_offset+width]
        if len(low_to_high) != width:
            print(f"错误：{name} 需要的引脚不足（偏移 {sw_offset}，宽度 {width}）")
            sys.exit(1)
        high_to_low = list(reversed(low_to_high))
        lines.append(f"{name} ({', '.join(high_to_low)})")
        sw_offset += width

    return lines


def main():
    top_file = find_top_file()
    if not top_file:
        sys.exit(1)

    print(f"找到文件: {top_file}")

    ports = parse_top_ports(top_file)
    if not ports:
        print("错误：未能解析到任何端口")
        sys.exit(1)

    print(f"解析到 {len(ports)} 个端口")

    seg_signals, led_signals, sw_signals, clk_rst_flags = extract_signals(ports)

    if not seg_signals and not led_signals and not sw_signals:
        print("错误：未匹配到任何 seg/led/sw 信号")
        sys.exit(1)

    if seg_signals:
        print("匹配到 seg 信号:")
        for _, name, w in seg_signals:
            print(f"  {name} 宽度 {w}")
    if led_signals:
        print("匹配到 led 信号:")
        for _, name, w in led_signals:
            print(f"  {name} 宽度 {w}")
    if sw_signals:
        print("匹配到 sw 信号:")
        for _, name, w in sw_signals:
            print(f"  {name} 宽度 {w}")

    lines = generate_nxdc(seg_signals, led_signals, sw_signals)
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    with open(OUTPUT_FILE, "w") as f:
        f.write("\n".join(lines))

    print(f"成功生成 {OUTPUT_FILE}")
    print("内容如下：")
    for line in lines:
        if line == "":
            print("  [空行]")
        else:
            print(f"  {line}")

    update_makefile_cpp_file(*clk_rst_flags)


if __name__ == "__main__":
    main()
#!/usr/bin/env python3
"""Compile a C file with cc.sh and compare each func_XXXXXXXX in it against assets/PAL/main.exe.
Relocated words (hi16/lo16/jal/data) are masked in both sides. Usage: cmp_func.py file.c"""
import struct, subprocess, sys
from pathlib import Path
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[2]
EXE = (ROOT / "assets/PAL/main.exe").read_bytes()
BASE = 0x80010000

def mask(word, rtype):
    if rtype == 4:   # R_MIPS_26
        return word & 0xFC000000
    if rtype in (5, 6, 7):  # HI16/LO16/GPREL16
        return word & 0xFFFF0000
    if rtype == 2:   # R_MIPS_32
        return 0
    return word

def main():
    src = Path(sys.argv[1]); obj = ROOT / "build/test" / (src.stem + ".o")
    subprocess.run([ROOT / "tools/scripts/cc.sh", src, obj], check=True, stderr=subprocess.DEVNULL)
    ef = ELFFile(obj.open("rb"))
    text = ef.get_section_by_name(".text"); data = text.data()
    rel = {}
    rs = ef.get_section_by_name(".rel.text")
    if rs:
        for r in rs.iter_relocations():
            rel[r["r_offset"]] = r["r_info_type"]
    ok = True
    for sym in ef.get_section_by_name(".symtab").iter_symbols():
        n = sym.name
        if not n.startswith("func_") or sym["st_shndx"] == "SHN_UNDEF":
            continue
        addr = int(n[5:], 16)
        off, size = sym["st_value"], sym["st_size"]
        if size == 0:  # no .size from gcc: run to next symbol/end
            size = len(data) - off
            for s2 in ef.get_section_by_name(".symtab").iter_symbols():
                if s2["st_shndx"] != "SHN_UNDEF" and off < s2["st_value"] < off + size and s2.name.startswith("func_"):
                    size = s2["st_value"] - off
        mine = [struct.unpack_from("<I", data, off + i)[0] for i in range(0, size, 4)]
        orig = [struct.unpack_from("<I", EXE, 0x800 + addr - BASE + i)[0] for i in range(0, size, 4)]
        diffs = []
        for i, (a, b) in enumerate(zip(mine, orig)):
            t = rel.get(off + 4 * i)
            if mask(a, t) != mask(b, t) if t else a != b:
                diffs.append(i)
        status = "MATCH" if not diffs else f"DIFF ({len(diffs)}/{len(mine)} words)"
        print(f"{n}: {status}")
        for i in diffs[:6]:
            print(f"   +{4*i:#x} mine={mine[i]:08x} orig={orig[i]:08x}")
        ok &= not diffs
    sys.exit(0 if ok else 1)
main()

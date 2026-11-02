#!/usr/bin/env python3
"""Function diff: compile a C file and compare each func_XXXXXXXX in it against the original PAL exe.
usage: fd.py FILE.c [--show]     (run with .venv/bin/python)
Exit 0 only if every function in the file is byte-identical (relocations masked) and the object
has no unsupported data sections. Reports instruction Levenshtein distance;
on mismatch also prints original asm and our disassembly."""
import re, struct, subprocess, sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from levenshtein import levenshtein

ROOT = Path(__file__).resolve().parents[2]
EXE = (ROOT / "assets/PAL/main.exe").read_bytes()
BASE = 0x80010000
FUNCS = {}
for l in (ROOT / "configs/PAL/functions.txt").read_text().splitlines():
    n, a, s = l.split(); FUNCS[n] = (int(a, 16), int(s, 16))

def mask(w, t):
    return {4: w & 0xFC000000, 5: w & 0xFFFF0000, 6: w & 0xFFFF0000, 7: w & 0xFFFF0000, 2: 0}.get(t, w)

def clean_orig(name):
    p = ROOT / f"build/baseline/asm/nonmatchings/{name}/{name}.s"
    out = []
    for l in p.read_text().splitlines():
        if l.startswith(("glabel", "endlabel", "nonmatching", ".set", "\n")) or not l.strip(): continue
        out.append(re.sub(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/", r"\1:", l))
    return "\n".join(out)

def main():
    src = Path(sys.argv[1]).resolve()
    obj = Path("/tmp") / "rr4fd" / (src.stem + ".o") if False else ROOT / "build/fd" / (src.stem + ".o")
    obj.parent.mkdir(parents=True, exist_ok=True)
    r = subprocess.run([str(ROOT / "tools/scripts/cc.sh"), str(src), str(obj)], capture_output=True, text=True, errors="replace")
    if r.returncode:
        print("COMPILE ERROR\n" + r.stderr[-1500:]); sys.exit(2)
    if r.stderr.strip(): print(r.stderr[-600:])
    ef = ELFFile(obj.open("rb"))
    for sec in ef.iter_sections():
        if sec.name in (".data", ".sdata", ".bss", ".sbss", ".rdata") and sec["sh_size"]:
            print(f"UNSUPPORTED: object has {sec.name} ({sec['sh_size']} bytes) - static/initialised data is not supported yet"); sys.exit(3)
    ro = ef.get_section_by_name(".rodata")
    if ro is not None and ro["sh_size"]:
        rdata = bytearray(ro.data()); wild = {}
        rr_sec = ef.get_section_by_name(".rel.rodata")
        if rr_sec:
            for rr in rr_sec.iter_relocations():
                wild[rr["r_offset"] // 4] = struct.unpack_from("<I", rdata, rr["r_offset"])[0]
                rdata[rr["r_offset"]:rr["r_offset"] + 4] = b"\0\0\0\0"
        words = [struct.unpack_from("<I", rdata, k)[0] for k in range(0, len(rdata) - len(rdata) % 4, 4)]
        n = len(words); hits = []
        tsyms = sorted((x["st_value"], x.name) for x in ef.get_section_by_name(".symtab").iter_symbols()
                       if x.name in FUNCS and x["st_shndx"] != "SHN_UNDEF")
        def expect(v):
            base = max((o, nm) for o, nm in tsyms if o <= v)
            return FUNCS[base[1]][0] + (v - base[0])
        fr = [(FUNCS[x.name][0], FUNCS[x.name][0] + FUNCS[x.name][1]) for x in ef.get_section_by_name(".symtab").iter_symbols()
              if x.name in FUNCS and x["st_shndx"] != "SHN_UNDEF"]
        inf = lambda v: any(a <= v < e for a, e in fr)
        for k in range(0, 0x2A08 - 0x800 - 4 * n + 1, 4):
            if all((struct.unpack_from("<I", EXE, 0x800 + k + 4 * i)[0] == expect(wild[i]) if i in wild else struct.unpack_from("<I", EXE, 0x800 + k + 4 * i)[0] == words[i]) for i in range(n)):
                hits.append(0x80010000 + k)
        want = re.search(r"RODATA\s+(0x[0-9A-Fa-f]+)\s+(0x[0-9A-Fa-f]+)", src.read_text())
        print(f"RODATA: object has {ro['sh_size']:#x} bytes of .rodata; matches original rodata at: {[hex(h) for h in hits[:6]] or 'NOWHERE (contents differ)'}")
        rodata_fail = 4 if not hits else 0
        if rodata_fail: print("  (rodata mismatch - code comparison follows anyway)")
        size = (ro["sh_size"] + 3) & ~3
        if hits:
            print(f"  add this line to the C file:  /* RODATA {hits[0]:#010x} {size:#x} */")
            if not want or int(want[1], 16) not in hits: print("  (missing/incorrect RODATA annotation)"); rodata_fail = 5
    else:
        rodata_fail = 0
    data = ef.get_section_by_name(".text").data()
    rel = {}
    rs = ef.get_section_by_name(".rel.text")
    if rs:
        for rr in rs.iter_relocations(): rel[rr["r_offset"]] = rr["r_info_type"]
    syms = sorted((s["st_value"], s.name) for s in ef.get_section_by_name(".symtab").iter_symbols()
                  if s.name.startswith("func_") and s["st_shndx"] != "SHN_UNDEF")
    if not syms:
        print("ERROR: object defines no func_XXXXXXXX functions")
        sys.exit(3)
    # Local jump relocations must be compared after resolving their targets.
    # Masking R_MIPS_26 hides jumps to the wrong instruction within this unit.
    resolved_jumps = {}
    text_index = ef.get_section_index(".text")
    if rs and syms[0][1] in FUNCS:
        text_base = FUNCS[syms[0][1]][0] - syms[0][0]
        symtab = ef.get_section(rs["sh_link"])
        for rr in rs.iter_relocations():
            symbol = symtab.get_symbol(rr["r_info_sym"])
            if rr["r_info_type"] == 4 and symbol["st_shndx"] == text_index:
                off = rr["r_offset"]
                word = struct.unpack_from("<I", data, off)[0]
                target = text_base + symbol["st_value"] + ((word & 0x03FFFFFF) << 2)
                resolved_jumps[off] = (word & 0xFC000000) | ((target >> 2) & 0x03FFFFFF)
                del rel[off]
    ok = True
    for i, (off, name) in enumerate(syms):
        end = syms[i + 1][0] if i + 1 < len(syms) else len(data)
        if name not in FUNCS: print(f"{name}: not a known function"); ok = False; continue
        addr, size = FUNCS[name]
        mine = [struct.unpack_from("<I", data, off + k)[0] for k in range(0, end - off, 4)]
        mine = [resolved_jumps.get(off + 4 * k, word) for k, word in enumerate(mine)]
        orig = [struct.unpack_from("<I", EXE, 0x800 + addr - BASE + k)[0] for k in range(0, size, 4)]
        distance = levenshtein(
            [mask(w, rel.get(off + 4 * k)) for k, w in enumerate(mine)],
            [mask(w, rel.get(off + 4 * k)) for k, w in enumerate(orig)])
        if distance == 0:
            print(f"{name}: MATCH (Levenshtein: 0 instructions)"); continue
        ok = False
        print(f"{name}: MISMATCH (Levenshtein: {distance} instructions)")
        if "--no-asm" not in sys.argv:
            print("---- ORIGINAL ----\n" + clean_orig(name))
            d = subprocess.run([str(ROOT / "tools/toolchain/bin/mips-linux-gnu-objdump"), "-d", "-M", "no-aliases,reg-names=32", "--start-address", str(off), "--stop-address", str(end), str(obj)],
                               capture_output=True, text=True).stdout
            print("---- MINE ----\n" + "\n".join(l for l in d.splitlines() if re.match(r"\s+[0-9a-f]+:", l)))
    sys.exit(0 if ok and not rodata_fail else 1)

main()

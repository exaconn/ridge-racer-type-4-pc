#!/usr/bin/env python3
"""Generate configs/<V>/main.yaml from main.base.yaml by cutting C units out of the asm text segment.

A C unit is src/main/<V>/*.c; the functions it defines (func_XXXXXXXX, sizes from functions.txt)
must be one contiguous run. Usage: gen_config.py [VERSION]"""
import re, sys
from pathlib import Path
from c_functions import function_definitions

ROOT = Path(__file__).resolve().parents[2]
V = sys.argv[1] if len(sys.argv) > 1 else "PAL"
BASE_VRAM, FILE_OFF = 0x80010000, 0x800
TEXT_START, TEXT_END = 0x80012208, 0x8009DF14  # asm text range; keep in sync with main.base.yaml

funcs = {}
for l in (ROOT / f"configs/{V}/functions.txt").read_text().splitlines():
    n, a, s = l.split()
    funcs[n] = (int(a, 16), int(s, 16))

units = []
ro_units = []
for c in sorted((ROOT / f"src/main/{V}").glob("*.c")):
    names = function_definitions(c.read_text())
    if not names:
        sys.exit(f"{c.name}: no function definitions found")
    unknown = set(names) - funcs.keys()
    if unknown:
        sys.exit(f"{c.name}: unknown functions: {sorted(unknown)}")
    if names != sorted(set(names), key=lambda n: funcs[n][0]):
        sys.exit(f"{c.name}: definitions must be unique and in original address order")
    rng = sorted(funcs[n] for n in names)
    for (a, s), (a2, _) in zip(rng, rng[1:]):
        if a + s != a2:
            sys.exit(f"{c.name}: functions not contiguous at {a + s:#x} vs {a2:#x}")
    units.append((rng[0][0], rng[-1][0] + rng[-1][1], c.stem))
    for m in re.finditer(r"RODATA\s+(0x[0-9A-Fa-f]+)\s+(0x[0-9A-Fa-f]+)", c.read_text()):
        ro_units.append((int(m[1], 16), int(m[1], 16) + int(m[2], 16), c.stem))
units = [(a, e, n, "c") for a, e, n in units]
# Hand-written assembly (configs/<V>/hasm.txt): own splat `hasm` segments, so they
# are reported apart from not-yet-decompiled `asm` and excluded from C coverage.
hasm_path = ROOT / f"configs/{V}/hasm.txt"
if hasm_path.exists():
    c_names = {n for c in (ROOT / f"src/main/{V}").glob("*.c")
               for n in function_definitions(c.read_text())}
    for line in hasm_path.read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        n = line.split()[0]
        if n not in funcs:
            sys.exit(f"hasm.txt: unknown function {n}")
        if n in c_names:
            sys.exit(f"hasm.txt: {n} is defined in C")
        a, s = funcs[n]
        units.append((a, a + s, n, "hasm"))
units.sort()
for (a, e, n, _), (a2, e2, n2, _) in zip(units, units[1:]):
    if e > a2:
        sys.exit(f"{n} and {n2} overlap")

off = lambda v: v - BASE_VRAM + FILE_OFF
lines = []
pos = TEXT_START
for a, e, n, kind in units:
    if a > pos:
        lines.append(f"      - [{off(pos):#x}, asm, main_{pos:08X}]")
    lines.append(f"      - [{off(a):#x}, {kind}, {n}]")
    pos = e
if pos < TEXT_END:
    lines.append(f"      - [{off(pos):#x}, asm, main_{pos:08X}]")

RO_START, RO_END = 0x80010000, 0x80012208
ro_units.sort()
ro_lines = []
pos = RO_START
for a, e, n in ro_units:
    if a < pos: sys.exit(f"rodata overlap at {a:#x} ({n})")
    if a > pos: ro_lines.append(f"      - [{off(pos):#x}, rodata, main_rodata_{pos:08X}]")
    ro_lines.append(f"      - [{off(a):#x}, .rodata, {n}]")
    pos = e
if pos < RO_END: ro_lines.append(f"      - [{off(pos):#x}, rodata, main_rodata_{pos:08X}]")

base = (ROOT / f"configs/{V}/main.base.yaml").read_text()
m = re.search(r"      - \[0x2A08, asm, main\]\n", base)
assert m, "base yaml must contain the text asm subsegment"
out = base.replace(m[0], "\n".join(lines) + "\n")
m2 = re.search(r"      - \[0x800, rodata, main\]\n", out)
assert m2, "base yaml must contain the rodata subsegment"
out = out.replace(m2[0], "\n".join(ro_lines) + "\n")
(ROOT / f"configs/{V}/main.yaml").write_text(out)
for kind in ("c", "hasm"):
    sel = [(a, e) for a, e, _, k in units if k == kind]
    print(f"{len(sel)} {kind} units, {sum(e - a for a, e in sel):#x} bytes")

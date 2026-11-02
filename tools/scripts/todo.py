#!/usr/bin/env python3
"""List functions that are not yet C. usage: todo.py [--chunks N] (writes configs/work/chunk_K.txt, balanced by size)"""
import re, sys
from pathlib import Path
from c_functions import function_definitions
ROOT = Path(__file__).resolve().parents[2]
done = set()
for c in (ROOT / "src/main/PAL").glob("*.c"):
    done.update(function_definitions(c.read_text()))
rows = []
for l in (ROOT / "configs/PAL/functions.txt").read_text().splitlines():
    n, a, s = l.split(); a = int(a, 16); s = int(s, 16)
    if n in done: continue
    asm = (ROOT / f"build/baseline/asm/nonmatchings/{n}/{n}.s").read_text() if (ROOT / f"build/baseline/asm/nonmatchings/{n}/{n}.s").exists() else ""
    flags = []
    if re.search(r"\b(mtc2|mfc2|ctc2|cfc2|lwc2|swc2|cop2|rtps|rtpt|nclip|mvmva|sqr|op |gpf|gpl)\b", asm): flags.append("gte")
    if re.search(r"\$f\d|lwc1|swc1|mtc1|mfc1", asm): flags.append("fpu")
    rows.append((a, s, n, ",".join(flags)))
print(f"pending: {len(rows)} functions, {sum(r[1] for r in rows):#x} bytes; done: {len(done)}")
if "--chunks" in sys.argv:
    k = int(sys.argv[sys.argv.index("--chunks") + 1])
    game = [r for r in rows if r[0] < 0x8007D000 and "gte" not in r[3]]
    chunks = [[] for _ in range(k)]
    # contiguous address ranges of roughly equal total size (adjacent functions share data/structs)
    total = sum(r[1] for r in game); acc = 0
    for r in game:
        chunks[min(k - 1, acc * k // total)].append(r); acc += r[1]
    for i, ch in enumerate(chunks):
        (ROOT / f"configs/work/chunk_{i}.txt").write_text("".join(f"{n} {a:#010x} {s:#x} {f}\n" for a, s, n, f in ch))
        print(f"chunk_{i}: {len(ch)} funcs, {sum(r[1] for r in ch):#x} bytes, {hex(ch[0][0])}-{hex(ch[-1][0])}")
    rest = [r for r in rows if r not in game]
    (ROOT / "configs/work/rest.txt").write_text("".join(f"{n} {a:#010x} {s:#x} {f}\n" for a, s, n, f in rest))
    print("rest (lib region / gte):", len(rest))

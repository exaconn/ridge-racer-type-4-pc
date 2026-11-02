#!/usr/bin/env python3
"""Name RR4 functions that are skeleton-identical to named functions in a reference exe (opcode+regs equal, all immediates masked).
usage: import_names.py REF.exe REF_sym.txt OUT_sym.txt [min_instrs]"""
import sys, re, collections
sys.path.insert(0, str(__import__("pathlib").Path(__file__).parent))
src = open(__import__("pathlib").Path(__file__).parent / "xmatch.py").read().replace("\nmain()\n", "\n")
exec(src)

def sk(f):
    return tuple((x & 0xFC000000) if (x >> 26) in (2, 3) else
                 (x if (x >> 26) in (0, 0x10, 0x12) or (x >> 26) == 1 else x & 0xFFFF0000) for x in f)

ref_exe, ref_sym, out = sys.argv[1:4]
mn = int(sys.argv[4]) if len(sys.argv) > 4 else 8
names = {}
for l in open(ref_sym):
    m = re.match(r"(\w+) = (0x[0-9A-Fa-f]+); // type:func", l)
    if m: names[int(m[2], 16)] = m[1]
a, ab = load("assets/PAL/main.exe"); b, bb = load(ref_exe)
idx = collections.defaultdict(list)
for s, f in split_funcs(b, bb):
    if len(f) >= mn: idx[sk(f)].append(bb + 4 * s)
res = []
for s, f in split_funcs(a, ab):
    if len(f) < mn: continue
    c = idx.get(sk(f))
    if c:
        nm = [names[t] for t in c if t in names]
        res.append((ab + 4 * s, len(f), nm, c))
named = [r for r in res if r[2]]
with open(out, "w") as o:
    for addr, n, nm, c in named:
        o.write(f"{nm[0]} = 0x{addr:08X}; // type:func size:0x{n*4:X}\n")
print(f"skeleton matches: {len(res)} funcs / {sum(r[1] for r in res)} instrs; named: {len(named)} funcs / {sum(r[1] for r in named)} instrs")
amb = sum(1 for r in named if len(set(r[2])) > 1); print("ambiguous names:", amb)

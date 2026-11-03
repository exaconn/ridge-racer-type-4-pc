#!/usr/bin/env python3
"""tu_coalesce.py <scratch dir> [max functions per unit]

Greedily merge address-adjacent units of one directory that share compiler markers (tumerge alias
mode, every candidate verified by fd.py, contiguous .rodata required). Prints the plan; with --apply
the merged text replaces the first file of each group and the others are deleted (then run
gen_config.py, make split, make check)."""
import re, subprocess, sys, json
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
M = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(M / 'tools/scripts'))
import tumerge
from c_functions import function_definitions
F = {l.split()[0]: (int(l.split()[1], 16), int(l.split()[2], 16)) for l in open(M / 'configs/PAL/functions.txt')}
args = [a for a in sys.argv[1:] if a != '--apply']
APPLY = '--apply' in sys.argv
OUT = Path(args[0]).resolve(); OUT.mkdir(exist_ok=True)
MAXFN = int(args[1]) if len(args) > 1 else 40
units = []
for p in sorted((M / 'src/main/PAL').rglob('*.c')):
    t = p.read_text(); fn = sorted(function_definitions(t), key=lambda n: F[n][0])
    if not fn: continue
    marks = tuple(re.findall(r'^/\*\s*(?:GCC_VERSION|CC1_FLAGS|MASPSX_FLAGS)\b[^*]*\*/', t, re.M))
    units.append(dict(path=p, funcs=fn, lo=F[fn[0]][0], hi=F[fn[-1]][0] + F[fn[-1]][1], marks=marks, dir=p.parent))
units.sort(key=lambda u: u['lo'])
def rodata(p): return [(int(a, 16), int(b, 16)) for a, b in re.findall(r'RODATA\s+(0x[0-9A-Fa-f]+)\s+(0x[0-9A-Fa-f]+)', p.read_text())]
def fd(p):
    r = subprocess.run([str(M / '.venv/bin/python'), str(M / 'tools/scripts/fd.py'), str(p), '--no-asm'], capture_output=True, text=True, cwd=M)
    o = r.stdout + r.stderr; ds = [int(x) for x in re.findall(r'Levenshtein: (\d+)', o)]
    return r.returncode == 0 and 'COMPILE ERROR' not in o and bool(ds) and not any(ds)
def try_merge(paths, marks, tag):
    ro = []
    for p in paths: ro += rodata(p)
    ro.sort()
    for (a, s), (b, _) in zip(ro, ro[1:]):
        if a + ((s + 3) & ~3) != b: return None
    for fm in ('proto', 'knr'):
        txt, conf, rod, fn = tumerge.merge(paths, list(marks), alias=True, fwd_mode=fm, root=M)
        if rod: txt = f'/* RODATA 0x{rod[0][0]:08x} 0x{sum((s + 3) & ~3 for a, s in rod):x} */\n' + txt
        o = OUT / f'{tag}.c'; o.write_text(txt)
        if fd(o): return txt
    return None
def run_dir(items):
    d, us = items; res = []; cur = None; k = 0
    for u in us:
        if cur is None: cur = dict(paths=[u['path']], hi=u['hi'], marks=u['marks'], nfn=len(u['funcs']), txt=None); continue
        ok = u['lo'] == cur['hi'] and u['marks'] == cur['marks'] and cur['nfn'] + len(u['funcs']) <= MAXFN
        txt = try_merge(cur['paths'] + [u['path']], cur['marks'], f'{d.name}_{k}') if ok else None
        k += 1
        if txt:
            cur['paths'].append(u['path']); cur['hi'] = u['hi']; cur['nfn'] += len(u['funcs']); cur['txt'] = txt
        else:
            res.append(cur); cur = dict(paths=[u['path']], hi=u['hi'], marks=u['marks'], nfn=len(u['funcs']), txt=None)
    if cur: res.append(cur)
    return [(r['paths'], r['txt']) for r in res]
bydir = {}
for u in units: bydir.setdefault(u['dir'], []).append(u)
with ThreadPoolExecutor(8) as ex: allres = list(ex.map(run_dir, bydir.items()))
plan = [r for rr in allres for r in rr]
json.dump([[[str(p) for p in ps], t is not None] for ps, t in plan], open(OUT / 'plan.json', 'w'))
merged = [r for r in plan if len(r[0]) > 1]
print(len(units), 'units ->', len(plan), 'after coalescing;', len(merged), 'merged groups')
if APPLY:
    for ps, txt in plan:
        if len(ps) > 1 and txt:
            ps[0].write_text(txt)
            for p in ps[1:]: p.unlink()

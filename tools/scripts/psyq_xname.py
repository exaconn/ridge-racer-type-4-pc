#!/usr/bin/env python3
"""Propose names for unnamed library functions by matching their bodies against named
functions in other PS1 builds (ELF with symbols + EXE with the code). A match needs the same
instruction sequence once jal targets and address-forming immediates are masked, and the key
must be unique on both sides.  usage: psyq_xname.py REF.elf [REF.elf ...]"""
import struct, subprocess, sys, re, collections
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
SKIP = re.compile(r'^(func_|D_|jtbl_|L[0-9A-F]{8}|\.|__)|^_?[A-Z0-9_]+_(VRAM|ROM|RAM|END|START)')

def norm(ins):
    out = []
    for x in ins:
        op = x >> 26; rs = (x >> 21) & 31
        if op in (2, 3): x &= 0xFC000000
        elif op == 0xf: x &= 0xFFFF0000
        elif (op in (9, 0xd, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2b) and rs not in (0, 29)):
            x &= 0xFFFF0000
        out.append(x)
    return tuple(out)

def exe_words(path):
    d = Path(path).read_bytes()
    la, sz = struct.unpack_from("<II", d, 0x18)
    return d[0x800:0x800 + sz], la

def named_funcs(elf):
    exe = Path(elf).with_suffix('.exe')
    if not exe.exists(): return []
    code, base = exe_words(exe)
    out = subprocess.run(['mipsel-linux-gnu-nm', '-n', '-S', str(elf)], capture_output=True, text=True).stdout
    syms = []
    for l in out.splitlines():
        p = l.split()
        if len(p) == 4 and p[2] in 'Tt': syms.append((int(p[0], 16), int(p[1], 16), p[3]))
        elif len(p) == 3 and p[1] in 'Tt': syms.append((int(p[0], 16), 0, p[2]))
    res = []
    for i, (a, s, n) in enumerate(syms):
        if SKIP.match(n): continue
        if not s and i + 1 < len(syms): s = syms[i + 1][0] - a
        o = a - base
        s &= ~3
        if s >= 24 and 0 <= o and o + s <= len(code):
            res.append((n, struct.unpack("<%dI" % (s // 4), code[o:o + s])))
    return res

def main():
    refs = collections.defaultdict(set)   # key -> names
    for elf in sys.argv[1:]:
        for n, w in named_funcs(elf): refs[norm(w)].add(n)
    ours = collections.defaultdict(list)
    code, base = exe_words(ROOT / 'build/PAL/main.exe')
    for line in (ROOT / 'configs/PAL/functions.txt').read_text().splitlines():
        p = line.split()
        if not p or not p[0].startswith('func_'): continue
        a, s = int(p[1], 16), int(p[2], 16)
        if s < 24: continue
        o = a - base
        ours[norm(struct.unpack("<%dI" % (s // 4), code[o:o + s]))].append((p[0], a))
    for k, fs in sorted(ours.items(), key=lambda kv: kv[1][0][1]):
        if len(fs) == 1 and k in refs and len(refs[k]) == 1:
            print(fs[0][0], next(iter(refs[k])), len(k))
main()

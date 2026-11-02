#!/usr/bin/env python3
"""Find functions shared between two PS-X EXEs by normalised instruction hashes.
usage: xmatch.py A.exe B.exe [min_instrs]"""
import struct, sys, collections

def load(p):
    d = open(p, "rb").read()
    load_addr, size = struct.unpack_from("<II", d, 0x18)
    return d[0x800:0x800 + size], load_addr

def split_funcs(code, base):
    n = len(code) // 4
    w = struct.unpack("<%dI" % n, code[:n * 4])
    funcs = []; i = 0
    while i < n:
        # skip padding/data: start at first plausible prologue-or-code word
        start = i; hi = i; j = i
        while j < n:
            x = w[j]; op = x >> 26
            t = None
            if op in (4, 5, 6, 7) or (op == 1 and ((x >> 16) & 0x1e) in (0, 0x10)):
                t = j + 1 + ((x & 0xffff) ^ 0x8000) - 0x8000
            elif op == 2:
                t = ((x & 0x3ffffff) << 2) + 0x80000000 - base >> 2
                t = t if base <= (t * 4 + base) < base + len(code) and abs(t - j) < 4096 else None
            if t is not None and t > hi and t - j < 2048: hi = t
            if x == 0x03e00008 and j + 1 >= hi:
                j += 2; break
            j += 1
        funcs.append((start, w[start:j]))
        i = j
    return funcs

def norm(ins, level):
    out = []
    for k, x in enumerate(ins):
        op = x >> 26
        if op == 3 or op == 2:               # jal/j
            x &= 0xFC000000
        elif level == 1:
            if op == 0xf: x &= 0xFFFF0000    # lui
            elif op in (9, 0xd) and k > 0:   # addiu/ori: only mask if paired with a lui earlier into same reg
                pass
        if level == 2:
            if op not in (0, 1, 0x10, 0x12): x &= 0xFFFF0000 if op not in (4,5,6,7) else 0xFFFF0000
        out.append(x)
    return tuple(out)

def main():
    a, ab = load(sys.argv[1]); b, bb = load(sys.argv[2]); mn = int(sys.argv[3]) if len(sys.argv) > 3 else 8
    fa = [(s, f) for s, f in split_funcs(a, ab) if len(f) >= mn]
    fb = [(s, f) for s, f in split_funcs(b, bb) if len(f) >= mn]
    print(f"A funcs>={mn}: {len(fa)}  B funcs>={mn}: {len(fb)}")
    for level, name in ((0, "exact (jal masked)"), (2, "skeleton (all imms masked)")):
        idx = collections.defaultdict(list)
        for s, f in fb: idx[norm(f, level) if level else tuple(x & 0xFC000000 if x >> 26 in (2, 3) else x for x in f)].append(s)
        hits = []
        for s, f in fa:
            key = norm(f, level) if level else tuple(x & 0xFC000000 if x >> 26 in (2, 3) else x for x in f)
            if key in idx: hits.append((ab + 4 * s, len(f), [bb + 4 * t for t in idx[key]]))
        print(f"{name}: {len(hits)} of {len(fa)} A functions, {sum(h[1] for h in hits)} instrs")
        for h in hits[:15]: print(f"   {h[0]:08x} len={h[1]:4d} <-> {' '.join('%08x' % t for t in h[2][:3])}")
main()

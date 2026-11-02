#!/usr/bin/env python3
"""Full rebuild + SHA check. With --prune, C files whose function bytes differ from the original are moved to src/failed/PAL/ and the
build is retried (max 5 rounds). usage: verify.py [--prune]"""
import re, shutil, subprocess, sys
from pathlib import Path
from c_functions import function_definitions
ROOT = Path(__file__).resolve().parents[2]
funcs = []
for l in (ROOT / "configs/PAL/functions.txt").read_text().splitlines():
    n, a, s = l.split(); funcs.append((int(a, 16), int(s, 16), n))

def build():
    subprocess.run("rm -rf asm/PAL linkers/PAL/main.ld build/PAL", shell=True, cwd=ROOT)
    r = subprocess.run(["make", "split"], cwd=ROOT, capture_output=True, text=True, errors="replace")
    if r.returncode: print(r.stdout[-1500:], r.stderr[-1500:]); return None
    r = subprocess.run(["make", "build", "-k"], cwd=ROOT, capture_output=True, text=True, errors="replace")
    errs = [l for l in (r.stdout + r.stderr).splitlines() if re.search(r"\*\*\*|rror|undefined reference|multiple definition", l) and "Warning" not in l]
    if r.returncode: return ("buildfail", errs)
    return ("ok", [])

def main():
    prune = "--prune" in sys.argv
    for rnd in range(5):
        res = build()
        if res is None: sys.exit(2)
        if res[0] == "buildfail":
            print("BUILD FAILED"); print("\n".join(res[1][:15]))
            bad = set(re.findall(r"src/main/PAL/(\w+)\.c", "\n".join(res[1])))
            if not (prune and bad): sys.exit(2)
            for b in bad:
                (ROOT / "src/failed/PAL").mkdir(parents=True, exist_ok=True)
                shutil.move(str(ROOT / f"src/main/PAL/{b}.c"), ROOT / f"src/failed/PAL/{b}.c"); print("pruned (build error)", b)
            continue
        a = (ROOT / "assets/PAL/main.exe").read_bytes(); b = (ROOT / "build/PAL/main.exe").read_bytes()
        if a == b:
            print("MATCH: build/PAL/main.exe is byte-identical;", len(list((ROOT / "src/main/PAL").glob("*.c"))), "C files"); return
        diffs = [i for i in range(min(len(a), len(b))) if a[i] != b[i]]
        print(f"MISMATCH: {len(diffs)} differing bytes, first at file offset {diffs[0]:#x}, sizes {len(a)} vs {len(b)}")
        bad = set()
        for off in diffs:
            v = off - 0x800 + 0x80010000
            for fa, fs, fn in funcs:
                if fa <= v < fa + fs: bad.add(fn); break
        print("functions with differing bytes:", sorted(bad)[:40])
        if not prune: sys.exit(1)
        moved = 0
        for p in (ROOT / "src/main/PAL").glob("*.c"):
            names = set(function_definitions(p.read_text()))
            if names & bad:
                (ROOT / "src/failed/PAL").mkdir(parents=True, exist_ok=True)
                shutil.move(str(p), ROOT / "src/failed/PAL" / p.name); moved += 1; print("pruned", p.name)
        if not moved: print("no C file owns the differing bytes (rodata/data placement?)"); sys.exit(1)
    sys.exit(1)

main()

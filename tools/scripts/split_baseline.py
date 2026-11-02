#!/usr/bin/env python3
"""Split the text segment into one asm file per function (build/baseline/asm/.../nonmatchings/<func>/<func>.s)
by pretending every function in functions.txt is a C unit. Used as input for m2c / decompilation helpers."""
import re, subprocess, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
V = "PAL"
base = (ROOT / f"configs/{V}/main.base.yaml").read_text()
funcs = [l.split() for l in (ROOT / f"configs/{V}/functions.txt").read_text().splitlines()]
off = lambda v: v - 0x80010000 + 0x800
sub = "\n".join(f"      - [{off(int(a, 16)):#x}, c, {n}]" for n, a, s in funcs) + "\n"
y = base.replace("      - [0x2A08, asm, main]\n", sub)
y = y.replace("asm_path: asm/PAL/main", "asm_path: build/baseline/asm")
y = y.replace("ld_script_path: linkers/PAL/main.ld", "ld_script_path: build/baseline/main.ld")
y = y.replace("linkers/PAL/undefined", "build/baseline/undefined")
(ROOT / "build/baseline").mkdir(parents=True, exist_ok=True)
p = ROOT / "build/baseline/main.yaml"
# base_path is relative to the yaml; keep paths valid
y = y.replace("src_path: src/main/PAL", "src_path: build/baseline/src")
p.write_text(y)
subprocess.run([str(ROOT / ".venv/bin/python"), "-m", "splat", "split", str(p)], check=True, cwd=ROOT, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
print(len(list((ROOT / "build/baseline/asm").rglob("nonmatchings/*/*.s"))), "function files")

#!/usr/bin/env python3
"""Try stock-option variants (GCC version marker, CC1_FLAGS, ASPSX version, MASPSX flags) on non-matching drafts.
usage: sweep_markers.py <draft.c>... ; prints MATCH variants. Never touches src/main/PAL."""
import itertools, os, re, subprocess, sys, tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
PY = str(ROOT / ".venv/bin/python")
GCC = ["", "/* GCC_VERSION: 2.7.2 */\n"]
EXTRA_CC1 = os.environ.get("SWEEP_CC1", "-mno-split-addresses;-fno-strength-reduce;-fno-strength-reduce -mno-split-addresses;-fno-schedule-insns2;-fno-schedule-insns;-fno-expensive-optimizations;-fcall-used-$1;-fcall-used-$1 -fno-schedule-insns2;-fcall-used-$1 -fno-expensive-optimizations;-fno-peephole;-fno-cse-skip-blocks;-fno-schedule-insns -fno-schedule-insns2;-fno-gcse;-fno-cse-follow-jumps;-fno-rerun-cse-after-loop;-fno-delayed-branch;-fno-thread-jumps;-fno-defer-pop;-fomit-frame-pointer;-ffast-math").split(";")
CC1 = [""] + [f"/* CC1_FLAGS: {x} */\n" for x in EXTRA_CC1 if x]
VERS = os.environ.get("SWEEP_VERS", "2.79;2.21;2.30;2.56;2.60;2.70").split(";")
MFL = os.environ.get("SWEEP_MFL", ";--dont-expand-li").split(";")
def strip_markers(t):
    return re.sub(r"^/\* (?:GCC_VERSION|CC1_FLAGS|NOSPLIT|FILL|ASPSX_VERSION|MASPSX_FLAGS)[^\n]*\*/\n", "", t, flags=re.M)
def run(args):
    src, g, c, v, m = args
    d = ROOT / "build/sweep"; stem = f"{src.stem}__{abs(hash((g,c,v,m)))%10**8}"
    f = d / f"{stem}.c"; f.write_text(g + c + strip_markers(src.read_text()))
    env = dict(os.environ, RR_ASPSX_VERSION=v, RR_MASPSX_FLAGS=m)
    r = subprocess.run([PY, str(ROOT / "tools/scripts/fd.py"), str(f)], capture_output=True, text=True, env=env, timeout=300)
    ok = r.returncode == 0 and "MATCH" in r.stdout and "MISMATCH" not in r.stdout
    f.unlink(missing_ok=True)
    return (src.name, g.strip(), c.strip(), v, m, ok)
jobs = [(Path(s), g, c, v, m) for s in sys.argv[1:] for g, c, v, m in itertools.product(GCC, CC1, VERS, MFL)]
with ThreadPoolExecutor(4) as ex:
    for name, g, c, v, m, ok in ex.map(run, jobs):
        if ok: print("MATCH", name, "|", g, "|", c, "| aspsx", v, "|", m or "-", flush=True)

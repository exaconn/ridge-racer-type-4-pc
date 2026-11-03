#!/usr/bin/env python3
"""Run fd.py on every src/main/PAL/*.c in parallel. With --move, files that do not MATCH go to src/failed/PAL/.
usage: fd_all.py [--move]"""
import shutil, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
files = sorted((ROOT / "src/main/PAL").rglob("*.c"))
def run(f):
    r = subprocess.run([str(ROOT / ".venv/bin/python"), str(ROOT / "tools/scripts/fd.py"), str(f), "--no-asm"], capture_output=True, text=True, errors="replace")
    return f, r.returncode, r.stdout
with ThreadPoolExecutor(16) as ex:
    res = list(ex.map(run, files))
bad = [(f, o) for f, rc, o in res if rc != 0]
print(f"{len(files) - len(bad)} ok, {len(bad)} failing")
for f, o in bad:
    print("FAIL", f.name, [l for l in o.splitlines() if "MISMATCH" in l or "ERROR" in l or "UNSUPPORTED" in l][:1])
    if "--move" in sys.argv:
        (ROOT / "src/failed/PAL").mkdir(parents=True, exist_ok=True); shutil.move(str(f), ROOT / "src/failed/PAL" / f.name)

#!/usr/bin/env python3
"""List empty-asm "barriers" (asm("" ...) register/CSE tricks) in src/main/PAL so they can be reviewed/removed later.
usage: list_barriers.py [--undocumented]   (documented = a `/* ASM_BARRIER: reason */` comment within 3 lines above/on the same line)"""
import re, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
pat = re.compile(r'__?asm__?\s*(?:volatile\s*)?\(\s*""')
only_bad = "--undocumented" in sys.argv
tot = bad = 0
for f in sorted((ROOT / "src/main/PAL").glob("*.c")):
    lines = f.read_text().splitlines()
    for i, l in enumerate(lines):
        if pat.search(l):
            tot += 1
            doc = any("ASM_BARRIER:" in x for x in lines[max(0, i - 3):i + 1])
            bad += not doc
            if not (only_bad and doc):
                print(f"{f.name}:{i+1}: {'ok ' if doc else 'UNDOCUMENTED '}{l.strip()}")
print(f"{tot} barriers, {bad} undocumented")

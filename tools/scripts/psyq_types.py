#!/usr/bin/env python3
"""Move byte-identical PsyQ type definitions that several units define privately into one
header named after the real SDK header (libgte.h, libspu.h, libsnd.h, libcd.h, libgpu.h).
Usage: psyq_types.py [--apply]; the caller verifies with `make check`."""
import re, sys, glob, collections
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
HDR = {"SVECTOR": "libgte/libgte", "VECTOR": "libgte/libgte", "MATRIX": "libgte/libgte",
       "SpuVolume": "libspu/libspu", "SpuVoiceAttr": "libspu/libspu", "SpuVoice": "libspu/libspu",
       "VagAtr": "libsnd/libsnd", "ProgAtr": "libsnd/libsnd", "VabHdr": "libsnd/libsnd",
       "CdlFILE": "libcd/libcd", "P_TAG": "libgpu/libgpu"}
PAT = re.compile(r"typedef struct (\w+)?\s*\{(.*?)\}\s*(\w+);\n?", re.S)
groups = collections.defaultdict(lambda: collections.defaultdict(list))
for p in sorted((ROOT / "src/main/PAL").rglob("*.c")):
    t = p.read_text(errors="replace")
    for m in PAT.finditer(t):
        if m[3] in HDR:
            groups[m[3]][re.sub(r"\s+", " ", m[0]).strip()].append((p, m[0]))
out = collections.defaultdict(list); edits = collections.defaultdict(list)
for name, bodies in groups.items():
    key, uses = max(bodies.items(), key=lambda kv: len(kv[1]))
    if len(uses) < 2: continue
    out[HDR[name]].append((name, uses[0][1].strip()))
    for p, txt in uses: edits[p].append((txt, HDR[name]))
for h, defs in out.items():
    print(h, [n for n, _ in defs])
if "--apply" in sys.argv:
    for h, defs in out.items():
        path = ROOT / "include/psyq" / (h + ".h")
        guard = "RR_PSYQ_" + h.split("/")[-1].upper() + "_H"
        path.write_text(f"#ifndef {guard}\n#define {guard}\n\n#include \"types.h\"\n\n" + "\n\n".join(t for _, t in defs) + f"\n\n#endif\n")
    for p, es in edits.items():
        t = p.read_text()
        for txt, h in es:
            t = t.replace(txt, "", 1)
        incs = sorted({h for _, h in es})
        line = "".join(f'#include "psyq/{h}.h"\n' for h in incs)
        t = t.replace('#include "types.h"\n', '#include "types.h"\n' + line, 1) if '#include "types.h"\n' in t else line + t
        p.write_text(t)

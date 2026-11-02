#!/usr/bin/env python3
"""Try candidate C files under compiler variants (2.8.1 default, 2.7.2) with fd.py and install winners into src/main/PAL.
usage: place.py file.c ..."""
import re, subprocess, sys
from pathlib import Path
from c_functions import function_definitions, source_owners
ROOT = Path(__file__).resolve().parents[2]
VARIANTS = ["", "/* GCC_VERSION: 2.7.2 */\n"]
done = skipped = 0
owners = source_owners(ROOT / "src/main/PAL")
for f in map(Path, sys.argv[1:]):
    dest = ROOT / "src/main/PAL" / f.name
    body = re.sub(r"^/\* (GCC_VERSION|LIBCC|PIPELINE)[^\n]*\*/\n", "", f.read_text(), flags=re.M)
    names = function_definitions(body)
    existing = set(names) & owners.keys()
    if existing:
        print("already owned, kept:", ", ".join(f"{n} ({owners[n].name})" for n in sorted(existing)))
        skipped += 1
        continue
    won = None
    for v in VARIANTS:
        t = ROOT / "build/place" / f.name; t.parent.mkdir(parents=True, exist_ok=True)
        t.write_text(v + body)
        r = subprocess.run([str(ROOT / ".venv/bin/python"), str(ROOT / "tools/scripts/fd.py"), str(t), "--no-asm"], capture_output=True, text=True, errors="replace")
        if r.returncode == 0:
            won = v; break
    if won is None: skipped += 1; print("no match:", f.name); continue
    if dest.exists(): print("exists, kept:", f.name); continue
    dest.write_text(won + body); done += 1; print("installed", f.name, "2.7.2" if won else "2.8.1")
    owners.update((n, dest) for n in names)
print(done, "installed;", skipped, "skipped")

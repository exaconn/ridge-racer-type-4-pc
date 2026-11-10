#!/usr/bin/env python3
"""List local cross-unit declarations for registered and address-based names.

These are candidates for review, not proof that unification cannot match.

usage: proto_conflicts.py [dir-prefix]     e.g. proto_conflicts.py game/menu
"""
import re, sys
from collections import defaultdict
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from tumerge import parse
from c_functions import known_functions
import unit_headers as U

prefix = sys.argv[1] if len(sys.argv) > 1 else ''
known = known_functions()
defs, decls = {}, defaultdict(list)
for p in sorted(U.SRC.rglob('*.c')):
    rel = str(p.relative_to(U.SRC))[:-2]
    _, items = parse(p.read_text())
    for k, t, n in items:
        if k == 'func' and n in known:
            defs[n] = (rel, U.proto_of(t))
    for k, t, n in items:
        core = U.lead_free(t)
        if k == 'decl' and n in known and re.search(r'\b' + n + r'\s*\(', U.strip_comments(core)) \
                and not core.startswith(('static', 'typedef')):
            decls[n].append((rel, re.sub(r'\s+', ' ', re.sub(r'^extern\s+', '', U.strip_comments(core)).strip())))
rows = []
for n, l in decls.items():
    if n not in defs:
        continue
    l = [x for x in l if x[0] != defs[n][0]]
    if not l or not defs[n][0].startswith(prefix):
        continue
    rows.append((n, defs[n], sorted(set(f for _, f in l)), sorted(set(u for u, _ in l))))
rows.sort(key=lambda r: (r[1][0], r[0]))
for n, (u, d), forms, users in rows:
    print(f'{u}: {d}\n    callers declare: {" | ".join(forms)}\n    in: {", ".join(users)}')
print(f'\n{len(rows)} functions with local cross-unit declarations')

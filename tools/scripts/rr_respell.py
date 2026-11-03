#!/usr/bin/env python3
"""rr_respell.py: where an include/rr/*.h prototype disagrees with the spelling every local caller needs
(`s32 f(s32 sel);` in rr/, `extern int f();` in the units), respell the rr/ declaration with the form all
local callers accept. Verification of a batch: make check (full EXE), the three fully built overlays,
and bisection when a batch fails. Run dedupe_rr.py afterwards to drop the now-redundant local copies."""
import re, subprocess, sys
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from tumerge import parse
import unit_headers as U

ROOT = U.ROOT
LINE = re.compile(r'^(?P<decl>[A-Za-z_][\w\s\*]*?\b(?P<name>func_[0-9A-Fa-f]{8})\s*\([^;{}()]*\))\s*;\s*$')


def sh(*a):
    return subprocess.run(a, cwd=ROOT, capture_output=True, text=True)


def verify():
    r = sh('make', '-j8', 'check')
    if 'main.exe: OK' not in r.stdout:
        return False
    for o in ('659', '663', '666'):
        r = sh('.venv/bin/python', 'tools/scripts/build_overlay.py', '--overlay', o)
        if 'identical' not in r.stdout:
            return False
    return True


def main():
    units, defs, local = {}, {}, defaultdict(list)
    for p in sorted(U.SRC.rglob('*.c')):
        units[p] = p.read_text()
        _, items = parse(units[p])
        for k, t, n in items:
            if k == 'func' and n:
                defs[n] = p
        for k, t, n in items:
            core = U.lead_free(t)
            if k == 'decl' and n and U.FUNC.match(n) and not core.startswith(('static', 'typedef')) \
                    and re.search(r'\b' + n + r'\s*\(', U.strip_comments(core)) and ',' not in U.strip_comments(core):
                local[n].append((p, core))
    rr = {}
    for h in sorted((U.INC / 'rr').glob('*.h')):
        for i, line in enumerate(h.read_text().splitlines()):
            m = LINE.match(line.strip())
            if m:
                rr.setdefault(m.group('name'), []).append((h, line))
    targets = [n for n in rr if n in local and n in defs and any(p != defs[n] for p, _ in local[n])]
    print('rr/ declarations with disagreeing local callers', len(targets))
    prov = {k.split()[-1]: v for k, v in U.type_providers().items()}
    known = U.known_types() | set(prov)

    def best(n):
        callers = [(p, c) for p, c in local[n] if p != defs[n]]
        forms = []
        for _, c in callers:
            f = re.sub(r'\s+', ' ', re.sub(r'^extern\s+', '', U.strip_comments(c)).strip())
            f = f if f.endswith(';') else f + ';'
            if f not in forms and U.usable(f, n, known):
                forms.append(f)
        for f in forms:
            ok = True
            for p, c in callers:
                t = units[p]
                i = t.find(c)
                p.write_text(t[:i] + f + t[i + len(c):])
                ok = U.fd_ok(p)
                p.write_text(t)
                if not ok:
                    break
            if ok:
                return n, f
        return n, None
    chosen = {}
    by_file = defaultdict(list)
    for n in targets:
        by_file[units[next(p for p, _ in local[n] if p != defs[n])]].append(n)
    with ThreadPoolExecutor(8) as ex:
        for n, f in ex.map(best, targets):
            if f:
                chosen[n] = f
    print('names with an accepted spelling', len(chosen))

    def apply(names):
        for n in names:
            for h, line in rr[n]:
                t = h.read_text()
                h.write_text(t.replace(line, line[:len(line) - len(line.lstrip())] + chosen[n], 1))

    def revert():
        sh('git', 'checkout', '--', 'include/rr')

    def run(names):
        if not names:
            return []
        revert()
        apply(names)
        if verify():
            return names
        revert()
        if len(names) == 1:
            return []
        mid = len(names) // 2
        a = run(names[:mid])
        b = run(names[mid:])
        revert()
        apply(a + b)
        return a + b
    good = run(sorted(chosen))
    revert()
    apply(good)
    print('respelled in rr/', len(good), 'of', len(chosen))


if __name__ == '__main__':
    main()

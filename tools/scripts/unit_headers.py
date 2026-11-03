#!/usr/bin/env python3
"""unit_headers.py: give every translation unit that exports functions its own header and make its
callers include it instead of repeating a local prototype.

  include/game/car/carmove1.h   <-  src/main/PAL/game/car/carmove1.c   (mirrored path, one header per unit)

A function is exported when another unit declares it locally and its signature uses only builtin /
types.h types. Callers are rewritten one by one; a caller that no longer matches at relocated
Levenshtein distance 0 (checked with fd.py) is restored. Headers nobody ended up including are removed.

usage: unit_headers.py [--dry-run] [--jobs N]
"""
import re, subprocess, sys
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from tumerge import parse, KNR

ROOT = Path(__file__).resolve().parents[2]
SRC = ROOT / 'src/main/PAL'
INC = ROOT / 'include'
KEYWORDS = {'extern', 'const', 'volatile', 'unsigned', 'signed', 'char', 'short', 'int', 'long', 'void',
            'float', 'double'}
FUNC = re.compile(r'func_[0-9A-Fa-f]{8}$')


def strip_comments(t):
    return re.sub(r'/\*.*?\*/|//[^\n]*', '', t, flags=re.S)


def lead_free(text):
    t = text.lstrip()
    while t.startswith(('/*', '//')):
        t = t[t.index('*/') + 2:] if t.startswith('/*') else t[t.find('\n') + 1:]
        t = t.lstrip()
    return t.strip()


def known_types():
    t = strip_comments((INC / 'types.h').read_text())
    return set(re.findall(r'\b(\w+)\s*;', t)) | {'M2C_UNK'}


def header_text():
    out = {}
    for h in INC.rglob('*.h'):
        if 'game' in h.relative_to(INC).parts[:1] or 'psyq' in h.relative_to(INC).parts[:1]:
            continue
        out[h] = h.read_text()
    return '\n'.join(out.values())


def proto_of(func_text):
    h = strip_comments(func_text).strip()
    if KNR.match(h):
        h = h[:h.index('(')].strip() + '()'
    else:
        h = h[:h.index('{')].strip()
    return re.sub(r'\s+', ' ', h) + ';'


def usable(proto, name, known):
    if proto.startswith('static'):
        return False
    ids = set(re.findall(r'[A-Za-z_]\w*', proto)) - {name}
    ids -= {i for i in ids if FUNC.match(i) or re.fullmatch(r'D_[0-9A-Fa-f]{8}', i)}
    return ids <= (known | KEYWORDS)


def fd_ok(p):
    r = subprocess.run([str(ROOT / '.venv/bin/python'), str(ROOT / 'tools/scripts/fd.py'), str(p), '--no-asm'],
                       capture_output=True, text=True, cwd=ROOT)
    ds = re.findall(r'Levenshtein: (\d+)', r.stdout + r.stderr)
    return r.returncode == 0 and bool(ds) and all(d == '0' for d in ds)


def main():
    dry = '--dry-run' in sys.argv
    jobs = int(sys.argv[sys.argv.index('--jobs') + 1]) if '--jobs' in sys.argv else 8
    known = known_types()
    existing = header_text()
    units = {}
    for p in sorted(SRC.rglob('*.c')):
        text = p.read_text()
        _, items = parse(text)
        units[p] = (text, items)
    owner = {}
    for p, (_, items) in units.items():
        for kind, t, name in items:
            if kind == 'func' and name and FUNC.match(name):
                proto = proto_of(t)
                if usable(proto, name, known) and not re.search(r'\b' + name + r'\b', existing):
                    owner[name] = (p, proto)
    users = defaultdict(lambda: defaultdict(list))        # owner unit -> caller -> [(name, core)]
    for p, (_, items) in units.items():
        for kind, t, name in items:
            if kind != 'decl' or name not in owner or owner[name][0] == p:
                continue
            core = lead_free(t)
            if re.search(r'\b' + name + r'\s*\(', strip_comments(core)) and not core.startswith(('typedef', 'static')):
                users[owner[name][0]][p].append((name, core))
    exported = defaultdict(set)
    for u, callers in users.items():
        for c, ds in callers.items():
            for n, _ in ds:
                exported[u].add(n)
    print('units exporting', len(exported), 'functions', sum(len(v) for v in exported.values()),
          'caller edges', sum(len(c) for c in users.values()))
    if dry:
        return

    def hpath(u):
        return INC / u.relative_to(SRC).with_suffix('.h')

    def addr(n):
        return int(n[5:], 16)
    for u, names in exported.items():
        h = hpath(u)
        h.parent.mkdir(parents=True, exist_ok=True)
        guard = 'RR_' + re.sub(r'\W', '_', str(u.relative_to(SRC).with_suffix(''))).upper() + '_H'
        body = [f'#ifndef {guard}', f'#define {guard}', '', '#include "types.h"', '']
        body += [owner[n][1] for n in sorted(names, key=addr)]
        body += ['', f'#endif /* {guard} */', '']
        h.write_text('\n'.join(body))

    callers = defaultdict(list)                           # caller -> [owner units]
    for u, cs in users.items():
        for c in cs:
            callers[c].append(u)
    orig = {c: units[c][0] for c in callers}

    def rewrite(c):
        text = orig[c]
        drop = []
        for u in callers[c]:
            for n, core in users[u][c]:
                i = text.find(core)
                if i < 0:
                    continue
                j = i + len(core)
                if j < len(text) and text[j] == '\n':
                    j += 1
                text = text[:i] + text[j:]
            drop.append(u)
        inc = ''.join(f'#include "{hpath(u).relative_to(INC).as_posix()}"\n' for u in sorted(drop))
        last = None
        for m in re.finditer(r'^#include[^\n]*\n', text, re.M):
            last = m
        if last:
            text = text[:last.end()] + inc + text[last.end():]
        else:
            m = re.match(r'(?:/\*[^\n]*\*/\n)*', text)
            text = text[:m.end()] + inc + text[m.end():]
        return re.sub(r'\n{3,}', '\n\n', text)

    for c in callers:
        c.write_text(rewrite(c))
    with ThreadPoolExecutor(jobs) as ex:
        res = list(ex.map(lambda c: (c, fd_ok(c)), list(callers)))
    bad = [c for c, ok in res if not ok]
    for c in bad:
        c.write_text(orig[c])
    used = {u for c in callers if c not in bad for u in callers[c]}
    for u in exported:
        if u not in used:
            hpath(u).unlink(missing_ok=True)
    print('callers rewritten', len(callers) - len(bad), 'restored', len(bad))
    for c in bad:
        print('  restored', c.relative_to(SRC))


if __name__ == '__main__':
    main()

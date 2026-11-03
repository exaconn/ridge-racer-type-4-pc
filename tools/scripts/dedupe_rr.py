#!/usr/bin/env python3
"""dedupe_rr.py: a unit that re-declares a symbol already declared in an include/rr/*.h header drops its
local copy and includes that header instead. Each unit is verified (strict fd) and restored on failure."""
import re, sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from tumerge import parse
import unit_headers as U

OWN = {}
for h in sorted((U.INC / 'rr').glob('*.h')):
    t = U.strip_comments(h.read_text())
    for m in re.finditer(r'^[^#\n][^;{}\n]*?\b((?:func|D)_[0-9A-Fa-f]{8})\b[^;{}]*;', t, re.M):
        OWN.setdefault(m.group(1), h)


def process(c):
    text = c.read_text()
    _, items = parse(text)
    new = text
    need = set()
    for kind, t, name in items:
        if kind != 'decl' or name not in OWN:
            continue
        core = U.lead_free(t)
        if core.startswith(('typedef', 'static')) or ',' in U.strip_comments(core):
            continue
        i = new.find(core)
        if i < 0:
            continue
        j = i + len(core)
        if j < len(new) and new[j] == '\n':
            j += 1
        new = new[:i] + new[j:]
        need.add(OWN[name])
    if not need:
        return c, None
    have = set(re.findall(r'#include "([^"]+)"', new))
    add = ''.join(f'#include "{h.relative_to(U.INC).as_posix()}"\n' for h in sorted(need)
                  if h.relative_to(U.INC).as_posix() not in have)
    last = None
    for m in re.finditer(r'^#include[^\n]*\n', new, re.M):
        last = m
    if add:
        if last:
            new = new[:last.end()] + add + new[last.end():]
        else:
            m = re.match(r'(?:/\*[^\n]*\*/\n)*', new)
            new = new[:m.end()] + add + new[m.end():]
    new = re.sub(r'\n{3,}', '\n\n', new)
    c.write_text(new)
    ok = U.fd_ok(c)
    if not ok:
        c.write_text(text)
        ok = per_decl(c, text, items)
    return c, ok


def per_decl(c, text, items):
    """salvage what we can: apply the deduplications one declaration at a time"""
    cur = text
    done = 0
    for kind, t, name in items:
        if kind != 'decl' or name not in OWN:
            continue
        core = U.lead_free(t)
        if core.startswith(('typedef', 'static')) or ',' in U.strip_comments(core):
            continue
        i = cur.find(core)
        if i < 0:
            continue
        j = i + len(core)
        if j < len(cur) and cur[j] == '\n':
            j += 1
        new = cur[:i] + cur[j:]
        inc = OWN[name].relative_to(U.INC).as_posix()
        if f'#include "{inc}"' not in new:
            last = None
            for m in re.finditer(r'^#include[^\n]*\n', new, re.M):
                last = m
            line = f'#include "{inc}"\n'
            if last:
                new = new[:last.end()] + line + new[last.end():]
            else:
                m = re.match(r'(?:/\*[^\n]*\*/\n)*', new)
                new = new[:m.end()] + line + new[m.end():]
        c.write_text(new)
        if U.fd_ok(c):
            cur = new
            done += 1
        else:
            c.write_text(cur)
    c.write_text(cur)
    return True if done else False


def main():
    with ThreadPoolExecutor(8) as ex:
        res = list(ex.map(process, sorted(U.SRC.rglob('*.c'))))
    print('deduplicated', sum(1 for _, ok in res if ok), 'units; restored', sum(1 for _, ok in res if ok is False))


if __name__ == '__main__':
    main()

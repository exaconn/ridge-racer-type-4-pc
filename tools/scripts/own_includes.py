#!/usr/bin/env python3
"""own_includes.py: make each unit include its own header (include/<unit>.h) and drop the local
forward prototypes / externs that header now provides. A unit that stops matching (checked with
fd.py, strict) is restored."""
import re, sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from tumerge import parse
import unit_headers as U

ROOT, SRC, INC = U.ROOT, U.SRC, U.INC


def process(c):
    h = INC / c.relative_to(SRC).with_suffix('.h')
    text = c.read_text()
    htxt = h.read_text()
    try:
        _, hitems = parse(htxt)
    except Exception:
        return c, None
    provided = {
        name for kind, t, name in hitems
        if kind == 'decl' and name and not t.lstrip().startswith(('typedef', 'struct', 'union', 'enum', 'static'))
    }
    _, items = parse(text)
    new = text
    inc_line = f'#include "{h.relative_to(INC).as_posix()}"\n'
    changed = False
    for kind, t, name in items:
        if kind != 'decl' or name not in provided:
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
        changed = True
    if inc_line not in new:
        last = None
        for m in re.finditer(r'^#include[^\n]*\n', new, re.M):
            last = m
        if last:
            new = new[:last.end()] + inc_line + new[last.end():]
        else:
            m = re.match(r'(?:/\*[^\n]*\*/\n)*', new)
            new = new[:m.end()] + inc_line + new[m.end():]
        changed = True
    if not changed:
        return c, None
    new = re.sub(r'\n{3,}', '\n\n', new)
    c.write_text(new)
    ok = U.fd_ok(c)
    if not ok:
        c.write_text(text)
    return c, ok


def main():
    units = [c for c in sorted(SRC.rglob('*.c')) if (INC / c.relative_to(SRC).with_suffix('.h')).exists()]
    with ThreadPoolExecutor(8) as ex:
        res = list(ex.map(process, units))
    done = sum(1 for _, ok in res if ok)
    bad = [c for c, ok in res if ok is False]
    print('units', len(units), 'now include their own header', done, 'restored', len(bad))
    for c in bad:
        print('  restored', c.relative_to(SRC))


if __name__ == '__main__':
    main()

#!/usr/bin/env python3
"""split_decls.py: `extern int func_A(), func_B();` -> one declaration per symbol (byte-neutral), so
each symbol can be owned by a header. Units that stop matching are restored (strict fd check)."""
import re, sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from tumerge import parse
import unit_headers as U


def split(core):
    body = U.strip_comments(core).strip().rstrip(';')
    if '=' in body or '{' in body:
        return None
    depth = 0
    cuts = []
    for i, ch in enumerate(body):
        depth += ch in '(['
        depth -= ch in ')]'
        if ch == ',' and depth == 0:
            cuts.append(i)
    if not cuts:
        return None
    pieces = [body[a:b].strip() for a, b in zip([0] + [c + 1 for c in cuts], cuts + [len(body)])]
    m = re.match(r'^(.*?[\w\s])\s*(\**)\s*([A-Za-z_]\w*)\s*((?:\(.*\)|\[.*\])?)$', pieces[0], re.S)
    if not m:
        return None
    base = m.group(1).strip()
    out = [f'{base} {m.group(2)}{m.group(3)}{m.group(4)};']
    out += [f'{base} {q};' for q in pieces[1:]]
    return out


def process(c):
    text = c.read_text()
    _, items = parse(text)
    new = text
    n = 0
    for kind, t, name in items:
        if kind != 'decl':
            continue
        core = U.lead_free(t)
        if core.startswith(('typedef', 'static', 'struct', 'union', 'enum')) or not core.startswith('extern'):
            continue
        parts = split(core)
        if not parts:
            continue
        i = new.find(core)
        if i < 0:
            continue
        new = new[:i] + '\n'.join(parts) + new[i + len(core):]
        n += 1
    if not n:
        return c, None
    c.write_text(new)
    ok = U.fd_ok(c)
    if not ok:
        c.write_text(text)
    return c, ok


def main():
    with ThreadPoolExecutor(8) as ex:
        res = list(ex.map(process, sorted(U.SRC.rglob('*.c'))))
    print('split in', sum(1 for _, ok in res if ok), 'units; restored', [str(c.relative_to(U.SRC)) for c, ok in res if ok is False])


if __name__ == '__main__':
    main()

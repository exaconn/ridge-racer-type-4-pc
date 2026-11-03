#!/usr/bin/env python3
"""retype_defs.py: a function whose callers declare a different return type than its definition
(`int f();` in the callers, `void f(int)` in the definition) gets the return type that every caller
accepts, when the owner unit and all callers still match at distance 0 (strict fd). The change makes the
definition's prototype a spelling all callers accept, so unit_headers.py can then hoist it."""
import re, sys
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from tumerge import parse
import unit_headers as U


def main():
    prov = {k.split()[-1]: v for k, v in U.type_providers().items()}
    known = U.known_types() | set(prov)
    units = {}
    for p in sorted(U.SRC.rglob('*.c')):
        units[p] = p.read_text()
    defs, decls = {}, defaultdict(list)
    for p, t in units.items():
        _, items = parse(t)
        for k, tx, n in items:
            if k == 'func' and n and U.FUNC.match(n):
                defs[n] = p
        for k, tx, n in items:
            core = U.lead_free(tx)
            if k == 'decl' and n and U.FUNC.match(n) and re.search(r'\b' + n + r'\s*\(', U.strip_comments(core)) \
                    and not core.startswith(('static', 'typedef')) and ',' not in U.strip_comments(core):
                decls[n].append((p, core))
    todo = []
    for n, l in decls.items():
        l = [x for x in l if x[0] != defs.get(n)]
        if n in defs and l:
            todo.append(n)

    def ret_of(core, n):
        m = re.match(r'^\s*(?:extern\s+)?(.*?)\b' + n + r'\s*\(', U.strip_comments(core), re.S)
        return re.sub(r'\s+', ' ', m.group(1)).strip() if m else None

    def attempt(n):
        owner = defs[n]
        text = units[owner]
        _, items = parse(text)
        dtext = next(t for k, t, nm in items if k == 'func' and nm == n)
        dret = ret_of(U.proto_of(dtext), n)
        rets = []
        for _, core in decls[n]:
            r = ret_of(core, n)
            if r and r != dret and r not in rets and U.usable(r + ' ' + n + '();', n, known):
                rets.append(r)
        for r in rets:
            m = re.search(r'(^|\n|\*/\s*)' + re.escape(dret) + r'(\s+\*?\s*)' + n + r'\s*\(', text)
            if not m:
                continue
            new_owner = text[:m.start()] + m.group(1) + r + m.group(2) + n + '(' + text[m.end():]
            owner.write_text(new_owner)
            ok = U.fd_ok(owner)
            if ok:
                good = True
                for c, core in decls[n]:
                    t = units[c]
                    i = t.find(core)
                    if i < 0:
                        good = False
                        break
                    proto = U.proto_of(re.sub(r'^\s*extern\s+', '', core).replace(';', '') + '{}')
                    ret_new = re.sub(re.escape(ret_of(core, n) or ''), r, core, count=1) if ret_of(core, n) else core
                    c.write_text(t[:i] + ret_new + t[i + len(core):])
                    if not U.fd_ok(c):
                        good = False
                    c.write_text(t)
                    if not good:
                        break
                if good:
                    return n, r, new_owner
            owner.write_text(text)
        return n, None, None

    res = []
    # sequential per owner file (they share files), parallel across functions of different owners
    by_owner = defaultdict(list)
    for n in todo:
        by_owner[defs[n]].append(n)

    def run_owner(o):
        out = []
        for n in by_owner[o]:
            units[o] = o.read_text()
            n_, r, new = attempt(n)
            if new:
                o.write_text(new)
                units[o] = new
            else:
                o.write_text(units[o])
            out.append((n_, r))
        return out
    with ThreadPoolExecutor(8) as ex:
        for out in ex.map(run_owner, list(by_owner)):
            res += out
    print('functions tried', len(todo), 'retyped', sum(1 for _, r in res if r))


if __name__ == '__main__':
    main()

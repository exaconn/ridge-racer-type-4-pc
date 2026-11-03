#!/usr/bin/env python3
"""split_rr.py: move function prototypes out of the big include/rr/*.h headers into the header of the unit
that defines the function (include/game|psyq/<unit>.h). The rr/ header keeps working: it #includes the
unit header, so host tests and overlays see the same declarations. A unit can now include exactly the
declarations it needs instead of a whole rr/ header.

Only prototypes whose types come from types.h or from headers the rr/ header already includes are moved
(a prototype that needs a type defined inside the same rr/ header stays).
usage: split_rr.py            (verification is done separately: make check, overlays, tests)
"""
import re, sys
from collections import defaultdict
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from tumerge import parse
import unit_headers as U

INC, SRC = U.INC, U.SRC


def unit_header(u):
    return INC / u.relative_to(SRC).with_suffix('.h')


def guard(u):
    return 'RR_' + re.sub(r'\W', '_', str(u.relative_to(SRC).with_suffix(''))).upper() + '_H'


def shape(proto, name):
    """(return type, parameter count or None for an unprototyped list)"""
    m = re.match(r'^\s*(?:extern\s+)?(.*?)\b' + name + r'\s*\((.*)\)\s*;?\s*$', proto, re.S)
    ret = re.sub(r'\s+', ' ', m.group(1)).strip().replace('s32', 'int').replace('u32', 'unsigned').replace('unsigned int', 'unsigned')
    par = m.group(2).strip()
    if par == '':
        return ret, None
    if par == 'void':
        return ret, 0
    return ret, par.count(',') + 1


def compatible(form, dform, name):
    r1, n1 = shape(form, name)
    r2, n2 = shape(dform, name)
    return r1 == r2 and (n1 is None or n2 is None or n1 == n2)


def main():
    defs, dproto, locals_ = {}, {}, {}
    for p in sorted(SRC.rglob('*.c')):
        _, items = parse(p.read_text())
        for k, t, n in items:
            if k == 'func' and n and U.FUNC.match(n):
                defs[n] = p
                dproto[n] = U.proto_of(t)
        for k, t, n in items:
            c = U.lead_free(t)
            if k == 'decl' and n and U.FUNC.match(n) and not c.startswith(('typedef', 'static')) \
                    and re.search(r'\b' + n + r'\s*\(', U.strip_comments(c)):
                f = re.sub(r'\s+', ' ', re.sub(r'^extern\s+', '', U.strip_comments(c)).strip())
                locals_.setdefault(n, set()).add(f if f.endswith(';') else f + ';')
    prov_all = U.type_providers()
    known0 = U.known_types()
    uhdr = {}           # unit header -> {name: line}
    for p in set(defs.values()):
        h = unit_header(p)
        d = {}
        if h.exists():
            for l in h.read_text().splitlines():
                m = re.search(r'(?:func|D)_[0-9A-Fa-f]{8}', l)
                if m and not l.startswith(('#', '/*')):
                    d[m.group(0)] = l.strip()
        uhdr[h] = d
    # which headers does each unit see (direct includes closed over header includes)?
    hgraph = {}
    for h in INC.rglob('*.h'):
        hgraph[h.relative_to(INC).as_posix()] = set(re.findall(r'#include "([^"]+)"', h.read_text()))
    sees = {}
    for p in set(defs.values()):
        todo = list(re.findall(r'#include "([^"]+)"', p.read_text()))
        seen = set()
        while todo:
            x = todo.pop()
            if x in seen:
                continue
            seen.add(x)
            todo += list(hgraph.get(x, ()))
        sees[p] = seen
    extra_inc = defaultdict(set)
    moved_total = 0
    for H in sorted((INC / 'rr').glob('*.h')):
        text = H.read_text()
        _, items = parse(text)
        own_types = {k.split()[-1] for k, v in prov_all.items() if v == H}
        hinc = set(re.findall(r'#include "([^"]+)"', text))
        # types visible from this header without its own definitions
        vis = known0 | {k.split()[-1] for k, v in prov_all.items()
                        if v != H and v.relative_to(INC).as_posix() in hinc}
        new = text
        add_inc = set()
        moved = 0
        for kind, t, name in items:
            if kind != 'decl' or not name or name not in defs:
                continue
            core = U.lead_free(t)
            if core.startswith(('typedef', 'static', 'extern')) or ',' in U.strip_comments(core):
                continue
            if not re.search(r'\b' + name + r'\s*\(', U.strip_comments(core)):
                continue
            form = re.sub(r'\s+', ' ', U.strip_comments(core)).strip()
            if not U.usable(form, name, vis) or not compatible(form, dproto[name], name):
                continue
            if H.relative_to(INC).as_posix() in sees.get(defs[name], ()):
                continue                                # the owner includes this header: keep it there
            if locals_.get(name, set()) - {form}:
                continue                                # a unit declares it differently: leave in rr/
            uh = unit_header(defs[name])
            cur = uhdr.setdefault(uh, {})
            if name in cur and re.sub(r'\s+', ' ', cur[name]).strip() != form:
                continue                                  # a different spelling already lives there
            i = new.find(core)
            if i < 0:
                continue
            j = i + len(core)
            if j < len(new) and new[j] == '\n':
                j += 1
            new = new[:i] + new[j:]
            cur.setdefault(name, form)
            add_inc.add(uh.relative_to(INC).as_posix())
            ids = U.type_ids(form, name) & {k.split()[-1] for k in prov_all}
            for tname in ids:
                for k, v in prov_all.items():
                    if k.split()[-1] == tname and v != H:
                        extra_inc[uh].add(v.relative_to(INC).as_posix())
            moved += 1
        if not moved:
            continue
        last = None
        for m in re.finditer(r'^#include[^\n]*\n', new, re.M):
            last = m
        inc = ''.join(f'#include "{i}"\n' for i in sorted(add_inc) if f'#include "{i}"' not in new)
        if last:
            new = new[:last.end()] + inc + new[last.end():]
        else:
            m = re.search(r'#define[^\n]*\n', new)
            new = new[:m.end()] + inc + new[m.end():] if m else inc + new
        H.write_text(re.sub(r'\n{3,}', '\n\n', new))
        moved_total += moved
    for uh, d in uhdr.items():
        if not d:
            continue
        u = next(p for p in defs.values() if unit_header(p) == uh)
        g = guard(u)
        lines = [f'#ifndef {g}', f'#define {g}', '', '#include "types.h"']
        lines += [f'#include "{i}"' for i in sorted(extra_inc.get(uh, ()))]
        lines += [''] + [d[n] for n in sorted(d, key=lambda x: (x.startswith('func_'), int(x.split('_')[1], 16)))] + ['', f'#endif /* {g} */', '']
        uh.parent.mkdir(parents=True, exist_ok=True)
        uh.write_text('\n'.join(lines))
    print('prototypes moved out of rr/ headers:', moved_total)


if __name__ == '__main__':
    main()

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


def type_ids(proto, name):
    """identifiers used as types: parameter names are not counted"""
    head, _, rest = proto.partition('(')
    ids = set(re.findall(r'[A-Za-z_]\w*', head)) - {name}
    params = rest.rsplit(')', 1)[0]
    for prm in params.split(','):
        prm = re.sub(r'\[[^\]]*\]', '', prm)
        toks = re.findall(r'[A-Za-z_]\w*|\*|\.\.\.', prm)
        words = [t for t in toks if t not in ('*', '...')]
        if len(words) > 1 and words[-1] not in KEYWORDS:
            words = words[:-1]                      # the parameter name
        ids |= set(words)
    return ids


def usable(proto, name, known):
    if proto.startswith('static'):
        return False
    ids = type_ids(proto, name)
    ids -= {i for i in ids if FUNC.match(i) or re.fullmatch(r'D_[0-9A-Fa-f]{8}', i)}
    return ids <= (known | KEYWORDS)


def fd_ok(p):
    r = subprocess.run([str(ROOT / '.venv/bin/python'), str(ROOT / 'tools/scripts/fd.py'), str(p), '--no-asm'],
                       capture_output=True, text=True, cwd=ROOT)
    out = r.stdout + r.stderr
    ds = re.findall(r'Levenshtein: (\d+)', out)
    bad = re.search(r'conflicting types|error:|\berror\b.*\n|previous declaration|undeclared', out)
    _, items = parse(Path(p).read_text())
    nfunc = sum(1 for k, _, _ in items if k == 'func')
    return r.returncode == 0 and not bad and len(ds) >= nfunc and all(d == '0' for d in ds)


def multi_declared(items):
    """names that appear in a declaration with several declarators (`extern int a(), b();`)"""
    out = set()
    for kind, t, name in items:
        if kind != 'decl':
            continue
        core = strip_comments(t)
        depth = 0
        commas = 0
        for ch in core:
            depth += ch in '([{'
            depth -= ch in ')]}'
            if ch == ',' and depth == 0:
                commas += 1
        if commas:
            out |= set(re.findall(r'func_[0-9A-Fa-f]{8}', core))
    return out


def norm_decl(core):
    core = re.sub(r'\s+', ' ', strip_comments(core)).strip()
    return re.sub(r'^extern\s+', '', core)


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
    blocked = set()
    for p, (_, items) in units.items():
        blocked |= multi_declared(items)
    owner = {}
    for p, (_, items) in units.items():
        for kind, t, name in items:
            if kind == 'func' and name and FUNC.match(name) and name not in blocked:
                proto = proto_of(t)
                if usable(proto, name, known) and not re.search(r'\b' + name + r'\b', existing):
                    owner[name] = (p, proto)
    edges = defaultdict(list)                 # caller -> [(name, core, [candidate forms])]
    forms = defaultdict(list)
    for p, (_, items) in units.items():
        for kind, t, name in items:
            if kind != 'decl' or name not in owner or owner[name][0] == p:
                continue
            core = lead_free(t)
            if re.search(r'\b' + name + r'\s*\(', strip_comments(core)) and not core.startswith(('typedef', 'static')):
                edges[p].append((name, core))
                f = norm_decl(core)
                if f not in forms[name] and usable(f, name, known):
                    forms[name].append(f if f.endswith(';') else f + ';')
    # ---- shared global variables: owner inferred from the monotonic data layout ----
    import bisect
    VAR = re.compile(r'D_[0-9A-Fa-f]{8}$')
    unit_addr = {}
    for p, (_, items) in units.items():
        a = [int(n[5:], 16) for k, _, n in items if k == 'func' and n and FUNC.match(n)]
        if a:
            unit_addr[p] = min(a)
    rank = {p: i for i, p in enumerate(sorted(unit_addr, key=unit_addr.get))}
    vusers = defaultdict(dict)
    for p, (_, items) in units.items():
        for kind, t, name in items:
            if kind != 'decl' or not name or not VAR.match(name) or name in blocked or p not in rank or re.search(r'\b' + name + r'\b', existing):
                continue
            core = lead_free(t)
            if core.startswith('extern') and '(' not in core and ',' not in strip_comments(core):
                vusers[name][p] = core
    anchors = sorted((int(n[2:], 16), rank[next(iter(u))]) for n, u in vusers.items() if len(u) == 1)
    akeys = [a for a, _ in anchors]

    def interp(addr):
        i = bisect.bisect_left(akeys, addr)
        lo = anchors[max(i - 1, 0)][1]
        hi = anchors[min(i, len(anchors) - 1)][1]
        return (lo + hi) / 2
    vedges = defaultdict(list)
    for name, us in vusers.items():
        if len(us) < 2:
            continue
        want = interp(int(name[2:], 16))
        own = min(us, key=lambda q: abs(rank[q] - want))
        fl = []
        for q, core in us.items():
            f = norm_decl(core)
            f = f if f.endswith(';') else f + ';'
            f = 'extern ' + f
            if f not in fl and usable(f, name, known):
                fl.append(f)
        if not fl:
            continue
        owner[name] = (own, fl[0])
        forms[name] = fl
        for q, core in us.items():
            vedges[q].append((name, core))
    for q, v in vedges.items():
        edges[q].extend(v)
    cands = {n: [owner[n][1]] + [f for f in forms[n] if f != owner[n][1]] for n in {e[0] for v in edges.values() for e in v}}
    print('functions with callers', len(cands), 'caller edges', sum(len(v) for v in edges.values()))
    if dry:
        return

    def hpath(u):
        return INC / u.relative_to(SRC).with_suffix('.h')

    def test_caller(c):
        """for every function declared in caller c find the candidate indexes that keep distance 0"""
        text = units[c][0]
        ok = {}
        for name, core in edges[c]:
            for ci, cand in enumerate(cands[name][:4]):
                i = text.find(core)
                if i < 0:
                    break
                t2 = text[:i] + cand + text[i + len(core):]
                c.write_text(t2)
                if fd_ok(c):
                    ok.setdefault(name, set()).add(ci)
        c.write_text(text)
        return c, ok

    with ThreadPoolExecutor(jobs) as ex:
        res = dict(ex.map(test_caller, list(edges)))
    chosen = {}
    for name in cands:
        sets = [res[c].get(name, set()) for c in edges if any(e[0] == name for e in edges[c])]
        common = set.intersection(*sets) if sets else set()
        if common:
            chosen[name] = min(common)
    print('functions with a form accepted by all callers', len(chosen), 'of', len(cands))

    banned = set()
    base_hdr = {hpath(owner[n][0]): (hpath(owner[n][0]).read_text() if hpath(owner[n][0]).exists() else '') for n in chosen}
    for rnd in range(6):
        active = {n: cands[n][i] for n, i in chosen.items() if n not in banned}
        byunit = defaultdict(list)
        for n, f in active.items():
            byunit[owner[n][0]].append((n, f))
        for u, lst in byunit.items():
            h = hpath(u)
            h.parent.mkdir(parents=True, exist_ok=True)
            guard = 'RR_' + re.sub(r'\W', '_', str(u.relative_to(SRC).with_suffix(''))).upper() + '_H'
            body = [f'#ifndef {guard}', f'#define {guard}', '', '#include "types.h"', '']
            old = {}
            if base_hdr.get(h):
                for l in base_hdr[h].splitlines():
                    m = re.search(r'(?:func|D)_[0-9A-Fa-f]{8}', l)
                    if m and not l.startswith(('#', '/*')):
                        old[m.group(0)] = l
            old.update({n: f for n, f in lst})
            body += [old[n] for n in sorted(old, key=lambda x: (x.startswith('func_'), int(x.split('_')[1], 16)))]
            body += ['', f'#endif /* {guard} */', '']
            h.write_text('\n'.join(body))
        changed = {}
        for c, ds in edges.items():
            text = units[c][0]
            owners_used = set()
            for name, core in ds:
                if name not in active:
                    continue
                i = text.find(core)
                if i < 0:
                    continue
                j = i + len(core)
                if j < len(text) and text[j] == '\n':
                    j += 1
                text = text[:i] + text[j:]
                owners_used.add(owner[name][0])
            if not owners_used:
                continue
            inc = ''.join(f'#include "{hpath(u).relative_to(INC).as_posix()}"\n' for u in sorted(owners_used))
            last = None
            for m in re.finditer(r'^#include[^\n]*\n', text, re.M):
                last = m
            if last:
                text = text[:last.end()] + inc + text[last.end():]
            else:
                m = re.match(r'(?:/\*[^\n]*\*/\n)*', text)
                text = text[:m.end()] + inc + text[m.end():]
            changed[c] = re.sub(r'\n{3,}', '\n\n', text)
        for c, t in changed.items():
            c.write_text(t)
        with ThreadPoolExecutor(jobs) as ex:
            r = list(ex.map(lambda c: (c, fd_ok(c)), list(changed)))
        bad = [c for c, ok in r if not ok]
        for c in changed:
            c.write_text(units[c][0])
        if not bad:
            for c, t in changed.items():
                c.write_text(t)
            print('round', rnd, 'callers rewritten', len(changed))
            return
        for c in bad:
            banned |= {n for n, _ in edges[c]}
        for h, t in base_hdr.items():
            if t:
                h.write_text(t)
            else:
                h.unlink(missing_ok=True)
        print('round', rnd, 'failed callers', len(bad), 'banned functions', len(banned))


if __name__ == '__main__':
    main()

#!/usr/bin/env python3
"""tumerge.py: parse a C translation unit into items and merge several files into one TU
(used to build src/main/PAL/{game,psyq}; see docs/SOURCE_LAYOUT.md).

parse(text) -> markers, items ; merge(paths, header_markers) -> text
Items: ('pp', text), ('decl', text, name), ('func', text, name). Leading comments stay attached.
"""
import re
from pathlib import Path

MARKER = re.compile(r'^/\*\s*(GCC_VERSION|CC1_FLAGS|MASPSX_FLAGS|ASPSX_VERSION|NOSPLIT|RODATA)\b[^*]*\*/[ \t]*\n?', re.M)


def _skip_ws(t, i):
    while i < len(t) and t[i] in ' \t\r\n': i += 1
    return i


KNR = re.compile(r'[^;{}=()]*\w\s*\([\w\s,]*\)\s*(?:[^;{}()]+;\s*)+\{')


def _body_end(text, k):
    d = 0
    while True:
        ch = text[k]
        if text.startswith('/*', k): k = text.index('*/', k) + 2; continue
        if text.startswith('//', k): k = text.find('\n', k); continue
        if ch == '"' or ch == "'":
            q = ch; k += 1
            while text[k] != q: k += 2 if text[k] == '\\' else 1
            k += 1; continue
        if ch == '{': d += 1
        elif ch == '}':
            d -= 1
            if d == 0: return k + 1
        k += 1


def parse(text):
    """Return (marker_lines, items)."""
    marks = [m.group(0).strip() for m in MARKER.finditer(text)]
    text = MARKER.sub('', text)
    items = []; i = 0; n = len(text); lead = ''
    while True:
        j = _skip_ws(text, i)
        if j >= n: break
        # leading comments
        if text.startswith('/*', j):
            e = text.index('*/', j) + 2
            lead += text[j:e] + '\n'; i = e; continue
        if text.startswith('//', j):
            e = text.find('\n', j); e = n if e < 0 else e
            lead += text[j:e] + '\n'; i = e; continue
        start = j
        if text[j] == '#':
            e = j
            while True:
                k = text.find('\n', e)
                if k < 0: e = n; break
                if text[k - 1] == '\\': e = k + 1; continue
                e = k; break
            items.append(['pp', lead + text[start:e], None]); lead = ''; i = e; continue
        # general item
        depth = par = 0; k = j; incom = False; first_brace = None
        mk = KNR.match(text, j)
        if mk:
            k = mk.end() - 1
            end = _body_end(text, k)
            name = re.findall(r'[A-Za-z_]\w*', text[j:k].split('(')[0])[-1]
            items.append(['func', lead + text[j:end], name]); lead = ''; i = end; continue
        while k < n:
            c = text[k]
            if text.startswith('/*', k):
                k = text.index('*/', k) + 2; continue
            if text.startswith('//', k):
                k = text.find('\n', k); k = n if k < 0 else k; continue
            if c == '"' or c == "'":
                q = c; k += 1
                while text[k] != q:
                    k += 2 if text[k] == '\\' else 1
                k += 1; continue
            if c == '(': par += 1
            elif c == ')': par -= 1
            elif c == '{':
                if depth == 0 and par == 0:
                    first_brace = k
                    head = re.sub(r'/\*.*?\*/', '', text[j:k], flags=re.S).strip()
                    isfunc = bool(re.search(r'\)\s*(?:[^{};()]*;\s*)*$', head)) and not re.match(r'(typedef|struct|union|enum)\b[^()]*$', head)
                    if isfunc:
                        # skip to matching brace
                        d = 0
                        while True:
                            ch = text[k]
                            if text.startswith('/*', k): k = text.index('*/', k) + 2; continue
                            if text.startswith('//', k): k = text.find('\n', k); continue
                            if ch == '"' or ch == "'":
                                q = ch; k += 1
                                while text[k] != q: k += 2 if text[k] == '\\' else 1
                                k += 1; continue
                            if ch == '{': d += 1
                            elif ch == '}':
                                d -= 1
                                if d == 0: k += 1; break
                            k += 1
                        body = text[j:k]
                        m = re.search(r'([A-Za-z_]\w*)\s*\(', re.sub(r'/\*.*?\*/', '', head.split('(')[0] + '(', flags=re.S))
                        name = re.findall(r'[A-Za-z_]\w*', head.split('(')[0])[-1]
                        items.append(['func', lead + body, name]); lead = ''; i = k; break
                depth += 1
            elif c == '}': depth -= 1
            elif c == ';' and depth == 0 and par == 0:
                k += 1
                body = text[j:k]
                items.append(['decl', lead + body, _declname(body)]); lead = ''; i = k; break
            k += 1
        else:
            raise ValueError('unterminated item near: ' + text[j:j + 80])
    if lead.strip(): items.append(['pp', lead.rstrip(), None])
    return marks, items


def _declname(body):
    b = re.sub(r'/\*.*?\*/|//[^\n]*', '', body, flags=re.S)
    b = re.sub(r'\s+', ' ', b).strip()
    m = re.match(r'typedef\b', b)
    if m:
        # function pointer typedef or ordinary typedef: last identifier before ';' (or inside (*name))
        mm = re.search(r'\(\s*\*\s*([A-Za-z_]\w*)\s*\)', b)
        if mm: return mm.group(1)
        mm = re.findall(r'[A-Za-z_]\w*', re.sub(r'\[[^\]]*\]', '', b))
        return mm[-1] if mm else None
    m = re.match(r'(struct|union|enum)\s+([A-Za-z_]\w*)\s*\{', b)
    if m: return m.group(1) + ' ' + m.group(2)
    # variable / function declaration
    b2 = re.sub(r'=.*', '', b)
    mm = re.search(r'\(\s*\*\s*([A-Za-z_]\w*)', b2)
    if mm: return mm.group(1)
    b3 = re.sub(r'\([^()]*\)\s*;?$', '', b2) if re.search(r'\w\s*\([^()]*\)\s*;$', b2) else b2
    ids = re.findall(r'[A-Za-z_]\w*', re.sub(r'\[[^\]]*\]', ' ', b3))
    return ids[-1] if ids else None


def norm(t):
    t = re.sub(r'/\*.*?\*/|//[^\n]*', '', t, flags=re.S)
    return re.sub(r'\s+', '', t)


def merge(paths, markers_out, policy=None, fwd_mode='proto'):
    """paths in address order; markers_out: list of marker lines for the merged file.
    policy: dict name -> index of the variant to keep when declarations conflict (default 0)."""
    policy = policy or {}
    pp, types, externs, funcs = [], [], [], []
    seen_pp = set(); seen = {}; conflicts = {}
    ro = []; seen_types = {}; seen_types_ct = {}; seen_funcs = set(); seen_funcs_ct = {}
    for p in paths:
        t = Path(p).read_text()
        marks, items = parse(t)
        # local type / static-function names that collide with a different earlier definition are renamed
        ren = {}
        for kind, text, name in items:
            if not name: continue
            istype = kind == 'decl' and text.lstrip().startswith(('typedef', 'struct', 'union', 'enum'))
            isstat = kind == 'func' and not name.startswith('func_')
            if istype and name in seen_types and seen_types[name] != norm(text):
                ren[name] = f'{name}_{len(seen_types_ct.setdefault(name, [0]))}'
                seen_types_ct[name].append(0)
            elif isstat and name in seen_funcs:
                ren[name] = f'{name}_{len(seen_funcs_ct.setdefault(name, [0]))}'
                seen_funcs_ct[name].append(0)
        if ren:
            def sub(x):
                for a, b in ren.items():
                    x = re.sub(r'(?<![\w$])' + re.escape(a.split()[-1]) + r'(?![\w$])', b.split()[-1], x)
                return x
            items = [[k_, sub(t_), (ren.get(n_, n_) if n_ else n_)] for k_, t_, n_ in items]
        for kind, text, name in items:
            if kind == 'decl' and name and text.lstrip().startswith(('typedef', 'struct', 'union', 'enum')):
                seen_types.setdefault(name, norm(text))
            if kind == 'func' and name and not name.startswith('func_'): seen_funcs.add(name)
        for m in marks:
            mm = re.match(r'/\*\s*RODATA\s+(0x[0-9A-Fa-f]+)\s+(0x[0-9A-Fa-f]+)', m)
            if mm: ro.append((int(mm[1], 16), int(mm[2], 16)))
        for kind, text, name in items:
            if kind == 'pp':
                k = norm(text)
                if k in seen_pp: continue
                seen_pp.add(k); pp.append(text)
            elif kind == 'decl':
                k = norm(text)
                bucket = seen.setdefault(name, [])
                if any(b[0] == k for b in bucket): continue
                bucket.append((k, text, p))
                is_extern = bool(re.match(r'\s*(?:/\*.*?\*/\s*)*extern\b', text, re.S)) or bool(
                    re.search(r'\)\s*;\s*$', norm(text) and text)) and not text.lstrip().startswith(('typedef', 'struct', 'union', 'enum'))
                (types if text.lstrip().startswith(('typedef', 'struct', 'union', 'enum')) else externs).append((name, text))
            else:
                funcs.append((name, text, p))
    for name, b in seen.items():
        if len(b) > 1: conflicts[name] = [x[2] for x in b]
    # choose variants for conflicting declarations
    def pick(entries):
        out = []; used = set()
        for name, text in entries:
            if name in conflicts and name is not None:
                cands = [e for e in entries if e[0] == name]
                keep = cands[min(policy.get(name, 0), len(cands) - 1)]
                if keep[1] is not text: continue
            out.append(text)
        return out
    # externs for names defined in this TU: replace by a forward declaration taken from the definition
    defined = {f[0]: f[1] for f in funcs}
    fwd = []
    for name, text in list(externs):
        pass
    externs = [(n_, t_) for n_, t_ in externs if n_ not in defined]
    for name, text, _ in funcs:
        h = re.sub(r'/\*.*?\*/|//[^\n]*', '', text, flags=re.S).strip()
        if KNR.match(h) or fwd_mode == 'knr':
            h = h[:h.index('(')].strip() + '()'
        else:
            h = h[:h.index('{')].strip()
        if not h.startswith('static'): fwd.append(re.sub(r'\s+', ' ', h) + ';')
    parts = []
    parts += markers_out
    inc = [x for x in pp if x.lstrip().startswith('#include')]
    other_pp = [x for x in pp if not x.lstrip().startswith('#include')]
    inc.sort(key=lambda x: 0 if 'types.h' in x else 1)
    body = []
    if inc: body.append('\n'.join(s.strip() for s in inc))
    if other_pp: body.append('\n'.join(s.strip() for s in other_pp))
    for t in pick(types): body.append(t.strip())
    ex = pick(externs)
    if ex: body.append('\n'.join(s.strip() for s in ex))
    if fwd and fwd_mode != 'none': body.append('\n'.join(fwd))
    for _, t, _ in funcs: body.append(t.strip())
    head = '\n'.join(parts)
    return (head + '\n' if head else '') + '\n\n'.join(body) + '\n', conflicts, ro, [f[0] for f in funcs]

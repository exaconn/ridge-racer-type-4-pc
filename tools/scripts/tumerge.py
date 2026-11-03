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
    if not b.startswith('typedef') and re.match(r'(?:extern\s+)?(?:struct|union)\s*[A-Za-z_]*\s*\{', b):
        d_ = 0; e_ = None
        for i_, c_ in enumerate(b):                       # drop the member list: `extern struct {..} *name;`
            if c_ == '{': d_ += 1
            elif c_ == '}':
                d_ -= 1
                if d_ == 0: e_ = i_; break
        if e_ is not None and re.search(r'[A-Za-z_]', b[e_ + 1:]):
            b = b[:b.index('{')] + b[e_ + 1:]
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



SYN = [(r'\bu8\b|\bU8\b', 'unsignedchar'), (r'\bs8\b', 'signedchar'), (r'\bu16\b', 'unsignedshort'),
       (r'\bs16\b', 'short'), (r'\bu32\b', 'unsignedint'), (r'\bs32\b', 'int')]


def key(text):
    t = re.sub(r'/\*.*?\*/|//[^\n]*', '', text, flags=re.S)
    for a, b in SYN: t = re.sub(a, b, t)
    t = re.sub(r'\s+', '', t).replace('unsignedint', 'unsigned').replace('unsignedlong', 'unsignedlong')
    return re.sub(r'^extern', '', t)


def split_declarators(text, name_of=None):
    """`extern volatile int *a, *b;` -> two single declarations (simple cases only)."""
    body = re.sub(r'/\*.*?\*/', '', text, flags=re.S).strip().rstrip(';')
    if '=' in body or '{' in body or body.lstrip().startswith(('typedef', 'struct', 'union', 'enum')):
        return [text]
    depth = 0; cuts = []
    for i, c in enumerate(body):
        if c in '([': depth += 1
        elif c in ')]': depth -= 1
        elif c == ',' and depth == 0: cuts.append(i)
    if not cuts: return [text]
    pieces = [body[a:b] for a, b in zip([0] + [c + 1 for c in cuts], cuts + [len(body)])]
    m = re.match(r'^(.*?)([*\s(]*)([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*(\)\s*\(.*)?$', pieces[0].strip(), re.S)
    if not m: return [text]
    base = m.group(1).strip()
    return [pieces[0].strip() + ';'] + [f'{base} {q.strip()};' for q in pieces[1:]]


def alias_macro(name, text):
    """#define NAME (*(T *)&NAME) (variable) or ((T)NAME) (function) with T from the variant declaration"""
    d = re.sub(r'/\*.*?\*/|//[^\n]*', '', text, flags=re.S).strip().rstrip(';')
    d = re.sub(r'^extern\s+', '', d)
    isfunc = re.search(r'(?<![*(\w])\b' + re.escape(name) + r'\s*\(', d) is not None and not re.search(r'\(\s*\*\s*' + re.escape(name), d)
    ty = re.sub(r'(?<!\w)' + re.escape(name) + r'(?!\w)', '(*)', d, count=1)
    return f'#define {name} ((' + ty + ')' + name + ')' if isfunc else f'#define {name} (*({ty})&{name})'


def header_decls(root, includes, seen=None, out=None):
    """name -> (key, text) for every extern declaration visible through the given #include lines."""
    seen = seen if seen is not None else set(); out = out if out is not None else {}
    for inc in includes:
        m = re.search(r'"([^"]+)"', inc)
        if not m or m[1] in seen: continue
        seen.add(m[1])
        for base in (root / 'include', root / 'include/rr'):
            path = base / m[1]
            if path.exists():
                try: marks, items = parse(path.read_text())
                except Exception: break
                for kind, text, name in items:
                    if kind == 'pp' and '#include' in text:
                        header_decls(root, [text], seen, out)
                    elif kind == 'decl' and not text.lstrip().startswith(('typedef', 'struct', 'union', 'enum', 'static')):
                        for q in split_declarators(text):
                            n_ = _declname(q)
                            if n_: out.setdefault(n_, (key(q), q))
                break
    return out


def merge(paths, markers_out, policy=None, fwd_mode='proto', alias=False, root=None, canon_pick=None):
    """paths in address order; markers_out: list of marker lines for the merged file.
    policy: dict name -> index of the variant to keep when declarations conflict (default 0)."""
    policy = policy or {}
    pp, types, externs, funcs = [], [], [], []
    seen_pp = set(); seen = {}; conflicts = {}; file_ext = {}
    ro = []; seen_types = {}; seen_types_ct = {}; seen_funcs = set(); seen_funcs_ct = {}
    for p in paths:
        t = Path(p).read_text()
        marks, items = parse(t)
        if alias:
            new_items = []
            for kind, text, name in items:
                if kind == 'decl' and not text.lstrip().startswith(('typedef', 'struct', 'union', 'enum')):
                    for q in split_declarators(text):
                        new_items.append([kind, q, _declname(q) if q is not text else name])
                else: new_items.append([kind, text, name])
            items = new_items
        seen_func = False
        for it_ in items:
            if it_[0] == 'func': seen_func = True
            elif it_[0] == 'pp' and ('legacy typing of shared globals' in it_[1] or re.match(r'\s*#\s*undef\b', it_[1])): it_[0] = 'fpp'
            elif it_[0] == 'pp' and seen_func and re.match(r'(?:/\*.*?\*/\s*)*#\s*(?:define|undef)\b', it_[1].strip(), re.S): it_[0] = 'fpp'
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
                k = key(text) if alias else norm(text)
                file_ext.setdefault(p, {})[name] = (k, text)
                bucket = seen.setdefault(name, [])
                if any(b[0] == k for b in bucket): continue
                bucket.append((k, text, p))
                is_extern = bool(re.match(r'\s*(?:/\*.*?\*/\s*)*extern\b', text, re.S)) or bool(
                    re.search(r'\)\s*;\s*$', norm(text) and text)) and not text.lstrip().startswith(('typedef', 'struct', 'union', 'enum'))
                (types if text.lstrip().startswith(('typedef', 'struct', 'union', 'enum')) else externs).append((name, text))
            elif kind == 'fpp':
                funcs.append((None, text, p))
            else:
                funcs.append((name, text, p))
    for name, b in seen.items():
        if len(b) > 1: conflicts[name] = [x[2] for x in b]
    hdr = {}
    if alias and root is not None:
        hdr = header_decls(root, [x for x in pp if x.lstrip().startswith('#include')])
        for n_, (k_, t_) in hdr.items():
            if n_ in seen:
                seen[n_].insert(0, (k_, t_, '<header>'))
                file_ext.setdefault('<header>', {})[n_] = (k_, t_)
    # choose variants for conflicting declarations
    seen_order = {}
    for name_, b_ in seen.items():
        for i_, v_ in enumerate(b_): seen_order[v_[0]] = i_
    def _score(v):
        t = v[1]
        return (('Rr' in t or 'struct' in t or 'union' in t) * 2 + ('[' not in t and '(' not in t), -seen_order.get(v[0], 0))
    canon_pick = canon_pick or {}
    def canon_of(n_):
        if n_ in canon_pick and n_ not in hdr:
            v_ = seen[n_]; return v_[min(canon_pick[n_], len(v_) - 1)][0]
        return hdr[n_][0] if n_ in hdr else max(seen[n_], key=_score)[0]
    def pick(entries):
        out = []; used = set()
        for name, text in entries:
            if alias and name in hdr:
                continue
            if alias and name in conflicts and name is not None:
                if key(text) != canon_of(name): continue
            elif name in conflicts and name is not None:
                cands = [e for e in entries if e[0] == name]
                keep = cands[min(policy.get(name, 0), len(cands) - 1)]
                if keep[1] is not text: continue
            out.append(text)
        return out
    # externs for names defined in this TU: replace by a forward declaration taken from the definition
    defined = {f[0]: f[1] for f in funcs if f[0]}
    fwd = []
    for name, text in list(externs):
        pass
    externs = [(n_, t_) for n_, t_ in externs if n_ not in defined]
    for name, text, _ in funcs:
        if name is None: continue
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
    if alias:
        def score(v):
            t = v[1]
            return (('Rr' in t or 'struct' in t or 'union' in t) * 2 + ('[' not in t and '(' not in t), -seen_order.get(v[0], 0))
        canon = {n: canon_of(n) for n in seen if n}
        by_file = []
        for n_, t_, f_ in funcs:
            if by_file and by_file[-1][0] == f_: by_file[-1][1].append(t_)
            else: by_file.append((f_, [t_]))
        for f_, ts in by_file:
            joined = '\n'.join(ts)
            macros = []
            for n_, (k_, text_) in file_ext.get(f_, {}).items():
                if n_ and k_ != canon.get(n_) and n_ not in defined and re.search(r'(?<![\w$])' + re.escape(n_) + r'(?![\w$])', joined):
                    macros.append((n_, alias_macro(n_, text_)))
            if macros:
                body.append('/* legacy typing of shared globals in this part of the unit */\n' + '\n'.join(m for _, m in macros))
            for t_ in ts: body.append(t_.strip())
            if macros: body.append('\n'.join(f'#undef {n_}' for n_, _ in macros))
    else:
        for _, t, _ in funcs: body.append(t.strip())
    head = '\n'.join(parts)
    return (head + '\n' if head else '') + '\n\n'.join(body) + '\n', conflicts, ro, [f[0] for f in funcs if f[0]]

#!/usr/bin/env python3
"""Write build/legacy/<func>.c: the single-function view of every function inside its translation unit.

Several unit tests were written when each function lived in its own file. The view keeps the
unit's markers, includes, typedefs and externs and drops every other function definition.
Run via `make views` (also a prerequisite of `make test`)."""
import re
import sys
from pathlib import Path
from c_functions import source_owners
from tumerge import parse

ROOT = Path(__file__).resolve().parents[2]


IDENT = re.compile(r"\b(?:Rr\w+|D_[0-9A-Fa-f]{8}|func_\w+|[A-Z]\w*[a-z]\w*|[A-Z][A-Z0-9_]{3,})\b")


def used_names(text):
    return set(IDENT.findall(re.sub(r"/\*.*?\*/|//[^\n]*", "", text, flags=re.S)))


def header_names(root, include, seen=None, deep=True):
    seen = seen if seen is not None else set()
    m = re.search(r'"([^"]+)"', include)
    if not m or m[1] in seen:
        return set()
    seen.add(m[1])
    for base in (root / "include", root / "include/rr"):
        path = base / m[1]
        if path.exists():
            text = path.read_text()
            names = used_names(text)
            for inc in re.findall(r'#include\s+"[^"]+"', text) if deep else ():
                names |= header_names(root, inc, seen)
            return names
    return set()


def view(root, name, marks, items):
    """Single-function view: markers, the includes/types/externs the function actually mentions."""
    target = next(t for kind, t, n in items if kind == "func" and n == name)
    needed = used_names(target)
    kept = []
    # types/externs the function (transitively) mentions
    defined = {n for k, t, n in items if k == "func"}
    decls = [(k, t, n) for k, t, n in items if k == "decl" and n not in defined]
    changed = True
    chosen = set()
    while changed:
        changed = False
        for i, (k, t, n) in enumerate(decls):
            if i in chosen:
                continue
            words = set(re.findall(r"\w+", re.sub(r"/\*.*?\*/", "", t, flags=re.S)))
            hay = target + "".join(decls[j][1] for j in chosen)
            syms = re.findall(r"\b(?:D_[0-9A-Fa-f]{8}|func_\w+)\b", t)
            if (n and re.search(r"\b" + re.escape(n.split()[-1]) + r"\b", hay)) or any(re.search(r"\b" + y + r"\b", hay) for y in syms):
                chosen.add(i); needed |= used_names(t); changed = True
    out = [m for m in marks if not m.startswith("/* RODATA")]
    incs = [t for k, t, n in items if k == "pp" and "#include" in t and "types.h" not in t]
    direct = [(t, header_names(root, t, deep=False)) for t in incs]
    keep = [t for t, names in direct if names & needed]
    covered = set().union(*[names for t, names in direct if t in keep]) if keep else set()
    for t, names in direct:                      # names only reachable through a nested include
        if t not in keep and header_names(root, t) & (needed - covered):
            keep.append(t)
    for kind, text, n in items:
        if kind == "pp":
            if "#include" not in text or text in keep or ("types.h" in text and not keep):
                out.append(text.strip())
    out += [decls[i][1].strip() for i in sorted(chosen)]
    # same-unit helpers (static functions) are kept: the function may call them
    out += [t.strip() for k, t, n in items if k == "func" and n != name and not n.startswith("func_") and re.search(r"\b" + re.escape(n) + r"\b", target)]
    out.append(target.strip())
    return "\n\n".join(out) + "\n"


def views(root=ROOT):
    out = root / "build/legacy"
    out.mkdir(parents=True, exist_ok=True)
    count = 0
    for name, path in source_owners(root / "src/main/PAL").items():
        marks, items = parse(path.read_text())
        (out / f"{name}.c").write_text(view(root, name, marks, items))
        count += 1
    return count


if __name__ == "__main__":
    print(views(), "single-function views in build/legacy")

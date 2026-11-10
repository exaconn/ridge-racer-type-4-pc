#!/usr/bin/env python3
"""unify_globals.py: a scalar global that units declare with several spellings (`int`, `s32`, `u32`; `short`, `s16`)
gets ONE declaration in include/game/data/globals.h when a single spelling keeps every unit that declares it at
relocated Levenshtein distance 0 (checked with fd.py). The units drop their local line and include the header.

usage: unify_globals.py [--dry-run] [--jobs N]"""
import re, sys
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
import unit_headers as U

DECL = re.compile(r'^extern ((?:unsigned |signed )?(?:char|short|int|long)|u8|s8|u16|s16|u32|s32)\s+([A-Za-z_]\w*);[ \t]*\n', re.M)
HEADER = U.INC / 'game/data/globals.h'


def main():
    dry = '--dry-run' in sys.argv
    jobs = int(sys.argv[sys.argv.index('--jobs') + 1]) if '--jobs' in sys.argv else 8
    users = defaultdict(dict)                        # name -> {unit: type}
    for p in sorted(U.SRC.rglob('*.c')):
        for m in DECL.finditer(p.read_text()):
            users[m.group(2)][p] = m.group(1)
    in_headers = '\n'.join(h.read_text() for h in U.INC.rglob('*.h'))
    multi = {n: u for n, u in users.items() if len(u) >= 2 and not re.search(r'\b' + n + r'\b', in_headers)}
    print('scalar globals declared in 2+ units', len(multi))

    def trial(job):
        n, p, cand = job
        text = p.read_text()
        new = re.sub(r'^extern [^\n]*\b' + n + r';[ \t]*\n', f'extern {cand} {n};\n', text, count=1, flags=re.M)
        p.write_text(new)
        ok = U.fd_ok(p)
        p.write_text(text)
        return (n, p, cand), ok
    jobs_l = []
    for n, u in multi.items():
        for cand in sorted(set(u.values())):
            for p in u:
                jobs_l.append((n, p, cand))
    # one unit-file is touched by one trial at a time: group by file
    by_file = defaultdict(list)
    for j in jobs_l:
        by_file[j[1]].append(j)

    def run_file(js):
        return [trial(j) for j in js]
    res = {}
    with ThreadPoolExecutor(jobs) as ex:
        for out in ex.map(run_file, by_file.values()):
            for k, ok in out:
                res[k] = ok
    chosen = {}
    for n, u in multi.items():
        for cand in sorted(set(u.values()), key=lambda c: -list(u.values()).count(c)):
            if all(res[(n, p, cand)] for p in u):
                chosen[n] = cand
                break
    print('globals with one spelling accepted by every unit', len(chosen), 'of', len(multi))
    if dry:
        for n in sorted(multi):
            print(n, chosen.get(n, '-'), sorted(set(multi[n].values())))
        return
    lines = [f'extern {chosen[n]} {n};' for n in sorted(chosen)]
    if not lines:
        return
    HEADER.parent.mkdir(parents=True, exist_ok=True)
    if HEADER.exists():
        htext = HEADER.read_text()
        idx = htext.rfind('#endif')
        HEADER.write_text(htext[:idx].rstrip() + '\n' + '\n'.join(lines) + '\n\n' + htext[idx:])
    else:
        HEADER.write_text('#ifndef RR_GAME_DATA_GLOBALS_H\n#define RR_GAME_DATA_GLOBALS_H\n\n/* Scalar globals shared by several units: one declaration each. */\n#include "types.h"\n\n'
                          + '\n'.join(lines) + '\n\n#endif\n')
    touched = []
    for n in chosen:
        for p in multi[n]:
            t = p.read_text()
            t = re.sub(r'^extern [^\n]*\b' + n + r';[ \t]*\n', '', t, count=1, flags=re.M)
            p.write_text(t)
            if p not in touched:
                touched.append(p)
    for p in touched:
        t = p.read_text()
        inc = '#include "game/data/globals.h"\n'
        if inc not in t:
            last = None
            for m in re.finditer(r'^#include[^\n]*\n', t, re.M):
                last = m
            if last:
                t = t[:last.end()] + inc + t[last.end():]
            else:
                m = re.match(r'(?:/\*[^\n]*\*/\n)*', t)
                t = t[:m.end()] + inc + t[m.end():]
            p.write_text(t)
    bad = [p for p in touched if not U.fd_ok(p)]
    print('units changed', len(touched), 'failing', len(bad))
    for p in bad:
        print('  FAIL', p)


if __name__ == '__main__':
    main()

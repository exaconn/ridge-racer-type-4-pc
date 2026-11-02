#!/usr/bin/env python3
"""Bulk pipeline: m2c every function in build/baseline, compile with cc.sh, accept those whose text matches the
original bytes (relocations masked). Accepted sources land in src/main/PAL/<func>.c; the full build still has to pass `make check`.
usage: auto_decomp.py [--only func_X ...] [--out DIR] [--jobs N]"""
import argparse, re, struct, subprocess, sys, tempfile
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path
from c_functions import source_owners
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[2]
EXE = (ROOT / "assets/PAL/main.exe").read_bytes()
BASE = 0x80010000

def mask(w, t):
    if t == 4: return w & 0xFC000000
    if t in (5, 6, 7): return w & 0xFFFF0000
    if t == 2: return 0
    return w

def m2c(name):
    asm = ROOT / f"build/baseline/asm/nonmatchings/{name}/{name}.s"
    r = subprocess.run([str(ROOT / ".venv/bin/python"), str(ROOT / "tools/m2c/m2c.py"), "--target", "mipsel-gcc-c",
                        "--no-andor", "--valid-syntax", str(asm)], capture_output=True, text=True, errors="replace")
    return r.stdout if r.returncode == 0 else None

def to_c(src, name):
    out = ["#include \"types.h\"", "#include \"m2c_macros.h\"", ""]
    body = []
    for l in src.splitlines():
        m = re.match(r"^\?? ?(\w[\w\s\*\?]*?)\b(\w+)\(([^)]*)\);\s*/\* extern \*/$", l)
        if re.search(r"/\* extern \*/$", l) and "(" in l:
            fn = re.search(r"(\w+)\(", l)[1]
            out.append(f"extern int {fn}();")
            continue
        if l.startswith("extern "):
            l = re.sub(r"\?", "int", l)
        body.append(l)
    return "\n".join(out + body).replace("?", "int") + "\n"

def compile_cmp(args):
    name, text = args
    with tempfile.TemporaryDirectory() as td:
        c = Path(td) / f"{name}.c"; o = Path(td) / f"{name}.o"
        c.write_text(text)
        r = subprocess.run([str(ROOT / "tools/scripts/cc.sh"), str(c), str(o)], capture_output=True, text=True, errors="replace")
        if r.returncode != 0:
            return name, "compile", None
        ef = ELFFile(o.open("rb"))
        for sec in ef.iter_sections():
            if sec.name in (".rodata", ".data", ".sdata", ".bss", ".sbss", ".rdata") and sec["sh_size"]:
                return name, "has-" + sec.name, None
        text_sec = ef.get_section_by_name(".text"); data = text_sec.data()
        rel = {}
        rs = ef.get_section_by_name(".rel.text")
        if rs:
            for rr in rs.iter_relocations(): rel[rr["r_offset"]] = rr["r_info_type"]
        addr = int(name[5:], 16)
        fsize = next((int(l.split()[2], 16) for l in (ROOT / "configs/PAL/functions.txt").read_text().splitlines() if l.startswith(name + " ")), None)
        if len(data) != fsize:
            return name, f"size {len(data):#x}!={fsize:#x}", None
        for i in range(0, len(data), 4):
            a = struct.unpack_from("<I", data, i)[0]
            b = struct.unpack_from("<I", EXE, 0x800 + addr - BASE + i)[0]
            t = rel.get(i)
            if mask(a, t) != mask(b, t):
                return name, "mismatch", None
        return name, "MATCH", text

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--only", nargs="*"); ap.add_argument("--out", default=str(ROOT / "src/main/PAL"))
    ap.add_argument("--jobs", type=int, default=16); ap.add_argument("--dry", action="store_true"); ap.add_argument("--limit", type=int, default=0)
    a = ap.parse_args()
    names = [l.split()[0] for l in (ROOT / "configs/PAL/functions.txt").read_text().splitlines()]
    done = set(source_owners(a.out))
    todo = [n for n in (a.only or names) if n not in done and (ROOT / f"build/baseline/asm/nonmatchings/{n}/{n}.s").exists()]
    if a.limit: todo = todo[:a.limit]
    with ProcessPoolExecutor(a.jobs) as ex:
        srcs = list(ex.map(m2c, todo, chunksize=8))
        jobs = [(n, to_c(s, n)) for n, s in zip(todo, srcs) if s]
        res = list(ex.map(compile_cmp, jobs, chunksize=8))
    stat = {}
    for n, st, text in res:
        stat[st.split(" ")[0]] = stat.get(st.split(" ")[0], 0) + 1
        if st == "MATCH" and not a.dry:
            (Path(a.out) / f"{n}.c").write_text(text)
    print(f"tried {len(todo)}; m2c ok {len(jobs)}; ", stat)
if __name__ == "__main__":
    main()

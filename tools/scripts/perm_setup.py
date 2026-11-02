#!/usr/bin/env python3
"""Prepare a decomp-permuter job for a near-miss function.
usage: perm_setup.py func_XXXXXXXX [source.c]   -> build/perm/<func>/ ; then run
  .venv/bin/python tools/decomp-permuter/permuter.py build/perm/<func> -j8 --best-only"""
import shutil, subprocess, sys
from pathlib import Path
from c_functions import source_owners
ROOT = Path(__file__).resolve().parents[2]
fn = sys.argv[1]
if len(sys.argv) > 2:
    src = Path(sys.argv[2])
else:
    src = source_owners(ROOT / "src/main/PAL").get(fn)
    if src is None:
        src = ROOT / f"src/failed/PAL/{fn}.c"
    if not src.exists():
        sys.exit(f"No source found for {fn}; pass the source path explicitly")
d = ROOT / "build/perm" / fn
shutil.rmtree(d, ignore_errors=True); d.mkdir(parents=True)
text = src.read_text()
(d / "base.c").write_text(text)
asm = (ROOT / f"build/baseline/asm/nonmatchings/{fn}/{fn}.s").read_text()
(d / "target.s").write_text('.include "macro.inc"\n' + asm)
subprocess.run([str(ROOT / "tools/toolchain/bin/mipsel-none-elf-as"), "-EL", "-G0", "-march=r3000", "-no-pad-sections", f"-I{ROOT/'include'}",
                "-o", str(d / "target.o"), str(d / "target.s")], check=True)
(d / "compile.sh").write_text(f'#!/usr/bin/env bash\nset -euo pipefail\nexec "{ROOT}/tools/scripts/cc.sh" "$1" "$3" >/dev/null 2>&1\n')
(d / "compile.sh").chmod(0o755)
(d / "settings.toml").write_text(f'compiler_type = "gcc"\nfunc_name = "{fn}"\n')
print(d)

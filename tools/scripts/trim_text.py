#!/usr/bin/env python3
"""Drop trailing padding nops after the last `jr $ra` + delay slot in an object's .text (gas 2.7 reorder mode appends one).
usage: trim_text.py obj.o  (uses tools/toolchain/bin/mipsel-none-elf-objcopy)"""
import struct, subprocess, sys, tempfile
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT = Path(__file__).resolve().parents[2]
o = Path(sys.argv[1])
data = ELFFile(o.open("rb")).get_section_by_name(".text").data()
w = struct.unpack("<%dI" % (len(data) // 4), data)
last = max(i for i, x in enumerate(w) if x == 0x03e00008)
end = (last + 2) * 4
if end < len(data) and all(x == 0 for x in w[last + 2:]):
    with tempfile.NamedTemporaryFile(delete=False) as t:
        t.write(data[:end]); name = t.name
    subprocess.run([str(ROOT / "tools/toolchain/bin/mipsel-none-elf-objcopy"), "--update-section", f".text={name}", str(o)], check=True)
    Path(name).unlink()

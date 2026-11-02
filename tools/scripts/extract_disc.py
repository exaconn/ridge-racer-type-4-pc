#!/usr/bin/env python3
"""Extract all ISO9660 files from a MODE2/2352 PS1 .bin (Form1 + Form2 sectors) without external tools."""
import argparse, hashlib, struct, sys
from pathlib import Path

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("bin", type=Path)
    ap.add_argument("out", type=Path)
    ap.add_argument("--list", action="store_true")
    a = ap.parse_args()
    f = a.bin.open("rb")

    def sector(lba):
        f.seek(lba * 2352)
        s = f.read(2352)
        return s

    def read_file(lba, size):
        out = bytearray()
        while len(out) < size:
            s = sector(lba)
            sub = s[18]  # submode byte of subheader
            if sub & 0x20:  # Form2
                out += s[24:24 + 2324]
            else:
                out += s[24:24 + 2048]
            lba += 1
        return bytes(out[:size])

    pvd = sector(16)[24:24 + 2048]
    assert pvd[1:6] == b"CD001", "no ISO9660 PVD"
    root = pvd[156:190]
    rlba, rsize = struct.unpack("<I", root[2:6])[0], struct.unpack("<I", root[10:14])[0]

    def walk(lba, size, path):
        data = read_file(lba, size)
        i = 0
        while i < len(data):
            n = data[i]
            if n == 0:
                i = (i // 2048 + 1) * 2048
                continue
            rec = data[i:i + n]
            i += n
            elba = struct.unpack("<I", rec[2:6])[0]
            esize = struct.unpack("<I", rec[10:14])[0]
            flags = rec[25]
            name = rec[33:33 + rec[32]].decode("ascii", "replace")
            if name in ("\x00", "\x01"):
                continue
            yield path + [name], elba, esize, flags, (rec[18:25], rec)
            if flags & 2:
                yield from walk(elba, esize, path + [name])

    for p, lba, size, flags, _ in walk(rlba, rsize, []):
        full = "/".join(p)
        if flags & 2:
            continue
        print(f"{lba:7d} {size:10d} {full}")
        if not a.list and not full.startswith("R4.STR"):
            dest = a.out / full.split(";")[0]
            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_bytes(read_file(lba, size) if size else b"")

main()

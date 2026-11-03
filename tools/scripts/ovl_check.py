#!/usr/bin/env python3
"""Check ONE overlay function: compile its source, link at the original VA with
every external resolved, and report relocated instruction Levenshtein distance.

Usage:
  ovl_check.py <overlay> <hex_addr> <hex_size> [--rodata <hex_addr> <hex_size>] [--diff]

Source/symbol come from overlay_units.function_owner (default
src/overlays/PAL/<overlay>/func_<ADDR>.c, symbol ovl<overlay>_func_<ADDR>).
Uses the same link_unit / section validation as verify_overlay_samples.py, but
does not need the function registered in EXTRA_FUNCTIONS.
"""
import difflib
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from elftools.elf.elffile import ELFFile  # noqa: E402
from levenshtein import levenshtein  # noqa: E402
from overlay_units import function_owner, link_unit  # noqa: E402
import verify_overlay_samples as vos  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]
OBJDUMP = ROOT / 'tools/toolchain/bin/mips-linux-gnu-objdump'


def disasm(data, address):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(data)
        f.flush()
        out = subprocess.run([str(OBJDUMP), '-D', '-b', 'binary', '-m', 'mips:3000', '-EL',
                              f'--adjust-vma=0x{address:X}', '-M', 'no-aliases', f.name],
                             capture_output=True, text=True).stdout
    lines = []
    for line in out.splitlines():
        parts = line.split('\t')
        if len(parts) >= 3 and parts[0].strip().endswith(':'):
            lines.append(' '.join(p.strip() for p in parts[2:]))
    return lines


def main():
    args = sys.argv[1:]
    show_diff = '--diff' in args
    args = [a for a in args if a != '--diff']
    rodata = None
    if '--rodata' in args:
        i = args.index('--rodata')
        rodata = (int(args[i + 1], 16), int(args[i + 2], 16))
        del args[i:i + 3]
    if len(args) != 3:
        print(__doc__)
        sys.exit(2)
    index, address, size = int(args[0]), int(args[1], 16), int(args[2], 16)
    source, name = function_owner(index, address, ROOT)
    output = ROOT / f'build/ovlchk/{index}_{address:08X}'
    output.mkdir(parents=True, exist_ok=True)
    spans = dict(vos.RODATA_SPANS)
    if rodata:
        spans[(index, address)] = rodata
    try:
        elf, span = link_unit(index, source, [(index, address, size)], spans, output,
                              vos.validate_object_sections, ROOT)
    except subprocess.CalledProcessError as e:
        print('BUILD ERROR:\n' + (e.stderr or b'').decode(errors='replace')[-3000:])
        sys.exit(2)
    except ValueError as e:
        print(f'BUILD ERROR: {e}')
        sys.exit(2)
    blob = (ROOT / 'disc/PAL/R4.BIN').read_bytes()
    sectors = struct.unpack_from('<951I', blob, 4)
    module = blob[sectors[index] * 2048:sectors[index + 1] * 2048]
    with elf.open('rb') as stream:
        parsed = ELFFile(stream)
        text = parsed.get_section_by_name('.text')
        sym = parsed.get_section_by_name('.symtab').get_symbol_by_name(name)
        if not sym:
            print(f'BUILD ERROR: symbol {name} not defined in {source}')
            sys.exit(2)
        sym = sym[0]
        off = sym['st_value'] - text['sh_addr']
        code = text.data()[off:off + sym['st_size']]
        rodata_ok = None
        if span:
            try:
                vos.read_linked_rodata(parsed, span, module)
                rodata_ok = True
            except ValueError as e:
                rodata_ok = f'MISMATCH: {e}'
    original = module[address - 0x80114878:address - 0x80114878 + size]
    words = lambda d: list(struct.unpack(f'<{len(d) // 4}I', d))
    dist = levenshtein(words(original), words(code))
    print(f'{index}: {name}: relocated Levenshtein = {dist} '
          f'(original 0x{size:X} bytes, compiled 0x{len(code):X} bytes)'
          + (f' rodata={rodata_ok}' if span else ''))
    if show_diff and dist:
        for line in difflib.unified_diff(disasm(original, address), disasm(code, address),
                                         'ORIGINAL', 'MINE', lineterm='', n=2):
            print(line)
    sys.exit(0 if dist == 0 and rodata_ok in (None, True) else 1)


if __name__ == '__main__':
    main()

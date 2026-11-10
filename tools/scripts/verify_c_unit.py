#!/usr/bin/env python3
"""Verify a resident C TU after linking every MIPS relocation at PAL addresses.

This is a per-unit check; `make check` must still confirm the complete EXE.
The current resident ELF supplies addresses for symbols not in the registry.
"""
import argparse
import json
from pathlib import Path
import re
import struct
import subprocess

from elftools.elf.elffile import ELFFile

from c_functions import function_definitions
from levenshtein import levenshtein
from overlay_units import symbol_addresses

ROOT = Path(__file__).resolve().parents[2]


def run(*args):
    subprocess.run([str(a) for a in args], cwd=ROOT, check=True, capture_output=True)


def verify(source, output):
    source = Path(source).resolve()
    output = Path(output).resolve()
    output.mkdir(parents=True, exist_ok=True)
    spans = {n: (int(a, 16), int(size, 16)) for n, a, size in
             map(str.split, (ROOT/'configs/PAL/functions.txt').read_text().splitlines())}
    names = function_definitions(source.read_text())
    if not names:
        raise ValueError(f'No known resident functions in {source}')
    for left, right in zip(names, names[1:]):
        a, size = spans[left]
        if a + size != spans[right][0]:
            raise ValueError(f'Noncontiguous or unordered functions: {left}, {right}')
    addresses = {}
    with (ROOT/'build/PAL/main.elf').open('rb') as stream:
        elf = ELFFile(stream)
        for symbol in elf.get_section_by_name('.symtab').iter_symbols():
            if symbol.name and symbol['st_shndx'] != 'SHN_UNDEF':
                addresses[symbol.name] = symbol['st_value']
    addresses.update(symbol_addresses())
    obj, linked, script = (output / (source.stem + suffix) for suffix in ('.o', '.elf', '.ld'))
    run(ROOT/'tools/scripts/cc.sh', source, obj)
    definitions = []
    with obj.open('rb') as stream:
        elf = ELFFile(stream)
        for section in elf.iter_sections():
            if section.name in ('.data', '.sdata', '.bss', '.sbss', '.rdata') and section['sh_size']:
                raise ValueError(f'Unsupported data section: {section.name}')
        for symbol in elf.get_section_by_name('.symtab').iter_symbols():
            if symbol.name and symbol['st_shndx'] == 'SHN_UNDEF':
                if symbol.name in addresses:
                    address = addresses[symbol.name]
                elif re.fullmatch(r'(?:D|func|jtbl)_[0-9A-Fa-f]{8}', symbol.name):
                    address = int(symbol.name[-8:], 16)
                else:
                    raise ValueError(f'Unknown address: {symbol.name}')
                definitions.append(f'{symbol.name} = 0x{address:X};')
        rodata = elf.get_section_by_name('.rodata')
        rosize = rodata['sh_size'] if rodata else 0
    tables = [(int(a, 16), int(size, 16)) for a, size in
              re.findall(r'RODATA\s+(0x[0-9A-Fa-f]+)\s+(0x[0-9A-Fa-f]+)', source.read_text())]
    tables.sort()
    for (a, size), (b, _) in zip(tables, tables[1:]):
        if a + size != b:
            raise ValueError('Separate rodata spans need an explicit layout')
    if rosize and (not tables or sum(size for _, size in tables) != rosize):
        raise ValueError('Missing or incorrect RODATA annotations')
    sections = [f'.text 0x{spans[names[0]][0]:X} : SUBALIGN(4) {{ *(.text) }}']
    if rosize:
        sections.append(f'.rodata 0x{tables[0][0]:X} : SUBALIGN(4) {{ *(.rodata) }}')
    sections.append('/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }')
    script.write_text('\n'.join(definitions) + '\nSECTIONS {\n' + '\n'.join(sections) + '\n}\n')
    run(ROOT/'tools/toolchain/bin/mipsel-none-elf-ld', '-EL', '-T', script, '-o', linked, obj)
    original = (ROOT/'assets/PAL/main.exe').read_bytes()
    base, = struct.unpack_from('<I', original, 0x18)
    def original_at(address, size):
        start = 0x800 + address - base
        data = original[start:start + size]
        if len(data) != size:
            raise ValueError(f'Original span outside EXE: {address:#x}')
        return data
    def words(data):
        return struct.unpack(f'<{len(data)//4}I', data)
    rows = []
    with linked.open('rb') as stream:
        elf = ELFFile(stream)
        symbols = elf.get_section_by_name('.symtab')
        expected_end = spans[names[-1]][0] + spans[names[-1]][1]
        text_extent_correct = elf.get_section_by_name('.text')['sh_size'] == expected_end - spans[names[0]][0]
        for name in names:
            address, size = spans[name]
            symbol = symbols.get_symbol_by_name(name)[0]
            section = elf.get_section(symbol['st_shndx'])
            start = symbol['st_value'] - section['sh_addr']
            data = section.data()[start:start + symbol['st_size']]
            rows.append(dict(symbol=name, address_correct=symbol['st_value'] == address,
                             levenshtein=levenshtein(words(original_at(address, size)), words(data))))
        rodata = elf.get_section_by_name('.rodata')
        rodata_identical = not rosize or rodata.data() == original_at(tables[0][0], rosize)
    report = dict(source=str(source), functions=rows, rodata_identical=rodata_identical,
                  text_extent_correct=text_extent_correct)
    (output/(source.stem+'.json')).write_text(json.dumps(report, indent=2)+'\n')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('source', type=Path)
    parser.add_argument('--output', type=Path, default=ROOT/'build/unit_checks')
    args = parser.parse_args()
    try:
        report = verify(args.source, args.output)
    except subprocess.CalledProcessError as error:
        raise SystemExit(error.stderr.decode(errors='replace'))
    for row in report['functions']:
        print(f"{row['symbol']}: relocated Levenshtein {row['levenshtein']}; address correct: {row['address_correct']}")
    print(f"Read-only data identical: {report['rodata_identical']}")
    print(f"Complete text extent correct: {report['text_extent_correct']}")
    if not report['text_extent_correct'] or not report['rodata_identical'] or any(r['levenshtein'] or not r['address_correct'] for r in report['functions']):
        raise SystemExit(1)


if __name__ == '__main__':
    main()

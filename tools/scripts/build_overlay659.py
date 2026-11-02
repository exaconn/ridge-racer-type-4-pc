#!/usr/bin/env python3
"""Rebuild PAL overlay 659/670; report C recovery separately from reference asm."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
from elftools.elf.elffile import ELFFile
from levenshtein import levenshtein

ROOT = Path(__file__).resolve().parents[2]


def run(*args):
    result = subprocess.run([str(a) for a in args], cwd=ROOT, capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(result.stderr or result.stdout)
    return result.stdout


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--require-complete', action='store_true')
    args = parser.parse_args()
    manifest = json.loads((ROOT / 'configs/PAL/overlays/659.json').read_text())
    output = ROOT / 'build/overlays/659-full'
    output.mkdir(parents=True, exist_ok=True)
    tools = ROOT / 'tools/toolchain/bin'
    sections, objects, aliases = [], [], []
    for row in manifest['functions']:
        address = row['address']
        obj = output / f'func_{address:08X}.o'
        if row['source']:
            run(ROOT / 'tools/scripts/cc.sh', ROOT / row['source'], obj)
            aliases.append(f'func_{address:08X} = ovl659_func_{address:08X};')
        else:
            run(tools / 'mipsel-none-elf-as', '-EL', '-G0', '-march=r3000',
                '-no-pad-sections', '-Iinclude', '-o', obj, ROOT / row['reference_asm'])
        objects.append(obj)
        sections.append(f'.f{address:X} 0x{address:X} : SUBALIGN(4) {{ {obj}(.text) }}')
    data = output / 'data.o'
    run(ROOT / 'tools/scripts/cc.sh', ROOT / 'src/overlays/PAL/659/data.c', data)
    objects.append(data)
    defined, undefined = set(), set()
    for obj in objects:
        with obj.open('rb') as stream:
            elf = ELFFile(stream)
            for symbol in elf.get_section_by_name('.symtab').iter_symbols():
                if not symbol.name:
                    continue
                if symbol['st_shndx'] == 'SHN_UNDEF':
                    undefined.add(symbol.name)
                else:
                    defined.add(symbol.name)
    for name in sorted(undefined - defined):
        if any(line.startswith(name + ' =') for line in aliases):
            continue
        match = re.fullmatch(r'(?:D|func|jtbl)_([0-9A-Fa-f]{8})', name)
        if not match:
            raise ValueError(f'Unknown external symbol: {name}')
        aliases.append(f'{name} = 0x{match[1]};')
    script = output / 'overlay.ld'
    script.write_text('\n'.join(aliases) + '\nSECTIONS {\n' +
        f'.header 0x80114878 : SUBALIGN(4) {{ {data}(.ovl659.header) }}\n' +
        '.table0 0x8011487C : SUBALIGN(4) { *(.rodata.80115224) }\n' +
        '.table1 0x801148A0 : SUBALIGN(4) { *(.rodata.80115CE4) }\n' +
        '\n'.join(sections) + '\n' +
        f'.data 0x80116530 : SUBALIGN(4) {{ {data}(.ovl659.data) }}\n' +
        '.padding 0x801165C0 : { BYTE(0); . = 0x2B8; }\n' +
        '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n')
    elf_path, binary = output / 'overlay.elf', output / 'overlay.bin'
    run(tools / 'mipsel-none-elf-ld', '-EL', '-T', script, '-Map', output / 'overlay.map',
        '-o', elf_path, *objects)
    run(tools / 'mipsel-none-elf-objcopy', '-O', 'binary', '--gap-fill=0',
        '--pad-to=0x80116878', elf_path, binary)
    blob = (ROOT / 'disc/PAL/R4.BIN').read_bytes()
    sectors = struct.unpack_from('<951I', blob, 4)
    rebuilt = binary.read_bytes()
    base = manifest['load_address']
    assert len(rebuilt) == manifest['image_size']
    reports = []
    original = blob[sectors[659]*2048:sectors[660]*2048]
    for row in manifest['functions']:
        start = row['address'] - base
        left = original[start:start+row['size']]
        right = rebuilt[start:start+row['size']]
        words = lambda data: list(struct.unpack(f'<{len(data)//4}I', data))
        distance = levenshtein(words(left), words(right))
        reports.append(dict(address=row['address'], source=row['source'], levenshtein=distance))
        print(f"0x{row['address']:08X}: {'C' if row['source'] else 'reference asm'}; Levenshtein {distance}")
    sites = [manifest['entry']] + manifest['aliases']
    identity = {str(site): rebuilt == blob[sectors[site]*2048:sectors[site+1]*2048] for site in sites}
    report = dict(c_functions=sum(bool(row['source']) for row in reports), total_functions=len(reports),
                  c_code_bytes=sum(row['size'] for row in manifest['functions'] if row['source']),
                  text_bytes=sum(row['size'] for row in manifest['functions']),
                  functions=reports, whole_overlay_identical=identity,
                  rebuilt_sha256=hashlib.sha256(rebuilt).hexdigest(),
                  full_exe_identical=(ROOT/'assets/PAL/main.exe').read_bytes() == (ROOT/'build/PAL/main.exe').read_bytes())
    (output / 'verified.json').write_text(json.dumps(report, indent=2)+'\n')
    assert all(identity.values()) and all(row['levenshtein'] == 0 for row in reports)
    assert report['full_exe_identical']
    print(f"Verified whole images 659/670; C recovery {report['c_functions']}/{report['total_functions']} functions.")
    if args.require_complete and report['c_functions'] != report['total_functions']:
        raise SystemExit('C recovery incomplete: reference assembly remains.')


if __name__ == '__main__':
    main()

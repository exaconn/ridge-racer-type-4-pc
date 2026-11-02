#!/usr/bin/env python3
"""Express original three-instruction PlayStation BIOS entries as service-specific BIOS macros.

Run after splat so that regenerating assembly preserves the explicit BIOS ABI.
Only complete named functions with the exact dispatch instruction sequence qualify.
Padding and all other assembly remain unchanged.
"""
from pathlib import Path
import re
import json
import sys

ROOT = Path(__file__).resolve().parents[2]
PATTERN = re.compile(
    r'^glabel (?P<name>func_[0-9A-Fa-f]{8})\n'
    r'\s*(?:/\*[^\n]*?\*/\s*)?addiu\s+\$t2,\s*\$zero,\s*(?P<vector>0x[A-Ca-c]0)\s*\n'
    r'\s*(?:/\*[^\n]*?\*/\s*)?jr\s+\$t2\s*\n'
    r'\s*(?:/\*[^\n]*?\*/\s*)?addiu\s+\$t1,\s*\$zero,\s*(?P<service>0x[0-9A-Fa-f]+)\s*\n'
    r'endlabel (?P=name)\n', re.M)


def services(version='PAL'):
    rows = json.loads((ROOT / f'configs/{version}/bios_services.json').read_text())
    return {(row['vector'], row['service']): row['macro'] for row in rows}


def normalize(source, version='PAL'):
    names = services(version)
    def call(name, vector, service):
        return f"{names[int(vector, 16), int(service, 16)]} {name}\n"
    source, existing = re.subn(
        r'^BIOS_CALL (func_[0-9A-Fa-f]{8}), (0x[0-9A-Fa-f]+), (0x[0-9A-Fa-f]+)\n',
        lambda m: call(*m.groups()), source, flags=re.M)
    source, found = PATTERN.subn(lambda m: call(m['name'], m['vector'], m['service']), source)
    return source, existing + found


def render_macros():
    lines = ['# Named PlayStation BIOS entries. Each macro fixes one concrete service.',
             '# ABI: t2 is the dispatch vector; t1 is the service selector.',
             '# Names: https://psx-spx.consoledev.net/kernelbios/#bios-function-summary',
             '# Generated from configs/PAL/bios_services.json; padding stays in the caller unit.', '']
    for (vector, service), name in services().items():
        lines += [f'.macro {name} symbol', '    glabel \\symbol',
                  f'        addiu $t2, $zero, 0x{vector:X}', '        jr $t2',
                  f'         addiu $t1, $zero, 0x{service:X}',
                  '    endlabel \\symbol', '.endm', '']
    return '\n'.join(lines)


def main():
    version = sys.argv[1] if len(sys.argv) > 1 else 'PAL'
    include = ROOT / 'include/bios_calls.inc'
    macros = render_macros()
    if include.read_text() != macros:
        include.write_text(macros)
    count = 0
    for path in sorted((ROOT / f'asm/{version}/main').glob('*.s')):
        source = path.read_text()
        updated, found = normalize(source, version)
        if found:
            path.write_text(updated)
            count += found
    print(f'BIOS entries expressed with named BIOS macros: {count}')


if __name__ == '__main__':
    main()

"""Execute binary32/binary64 literal encodings emitted by a PSX compiler.

The expected values come from independent IEEE encodings, including both zeros,
subnormals, exponent boundaries and mantissas with nonzero low words.
"""
import argparse
from pathlib import Path
import random
import struct
import subprocess
import sys
import tempfile

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32
from mips_preview_engine import Mips

ROOT = Path(__file__).resolve().parents[2]


def cases():
    result = []
    formats = [(64, [0, 1, 2, 511, 970, 1022, 1023, 1024, 1035, 1039, 1047, 1054, 1075, 2046], 52, 'd'),
               (32, [0, 1, 2, 63, 126, 127, 128, 139, 143, 151, 254], 23, 'f')]
    banks = {}
    rng = random.Random(0x272281)
    for width, exponents, fraction_bits, fmt in formats:
        values = {0, 1 << (width - 1)}
        for exponent in exponents:
            for fraction in [0, 1, (1 << (fraction_bits - 1)) - 1, (1 << fraction_bits) - 1]:
                for sign in [0, 1 << (width - 1)]:
                    values.add(sign | exponent << fraction_bits | fraction)
        banks[width] = values
    for _ in range(128):
        for width, _, fraction_bits, _ in formats:
            banks[width].add(rng.getrandbits(1) << (width - 1) |
                             rng.randrange((1 << (width - fraction_bits - 1)) - 1) << fraction_bits |
                             rng.getrandbits(fraction_bits))
    for width, _, fraction_bits, fmt in formats:
        for index, bits in enumerate(sorted(banks[width])):
            value, = struct.unpack('<' + fmt, bits.to_bytes(width // 8, 'little'))
            literal = format(value, '.17g')
            if '.' not in literal and 'e' not in literal:
                literal += '.0'
            if width == 32:
                literal += 'F'
            result.append(dict(name=f'real{width}_{index}', width=width, expected=bits, literal=literal))
    return result


def assemble_and_link(assembly, directory, gnu_profile=True):
    directory = Path(directory)
    directory.mkdir(parents=True, exist_ok=True)
    obj, elf, script = [directory / ('literals' + suffix) for suffix in ['.o', '.elf', '.ld']]
    command = [
        sys.executable, str(ROOT / 'tools/maspsx/maspsx.py'),
        '--aspsx-version=2.79', '--expand-div',
        '--gnu-as-path=' + str(ROOT / 'tools/toolchain/bin/mipsel-none-elf-as'),
        '--run-assembler', '-EL', '-G0', '-march=r3000', '-mtune=r3000', '-no-pad-sections', '-o', str(obj),
    ]
    if gnu_profile:
        command.insert(4, '--assembler-profile=gnu')
    result = subprocess.run(command, input=assembly, capture_output=True, text=True)
    if result.returncode:
        raise AssertionError(result.stderr)
    script.write_text('''SECTIONS {
 .text 0x80010000 : { *(.text) }
 .rodata 0x80100000 : { *(.rodata) *(.rdata) }
 .data 0x80200000 : { *(.data) *(.sdata) }
 /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }
}
''')
    result = subprocess.run([str(ROOT / 'tools/toolchain/bin/mipsel-none-elf-ld'), '-EL', '-T', str(script),
                             '-o', str(elf), str(obj)], capture_output=True, text=True)
    if result.returncode:
        raise AssertionError(result.stderr)
    return elf


def memory_and_symbols(elf_path):
    memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32)
    memory.mem_map(0, 0x400000)
    with Path(elf_path).open('rb') as stream:
        elf = ELFFile(stream)
        for segment in elf.iter_segments():
            if segment['p_type'] == 'PT_LOAD':
                memory.mem_write(segment['p_vaddr'] & 0x1FFFFFFF, segment.data())
        symbols = {s.name: s['st_value'] for s in elf.get_section_by_name('.symtab').iter_symbols() if s.name}
    return memory, symbols


def execute(memory, address):
    stack, stop = 0x803F0000, 0x803FF000
    def store(cpu, access, at, size, value, user):
        if not stack - 128 <= at < at + size <= stack + 16:
            raise AssertionError(('literal store outside stack', hex(cpu.pc), hex(at)))
    cpu = Mips(memory, 0, 100, 101, 102, store)
    cpu.r = [0] + [0xBEEF0000 + i for i in range(1, 32)]
    cpu.r[29], cpu.r[31], cpu.pc = stack, stop, address
    for _ in range(100):
        if cpu.pc == stop:
            return cpu
        cpu.step()
    raise AssertionError('Literal function did not return')


def verify(compiler, version, directory):
    directory = Path(directory)
    directory.mkdir(parents=True, exist_ok=True)
    source, assembly = directory / 'literals.c', directory / 'literals.s'
    corpus = cases()
    lines = ['typedef unsigned int U32; typedef unsigned long long U64;']
    for case in corpus:
        width = case['width']
        ctype = 'double' if width == 64 else 'float'
        lines.append(f'U{width} {case["name"]}(void) {{ union {{{ctype} number;U{width} bits;}} v;'
                     f'v.number={case["literal"]};return v.bits;}}')
    for width, ctype in [(64, 'double'), (32, 'float')]:
        lines.append(f'const {ctype} values{width}[]={{' +
                     ','.join(c['literal'] for c in corpus if c['width'] == width) + '};')
    source.write_text('\n'.join(lines) + '\n')
    flags = ['-quiet', '-w', '-O2', '-G0', '-funsigned-char', '-mips1', '-mcpu=3000']
    if version == '970318':
        flags += ['-mhard-float', '-fno-exceptions']
    result = subprocess.run([str(compiler), *flags, str(source), '-o', str(assembly)], capture_output=True, text=True)
    if result.returncode:
        raise AssertionError(result.stderr)
    memory, symbols = memory_and_symbols(assemble_and_link(assembly.read_text(), directory))
    for width in [32, 64]:
        expected = b''.join(c['expected'].to_bytes(width // 8, 'little') for c in corpus if c['width'] == width)
        actual = bytes(memory.mem_read(symbols[f'values{width}'] & 0x1FFFFFFF, len(expected)))
        if actual != expected:
            raise AssertionError((version, 'incorrect static literal encoding', width))
    for case in corpus:
        cpu = execute(memory, symbols[case['name']])
        actual = cpu.r[2] | cpu.r[3] << 32 if case['width'] == 64 else cpu.r[2]
        if actual != case['expected']:
            raise AssertionError((version, case['name'], case['literal'], hex(actual), hex(case['expected'])))
        saved = [*range(16, 24), 28, 30]
        if cpu.r[29] != 0x803F0000 or any(cpu.r[i] != 0xBEEF0000 + i for i in saved):
            raise AssertionError((version, 'literal ABI violation', case['name']))
    return len(corpus)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path, required=True)
    parser.add_argument('--version', choices=['2.7.2', '2.8.1', '970318'], required=True)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory() as directory:
        count = verify(args.compiler.resolve(), args.version, directory)
    print(f'{args.version}: {count} correct executable and static literal encodings')


if __name__ == '__main__':
    main()

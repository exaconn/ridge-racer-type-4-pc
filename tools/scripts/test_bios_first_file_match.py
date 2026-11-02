"""Verify relocated MIPS instructions and the complete linked PAL executable."""
import json
from pathlib import Path
import struct
import unittest

from elftools.elf.elffile import ELFFile
from levenshtein import levenshtein

ROOT = Path(__file__).resolve().parents[2]


class BiosFirstFileMatchTest(unittest.TestCase):
    def test_relocated_functions_and_complete_exe(self):
        original = (ROOT / 'assets/PAL/main.exe').read_bytes()
        built = (ROOT / 'build/PAL/main.exe').read_bytes()
        inventory = {
            name: (int(address, 16), int(size, 16))
            for name, address, size in map(str.split,
                (ROOT / 'configs/PAL/functions.txt').read_text().splitlines())
        }
        verified = []
        with (ROOT / 'build/PAL/main.elf').open('rb') as stream:
            elf = ELFFile(stream)
            symbols = elf.get_section_by_name('.symtab')
            for name in ('func_80097114', 'func_800972B0', 'func_8007834C'):
                address, size = inventory[name]
                symbol = symbols.get_symbol_by_name(name)[0]
                self.assertEqual(symbol['st_value'], address, name)
                self.assertEqual(symbol['st_size'], size, name)
                section = elf.get_section(symbol['st_shndx'])
                offset = address - section['sh_addr']
                instructions = section.data()[offset:offset+size]
                original_offset = address - 0x80010000 + 0x800
                expected = original[original_offset:original_offset+size]
                distance = levenshtein(struct.unpack('<%dI' % (size//4), expected),
                                       struct.unpack('<%dI' % (size//4), instructions))
                self.assertEqual(distance, 0, name)
                verified.append(dict(symbol=name, levenshtein=distance))
        self.assertEqual(built, original)
        report = ROOT / 'build/smallest10/firstfile_verified.json'
        report.parent.mkdir(parents=True, exist_ok=True)
        report.write_text(json.dumps(dict(functions=verified, full_exe_identical=True),
                                     indent=2) + '\n')


if __name__ == '__main__':
    unittest.main()

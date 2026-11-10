"""Characterize the original leading-space dependency without inventing inputs.

For proportional spacing the original reads incoming s2/s4 before defining
glyph coordinates. This MIPS-only regression makes that unresolved behavior
explicit; it does not claim a native-C oracle for uninitialized coordinates.
"""
import os
from pathlib import Path
import struct
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32

from c_functions import address_of
from test_vehicle_initializer import Cpu

ROOT = Path(__file__).resolve().parents[2]


class TextSpriteSpacingTest(unittest.TestCase):
    def test_leading_space_observes_incoming_atlas_registers(self):
        original = (ROOT/'assets/PAL/main.exe').read_bytes()
        load = struct.unpack_from('<I', original, 24)[0]
        widths_address = address_of('SmallFontGlyphAdvances')
        widths = original[0x800+widths_address-load:0x800+widths_address-load+54]
        other = next(i for i in range(1, 54) if widths[i] != widths[0])
        code = None
        if os.getenv('RR_COMPILED_ELF'):
            with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                elf = ELFFile(stream)
                symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('AddTextSprites')[0]
                section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                self.assertEqual(symbol['st_value'], address_of('AddTextSprites'))
                code = section.data()[offset:offset+symbol['st_size']]
        results = []
        for spacing in (0, 1, 2):
            for incoming in (0, other):
                memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32);memory.mem_map(0, 0x400000)
                write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
                write(load, original[0x800:])
                if code:write(address_of('AddTextSprites'), code)
                write(0x801D0000, b' A\0');stack = 0x803F0000
                write(stack-128, b'\xA5'*256)
                write(stack+16, struct.pack('<4I', 0x801D0000, 0x7FF8, spacing, 0))

                def store(cpu, access, address, length, value, user):
                    self.assertTrue(stack-56 <= address and address+length <= stack+16)

                cpu = Cpu(memory, 0, 100, 101, 102, store)
                cpu.r = [0]+[0xBEEF0000+i for i in range(1, 32)]
                cpu.r[4:8] = [0x801B0000, 0x801C0000, 100, 200]
                cpu.r[18], cpu.r[20] = (incoming//32)*16, (incoming % 32)*8
                cpu.r[29], cpu.r[31], cpu.pc = stack, 0x803FF000, address_of('AddTextSprites')
                for _ in range(500):
                    if cpu.pc == address_of('SetSprt'):break
                    cpu.step()
                self.assertEqual(cpu.pc, address_of('SetSprt'))
                expected = 106 if spacing == 0 else 100+widths[incoming]+(2 if spacing == 1 else 0)
                self.assertEqual(cpu.r[17], expected)
                results.append(cpu.r[17])
        self.assertEqual(results[:2], [106, 106])
        self.assertNotEqual(results[2], results[3]);self.assertNotEqual(results[4], results[5])


if __name__ == '__main__':
    unittest.main()

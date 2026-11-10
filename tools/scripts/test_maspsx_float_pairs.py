"""Execute li.d for numeric and named GPR pairs, checking both words and guards."""
from pathlib import Path
import struct
import tempfile
import unittest
from target_float_literals import assemble_and_link, execute, memory_and_symbols


class MaspsxFloatPairsTest(unittest.TestCase):
    def test_argument_temporary_and_saved_register_pairs(self):
        registers = [(f'${i}', i) for i in range(2, 26, 2)]
        registers += list(zip(['$v0', '$a0', '$a2', '$t0', '$t2', '$t4', '$t6', '$s0', '$s2', '$s4', '$s6', '$t8'],
                             [2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24]))
        encodings = [0, 0x8000000000000000, 0x3FF0000000000000, 0xBFF0000000000000,
                     0x40B0000000000000, 0x40F0000000000000, 0x3F30000000000000,
                     0x41DFFFFFFFC00000, 0xC1E0000000000000, 0x3FF0000000000001,
                     0x400921FB54442D18, 0x7FEFFFFFFFFFFFFF, 0xFFEFFFFFFFFFFFFF,
                     0x0010000000000000, 1, 0x000FFFFFFFFFFFFF]
        lines, cases = ['.text'], []
        for spelling, number in registers:
            for bits in encodings:
                name = f'pair_{len(cases)}'
                value, = struct.unpack('<d', bits.to_bytes(8, 'little'))
                lines += ['.align 2', f'.globl {name}', f'.ent {name}', name + ':',
                          f'li.d {spelling},{value:.17g}', 'jr $31', 'nop', f'.end {name}']
                cases.append((name, number, bits))
        for gnu_profile in [False, True]:
            with tempfile.TemporaryDirectory() as directory:
                memory, symbols = memory_and_symbols(assemble_and_link('\n'.join(lines) + '\n', Path(directory), gnu_profile))
                for name, number, bits in cases:
                    with self.subTest(name=name, pair=number, gnu_profile=gnu_profile):
                        cpu = execute(memory, symbols[name])
                        self.assertEqual(cpu.r[number] | cpu.r[number + 1] << 32, bits)
                        for i in range(32):
                            # The assembler owns $at and may use it for a literal pool.
                            if i in [1, number, number + 1]:
                                continue
                            expected = 0 if i == 0 else 0x803F0000 if i == 29 else 0x803FF000 if i == 31 else 0xBEEF0000 + i
                            self.assertEqual(cpu.r[i], expected)
        self.assertEqual(len(cases), 384)
        print('768 li.d executions checked across default/GNU profiles, numeric/named pairs and register guards')

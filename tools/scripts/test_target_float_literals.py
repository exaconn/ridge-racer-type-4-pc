"""Real compiler/assembler output must preserve executable IEEE literal values."""
from pathlib import Path
import tempfile
import unittest
from target_float_literals import verify

ROOT = Path(__file__).resolve().parents[2]


class TargetFloatLiteralsTest(unittest.TestCase):
    def test_all_three_compilers_static_and_executable_literal_values(self):
        for version in ['2.7.2', '2.8.1', '970318']:
            with self.subTest(version=version), tempfile.TemporaryDirectory() as directory:
                count = verify(ROOT / f'tools/toolchain/gcc-{version}/cc1', version, directory)
                self.assertEqual(count, 456)
                print(f'{version}: {count} executable binary32/binary64 literals and static encodings passed')

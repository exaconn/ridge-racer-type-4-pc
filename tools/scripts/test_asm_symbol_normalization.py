"""Grouped hasm definitions and cross-segment references share one symbol."""
import unittest

from normalize_asm_symbols import normalize


class AsmSymbolNormalizationTest(unittest.TestCase):
    def test_definition_reference_and_interior_offset(self):
        source = 'glabel _memcard_patch_stub\n lui $t0, %hi(_memcard_patch_stub + 0x14)\n jr func_80095e74\n'
        expected = 'glabel _memcard_patch_stub\n lui $t0, %hi(_memcard_patch_stub + 0x14)\n jr _memcard_patch_stub\n'
        self.assertEqual(normalize(source, {0x80095E74: '_memcard_patch_stub'}), expected)

    def test_unknown_aliases_local_names_and_data_are_preserved(self):
        source = 'jal func_80012340\n.Lfunc_80095E74:\nfunc_80095E74_suffix:\nD_80095E74:\n'
        self.assertEqual(normalize(source, {0x80095E74: '_memcard_patch_stub'}), source)

    def test_already_registered_source_is_idempotent(self):
        source = 'glabel _memcard_patch_stub\n jal _memcard_patch_stub\n'
        self.assertEqual(normalize(source, {0x80095E74: '_memcard_patch_stub'}), source)


if __name__ == '__main__':
    unittest.main()

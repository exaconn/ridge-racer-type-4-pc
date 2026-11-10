"""Regression checks for source-audit blind spots, independent of repo totals."""
from pathlib import Path
from tempfile import TemporaryDirectory
import unittest

import crutch_debt as audit


class SourceAuditTest(unittest.TestCase):
    def collect(self, source):
        with TemporaryDirectory(dir=audit.ROOT/'build') as directory:
            path = Path(directory)/'sample.c'
            path.write_text(source)
            return audit.collect(Path(directory))

    def test_numeric_and_named_register_bindings(self):
        counts, affected, files = self.collect('''
void f(void) {
    register int a asm("$2");
    register int b __asm__("v0");
    register int c __asm__("$s0");
}
''')
        self.assertEqual(counts['pins'], 3)
        self.assertEqual(len(affected), 1)
        self.assertEqual(files, 1)

    def test_comments_do_not_add_asm(self):
        counts, _, _ = self.collect('''
/* register int phantom __asm__("$2"); __asm__(""); */
void f(void) {
    // asm("");
    __asm__ volatile("" : : : "memory");
    asm("" : : "r"(1));
}
''')
        self.assertEqual(counts['pins'], 0)
        self.assertEqual(counts['barriers'], 2)

    def test_hardware_and_layout_are_separate_indicators(self):
        counts, affected, _ = self.collect('''
struct S { char pad[12]; };
void f(void) {
    volatile int *port;
    __asm__ volatile("mfc2 %0, $11" : "=r"(port));
}
''')
        self.assertEqual(counts['volatile_qualifiers'], 2)
        self.assertEqual(counts['gte_asm'], 1)
        self.assertEqual(counts['asm_bodies'], 0)
        self.assertEqual(counts['pad_arrays'], 1)
        self.assertEqual(affected, [])

    def test_integer_casts_include_native_address_width(self):
        counts, _, _ = self.collect('''
unsigned long address = (unsigned long)pointer;
u32 bits = (u32)value;
''')
        self.assertEqual(counts['integer_casts'], 2)


if __name__ == '__main__':
    unittest.main()

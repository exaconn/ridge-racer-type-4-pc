import unittest
from pathlib import Path
from tempfile import TemporaryDirectory
from unittest.mock import patch
import c_functions
import os

from c_functions import definition_source, function_definitions, source_owners


class FunctionDefinitionsTest(unittest.TestCase):
    def test_ansi_and_pointer_return(self):
        self.assertEqual(function_definitions('''
int func_80000000(int x) { return x; }
char *func_80000004(void) { return 0; }
'''), ['func_80000000', 'func_80000004'])

    def test_knr_parameters(self):
        self.assertEqual(function_definitions('''
void func_80000000(a, b)
struct P *a; int b;
{ }
int func_80000004(a, b) int a, b; { return a; }
'''), ['func_80000000', 'func_80000004'])

    def test_prototypes_calls_and_comments_are_ignored(self):
        self.assertEqual(function_definitions('''
/* int func_80000000(void) {} */
extern int func_80000004(), func_80000008();
int func_8000000C(int x);
void func_80000010(void) {
    func_80000004();
    char *s = "int func_80000014(void) {}";
}
'''), ['func_80000010'])

    def test_incomplete_decompilation_has_no_definition(self):
        self.assertEqual(function_definitions('''
#include "types.h"
/* Decompilation failure in function func_80000000 */
'''), [])

    def test_module_filename_can_own_multiple_functions(self):
        with TemporaryDirectory() as directory:
            module = Path(directory) / 'tables.c'
            module.write_text('''
int func_80000000(int key) { return key; }
int func_80000004(key) int key; { return key; }
''')
            self.assertEqual(source_owners(directory), {
                'func_80000000': module, 'func_80000004': module})
            (Path(directory) / 'func_80000000.c').write_text(
                'int func_80000000(void) { return 0; }')
            with self.assertRaisesRegex(ValueError, 'defined in both'):
                source_owners(directory)

    def test_extracts_only_the_body_with_knr_parameters_and_literal_braces(self):
        body = '''int func_80000004(value) int value; {
    const char *text = "} {";
    /* } */ if (value) { return text[0]; }
    return '{';
}'''
        source = '''#define Shared (*(int *)&Shared)
void func_80000000(void) { }
''' + body + '\n#undef Shared\nvoid func_80000008(void) { }\n'
        self.assertEqual(definition_source(source, 'func_80000004'), body+'\n')

    def test_extraction_rejects_prototypes_and_unclosed_bodies(self):
        with self.assertRaisesRegex(ValueError, 'Missing definition'):
            definition_source('extern int func_80000000(int);', 'func_80000000')
        with self.assertRaisesRegex(ValueError, 'Unclosed definition'):
            definition_source('int func_80000000(void) { if (1) { }', 'func_80000000')

    def test_symbol_index_refreshes_after_atomic_registry_replacement(self):
        with TemporaryDirectory() as directory:
            root=Path(directory);pal=root/'configs/PAL';pal.mkdir(parents=True)
            listing=pal/'functions.txt';registry=pal/'sym.main.txt'
            listing.write_text('First 80010000 10\n')
            registry.write_text('First = 0x80010000; // type:func\nData = 0x80020000; // type:u32\n')
            with patch.object(c_functions, '_ROOT', root):
                self.assertEqual(c_functions.address_of('First'),0x80010000)
                self.assertEqual(c_functions.address_of('Data'),0x80020000)
                self.assertEqual(c_functions.name_at(0x80010000),'First')
                previous=registry.stat()
                replacement=pal/'replacement';replacement.write_text('Other = 0x80010000; // type:func\nData = 0x80030000; // type:u32\n')
                os.utime(replacement,ns=(previous.st_atime_ns,previous.st_mtime_ns))
                replacement.replace(registry)
                self.assertEqual(c_functions.name_at(0x80010000),'Other')
                listing.write_text('Other 80010000 10\n')
                self.assertEqual(set(c_functions.known_functions()),{'Other'})
                self.assertEqual(c_functions.name_at('80010000'),'Other')
                self.assertEqual(c_functions.address_of('Data'),0x80030000)
                with self.assertRaises(KeyError):
                    c_functions.address_of('First')


if __name__ == '__main__':
    unittest.main()

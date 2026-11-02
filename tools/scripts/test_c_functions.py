import unittest
from pathlib import Path
from tempfile import TemporaryDirectory

from c_functions import function_definitions, source_owners


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


if __name__ == '__main__':
    unittest.main()

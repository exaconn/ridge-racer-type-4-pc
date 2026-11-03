"""Native recovered row initializer: bounds, full pointers, and live pointer aliasing."""
import ctypes as C
from pathlib import Path
import random
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest


class UiRowInitializationTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        header = (root/'include/rr/ui.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        body = (root/'tools/scripts/fixtures/views/func_8003E928.c').read_text()
        body = re.sub(r'^#include[^\n]*\n', '', body, flags=re.M)
        body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
        assert body.count('page=(u8*)0x800F0000U;') == 1
        body = body.replace('page=(u8*)0x800F0000U;', 'page=test_page;')
        source = '''#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
typedef uint8_t u8;typedef int16_t s16;typedef uint32_t u32;typedef int32_t s32;
#define RR_FIELD_OFFSET(T,F) offsetof(T,F)
''' + header + '''
s32 D_800F4BEC;
u8 *test_page;
s32 test_initialize(void) {return posix_memalign((void **)&test_page,65536,65536);}
void test_release(void) {free(test_page);}
unsigned long test_row_size(void) {return sizeof(RrUiRow24);}
''' + body
        directory = Path(cls.temp.name)
        (directory/'row.c').write_text(source)
        result = subprocess.run(['cc', '-shared', '-fPIC', '-O2', '-Wall', '-Werror','-Wno-error=deprecated-non-prototype',
                                 str(directory/'row.c'), '-o', str(directory/'row.so')],
                                capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stderr)
        cls.lib = C.CDLL(str(directory/'row.so'))
        cls.lib.test_initialize.restype = C.c_int
        if cls.lib.test_initialize():
            raise AssertionError('aligned page allocation failed')
        cls.addClassCleanup(cls.lib.test_release)
        cls.address = C.c_void_p.in_dll(cls.lib, 'test_page').value
        cls.memory = (C.c_uint8*65536).from_address(cls.address)
        cls.limit = C.c_int32.in_dll(cls.lib, 'D_800F4BEC')
        cls.initialize = cls.lib.func_8003E928
        cls.initialize.argtypes = [C.c_int32]*10
        cls.initialize.restype = None
        cls.lib.test_row_size.restype = C.c_ulong

    def setUp(self):
        self.memory[:] = bytes((i*73+19)&255 for i in range(65536))
        self.set_base(0x8000)
        self.limit.value = 8

    def set_base(self, offset):
        C.c_void_p.from_address(self.address+0x4D80).value = self.address+offset

    def run_case(self, args, limit=None):
        if limit is not None:
            self.limit.value = limit
        expected = bytearray(self.memory)
        index,a,b,c,d,e,f,count,g,h = [C.c_int32(x).value for x in args]
        if index < self.limit.value:
            offset = C.c_int32(index*36).value
            def row():
                pointer = int.from_bytes(expected[0x4D80:0x4D80+C.sizeof(C.c_void_p)], 'little')
                result = pointer-self.address+offset
                self.assertGreaterEqual(result, 0)
                self.assertLess(result+36, 65536)
                return result
            for field,value in ((0x15,g),(0x16,h),(1,f),(0x14,e)):
                expected[row()+field] = value&255
            current = row()
            expected[current+2] = count&255
            tail = row()
            for field,value in ((4,a),(6,b),(8,c),(10,d)):
                struct.pack_into('<H', expected, current+field, value&65535)
            expected[tail+0x17] = 0
            struct.pack_into('<H', expected, row()+0x20, 0)
            for i in range(max(count,0)):
                target = row()+0xC+i
                self.assertLess(target, 65536)
                expected[target] = 0
        self.initialize(*args)
        self.assertEqual(bytes(self.memory), bytes(expected))

    def test_layout_bounds_and_accessible_negative_prefix(self):
        self.assertEqual(self.lib.test_row_size(), 36)
        args = [0,0x12345678,-1,0x8000,0x7FFFFFFF,0x123,0x345,4,0x456,0x567]
        for limit in (-5,0,1,8):
            for index in (-8,-1,0,1,7,8,9,0x7FFFFFFF):
                args[0] = index
                self.run_case(args, limit)
        self.set_base(0)  # A rejected index never dereferences the row allocation.
        self.run_case([0x7FFFFFFF]+[0]*9, 1)

    def test_clear_lengths_and_truncation_with_neighbor_guards(self):
        for count in (-0x80000000,-257,-1,0,1,7,8,9,20,24,25,36,72,127,255,256,257):
            self.setUp()
            self.run_case([2,-0x1234567,0x76543210,-65537,65536,0x12345,-1,count,256,-257])
        generator = random.Random(0x3E928)
        for _ in range(1000):
            self.set_base(0x8000)
            args = [generator.randrange(-8,9)]+[generator.randrange(-2**31,2**31) for _ in range(9)]
            args[7] = generator.randrange(-4,100)
            self.run_case(args, generator.randrange(-2,9))

    def test_pointer_reload_capture_and_clear_loop_aliasing(self):
        self.set_base(0x4D6A)  # byte16 writes the low byte of the global row pointer.
        self.run_case([0,1,2,3,4,5,6,2,7,0x8A])
        self.setUp()
        self.set_base(0x4D7C)  # half04 rewrites the low pointer halfword after tail capture.
        pointer = self.address+0x4D9C
        self.run_case([0,pointer&65535,(pointer>>16)&65535,(pointer>>32)&65535,
                       (pointer>>48)&65535,1,2,0,3,4])
        self.assertEqual(self.memory[0x4D7C+0x17],0)
        self.assertEqual(bytes(self.memory[0x4D9C+0x20:0x4D9C+0x22]),b'\0\0')
        self.setUp()
        self.set_base(0x4D74)  # First clear changes the global pointer; the next clear reloads it.
        self.run_case([0,1,2,3,4,5,6,2,7,8])
        self.assertEqual(C.c_void_p.from_address(self.address+0x4D80).value,self.address+0x4D00)
        self.assertEqual(self.memory[0x4D0D],0)


if __name__ == '__main__':
    unittest.main()

"""Actual byte selection/history TU with native pointer fields and prefix reads."""
import ctypes as C
from pathlib import Path
import random
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class Transfer(C.Structure):
    _fields_=[('pad00',C.c_uint8*0x28),('data28',C.c_void_p),('data2C',C.c_void_p),
              ('pad30',C.c_uint8*4),('length34',C.c_uint8),('pad35',C.c_uint8),
              ('length36',C.c_uint8),('command37',C.c_uint8),('previous38',C.c_uint8),
              ('pad39',C.c_uint8*12),('phase45',C.c_uint8),('pad46',C.c_uint8*14),('gates54',C.c_uint8*9)]


class ChannelTransferByteTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2];d=Path(cls.temp.name)
        header=(root/'include/rr/channels.h').read_text()
        header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
        body=(root/'src/main/PAL/psyq/libpad/padlist.c').read_text().replace('#include "rr/channels.h"','')
        body=re.sub(r'\s+__asm__\("\$[0-9]+"\)','',body)
        code='''#include <stdint.h>
#include <stddef.h>
typedef uint8_t u8; typedef int8_t s8; typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
'''+header+'''
unsigned long native_size=sizeof(RrChannelTransferView);
unsigned long native_offsets[]={offsetof(RrChannelTransferView,data28),offsetof(RrChannelTransferView,phase45),offsetof(RrChannelTransferView,gates54)};
'''+body
        (d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'))
        assert C.c_ulong.in_dll(cls.lib,'native_size').value==C.sizeof(Transfer)
        assert list((C.c_ulong*3).in_dll(cls.lib,'native_offsets'))==[Transfer.data28.offset,Transfer.phase45.offset,Transfer.gates54.offset]
        cls.get=cls.lib.func_80099FA0;cls.get.argtypes=[C.POINTER(Transfer)];cls.get.restype=C.c_uint32
        cls.history=cls.lib.func_80099F90;cls.history.argtypes=[C.POINTER(Transfer)];cls.history.restype=None

    def setUp(self):
        self.ctx=Transfer();C.memset(C.addressof(self.ctx),0xA5,C.sizeof(self.ctx))
        self.primary=(C.c_uint8*264)(*[(i*37+0x80)&255 for i in range(264)])
        self.secondary=(C.c_uint8*264)(*[(i*73+0x33)&255 for i in range(264)])
        self.ctx.data28=C.addressof(self.primary)+3;self.ctx.data2C=C.addressof(self.secondary)+3
        self.ctx.gates54[:]=[1]*9;self.ctx.length34=255;self.ctx.length36=255;self.ctx.command37=0;self.ctx.phase45=3
        self.primary_before=bytes(self.primary);self.secondary_before=bytes(self.secondary)

    def expected(self):
        c=self.ctx;position=int(c.phase45)-3
        if c.command37==0:
            if c.phase45<=8 and c.gates54[c.phase45]==0:return 0
            return self.primary[position+3] if position<c.length34 else 0
        if position<c.length36:return self.secondary[position+3]
        return 255 if c.command37==0x4D else 0

    def check(self):self.assertEqual(self.get(C.byref(self.ctx)),self.expected())

    def test_all_commands_phases_and_length_boundaries(self):
        for command in range(256):
            self.ctx.command37=command
            self.ctx.gates54[:]=[0 if (command+i)%3==0 else 0x80+i for i in range(9)]
            for first,last in ((0,255),(1,6),(6,1),(255,0)):
                self.ctx.length34=first;self.ctx.length36=last
                for phase in range(256):self.ctx.phase45=phase;self.check()
        self.assertEqual(bytes(self.primary),self.primary_before);self.assertEqual(bytes(self.secondary),self.secondary_before)

    def test_gate_flags_prefix_reads_and_unused_null_buffers(self):
        for phase in range(9):
            for flag in (0,1,2,128,255):
                self.ctx.command37=0;self.ctx.phase45=phase;self.ctx.gates54[phase]=flag
                self.check()
                if flag==0:
                    saved=self.ctx.data28;self.ctx.data28=None
                    self.assertEqual(self.get(C.byref(self.ctx)),0);self.ctx.data28=saved
        for command in (0,1,0x4D,255):
            self.ctx.command37=command;self.ctx.length34=0;self.ctx.length36=0
            self.ctx.data28=None;self.ctx.data2C=None
            for phase in (3,4,8,9,255):
                self.ctx.phase45=phase
                self.assertEqual(self.get(C.byref(self.ctx)),255 if command==0x4D else 0)
        self.ctx.length34=255;self.ctx.length36=255;self.ctx.phase45=0
        self.ctx.command37=0x4D;self.ctx.data2C=C.addressof(self.secondary)+3
        self.assertEqual(self.get(C.byref(self.ctx)),self.secondary[0])
        self.ctx.command37=0;self.ctx.gates54[0]=1;self.ctx.data28=C.addressof(self.primary)+3;self.ctx.data2C=None
        self.assertEqual(self.get(C.byref(self.ctx)),self.primary[0])

    def test_random_records_do_not_mutate_inputs_or_payloads(self):
        rng=random.Random(0x99FA0)
        for _ in range(10000):
            self.ctx.command37=rng.randrange(256);self.ctx.phase45=rng.randrange(256)
            self.ctx.length34=rng.randrange(256);self.ctx.length36=rng.randrange(256)
            self.ctx.gates54[:]=[rng.randrange(256) for _ in range(9)]
            before=bytes(self.ctx);self.check();self.assertEqual(bytes(self.ctx),before)
        self.assertEqual(bytes(self.primary),self.primary_before);self.assertEqual(bytes(self.secondary),self.secondary_before)

    def test_history_helper_changes_only_command_and_previous_byte(self):
        for command in range(256):
            self.ctx.command37=command;self.ctx.previous38=command^255;self.ctx.phase45=7
            self.ctx.gates54[7]=0
            self.check()
            expected=bytearray(self.ctx);expected[Transfer.command37.offset]=0;expected[Transfer.previous38.offset]=command
            self.history(C.byref(self.ctx))
            self.assertEqual(bytes(self.ctx),bytes(expected))
            self.assertEqual(self.get(C.byref(self.ctx)),0)
        self.assertEqual(bytes(self.primary),self.primary_before);self.assertEqual(bytes(self.secondary),self.secondary_before)

"""Vehicle wire adapters: real native pointers, independent PAL fields and guards."""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from vehicle_native import C_BRIDGE, NativeVehicles

ROOT=Path(__file__).resolve().parents[2]


class NativeVehicleTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        path=Path(cls.temp.name)
        source=C_BRIDGE+r'''
static struct {u32 before[4];RaceVehicle cars[2];u32 after[4];} root;
void *test_root(void) {return root.cars;}
uintptr_t test_pointer(unsigned index) {return (uintptr_t)root.cars[index].control.handling1C;}
uintptr_t test_record(unsigned index) {return (uintptr_t)root.cars[index].control.record18;}
int test_related(unsigned index) {return root.cars[index].control.handling1C==&root.cars[index].control.record18->handling1C8;}
u32 test_field(unsigned index,unsigned field) {
    RaceVehicle *v=&root.cars[index];
    switch(field) {
    case 0:return (u32)v->posScaled00[0];
    case 1:return (u32)v->first;
    case 2:return (u32)v->control.speed4C;
    case 3:return v->control.engine54.halves.rpm;
    case 4:return (u16)v->control.metric12A;
    default:assert(0);return 0;
    }
}
void test_change(unsigned index,u32 speed,u16 rpm,s16 metric) {
    RaceVehicleControl *c=&root.cars[index].control;
    c->speed4C=(s32)speed;c->engine54.halves.rpm=rpm;c->metric12A=metric;
}
void test_pointee(unsigned index,s16 value) {root.cars[index].control.handling1C->steeringMode0A=value;}
s16 test_surface(unsigned index) {return root.cars[index].control.handling1C->steeringMode0A;}
'''
        (path/'test.c').write_text(source)
        built=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC',
                              '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                              '-I',str(ROOT/'include'),'-I',str(ROOT/'tools/scripts'),
                              str(path/'test.c'),'-o',str(path/'test.dylib')],
                             capture_output=True,text=True)
        if built.returncode:
            raise AssertionError(built.stderr)
        cls.lib=C.CDLL(str(path/'test.dylib'))
        cls.lib.test_root.restype=C.c_void_p
        cls.lib.test_pointer.restype=C.c_size_t
        cls.lib.test_record.restype=C.c_size_t
        cls.lib.test_field.restype=C.c_uint32
        cls.lib.test_change.argtypes=[C.c_uint,C.c_uint32,C.c_uint16,C.c_int16]
        cls.lib.test_pointee.argtypes=[C.c_uint,C.c_int16]
        cls.lib.test_surface.restype=C.c_int16
        cls.native=cls.lib.test_root();cls.address=0x80180000
        cls.vehicles=NativeVehicles(cls.lib)
        cls.vehicles.add(cls.native,cls.address,2,16)

    def test_pal_fields_full_native_stride_pointer_alias_and_roundtrip(self):
        randomizer=random.Random(0x210320)
        self.assertGreater(self.native,0xFFFFFFFF)
        for case in range(128):
            data=bytearray(randomizer.randbytes(1632))
            pointer=0x80200000+case*4096
            for record in range(2):
                struct.pack_into('<I',data,16+record*800+0x20C,pointer)
                struct.pack_into('<I',data,16+record*800+0x210,pointer+0x1C8)
            self.vehicles.reset()
            self.vehicles.write(self.native-16,self.address-16,data)
            self.assertEqual(self.vehicles.read(self.native-16,self.address-16,1632),data)
            self.assertEqual(self.lib.test_pointer(0),self.lib.test_pointer(1))
            self.assertGreater(self.lib.test_pointer(0),0xFFFFFFFF)
            self.assertGreater(self.lib.test_record(0),0xFFFFFFFF)
            self.assertTrue(self.lib.test_related(0))
            self.lib.test_pointee(0,case-64)
            self.assertEqual(self.lib.test_surface(1),case-64)
            for record in range(2):
                for field,offset,width in ((0,0,4),(1,0x184,4),(2,0x240,4),
                                           (3,0x24A,2),(4,0x31E,2)):
                    start=16+record*800+offset
                    self.assertEqual(self.lib.test_field(record,field),
                                     int.from_bytes(data[start:start+width],'little'))
            speed=0x80000000+case;rpm=65535-case;metric=case-64
            self.lib.test_change(1,speed,rpm,metric)
            struct.pack_into('<I',data,816+0x240,speed)
            struct.pack_into('<H',data,816+0x24A,rpm)
            struct.pack_into('<h',data,816+0x31E,metric)
            self.assertEqual(self.vehicles.read(self.native-16,self.address-16,1632),data)

    def test_partial_wire_writes_and_pointer_normalization(self):
        self.vehicles.reset()
        data=bytearray(b'\xA6'*1632)
        self.vehicles.write(self.native-16,self.address-16,data)
        for record in range(2):
            for offset in (0,0x184,0x1F4,0x240,0x2A8,0x31E):
                address=self.address+record*800+offset
                native=self.vehicles.to_native(address)
                self.assertEqual(self.vehicles.to_pal(native),address)
                changed=bytes((record+17,offset&255))
                self.vehicles.write(native,address,changed)
                data[16+record*800+offset:18+record*800+offset]=changed
        self.assertEqual(self.vehicles.read(self.native-16,self.address-16,1632),data)
        if C.sizeof(C.c_void_p)==8:
            self.assertEqual(self.vehicles.vehicle_size,816)
            with self.assertRaisesRegex(AssertionError,'native vehicle padding'):
                self.vehicles.to_pal(self.native+0x1F4)

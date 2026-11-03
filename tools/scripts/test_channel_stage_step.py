"""Actual stage dispatcher with native channel packet pointers and callback mutation."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest
from test_cd_disc_probe import native_body
from test_channel_callbacks import ChannelRecord,ChannelStatus

Hook=C.CFUNCTYPE(None,C.c_void_p)


class ChannelStageStepTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        root=Path(__file__).resolve().parents[2];d=Path(cls.temp.name)
        h=(root/'include/rr/channels.h').read_text()
        h=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',h,flags=re.M)
        body=native_body((root/'build/legacy/func_80098488.c').read_text())
        assert body.count('(s32 *)0x800B0000U')==1
        body=body.replace('(s32 *)0x800B0000U','ram+0x4354/4')
        code='''#include <stdint.h>
#include <stddef.h>
typedef uint8_t u8;typedef int8_t s8;typedef uint16_t u16;typedef int16_t s16;typedef uint32_t u32;typedef int32_t s32;
#define RR_FIELD_OFFSET(type,field) offsetof(type,field)
#define RR_NOP_DEP_ON(value) ((void)(value))
'''+h+'''
s32 ram[0x4354/4+2];
#define D_800ABCAC ram[0]
RrChannelRecordF0 channel;
s32 handler_result,mutate_stage,stage_after,wait_result,events,event_kind[16],event_stage[16],event_code[16];
void *event_pointer[16];void (*on_handler)(void *),(*on_timer)(void *),(*on_wait)(void *),(*on_error)(void *);
unsigned long record_size=sizeof(RrChannelRecordF0),packet_offset=offsetof(RrChannelStatusView,packet);
void trace(s32 kind,s32 code,void *p){s32 i=events++;event_kind[i]=kind;event_stage[i]=D_800ABCAC;event_code[i]=code;event_pointer[i]=p;}
s32 handler(s32 selected,RrChannelRecordF0 *p){
    trace(10+selected,0,p);if(mutate_stage)D_800ABCAC=stage_after;
    if(on_handler)on_handler(p);return handler_result;
}
s32 h0(RrChannelRecordF0 *p){return handler(0,p);}s32 h1(RrChannelRecordF0 *p){return handler(1,p);}
s32 h2(RrChannelRecordF0 *p){return handler(2,p);}s32 h3(RrChannelRecordF0 *p){return handler(3,p);}
s32 h4(RrChannelRecordF0 *p){return handler(4,p);}
RrChannelStageCallback D_800ABCEC[5]={h0,h1,h2,h3,h4};
void error_a(s32 code){trace(3,code,0);if(on_error)on_error(0);}
void error_b(s32 code){trace(4,code,0);if(on_error)on_error(0);}
RrChannelErrorCallback D_800ABC68=error_a;
void func_8009A7F4(s32 limit){trace(1,limit,0);if(on_timer)on_timer(0);}
s32 func_80098A00(void){trace(2,0,0);if(on_wait)on_wait(0);return wait_result;}
'''+body
        (d/'f.c').write_text(code)
        subprocess.run(['cc','-shared','-fPIC','-O2',str(d/'f.c'),'-o',str(d/'f.so')],check=True,capture_output=True)
        cls.lib=C.CDLL(str(d/'f.so'));cls.f=cls.lib.func_80098488;cls.f.argtypes=[C.POINTER(ChannelRecord)];cls.f.restype=None
        assert C.c_ulong.in_dll(cls.lib,'record_size').value==C.sizeof(ChannelRecord)
        assert C.c_ulong.in_dll(cls.lib,'packet_offset').value==ChannelStatus.packet.offset
        cls.channel=ChannelRecord.in_dll(cls.lib,'channel');cls.ram=(C.c_int32*(0x4354//4+2)).in_dll(cls.lib,'ram')
        cls.values={n:C.c_int32.in_dll(cls.lib,n) for n in ('handler_result','mutate_stage','stage_after','wait_result','events')}
        cls.kind=(C.c_int32*16).in_dll(cls.lib,'event_kind');cls.stage=(C.c_int32*16).in_dll(cls.lib,'event_stage')
        cls.code=(C.c_int32*16).in_dll(cls.lib,'event_code');cls.pointer=(C.c_void_p*16).in_dll(cls.lib,'event_pointer')
        cls.error=C.c_void_p.in_dll(cls.lib,'D_800ABC68')

    def setUp(self):self.reset()

    def reset(self):
        C.memset(self.ram,0xA5,C.sizeof(self.ram));C.memset(C.addressof(self.channel),0xC3,C.sizeof(self.channel))
        self.channel.status.packet=None;self.ram[0]=0
        for v in self.values.values():v.value=0
        self.values['wait_result'].value=1
        for n in ('on_handler','on_timer','on_wait','on_error'):C.c_void_p.in_dll(self.lib,n).value=None
        self.error.value=C.cast(self.lib.error_a,C.c_void_p).value
        self.retained=[];self.errors=[]

    def hook(self,name,fn):
        def safe(p):
            try:fn(p)
            except BaseException as error:self.errors.append(error)
        callback=Hook(safe);self.retained.append(callback);C.c_void_p.in_dll(self.lib,name).value=C.cast(callback,C.c_void_p).value

    def invoke(self):
        guards=bytes(self.ram)[4:]
        self.f(C.byref(self.channel))
        if self.errors:raise self.errors.pop(0)
        self.assertEqual(bytes(self.ram)[4:],guards)

    def trace(self):return list(self.kind)[:self.values['events'].value]

    def test_every_table_entry_publishes_increment_before_single_channel_argument(self):
        for selected in range(5):
            self.reset();self.ram[0]=selected;self.values['handler_result'].value=-123
            before=bytes(self.channel);self.invoke()
            self.assertEqual(self.trace(),[10+selected,3])
            self.assertEqual((self.stage[0],self.pointer[0]),(selected+1,C.addressof(self.channel)))
            self.assertEqual(self.code[1],-123);self.assertEqual(self.ram[0],selected+1)
            self.assertEqual(bytes(self.channel),before)

    def test_positive_paths_marker_gate_and_one_step_saturation(self):
        packet=(C.c_uint8*4)(0,0xA5,0x5A,0xFF)
        for selected in range(5):
            for after in (-2147483648,-1,0,1,2,3,4,5,6,100,2147483647):
                for marker in (0,0x80,0xFF):
                    self.reset();self.ram[0]=selected;self.values['mutate_stage'].value=1
                    self.values['stage_after'].value=after;self.values['handler_result'].value=2147483647
                    packet[0]=marker
                    self.channel.status.packet=C.addressof(packet) if after==3 else None
                    before=(bytes(self.channel),bytes(packet));self.invoke()
                    wait=after!=0 and not(after==3 and marker==0x80)
                    self.assertEqual(self.trace(),[10+selected]+([1,2] if wait else []))
                    if wait:self.assertEqual(self.code[1],60)
                    self.assertEqual(self.ram[0],after-int(after>=5))
                    self.assertEqual((bytes(self.channel),bytes(packet)),before)

    def test_negative_result_uses_fresh_error_callback_and_skips_saturation(self):
        for code in (-2147483648,-3,-1):
            self.reset();self.values['handler_result'].value=code
            def handler(p):self.error.value=C.cast(self.lib.error_b,C.c_void_p).value
            def error(p):self.ram[0]=100
            self.hook('on_handler',handler);self.hook('on_error',error)
            self.invoke()
            self.assertEqual(self.trace(),[10,4]);self.assertEqual(self.code[1],code)
            self.assertEqual(self.ram[0],100)

    def test_timer_wait_error_mutations_are_reloaded_before_final_decrement(self):
        self.ram[0]=2;self.values['mutate_stage'].value=1;self.values['stage_after'].value=6;self.values['wait_result'].value=0
        self.hook('on_timer',lambda p:self.ram.__setitem__(0,99))
        def wait(p):
            self.ram[0]=6;self.error.value=C.cast(self.lib.error_b,C.c_void_p).value
        self.hook('on_wait',wait);self.hook('on_error',lambda p:self.ram.__setitem__(0,11))
        self.invoke()
        self.assertEqual(self.trace(),[12,1,2,4]);self.assertEqual(list(self.stage)[:4],[3,6,99,6])
        self.assertEqual(list(self.code)[:4],[0,60,0,-3]);self.assertEqual(self.ram[0],10)

    def test_handler_can_replace_full_native_packet_pointer_before_marker_read(self):
        packet=(C.c_uint8*4)(0x80,1,2,3)
        self.ram[0]=2;self.values['handler_result'].value=0
        self.hook('on_handler',lambda p:setattr(self.channel.status,'packet',C.addressof(packet)))
        self.invoke()
        if C.sizeof(C.c_void_p)>4:self.assertGreater(self.channel.status.packet,0xFFFFFFFF)
        self.assertEqual(self.trace(),[12]);self.assertEqual(self.ram[0],3)
        self.assertEqual(bytes(packet),b'\x80\1\2\3')

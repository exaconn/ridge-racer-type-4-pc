"""Actual immediate VRAM move, callback exchange, and DMA packet launch."""
import ctypes as C
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest


class Rect(C.Structure):
    _fields_ = [('x', C.c_int16), ('y', C.c_int16), ('w', C.c_int16), ('h', C.c_int16)]


Poll = C.CFUNCTYPE(C.c_int32)
Hook = C.CFUNCTYPE(None)


class GpuMoveImageTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = TemporaryDirectory()
        cls.addClassCleanup(cls.temp.cleanup)
        root = Path(__file__).resolve().parents[2]
        headers = ''
        for name in ('gpu', 'library', 'timing'):
            text = (root/('include/rr/' + name + '.h')).read_text()
            text = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', text, flags=re.M)
            text = text.replace('extern RrGpuMovePayload D_800ABB60;', '')
            headers += text
        code = '''#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef uint8_t u8;typedef int8_t s8;typedef uint16_t u16;typedef int16_t s16;typedef uint32_t u32;typedef int32_t s32;
''' + headers + '''
struct {u32 guard;u32 prefix[2];RrGpuMovePayload payload;u32 after;} packet;
#define D_800ABB60 packet.payload
u32 ports[8];
volatile u32 *D_800ABBC8=&ports[0],*D_800ABBD4=&ports[1],*D_800ABBCC=&ports[2],*D_800ABBD0=&ports[3];
s32 D_800ABBFC,D_800ABC00;
char D_80011EFC[]="MoveImage";
s32 test_now,test_timeout_calls,test_debug_calls,test_exchange_calls,test_draw_calls,test_draw_driver;
s32 test_event_count;u32 test_events[64][3];
void *test_rect;const char *test_name;unsigned long test_address;
u32 test_payload[3];s32 test_channel;RrLibInterruptCallback test_callback;
s32 (*test_poll)(void);void (*test_debug_hook)(void),(*test_exchange_hook)(void);
void event(u32 kind,u32 arg,u32 value){s32 i=test_event_count++;if(i<64){test_events[i][0]=kind;test_events[i][1]=arg;test_events[i][2]=value;}}
void checkRECT(const char *name,RrGpuRectangle *rect){
 test_debug_calls++;test_name=name;test_rect=rect;event(1,0,0);if(test_debug_hook)test_debug_hook();
}
s32 VSync(s32 mode){event(2,(u32)mode,0);return test_now;}
s32 get_alarm(void){test_timeout_calls++;event(3,0,0);return test_poll();}
u32 test_dma_read(volatile u32 *p){event(4,p==&ports[1]?1:5,*p);return *p;}
u32 test_gpu_read(volatile u32 *p){event(5,p==&ports[0]?0:4,*p);return *p;}
void _exeque(void){}
RrLibInterruptCallback test_exchange(s32 channel,RrLibInterruptCallback callback){
 RrLibInterruptCallback old=test_callback;test_callback=callback;test_channel=channel;
 test_exchange_calls++;event(6,channel,0);if(test_exchange_hook)test_exchange_hook();return old;
}
RrLibInterruptDriverPrefix irq_driver={0,test_exchange,0,0};
RrLibInterruptDriverPrefix *D_800A6350=&irq_driver;
void draw(unsigned long address,s32 driver){
 test_draw_calls++;test_address=address;test_draw_driver=driver;
 memcpy(test_payload,&packet.payload,sizeof(test_payload));event(7,driver,0);
 _cwc(address);
}
void draw0(unsigned long address){draw(address,0);}
void draw1(unsigned long address){draw(address,1);}
RrLibGraphicsDriverPrefix drivers[2],*D_800ABAB8;
void test_select(s32 index){D_800ABAB8=&drivers[index];}
u32 test_layout[]={sizeof(RrGpuRectangle),sizeof(RrGpuMovePayload),offsetof(RrLibGraphicsDriverPrefix,draw_ordering_table)};
void test_reset(void){
 s32 i;memset(&packet,0xA5,sizeof(packet));for(i=0;i<8;i++)ports[i]=0xA5000000+i;
 ports[0]=0x04000000;ports[1]=0;
 D_800ABBC8=&ports[0];D_800ABBD4=&ports[1];D_800ABBCC=&ports[2];D_800ABBD0=&ports[3];
 memset(drivers,0x5A,sizeof(drivers));drivers[0].draw_ordering_table=draw0;drivers[1].draw_ordering_table=draw1;D_800ABAB8=&drivers[0];
 D_800ABBFC=-1;D_800ABC00=999;test_now=100;
 test_timeout_calls=test_debug_calls=test_exchange_calls=test_draw_calls=test_event_count=0;
 test_debug_hook=test_exchange_hook=0;test_callback=0;test_address=0;
}
void *test_packet_address(void){return packet.prefix;}
'''
        for name in ('8008B3C4', '80095990', '80094B28', '80095750'):
            body = (root/('tools/scripts/fixtures/views/func_' + name + '.c')).read_text()
            body = re.sub(r'^#include[^\n]*\n', '', body, flags=re.M)
            body = re.sub(r'\s+__asm__\("\$[0-9]+"\)', '', body)
            body = re.sub(r'"\$[0-9]+",\s*', '', body)
            body = re.sub(r':\s*"\$[0-9]+"(?:,\s*"\$[0-9]+")*(?=\))', '', body)
            if name == '80095750':
                assert body.count('status=*D_800ABBD4;') == 2
                assert body.count('*D_800ABBC8&') == 1
                body = body.replace('status=*D_800ABBD4;', 'status=test_dma_read(D_800ABBD4);')
                body = body.replace('*D_800ABBC8&', 'test_gpu_read(D_800ABBC8)&')
            code += body
        directory = Path(cls.temp.name)
        (directory/'function.c').write_text(code)
        result = subprocess.run(['cc', '-shared', '-fPIC', '-O2', str(directory/'function.c'),
                                 '-o', str(directory/'function.so')], capture_output=True)
        if result.returncode:
            raise AssertionError(result.stderr.decode())
        cls.lib = C.CDLL(str(directory/'function.so'))
        cls.move = cls.lib.MoveImage2
        cls.move.argtypes = [C.POINTER(Rect), C.c_int32, C.c_int32]
        cls.move.restype = C.c_int32
        cls.ports = (C.c_uint32 * 8).in_dll(cls.lib, 'ports')
        cls.packet = (C.c_uint32 * 7).in_dll(cls.lib, 'packet')
        cls.lib.test_packet_address.restype = C.c_void_p
        cls.lib.test_select.argtypes = [C.c_int32]

    def scalar(self, name):
        return C.c_int32.in_dll(self.lib, name)

    def invoke(self, rect, x, y, poll=lambda: 0, debug=None, exchange=None):
        errors = []
        def checked_poll():
            try:
                self.assertEqual(self.scalar('D_800ABC00').value, 0)
                return poll()
            except BaseException as error:
                errors.append(error)
                return -1
        def check_hook(hook):
            def checked():
                try:
                    hook()
                except BaseException as error:
                    errors.append(error)
            return checked
        callbacks = [Poll(checked_poll)]
        for name, hook in [('test_debug_hook', debug), ('test_exchange_hook', exchange)]:
            if hook:
                callback = Hook(check_hook(hook));callbacks.append(callback)
                C.c_void_p.in_dll(self.lib, name).value = C.cast(callback, C.c_void_p).value
        C.c_void_p.in_dll(self.lib, 'test_poll').value = C.cast(callbacks[0], C.c_void_p).value
        result = self.move(C.byref(rect), x, y)
        if errors:
            raise errors[0]
        return result

    def test_rectangles_and_destination_bits_with_actual_dma_writer(self):
        for width in (-32768, -1, 0, 1, 32767):
            for height in (-32768, -1, 0, 1, 32767):
                for x, y in [(-0x80000000, -1), (-1, 0x7FFFFFFF), (0, 0), (65535, 65536), (0x12345678, -0x1234567)]:
                    self.lib.test_reset()
                    rect = Rect(-123, 456, width, height);before = bytes(rect)
                    result = self.invoke(rect, x, y)
                    valid = width != 0 and height != 0
                    self.assertEqual(result, 0 if valid else -1)
                    self.assertEqual(self.scalar('test_exchange_calls').value, 1)
                    self.assertEqual(self.scalar('test_draw_calls').value, int(valid))
                    self.assertEqual(bytes(rect), before)
                    self.assertEqual((self.scalar('D_800ABBFC').value,self.scalar('D_800ABC00').value),(340,0))
                    self.assertEqual((self.packet[0],self.packet[1],self.packet[2],self.packet[6]),(0xA5A5A5A5,)*4)
                    if valid:
                        packed_x_y = int.from_bytes(before[:4], 'little')
                        packed_size = int.from_bytes(before[4:], 'little')
                        self.assertEqual(list(self.packet)[3:6], [packed_x_y,((y<<16)|(x&65535))&0xFFFFFFFF,packed_size])
                        address = self.lib.test_packet_address()
                        self.assertEqual(C.c_ulong.in_dll(self.lib,'test_address').value,address)
                        if C.sizeof(C.c_void_p)>4:self.assertGreater(address,0xFFFFFFFF)
                        self.assertEqual(list(self.ports)[:4],[0x04000002,0x01000401,address&0xFFFFFFFF,0])
                    else:
                        self.assertEqual(list(self.packet),[0xA5A5A5A5]*7)

    def test_timeout_precedes_fresh_dma_read_and_gpu_wait(self):
        for value in (-1, 1, -0x80000000, 0x7FFFFFFF):
            self.lib.test_reset();self.ports[1]=0x01000000
            rect=Rect(1,2,3,4)
            self.assertEqual(self.invoke(rect,5,6,poll=lambda:value),-1)
            self.assertEqual(self.scalar('test_timeout_calls').value,1)
            self.assertEqual(self.scalar('test_exchange_calls').value,0)
            self.assertEqual(list(self.packet),[0xA5A5A5A5]*7)
            self.assertEqual(self.scalar('test_event_count').value,4)
        self.lib.test_reset();self.ports[0]=0
        self.assertEqual(self.invoke(Rect(1,2,3,4),0,0,poll=lambda:1),-1)
        self.assertEqual(self.scalar('test_timeout_calls').value,1)

    def test_helper_mutations_select_fresh_ports_rectangle_and_driver(self):
        self.lib.test_reset();self.ports[1]=0x01000000
        rect=Rect(1,2,0,0)
        def debug():
            self.scalar('test_now').value=0x7FFFFFFF
            rect.w=7;rect.h=8
        def poll():
            C.c_void_p.in_dll(self.lib,'D_800ABBD4').value=C.addressof(self.ports)+20
            C.c_void_p.in_dll(self.lib,'D_800ABBC8').value=C.addressof(self.ports)+16
            self.ports[5]=0;self.ports[4]=0x04000000
            return 0
        def exchange():
            self.lib.test_select(1)
            rect.x=-9;rect.y=10;rect.w=-11;rect.h=12
        self.assertEqual(self.invoke(rect,-1,0x12345678,poll,debug,exchange),0)
        self.assertEqual(self.scalar('D_800ABBFC').value,C.c_int32(0x7FFFFFFF+240).value)
        self.assertEqual(self.scalar('test_draw_driver').value,1)
        self.assertEqual(list(self.packet)[3:6],[0x000AFFF7,0x5678FFFF,0x000CFFF5])
        self.assertEqual((self.ports[0],self.ports[1]),(0x04000000,0x01000000))
        self.assertEqual((self.ports[4],self.ports[5]),(0x04000002,0x01000401))
        callback=C.c_void_p.in_dll(self.lib,'test_callback').value
        self.assertEqual(callback,C.cast(self.lib.func_80095990,C.c_void_p).value)
        Hook(callback)()
        self.assertEqual(C.c_void_p.in_dll(self.lib,'test_callback').value,C.cast(self.lib._exeque,C.c_void_p).value)

    def test_source_rectangle_may_overlap_packet_payload(self):
        self.lib.test_reset()
        rect=Rect.from_buffer(self.packet,12)
        rect.x=-1;rect.y=2;rect.w=3;rect.h=4
        self.assertEqual(self.invoke(rect,0x1234,0x5678),0)
        # The original stores destination before reloading the source size word.
        self.assertEqual(list(self.packet)[3:6],[0x0002FFFF,0x56781234,0x56781234])
        self.assertEqual((self.packet[0],self.packet[1],self.packet[2],self.packet[6]),(0xA5A5A5A5,)*4)

    def test_dma_and_gpu_not_ready_each_return_to_watchdog(self):
        self.lib.test_reset();self.ports[1]=0x01000000;self.ports[0]=0
        polls=[]
        def poll():
            polls.append(1)
            self.ports[1]=0
            if len(polls)==2:self.ports[0]=0x04000000
            return 0
        self.assertEqual(self.invoke(Rect(1,2,3,4),5,6,poll=poll),0)
        self.assertEqual(len(polls),2)
        self.assertEqual(self.scalar('test_exchange_calls').value,1)

"""Actual camera/model setter and icon/sprite chain with recovered public APIs.

Camera uses a controlled BIOS clear boundary; native AddPrim is a controlled GPU
link boundary. All other included production bodies are unchanged. The icon
chain also runs all original/linked MIPS helpers, including the actual AddPrim.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu

ROOT=Path(__file__).resolve().parents[2]
CAMERA_PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "game/gfx/scene1.h"
#include "game/gfx/gtepoly1.h"
#include "game/car/bounds.h"
#include "rr/player_cars.h"
#include "rr/bios_devices.h"
#include "rr/camera_curve_tables.h"
typedef struct {
 u32 before[4];SceneRaceContext context;SceneCameraState160 alternate[2];
 RaceVehicle cars[2];CarBounds bounds[3];CarBounds *table[3];
 s32 masks[4];PairS16 curve[3];s16 maker,choice;s32 draw,page;u32 calls,after[4];
} World;
static World actual,expected;static unsigned scenario;
#define RaceContext actual.context
#define PlayerCars actual.cars
#define CarBoundsTable actual.table
#define ReplayCamBankWeights actual.masks
#define SelectedMakerIndex actual.maker
#define SelectedMakerCarIndex actual.choice
#define ShowcaseCamDepthCurve actual.curve
#define DrawSubOtFlag actual.draw
#define ModelTexturePage actual.page
static void clear_boundary(World *w){
 memset(&w->context.cameraEC,0,sizeof(SceneCameraState160));w->calls++;
 w->context.camera24C=scenario%3?&actual.alternate[scenario%3-1]:&actual.context.cameraEC;
 w->table[0]=&actual.bounds[scenario%3];
 w->maker=(s16)((scenario/32)%4);w->choice=(s16)(scenario%32);
}
void PsyqBzero(void *p,s32 bytes){
 assert(p==&actual.context.cameraEC&&bytes==sizeof(SceneCameraState160));clear_boundary(&actual);
}
'''
CAMERA_TAIL=r'''
static u32 seed=0xCA11AB1U;
static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
static void reference(s32 mode,s32 flags){
 clear_boundary(&expected);
 SceneCameraState160 *c=scenario%3?&expected.alternate[scenario%3-1]:&expected.context.cameraEC;
 static const s16 directions[3][3]={{449,-3576,-1946},{2970,-2779,482},{-2637,-1602,2694}};
 for(unsigned i=0;i<3;i++){
  c->lighting78.matrix.fields.vectors[i].x=directions[i][0];c->lighting78.matrix.fields.vectors[i].y=directions[i][1];c->lighting78.matrix.fields.vectors[i].z=directions[i][2];
  c->lighting78.matrix.fields.colors[i].red=c->lighting78.matrix.fields.colors[i].green=c->lighting78.matrix.fields.colors[i].blue=(u8)(128>>i);
  c->lighting78.ambient[i]=64;
 }
 c->position110.xyz.x=0;c->position110.xyz.y=-240;c->position110.xyz.z=0;
 c->rotation120.fields.x=c->rotation120.fields.y=c->rotation120.fields.z=0;
 c->value12C=2048;c->value130=440;c->parts18[0].position00.xyz.x=-192;c->parts18[1].position00.xyz.x=192;
 unsigned on=((u32)expected.masks[expected.maker]>>expected.choice)&1;
 c->extentA0=on?202:169;c->mirrorA4=(s32)on;
 expected.curve[0].a=(s16)(c->extentA0*4);
 c->distance100=c->extentA0-4-expected.bounds[scenario%3].unk08-expected.cars[0].halfSpan10E/2;
 c->mode13C=mode;c->mode140=flags;expected.page=25;expected.draw=0;
}
int main(void){
 const s32 edges[]={INT32_MIN,INT32_MAX,-65536,-1,0,1,65536};
 for(scenario=0;scenario<4096;scenario++){
  for(unsigned i=0;i<sizeof(actual);i++)((u8*)&actual)[i]=(u8)next();
  actual.context.camera24C=&actual.alternate[(scenario>>1)&1];actual.calls=0;
  for(unsigned i=0;i<3;i++)actual.table[i]=&actual.bounds[i];
  for(unsigned i=0;i<4;i++)actual.masks[i]=(s32)((scenario&4)?0xFFFFFFFFU:1U<<(scenario%32));
  expected=actual;s32 mode=edges[scenario%7],flags=edges[(scenario/7)%7];
  reference(mode,flags);InitCameraState(mode,flags);
  assert(sizeof(void*)==8&&(uintptr_t)actual.context.camera24C>UINT32_MAX);
  assert(!memcmp(&actual,&expected,sizeof(actual)));
 }
 return 0;
}
'''
ICON_PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "rr/gpu.h"
#include "rr/gpu_address.h"
#include "rr/animated_glyph.h"
#include "game/ui/badge1.h"
#include "game/ui/badgespr.h"
#include "game/menu/menu13.h"
#include "psyq/libgpu/gpudisp.h"
#include "psyq/libgpu/primattr.h"
#include "psyq/libgpu/settrans.h"
static struct {u32 before[4];u32 words[256];u32 after[4];} arena;
static GpuDmaRegion region;
/* A controlled native GPU-link boundary; original/linked MIPS execute the
 * real AddPrim. Keep both store ordering and self-alias behavior observable. */
void AddPrim(u32 *head,u32 *packet){
 u32 address;assert(GpuDmaAddressOf(packet,&address));
 *packet=(*packet&0xFF000000U)|(*head&0xFFFFFFU);
 *head=(*head&0xFF000000U)|(address&0xFFFFFFU);
}
'''
ICON_TAIL=r'''
void run_icon(const u8 *input,const u8 *wanted,u32 start,u32 head,u32 cpu,s32 x,s32 y,s32 frame){
 memset(&arena,0xA5,sizeof(arena));memcpy(arena.words,input,sizeof(arena.words));
 assert(start%4==0&&head%4==0&&start+44<=sizeof(arena.words)&&head+4<=sizeof(arena.words));
 region=(GpuDmaRegion){arena.words,cpu,sizeof(arena.words)};assert(GpuDmaSetRegions(&region,1));
 u8 *cursor=(u8*)arena.words+start;
 void *result=EmitSpinningWheelIcon((u32*)((u8*)arena.words+head),cursor,x,y,frame);
 assert((uintptr_t)cursor>UINT32_MAX&&result==cursor+44);
 assert(!memcmp(arena.words,wanted,sizeof(arena.words)));
 for(unsigned i=0;i<4;i++)assert(arena.before[i]==0xA5A5A5A5U&&arena.after[i]==0xA5A5A5A5U);
 assert(GpuDmaSetRegions(0,0));
}
'''


def signed(n):
    n &= 0xFFFFFFFF
    return n-(1<<32) if n&(1<<31) else n


def icon_oracle(initial,start,head,cpu,x,y,frame):
    data=bytearray(initial)
    def word(p,n):struct.pack_into('<I',data,p,n&0xFFFFFFFF)
    def read(p):return struct.unpack_from('<I',data,p)[0]
    def link(p):
        word(p,(read(p)&0xFF000000)|(read(head)&0xFFFFFF))
        word(head,(read(head)&0xFF000000)|((cpu+p)&0xFFFFFF))
    def mode(p,page):
        data[p+3]=2;word(p+4,0xE1000200|(page&0x9FF));word(p+8,0);link(p)
    mode(start,5)
    p=start+12;data[p+3]=4;data[p+7]=0x64
    for offset,n in ((8,x),(10,y),(16,32),(18,64)):struct.pack_into('<H',data,p+offset,n&0xFFFF)
    data[p+12]=(frame*32)&255;data[p+13]=192
    data[p+7]&=0xFD;data[p+7]|=1
    struct.pack_into('<H',data,p+14,(frame*64+0x3828)&0xFFFF);link(p)
    mode(start+32,0x9A)
    return bytes(data)


class RecoveredCallContractsTest(unittest.TestCase):
    def body(self,names):
        owners=source_owners(ROOT/'src/main/PAL')
        bodies=[definition_source(owners[n].read_text(),n) for n in names]
        for body in bodies:self.assertNotRegex(body,r'\b(?:__asm__|asm)\s*\(')
        return '\n'.join(bodies)

    def test_actual_camera_and_model_page_setter_with_full_native_owners(self):
        with TemporaryDirectory() as temporary:
            source=Path(temporary)/'camera.c';binary=source.with_suffix('')
            source.write_text(CAMERA_PREFIX+self.body(('SetModelTexturePage','InitCameraState'))+CAMERA_TAIL)
            built=subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined,bounds',
                                  '-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(source),'-o',str(binary)],capture_output=True,text=True)
            self.assertEqual(built.returncode,0,built.stderr);subprocess.run([str(binary)],check=True)

    def test_actual_icon_and_sprite_chain_native_original_and_linked(self):
        names=('get_mode','get_tw','SetDrawMode','SetSprt','SetSemiTrans','SetShadeTex',
               'AddDrawModePrim','AddSpriteWithDrawMode','EmitSpinningWheelIcon')
        with TemporaryDirectory() as temporary:
            source=Path(temporary)/'icon.c';library=source.with_suffix('.dylib')
            source.write_text(ICON_PREFIX+self.body(names)+ICON_TAIL)
            built=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                                  '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),
                                  str(source),str(ROOT/'src/native/gpu_address.c'),'-o',str(library)],capture_output=True,text=True)
            self.assertEqual(built.returncode,0,built.stderr);native=C.CDLL(str(library))
            native.run_icon.argtypes=[C.c_void_p,C.c_void_p]+[C.c_uint32]*3+[C.c_int32]*3
            machines=[]
            for label,path in (('original',ROOT/'assets/PAL/main.exe'),('linked',ROOT/'build/PAL/main.exe')):
                memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
                image=path.read_bytes();memory.mem_write(struct.unpack_from('<I',image,24)[0]&0x1FFFFFFF,image[0x800:]);machines.append((label,memory))
            rng=random.Random(0x1C0AB1);edges=(-2147483648,2147483647,-65537,-256,-9,-8,-1,0,1,7,8,255,256,65536)
            stack,stop=0x803F0000,0x803FF000;entry=address_of('EmitSpinningWheelIcon');seen=set();seen_frames=set()
            for case in range(4096):
                frame=edges[(case//2)%len(edges)] if case&1 else signed(rng.getrandbits(32))
                seen_frames.add(frame)
                x,y=signed(rng.getrandbits(32)),signed(rng.getrandbits(32));start=64+(case%8)*4
                kind=case%10;head=0 if not kind else start+(0,4,8,12,16,20,24,32,40)[kind-1]
                cpu=(0,0x80000000,0xA0000000)[case%3]|0x200000
                initial=rng.randbytes(1024);wanted=icon_oracle(initial,start,head,cpu,x,y,frame)
                native.run_icon(C.create_string_buffer(initial),C.create_string_buffer(wanted),start,head,cpu,x,y,frame);seen.add(kind)
                for label,memory in machines:
                    read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
                    write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b))
                    write(cpu-16,b'\xA5'*16);write(cpu,initial);write(cpu+1024,b'\xA5'*16);write(stack-512,b'\xA5'*640)
                    write(stack+16,struct.pack('<I',frame&0xFFFFFFFF))
                    allowed=((cpu,1024),(stack-496,516))
                    def store(machine,access,address,size,value,user):
                        address&=0x1FFFFFFF
                        self.assertTrue(any((base&0x1FFFFFFF)<=address and address+size<=(base&0x1FFFFFFF)+length for base,length in allowed),(case,label,hex(machine.pc),hex(address)))
                    machine=Cpu(memory,0,100,101,102,store);machine.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                    machine.r[4:8]=[cpu+head,cpu+start,x&0xFFFFFFFF,y&0xFFFFFFFF]
                    machine.r[29],machine.r[31],machine.pc=stack,stop,entry
                    for _ in range(3000):
                        if machine.pc==stop:break
                        machine.step()
                    self.assertEqual((machine.pc,machine.r[29],machine.r[2]),(stop,stack,cpu+start+44),(case,label))
                    self.assertEqual(read(cpu,1024),wanted,(case,label,kind))
                    self.assertEqual(read(cpu-16,16),b'\xA5'*16);self.assertEqual(read(cpu+1024,16),b'\xA5'*16)
                    self.assertEqual(read(stack-512,16),b'\xA5'*16);self.assertEqual(read(stack+20,108),b'\xA5'*108)
                    for register in (*range(16,24),28,30):self.assertEqual(machine.r[register],0xBEEF0000+register)
            self.assertEqual(seen,set(range(10)))
            self.assertTrue(set(edges)<=seen_frames)


if __name__=='__main__':unittest.main()

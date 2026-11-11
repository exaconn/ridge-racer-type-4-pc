"""Current viewport -> real drawing-area/helpers -> real DMA linker.

GTE geometry services are mutable adapters. SDK helpers and AddPrim execute their
current bodies on native complete owners and original/linked MIPS. Native DMA
regions retain actual pointers above 4 GiB. AddPrim's actual body is included
literally; a test symbol binding names it native_link for boundary observation.
"""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from test_wheel_bump_owner import Cpu

ROOT = Path(__file__).resolve().parents[2]
FRAME_PREFIX = 0x1670
BLOCKS = [('scene',0x1F800000,0x7C),('frame0',0x80140000,FRAME_PREFIX),
          ('frame1',0x80180000,FRAME_PREFIX),('packets',0x80200000,32*12),
          ('CourseSelectPreviewUvs',None,112),('ViewOffsetX',None,14),
          ('ViewOffsetY',None,14),('ViewScreenH',None,28),
          ('CourseSelectMapCoordsX',None,16),('CourseSelectMapCoordsY',None,16),
          ('CourseSelectLabelPosX',None,16),('CourseSelectLabelPosY',None,16),
          ('DrawBufIndex',None,4),('ActiveViewportIndex',None,4),
          ('RearViewMirrorY',None,4),('CurrentDrawBuf',None,4),
          ('D_800ABAC4',None,2),('D_800ABAC6',None,2)]
STATE = sum(row[2] for row in BLOCKS)

PREFIX = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "game/camera/viewcam.h"
#include "rr/rro_model.h"
#include "rr/frame_storage_types.h"
#include "rr/gpu_address.h"
#include "rr/animated_glyph.h"
#include "psyq/libgpu/gpudisp.h"
enum {FRAME_PREFIX=0x1670,STATE=RR_STATE};
typedef struct {
    u32 before[4];RroSceneScratchPrefix scene;FrameStorage frames[2];
    GpuDrawModePacket packets[32];GpuRectangleFields rectangles[2][7];
    s16 offsetX[7],offsetY[7];s32 screenH[7];
    u16 mapX[8];s16 mapY[8];u16 labelX[8];s16 labelY[8];
    s32 drawBank,active,mirror;unsigned frame;s16 width,height;u32 after[4];
} World;
typedef struct {u32 kind,args[3],result;u8 entry[STATE],body[STATE],after[STATE];} Event;
typedef struct {u32 count;u8 initial[STATE],final[STATE];Event events[6];} Capture;
static World w,before;static Capture capture;static GpuDmaRegion regions[4];
static unsigned scenario,profile;static u32 rng;
#define SCRATCHPAD (&w.scene)
#define CurrentDrawBuf (&w.frames[w.frame])
#define CourseSelectPreviewUvs (w.rectangles)
#define ViewOffsetX (w.offsetX)
#define ViewOffsetY (w.offsetY)
#define ViewScreenH (w.screenH)
#define CourseSelectMapCoordsX (w.mapX)
#define CourseSelectMapCoordsY (w.mapY)
#define CourseSelectLabelPosX (w.labelX)
#define CourseSelectLabelPosY (w.labelY)
#define DrawBufIndex (w.drawBank)
#define ActiveViewportIndex (w.active)
#define RearViewMirrorY (w.mirror)
#define D_800ABAC4 (w.width)
#define D_800ABAC6 (w.height)
static u32 next(void){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static s32 word(u32 bits){s32 value;memcpy(&value,&bits,4);return value;}
static s16 half(u16 bits){s16 value;memcpy(&value,&bits,2);return value;}
static u32 address(unsigned frame){return ((scenario&2)?0xA0140000U:0x80140000U)+frame*0x40000U;}
static u32 wire(const void *p){return GpuDmaEncodeAddress(p);}
static void put32(u8 *p,u32 value){memcpy(p,&value,4);}
static void snapshot(u8 *out){
    RroSceneScratchPrefix *s=&w.scene;u8 *start=out;
    assert(!s->modelEntries48&&!s->commands50&&!s->vertices54&&!s->normals58);
    put32(out,wire(s->view.projection.pointers.cursor));
    put32(out+4,wire(s->view.projection.pointers.order));
    memcpy(out+8,&s->view.projection.pose08,32);memcpy(out+0x28,&s->view.matrix28,32);
    put32(out+0x48,0);put32(out+0x4C,s->reserved4C);
    put32(out+0x50,0);put32(out+0x54,0);put32(out+0x58,0);
    put32(out+0x5C,s->reserved5C);put32(out+0x60,s->depthShift60);
    put32(out+0x64,s->renderFlags64);put32(out+0x68,s->reserved68);
    memcpy(out+0x6C,&s->viewport6C,8);memcpy(out+0x74,s->draw74.words,8);out+=0x7C;
    for(unsigned i=0;i<2;i++){memcpy(out,w.frames[i].bytes,FRAME_PREFIX);out+=FRAME_PREFIX;}
    memcpy(out,w.packets,sizeof(w.packets));out+=sizeof(w.packets);
    memcpy(out,w.rectangles,sizeof(w.rectangles));out+=sizeof(w.rectangles);
    memcpy(out,w.offsetX,sizeof(w.offsetX));out+=sizeof(w.offsetX);
    memcpy(out,w.offsetY,sizeof(w.offsetY));out+=sizeof(w.offsetY);
    memcpy(out,w.screenH,sizeof(w.screenH));out+=sizeof(w.screenH);
    memcpy(out,w.mapX,16);out+=16;memcpy(out,w.mapY,16);out+=16;
    memcpy(out,w.labelX,16);out+=16;memcpy(out,w.labelY,16);out+=16;
    put32(out,w.drawBank);put32(out+4,w.active);put32(out+8,w.mirror);
    put32(out+12,address(w.frame));out+=16;
    memcpy(out,&w.width,2);memcpy(out+2,&w.height,2);out+=4;
    assert(out-start==STATE);
}
static void mutate(unsigned kind){
    if(!profile)return;
    w.drawBank^=1;w.frame^=1;w.mirror=word(next());w.active^=word(next());
    w.scene.view.projection.pointers.cursor=(u8*)&w.packets[20];
    w.scene.view.projection.pointers.order=w.frames[w.frame].buffer.ordering70.words;
    w.scene.renderFlags64=word(next());
    for(unsigned i=0;i<7;i++){
        w.offsetX[i]=half((u16)next());w.offsetY[i]=half((u16)next());w.screenH[i]=word(next());
        for(unsigned bank=0;bank<2;bank++){
            GpuRectangleFields *r=&w.rectangles[bank][i];
            r->x=half((u16)next());r->y=half((u16)next());
            r->w=half((u16)next());r->h=half((u16)next());
        }
    }
    for(unsigned i=0;i<8;i++){
        w.mapX[i]=(u16)next();w.mapY[i]=half((u16)next());
        w.labelX[i]=(u16)next();w.labelY[i]=half((u16)next());
    }
    w.width=half((u16)next());w.height=half((u16)next());
    w.frames[w.frame].buffer.ordering70.words[703]^=next()^kind;
}
static Event *begin(unsigned kind,u32 a,u32 b){
    assert(capture.count<6);Event *e=&capture.events[capture.count++];
    e->kind=kind;e->args[0]=a;e->args[1]=b;e->args[2]=0;snapshot(e->entry);return e;
}
static void end(Event *e,u32 result){
    e->result=result;snapshot(e->body);
    if(e->kind!=3)mutate(e->kind);snapshot(e->after);
}
static u32 native_cs(s32,s32),native_ce(s32,s32);
static void native_area(void *,GpuRectangleFields *);
static void native_link(u32 *,u32 *);
void SetGeomOffset(s32 x,s32 y){Event *e=begin(1,x,y);end(e,0);}
void SetGeomScreen(s32 h){Event *e=begin(2,h,0);end(e,0);}
void SetDrawArea(void *p,GpuRectangleFields *r){Event *e=begin(3,wire(p),wire(r));native_area(p,r);end(e,0);}
u32 get_cs(s32 x,s32 y){Event *e=begin(4,x,y);u32 value=native_cs(x,y);end(e,value);return value;}
u32 get_ce(s32 x,s32 y){Event *e=begin(5,x,y);u32 value=native_ce(x,y);end(e,value);return value;}
void AddPrim(u32 *ot,u32 *p){Event *e=begin(6,wire(ot),wire(p));native_link(ot,p);end(e,0);}
'''

TAIL = r'''
const void *run_viewport(unsigned number){
    const s32 mirrors[]={INT32_MIN,-65537,-32769,-1,0,1,255,256,32767,32768,65535,INT32_MAX};
    const s16 limits[]={INT16_MIN,-1,0,1,320,1024,INT16_MAX};
    unsigned i;scenario=number;profile=(number>>3)&1;rng=0x3291517U^(number*0x9E3779B9U);
    for(i=0;i<sizeof(w);i++)((u8*)&w)[i]=(u8)next();
    memset(&capture,0,sizeof(capture));
    regions[0]=(GpuDmaRegion){&w.frames[0],address(0),sizeof(FrameStorage)};
    regions[1]=(GpuDmaRegion){&w.frames[1],address(1),sizeof(FrameStorage)};
    regions[2]=(GpuDmaRegion){w.packets,(scenario&2)?0xA0200000U:0x80200000U,sizeof(w.packets)};
    regions[3]=(GpuDmaRegion){w.rectangles,0x800A3C20,sizeof(w.rectangles)};
    assert(GpuDmaSetRegions(regions,4));
    w.frame=(number>>1)&1;w.drawBank=number&1;w.mirror=mirrors[(number/7)%12];
    w.width=limits[(number/3)%7];w.height=limits[(number/11)%7];
    w.scene.modelEntries48=NULL;w.scene.commands50=NULL;w.scene.vertices54=NULL;w.scene.normals58=NULL;
    w.scene.view.projection.pointers.cursor=(u8*)&w.packets[number%12];
    w.scene.view.projection.pointers.order=w.frames[w.frame].buffer.ordering70.words;
    assert(sizeof(void*)==8&&(uintptr_t)&w.scene>UINT32_MAX&&(uintptr_t)w.packets>UINT32_MAX);
    memcpy(&before,&w,sizeof(w));snapshot(capture.initial);
    SetupViewport(number%7);snapshot(capture.final);assert(capture.count==6);
    assert(!memcmp(w.before,before.before,sizeof(w.before))&&!memcmp(w.after,before.after,sizeof(w.after)));
    for(i=0;i<2;i++)assert(!memcmp(w.frames[i].bytes+FRAME_PREFIX,before.frames[i].bytes+FRAME_PREFIX,sizeof(FrameStorage)-FRAME_PREFIX));
    {RroSceneScratchPrefix s=w.scene;
     s.view.projection.pointers=before.scene.view.projection.pointers;
     s.renderFlags64=before.scene.renderFlags64;s.viewport6C=before.scene.viewport6C;
     assert(!memcmp(&s,&before.scene,sizeof(s)));}
    return &capture;
}
unsigned capture_size(void){return sizeof(capture);}
'''


class ViewportDrawAreaTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owners=source_owners(ROOT/'src/main/PAL')
        body=lambda name:definition_source(owners[name].read_text(),name)
        viewport=body('SetupViewport')
        if re.search(r'\b(?:asm|__asm__)\s*\(',viewport):raise AssertionError('Viewport still needs host-only asm removal')
        link=body('AddPrim')
        if re.search(r'\b(?:asm|__asm__)\s*\(',link):raise AssertionError('AddPrim still needs host-only asm removal')
        source=(PREFIX.replace('RR_STATE',str(STATE))+
                body('get_cs').replace('u32 get_cs(','static u32 native_cs(',1)+
                body('get_ce').replace('u32 get_ce(','static u32 native_ce(',1)+
                body('SetDrawArea').replace('void SetDrawArea(','static void native_area(',1)+
                '\n#define AddPrim native_link\n'+link+'\n#undef AddPrim\n'+viewport+TAIL)
        if source.count(link)!=1:raise AssertionError('Actual AddPrim body must remain literal')
        cls.temporary=TemporaryDirectory();cls.addClassCleanup(cls.temporary.cleanup)
        directory=Path(cls.temporary.name);c=directory/'native.c';library=directory/'native.dylib';c.write_text(source)
        built=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                              '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                              '-I',str(ROOT/'include'),str(c),str(ROOT/'src/native/gpu_address.c'),'-o',str(library)],
                             capture_output=True,text=True)
        if built.returncode:raise AssertionError(built.stderr)
        cls.lib=C.CDLL(str(library));cls.lib.run_viewport.argtypes=[C.c_uint];cls.lib.run_viewport.restype=C.c_void_p
        cls.lib.capture_size.restype=C.c_uint
        cls.capture_size=4+2*STATE+6*(20+3*STATE)
        if cls.lib.capture_size()!=cls.capture_size:raise AssertionError('Capture layout')

    def test_current_complete_pipeline_original_and_linked_mips(self):
        memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN)
        memory.mem_map(0,0x400000);memory.mem_map(0x1F800000,0x1000)
        read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
        write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b))
        blocks=[(name,address if address is not None else address_of(name),size) for name,address,size in BLOCKS]
        def snapshot():return b''.join(read(address,size) for _,address,size in blocks)
        def restore(data):
            offset=0
            for _,address,size in blocks:write(address,data[offset:offset+size]);offset+=size
        start=address_of('SetupViewport');stop=0x803FF000;stack=0x803F0000
        names=('SetGeomOffset','SetGeomScreen','SetDrawArea','get_cs','get_ce','AddPrim')
        services={address_of(name):kind for kind,name in enumerate(names,1)}
        spans={row.split()[0]:(int(row.split()[1],16),int(row.split()[2],16))
               for row in (ROOT/'configs/PAL/functions.txt').read_text().splitlines()}
        images=[(ROOT/path).read_bytes() for path in ('assets/PAL/main.exe','build/PAL/main.exe')]
        base=struct.unpack_from('<I',images[0],0x18)[0]
        def store(cpu,access,address,size,value,user):
            address&=0x1FFFFFFF
            self.assertTrue(any((a&0x1FFFFFFF)<=address and address+size<=(a&0x1FFFFFFF)+n for _,a,n in blocks) or
                            (stack&0x1FFFFFFF)-0x200<=address and address+size<=(stack&0x1FFFFFFF),
                            (scenario,hex(cpu.pc),hex(address),size))
        cpu=Cpu(memory,0,100,101,102,store)
        profiles,viewports,mirror_inputs=set(),set(),set()
        for scenario in range(336):
            raw=C.string_at(self.lib.run_viewport(scenario),self.capture_size)
            count=struct.unpack_from('<I',raw)[0];self.assertEqual(count,6)
            initial=raw[4:4+STATE];final=raw[4+STATE:4+2*STATE]
            events=[]
            for i in range(6):
                pos=4+2*STATE+i*(20+3*STATE);kind,*values=struct.unpack_from('<5I',raw,pos);pos+=20
                events.append((kind,values[:3],values[3],raw[pos:pos+STATE],raw[pos+STATE:pos+2*STATE],raw[pos+2*STATE:pos+3*STATE]))
            profiles.add((scenario>>3)&1);viewports.add(scenario%7)
            mirror_offset=sum(size for name,_,size in blocks[:14])
            mirror_inputs.add(struct.unpack_from('<I',initial,mirror_offset)[0])
            for image in images:
                for name in ('SetupViewport','SetDrawArea','get_cs','get_ce','AddPrim'):
                    address,size=spans[name];write(address,image[0x800+address-base:0x800+address-base+size])
                restore(initial);write(stack-0x210,b'\xA5'*0x220)
                cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                cpu.r[4],cpu.r[29],cpu.r[31],cpu.pc=scenario%7,stack,stop,start
                observed=0;pending=[]
                def clobber(result,resume):
                    for r in (2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25):cpu.r[r]=0xDEAD0000+r
                    cpu.r[2],cpu.hi,cpu.lo,cpu.pc=result,0x12345678,0x87654321,resume
                for instruction in range(5000):
                    if pending and cpu.pc==pending[-1][0]:
                        resume,index=pending.pop();kind,args,result,entry,body,after=events[index]
                        self.assertEqual(snapshot(),body,(scenario,kind,'actual helper output'))
                        if kind in (4,5):self.assertEqual(cpu.r[2],result,(scenario,kind,'scalar result'))
                        if kind!=3:restore(after);clobber(result,resume)
                        else:self.assertEqual(body,after)
                        continue
                    if cpu.pc==stop:break
                    if cpu.pc in services:
                        kind=services[cpu.pc];self.assertLess(observed,6)
                        expected_kind,args,result,entry,body,after=events[observed]
                        self.assertEqual(kind,expected_kind,(scenario,observed))
                        self.assertEqual(cpu.r[4],args[0]);self.assertEqual(cpu.r[5],args[1]) if kind!=2 else None
                        self.assertEqual(snapshot(),entry,(scenario,kind,'call entry'))
                        index=observed;observed+=1;resume=cpu.r[31]
                        if kind in (1,2):
                            self.assertEqual(entry,body);restore(after);clobber(0,resume);continue
                        pending.append((resume,index))
                    cpu.step()
                self.assertEqual(cpu.pc,stop,scenario);self.assertFalse(pending)
                self.assertEqual(observed,6);self.assertEqual(snapshot(),final,(scenario,'final owners'))
                self.assertEqual(cpu.r[29],stack);self.assertEqual(cpu.r[31],stop)
                for r in (*range(16,24),28,30):self.assertEqual(cpu.r[r],0xBEEF0000+r)
                self.assertEqual(read(stack-0x210,16),b'\xA5'*16);self.assertEqual(read(stack,16),b'\xA5'*16)
        self.assertEqual(profiles,{0,1});self.assertEqual(viewports,set(range(7)))
        self.assertIn(0x80000000,mirror_inputs);self.assertIn(0x7FFFFFFF,mirror_inputs)


if __name__=='__main__':
    unittest.main()

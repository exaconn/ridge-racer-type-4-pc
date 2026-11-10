"""Typed track-object loading, matrix helpers and animated dispatcher caller.

The animated dispatch path executes its actual C callee and SDK packet functions.
Other dispatcher branches abort at their service boundaries in this fixture.
The existing animated-owner fixture retains its broader callee case matrix.
"""
import ctypes as C
from pathlib import Path
import re
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from test_gte_matrix_products import adapted_body
from test_animated_track_owners import PREFIX as BASE_PREFIX, TAIL as BASE_TAIL, ParticleCpu, next_word, digest, word, signed, half
from test_view_lighting_pipeline import Hardware

ROOT=Path(__file__).resolve().parents[2]
ARENA,ORDER,CAMERA,DRAW=0x80200000,0x80240000,0x1F800000,0x80260000
FILE,STATES,FRAME,MATRIX,ANGLES=0x802A0000,0x802B0000,0x802C0000,0x802D0000,0x802E0000
STACK,STOP=0x803F0000,0x803FF000
FILE_SIZE,STATE_SIZE=16384,40*44
NAMES=['InitAnimatedTrackObjects','SetAnimatedTrackObjectState','SetupAnimatedObjectMatrices','DrawAnimatedTrackObject',
       'SetLineG2','CatPrim','AddPrims','DpqColor','DrawTrackObjects','BuildObjectMatrix','SetTrackObjectTransform',
       'LoadTrackObjects','AddPrim','SetDrawMode','get_mode','get_tw','ApplyRotMatrix']
SERVICES=['BuildRotMatrixY','BuildRotMatrixX','BuildRotMatrixZ','MulMatrix2','MulMatrix0','MulMatrix','ApplyMatrixLV','ApplyMatrixSV',
          'SetTransMatrix','SetRotMatrix','PsyqRand','PsyqSrand','VSync','SquareRoot0','Atan2Table','func_8006DB0C','PsyqMemset',
          'PsyqBzero','PlaySfxVolPan']
EXTRA=r'''
MatrixWords32 CameraViewMatrix;
PreviewMatrixWorkspace LightMatrix;
s32 TrackObjectCount;
S_ECB0 *TrackObjectDefs,*TrackObjMeshTablePtr;
S_ECB4 *TrackObjectStates,*TrackObjInstanceStates;
Node24 *TrackObjectKeyframes,*TrackObjSplineNodes;
TrackSample3C *TrackNodes;
TrackLighting2C *TrackLightTable;
SVec CourseLightColors[27];CVec OpeningSkipHoldTimer[4];
s32 OpeningSunbeamPolys[4],OpeningCreditTextTable[4],ReplayState,ModeFrame,RaceState,TrackObjCullDistance,TrackObjVisibleFrame[4];
s16 CourseIndex,GameModeType,RaceFinishBannerTimer;
s8 RacePaused;
Vec16 TrackObjWorldOrigin;
ObjectTrailPoint16 TrackObjectTrailPoints[16];
static struct {u8 before[16];u32 data[4096];u8 after[16];} fileStorage;
static struct {u8 before[16];S_ECB4 values[40];u8 after[16];} stateStorage;
static struct {u8 before[16];u32 words[1024];u8 after[16];} frameStorage;
static MatrixWords32 helperMatrix;
static s16 helperAngles[4];
static unsigned operation,currentRows;
static u32 control[8],vectorData[2],macs[3],irs[3];
static void load_rotation(const void *matrix){
 const u8 *p=matrix;for(unsigned i=0;i<4;i++)control[i]=get32(p+i*4);
 s16 last;memcpy(&last,p+16,2);control[4]=(u32)(s32)last;
}
static void load_translation(const void *matrix){memcpy(control+5,(const u8*)matrix+20,12);}
static void load_vector(const void *input){
 const u8 *p=input;vectorData[0]=get32(p);s16 z;memcpy(&z,p+4,2);vectorData[1]=(u32)(s32)z;
}
static void rotate_vector(void){
 s16 v[3]={signed16((u16)vectorData[0]),signed16((u16)(vectorData[0]>>16)),signed16((u16)vectorData[1])};
 for(unsigned row=0;row<3;row++){
  int64_t sum=0;for(unsigned col=0;col<3;col++){unsigned n=row*3+col;sum+=(int64_t)signed16((u16)(control[n/2]>>((n&1U)*16)))*v[col];}
  int64_t out=sum>=0?sum/4096:-((-sum+4095)/4096);macs[row]=(u32)out;
  if(out>32767)out=32767;if(out<-32768)out=-32768;irs[row]=(u32)out;
 }
}
#define RR_GTE_SETROT(p) load_rotation(p)
#define RR_GTE_SETTR_AT20(p) load_translation(p)
#define RR_GTE_LDV0_2W(p) load_vector(p)
#define RR_GTE_MVMVA_ROT_V0_1NOP() rotate_vector()
#define RR_GTE_SWC2_TRIPLE_AT(r0,o0,r1,o1,r2,o2,p) do {assert((r0)==9&&(r1)==10&&(r2)==11);put32((u8*)(p)+(o0),irs[0]);put32((u8*)(p)+(o1),irs[1]);put32((u8*)(p)+(o2),irs[2]);} while(0)
#define RR_GTE_COPY_ROT_TR_TO_LLM_BK() assert(!"uncovered dispatcher GTE branch")
#define RR_GTE_SET_IDENTITY_ROT_ZERO_TR(x) do {(void)(x);assert(!"uncovered dispatcher GTE branch");} while(0)
#define RR_GTE_SETROT_TR(p) do {(void)(p);assert(!"uncovered dispatcher GTE branch");} while(0)
MatrixWords32 *MulMatrix0(const MatrixWords32 *a,const MatrixWords32 *b,MatrixWords32 *out){
 u32 x=hash(a,18),y=hash(b,18);event(16,x,y,a==b,0);for(unsigned i=0;i<9;i++){x=next(x+y+i);out->halves[i]=(u16)x;}return out;
}
void PsyqBzero(void *buffer,s32 bytes){
 assert(bytes>=0&&bytes<=(s32)sizeof(stateStorage.values));
 event(17,pointer(buffer),(u32)bytes,pointer(TrackObjectDefs),pointer(TrackObjectKeyframes));
 event(23,(u32)TrackObjectCount,pointer(TrackObjectStates),pointer(TrackObjMeshTablePtr),pointer(TrackObjSplineNodes));
 memset(buffer,0,(unsigned)bytes);
 if(number&16U){TrackObjectCount--;TrackObjectStates=stateStorage.values+1;}
}
void PlaySfxVolPan(s32 sound,s16 volume,s16 pan,s32 mode){
 event(18,(u32)sound,(u32)volume,(u32)pan,(u32)mode);
 if(number&32U){TrackObjectCount=1;RacePaused=1;}
}
/* Fail loudly if a generated case leaves the explicitly covered dispatch path. */
#define UNREACHED() assert(!"uncovered dispatcher service")
Vec3i *ApplyMatrix(const MatrixWords32 *a,const Vec3s8 *b,Vec3i *c){(void)a;(void)b;(void)c;UNREACHED();return c;}
void SetBackColor(s32 a,s32 b,s32 c){(void)a;(void)b;(void)c;UNREACHED();}
void SetColorMatrix(const MatrixWords32 *a){(void)a;UNREACHED();}
void SetLightMatrix(const MatrixWords32 *a){(void)a;UNREACHED();}
void SubmitModel(RroSceneScratchPrefix *p,s32 a,s32 b,s32 c){(void)p;(void)a;(void)b;(void)c;UNREACHED();}
void SubmitCourseModel2(void *p,s32 a){(void)p;(void)a;UNREACHED();}
s32 rsin(s32 x){(void)x;UNREACHED();return 0;}
s32 rcos(s32 x){(void)x;UNREACHED();return 0;}
MatrixWords32 *RotMatrix(const Vec3sFields8 *a,MatrixWords32 *b){(void)a;(void)b;UNREACHED();return 0;}
void RotTrans(int *a,int *b,int *c){(void)a;(void)b;(void)c;UNREACHED();}
s32 ratan2(s32 a,s32 b){(void)a;(void)b;UNREACHED();return 0;}
void DrawTrackObjectLitModel(int a,int b){(void)a;(void)b;UNREACHED();}
void DrawTrackObjectBillboard(int a){(void)a;UNREACHED();}
void DrawTrackObjectFlashQuads(int a){(void)a;UNREACHED();}
void SelectModelBank(s32 a){(void)a;UNREACHED();}
s32 CordicSqrt(s32 a){(void)a;UNREACHED();return 0;}
s32 FindTrackNodeAtXZ(s32 a,s32 b){(void)a;(void)b;UNREACHED();return 0;}
void SetSoundChannelVolLR(s32 a,s16 b,s16 c){(void)a;(void)b;(void)c;UNREACHED();}
void EmitSemiTransG3List(SemiTransTri38 *a,s32 b,s32 c){(void)a;(void)b;(void)c;UNREACHED();}
void EmitSemiTransG4List(SemiTransQuad48 *a,s32 b,s32 c){(void)a;(void)b;(void)c;UNREACHED();}
'''
TAIL=r'''
static void install_extra_regions(void){
 static GpuDmaRegion regions[6];
 regions[0]=(GpuDmaRegion){arena->particles,0x80200000U,45900};regions[1]=(GpuDmaRegion){arena->packets,0x8020B824U,51000+colorLength};regions[2]=(GpuDmaRegion){ordering.words,0x80240000U,4096};
 regions[3]=(GpuDmaRegion){fileStorage.data,0x802A0000U,16384};regions[4]=(GpuDmaRegion){stateStorage.values,0x802B0000U,sizeof(stateStorage.values)};regions[5]=(GpuDmaRegion){frameStorage.words,0x802C0000U,4096};assert(GpuDmaSetRegions(regions,6));
}
unsigned dispatch_size(void){return output_size()+16384+sizeof(stateStorage.values)+4096+32+8+32+64+64;}
void dispatch_capture(u8 *out){
 capture(out);
 if(operation==3){u8 *scene=out+output_size()-196-456;put32(scene+0x74,pointer(camera.scene.draw74.draw.primitive));put32(scene+0x78,(u32)camera.scene.draw74.draw.counter);}
 out+=output_size();memcpy(out,fileStorage.data,16384);out+=16384;memcpy(out,stateStorage.values,sizeof(stateStorage.values));out+=sizeof(stateStorage.values);memcpy(out,frameStorage.words,4096);out+=4096;
 if(operation==2){memcpy(out,&helperMatrix,18);memset(out+18,0,14);}else memcpy(out,&helperMatrix,32);out+=32;memcpy(out,helperAngles,8);out+=8;memcpy(out,&CameraViewMatrix,32);out+=32;
 memcpy(out,control,32);memcpy(out+32,vectorData,8);memcpy(out+40,macs,12);memcpy(out+52,irs,12);out+=64;
 memset(out,0,64);put32(out,(u32)TrackObjectCount);put32(out+4,pointer(TrackObjectDefs));put32(out+8,pointer(TrackObjectStates));put32(out+12,pointer(TrackObjectKeyframes));put32(out+16,pointer(TrackObjMeshTablePtr));put32(out+20,pointer(TrackObjSplineNodes));put32(out+24,pointer(TrackObjInstanceStates));put32(out+28,(u32)ReplayState);put32(out+32,(u32)(s32)GameModeType);put32(out+36,(u32)(s32)RaceFinishBannerTimer);put32(out+40,(u32)(s32)RacePaused);
 for(unsigned i=0;i<16;i++)assert(fileStorage.before[i]==0xA5&&fileStorage.after[i]==0xA5&&stateStorage.before[i]==0xA5&&stateStorage.after[i]==0xA5&&frameStorage.before[i]==0xA5&&frameStorage.after[i]==0xA5);
}
void dispatch_configure(unsigned kind,unsigned n,u8 *before){
 operation=kind;install_extra_regions();u8 *scratch=malloc(output_size());assert(scratch);configure(2,n,scratch);free(scratch);
 memset(&fileStorage,0xA5,sizeof(fileStorage));memset(&stateStorage,0xA5,sizeof(stateStorage));memset(&frameStorage,0xA5,sizeof(frameStorage));
 for(unsigned i=0;i<4096;i++)((u8*)frameStorage.words)[i]=(u8)(n*17U+i*29U);
 TrackObjectFile *file=(TrackObjectFile*)fileStorage.data;currentRows=kind==1?n%33U:n%9U;file->count=(s32)currentRows;
 for(unsigned i=0;i<currentRows;i++){
  S_ECB0 *d=&file->definitions[i];memset(d,0,sizeof(*d));d->f0=(s32)(n%5U);d->f4=(s32)(5U+(i+n)%3U*10U);
  S_ECB4 *s=&stateStorage.values[i];memset(s,0,sizeof(*s));s->f24=((n+i)%4U)?1:0;s->f8=((n+i)%5U)?1:0;s->f4=0;s->f0=((n+i)&1U)?100:99;
 }
 TrackObjectCount=(s32)currentRows;TrackObjectDefs=file->definitions;TrackObjectStates=stateStorage.values;TrackObjectKeyframes=(Node24*)(file->definitions+currentRows);
 TrackObjMeshTablePtr=file->definitions+1;TrackObjSplineNodes=TrackObjectKeyframes+1;TrackObjInstanceStates=stateStorage.values+1;
 ReplayState=(n&1U)?2:0;GameModeType=(s16)(n/2U%3U);RaceFinishBannerTimer=(s16)(n/3U%9U);RacePaused=(s8)(n/5U%2U);
 for(unsigned i=0;i<5;i++){arena->objects[i].initialized10=255;arena->objects[i].frame14=0;arena->objects[i].startFrame28=32;arena->objects[i].endFrame2C=181;}
 camera.scene.view.projection.pointers.cursor=(u8*)frameStorage.words;draw.scene.view.projection.pointers.cursor=(u8*)frameStorage.words;
 u32 seed=n+0xA345U;for(unsigned i=0;i<8;i++){seed=next(seed);CameraViewMatrix.w[i]=signed32(seed);seed=next(seed);helperMatrix.w[i]=signed32(seed);seed=next(seed);camera.scene.view.matrix28.w[i]=signed32(seed);}
 for(unsigned i=0;i<4;i++)helperAngles[i]=signed16((u16)(n*1717U+i*12345U));
 if(kind==3){
  s32 *fields[3]={&stateStorage.values[0].fC,&stateStorage.values[0].f10,&stateStorage.values[0].f14};
  for(unsigned i=0;i<3;i++){u32 value=next(n*0x17171717U+i);*fields[i]=signed32(value);camera.scene.view.projection.pose08.position00.words[i]=signed32(next(value));}
  camera.scene.draw74.draw.primitive=frameStorage.words+29;camera.scene.draw74.draw.counter=signed32(n*0x17171717U);
 }
 if(kind==1)for(unsigned i=0;i<sizeof(stateStorage.values);i++)((u8*)stateStorage.values)[i]=(u8)(n*13U+i*17U);
 memset(control,0,sizeof(control));memset(vectorData,0,sizeof(vectorData));memset(macs,0,sizeof(macs));memset(irs,0,sizeof(irs));dispatch_capture(before);
}
void dispatch_invoke(unsigned kind,unsigned n,u8 *out,u32 *trace){
 s32 result=0;
 if(kind==1)result=LoadTrackObjects(fileStorage.data,stateStorage.values);
 else if(kind==2)BuildObjectMatrix(0,helperAngles,&helperMatrix,(s32)(n%4U));
 else if(kind==3)SetTrackObjectTransform(0,&helperMatrix,(s32)(n%4U));
 else if(kind==4)DrawTrackObjects((s32)(n%3U));
 else assert(0);
 dispatch_capture(out);trace[0]=eventCount;memcpy(trace+1,events,eventCount*20);trace[2561]=(u32)result;
}
void dispatch_original_file(const u8 *bytes,unsigned length,u8 *before){
 assert(operation==1&&length<=sizeof(fileStorage.data));
 memcpy(fileStorage.data,bytes,length);dispatch_capture(before);
}
'''

class TrackCpu(ParticleCpu):
    def step(self):
        instruction=self.load(self.pc,4);op=instruction>>26
        rs=(instruction>>21)&31;rt=(instruction>>16)&31;rd=(instruction>>11)&31
        handled=False
        if op==18:
            if instruction==0x4A486012:self.hw.multiply();handled=True
            elif rs==6 and rd<8:self.hw.write(0,rd,self.r[rt]);handled=True
            elif rs==4 and rd<2:self.hw.write(1,rd,self.r[rt]);handled=True
            elif rs==0 and 9<=rd<=11:self.r[rt]=self.hw.data[rd];handled=True
        elif op in (50,58):
            address=word(self.r[rs]+half(instruction))
            if op==50 and rt<2:self.hw.write(1,rt,self.load(address,4));handled=True
            elif op==58 and 9<=rt<=11:self.store(address,self.hw.data[rt],4);handled=True
        if not handled:return super().step()
        destination=self.delay;self.delay=None;self.pc=word(self.pc+4 if destination is None else destination)


class TrackObjectDispatchTest(unittest.TestCase):
    def test_actual_loader_helpers_and_animated_dispatch(self):
        owners=source_owners(ROOT/'src/main/PAL')
        preamble=owners['DrawTrackObjects'].read_text().split('void DrawTrackObjects(s32 arg0) {')[0]
        # Hardware memory/assembly bind to fixture owners; instrumented rotation
        # services supply their own explicit declarations instead of K&R headers.
        preamble='\n'.join(l for l in preamble.splitlines() if l not in
                          ['#include "rr_asm.h"','#include "scratchpad.h"','#include "game/math/rotmat.h"'])
        bodies=[]
        for name in NAMES:
            body=adapted_body(owners[name].read_text(),name) if name=='ApplyRotMatrix' else definition_source(owners[name].read_text(),name)
            bodies.append(re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)','',body))
        disc=(ROOT/'disc/PAL/R4.BIN').read_bytes();sectors=struct.unpack_from('<951I',disc,4);actual_files=[]
        for course in range(9):
            data=disc[sectors[course*2+1]*2048:sectors[course*2+2]*2048];begin,end=struct.unpack_from('<2I',data,32)
            count=struct.unpack_from('<i',data,begin)[0];self.assertTrue(0<=count<=32)
            self.assertEqual((end-begin-4-count*48)%36,0);actual_files.append(data[begin:end])
        data=disc[sectors[15]*2048:sectors[16]*2048];start=struct.unpack_from('<I',data,44)[0]+0x17F5C;end=start+72
        for relative in struct.unpack_from('<18I',data,start):
            cursor=start+relative
            for _ in range(128):
                duration=struct.unpack_from('<i',data,cursor+4)[0];cursor+=8
                if duration==-1:break
                self.assertGreater(duration,0)
            else:self.fail('unterminated original color stream')
            end=max(end,cursor)
        colors=data[start:end];arena_size=0x17F5C+len(colors);base_global=arena_size+4096+456;base_size=base_global+196
        file_at=base_size;states_at=file_at+FILE_SIZE;frame_at=states_at+STATE_SIZE;matrix_at=frame_at+4096;angles_at=matrix_at+32;camera_at=angles_at+8;hardware_at=camera_at+32;meta_at=hardware_at+64
        basic=[('ColorMatrix',0,32),('HalfTransViewMatrix',32,32),('LightSourceAngles',64,24),('PolyStripTempCoords',88,6),('DrawBufIndex',96,4),
               ('TrackParticlePoolPtr',116,4),('AnimatedTrackObjPool',120,4),('TrackColorKeysPrimary',124,36),('TrackColorKeysSecondary',160,36)]
        fields=[('TrackObjectCount',0,4),('TrackObjectDefs',4,4),('TrackObjectStates',8,4),('TrackObjectKeyframes',12,4),
                ('TrackObjMeshTablePtr',16,4),('TrackObjSplineNodes',20,4),('TrackObjInstanceStates',24,4),('ReplayState',28,4),('GameModeType',32,2),('RaceFinishBannerTimer',36,2),('RacePaused',40,1)]
        addresses={n:address_of(n) for n,_,_ in basic+fields};addresses['CameraViewMatrix']=address_of('CameraViewMatrix')
        starts={n:address_of(n) for n in NAMES};services={address_of(n):n for n in SERVICES}
        exe=(ROOT/'assets/PAL/main.exe').read_bytes();base=struct.unpack_from('<I',exe,24)[0];variants={'original':{}}
        with (ROOT/'build/PAL/main.elf').open('rb') as stream:
            elf=ELFFile(stream);table=elf.get_section_by_name('.symtab');code={}
            for n in NAMES:
                symbol=table.get_symbol_by_name(n)[0];section=elf.get_section(symbol['st_shndx']);offset=symbol['st_value']-section['sh_addr']
                self.assertEqual(symbol['st_value'],starts[n]);code[n]=section.data()[offset:offset+symbol['st_size']]
            variants['compiled']=code
        with TemporaryDirectory() as directory:
            source,library=Path(directory)/'native.c',Path(directory)/'native.dylib';source.write_text(BASE_PREFIX+'\n'+preamble+'\n'+EXTRA+'\n'+'\n'.join(bodies)+BASE_TAIL+TAIL)
            subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                            '-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(source),str(ROOT/'src/native/gpu_address.c'),'-o',str(library)],check=True)
            native=C.CDLL(str(library));native.prepare.argtypes=[C.c_void_p,C.c_uint];native.dispatch_configure.argtypes=[C.c_uint,C.c_uint,C.c_void_p];native.dispatch_invoke.argtypes=[C.c_uint,C.c_uint,C.c_void_p,C.c_void_p]
            native.prepare(C.create_string_buffer(colors),len(colors));size=native.dispatch_size();self.assertEqual(size,meta_at+64)
            native.dispatch_original_file.argtypes=[C.c_void_p,C.c_uint,C.c_void_p]
            before=(C.c_ubyte*size)();out=(C.c_ubyte*size)();trace=(C.c_uint32*2562)();engines=[]
            for variant,code in variants.items():
                memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000);memory.mem_map(CAMERA,0x1000)
                write=lambda a,b,m=memory:m.mem_write(a&0x1FFFFFFF,bytes(b));read=lambda a,n,m=memory:bytes(m.mem_read(a&0x1FFFFFFF,n))
                write(base,exe[0x800:])
                for name,data in code.items():write(starts[name],data)
                def store(cpu,access,address,length,value,user):
                    a=address&0x1FFFFFFF
                    allowed=[(ARENA&0x1FFFFFFF,arena_size),(ORDER&0x1FFFFFFF,4096),(CAMERA,228),(DRAW&0x1FFFFFFF,228),
                             (FILE&0x1FFFFFFF,FILE_SIZE),(STATES&0x1FFFFFFF,STATE_SIZE),(FRAME&0x1FFFFFFF,4096),(MATRIX&0x1FFFFFFF,32),
                             ((STACK-32768)&0x1FFFFFFF,32768+16),*[(addresses[n]&0x1FFFFFFF,width) for n,_,width in basic+fields]]
                    self.assertTrue(any(lo<=a and a+length<=lo+width for lo,width in allowed),(hex(address),length))
                engines.append((variant,TrackCpu(memory,0,100,101,102,store),write,read))
            def install(cpu,write,blob):
                for addr,at,width in [(ARENA,0,arena_size),(ORDER,arena_size,4096),(CAMERA,arena_size+4096,228),(DRAW,arena_size+4096+228,228),
                                      (FILE,file_at,FILE_SIZE),(STATES,states_at,STATE_SIZE),(FRAME,frame_at,4096),(MATRIX,matrix_at,32),(ANGLES,angles_at,8),
                                      (addresses['CameraViewMatrix'],camera_at,32)]:write(addr,blob[at:at+width])
                for n,offset,width in basic:write(addresses[n],blob[base_global+offset:base_global+offset+width])
                for n,offset,width in fields:write(addresses[n],blob[meta_at+offset:meta_at+offset+width])
                cpu.far=list(struct.unpack_from('<3I',blob,base_global+100));cpu.random=struct.unpack_from('<I',blob,base_global+112)[0];cpu.events=[]
                h=struct.unpack_from('<16I',blob,hardware_at);ctrl=list(h[:8])+[0]*24;values=[0]*32;values[:2]=h[8:10];values[25:28]=h[10:13];values[9:12]=h[13:16];cpu.hw=Hardware(ctrl,values)
            def capture(cpu,read,operation):
                globals_=bytearray(196);meta=bytearray(64)
                for n,offset,width in basic:globals_[offset:offset+width]=read(addresses[n],width)
                struct.pack_into('<3I',globals_,100,*cpu.far);struct.pack_into('<I',globals_,112,cpu.random)
                for n,offset,width in fields:
                    value=int.from_bytes(read(addresses[n],width),'little',signed=n in ('GameModeType','RaceFinishBannerTimer','RacePaused'));struct.pack_into('<I',meta,offset,word(value))
                matrix=read(MATRIX,18)+bytes(14) if operation==2 else read(MATRIX,32)
                h=[*cpu.hw.control[:8],*cpu.hw.data[:2],*cpu.hw.data[25:28],*cpu.hw.data[9:12]]
                return (read(ARENA,arena_size)+read(ORDER,4096)+read(CAMERA,228)+read(DRAW,228)+bytes(globals_)+
                        read(FILE,FILE_SIZE)+read(STATES,STATE_SIZE)+read(FRAME,4096)+matrix+read(ANGLES,8)+read(addresses['CameraViewMatrix'],32)+struct.pack('<16I',*h)+bytes(meta))
            def execute(cpu,write,read,name,args,number):
                cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)];cpu.r[4:4+len(args)]=args;cpu.r[29]=STACK;cpu.r[31]=STOP;cpu.pc=starts[name]
                write(STACK-33280,b'\xA5'*33792)
                def event(kind,*args):cpu.events.append((kind,*args,*([0]*(4-len(args)))))
                for _ in range(200000):
                    if cpu.pc==STOP:break
                    if cpu.pc not in services:cpu.step();continue
                    self.assertIsNone(cpu.delay);kind=services[cpu.pc];a,b,c,d=cpu.r[4:8];resume=cpu.r[31];result=0
                    if kind.startswith('BuildRotMatrix'):
                        k={'BuildRotMatrixY':1,'BuildRotMatrixX':2,'BuildRotMatrixZ':3}[kind];event(k,b);x=b^word(k*0x17171717);values=[]
                        for i in range(9):x=next_word(word(x+i+1));values.append(x&65535)
                        write(a,struct.pack('<9H',*values))
                    elif kind in ('MulMatrix2','MulMatrix','MulMatrix0'):
                        if kind=='MulMatrix0':left,right,dest,k=a,b,c,16
                        elif kind=='MulMatrix2':left,right,dest,k=a,b,b,4
                        else:left,right,dest,k=b,a,a,8
                        x=digest(read(left,18));y=digest(read(right,18));event(k,x,y,int(left==right));values=[]
                        for i in range(9):x=next_word(word(x+y+i));values.append(x&65535)
                        write(dest,struct.pack('<9H',*values));result=dest
                    elif kind in ('ApplyMatrixLV','ApplyMatrixSV'):
                        h=digest(read(a,18));values=struct.unpack('<3I' if kind=='ApplyMatrixLV' else '<3h',read(b,12 if kind=='ApplyMatrixLV' else 6));event(5 if kind=='ApplyMatrixLV' else 6,h,*[word(v) for v in values]);hashes=[h,next_word(h),next_word(next_word(h))]
                        if kind=='ApplyMatrixLV':
                            write(c,struct.pack('<3I',*[v^q for v,q in zip(values,hashes)]))
                            if number&1:
                                at=addresses['HalfTransViewMatrix']+20;old=struct.unpack('<3I',read(at,12));write(at,struct.pack('<3I',*[word(v+number*0x1234567+i) for i,v in enumerate(old,5)]))
                        else:write(c,struct.pack('<3H',*[v&65535 for v in hashes]))
                        result=c
                    elif kind=='SetTransMatrix':event(7,*struct.unpack('<3I',read(a+20,12)))
                    elif kind=='SetRotMatrix':event(9,digest(read(a,18)))
                    elif kind=='PsyqRand':cpu.random=next_word(cpu.random);event(10,cpu.random);result=cpu.random&32767
                    elif kind=='PsyqSrand':cpu.random=a;event(11,a)
                    elif kind=='VSync':result=word(number*71+13);event(12,a,result)
                    elif kind=='SquareRoot0':result=next_word(a)&32767;event(13,a,result)
                    elif kind=='Atan2Table':result=(word(a*17)^word(b*31))&4095;event(14,a,b,result)
                    elif kind=='PsyqMemset':write(a,bytes([b&255])*c);result=a
                    elif kind=='PsyqBzero':
                        g=lambda n:struct.unpack('<I',read(addresses[n],4))[0]
                        event(17,a,b,g('TrackObjectDefs'),g('TrackObjectKeyframes'));event(23,g('TrackObjectCount'),g('TrackObjectStates'),g('TrackObjMeshTablePtr'),g('TrackObjSplineNodes'));write(a,bytes(b))
                        if number&16:write(addresses['TrackObjectCount'],struct.pack('<I',word(g('TrackObjectCount')-1)));write(addresses['TrackObjectStates'],struct.pack('<I',STATES+44))
                    elif kind=='PlaySfxVolPan':
                        event(18,a,word(half(b)),word(half(c)),d)
                        if number&32:write(addresses['TrackObjectCount'],struct.pack('<I',1));write(addresses['RacePaused'],b'\x01')
                    elif kind=='func_8006DB0C':self.fail('fixture expected the initial animated frames; broader emitter paths remain covered by the preserved animated-owner matrix')
                    else:raise AssertionError(kind)
                    for r in (1,2,3,*range(4,16),24,25):cpu.r[r]=0xDEAD0000+r
                    cpu.r[2]=result;cpu.hi=0x12345678;cpu.lo=0x87654321;cpu.pc=resume
                self.assertEqual(cpu.pc,STOP,(name,number));self.assertEqual(cpu.r[29],STACK);self.assertEqual(cpu.r[31],STOP)
                for r in (*range(16,24),28,30):self.assertEqual(cpu.r[r],0xBEEF0000+r)
                # O32 reserves the caller's first16 bytes as the argument home
                # area. DrawTrackObjects spills its incoming view mode there.
                self.assertEqual(read(STACK-33280,512),b'\xA5'*512);self.assertEqual(read(STACK+16,496),b'\xA5'*496)
                return cpu.r[2]
            seen=set();flows=0
            for operation,count in [(1,96),(2,256),(3,1024),(4,1024)]:
                for number in range(count):
                    native.dispatch_configure(operation,number,before);native.dispatch_invoke(operation,number,out,trace);expected=bytes(out)
                    native_events=[tuple(trace[1+i*5:6+i*5]) for i in range(trace[0])]
                    for variant,cpu,write,read in engines:
                        install(cpu,write,bytes(before))
                        if operation==1:name,args='LoadTrackObjects',[FILE,STATES]
                        elif operation==2:name,args='BuildObjectMatrix',[0,ANGLES,MATRIX,number%4]
                        elif operation==3:name,args='SetTrackObjectTransform',[0,MATRIX,number%4]
                        else:name,args='DrawTrackObjects',[number%3]
                        result=execute(cpu,write,read,name,args,number)
                        if operation==1:self.assertEqual(result,trace[2561])
                        self.assertEqual(cpu.events,native_events,(variant,operation,number,'services'));self.assertEqual(capture(cpu,read,operation),expected,(variant,operation,number,'owners'));flows+=1
                    seen.add((operation,number%4))
            self.assertEqual(seen,{(op,mode) for op in range(1,5) for mode in range(4)})
            for course,payload in enumerate(actual_files):
                number=10000+course;native.dispatch_configure(1,number,before)
                native.dispatch_original_file(C.create_string_buffer(payload),len(payload),before);native.dispatch_invoke(1,number,out,trace)
                expected=bytes(out);native_events=[tuple(trace[1+i*5:6+i*5]) for i in range(trace[0])]
                self.assertEqual(trace[2561],struct.unpack_from('<I',payload)[0]*44)
                for variant,cpu,write,read in engines:
                    install(cpu,write,bytes(before));result=execute(cpu,write,read,'LoadTrackObjects',[FILE,STATES],number)
                    self.assertEqual(result,trace[2561]);self.assertEqual(cpu.events,native_events,(variant,'course',course))
                    self.assertEqual(capture(cpu,read,1),expected,(variant,'original course file',course));flows+=1
            print(f'2400 native loader/helper/animated-dispatch cases +9 original course object files; {flows} original/linked MIPS flows; real renderer and GPU packet bodies, full native file/state/frame pointers, live publication/counter checks, defined matrix coefficients, complete owner/ABI guards and UBSan')

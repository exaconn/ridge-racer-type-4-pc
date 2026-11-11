"""Card header producers, real save controller and file I/O on full pointers.

The independent byte model drives native C and original/linked MIPS. Formatting,
GPU readback, game-data construction and BIOS file services are controlled
boundaries. Production bodies are compiled unchanged, with full-width native pointers.
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
BUFFERS=(0x80200010,0x80205010)
DIRECTORY=0x80210010
STACK,STOP,PATH=0x803F0000,0x803FF000,0xFFFFFFFF
TITLES,EXTRAS,RECTS,RECORDS,SCALARS,NAME,TIMES=49152,49472,49984,50000,50308,50344,50360
STATE,EVENT,CALLS,FRAMES=50372,104+2*50372,8,9
EVENTS_AT=8+CALLS*32+FRAMES*(4+STATE)
NAMES=('BuildCardHeaderAnim','BuildCardHeaderStatic','CardSaveStep','CardWriteFile','CardReadFile')
SERVICES={'PsyqSprintf':(0,0),'StoreImage':(1,2),'DrawSync':(2,1),'PsyqBzero':(3,2),
          'BuildSaveData':(4,1),'FileOpen':(5,2),'FileClose':(6,1),'ClearHwCardEvents':(7,0),
          'FileWrite':(8,3),'FileRead':(9,3),'GetHwCardEvent':(10,0)}

NATIVE=r'''
#include <assert.h>
#include "rr/save_slot_types.h"
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "game/data/savedat.h"
#include "game/data/savetype.h"
#include "game/data/cardini.h"
#include "game/data/savebld.h"
#include "game/menu/savemenu.h"
#include "psyq/libc/printf.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/bios_events.h"
#include "rr/serial_handles.h"
#include "rr/overlay667_packet.h"
typedef union {CardFileHeader280 header;CardFileImage4000 image;u8 bytes[0x4000];u32 alignment;} Buffer;
typedef struct {u32 before[4];Buffer value;u32 after[4];} GuardedBuffer;
typedef struct {
 GuardedBuffer buffers[2],directory;
 char titles[5][64];CardHeaderBlock128 extras[4];GpuRectangle rectangles[2];
 SavedMenuSettings records[7];u8 step,main;s16 retries,transfer,cursor,operation;
 s32 unlocked,handle;u8 *root;char filename[16];s32 times[3];u32 after[4];
} World;
static World actual,initial;
static const u8 *trace;static unsigned count,eventCursor,scenario;
static char StrFmtPercentS[]="%s",StrCardMagicCrd0[]="CRD0",CardFilePathFmt[]="bu%1d%1d:%s";
#define SaveBuffer actual.root
#define CdDirectoryBuffer actual.directory.value.bytes
#define CardHeaderTitles actual.titles
#define CardTitleStrings (actual.titles+2)
#define CardExtraDataTitleBuf actual.extras[0]
#define CardSlotHeaderTitles (actual.extras+1)
#define SavedEmblemClutRect actual.rectangles[0]
#define SavedEmblemPixelRect actual.rectangles[1]
#define SelectionState actual.records[0]
#define SaveSlotSelections (actual.records+1)
#define SaveSlotSnapshots (actual.records+4)
#define CardSaveStepState actual.step
#define CardSaveIsMainData actual.main
#define CardSaveRetryCount actual.retries
#define CardTransferMode actual.transfer
#define GpSaveSlotCursor actual.cursor
#define CardMenuOperation actual.operation
#define ExtraModeUnlockedFlag actual.unlocked
#define CardFileHandle actual.handle
#define CardTargetFileName actual.filename
#define GarageExtraDefaultTimes actual.times
static u32 get(const u8*p){u32 v;memcpy(&v,p,4);return v;}
static u32 token(const void *p,int stack){
 uintptr_t a=(uintptr_t)p;assert(a>UINT32_MAX);
 for(unsigned i=0;i<2;i++){uintptr_t b=(uintptr_t)actual.buffers[i].value.bytes;if(a>=b&&a-b<0x4000)return @BUF0@+i*0x5000U+(u32)(a-b);}
 uintptr_t b=(uintptr_t)actual.directory.value.bytes;if(a>=b&&a-b<0x4000)return @DIRECTORY@+(u32)(a-b);
 b=(uintptr_t)actual.titles;if(a>=b&&a-b<320)return @TITLE@+(u32)(a-b);
 b=(uintptr_t)actual.extras;if(a>=b&&a-b<512)return @EXTRA@+(u32)(a-b);
 if(p==actual.filename)return @FILENAME@;
 if(p==StrCardMagicCrd0)return @MAGIC@;
 assert(stack);return 0xFFFFFFFFU;
}
static void *resolve(u32 a){
 for(unsigned i=0;i<2;i++)if(a>=@BUF0@+i*0x5000U&&a-(@BUF0@+i*0x5000U)<0x4000)return actual.buffers[i].value.bytes+(a-(@BUF0@+i*0x5000U));
 if(a>=@TITLE@&&a-@TITLE@<320)return (u8*)actual.titles+(a-@TITLE@);
 if(a>=@EXTRA@&&a-@EXTRA@<512)return (u8*)actual.extras+(a-@EXTRA@);
 if(a==@FILENAME@)return actual.filename;
 assert(!"unbound input pointer");return NULL;
}
static void load(World*w,const u8*p){
 for(unsigned i=0;i<2;i++)memcpy(w->buffers[i].value.bytes,p+i*0x4000,0x4000);
 memcpy(w->directory.value.bytes,p+32768,0x4000);memcpy(w->titles,p+49152,320);
 memcpy(w->extras,p+49472,512);memcpy(w->rectangles,p+49984,16);memcpy(w->records,p+50000,308);
 w->step=(u8)get(p+50308);w->main=(u8)get(p+50312);w->retries=(s16)get(p+50316);
 w->transfer=(s16)get(p+50320);w->cursor=(s16)get(p+50324);w->operation=(s16)get(p+50328);
 w->unlocked=(s32)get(p+50332);w->handle=(s32)get(p+50336);
 u32 root=get(p+50340);assert(root<2);w->root=actual.buffers[root].value.bytes;
 memcpy(w->filename,p+50344,16);memcpy(w->times,p+50360,12);
}
static void check(const u8*p){World wanted;memcpy(&wanted,&initial,sizeof(wanted));load(&wanted,p);assert(!memcmp(&wanted,&actual,sizeof(wanted)));}
static const u8 *begin(u32 kind,const u32 args[8]){
 assert(eventCursor<count);const u8 *e=trace+@EVENTS@+eventCursor*@EVENT@;
 assert(get(e)==kind&&!memcmp(e+4,args,32));check(e+104);return e;
}
static s32 finish(const u8*e){load(&actual,e+104+@STATE@);check(e+104+@STATE@);eventCursor++;return(s32)get(e+36);}
static void output_check(const u8*e,void*p,unsigned n){u32 a=token(p,0);unsigned at;
 if(a>=@BUF0@&&a-@BUF0@<0x4000)at=a-@BUF0@;
 else {assert(a>=@BUF0@+0x5000U&&a-(@BUF0@+0x5000U)<0x4000);at=0x4000+a-(@BUF0@+0x5000U);}
 assert(!memcmp(p,e+104+@STATE@+at,n));
}
s32 PsyqSprintf(s8*dst,const s8*fmt,...){
 va_list ap;va_start(ap,fmt);u32 args[8]={token(dst,strcmp((const char*)fmt,"%s")!=0)};
 char result[64]={0};
 if(!strcmp((const char*)fmt,"%s")){const char*s=va_arg(ap,const char*);args[1]=@FORMAT@;args[2]=token(s,0);assert(strlen(s)<64);memcpy(result,s,strlen(s)+1);}
 else {s32 port=va_arg(ap,s32),slot=va_arg(ap,s32);const char*s=va_arg(ap,const char*);args[1]=@PATHFMT@;args[2]=port;args[3]=slot;args[4]=token(s,0);int n=snprintf(result,sizeof(result),(const char*)fmt,port,slot,s);assert(n>=0&&n<32);}
 va_end(ap);const u8*e=begin(0,args);assert(!memcmp(result,e+40,64));memmove(dst,result,strlen(result)+1);
 if(args[0]!=0xFFFFFFFFU)output_check(e,dst,(unsigned)strlen(result)+1);
 return finish(e);
}
void StoreImage(GpuRectangle*r,void*p){
 unsigned id=r==&actual.rectangles[1];assert(r==&actual.rectangles[id]);unsigned n=(u16)r->rect.w*(u16)r->rect.h*2U;
 u32 args[8]={id?@RECT1@:@RECT0@,token(p,0),n,r->words[0],r->words[1]};const u8*e=begin(1,args);
 for(unsigned i=0;i<n;i++)((u8*)p)[i]=(u8)(scenario*17U+eventCursor*29U+i*3U+(u32)r->rect.x*5U+(u32)r->rect.y*7U);
 output_check(e,p,n);finish(e);
}
void DrawSync(s32 mode){u32 a[8]={mode};finish(begin(2,a));}
void PsyqBzero(void*p,s32 n){u32 a[8]={token(p,0),n};const u8*e=begin(3,a);memset(p,0,n);output_check(e,p,n);finish(e);}
void BuildSaveData(int mode){u8*p=SaveBuffer+0x280;u32 a[8]={mode,token(SaveBuffer,0)};const u8*e=begin(4,a);for(unsigned i=0;i<0x1AA8-0x280;i++)p[i]=(u8)(scenario*11U+eventCursor*7U+i);output_check(e,p,0x1AA8-0x280);finish(e);}
s32 FileOpen(const char*path,s32 mode){u32 a[8]={token(path,1),mode};const u8*e=begin(5,a);assert(!memcmp(path,e+40,strlen(path)+1));return finish(e);}
s32 FileClose(s32 fd){u32 a[8]={fd};return finish(begin(6,a));}
void ClearHwCardEvents(void){u32 a[8]={0};finish(begin(7,a));}
s32 FileWrite(s32 fd,const void*p,s32 n){u32 sum=0;for(s32 i=0;i<n;i++)sum+=((const u8*)p)[i];u32 a[8]={fd,token(p,0),n,sum};return finish(begin(8,a));}
s32 FileRead(s32 fd,void*p,s32 n){u32 a[8]={fd,token(p,0),n};const u8*e=begin(9,a);if((s32)get(e+36)!=0){for(s32 i=0;i<n;i++)((u8*)p)[i]=(u8)(scenario*31U+eventCursor*13U+(u32)i);output_check(e,p,n);}return finish(e);}
s32 GetHwCardEvent(void){u32 a[8]={0};return finish(begin(10,a));}
'''

TAIL=r'''
void run_pipeline(const u8*input){
 trace=input;count=get(input);scenario=get(input+4);eventCursor=0;assert(count<=64);
 memset(&actual,0xA5,sizeof(actual));memcpy(&initial,&actual,sizeof(initial));
 const u8*frames=input+8+8*32;load(&actual,frames+4);check(frames+4);
 for(unsigned i=0;i<8;i++){
  const u8*call=input+8+i*32;u32 id=get(call);s32 a[7];memcpy(a,call+4,28);s32 result=0;
  switch(id){
  case 0:BuildCardHeaderAnim(resolve((u32)a[0]),resolve((u32)a[1]),a[2],a[3],a[4],a[5]);break;
  case 1:BuildCardHeaderStatic(resolve((u32)a[0]),resolve((u32)a[1]),a[2],a[3],a[4],a[5],resolve((u32)a[6]));break;
  case 2:result=CardSaveStep((s16)a[0],a[1]);break;
  case 3:result=CardWriteFile(a[0],a[1],resolve((u32)a[2]),resolve((u32)a[3]),a[4],a[5]);break;
  case 4:result=CardReadFile(a[0],a[1],resolve((u32)a[2]),resolve((u32)a[3]),a[4]);break;
  default:assert(!"unknown entry");
  }
  const u8*frame=frames+(i+1)*(4+@STATE@);if(id>=2)assert(result==(s32)get(frame));check(frame+4);
 }
 assert(eventCursor==count);
}
'''


def s16(v):return C.c_int16(v).value
def s32(v):return C.c_int32(v).value


class Model:
    def __init__(self,state,number,addresses):
        self.state=bytearray(state);self.number=number;self.a=addresses;self.events=[];self.writes=self.reads=0;self.path=b''
    def word(self,at):return struct.unpack_from('<i',self.state,at)[0]
    def put(self,at,v):struct.pack_into('<I',self.state,at,v&0xFFFFFFFF)
    def scalar(self,i):return self.word(SCALARS+i*4)
    def set(self,i,v):self.put(SCALARS+i*4,v)
    def offset(self,address):
        for i,base in enumerate(BUFFERS):
            if base<=address<base+0x4000:return i*0x4000+address-base
        if self.a['CardHeaderTitles']<=address<self.a['CardHeaderTitles']+320:return TITLES+address-self.a['CardHeaderTitles']
        if self.a['CardExtraDataTitleBuf']<=address<self.a['CardExtraDataTitleBuf']+512:return EXTRAS+address-self.a['CardExtraDataTitleBuf']
        if address==self.a['CardTargetFileName']:return NAME
        raise AssertionError(hex(address))
    def text(self,address):
        if address==self.a['StrCardMagicCrd0']:return b'CRD0'
        at=self.offset(address);return bytes(self.state[at:]).split(b'\0',1)[0]
    def effect(self,kind,n):
        if self.number&1:
            self.set(8,(self.scalar(8)+1)%2);self.set(3,(0,0x10,0x11,0x20)[(self.number+n)%4])
            self.set(4,(self.number+n)%3);self.set(6,n&1);self.set(5,(3,6,7)[n%3])
        if self.number&2:
            for i in range(16):self.state[RECTS+i]^=(n*17+i*3)&255
            for i in range(512):self.state[EXTRAS+i]^=(self.number+n+i)&255
    def emit(self,kind,args=(),aux=b''):
        n=len(self.events);args=tuple(v&0xFFFFFFFF for v in args)+(0,)*(8-len(args));before=bytes(self.state);ret=0
        if kind==0:
            output=aux+b'\0';ret=len(aux)
            if args[0]==PATH:self.path=output
            else:
                at=self.offset(args[0]);self.state[at:at+len(output)]=output
        elif kind==1:
            rect=0 if args[0]==self.a['SavedEmblemClutRect'] else 1
            x,y,w,h=struct.unpack_from('<4h',self.state,RECTS+rect*8);length=w*h*2
            at=self.offset(args[1]);self.state[at:at+length]=bytes((self.number*17+n*29+i*3+x*5+y*7)&255 for i in range(length))
        elif kind==3:
            at=self.offset(args[0]);self.state[at:at+args[1]]=bytes(args[1])
        elif kind==4:
            at=self.scalar(8)*0x4000+0x280;self.state[at:at+0x1AA8-0x280]=bytes((self.number*11+n*7+i)&255 for i in range(0x1AA8-0x280))
        elif kind==5:ret=-1 if self.number%11==0 else 7+n
        elif kind==8:self.writes+=1;ret=0 if self.writes%2 else args[2]
        elif kind==9:
            self.reads+=1;ret=0 if self.reads%2 else args[2]
            if ret:
                at=self.offset(args[1]);self.state[at:at+args[2]]=bytes((self.number*31+n*13+i)&255 for i in range(args[2]))
        elif kind==10:ret=(0,1,7)[self.number%3]
        self.effect(kind,n)
        self.events.append(struct.pack('<10I',kind,*args,ret&0xFFFFFFFF)+aux.ljust(64,b'\0')+before+bytes(self.state));return ret
    def rectangle(self,rect,field,value):struct.pack_into('<H',self.state,RECTS+rect*8+field*2,value&65535)
    def transfer(self,rect,destination):
        words=struct.unpack_from('<2I',self.state,RECTS+rect*8);w,h=struct.unpack_from('<2h',self.state,RECTS+rect*8+4)
        self.emit(1,(self.a[('SavedEmblemClutRect','SavedEmblemPixelRect')[rect]],destination,w*h*2,*words));self.emit(2,(0,))
    def header(self,animated,buffer,title,px,py,x,y,extra=None):
        at=self.offset(buffer);self.state[at:at+4]=b'SC'+bytes((0x13 if animated else 0x11,1))
        self.emit(0,(buffer+4,self.a['StrFmtPercentS'],title),self.text(title))
        if not animated:
            self.state[at+0x50:at+0x52]=b'\1\0';self.emit(0,(buffer+0x52,self.a['StrFmtPercentS'],self.a['StrCardMagicCrd0']),b'CRD0')
        rect=0 if animated else 1
        for field,value in ((0,px),(2,16),(1,py),(3,1)):self.rectangle(rect,field,value)
        self.transfer(rect,buffer+0x60)
        for i in range(3 if animated else 1):
            for field,value in enumerate((s32(x+i*4),y,4,16)):self.rectangle(rect,field,value)
            self.transfer(rect,buffer+0x80+i*128)
        if not animated:
            for i in range(3):self.state[at+0x100+i*128:at+0x180+i*128]=self.state[self.offset(extra):self.offset(extra)+128]
    def io(self,write,port,slot,name,buffer,blocks,create=0):
        path=('bu%1d%1d:'%(port,slot)).encode()+self.text(name)
        self.emit(0,(PATH,self.a['CardFilePathFmt'],port,slot,name),path)
        if write and create==1:
            fd=self.emit(5,(PATH,(blocks<<16)|0x200),path)
            if fd==-1:return -1
            self.emit(6,(fd,))
        fd=self.emit(5,(PATH,0x8002 if write else 0x8001),path)
        if fd==-1:return -1
        self.set(7,fd);self.emit(7);length=blocks*8192
        while True:
            args=(fd,buffer,length)
            if write:args+= (sum(self.state[self.offset(buffer):self.offset(buffer)+length])&0xFFFFFFFF,)
            if self.emit(8 if write else 9,args):break
        self.emit(6,(fd,));return 0
    def save_step(self,init,port):
        if init>0:self.set(0,0)
        step=self.scalar(0)
        if step==0:
            self.set(2,0);self.emit(3,(BUFFERS[self.scalar(8)],0x1AA8));unlocked=int(self.scalar(6)!=0)
            if init>=1:
                self.set(1,int(init==1));mode=self.scalar(3)
                if mode<0x10:
                    self.header(False,BUFFERS[self.scalar(8)],self.a['CardHeaderTitles']+unlocked*64,0x250,0x1FC,0x254,0x1D8,self.a['CardExtraDataTitleBuf'])
                    self.emit(4,(self.scalar(3),BUFFERS[self.scalar(8)]))
                elif mode&0x10:
                    cursor=self.scalar(4);self.header(False,BUFFERS[self.scalar(8)],self.a['CardTitleStrings']+cursor*64,0x250,0x1FC,0x254,0x1D8,self.a['CardSlotHeaderTitles']+cursor*128)
                    self.emit(4,(self.scalar(3),BUFFERS[self.scalar(8)]))
                elif mode==0x20:
                    at=self.scalar(8)*0x4000;self.state[at:at+0x4000]=self.state[32768:49152]
            self.set(0,1)
        elif step==1:self.set(0,2)
        elif step==2:
            ret=self.io(True,s16(port),0,self.a['CardTargetFileName'],BUFFERS[self.scalar(8)],2 if self.scalar(3)==0x20 else 1,self.scalar(1))
            if ret==-1:
                if self.scalar(2)<3:self.set(2,s16(self.scalar(2)+1))
                else:self.set(0,5)
            else:self.set(0,3)
        elif step==3:
            ret=self.emit(10)
            if ret:
                if ret==1:
                    self.set(0,4);cursor=self.scalar(4)
                    if self.scalar(5)==3:
                        self.state[RECORDS+176+cursor*44:RECORDS+220+cursor*44]=self.state[RECORDS:RECORDS+44]
                        self.state[RECORDS+44:RECORDS+176]=self.state[RECORDS+176:RECORDS+308]
                    elif self.scalar(5) in (6,7):self.put(TIMES+cursor*4,self.word(self.scalar(8)*0x4000+0x280))
                elif self.scalar(2)>=3:self.set(0,5)
                else:self.set(0,1);self.set(2,s16(self.scalar(2)+1))
        return self.scalar(0)


class CardHeaderPipelineTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        symbols=['CardHeaderTitles','CardTitleStrings','CardExtraDataTitleBuf','CardSlotHeaderTitles','CardTargetFileName',
                 'StrFmtPercentS','StrCardMagicCrd0','CardFilePathFmt','SavedEmblemClutRect','SavedEmblemPixelRect',
                 'SelectionState','SaveSlotSelections','SaveSlotSnapshots','CardSaveStepState','CardSaveIsMainData',
                 'CardSaveRetryCount','CardTransferMode','GpSaveSlotCursor','CardMenuOperation','ExtraModeUnlockedFlag','CardFileHandle','SaveBuffer','GarageExtraDefaultTimes','CdDirectoryBuffer']
        aliases={'CardSaveIsMainData':('CardSaveStepState',1),'CardSaveRetryCount':('CardSaveStepState',2)}
        cls.a={n:(address_of(aliases[n][0])+aliases[n][1] if n in aliases else address_of(n)) for n in symbols};a=cls.a
        assert a['CardTitleStrings']==a['CardHeaderTitles']+128 and a['CardSlotHeaderTitles']==a['CardExtraDataTitleBuf']+128
        owners=source_owners(ROOT/'src/main/PAL');bodies='\n'.join(definition_source(owners[n].read_text(),n) for n in NAMES)
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup);source=Path(cls.temp.name)/'header.c';library=source.with_suffix('.dylib')
        code=NATIVE+bodies+TAIL
        replacements={'BUF0':BUFFERS[0],'DIRECTORY':DIRECTORY,'STATE':STATE,'EVENT':EVENT,'EVENTS':EVENTS_AT,
                      'TITLE':a['CardHeaderTitles'],'EXTRA':a['CardExtraDataTitleBuf'],'FILENAME':a['CardTargetFileName'],
                      'MAGIC':a['StrCardMagicCrd0'],'FORMAT':a['StrFmtPercentS'],'PATHFMT':a['CardFilePathFmt'],
                      'RECT0':a['SavedEmblemClutRect'],'RECT1':a['SavedEmblemPixelRect']}
        for key,value in replacements.items():code=code.replace('@'+key+'@',str(value)+'U')
        source.write_text(code)
        proc=subprocess.run(['cc','-std=gnu11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror','-fsanitize=undefined,bounds',
                             '-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(source),'-o',str(library)],capture_output=True,text=True)
        if proc.returncode:raise AssertionError(proc.stderr)
        cls.native=C.CDLL(str(library));cls.native.run_pipeline.argtypes=[C.c_void_p]

    def test_headers_real_save_controller_and_file_consumers_on_both_images(self):
        a=self.a
        scalar_names=['CardSaveStepState','CardSaveIsMainData','CardSaveRetryCount','CardTransferMode','GpSaveSlotCursor','CardMenuOperation','ExtraModeUnlockedFlag','CardFileHandle','SaveBuffer']
        widths=[1,1,2,2,2,2,4,4,4]
        regions=[(BUFFERS[0],16384),(BUFFERS[1],16384),(DIRECTORY,16384),(a['CardHeaderTitles'],320),(a['CardExtraDataTitleBuf'],512),
                 (a['SavedEmblemClutRect'],8),(a['SavedEmblemPixelRect'],8),(a['SelectionState'],44),(a['SaveSlotSelections'],132),(a['SaveSlotSnapshots'],132)]
        machines=[]
        for label,path in [('original',ROOT/'assets/PAL/main.exe'),('linked',ROOT/'build/PAL/main.exe')]:
            memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
            image=path.read_bytes();memory.mem_write(struct.unpack_from('<I',image,24)[0]&0x1FFFFFFF,image[0x800:]);machines.append((label,memory))
        service_addresses={address_of(n):v for n,v in SERVICES.items()};rng=random.Random(0x53434344);seen=set();save_states=set();initializations=set()
        for number in range(256):
            state=bytearray(rng.randbytes(STATE));model=Model(state,number,a)
            for i in range(5):
                text=('R4_%d_%d'%(number,i)).encode();model.state[TITLES+i*64:TITLES+(i+1)*64]=text+b'\0'+bytes(63-len(text))
            model.state[NAME:NAME+16]=b'R4TEST\0'+bytes(9)
            resume=bool(number&128)
            initial_step=(2 if number&64 else 3) if resume else 0
            initialize=0 if resume else 1+((number>>5)&1)
            initializations.add(initialize)
            for i,v in enumerate((initial_step,(number>>3)&1,(number>>2)&3,(0,0x10,0x11,0x20)[number%4],number%3,(3,6,7)[number%3],number&1,-1,number&1)):model.set(i,v)
            coords=(-2147483648,-32769,-1,0,32767,65535,2147483647)
            x=coords[number%7];y=coords[(number//7)%7]
            calls=[(0,(BUFFERS[0],a['CardHeaderTitles'],x,y,x,y)),
                   (1,(BUFFERS[1],a['CardTitleStrings']+(number%3)*64,x,y,x,y,
                       BUFFERS[1]+0x80 if number&4 else a['CardExtraDataTitleBuf'])),
                   (2,(initialize,number%4)),(2,(0,number%4)),(2,(0,number%4)),(2,(0,number%4)),
                   (3,(number%4,0,a['CardTargetFileName'],BUFFERS[number&1],1+((number>>2)&1),(number>>3)&1)),
                   (4,(number%4,0,a['CardTargetFileName'],BUFFERS[1-(number&1)],1+((number>>4)&1)))]
            frames=[struct.pack('<I',0)+bytes(model.state)]
            for entry,args in calls:
                if entry<2:model.header(entry==0,*args);ret=0
                elif entry==2:
                    save_states.add(model.scalar(0));ret=model.save_step(*args);save_states.add(ret)
                else:ret=model.io(entry==3,*args)
                frames.append(struct.pack('<I',ret&0xFFFFFFFF)+bytes(model.state))
            self.assertLessEqual(len(model.events),64)
            metadata=b''.join(struct.pack('<8I',entry,*(v&0xFFFFFFFF for v in args),*((0,)*(7-len(args)))) for entry,args in calls)
            trace=struct.pack('<2I',len(model.events),number)+metadata+b''.join(frames)+b''.join(model.events)
            self.native.run_pipeline(C.create_string_buffer(trace))
            for label,memory in machines:
                read=lambda p,n:bytes(memory.mem_read(p&0x1FFFFFFF,n))
                write=lambda p,b:memory.mem_write(p&0x1FFFFFFF,bytes(b))
                def load(data):
                    at=0
                    for p,n in regions:write(p,data[at:at+n]);at+=n
                    self.assertEqual(at,SCALARS)
                    values=struct.unpack_from('<9I',data,SCALARS)
                    for i,(name,width,value) in enumerate(zip(scalar_names,widths,values)):
                        if i==8:value=BUFFERS[value]
                        write(a[name],struct.pack('<I',value)[:width])
                    write(a['CardTargetFileName'],data[NAME:NAME+16]);write(a['GarageExtraDefaultTimes'],data[TIMES:TIMES+12]);write(a['CdDirectoryBuffer'],struct.pack('<I',DIRECTORY))
                def snapshot():
                    vals=[]
                    for i,(name,width) in enumerate(zip(scalar_names,widths)):
                        value=int.from_bytes(read(a[name],width),'little',signed=width==2)
                        if i==8:value=BUFFERS.index(value)
                        vals.append(value&0xFFFFFFFF)
                    return b''.join(read(p,n) for p,n in regions)+struct.pack('<9I',*vals)+read(a['CardTargetFileName'],16)+read(a['GarageExtraDefaultTimes'],12)
                def store(cpu,access,address,length,value,user):
                    address&=0x1FFFFFFF;allowed=regions[:2]+regions[5:]+[(a[n],w) for n,w in zip(scalar_names,widths)]+[(a['GarageExtraDefaultTimes'],12),(STACK-0x800,0x800)]
                    self.assertTrue(any((p&0x1FFFFFFF)<=address and address+length<=(p&0x1FFFFFFF)+n for p,n in allowed),(number,label,hex(cpu.pc),hex(address)))
                load(frames[0][4:]);cpu=Cpu(memory,0,100,101,102,store);event_cursor=0
                for frame,(entry,args) in enumerate(calls,1):
                    cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)];cpu.r[4:4+min(4,len(args))]=[v&0xFFFFFFFF for v in args[:4]]
                    cpu.r[29],cpu.r[31],cpu.pc=STACK,STOP,address_of(NAMES[entry]);write(STACK-0x810,b'\xA5'*0x850)
                    for i,v in enumerate(args[4:]):write(STACK+16+i*4,struct.pack('<I',v&0xFFFFFFFF))
                    for _ in range(200000):
                        if cpu.pc==STOP:break
                        if cpu.pc in service_addresses:
                            kind,argc=service_addresses[cpu.pc];self.assertLess(event_cursor,len(model.events));event=model.events[event_cursor]
                            wanted=struct.unpack_from('<10I',event);got=[cpu.r[4+i] if i<4 else int.from_bytes(read(cpu.r[29]+16+(i-4)*4,4),'little') for i in range(argc)]
                            if kind==0:
                                argc=3 if cpu.r[5]==a['StrFmtPercentS'] else 5
                                got=[cpu.r[4+i] if i<4 else int.from_bytes(read(cpu.r[29]+16+(i-4)*4,4),'little') for i in range(argc)]
                                destination=got[0]
                                if got[1]!=a['StrFmtPercentS']:got[0]=PATH
                            if kind==1:
                                packed=struct.unpack('<2I',read(got[0],8));w,h=struct.unpack('<2h',read(got[0]+4,4));got += [w*h*2,*packed]
                            if kind==4:got += [int.from_bytes(read(a['SaveBuffer'],4),'little')]
                            if kind==5:self.assertEqual(read(got[0],len(event[40:104].split(b'\0',1)[0])+1),event[40:104].split(b'\0',1)[0]+b'\0');got[0]=PATH
                            if kind==8:got += [sum(read(got[1],got[2]))&0xFFFFFFFF]
                            self.assertEqual((kind,*got,*((0,)*(8-len(got)))),wanted[:9],(number,label,entry,event_cursor))
                            self.assertEqual(snapshot(),event[104:104+STATE],(number,label,entry,event_cursor,'before'))
                            if kind==0:write(destination,event[40:104].split(b'\0',1)[0]+b'\0')
                            load(event[104+STATE:]);event_cursor+=1;seen.add(kind)
                            for reg in (2,3,*range(4,16),24,25):cpu.r[reg]=0xBAD00000+reg
                            cpu.r[2]=wanted[9];cpu.pc=cpu.r[31];continue
                        cpu.step()
                    self.assertEqual((cpu.pc,cpu.r[29]),(STOP,STACK),(number,label,entry))
                    if entry>=2:self.assertEqual(cpu.r[2],struct.unpack_from('<I',frames[frame])[0],(number,label,entry))
                    self.assertEqual(snapshot(),frames[frame][4:],(number,label,entry))
                    for reg in (*range(16,24),28,30):self.assertEqual(cpu.r[reg],0xBEEF0000+reg)
                    self.assertEqual(read(STACK-0x810,16),b'\xA5'*16)
                self.assertEqual(event_cursor,len(model.events))
        self.assertEqual(seen,set(range(11)))
        self.assertEqual(initializations,{0,1,2})
        self.assertEqual(save_states,set(range(6)))


if __name__=='__main__':unittest.main()

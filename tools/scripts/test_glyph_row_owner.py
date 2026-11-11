"""Real glyph allocation, constructor, loader, consumer and results controllers.

Production bodies are included literally, with their public types. Independent
models run against native C and original/linked MIPS. CD, rendering, rectangle,
badge and layout services are controlled boundaries, not algorithms under test.
PAL addresses are serialized only at the test boundary; native image, dialogue,
row, frame and scratch owners retain full pointers and guards.
"""
import ctypes as C
from pathlib import Path
import random
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_MIPS32, UC_MODE_LITTLE_ENDIAN
from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu as BaseCpu

ROOT=Path(__file__).resolve().parents[2]
ROWS=(0x80120010,0x80121010);PRESETS=0x80122000
FRAMES=(0x80180010,0x801C0010);FRAME_SIZE=0x22778
IMAGES=(0x80200010,0x80480010);IMAGE_SIZE=0x240000
DIALOGUES=(0x80124010,0x80125010);BADGES=(0x80126010,0x80127010)
STACK,STOP=0x807F0000,0x807FF000
NAMES=('AllocGlyphRows','InitGlyphRow','SetGlyphRowImageBase','LoadGlyphRowsStep',
       'UpdateGlyphRow','InitResultsMenuRows','NewCarModeEnter','NewCarModeUpdate')
SERVICES=('PsyqBzero','DebugPrintStub','GetR4FileIndex','CdFileReadRange',
          'CdFileBytesLoaded','InitRect16Row','UpdateRect16Row','AddJitterSprites',
          'AddTilePrim','AddTileSemiTransPrim','AddDrawModePrim',
          'AddSpriteWithDrawMode','LoadTimImage','InitBadgeRow','InitUiRow',
          'DrawUiRow','UpdateScreenFade','UpdateResultsBarSelection',
          'StepResultsMenuChoice','LoadCarRangeStep','SetMenuFrameOptions',
          'InitMenuArenas','InitCarPreviewScene')
COUNTS=(2,1,2,4,0,7,1,6,9,9,3,12,1,7,10,1,1,1,0,2,3,0,0)
SCALARS=('GlyphRowCount','ModeFrame','MenuStepDirection','MenuReturnSubmodeIndex',
         'LoadState','LoadIndex','MenuActiveDialogId','ModeGroup','ModeIndex',
         'SubmodeState','VSyncThreshold')
SIZES=(4,4,4,4,1,4,2,2,2,2,4)
ROOTS=('GlyphRows','CurrentDrawBuf','LoadCursor','MenuArenaBase','Dialogue','BadgeRows')
SIZE=2*12*28+8*20+2*0x348+2*28+11*4+6*4+8
PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "game/ui/uidraw.h"
#include "game/ui/uiwin.h"
#include "game/ui/uibar.h"
#include "game/ui/uitext.h"
#include "game/ui/badge1.h"
#include "game/ui/badgespr.h"
#include "game/ui/uipart1.h"
#include "game/gfx/sprefct.h"
#include "game/menu/mntrow.h"
#include "game/menu/mnrows.h"
#include "game/menu/menuwork.h"
#include "game/menu/mnchoice.h"
#include "game/ui/barsel0.h"
#include "game/data/ldscn1.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"
#include "game/course/trkintp2.h"
#include "rr/audio_cd_controller.h"
#include "rr/load_cursor.h"
#include "rr/menu_background.h"
#include "rr/menu_text_state.h"
#include "rr/badge_rows.h"
#include "rr/frame_storage_types.h"
#include "rr/gpu_scratch_types.h"
#include "rr/sprite_effect.h"
typedef struct {u8 before[16];MenuGlyphRow1C rows[12];u8 after[16];} RowOwner;
typedef struct {u8 before[16];FrameStorage value;u8 after[16];} FrameOwner;
typedef struct {u8 before[16];u8 data[0x240000];u8 after[16];} ImageOwner;
typedef struct {u8 before[16];MenuTextState value;u8 after[16];} DialogueOwner;
typedef struct {u8 before[16];BadgeRow1C value;u8 after[16];} BadgeOwner;
static RowOwner rows[2];static FrameOwner frames[2];static ImageOwner images[2];
static DialogueOwner dialogues[2];static BadgeOwner badges[2];
static GpuScratchPointers scratch;
#define SCRATCHPAD ((uintptr_t)&scratch)
MenuGlyphRow1C *GlyphRows;MenuGlyphPreset14 AnimatedGlyphPresets[8];
FrameStorage *CurrentDrawBuf;MenuTextState *Dialogue;BadgeRow1C *BadgeRows;
void *LoadCursor,*MenuArenaBase;
int GlyphRowCount,ModeFrame,MenuStepDirection,MenuReturnSubmodeIndex,LoadIndex,VSyncThreshold;
signed char LoadState;short MenuActiveDialogId,ModeGroup,ModeIndex,SubmodeState;
char SizeLdFmt[]="%ld",NewCar0Msg[]="new0",StrNewCarLog[]="new1";
typedef uintptr_t (*Observer)(unsigned,const uintptr_t *);
static Observer observer;
void set_observer(Observer value){observer=value;}
static uintptr_t observe(unsigned op,const uintptr_t *args){return observer(op,args);}
void PsyqBzero(void *buffer,s32 size){uintptr_t a[]={(uintptr_t)buffer,(u32)size};observe(0,a);memset(buffer,0,(size_t)size);}
int DebugPrintStub(const char *text,...){uintptr_t a[]={(uintptr_t)text};return (int)observe(1,a);}
int GetR4FileIndex(int group,int index){uintptr_t a[]={(u32)group,(u32)index};return (int)observe(2,a);}
int CdFileReadRange(int index,int first,int count,void *buffer){uintptr_t a[]={(u32)index,(u32)first,(u32)count,(uintptr_t)buffer};return (int)observe(3,a);}
int CdFileBytesLoaded(void){return (int)observe(4,0);}
void InitRect16Row(s32 index,s16 x,s16 y,s16 width,s16 height,s16 phase,s16 flag){uintptr_t a[]={(u32)index,(u32)(s32)x,(u32)(s32)y,(u32)(s32)width,(u32)(s32)height,(u32)(s32)phase,(u32)(s32)flag};observe(5,a);}
int UpdateRect16Row(int index){uintptr_t a[]={(u32)index};return (int)observe(6,a);}
u8 *AddJitterSprites(u32 *order,u8 *cursor,SpriteEffectSource *desc,s32 semi,s32 strength,s32 count){uintptr_t a[]={(uintptr_t)order,(uintptr_t)cursor,(uintptr_t)desc,(u32)semi,(u32)strength,(u32)count};return (u8 *)observe(7,a);}
void *AddTilePrim(u32 *order,void *cursor,s32 x,s32 y,s32 width,s32 height,s32 red,s32 green,s32 blue){uintptr_t a[]={(uintptr_t)order,(uintptr_t)cursor,(u32)x,(u32)y,(u32)width,(u32)height,(u32)red,(u32)green,(u32)blue};return (void *)observe(8,a);}
void *AddTileSemiTransPrim(u32 *order,void *cursor,s32 x,s32 y,s32 width,s32 height,s32 red,s32 green,s32 blue){uintptr_t a[]={(uintptr_t)order,(uintptr_t)cursor,(u32)(s32)x,(u32)(s32)y,(u32)width,(u32)height,(u32)red,(u32)green,(u32)blue};return (void *)observe(9,a);}
void *AddDrawModePrim(u32 *order,GpuDrawModePacket *cursor,u16 page){uintptr_t a[]={(uintptr_t)order,(uintptr_t)cursor,page};return (void *)observe(10,a);}
void *AddSpriteWithDrawMode(u32 *order,void *cursor,int x,int y,int width,int height,int u,int v,u16 page,u16 clut,s32 semi,s32 brightness){uintptr_t a[]={(uintptr_t)order,(uintptr_t)cursor,(u32)x,(u32)y,(u32)width,(u32)height,(u32)u,(u32)v,page,clut,(u32)semi,(u32)brightness};return (void *)observe(11,a);}
void LoadTimImage(int *words){uintptr_t a[]={(uintptr_t)words};observe(12,a);}
void InitBadgeRow(s32 index,s32 x,s16 y,s32 width,u16 height,u16 phase,u16 flags){uintptr_t a[]={(u32)index,(u32)x,(u32)(s32)y,(u32)width,height,phase,flags};observe(13,a);}
void InitUiRow(s32 index,s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 count,s32 g,s32 h,...){uintptr_t args[]={(u32)index,(u32)a,(u32)b,(u32)c,(u32)d,(u32)e,(u32)f,(u32)count,(u32)g,(u32)h};observe(14,args);}
s32 DrawUiRow(s32 index){uintptr_t a[]={(u32)index};return (s32)observe(15,a);}
void UpdateScreenFade(s32 flags){uintptr_t a[]={(u32)flags};observe(16,a);}
int UpdateResultsBarSelection(int direction){uintptr_t a[]={(u32)direction};return (int)observe(17,a);}
void StepResultsMenuChoice(void){observe(18,0);}
int LoadCarRangeStep(int first,int count){uintptr_t a[]={(u32)first,(u32)count};return (int)observe(19,a);}
void SetMenuFrameOptions(s32 a,s32 b,s32 c){uintptr_t args[]={(u32)a,(u32)b,(u32)c};observe(20,args);}
void InitMenuArenas(void){observe(21,0);}
void InitCarPreviewScene(void){observe(22,0);}
void initialize(void){
 memset(rows,0xA5,sizeof(rows));memset(frames,0xB6,sizeof(frames));memset(images,0xC7,sizeof(images));
 memset(dialogues,0xA5,sizeof(dialogues));memset(badges,0xA5,sizeof(badges));
 assert(sizeof(void *)==8&&sizeof(MenuGlyphRow1C)==32);
 assert((uintptr_t)rows>UINT32_MAX&&(uintptr_t)images>UINT32_MAX);
}
/* Explicit owner mapping only: no low native mappings or truncated pointers. */
static void *resolve(u32 address){
 if(!address)return 0;
 for(unsigned i=0;i<2;i++){
  u32 base=0x80120010+i*0x1000;
  if(address>=base&&address<=base+12*28){assert((address-base)%28==0);return rows[i].rows+(address-base)/28;}
  base=0x80180010+i*0x40000;
  if(address>=base&&address<=base+sizeof(FrameStorage))return (u8 *)&frames[i].value+(address-base);
  base=0x80200010+i*0x280000;
  if(address>=base&&address<=base+0x240000)return images[i].data+(address-base);
  if(address==0x80124010+i*0x1000)return &dialogues[i].value;
  if(address==0x80126010+i*0x1000)return &badges[i].value;
 }
 assert(0);return 0;
}
static u32 encode(const void *pointer){
 if(!pointer)return 0;
 uintptr_t p=(uintptr_t)pointer;
 for(unsigned i=0;i<2;i++){
  uintptr_t base=(uintptr_t)rows[i].rows;
  if(p>=base&&p<=base+sizeof(rows[i].rows)){assert((p-base)%sizeof(MenuGlyphRow1C)==0);return 0x80120010+i*0x1000+(p-base)/sizeof(MenuGlyphRow1C)*28;}
  base=(uintptr_t)&frames[i].value;
  if(p>=base&&p<=base+sizeof(FrameStorage))return 0x80180010+i*0x40000+p-base;
  base=(uintptr_t)images[i].data;
  if(p>=base&&p<=base+0x240000)return 0x80200010+i*0x280000+p-base;
  if(pointer==&dialogues[i].value)return 0x80124010+i*0x1000;
  if(pointer==&badges[i].value)return 0x80126010+i*0x1000;
 }
 if(pointer==SizeLdFmt)return 1;
 if(pointer==NewCar0Msg)return 2;
 if(pointer==StrNewCarLog)return 3;
 assert(0);return 0;
}
uintptr_t native_pointer(u32 address){return (uintptr_t)resolve(address);}
u32 pal_pointer(uintptr_t pointer){return encode((void *)pointer);}
static u32 get(const u8 **p){u32 v;memcpy(&v,*p,4);*p+=4;return v;}
static void put(u8 **p,u32 v){memcpy(*p,&v,4);*p+=4;}
void load_state(const u8 *p){
 for(unsigned b=0;b<2;b++)for(unsigned i=0;i<12;i++){memcpy(&rows[b].rows[i],p,24);p+=24;rows[b].rows[i].imageBase18=resolve(get(&p));}
 memcpy(AnimatedGlyphPresets,p,sizeof(AnimatedGlyphPresets));p+=sizeof(AnimatedGlyphPresets);
 for(unsigned b=0;b<2;b++){
  MenuTextState *d=&dialogues[b].value;d->p=resolve(get(&p));memcpy(&d->b4,p,28);p+=28;
  d->images20=resolve(get(&p));memcpy(&d->reserved24,p,804);p+=804;
 }
 for(unsigned b=0;b<2;b++){memcpy(&badges[b].value,p,28);p+=28;}
 GlyphRowCount=get(&p);ModeFrame=get(&p);MenuStepDirection=get(&p);MenuReturnSubmodeIndex=get(&p);
 LoadState=get(&p);LoadIndex=get(&p);MenuActiveDialogId=get(&p);ModeGroup=get(&p);ModeIndex=get(&p);SubmodeState=get(&p);VSyncThreshold=get(&p);
 GlyphRows=resolve(get(&p));CurrentDrawBuf=resolve(get(&p));LoadCursor=resolve(get(&p));MenuArenaBase=resolve(get(&p));Dialogue=resolve(get(&p));BadgeRows=resolve(get(&p));
 scratch.cursor=resolve(get(&p));scratch.order=resolve(get(&p));
}
void store_state(u8 *p){
 for(unsigned b=0;b<2;b++)for(unsigned i=0;i<12;i++){memcpy(p,&rows[b].rows[i],24);p+=24;put(&p,encode(rows[b].rows[i].imageBase18));}
 memcpy(p,AnimatedGlyphPresets,sizeof(AnimatedGlyphPresets));p+=sizeof(AnimatedGlyphPresets);
 for(unsigned b=0;b<2;b++){
  MenuTextState *d=&dialogues[b].value;put(&p,encode(d->p));memcpy(p,&d->b4,28);p+=28;
  put(&p,encode(d->images20));memcpy(p,&d->reserved24,804);p+=804;
 }
 for(unsigned b=0;b<2;b++){memcpy(p,&badges[b].value,28);p+=28;}
 put(&p,GlyphRowCount);put(&p,ModeFrame);put(&p,MenuStepDirection);put(&p,MenuReturnSubmodeIndex);
 put(&p,(s32)LoadState);put(&p,LoadIndex);put(&p,(s32)MenuActiveDialogId);put(&p,(s32)ModeGroup);put(&p,(s32)ModeIndex);put(&p,(s32)SubmodeState);put(&p,VSyncThreshold);
 put(&p,encode(GlyphRows));put(&p,encode(CurrentDrawBuf));put(&p,encode(LoadCursor));put(&p,encode(MenuArenaBase));put(&p,encode(Dialogue));put(&p,encode(BadgeRows));
 put(&p,encode(scratch.cursor));put(&p,encode(scratch.order));
}
void check_guards(int final){
 for(unsigned b=0;b<2;b++){
  for(unsigned i=0;i<16;i++)assert(rows[b].before[i]==0xA5&&rows[b].after[i]==0xA5&&dialogues[b].before[i]==0xA5&&dialogues[b].after[i]==0xA5&&badges[b].before[i]==0xA5&&badges[b].after[i]==0xA5);
  for(unsigned i=36;i<40;i++)assert(((u8 *)&dialogues[b].value)[i]==0xA5);
  for(unsigned i=852;i<sizeof(MenuTextState);i++)assert(((u8 *)&dialogues[b].value)[i]==0xA5);
  if(final){for(unsigned i=0;i<sizeof(FrameOwner);i++)assert(((u8 *)&frames[b])[i]==0xB6);for(unsigned i=0;i<sizeof(ImageOwner);i++)assert(((u8 *)&images[b])[i]==0xC7);}
 }
}
'''

def signed(value,bits=32):
 value&=(1<<bits)-1
 return value-(1<<bits)if value&(1<<(bits-1))else value
def div(a,b):return -(abs(a)//b)if a<0 else a//b
def word(value):return struct.pack('<I',value&0xFFFFFFFF)

class Cpu(BaseCpu):
 def step(self):
  instruction=self.load(self.pc,4)
  if instruction>>26==0 and instruction&63==0x1A:
   assert instruction&0xFFC0==0
   a=signed(self.r[(instruction>>21)&31]);b=signed(self.r[(instruction>>16)&31])
   assert b!=0
   quotient=div(a,b)if b>0 else -div(a,-b)
   self.lo=quotient&0xFFFFFFFF;self.hi=(a-quotient*b)&0xFFFFFFFF
   resume=self.delay;self.delay=None;self.pc=self.pc+4 if resume is None else resume
   return
  return super().step()

class State:
 def __init__(self,data):
  p=0
  def take(n):
   nonlocal p
   v=bytearray(data[p:p+n]);p+=n;return v
  self.rows=[take(12*28)for _ in range(2)];self.presets=take(160)
  self.dialogues=[take(0x348)for _ in range(2)];self.badges=[take(28)for _ in range(2)]
  self.scalars=list(struct.unpack('<11I',take(44)));self.roots=list(struct.unpack('<6I',take(24)));self.scratch=list(struct.unpack('<2I',take(8)))
  assert p==SIZE
 def snapshot(self):return b''.join([*self.rows,self.presets,*self.dialogues,*self.badges,*map(word,self.scalars),*map(word,self.roots),*map(word,self.scratch)])
 def row(self,index=0):
  for b,base in enumerate(ROWS):
   at=self.roots[0]-base+index*28
   if 0<=self.roots[0]-base<=12*28:
    assert 0<=at<=11*28;return self.rows[b],at
  raise AssertionError(('row root',self.roots[0]))
 def field(self,index,offset,fmt,value=None):
  data,at=self.row(index)
  if value is None:return struct.unpack_from('<'+fmt,data,at+offset)[0]
  if fmt in('b','h','i'):value=signed(value,struct.calcsize(fmt)*8)
  else:value&=(1<<(struct.calcsize(fmt)*8))-1
  struct.pack_into('<'+fmt,data,at+offset,value)
 def dialogue(self):return self.dialogues[DIALOGUES.index(self.roots[4])]
 def badge(self):return self.badges[BADGES.index(self.roots[5])]

def model(state,name,args,service):
 g=lambda off,fmt='B':state.field(args[0],off,fmt)
 p=lambda off,v,fmt='B':state.field(args[0],off,fmt,v)
 def call(op,values=()):return service(op,list(values),state)
 def init(index,control,glyph,preset):
  if index<signed(state.scalars[0]):
   state.field(index,2,'b',control);state.field(index,7,'B',glyph)
   table=state.presets[preset*20:(preset+1)*20]
   for offset,fmt,source in((8,'h',0),(10,'h',2),(12,'h',4),(14,'h',6),(1,'B',14),(5,'B',15),(6,'B',16),(16,'H',8),(18,'H',10),(20,'B',12),(21,'B',13)):
    state.field(index,offset,fmt,struct.unpack_from('<'+fmt,table,source)[0])
 def publish(base,first):
  for i in range(4):state.field(first+i,24,'I',base+i*0xC000)
 def results(first):
  for i in range(4):
   target=first+i;state.field(target,24,'I',state.field(i,24,'I')+0x30000);init(target,-2,0,0)
   call(5,[state.field(target,7,'B'),*(state.field(target,o,'h')for o in(8,10,12,14)),0,0])
  call(13,[0,12,304,296,88,0,0]);call(14,[0,40,72,192,32,0,0,4,3,4])
  struct.pack_into('<h',state.badge(),2,1);state.dialogue()[4]=1;state.dialogue()[7]=0
  state.scalars[1]=state.scalars[4]=0;state.roots[2]=state.roots[3]
 if name=='AllocGlyphRows':
  n=signed(state.scalars[0])*28;state.roots[0]=args[0];call(0,[args[0],n])
  b,at=state.row(0);b[at:at+n]=bytes(n);call(1,[1]);return n
 if name=='InitGlyphRow':init(*args)
 elif name=='SetGlyphRowImageBase':publish(*args)
 elif name=='LoadGlyphRowsStep':
  phase=signed(state.scalars[4],8)
  if phase==0:state.scalars[5]=0;state.scalars[4]=1;state.scalars[0]=4;phase=1
  if phase==1:
   file=call(2,[1,64]);result=call(3,[file,0,192,state.roots[2]])
   if result:state.scalars[4]=2
  elif phase==2:
   n=call(4)
   if n:publish(state.roots[2],0);state.scalars[4]=3;state.roots[2]+=signed(n)
  return int(phase==3)
 elif name=='InitResultsMenuRows':results(args[0])
 elif name=='NewCarModeEnter':
  call(16,[0x10006]);results(0);state.scalars[1]=0;state.scalars[10]=128;state.scalars[7:10]=[1,10,0]
 elif name=='NewCarModeUpdate':
  state.scalars[1]=(state.scalars[1]+1)&0xFFFFFFFF
  if signed(state.dialogue()[4],8)>=34:call(15,[0]);flags=0x10006
  else:flags=0x10007
  call(16,[flags])
  if call(17,[state.scalars[2]])and struct.unpack_from('<h',state.badge(),16)[0]==3:call(18);return
  if state.dialogue()[4]==1 and(state.scalars[3]or state.scalars[1]>=501)and state.dialogue()[6]==4:
   state.dialogue()[4]=34;struct.pack_into('<h',state.badge(),16,4);return
  if signed(state.scalars[6],16)==2 and call(19,[0,1])==1 and struct.unpack_from('<h',state.badge(),16)[0]==11:
   call(20,[0,1,0]);call(1,[2]);saved=state.roots[2];call(21);call(22)
   state.scalars[7:10]=[1,34,0];state.roots[2]=saved;call(1,[3])
 elif name=='UpdateGlyphRow':
  initial=g(2,'b');tick=g(3)
  x,y,w,h=(g(o,'h')for o in(8,10,12,14));page,clut=g(16,'H'),g(18,'H');u,v=g(20),g(21)
  caption=struct.pack('<hhHHBBBB',x,signed(y+div(h,2)-10,16),page,clut,u,(v+h)&255,w&255,16)
  image=struct.pack('<hhHHBBBB',x,y,page,clut,u,v,w&255,h&255)
  def rect():return call(6,[g(7)])
  def tile(op,slot,cursor,shade):return call(op,[state.roots[1]+112+slot*4,cursor,*(g(o,'h')for o in(8,10,12,14)),shade,shade,shade])
  def jitter(cursor,desc,strength,count):return call(7,[state.roots[1]+112,cursor,desc,1,strength,count])
  if initial==-2:p(3,0);p(2,-4,'b');p(0,0)
  elif initial==-3:pass
  elif initial==-4:call(5,[g(7),*(g(o,'h')for o in(8,10,12,14)),0,0]);p(2,-5,'b')
  elif initial==-5:
   if rect():p(2,-1,'b')
  elif initial in(-6,-7):
   if initial==-6:p(3,0);p(2,-7,'b')
   rect()
   if g(3)>=20:p(2,-1,'b');p(3,0)
   else:p(3,g(3)+1)
  elif initial==-1:
   cursor=tile(9,1,state.scratch[0],tick*124//10)
   state.scratch[0]=call(10,[state.roots[1]+116,cursor,69])
   if tick>=10:p(2,0,'b');p(3,0)
   else:p(3,g(3)+1)
   rect()
  elif initial==0:
   cursor=state.scratch[0]
   if tick>=3:cursor=jitter(cursor,caption,255,3)
   state.scratch[0]=tile(8,1,cursor,8)
   if tick>=21:p(2,1,'b');p(3,0)
   else:p(3,g(3)+1)
   if tick==1:call(12,[g(24,'I')+g(6)*g(0,'b')*2048])
  elif initial==1:
   cursor=jitter(state.scratch[0],caption,(128-tick)*2,2);cursor=jitter(cursor,image,tick,1)
   state.scratch[0]=tile(8,3,cursor,8)
   if tick>=128:p(3,0);p(2,2,'b')
   else:p(3,g(3)+8)
  else:
   cursor=state.scratch[0]
   if g(0,'b')>=0:cursor=call(11,[state.roots[1]+112,cursor,*(g(o,'h')for o in(8,10,12,14)),g(20),g(21),g(16,'H'),g(18,'H'),0,128])
   state.scratch[0]=tile(8,1,cursor,8)
   if tick>=g(5)and g(1,'b')>=2:
    p(3,0);p(2,0,'b');v=signed(g(0)+1,8);p(0,v-div(v,g(1,'b'))*g(1,'b'))
   else:p(3,g(3)+1)
  return int(initial>=0)

class GlyphRowOwnerTest(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup);d=Path(cls.temp.name)
  owners=source_owners(ROOT/'src/main/PAL');bodies=[definition_source(owners[n].read_text(),n)for n in NAMES]
  source=PREFIX+'\n'.join(bodies)
  assert all(source.count(body)==1 for body in bodies)
  (d/'glyph.c').write_text(source)
  result=subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-shared','-fPIC','-Wall','-Wextra','-Werror','-Wno-implicit-fallthrough','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(d/'glyph.c'),'-o',str(d/'glyph.so')],capture_output=True,text=True)
  if result.returncode:raise AssertionError(result.stderr)
  cls.lib=C.CDLL(str(d/'glyph.so'));cls.lib.initialize()
  cls.lib.native_pointer.argtypes=[C.c_uint32];cls.lib.native_pointer.restype=C.c_size_t
  cls.lib.pal_pointer.argtypes=[C.c_size_t];cls.lib.pal_pointer.restype=C.c_uint32
  for n in NAMES:getattr(cls.lib,n).restype=C.c_int32 if n in('AllocGlyphRows','LoadGlyphRowsStep','UpdateGlyphRow')else None
  cls.lib.AllocGlyphRows.argtypes=[C.c_void_p];cls.lib.SetGlyphRowImageBase.argtypes=[C.c_void_p,C.c_int32]
  cls.original=(ROOT/'assets/PAL/main.exe').read_bytes()[0x800:]
  with(ROOT/'build/PAL/main.elf').open('rb')as f:
   elf=ELFFile(f);cls.linked=[(s['sh_addr'],s.data())for s in elf.iter_sections()if s['sh_flags']&2 and s['sh_type']=='SHT_PROGBITS']
 def read(self,a,n):return bytes(self.u.mem_read(a&0x1FFFFFFF,n))
 def write(self,a,data):self.u.mem_write(a&0x1FFFFFFF,bytes(data))
 def load(self,state,native):
  if native:self.lib.load_state(state.snapshot());return
  for a,data in zip(ROWS,state.rows):self.write(a,data)
  self.write(address_of('AnimatedGlyphPresets'),state.presets)
  for a,data in zip(DIALOGUES,state.dialogues):self.write(a,data)
  for a,data in zip(BADGES,state.badges):self.write(a,data)
  for name,size,value in zip(SCALARS,SIZES,state.scalars):self.write(address_of(name),word(value)[:size])
  for name,value in zip(ROOTS,state.roots):self.write(address_of(name),word(value))
  self.write(0x1F800000,b''.join(map(word,state.scratch)))
 def snapshot(self,native):
  if native:
   out=C.create_string_buffer(SIZE);self.lib.store_state(out);return State(out.raw)
  data=b''.join([*(self.read(a,336)for a in ROWS),self.read(address_of('AnimatedGlyphPresets'),160),*(self.read(a,0x348)for a in DIALOGUES),*(self.read(a,28)for a in BADGES),*(word(signed(int.from_bytes(self.read(address_of(n),z),'little'),z*8))for n,z in zip(SCALARS,SIZES)),*(self.read(address_of(n),4)for n in ROOTS),self.read(0x1F800000,8)])
  return State(data)
 def boundary(self,op,args,state):
  args=[v if isinstance(v,bytes)else v&0xFFFFFFFF for v in args]
  self.events.append((op,tuple(args),state.snapshot()));serial=len(self.events)
  # Services can replace live roots. Captured descriptors/cursors must survive.
  if self.case&16 and op not in(0,2):
   b=(self.case+serial)&1;state.roots[0]=ROWS[b]+56;state.roots[1]=FRAMES[b]
   state.roots[4]=DIALOGUES[b];state.roots[5]=BADGES[b]
   state.field(self.index,3,'B',20 if serial&1 else 255)
   if op in(1,4,21,22):state.roots[2]=IMAGES[b]+0xC0000+serial*32
  if op==14:
   b=self.case&1;state.roots[4]=DIALOGUES[b];state.roots[5]=BADGES[b]
  if op==0:return 0
  if op==2:return args[1]+0x292
  if op in(3,6,17,19):return(0,1,-1)[(self.case//32)%3]&0xFFFFFFFF
  if op==4:return(0,1,0x60000,-1)[(self.case//32)%4]&0xFFFFFFFF
  if op in(7,8,9,10,11):return args[1]+(16,12,20,32)[(self.case+op)%4]
  return 0
 def service(self,op,args,native):
  if native:
   for i in({0:(0,),1:(0,),3:(3,),7:(0,1),8:(0,1),9:(0,1),10:(0,1),11:(0,1),12:(0,)}.get(op,())):args[i]=self.lib.pal_pointer(args[i])
   if op==0:
    self.assertEqual(args[1]%32,0);args[1]=args[1]//32*28
   if op==7:args[2]=C.string_at(args[2],12)
  else:
   if op==1:args[0]={address_of('SizeLdFmt'):1,address_of('NewCar0Msg'):2,address_of('StrNewCarLog'):3}[args[0]]
   if op==7:args[2]=self.read(args[2],12)
  state=self.snapshot(native);result=self.boundary(op,args,state);self.load(state,native)
  if native and op in(7,8,9,10,11):return self.lib.native_pointer(result)
  return result
 def run_mips(self,name,args):
  cpu=self.cpu;cpu.reset();cpu.pc=address_of(name);cpu.r[29]=STACK;cpu.r[31]=STOP
  saved={r:0x9ABC0000+r for r in(*range(16,24),28,30)}
  for r,v in saved.items():cpu.r[r]=v
  for i,v in enumerate(args):
   if i<4:cpu.r[i+4]=v&0xFFFFFFFF
   else:self.write(STACK+i*4,word(v))
  for _ in range(20000):
   if cpu.pc==STOP:
    self.assertEqual(cpu.r[29],STACK);self.assertTrue(all(cpu.r[r]==v for r,v in saved.items()));return signed(cpu.r[2])
   if cpu.pc in self.hooks:
    op=self.hooks[cpu.pc];values=[cpu.r[4+i]if i<4 else int.from_bytes(self.read(cpu.r[29]+i*4,4),'little')for i in range(COUNTS[op])]
    result=self.service(op,values,False);resume=cpu.r[31]
    if op==0:self.write(values[0],bytes(values[1]))
    for r in(*range(2,16),24,25):cpu.r[r]=0xDEAD0000+r
    cpu.r[2]=result&0xFFFFFFFF;cpu.pc=resume
   else:cpu.step()
  self.fail('MIPS instruction limit')
 def check_operation(self,state,name,args,errors):
  expected=State(state.snapshot());self.events=[];wanted=model(expected,name,args,self.boundary);events=self.events[:]
  for native in(False,True):
   self.load(state,native);self.events=[];actual_args=args[:]
   if native and name in('AllocGlyphRows','SetGlyphRowImageBase'):actual_args[0]=self.lib.native_pointer(args[0])
   value=getattr(self.lib,name)(*actual_args)if native else self.run_mips(name,args)
   if errors:raise errors[0]
   self.assertEqual(events,self.events);self.assertEqual(expected.snapshot(),self.snapshot(native).snapshot())
   if wanted is not None:self.assertEqual(wanted//28*32 if native and name=='AllocGlyphRows'else wanted,value)
   if native:self.lib.check_guards(0)
  return expected
 def test_pipeline_with_real_native_owners_and_pal_traces(self):
  observer_type=C.CFUNCTYPE(C.c_size_t,C.c_uint,C.POINTER(C.c_size_t));errors=[]
  def observe(op,args):
   try:return self.service(op,list(args[:COUNTS[op]]),True)
   except BaseException as e:errors.append(e);return 0
  observer=observer_type(observe);self.lib.set_observer(observer)
  for linked in(False,True):
   self.u=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);self.u.mem_map(0,0x800000);self.write(0x80010000,self.original)
   self.u.mem_map(0x1F800000,0x1000)
   if linked:
    for a,data in self.linked:self.write(a,data)
   for a in FRAMES:self.write(a,bytes([0xB6])*FRAME_SIZE)
   for a in IMAGES:self.write(a,bytes([0xC7])*IMAGE_SIZE)
   allowed=[*((a,336)for a in ROWS),*((a,0x348)for a in DIALOGUES),*((a,28)for a in BADGES),*((address_of(n),z)for n,z in zip(SCALARS,SIZES)),*((address_of(n),4)for n in ROOTS),(0x1F800000,8),(STACK-1024,1040)]
   def store(cpu,access,a,n,v,user):
    a&=0x1FFFFFFF;self.assertTrue(any((p&0x1FFFFFFF)<=a and a+n<=(p&0x1FFFFFFF)+z for p,z in allowed),(hex(a),n))
   self.cpu=Cpu(self.u,0,100,101,102,store);self.hooks={address_of(n):i for i,n in enumerate(SERVICES)}
   rng=random.Random(0x61F0A)
   for self.case in range(256):
    state=State(rng.randbytes(SIZE));state.roots=[ROWS[self.case&1]+56,FRAMES[(self.case//2)&1],IMAGES[0]+0xC0000,IMAGES[1]+0xC0000,DIALOGUES[0],BADGES[0]]
    state.scratch=[FRAMES[0]+0x1670,FRAMES[1]+112]
    state.scalars=[8,(0,500,0x7FFFFFFF,0xFFFFFFFF)[self.case%4],self.case,0,self.case%5,9,2,9,9,9,2]
    for b in range(2):
     for i in range(12):struct.pack_into('<I',state.rows[b],i*28+24,IMAGES[b]+0xC0000+i*0x1000)
     struct.pack_into('<I',state.dialogues[b],0,IMAGES[b]+0x100000);struct.pack_into('<I',state.dialogues[b],32,IMAGES[b]+0x120000)
     state.dialogues[b][4]=(1,34,0,127)[self.case%4];state.dialogues[b][6]=4
     struct.pack_into('<h',state.badges[b],16,(3,11,0)[(self.case//4)%3])
    self.index=(-2,-1,0,1,3,5)[self.case%6]
    for b in range(2):
     old=state.roots[0];state.roots[0]=ROWS[b]+56
     for off,fmt,v in((0,'B',(0,1,127,128,255)[self.case%5]),(1,'B',(0,1,2,127,128,255)[self.case%6]),(2,'b',(-7,-6,-5,-4,-3,-2,-1,0,1,2,3,-128,127)[(self.case//2)%13]),(3,'B',(0,1,2,3,9,10,20,21,127,128,255)[(self.case//3)%11]),(6,'B',self.case%3)):
      state.field(self.index,off,fmt,v)
     state.roots[0]=old
    for name in NAMES:
     with self.subTest(linked=linked,case=self.case,function=name):
      if name=='AllocGlyphRows':args=[ROWS[self.case&1]+56]
      elif name=='InitGlyphRow':args=[self.index,-2,255,self.case%8]
      elif name=='SetGlyphRowImageBase':args=[IMAGES[self.case&1]+0xC0000,0]
      elif name=='UpdateGlyphRow':args=[self.index]
      elif name=='InitResultsMenuRows':args=[(-2,0,2,4)[self.case%4]]
      else:args=[]
      self.check_operation(state,name,args,errors)
    if self.case%16==0:
     skipped=State(state.snapshot());skipped.scalars[0]=0;skipped.roots[0]=0
     self.check_operation(skipped,'InitGlyphRow',[0,-2,0,0],errors)
   # Carry actual allocator/loader output into subsequent real constructors and
   # updates. No reconstructed stand-in row is inserted between these stages.
   for scenario in range(16):
    self.case=64+scenario;self.index=scenario%4
    state.scalars[0]=4;state.scalars[4]=0;state.roots[0]=ROWS[scenario&1]+56
    state.roots[2]=IMAGES[scenario&1]+0xC0000
    for i in range(8):
     state.presets[i*20+14]=3;state.presets[i*20+15]=2;state.presets[i*20+16]=scenario%3
    operations=[('AllocGlyphRows',[state.roots[0]])]+[('InitGlyphRow',[i,-2,i,(scenario+i)%8])for i in range(4)]
    operations += [('LoadGlyphRowsStep',[])]*3+[('UpdateGlyphRow',[self.index])]*64
    for step,(name,args)in enumerate(operations):
     with self.subTest(linked=linked,pipeline=scenario,step=step,function=name):
      state=self.check_operation(state,name,args,errors)
   for a in FRAMES:self.assertEqual(bytes([0xB6])*FRAME_SIZE,self.read(a,FRAME_SIZE))
   for a in IMAGES:self.assertEqual(bytes([0xC7])*IMAGE_SIZE,self.read(a,IMAGE_SIZE))
  self.lib.check_guards(1)

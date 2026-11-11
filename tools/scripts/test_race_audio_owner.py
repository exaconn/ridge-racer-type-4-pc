from selection_fixture_transport import transport as transport_selection
"""Race audio configuration, bank controller, slot start and queue on real owners.

Native independent models check complete objects before/after controlled I/O and
key-on boundaries. The same trace drives original/linked MIPS. The queue's one
target register annotation is removed only for native compilation.
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
from test_vehicle_initializer import Cpu

ROOT=Path(__file__).resolve().parents[2]
STATE=0x57C+0xA8D0+32+32+1000+200+64
FRAMES,MAX_EVENTS=12,64
FRAME=4+STATE
EVENT=40+2*STATE
OUTPUT=4+FRAMES*FRAME+MAX_EVENTS*EVENT
PROGRAMS,ARENA,STACK,STOP=0x80200010,0x80210010,0x803F0000,0x803FF000
NAMES=('GetPlayerCarVariantIndex','SetupRaceSoundCarConfig','LoadSoundBanksStep',
       'LoadRaceSoundStep','SlotSetPause','SlotKeyOn','SlotStart','SlotsStartForRace',
       'SoundQueueEnqueue')
PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/audio_loop.h"
#include "rr/sequence_queue.h"
#include "rr/scene_camera_state.h"
#include "rr/selection_types.h"
#include "rr/memory.h"
#include "game/data/trkld1.h"
#include "game/data/trkld5.h"
#include "game/data/trkld6.h"
#include "game/sound/sndslot.h"
typedef struct {
 u32 before[4];SceneRaceContext scene;u32 afterScene[4];
 AudioState audio;u32 afterAudio[4];SelectionRow rows[8];u32 afterRows[4];
 s16 mode,maker,host;s32 step,load,sub;char *cursor;void *end;
 s16 programs[25][2][10];s16 *tables[25][2];u8 arena[64];u32 after[4];
} World;
typedef struct {u32 kind;s32 args[8],result;World before,after;} ModelEvent;
static World actual,expected;
static ModelEvent models[64];
static unsigned scenario,modelCount,eventCount;
static u8 *events;
#define RaceContext actual.scene
#define SoundState actual.audio
#define SoundSlots actual.audio.slots38
#define SoundSequenceTable actual.tables
#define SelectionRows ((u8*)actual.rows)
#define GameModeType actual.mode
#define SelectedMakerIndex actual.maker
#define LinkHostFlag actual.host
#define RaceLoadStepIndex actual.step
#define RaceLoadState actual.load
#define RaceLoadSubState actual.sub
#define RaceLoadCursor actual.cursor
#define LoadCursor actual.end
static void put32(u8 *p,u32 value){memcpy(p,&value,4);}
static u32 arena_address(const void *p){
 for(unsigned i=0;i<64;i++)if(p==actual.arena+i)return 0x80210010U+i;
 assert(!"unknown arena root");return 0;
}
static void wire(const World *w,u8 *out){
 const SceneRaceContext *s=&w->scene;const AudioState *a=&w->audio;
 assert(!s->standings&&!s->cameraEC.tracks138&&!s->camera24C);
 assert(!s->cameraNodes550&&!s->workspace554&&!s->lightPresets558&&!s->workspace55C);
 assert(!s->soundPayload564&&!s->lookupTables);
 memcpy(out,s,0xE8);put32(out+0xE8,0);
 memcpy(out+0xEC,&s->cameraEC,0x138);put32(out+0x224,0);
 memcpy(out+0x228,&s->cameraEC.mode13C,0x24);put32(out+0x24C,0);
 memcpy(out+0x250,s->reserved250,0x300);memset(out+0x550,0,16);
 memcpy(out+0x560,&s->model560,4);put32(out+0x564,0);put32(out+0x568,0);
 memcpy(out+0x56C,&s->sound,16);out+=0x57C;
 memcpy(out,a,0x2634);
 for(unsigned i=0;i<2;i++){
  assert(!a->streamBanks[i].table&&!a->seqAddr2858[i]);
  memcpy(out+0x2634+i*8,&a->streamBanks[i].sector,4);put32(out+0x2638+i*8,0);
  const SequenceChannel *c=&a->sequences[i];assert(!c->c0&&!c->c4&&!c->c8&&!c->cC&&!c->p10);
  memset(out+0x26F8+i*176,0,20);
  memcpy(out+0x26F8+i*176+20,(const u8*)c+offsetof(SequenceChannel,b14),156);
  put32(out+0x2858+i*4,0);
 }
 memcpy(out+0x2644,a->reserved2644,0x26F8-0x2644);
 memcpy(out+0x2860,&a->seqInit2860,0xA8D0-0x2860);out+=0xA8D0;
 memcpy(out,w->rows,32);out+=32;
 s32 values[]={w->mode,w->maker,w->host,w->step,w->load,w->sub,
               (s32)arena_address(w->cursor),(s32)arena_address(w->end)};
 memcpy(out,values,32);out+=32;memcpy(out,w->programs,1000);out+=1000;
 for(unsigned i=0;i<25;i++)for(unsigned j=0;j<2;j++){
  assert(w->tables[i][j]==actual.programs[i][j]||w->tables[i][j]==(s16*)-1);
  put32(out,w->tables[i][j]==(s16*)-1?0xFFFFFFFFU:0x80200010U+(i*2+j)*20);out+=4;
 }
 memcpy(out,w->arena,64);
}
/* These effects specify the controlled service environment, shared by the
 * independent controller model and the actual native service adapters. */
static void effect(World *w,unsigned kind,unsigned ordinal,s32 argument){
 if((kind==1||kind==2)&&argument<0)return;
 if(scenario&1){
  w->scene.sound.sfxVabId=4+(ordinal&1);
  w->scene.sound.carVariants[0]=(scenario+ordinal)%12;
  w->scene.sound.carVariants[1]=ordinal%3?-1:(s32)((scenario+ordinal)%12);
  w->scene.sound.comBank=(s32)(ordinal%3)-1;
 }
 if(scenario&2){w->audio.split10=(s32)((ordinal+scenario)%3)-1;w->audio.sequenceVariant19CC=(s32)(ordinal%3)-1;}
 if((scenario&4)&&kind<3){
  w->step=(scenario+ordinal)%3;w->load=(ordinal/2)%3;w->sub=ordinal&1;
  w->cursor=(char*)actual.arena+16*(ordinal%4);
 }
 if((scenario&8)&&kind>=3)for(unsigned i=0;i<4;i++){
  AudioSlot620 *slot=&w->audio.slots38[i];
  slot->count0C=(scenario+ordinal+i)%4;slot->program14=(ordinal+i)&1;
  slot->bank04.half.value=(s16)((ordinal+i)%16);slot->voice08=10+ordinal+i;
  slot->flags00^=4;
 }
}
static s32 response(unsigned kind,s32 arg,unsigned ordinal){
 if((kind==1||kind==2)&&arg<0)return 1;
 return (scenario>>(ordinal%10))&1?0:(ordinal%3?1:-7);
}
static s32 model_event(unsigned kind,const s32 args[8]){
 assert(modelCount<64);unsigned n=modelCount++;ModelEvent *e=&models[n];
 e->kind=kind;memcpy(e->args,args,32);memcpy(&e->before,&expected,sizeof(World));
 effect(&expected,kind,n,args[0]);memcpy(&e->after,&expected,sizeof(World));
 return e->result=response(kind,args[0],n);
}
static s32 event(unsigned kind,const s32 args[8]){
 assert(eventCount<modelCount);unsigned n=eventCount++;const ModelEvent *e=&models[n];
 assert(e->kind==kind&&!memcmp(e->args,args,32)&&!memcmp(&actual,&e->before,sizeof(World)));
 u8 *p=events+n*@EVENT@;put32(p,kind);memcpy(p+4,args,32);put32(p+36,(u32)e->result);
 wire(&actual,p+40);effect(&actual,kind,n,args[0]);wire(&actual,p+40+@STATE@);
 assert(!memcmp(&actual,&e->after,sizeof(World)));return e->result;
}
int LoadSfxVabStep(int bank,int offset){const s32 a[8]={bank,offset};return event(0,a);}
int LoadSlotBankStep(int variant,int slot){const s32 a[8]={variant,slot};return event(1,a);}
int LoadComSlotBankStep(int bank){const s32 a[8]={bank};return event(2,a);}
void *PsyqMemset(void *p,s32 value,u32 length){
 assert((uintptr_t)p>UINT32_MAX&&((uintptr_t)p&3)==0&&value==0&&length==16);
 const s32 a[8]={0,value,(s32)length};event(3,a);return memset(p,value,length);
}
s32 SsUtKeyOnV(s32 voice,s32 bank,s32 program,s32 mode,s32 group,s32 unused,s32 left,s32 right){
 const s32 a[8]={voice,bank,program,mode,group,unused,left,right};return event(4,a);
}
'''
TAIL=r'''
static int variant(const SelectionRow *r){
 if(r->parameter==0x27)return r->variant?0:(r->selection==2?8:0);
 return r->variant+(r->parameter==0x13?4:0);
}
static void model_setup(void){
 int mode=expected.mode;int dual=mode==2||mode==3||mode==4;
 int special=mode==0||mode==2||mode==3||mode==4||mode==6;
 unsigned first=mode==4&&!expected.host?2:0;
 expected.audio.sequenceVariant19CC=special;
 expected.audio.split10=dual;
 expected.scene.sound.sfxVabId=special?5:4;
 expected.scene.sound.carVariants[0]=variant(&expected.rows[first]);
 expected.scene.sound.carVariants[1]=dual?variant(&expected.rows[first+1]):-1;
 expected.scene.sound.comBank=mode==6?expected.maker+1:(special?-1:0);
}
static int model_banks(s32 first,s32 second,s32 com){
 int stage=expected.load;const s32 args[8]={stage==1?second:(stage==2?com:first),stage==1};
 if(stage<0||stage>2)return 0;
 s32 result=model_event(stage==2?2:1,args);if(!result)return 0;
 if(stage==0){expected.load=second<0?2:1;return 0;}
 expected.load=0;return 1;
}
static int model_load(void){
 int stage=expected.step;if(stage<0||stage>2)return 0;
 if(stage<2){
  const s32 args[8]={stage?expected.scene.sound.sfxVabId:3,0};
  if(model_event(0,args))expected.step++;
  return 0;
 }
 RaceSoundConfig sound=expected.scene.sound;
 if(!model_banks(sound.carVariants[0],sound.carVariants[1],sound.comBank))return 0;
 expected.step=0;expected.end=expected.cursor;return 1;
}
static void model_slot(unsigned index){
 AudioSlot620 *slot=&expected.audio.slots38[index];slot->flags00|=1;
 const s32 clear[8]={0,0,16};model_event(3,clear);
 int voice=slot->voice08;
 for(int i=0;i<slot->count0C;i++){
  const s32 args[8]={(s16)voice,slot->bank04.half.value,
    (s16)slot->programData3C.programs[slot->program14][i],0,index==3?62:60,0,0,0};
  model_event(4,args);voice++;
 }
 slot->reserved2A=0;slot->group28=index==3?62:60;
 slot->previous18=slot->program14;slot->flags00&=~4U;
}
static void model_start(void){
 model_slot(0);
 if(expected.audio.split10)model_slot(1);
 else if(expected.scene.sound.comBank>=0){model_slot(2);model_slot(3);}
}
static u32 model_queue(unsigned id,int restart){
 unsigned column=expected.audio.sequenceVariant19CC!=0;
 if(expected.tables[id][column]==(s16*)-1)return 0xFFFFFFFFU;
 SoundSequenceState *q=&expected.audio.sequence19D0;
 unsigned length=0;while(expected.programs[id][column][length]>0)length++;
 unsigned used=q->active&&!restart?(unsigned)q->count:0;
 if(q->active&&!restart&&used+length>=8)return 0xFFFFFFFFU;
 if(!q->active||restart){
  q->active=1;q->readIndex=0;q->unknown0C=0;q->startImmediately=restart;
  q->leftVolume=80;q->rightVolume=80;
 }
 for(unsigned i=0;i<length;i++)q->programs[used+i]=expected.programs[id][column][i];
 q->programs[used+length]=-1;q->count=used+length;if(!length)q->active=0;
 return @AUDIO@+2*(used+length);
}
static void frame(u8 *out,u32 result){
 assert(!memcmp(&actual,&expected,sizeof(World))&&eventCount==modelCount);
 put32(out,result);wire(&actual,out+4);
}
void run_audio(unsigned number,u8 *output){
 const s16 modes[]={-32768,-1,0,1,2,3,4,5,6,32767};
 scenario=number;modelCount=0;eventCount=0;events=output+4+12*@FRAME@;
 memset(output,0xA5,@OUTPUT@);memset(&actual,0xA5,sizeof(actual));
 actual.mode=modes[number%10];actual.maker=(s16)(number*1771U);actual.host=number%3? (number&1?-1:7):0;
 actual.step=(number/10)%4;actual.load=(number/40)%4;actual.sub=0;
 actual.cursor=(char*)actual.arena;actual.end=actual.arena+48;
 actual.scene.standings=NULL;actual.scene.cameraEC.tracks138=NULL;actual.scene.camera24C=NULL;
 actual.scene.cameraNodes550=NULL;actual.scene.workspace554=NULL;actual.scene.lightPresets558=NULL;
 actual.scene.workspace55C=NULL;actual.scene.soundPayload564=NULL;actual.scene.lookupTables=NULL;
 for(unsigned i=0;i<2;i++){
  actual.audio.streamBanks[i].table=NULL;actual.audio.seqAddr2858[i]=NULL;
  SequenceChannel *c=&actual.audio.sequences[i];c->c0=NULL;c->c4=NULL;c->c8=NULL;c->cC=NULL;c->p10=NULL;
 }
 for(unsigned i=0;i<8;i++){
  const u8 parameters[]={0,0x13,0x27};actual.rows[i].selection=(number+i)%3;
  actual.rows[i].variant=(number+i)%8;actual.rows[i].parameter=parameters[(number/8+i)%3];
 }
 for(unsigned i=0;i<4;i++){
  AudioSlot620 *slot=&actual.audio.slots38[i];slot->voice08=10+i*4;slot->count0C=(number+i)%4;
  slot->bank04.half.value=i;slot->program14=(number+i)&1;
  for(unsigned j=0;j<2;j++)for(unsigned k=0;k<5;k++)slot->programData3C.programs[j][k]=1+(number+i+j+k)%127;
 }
 for(unsigned i=0;i<25;i++)for(unsigned j=0;j<2;j++){
  unsigned length=(number/5+i+j)%8;
  for(unsigned k=0;k<10;k++)actual.programs[i][j][k]=k<length?1+(number+i+j+k)%32767:0;
  actual.tables[i][j]=(number+i+j)%17?actual.programs[i][j]:(s16*)-1;
 }
 actual.audio.sequence19D0.active=(number/11)&1;actual.audio.sequence19D0.count=(number/7)%8;
 assert(sizeof(void*)==8&&(uintptr_t)&actual.audio>UINT32_MAX&&(uintptr_t)&actual.scene>UINT32_MAX);
 memcpy(&expected,&actual,sizeof(actual));frame(output+4,0);
 model_setup();SetupRaceSoundCarConfig();frame(output+4+@FRAME@,0);
 for(unsigned i=0;i<8;i++){
  int wanted=model_load();int got=LoadRaceSoundStep();assert(got==wanted);
  frame(output+4+(i+2)*@FRAME@,(u32)got);
 }
 model_start();SlotsStartForRace();frame(output+4+10*@FRAME@,0);
 unsigned id=number%25;int restart=(s32)((number/25)%3)-1;
 u32 wanted=model_queue(id,restart);void *got=SoundQueueEnqueue((s32)id,restart);
 assert(got==(wanted==0xFFFFFFFFU?(void*)-1:(u8*)&actual.audio+(wanted-@AUDIO@)));
 frame(output+4+11*@FRAME@,wanted);put32(output,eventCount);
}
'''


class RaceAudioOwnerTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        owners=source_owners(ROOT/'src/main/PAL')
        texts={n:owners[n].read_text() for n in NAMES}
        helper=definition_source(texts['SoundQueueEnqueue'],'ResetQueuedSoundPlayback')
        bodies='\n'.join(definition_source(texts[n],n) for n in NAMES)
        bodies,count=re.subn(r'\s+__asm__\("\$[0-9]+"\)','',bodies)
        assert count==0,'Review native handling of existing target annotations'
        cls.temp=TemporaryDirectory();cls.addClassCleanup(cls.temp.cleanup)
        source=Path(cls.temp.name)/'audio.c';library=source.with_suffix('.dylib')
        code=PREFIX+helper+bodies+TAIL
        for key,value in {'STATE':STATE,'EVENT':EVENT,'FRAME':FRAME,'OUTPUT':OUTPUT,
                          'AUDIO':address_of('SoundState')}.items():code=code.replace('@'+key+'@',str(value)+'U')
        source.write_text(transport_selection(code,definition_source(texts['GetPlayerCarVariantIndex'],'GetPlayerCarVariantIndex'),'GetPlayerCarVariantIndex','&actual.rows',32))
        proc=subprocess.run(['cc','-std=c11','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
                             '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                             '-I',str(ROOT/'include'),str(source),'-o',str(library)],capture_output=True,text=True)
        if proc.returncode:raise AssertionError(proc.stderr)
        cls.native=C.CDLL(str(library));cls.native.run_audio.argtypes=[C.c_uint,C.c_void_p]

    def test_configuration_loader_slots_and_queue_on_both_images(self):
        context,audio,selection=map(address_of,('RaceContext','SoundState','SelectionRows'))
        for alias,base,offset in [('PendingTrackSfxVabId',context,0x56C),('ComSoundBankIndex',context,0x578),
                                  ('DualPlayerSoundEnabled',audio,0x10),('SoundSeqVariantIndex',audio,0x19CC),
                                  ('SoundSlots',audio,0x38)]:self.assertEqual(address_of(alias),base+offset)
        services={'LoadSfxVabStep':(0,2),'LoadSlotBankStep':(1,2),'LoadComSlotBankStep':(2,1),
                  'PsyqMemset':(3,3),'SsUtKeyOnV':(4,8)}
        globals_=list(map(address_of,('GameModeType','SelectedMakerIndex','LinkHostFlag','RaceLoadStepIndex',
                                      'RaceLoadState','RaceLoadSubState','RaceLoadCursor','LoadCursor')))
        regions=[(context,0x57C),(audio,0xA8D0),(selection,32),*zip(globals_,(2,2,2,4,4,4,4,4)),
                 (PROGRAMS,1000),(address_of('SoundSequenceTable'),200),(ARENA,64)]
        guarded=[(context,0x57C),(selection,32+0xA8D0),*regions[-3:]]
        self.assertEqual(selection+32,audio)
        machines=[]
        for label,path in [('original',ROOT/'assets/PAL/main.exe'),('linked',ROOT/'build/PAL/main.exe')]:
            memory=Uc(UC_ARCH_MIPS,UC_MODE_MIPS32|UC_MODE_LITTLE_ENDIAN);memory.mem_map(0,0x400000)
            image=path.read_bytes();memory.mem_write(struct.unpack_from('<I',image,0x18)[0]&0x1FFFFFFF,image[0x800:])
            machines.append((label,memory))
        seen_modes=set();seen_services=set();seen_returns=set()
        for number in range(1024):
            output=C.create_string_buffer(OUTPUT);self.native.run_audio(number,output)
            raw=output.raw;count=struct.unpack_from('<I',raw)[0];self.assertLessEqual(count,MAX_EVENTS)
            frames=[raw[4+i*FRAME:4+(i+1)*FRAME] for i in range(FRAMES)]
            events=[raw[4+FRAMES*FRAME+i*EVENT:4+FRAMES*FRAME+(i+1)*EVENT] for i in range(count)]
            initial=frames[0][4:];seen_modes.add(struct.unpack_from('<i',initial,0x57C+0xA8D0+32)[0])
            for label,memory in machines:
                read=lambda a,n:bytes(memory.mem_read(a&0x1FFFFFFF,n))
                write=lambda a,b:memory.mem_write(a&0x1FFFFFFF,bytes(b))
                def load(data,guards=False):
                    p=0
                    for a,n in regions[:3]:
                        write(a,data[p:p+n])
                        p+=n
                    values=struct.unpack_from('<8I',data,p);p+=32
                    for a,n,value in zip(globals_,(2,2,2,4,4,4,4,4),values):write(a,value.to_bytes(4,'little')[:n])
                    for a,n in regions[-3:]:
                        write(a,data[p:p+n])
                        p+=n
                    self.assertEqual(p,STATE)
                    if guards:
                        for a,n in guarded:write(a-16,b'\xA6'*16);write(a+n,b'\x6A'*16)
                def snapshot():
                    values=[int.from_bytes(read(a,n),'little',signed=n==2)&0xFFFFFFFF for a,n in zip(globals_,(2,2,2,4,4,4,4,4))]
                    return b''.join(read(a,n) for a,n in regions[:3])+struct.pack('<8I',*values)+b''.join(read(a,n) for a,n in regions[-3:])
                def store(cpu,access,address,length,value,user):
                    address&=0x1FFFFFFF
                    writable=[(context+0x564,24),(audio,0xA8D0),*zip(globals_,(2,2,2,4,4,4,4,4)),(STACK-0x200,0x200)]
                    self.assertTrue(any((a&0x1FFFFFFF)<=address and address+length<=(a&0x1FFFFFFF)+n for a,n in writable),(hex(cpu.pc),hex(address),length))
                cpu=Cpu(memory,0,100,101,102,store);load(initial,True);event_index=0
                calls=[('SetupRaceSoundCarConfig',[]),*[( 'LoadRaceSoundStep',[]) for _ in range(8)],
                       ('SlotsStartForRace',[]),('SoundQueueEnqueue',[number%25,(number//25)%3-1])]
                service_addresses={address_of(n):(kind,argc) for n,(kind,argc) in services.items()}
                for frame_index,(name,args) in enumerate(calls,1):
                    write(STACK-0x210,b'\xA5'*0x220);cpu.reset();cpu.r=[0]+[0xBEEF0000+i for i in range(1,32)]
                    for i,value in enumerate(args):cpu.r[4+i]=value&0xFFFFFFFF
                    cpu.r[29],cpu.r[31],cpu.pc=STACK,STOP,address_of(name)
                    for _ in range(32768):
                        if cpu.pc==STOP:break
                        if cpu.pc in service_addresses:
                            kind,argc=service_addresses[cpu.pc];self.assertLess(event_index,count)
                            event=events[event_index];event_index+=1
                            wanted_kind,*words=struct.unpack_from('<10I',event);wanted_args=words[:8];result=words[8]
                            got=[cpu.r[4+i] if i<4 else int.from_bytes(read(cpu.r[29]+16+(i-4)*4,4),'little') for i in range(argc)]
                            buffer=got[0] if kind==3 else None
                            if kind==3:
                                self.assertGreaterEqual(buffer,STACK-0x200);self.assertLessEqual(buffer+16,STACK);got[0]=0
                            self.assertEqual((kind,got+[0]*(8-argc)),(wanted_kind,wanted_args),(number,label,name,event_index))
                            self.assertEqual(snapshot(),event[40:40+STATE],(number,label,name,'before service'))
                            load(event[40+STATE:]);seen_services.add(kind)
                            if kind==3:write(buffer,bytes([got[1]&255])*got[2]);result=buffer
                            cpu.r[2]=result;cpu.pc=cpu.r[31];continue
                        cpu.step()
                    self.assertEqual((cpu.pc,cpu.r[29]),(STOP,STACK),(number,label,name))
                    for reg in (*range(16,24),28,30):self.assertEqual(cpu.r[reg],0xBEEF0000+reg)
                    wanted_result=struct.unpack_from('<I',frames[frame_index])[0]
                    if name in ('LoadRaceSoundStep','SoundQueueEnqueue'):
                        self.assertEqual(cpu.r[2],wanted_result,(number,label,name));seen_returns.add(wanted_result)
                    self.assertEqual(snapshot(),frames[frame_index][4:],(number,label,name,frame_index))
                    self.assertEqual(read(STACK-0x210,16),b'\xA5'*16);self.assertEqual(read(STACK,16),b'\xA5'*16)
                    for a,n in guarded:
                        self.assertEqual(read(a-16,16),b'\xA6'*16);self.assertEqual(read(a+n,16),b'\x6A'*16)
                self.assertEqual(event_index,count)
        self.assertEqual(seen_modes,{-32768,-1,0,1,2,3,4,5,6,32767})
        self.assertEqual(seen_services,set(range(5)));self.assertTrue({0,1,0xFFFFFFFF}<=seen_returns)


if __name__=='__main__':unittest.main()

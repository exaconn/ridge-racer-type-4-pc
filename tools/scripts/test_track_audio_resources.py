"""Actual three resource controllers publish full native pointers from wire headers.

I/O, bank/image submission, empty mesh relocation and final sound reset are
controlled boundaries. This checks controller state, live roots, all section
arguments and complete owners; it does not decode VAB/TIM payloads or mesh data.
"""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest
from c_functions import source_owners, definition_source

ROOT=Path(__file__).resolve().parents[2]
PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rr/race_audio_resources.h"
#include "rr/scene_camera_state.h"
#include "rr/audio_state_types.h"
#include "game/data/cdfile1.h"
#include "game/data/cdfile2.h"
#include "game/data/trkld6.h"
#include "game/course/trkres2.h"
#include "game/course/trkintp2.h"
#include "game/sound/sndvab.h"
#include "game/sound/sndcmd.h"
#include "game/gfx/model.h"
typedef struct {
 u32 before[4];SceneRaceContext scene;u32 afterScene[4];
 AudioState audio;u32 afterAudio[4];VabData args;u32 afterArgs[4];
 VabLoadConfig configs[8];s32 step;char *cursor;void *end;u32 after[4];
} World;
static World actual,expected;
static u8 *allocations[2],*images[2],saved[2][1056];
static unsigned scenario,kind,bank,serial,reads,loaded,closed;
#define RaceContext actual.scene
#define SoundState actual.audio
#define VabLoadArgs actual.args
#define CourseVabLoadConfigs actual.configs
#define RaceLoadStepIndex actual.step
#define RaceLoadCursor actual.cursor
#define LoadCursor actual.end
static void check(void){assert(!memcmp(&actual,&expected,sizeof(World)));serial++;}
static unsigned image(void){for(unsigned i=0;i<2;i++)if(actual.cursor==(char*)images[i])return i;assert(0);return 0;}
static unsigned model_offset(unsigned i){return 64+i*32;}
static unsigned relocation_offset(unsigned i){return 256+i*16;}
static unsigned header_offset(unsigned i,unsigned b){return 128+i*16+b*32;}
static unsigned sample_offset(unsigned i,unsigned b){return 512+i*16+b*32+2*(scenario&1);}
static unsigned sequence_offset(unsigned i,unsigned b){return 384+i*16+b*16;}
static unsigned image_offset(unsigned i){return 704+i*16;}
static void move_root(unsigned flag){
 if(scenario&flag){unsigned i=1-image();actual.cursor=(char*)images[i];expected.cursor=(char*)images[i];}
}
static void expect_args(unsigned b){
 unsigned i=image();expected.args.header=images[i]+header_offset(i,b);
 expected.args.samples=(const u16*)(images[i]+sample_offset(i,b));
}
int GetR4FileIndex(int group,int index){check();assert(group==2&&index==(kind==0?1:kind==1?2:0));return 200+index;}
s32 CdFileReadStart(s32 file,void *destination){
 check();assert(file==200+(kind==0?1:kind==1?2:0)&&destination==actual.cursor);
 reads++;move_root(4);if((scenario>>1)&1)expected.step++;
 return (scenario>>1)&1;
}
s32 CdFileBytesLoaded(void){
 check();loaded++;move_root(8);
 if((scenario&1)&&kind==2)expect_args(0);
 return scenario&1;
}
void RegisterModelBank(RroModelBank *p,s32 slot){
 check();unsigned i=image();assert((u8*)p==images[i]+model_offset(i));
 assert((uintptr_t)p>UINT32_MAX&&slot==(kind==0?7:8));
 move_root(16);
}
void RelocateOffsetTable(int *p){
 check();unsigned i=image();assert((u8*)p==images[i]+relocation_offset(i)&&*p==0);
 /* The fixture uses a real empty relocation table, whose relocator is inert. */
 move_root(32);i=image();expected.scene.soundPayload564=(char*)images[i]+relocation_offset(i);
 expect_args(0);
}
void LoadVab(const VabData *p,s32 mode,s32 slot){
 check();assert(p==&actual.args&&bank<(kind==0?2U:1U));
 unsigned config=kind==0?bank+1:6;
 assert(mode==actual.configs[config].open_mode&&slot==actual.configs[config].bank_slot);
 move_root(64);unsigned i=image();
 if(kind==0){expected.audio.seqAddr2858[bank]=images[i]+sequence_offset(i,bank);if(bank==0)expect_args(1);}
 bank++;
}
void LoadTimList(int *p){
 check();unsigned i=image();assert((u8*)p==images[i]+image_offset(i));move_root(128);i=image();
 expected.step=0;
 if(kind!=1){
  uintptr_t end=(uintptr_t)(images[i]+sample_offset(i,0));
  if(kind==0)end=(end+3)&~(uintptr_t)3;
  expected.cursor=(char*)end;expected.end=(void*)end;
 }
}
void SoundCloseVabsAndReset(void){check();assert(kind==0);closed++;}
'''
TAIL=r'''
int main(void){
 for(unsigned i=0;i<2;i++){allocations[i]=malloc(1056);assert(allocations[i]);images[i]=allocations[i]+16;assert((uintptr_t)images[i]>UINT32_MAX);}
 for(scenario=0;scenario<1024;scenario++)for(kind=0;kind<3;kind++){
  memset(&actual,0xA5,sizeof(actual));bank=0;serial=0;reads=0;loaded=0;closed=0;
  for(unsigned i=0;i<2;i++){
   memset(allocations[i],0xA5,1056);unsigned char *p=images[i];
   if(kind==0){TrackAudioResourceHeader h={0,model_offset(i),relocation_offset(i),
      {sequence_offset(i,0),sequence_offset(i,1)},
      {header_offset(i,0),header_offset(i,1)},
      {sample_offset(i,0),sample_offset(i,1)},image_offset(i)};memcpy(p,&h,sizeof(h));}
   else if(kind==1){TrophyResourceHeader h={0,model_offset(i),image_offset(i)};memcpy(p,&h,sizeof(h));}
   else{AttractAudioResourceHeader h={0,header_offset(i,0),sample_offset(i,0),image_offset(i)};memcpy(p,&h,sizeof(h));}
   RroModelBank *model=(RroModelBank*)(p+model_offset(i));model->streamCount=0;model->vertices.pointer=NULL;model->normals.pointer=NULL;
   s32 count=0;memcpy(p+relocation_offset(i),&count,4);
   memcpy(saved[i],allocations[i],1056);
  }
  for(unsigned i=0;i<8;i++){actual.configs[i].file_index=10+i;actual.configs[i].open_mode=i%2;actual.configs[i].bank_slot=i;}
  actual.scene.standings=NULL;actual.scene.cameraEC.tracks138=NULL;actual.scene.camera24C=NULL;
  actual.scene.cameraNodes550=NULL;actual.scene.workspace554=NULL;actual.scene.lightPresets558=NULL;
  actual.scene.workspace55C=NULL;actual.scene.soundPayload564=NULL;actual.scene.lookupTables=NULL;
  for(unsigned i=0;i<2;i++){
   actual.audio.streamBanks[i].table=NULL;actual.audio.seqAddr2858[i]=NULL;
   SequenceChannel *c=&actual.audio.sequences[i];c->c0=NULL;c->c4=NULL;c->c8=NULL;c->cC=NULL;c->p10=NULL;
  }
  actual.args.header=NULL;actual.args.samples=NULL;
  unsigned initial=(scenario/4)%3;actual.step=initial;actual.cursor=(char*)images[0];actual.end=images[1]+800;
  memcpy(&expected,&actual,sizeof(actual));
  int result=kind==0?LoadTrackVabsStep():kind==1?LoadTrophyModelStep():LoadAttractRaceStep();
  int submitted=initial<2&&(initial==1||((scenario>>1)&1))&&(scenario&1);
  assert(result==submitted);check();
  assert(reads==(initial==0));assert(loaded==(initial==1||(initial==0&&((scenario>>1)&1))));
  assert(bank==(submitted?(kind==0?2U:kind==2?1U:0U):0U));assert(closed==(unsigned)(submitted&&kind==0));
  for(unsigned i=0;i<2;i++)assert(!memcmp(allocations[i],saved[i],1056));
 }
 for(unsigned i=0;i<2;i++)free(allocations[i]);
 return 0;
}
'''


class TrackAudioResourcesTest(unittest.TestCase):
    def test_three_live_resource_headers_and_full_native_roots(self):
        owners=source_owners(ROOT/'src/main/PAL')
        names=('AlignUp4','LoadTrackVabsStep','LoadTrophyModelStep','LoadAttractRaceStep')
        bodies='\n'.join(definition_source(owners[n].read_text(),n) for n in names)
        with TemporaryDirectory() as temporary:
            source=Path(temporary)/'resources.c';binary=source.with_suffix('')
            source.write_text(PREFIX+bodies+TAIL)
            proc=subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror',
                                 '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                                 '-I',str(ROOT/'include'),str(source),'-o',str(binary)],capture_output=True,text=True)
            if proc.returncode:raise AssertionError(proc.stderr)
            subprocess.run([str(binary)],check=True)


if __name__=='__main__':unittest.main()

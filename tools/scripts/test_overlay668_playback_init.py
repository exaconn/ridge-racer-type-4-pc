"""Playback initializer: complete native allocation and mutable setup services."""
from pathlib import Path
from tempfile import TemporaryDirectory
import subprocess
import unittest


class PlaybackInitTest(unittest.TestCase):
    def test_captured_allocation_and_fresh_resource(self):
        root = Path(__file__).resolve().parents[2]
        body = (root / 'src/overlays/PAL/668/func_801149E8.c').read_text()
        source = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(void*)!=4||sizeof(t)==n,"PAL size")
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(sizeof(void*)!=4||offsetof(t,f)==n,"PAL offset")
#include "rr/overlay668_playback_init.h"
typedef struct {u32 guard[4],rng;RrStrContextPrefix context;void *base,*cursor;s16 channel,mode,status;u32 tail[4];} State;
typedef struct {s32 kind,args[4];unsigned owner;u32 hash;} Event;
typedef struct {State s;Event events[8];unsigned count;} World;
typedef struct {u32 before[4];RrStrPlaybackState player;u32 after[4];} Owner;
static World actual,reference,initial,*w;
static Owner owners[3],expectedOwners[3],initialOwners[3];
static unsigned scenario,mutation,selectedSeen[3],resourceSeen[128],clearSeen;
#define D_800AC650 w->s.context
#define D_800F4D40 w->s.base
#define D_800ACC18 w->s.cursor
#define D_800F4C6A w->s.channel
#define D_800F3A26 w->s.mode
#define D_800F4D8A w->s.status
static s32 word(u32 n){s32 v;memcpy(&v,&n,4);return v;}
static u32 next(void){u32 n=w->s.rng;n^=n<<13;n^=n>>17;n^=n<<5;return w->s.rng=n;}
static unsigned ownerIndex(const void *p){unsigned i;assert((uintptr_t)p>UINT32_MAX);for(i=0;i<3;i++)if(p==&owners[i].player)return i;assert(0);return 0;}
static Event *event(s32 kind){unsigned i;u32 h=2166136261U;Owner *p=w==&actual?owners:expectedOwners;Event *e;
 assert(w->count<8);e=&w->events[w->count++];memset(e,0,sizeof(*e));e->kind=kind;
 for(i=0;i<sizeof(w->s);i++)h=(h^((u8*)&w->s)[i])*16777619U;
 for(i=0;i<sizeof(owners);i++)h=(h^((u8*)p)[i])*16777619U;e->hash=h;return e;
}
static void change(void){unsigned i;Owner *p=w==&actual?owners:expectedOwners;if(!mutation)return;
 w->s.base=&owners[next()%3].player;w->s.cursor=&owners[next()%3].player;D_800AC650.activeB8=&owners[next()%3].player;
 D_800AC650.resourceB0=next()%128;D_800AC650.phaseB4=word(next());D_800AC650.stopD0=word(next());
 w->s.channel=(s16)next();w->s.mode=(s16)next();w->s.status=(s16)next();
 for(i=0;i<3;i++)p[i].player.width00=word(next());
}
void func_80092CB8(s32 mode){event(1)->args[0]=mode;change();}
void func_8001ECE8(s32 r,s32 g,s16 b,u32 flags){Event *e=event(2);e->args[0]=r;e->args[1]=g;e->args[2]=b;e->args[3]=word(flags);change();}
void func_80096434(void *p,u32 count){unsigned i=ownerIndex(p);Event *e=event(3);Owner *o=w==&actual?owners:expectedOwners;
 assert(count==sizeof(RrStrPlaybackState));assert(D_800AC650.activeB8==p);assert(w->s.cursor==(u8*)p+sizeof(RrStrPlaybackState));
 e->owner=i;e->args[0]=word(count);selectedSeen[i]++;clearSeen++;memset(&o[i].player,0,count);change();
}
s32 func_800929D4(s32 mode){event(4)->args[0]=mode;change();return word(0xF0123456U);}
s32 func_8008C0D8(s32 mode,u8 *status){Event *e=event(5);e->args[0]=mode;assert(status==0);change();return -1;}
s32 func_8008C140(s32 command,const u8 *parameters,u8 *result){Event *e=event(6);e->args[0]=command;assert(parameters==0&&result==0);change();return scenario%3;}
void ovl668_func_80114910(s32 resource,RrStrCdFileInfo *file,u32 *sectors){Event *e=event(7);uintptr_t f=(uintptr_t)file,s=(uintptr_t)sectors;
 assert(f>UINT32_MAX&&s>UINT32_MAX);assert(s+sizeof(*sectors)<=f||f+sizeof(*file)<=s);assert(resource>=0&&resource<128);resourceSeen[resource]++;
 e->args[0]=resource;
 /* Like sector-to-location conversion, only the three address bytes are
  * outputs. The initializer must never inspect the remaining local bytes. */
 file->location.c[0]=12;file->location.c[1]=34;file->location.c[2]=56;file->byteSize=0x12345000U;*sectors=0x2468AU;change();
}
''' + body + r'''
static void expected(void){RrStrCdFileInfo file;u32 sectors;void *captured;
 func_80092CB8(0);func_8001ECE8(0,0,0,5);captured=w->s.base;
 w->s.cursor=captured;D_800AC650.activeB8=captured;w->s.cursor=(u8*)captured+sizeof(RrStrPlaybackState);
 func_80096434(captured,sizeof(RrStrPlaybackState));func_800929D4(3);func_8008C0D8(0,0);func_8008C140(9,0,0);
 D_800AC650.phaseB4=0;D_800AC650.stopD0=0;ovl668_func_80114910(D_800AC650.resourceB0,&file,&sectors);
 w->s.channel=2;w->s.mode=4;w->s.status=0;
}
int main(void){unsigned i;uint64_t cases=0;
 for(scenario=0;scenario<16384;scenario++)for(mutation=0;mutation<2;mutation++){
  memset(&initial,0,sizeof(initial));w=&initial;w->s.rng=0x149E8+scenario*73;
  for(i=0;i<sizeof(initialOwners);i++)((u8*)initialOwners)[i]=(u8)next();
  for(i=0;i<4;i++){w->s.guard[i]=next();w->s.tail[i]=next();}
  w->s.base=&owners[scenario%3].player;w->s.cursor=&owners[(scenario+1)%3].player;D_800AC650.activeB8=&owners[(scenario+2)%3].player;
  D_800AC650.resourceB0=scenario%128;D_800AC650.phaseB4=word(next());D_800AC650.stopD0=word(next());
  w->s.channel=(s16)next();w->s.mode=(s16)next();w->s.status=(s16)next();
  actual=reference=initial;memcpy(owners,initialOwners,sizeof(owners));memcpy(expectedOwners,initialOwners,sizeof(owners));
  w=&actual;ovl668_func_801149E8();w=&reference;expected();
  assert(actual.count==7&&!memcmp(&actual,&reference,sizeof(actual)));assert(!memcmp(owners,expectedOwners,sizeof(owners)));cases++;
 }
 assert(cases==32768&&clearSeen==65536);for(i=0;i<3;i++)assert(selectedSeen[i]);for(i=0;i<128;i++)assert(resourceSeen[i]);return 0;
}
'''
        with TemporaryDirectory() as tmp:
            p = Path(tmp)
            (p / 'test.c').write_text(source)
            subprocess.run(['cc', '-std=gnu11', '-O2', '-fsanitize=undefined',
                            '-fno-sanitize-recover=all', '-I', str(root / 'include'),
                            str(p / 'test.c'), '-o', str(p / 'test')], check=True)
            subprocess.run([str(p / 'test')], check=True)

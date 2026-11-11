"""Canonical native standings rows and an independent wrapped-distance oracle."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest
from c_functions import source_owners, definition_source

ROOT=Path(__file__).resolve().parents[2]


class RaceFrontSearchTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        owner=source_owners(ROOT/'src/main/PAL')['FindNearestCarAhead']
        text=owner.read_text()
        body=definition_source(text,'FindNearestCarAhead')
        dependencies=re.findall(r'^#define RR_STANDING_(?:PREFIX_SIZE|ROW)\b[^\n]*(?:\n[^\n]*)?',text,flags=re.M)
        assert len(dependencies)==2
        body='\n'.join(dependencies)+'\n'+body
        body=re.sub(r'\s+__asm__\("\$\d+"\)','',body)
        code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rr/layout.h"
#include "rr/geometry.h"
#include "rr/race_front_search.h"

typedef union {
    RaceStandingsTable table;
    struct {u8 pad[0x14];s16 value;} count;
} CountStorage;
typedef struct {
    u8 prefix[32];
    RaceVehicle vehicle;
    SceneRaceContext context;
    CountStorage counter;
    RaceStandingsTable ordinary,track;
    u8 suffix[32];
} World;
static World actual,expected;
#define RaceContext actual.context
#define NumCars actual.counter.count.value
#define TrackLength actual.track.gap14
''' + body + r'''
static u8 *locate(const void *address,size_t size) {
    uintptr_t offset=(uintptr_t)address-(uintptr_t)&actual;
    assert(offset<=sizeof(actual)-size);return (u8*)&expected+offset;
}
static s32 get32(const void *address) {
    s32 value;memcpy(&value,locate(address,4),4);return value;
}
static s32 bits(u32 value) {s32 result;memcpy(&result,&value,4);return result;}
static int reference(void) {
    s32 desired=bits((u32)(expected.vehicle.flags1E4&15)-1U+(u32)expected.context.playerCount14);
    RaceStandingsTable *table=expected.context.standings;
    int rows=expected.counter.count.value;
    int target=desired;
    for(int row=0;row<rows;row++) {
        if(get32(&table->entries18[row].unk00)==desired){target=row;break;}
    }
    rows=expected.counter.count.value;
    if(rows<=0)return -1;
    assert(rows<=8&&target>=0&&target<8);
    const int64_t length=expected.track.gap14;
    assert(length>0);
    int64_t closest=((int64_t)expected.vehicle.input+1)*16;
    int winner=-1;
    for(int row=0;row<rows;row++) {
        if(row==target)continue;
        int id=get32(&table->entries18[row].unk00);assert(id>=0&&id<8);
        u32 raw=(u32)get32(&table->entries18[id].totalDist04)-(u32)get32(&table->entries18[target].totalDist04);
        int64_t gap=(int64_t)bits(raw)%length;
        if(gap>=length/2)gap-=length;
        if(gap<-(length/2))gap+=length;
        if(gap>0&&gap<closest) {
            winner=id;closest=gap;
            s32 result=(s32)gap;memcpy(locate(&table->gap14,4),&result,4);
        }
    }
    return winner;
}
static RaceStandingsTable *initialize(int rows,s32 length,u32 seed,int alias) {
    memset(&actual,(u8)(seed^(seed>>8)),sizeof(actual));
    RaceStandingsTable *table=alias==1?&actual.counter.table:alias==2?&actual.track:&actual.ordinary;
    actual.context.standings=table;
    actual.context.playerCount14=1;
    actual.vehicle.flags1E4=0xFFF0;
    actual.vehicle.input=100;
    for(int i=0;i<8;i++) {
        table->entries18[i].unk00=i;
        seed=seed*1664525U+1013904223U;
        table->entries18[i].totalDist04=bits(seed);
        table->entries18[i].vehicle08=&actual.vehicle;
    }
    actual.counter.count.value=rows;
    actual.track.gap14=length;
    return table;
}
static void check(void) {
    memcpy(&expected,&actual,sizeof(actual));
    int wanted=reference();int got=FindNearestCarAhead(&actual.vehicle);
    assert(got==wanted);assert(!memcmp(&actual,&expected,sizeof(actual)));
}
static int permutation(int *values) {
    int i=6;while(i>=0&&values[i]>=values[i+1])i--;
    if(i<0)return 0;
    int j=7;while(values[j]<=values[i])j--;
    int t=values[i];values[i]=values[j];values[j]=t;
    for(int a=i+1,b=7;a<b;a++,b--){t=values[a];values[a]=values[b];values[b]=t;}
    return 1;
}
int main(int argc,char **argv) {
    assert(argc==2);assert(sizeof(void*)>4);assert(sizeof(RaceStandingEntry0C)>12);
    assert(offsetof(RaceStandingsTable,entries18)==sizeof(RaceStandingsTable)-sizeof(((RaceStandingsTable*)0)->entries18));
    int mode=atoi(argv[1]);
    if(mode==0) {
        const s32 lengths[]={1,2,3,4,5,7,8,127,128,1023,65535,2147483647};
        for(u32 input=0;input<65536;input++) {
            initialize(8,lengths[input%12],input,0);
            actual.vehicle.input=(s16)(input-32768U);
            actual.vehicle.flags1E4=(u16)(input&0xFFF0U);
            check();
        }
        for(u32 flags=0;flags<65536;flags++) {
            initialize(8,1023,flags,0);
            actual.vehicle.flags1E4=flags;
            actual.context.playerCount14=1-(int)(flags&15)+(flags/16)%8;check();
        }
        for(int rows=0;rows<=8;rows++)for(unsigned l=0;l<12;l++) {
            for(int target=0;target<8;target++)for(int delta=-130;delta<=130;delta++) {
                RaceStandingsTable *t=initialize(rows,lengths[l],rows*71+target,0);
                actual.context.playerCount14=target+1;
                for(int i=0;i<8;i++)t->entries18[i].totalDist04=delta+i*17;
                check();
            }
        }
        const s32 distance[]={(-2147483647-1),-2147483647,-65536,-1,0,1,65535,2147483646,2147483647};
        for(unsigned a=0;a<9;a++)for(unsigned b=0;b<9;b++)for(unsigned l=0;l<12;l++) {
            RaceStandingsTable *t=initialize(8,lengths[l],a*7+b,0);
            for(int i=0;i<8;i++)t->entries18[i].totalDist04=i?distance[b]:distance[a];
            check();
        }
    } else if(mode==1) {
        int ids[8]={0,1,2,3,4,5,6,7};u32 seed=0;
        do {
            RaceStandingsTable *t=initialize(8,511,seed++,0);
            for(int i=0;i<8;i++){t->entries18[i].unk00=ids[i];t->entries18[i].totalDist04=(i%3)*31;}
            actual.context.playerCount14=ids[seed%8]+1;
            check();
        }while(permutation(ids));
        for(int alias=1;alias<=2;alias++)for(int rows=2;rows<=8;rows++) {
            for(int selected=0;selected<rows;selected++)for(int n=0;n<127;n++) {
                RaceStandingsTable *t=initialize(rows,127,n,alias);
                for(int i=0;i<8;i++)t->entries18[i].totalDist04=i*i*7+n;
                actual.context.playerCount14=selected+1;
                check();
            }
        }
        RaceStandingsTable *aliased=initialize(8,127,0,1);
        const s32 count_gaps[8]={0,3,20,25,35,45,49,1};
        for(int i=0;i<8;i++)aliased->entries18[i].totalDist04=count_gaps[i];
        check(); /* First gap changes global count3; last row still wins at1. */
        aliased=initialize(3,100,0,2);
        aliased->entries18[0].totalDist04=0;
        aliased->entries18[1].totalDist04=20;
        aliased->entries18[2].totalDist04=45;
        check(); /* Captured length100 must survive the first gap20 store. */
    } else {
        const s16 inactive[]={-32768,-1,0};
        const s32 modes[]={(-2147483647-1),-1,0,1,2147483647};
        for(unsigned n=0;n<3;n++)for(unsigned m=0;m<5;m++) {
            initialize(inactive[n],0,n,0);
            actual.context.standings=0;actual.context.playerCount14=modes[m];
            actual.vehicle.flags1E4=0xFFFF;check();
        }
        for(int count=-32768;count<0;count++) {
            initialize(count,0,(u32)count,0);actual.context.standings=0;check();
        }
        /* The lookup may miss: the original identifier then remains the
         * reference index, provided it still names an accessible row. */
        for(int target=0;target<8;target++) {
            RaceStandingsTable *t=initialize(8,1000,target,0);
            for(int i=0;i<8;i++)t->entries18[i].unk00=(target+1)%8;
            actual.context.playerCount14=target+1;check();
        }
    }
    return 0;
}
'''
        source=Path(cls.temporary.name)/'race_front_search.c'
        source.write_text(code)
        cls.binary=source.with_suffix('')
        subprocess.run(['cc','-std=gnu99','-O2','-fno-strict-aliasing',
                        '-fsanitize=undefined','-fno-sanitize-recover=all',
                        '-I',str(ROOT/'include'),str(source),'-o',str(cls.binary)],check=True)

    def test_signed_inputs_wrap_and_track_boundaries(self):
        subprocess.run([str(self.binary),'0'],check=True)

    def test_all_permutations_ties_and_global_aliases(self):
        subprocess.run([str(self.binary),'1'],check=True)

    def test_inactive_and_missing_identifier(self):
        subprocess.run([str(self.binary),'2'],check=True)


if __name__=='__main__':
    unittest.main()

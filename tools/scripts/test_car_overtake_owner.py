"""Actual overtake caller: full native standings rows and captured vehicle roots.

Search/score are controlled service boundaries. This checks the successful
lane-target branch, including roots changed by those services; it does not
claim to test the search/score algorithms or the fallback projection path.
"""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest
from c_functions import definition_source, source_owners

ROOT=Path(__file__).resolve().parents[2]
PREFIX=r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "rr/race.h"
#include "rr/track_projection.h"
typedef struct {u32 before[4];RaceVehicle value;u32 after[4];} VehicleOwner;
typedef struct {u32 before[4];RaceStandingsTable value;u32 after[4];} TableOwner;
typedef struct {
    u32 before[4];SceneRaceContext scene;u32 afterScene[4];
    TableOwner tables[2];VehicleOwner current,rivals[8];
    s16 count;u32 after[4];
} World;
static World actual,expected;
static unsigned scenario,row,serial,replacement;
static s32 changedGap;
#define RaceContext actual.scene
#define NumPlayers actual.scene.playerCount14
#define NumCars actual.count
/* The selected branch does not access PlayerCars; keep the public complete
 * array type instead of exposing a private differently-strided projection. */
RaceVehicle PlayerCars[2];
s32 FindNearestCarAhead(RaceVehicle *p){
    assert(serial++==0&&p==&actual.current.value);
    if(scenario&1)actual.scene.standings=&actual.tables[1].value;
    return (s32)row;
}
s32 ScoreOvertakeOpportunity(RaceVehicle *p,s32 index){
    assert(serial++==1&&p==&actual.current.value&&index==(s32)row);
    if(scenario&2)actual.tables[0].value.entries18[row].vehicle08=&actual.rivals[replacement].value;
    if(scenario&4)actual.tables[0].value.gap14=changedGap;
    return 1;
}
void ComputeCarTrackSurfaceProj(RaceVehicle *p){(void)p;assert(!"unexpected fallback projection");}
'''
TAIL=r'''
int main(void){
    const s16 positions[]={-32768,-1025,-1024,-545,-544,-1,0,1,544,545,1024,1025,32767};
    const s16 spans[]={-32768,-1,0,1,2,100,1024,32767};
    const s32 gaps[]={-70000,-1024,-100,-1,0,1,100,1024,70000};
    assert(sizeof(void*)==8&&sizeof(RaceStandingEntry0C)>12);
    for(scenario=0;scenario<8192;scenario++){
        memset(&actual,0xA5,sizeof(actual));memset(PlayerCars,0xA5,sizeof(PlayerCars));
        row=scenario%8;unsigned selected=(row+scenario/8)%8;
        replacement=(selected+1)%8;serial=0;actual.count=8;
        actual.scene.playerCount14=0;actual.scene.standings=&actual.tables[0].value;
        actual.scene.cameraEC.tracks138=NULL;actual.scene.camera24C=NULL;
        actual.scene.cameraNodes550=NULL;actual.scene.workspace554=NULL;
        actual.scene.lightPresets558=NULL;actual.scene.workspace55C=NULL;
        actual.scene.soundPayload564=NULL;actual.scene.lookupTables=NULL;
        for(unsigned j=0;j<2;j++)for(unsigned i=0;i<8;i++){
            RaceVehicle *v=&actual.rivals[(i+scenario/8+j)%8].value;
            actual.tables[j].value.entries18[i].vehicle08=v;
            actual.tables[j].value.entries18[i].unk00=(s32)i;
            actual.tables[j].value.entries18[i].totalDist04=100;
            actual.rivals[i].value.lateralOffset172=positions[(i+scenario/64)%13];
            actual.rivals[i].value.control.record18=NULL;actual.rivals[i].value.control.handling1C=NULL;
            assert((uintptr_t)v>UINT32_MAX);
        }
        actual.tables[0].value.gap14=gaps[(scenario/8)%9];
        changedGap=gaps[(scenario/9)%9];
        RaceVehicle *v=&actual.current.value;RaceVehicleControl *q=&v->control;
        v->control.record18=NULL;v->control.handling1C=NULL;
        v->halfSpan10E=spans[(scenario/32)%8];v->lateralOffset172=positions[(scenario/128)%13];
        q->laneTarget3C=positions[(scenario/256)%13];q->laneActive6A=0;q->boostTimer44=0;
        memcpy(&expected,&actual,sizeof(actual));
        if(scenario&1)expected.scene.standings=&actual.tables[1].value;
        if(scenario&2)expected.tables[0].value.entries18[row].vehicle08=&actual.rivals[replacement].value;
        if(scenario&4)expected.tables[0].value.gap14=changedGap;
        int64_t gap=expected.tables[0].value.gap14;if(gap<0)gap=-gap;
        int span=expected.current.value.halfSpan10E;
        int lateral=expected.rivals[selected].value.lateralOffset172;
        int own=expected.current.value.lateralOffset172;
        int target;
        if(gap<span+span/2)target=(lateral+(lateral<own?1024:-1024))/2;
        else target=lateral+(lateral>0?-512:512);
        if(target < -544)target=-544;if(target>544)target=544;
        RaceVehicleControl *want=&expected.current.value.control;
        want->laneTimer38=10;want->laneActive6A=1;want->laneMode88=0;
        want->laneStart3A=want->laneTarget3C;want->laneTarget3C=(s16)target;
        u16 delta=(u16)((u16)want->laneTarget3C-(u16)want->laneStart3A);
        memcpy(&want->laneDelta6C,&delta,2);
        UpdateComCarOvertake(v);
        assert(serial==2);assert(!memcmp(&actual,&expected,sizeof(actual)));
        const u8 *players=(const u8*)PlayerCars;
        for(unsigned i=0;i<sizeof(PlayerCars);i++)assert(players[i]==0xA5);
    }
    return 0;
}
'''


class CarOvertakeOwnerTest(unittest.TestCase):
    def test_selected_rows_live_roots_and_complete_native_owners(self):
        owner=source_owners(ROOT/'src/main/PAL')['UpdateComCarOvertake']
        body=definition_source(owner.read_text(),'UpdateComCarOvertake')
        with TemporaryDirectory() as temporary:
            source=Path(temporary)/'overtake.c';binary=source.with_suffix('')
            source.write_text(PREFIX+body+TAIL)
            proc=subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror',
                                 '-fsanitize=undefined,bounds','-fno-sanitize-recover=all',
                                 '-I',str(ROOT/'include'),str(source),'-o',str(binary)],capture_output=True,text=True)
            if proc.returncode:raise AssertionError(proc.stderr)
            subprocess.run([str(binary)],check=True)


if __name__=='__main__':
    unittest.main()

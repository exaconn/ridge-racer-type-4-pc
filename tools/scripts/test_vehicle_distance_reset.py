from pathlib import Path
from tempfile import TemporaryDirectory
import subprocess
from c_functions import source_owners, definition_source
root=Path(__file__).resolve().parents[2]
import os
os.chdir(root)
p=Path('include');owner=source_owners(root/'src/main/PAL')['UpdateCarStartLineSteering'];body=definition_source(owner.read_text(),'UpdateCarStartLineSteering')
s=r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rr/race_types.h"
typedef struct {u32 guard[4];RaceVehicle vehicle;u32 tail[4];} State;
static s32 word(u32 x){s32 y;memcpy(&y,&x,4);return y;}static s16 half(u16 x){s16 y;memcpy(&y,&x,2);return y;}
'''+body+r'''
int main(void){State actual,expected;static const u32 words[]={0,1,6499,6500,6501,0x7FFFFFFFU,0x80000000U,0xFFFFFFFFU};
 assert(sizeof(void*)==8);assert((uintptr_t)&actual.vehicle>UINT32_MAX);for(unsigned scenario=0;scenario<65536;scenario++)for(unsigned profile=0;profile<8;profile++){
 memset(&actual,0xA6,sizeof(actual));actual.vehicle.control.previewSample84.halves.high=half(scenario);actual.vehicle.control.laneMode88=word(words[profile]);actual.vehicle.control.dynamics94.steering.angle0C=word(words[(scenario/16)%8]+scenario*0x1234567U);expected=actual;
 s32 delta=word((u32)expected.vehicle.control.laneMode88-6500U);expected.vehicle.control.relDist74=0;if(expected.vehicle.control.previewSample84.halves.high>=2)expected.vehicle.control.dynamics94.steering.angle0C=0;
 if(delta<0){s32 divided=delta/4;if(delta%4)divided--;expected.vehicle.control.relDist74=divided;expected.vehicle.control.dynamics94.steering.angle0C=word((u32)expected.vehicle.control.dynamics94.steering.angle0C*7U)/10;}
 else{expected.vehicle.control.relDist74=delta;expected.vehicle.control.laneMode88=word((u32)expected.vehicle.control.laneMode88-(u32)delta);expected.vehicle.control.dynamics94.steering.angle0C=word((u32)expected.vehicle.control.dynamics94.steering.angle0C-(u32)delta);}
 UpdateCarStartLineSteering(&actual.vehicle);assert(!memcmp(&actual,&expected,sizeof(actual)));}
 puts("524288 cases passed; full half27A sweep, wrapped32 subtraction/multiplication, full native vehicle pointer and complete guarded owner");}
'''
with TemporaryDirectory() as d:
 c=Path(d)/'test.c';exe=Path(d)/'test';c.write_text(s)
 subprocess.run(['cc','-std=gnu11','-O2','-funsigned-char','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I'+str(p),'-Iinclude',str(c),'-o',str(exe)],check=True)
 subprocess.run([str(exe)],check=True)

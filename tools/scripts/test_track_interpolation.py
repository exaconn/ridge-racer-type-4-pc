"""Actual track interpolation: signed wrapping, truncation and overlapping output."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT = Path(__file__).resolve().parents[2]


class TrackInterpolationTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        def header(name):
            text=(ROOT/'include/rr'/name).read_text()
            return re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',text,flags=re.M)
        body=(ROOT/'tools/scripts/fixtures/views/func_80043AB0.c').read_text()
        body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
        body=re.sub(r'\s+__asm__\("\$\d+"\)','',body)
        assert body.count('(u8*)0x800B0000U')==1
        body=body.replace('(u8*)0x800B0000U','count_page.bytes+0x10000')
        code=r'''
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
''' + header('race_types.h') + header('race.h') + header('track_sample_types.h') + header('track_interpolation.h') + r'''
static union {uint64_t alignment;u8 bytes[0x10000];} count_page;
''' + body + r'''
RrTrackSample3C *TrackNodes;
s32 TrackNodeCount;
s16 ReverseCourse;
static RrTrackSample3C actual[256], expected[256];
static unsigned random_state=0x873AF519;
static unsigned random_value(void) {
    random_state=random_state*1664525U+1013904223U;return random_state;
}
static void samples(void) {
    memset(actual,0xA5,sizeof(actual));
    for(int i=0;i<256;i++){
        actual[i].x08=(int)(random_value()%20001)-10000;
        actual[i].z0C=(int)(random_value()%20001)-10000;
        actual[i].y10=(int)(random_value()%20001)-10000;
    }
    memcpy(expected,actual,sizeof(actual));
}
static int floor1024(int value) {
    return value>=0?value/1024:-((-value+1023)/1024);
}
static void indices(int index,int difference,int direction,int count,int *first,int *second) {
    int a=index+(direction?1:-1)*floor1024(difference);
    if(direction) {
        if(a>=count)a-=count;
        *second=a+1;
        if(*second>=0)*second-=count;
    } else {
        if(a<0)a+=count;
        *second=a-1;
        if(*second<0)*second+=count;
    }
    *first=a;
}
static int interpolate(int first,int second,int fraction) {
    /* Wider arithmetic is an independent oracle in the no-overflow domain. */
    return first+(int)(((int64_t)second-first)*fraction/1024);
}
static void reference(int a,int b,int fraction,s32 *out) {
    RrTrackSample3C *x=&expected[128+a],*y=&expected[128+b];
    /* Keep the original order: an aliased output can change z before its read. */
    out[0]=4*interpolate(x->x08,y->x08,fraction);
    out[1]=interpolate(x->y10,y->y10,fraction);
    out[2]=4*interpolate(x->z0C,y->z0C,fraction);
}
static void one(int count,int index,int difference,int direction,int alias) {
    RrRaceVehicle vehicle, before;
    memset(&vehicle,0x39,sizeof(vehicle));
    vehicle.trackNodeF0=index;
    s16 offset=-123;
    memcpy(&vehicle.pad120[0x50],&offset,2);
    before=vehicle;
    TrackNodeCount=count;ReverseCourse=direction;
    memcpy(count_page.bytes+0xBECC,&count,4);
    TrackNodes=actual+128;
    int a,b;indices(index,difference,direction,count,&a,&b);
    assert(a>=-128&&a<128&&b>=-128&&b<128);
    s32 out[5]={0x13579,0,0,0,0x24680},want[3];
    s32 *output=out+1,*target=want;
    if(alias){
        int chosen=alias==1?a:b;
        output=&actual[128+chosen].x08;
        target=&expected[128+chosen].x08;
    }
    reference(a,b,(unsigned)difference&1023,target);
    GetTrackPointAhead(&vehicle,output,difference+offset);
    assert(memcmp(output,target,12)==0);
    assert(memcmp(actual,expected,sizeof(actual))==0);
    assert(memcmp(&vehicle,&before,sizeof(vehicle))==0);
    assert(out[0]==0x13579 && out[4]==0x24680);
}
int main(int argc,char **argv) {
    assert(argc==2);
    if(!strcmp(argv[1],"fractions")) {
        samples();
        for(int direction=-1;direction<=1;direction++) {
            for(int fraction=0;fraction<1024;fraction++) {
                for(int index=0;index<17;index++) {
                    one(17,index,fraction,direction,0);
                    one(17,index,-1024+fraction,direction,0);
                    one(17,index,17*1024+fraction,direction,0);
                }
            }
        }
    } else if(!strcmp(argv[1],"wrap")) {
        samples();
        for(int count=1;count<=32;count++)
            for(int index=0;index<count;index++)
                for(int step=-count;step<=count;step++)
                    for(int direction=0;direction<=1;direction++)
                        one(count,index,step*1024+513,direction,0);
    } else {
        for(int i=0;i<20000;i++) {
            samples();
            int count=1+random_value()%32;
            int index=random_value()%count;
            int difference=(int)(random_value()%(count*2048))-count*1024;
            int direction=random_value()&1;
            one(count,index,difference,direction,1+(random_value()&1));
        }
    }
    return 0;
}
'''
        directory=Path(cls.temporary.name)
        source=directory/'interpolation.c'
        source.write_text(code)
        cls.executable=directory/'interpolation'
        result=subprocess.run(['cc','-std=c99','-O2','-Wall','-Wextra','-Werror','-Wno-error=deprecated-non-prototype',
                               '-fsanitize=undefined','-fno-strict-aliasing',
                               '-I',str(ROOT/'include'),str(source),'-o',str(cls.executable)],
                              capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)

    def run_case(self,case):
        result=subprocess.run([str(self.executable),case],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(result.stderr,'')

    def test_all_fractions_and_signed_directions(self):self.run_case('fractions')
    def test_single_wrap_and_accessible_negative_neighbours(self):self.run_case('wrap')
    def test_output_overlapping_current_or_next_record(self):self.run_case('alias')


if __name__=='__main__':unittest.main()

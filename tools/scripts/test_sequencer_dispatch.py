"""Actual dispatcher: exact callback order, live state, pointer reload and reentry."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT=Path(__file__).resolve().parents[2]


class SequencerDispatchTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        def header(name):
            text=(ROOT/'include/rr'/name).read_text()
            return re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',text,flags=re.M)
        body=(ROOT/'build/legacy/func_80082D54.c').read_text()
        body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
        code=r'''
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
''' + header('spu.h') + header('library.h') + header('sequencer_init.h') + header('sequencer_dispatch.h') + r'''
typedef struct {
    RrLibStateB0 *table[32];
    s32 lock;
    u32 mask;
    s16 banks,rows;
    RrLibStateB0 primary[32][3],alternate[32][3];
} World;
static World actual,expected;
#define D_8010F068 actual.table
#define D_8010F05C actual.lock
#define D_8010F060 actual.mask
#define D_8010F8E8 actual.banks
#define D_8010F8EA actual.rows
''' + body + r'''
typedef struct {int id,bank,row;} Event;
static Event trace[1024],wanted[1024];
static int trace_count,wanted_count,scenario;
static void event(int id,int bank,int row) {
    assert(trace_count<1024);trace[trace_count++]=(Event){id,bank,row};
}
static void want(int id,int bank,int row) {
    assert(wanted_count<1024);wanted[wanted_count++]=(Event){id,bank,row};
}
static void reenter(void) {
    World before=actual;
    int count=trace_count;
    func_80082D54();
    assert(trace_count==count);
    assert(memcmp(&actual,&before,sizeof(actual))==0);
}
static void callback(int id,int bank,int row) {
    assert(actual.lock==1);
    event(id,bank,row);
    if(scenario==6)reenter();
    if(scenario==1) {
        if(id==1)actual.table[0]=actual.alternate[0];
        if(id==6){actual.table[0]=actual.primary[0];actual.lock=-7;}
    }
    if(scenario==2) {
        if(id==1)actual.primary[0][0].w98=0x82;
        if(id==3)actual.primary[0][0].w98=0xC;
        if(id==5)actual.primary[0][0].w98=8;
    }
    if(scenario==3&&bank==0&&row==0&&id==1) {
        actual.banks=3;actual.rows=3;actual.mask=5;
        actual.primary[0][1].w98=2;actual.primary[0][2].w98=8;
        actual.primary[2][0].w98=4;actual.primary[2][1].w98=0;
        actual.primary[2][2].w98=1;
    }
    if(scenario==4&&id==1){actual.banks=0;actual.rows=0;}
}
void func_80087064(void) {
    assert(actual.lock==1);event(0,-1,-1);
    if(scenario==5){actual.banks=actual.rows=1;actual.mask=1;}
    if(scenario==6)reenter();
}
void func_80083274(s16 bank,s16 row){callback(1,bank,row);}
void func_80082FC4(s32 bank,s32 row){callback(2,bank,row);}
void func_80084314(s32 bank,s32 row){callback(3,bank,row);}
void func_800831D4(s32 bank,s32 row){callback(4,bank,row);}
void func_80083A94(s32 bank,s16 row){callback(5,bank,row);}
void func_80083AF4(s32 bank,s32 row){callback(6,bank,row);}

static void prepare(int lock,int banks,int rows,u32 mask,u32 flags) {
    memset(&actual,0xA5,sizeof(actual));
    actual.lock=lock;actual.banks=banks;actual.rows=rows;actual.mask=mask;
    for(int bank=0;bank<32;bank++) {
        actual.table[bank]=(mask&(UINT32_C(1)<<bank))?actual.primary[bank]:NULL;
        for(int row=0;row<3;row++) {
            actual.primary[bank][row].w98=(s32)flags;
            actual.alternate[bank][row].w98=0xFF;
        }
    }
    trace_count=wanted_count=0;scenario=0;
}
static void expected_callbacks(int bank,int row,u32 flags) {
    if(flags&1) {
        want(1,bank,row);
        if(flags&0x10)want(2,bank,row);
        if(flags&0x20)want(2,bank,row);
        if(flags&0x40)want(3,bank,row);
        if(flags&0x80)want(3,bank,row);
    }
    if(flags&2)want(4,bank,row);
    if(flags&8)want(5,bank,row);
    if(flags&4)want(6,bank,row);
}
static void verify(void) {
    assert(trace_count==wanted_count);
    for(int i=0;i<trace_count;i++)
        assert(trace[i].id==wanted[i].id&&trace[i].bank==wanted[i].bank&&trace[i].row==wanted[i].row);
    assert(memcmp(&actual,&expected,sizeof(actual))==0);
}
static void normal(int lock,int banks,int rows,u32 mask,u32 flags) {
    prepare(lock,banks,rows,mask,flags);
    expected=actual;
    if(lock!=1) {
        want(0,-1,-1);
        for(int b=0;b<banks;b++)if(mask&(UINT32_C(1)<<b))
            for(int r=0;r<rows;r++) {
                expected_callbacks(b,r,flags);
                if(flags&4)expected.primary[b][r].w98=0;
            }
        expected.lock=0;
    }
    func_80082D54();verify();
}
static void live(int which) {
    prepare(0,which==4?3:1,which==4?3:1,which==4?7:1,which==1||which==3||which==4?1:0xFF);
    scenario=which;
    if(which==3)actual.table[2]=actual.primary[2];
    if(which==5){actual.banks=actual.rows=0;actual.mask=0;}
    expected=actual;expected.lock=0;
    want(0,-1,-1);
    if(which==1) {
        expected_callbacks(0,0,0xFF);
        expected.primary[0][0].w98=0;
    } else if(which==2) {
        want(1,0,0);want(3,0,0);want(5,0,0);
        expected.primary[0][0].w98=8;
    } else if(which==3) {
        want(1,0,0);want(4,0,1);want(5,0,2);want(6,2,0);want(1,2,2);
        expected.banks=3;expected.rows=3;expected.mask=5;
        expected.primary[0][1].w98=2;expected.primary[0][2].w98=8;
        expected.primary[2][0].w98=0;expected.primary[2][1].w98=0;
        expected.primary[2][2].w98=1;
    } else if(which==4) {
        want(1,0,0);expected.banks=expected.rows=0;
    } else {
        expected_callbacks(0,0,0xFF);
        expected.primary[0][0].w98=0;
        if(which==5){expected.banks=expected.rows=1;expected.mask=1;}
    }
    func_80082D54();verify();
}
static void maximum_rows(void) {
    prepare(0,1,32767,1,0);
    RrLibStateB0 *records=calloc(32767,sizeof(*records));
    assert(records);
    actual.table[0]=records;records[32766].w98=4;
    expected=actual;
    want(0,-1,-1);want(6,0,32766);
    func_80082D54();
    expected.lock=0;verify();
    for(int r=0;r<32767;r++)assert(records[r].w98==0);
    free(records);
}
int main(int argc,char **argv) {
    assert(argc==2);
    if(!strcmp(argv[1],"flags")) {
        for(int banks=0;banks<=3;banks++)
        for(int rows=0;rows<=3;rows++)
        for(unsigned mask=0;mask<8;mask++)
        for(unsigned flags=0;flags<256;flags++)
            normal(0,banks,rows,mask,flags|0x80004000U);
        for(unsigned flags=0;flags<256;flags++)normal(0,32,3,0x80000001U,flags);
    } else if(!strcmp(argv[1],"guard")) {
        const int locks[]={1,0,-1,2};
        for(unsigned i=0;i<sizeof(locks)/sizeof(*locks);i++) {
            normal(locks[i],3,3,7,0xFF);
            normal(locks[i],0,3,7,0xFF);
            normal(locks[i],-1,3,7,0xFF);
            normal(locks[i],3,-1,7,0xFF);
        }
        maximum_rows();
    } else {
        for(int which=1;which<=6;which++)live(which);
    }
    return 0;
}
'''
        directory=Path(cls.temporary.name);source=directory/'dispatch.c'
        source.write_text(code);cls.executable=directory/'dispatch'
        result=subprocess.run(['cc','-std=c99','-O2','-Wall','-Wextra','-Werror',
                               '-fsanitize=undefined','-fno-strict-aliasing',
                               '-I',str(ROOT/'include'),str(source),'-o',str(cls.executable)],
                              capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)

    def run_case(self,case):
        result=subprocess.run([str(self.executable),case],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(result.stderr,'')

    def test_all_control_flags_gating_order_and_bit31_bank(self):self.run_case('flags')
    def test_lock_nonpositive_dimensions_and_maximum_row(self):self.run_case('guard')
    def test_live_flags_dimensions_mask_pointer_reload_and_reentry(self):self.run_case('live')


if __name__=='__main__':unittest.main()

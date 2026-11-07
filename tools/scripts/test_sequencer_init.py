"""Actual sequencer table initialization: native pointers, mask and live dimensions."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT=Path(__file__).resolve().parents[2]


class SequencerInitTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary=TemporaryDirectory()
        cls.addClassCleanup(cls.temporary.cleanup)
        def header(name):
            text=(ROOT/'include/rr'/name).read_text()
            return re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',text,flags=re.M)
        body=(ROOT/'tools/scripts/fixtures/views/func_80083DE4.c').read_text()
        body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
        body=re.sub(r'\s+__asm__\("\$\d+"\)','',body)
        code=r'''
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
''' + header('spu.h') + header('library.h') + header('sequencer_init.h') + r'''
static struct {
    u8 prefix[64];
    RrLibStateB0 *entries[34];
    u8 suffix[64];
} pointers;
static s16 stored_banks,stored_rows,*banks_location,*rows_location;
#define _ss_score (pointers.entries+1)
#define _snd_seq_s_max (*banks_location)
#define _snd_seq_t_max (*rows_location)
u32 _snd_openflag;
''' + body + r'''
static void reset_record(RrLibStateB0 *record) {
    record->w98=record->w9c=record->wa0=record->wa4=record->wa8=record->wac=0;
    record->b22[0]=-1;record->b22[1]=0;
    for(int i=0;i<4;i++){record->h48[i]=0;record->h58[i]=127;}
}
static void one(int banks,int rows,u32 mask,int alias) {
    size_t count=(size_t)banks*rows;
    RrLibStateB0 *allocation=malloc((count+2)*sizeof(*allocation));
    RrLibStateB0 *expected=malloc((count+2)*sizeof(*expected));
    assert(allocation&&expected);
    for(size_t i=0;i<(count+2)*sizeof(*allocation);i++)
        ((u8*)allocation)[i]=(u8)(i*73U+(i>>7)+19U);
    RrLibStateB0 *base=allocation+1;
    memset(&pointers,0xA5,sizeof(pointers));
    for(int i=0;i<34;i++)pointers.entries[i]=allocation;
    union {uint64_t alignment;u8 bytes[sizeof(pointers)];} wanted_pointers;
    memcpy(wanted_pointers.bytes,&pointers,sizeof(pointers));
    RrLibStateB0 **wanted_table=(RrLibStateB0 **)(wanted_pointers.bytes+64);
    for(int i=0;i<banks;i++)wanted_table[i+1]=base+i*rows;
    banks_location=&stored_banks;rows_location=&stored_rows;
    stored_banks=-1;stored_rows=-1;
    if(alias&1)banks_location=&base->h48[0];
    if(alias&2)rows_location=&base->h48[1];
    memcpy(expected,allocation,(count+2)*sizeof(*allocation));
    if(alias&1)expected[1].h48[0]=banks;
    if(alias&2)expected[1].h48[1]=rows;
    int initialized=(int)count;
    if(alias&&banks>0&&rows>0)initialized=(alias&2)?1:rows;
    for(int i=0;i<initialized;i++)reset_record(&expected[i+1]);
    u32 wanted_mask=mask;
    for(int bit=banks;bit<32;bit++)wanted_mask|=UINT32_C(1)<<bit;
    _snd_openflag=mask;
    SsSetTableSize(banks?base:NULL,(s32)(0xA5A50000U|banks),(s32)(0x12340000U|rows));
    assert(_snd_openflag==wanted_mask);
    assert(memcmp(&pointers,wanted_pointers.bytes,sizeof(pointers))==0);
    assert(memcmp(allocation,expected,(count+2)*sizeof(*allocation))==0);
    assert(_snd_seq_s_max==((alias&1)&&initialized?0:banks));
    assert(_snd_seq_t_max==((alias&2)&&initialized?0:rows));
    free(expected);free(allocation);
}
int main(int argc,char **argv) {
    assert(argc==2);
    if(!strcmp(argv[1],"matrix")) {
        const u32 masks[]={0,0x12345678U,0x80000001U,0xFFFFFFFFU};
        for(int banks=0;banks<=32;banks++)
        for(int rows=0;rows<=17;rows++)
        for(unsigned i=0;i<sizeof(masks)/sizeof(*masks);i++)one(banks,rows,masks[i],0);
    } else if(!strcmp(argv[1],"bounds")) {
        one(0,32767,0,0);one(1,32767,0x01234567,0);
        one(2,1024,0x89ABCDEF,0);one(32,0,0xC001CAFE,0);
    } else {
        for(int alias=1;alias<=3;alias++)
        for(int banks=1;banks<=32;banks++)
        for(int rows=1;rows<=8;rows++)one(banks,rows,0x24681357,alias);
    }
    return 0;
}
'''
        directory=Path(cls.temporary.name);source=directory/'init.c'
        source.write_text(code);cls.executable=directory/'init'
        result=subprocess.run(['cc','-std=c99','-O2','-Wall','-Wextra','-Werror','-Wno-error=deprecated-non-prototype',
                               '-fsanitize=undefined','-fno-strict-aliasing',
                               '-I',str(ROOT/'include'),str(source),'-o',str(cls.executable)],
                              capture_output=True,text=True)
        if result.returncode:raise AssertionError(result.stderr)

    def run_case(self,case):
        result=subprocess.run([str(self.executable),case],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(result.stderr,'')

    def test_bank_row_matrix_mask_and_untouched_fields(self):self.run_case('matrix')
    def test_zero_and_maximum_dimension_boundaries(self):self.run_case('bounds')
    def test_dimensions_overlapping_initialized_state_remain_live(self):self.run_case('alias')


if __name__=='__main__':unittest.main()

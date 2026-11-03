"""Run the real tempo handler and decoder with native pointers and UBSan."""
from pathlib import Path
import re
import subprocess
import tempfile
import unittest


class SequencerTempoTest(unittest.TestCase):
    def test_tempo_and_live_stream_aliases(self):
        root = Path(__file__).resolve().parents[2]
        header = (root / 'include/rr/library.h').read_text()
        header = re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n', '', header, flags=re.M)
        bodies = []
        for name in ('80081AE4', '80081E04'):
            body = (root / f'tools/scripts/fixtures/views/func_{name}.c').read_text()
            body = re.sub(r'^#include[^\n]*\n', '', body, flags=re.M)
            body = re.sub(r'\s+(?:__asm__|asm)\("\$\d+"\)', '', body)
            body = re.sub(r'__asm__\(""[^;]*;', '', body)
            bodies.append(body)
        source = r'''
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
typedef uint8_t u8; typedef int8_t s8;
typedef uint16_t u16; typedef int16_t s16;
typedef uint32_t u32; typedef int32_t s32;
''' + header + r'''
static struct World {
    u32 guard0[8];
    RrLibStateB0 records[4][6];
    u8 streams[4][6][16];
    u32 guard1[8];
} world, initial, expected;
RrLibStateB0 *D_8010F068[32];
u32 D_8010F064;
s32 func_80081E04(s32, s32);
''' + '\n'.join(bodies) + r'''
static u32 seed=0x61a4e04;
static u32 random32(void) { seed ^= seed<<13; seed ^= seed>>17; seed ^= seed<<5; return seed; }
static void half(s16 *p,u32 v) { u16 h=v; memcpy(p,&h,2); }
static void word(s32 *p,u32 v) { memcpy(p,&v,4); }
static void reference(RrLibStateB0 *s) {
    u32 tempo=0,period,factor,product,threshold,q,n,rem,v,delta;
    int i;
    for(i=0;i<3;i++) tempo=(tempo<<8)|*s->c0++;
    period=60000000U/tempo;
    factor=(u32)(s32)s->h48[4];
    product=factor*period;
    threshold=D_8010F064*60U;
    word(&s->w94,period);
    if(product*10U<threshold) {
        q=(D_8010F064*600U)/product;
        half(&s->h48[5],q);half(&s->h48[6],q);
    } else {
        n=(u32)(s32)s->h48[4]*(u32)s->w94*10U;
        q=n/threshold;rem=n%threshold;
        half(&s->h48[5],65535);
        half(&s->h48[6],q+(D_8010F064*30U<rem));
    }
    v=*s->c0++;
    if(v&128U) {
        u32 byte;
        v&=127;
        do { byte=*s->c0++;v=(v<<7)|(byte&127); } while(byte&128);
    }
    delta=v*10U;
    word(&s->w88,(u32)s->w88+delta);
    word(&s->w90,delta);
}
int main(void) {
    unsigned iteration,mode,bank,row,b,r,i;
    assert(sizeof(void*)==8 && (uintptr_t)&world>UINT32_MAX);
    assert(offsetof(RrLibStateB0,c0)==0);
    assert(sizeof(world.records[0][0].h48[0])==2);
    assert(sizeof(world.records[0][0].w94)==4);
    for(iteration=0;iteration<393224;iteration++) {
        RrLibStateB0 *s;
        u8 *stream;
        u32 tempo,period,frequency;
        for(i=0;i<sizeof(world);i++) ((u8*)&world)[i]=(u8)random32();
        for(b=0;b<4;b++) for(r=0;r<6;r++) {
            world.records[b][r].c0=world.streams[b][r];
            world.records[b][r].c8=world.streams[b][r];
            world.records[b][r].p10=world.streams[b][r];
        }
        for(b=0;b<32;b++) D_8010F068[b]=&world.records[b%4][2];
        bank=random32()%32;row=random32()%6;
        s=&world.records[bank%4][row];
        mode=iteration%3;
        tempo=1+random32()%0xffffff;
        frequency=iteration%2?50:60;
        if(iteration%7==0) frequency=random32()|1U;
        D_8010F064=frequency;
        if(mode==0) {
            unsigned length=1+random32()%6;
            stream=world.streams[bank%4][row];
            for(i=0;i<length;i++) stream[3+i]=(random32()&127)|(i+1<length?128:0);
        } else if(mode==1) stream=(u8*)&s->w94;
        else {
            stream=(u8*)&s->h48[4];
            s->h58[0]=0;
        }
        if(iteration>=393216) {
            static const u32 periods[]={149,150,151,179,180,181,299,300};
            unsigned n=iteration-393216;
            D_8010F064=n>=3 && n<=5?60:50;
            tempo=60000000U/periods[n];
            mode=0;stream=world.streams[bank%4][row];stream[3]=0;
            half(&s->h48[4],n>=6?1:3);
        }
        stream[0]=tempo>>16;stream[1]=tempo>>8;stream[2]=tempo;
        s->c0=stream;
        period=60000000U/tempo;
        if((u32)(s32)s->h48[4]*period==0) {
            half(&s->h48[4],1);
            if(mode==2) {
                tempo=((u32)stream[0]<<16)|((u32)stream[1]<<8)|stream[2];
                assert(tempo!=0);
            }
        }
        memcpy(&initial,&world,sizeof(world));
        reference(s);
        memcpy(&expected,&world,sizeof(world));
        memcpy(&world,&initial,sizeof(world));
        func_80081AE4((s32)((random32()&0xffff0000U)|bank),
                      (s32)((random32()&0xffff0000U)|(u16)(s16)((int)row-2)));
        if(memcmp(&world,&expected,sizeof(world))) {
            fprintf(stderr,"tempo mismatch iteration=%u mode=%u\n",iteration,mode);return 1;
        }
    }
    puts("393224 native tempo/decoder cases passed");
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp)
            (path / 'test.c').write_text(source)
            subprocess.run(['cc', '-std=gnu11', '-O2', '-fsanitize=undefined',
                            '-fno-sanitize-recover=all', str(path / 'test.c'),
                            '-o', str(path / 'test')], check=True, capture_output=True)
            subprocess.run([str(path / 'test')], check=True, capture_output=True)


if __name__ == '__main__':
    unittest.main()

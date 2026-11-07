"""Actual SPU allocation cleanup against an independent serialized-record model."""
from pathlib import Path
import re
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT=Path(__file__).resolve().parents[2]


class SpuAllocationCleanupTest(unittest.TestCase):
    def test_actual_body_and_complete_table_owner(self):
        s=(ROOT/'tools/scripts/fixtures/views/func_8008A774.c').read_text()
        s=re.sub(r'^#include.*\n','',s,flags=re.M)
        s=re.sub(r' asm\("\$\d+"\)','',s)
        s=re.sub(r'__asm__\(""[^;]*;', ';',s)
        prefix=r'''
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,s) _Static_assert(sizeof(t)==(s),#t)
#define RR_ASSERT_OFFSET(t,f,o) _Static_assert(offsetof(t,f)==(o),#t "." #f)
#include "rr/spu_allocation.h"
_Static_assert(sizeof(RrSpuAllocation)==8,"serialized record");
_Static_assert(offsetof(RrSpuAllocation,size)==4,"size");
_Static_assert(sizeof(unsigned long)>=sizeof(void *),"full pointer arithmetic");
typedef struct {RrSpuAllocation records[34];int32_t last;uint32_t fence[4];} World;
static World initial,actual,expected,*world;
static RrSpuAllocation *table;
#define _spu_AllocLastNum (world->last)
#define _spu_memList table
'''
        reference=r'''
static uint32_t read32(const uint8_t *p){uint32_t v;memcpy(&v,p,4);return v;}
static void write32(uint8_t *p,uint32_t v){memcpy(p,&v,4);}
static void reference(void){
 int n=world->last;
 if(n<0)return;
 uint8_t *t=(uint8_t *)table;
 // Merge each contiguous successor, ignoring tombstones without skipping a live record.
 for(int i=0;i<=n;){
  uint32_t f=read32(t+i*8);
  int merged=0;
  if(f&UINT32_C(0x80000000)){
   int j=i+1;
   while(read32(t+j*8)==UINT32_C(0x2FFFFFFF))j++;
   uint32_t next=read32(t+j*8);
   uint32_t end=(f&UINT32_C(0x0FFFFFFF))+read32(t+i*8+4);
   if((next&UINT32_C(0x80000000)) && (next&UINT32_C(0x0FFFFFFF))==end){
    write32(t+j*8,UINT32_C(0x2FFFFFFF));
    write32(t+i*8+4,read32(t+i*8+4)+read32(t+j*8+4));merged=1;
   }
  }
  if(!merged)i++;
 }
 // Zero-length records become tombstones.
 for(int i=0;i<=n;i++)if(read32(t+i*8+4)==0)write32(t+i*8,UINT32_C(0x2FFFFFFF));
 // Exchange rows by their serialized address; stop both searches at the terminal bit.
 for(int i=0;i<=n && !(read32(t+i*8)&UINT32_C(0x40000000));i++){
  for(int j=i+1;j<=n && !(read32(t+j*8)&UINT32_C(0x40000000));j++){
   if((read32(t+j*8)&UINT32_C(0x0FFFFFFF))<(read32(t+i*8)&UINT32_C(0x0FFFFFFF))){
    uint8_t temporary[8];memcpy(temporary,t+i*8,8);memcpy(t+i*8,t+j*8,8);memcpy(t+j*8,temporary,8);
   }
  }
 }
 // Replace the first tombstone with the old terminal record.
 for(int i=0;i<=n;i++){
  uint32_t f=read32(t+i*8);
  if(f&UINT32_C(0x40000000))break;
  if(f==UINT32_C(0x2FFFFFFF)){memcpy(t+i*8,t+n*8,8);world->last=i;break;}
 }
 // Absorb all trailing free rows into the terminal, preserving modulo32 sizes.
 for(int i=world->last-1;i>=0;i--){
  uint32_t f=read32(t+i*8);
  if(!(f&UINT32_C(0x80000000)))break;
  uint32_t size=read32(t+i*8+4)+read32(t+world->last*8+4);
  write32(t+i*8,(f&UINT32_C(0x0FFFFFFF))|UINT32_C(0x40000000));write32(t+i*8+4,size);world->last=i;
 }
}
static uint32_t rng=0x57cad813;
static uint32_t next(void){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
static void bind(World *w){world=w;table=w->last<0?NULL:w->records+1;}
int main(void){
 for(unsigned k=0;k<131072;k++){
  for(size_t i=0;i<sizeof(initial);i++)((uint8_t *)&initial)[i]=next();
  int n=(int)(k%31)-6;initial.last=n;
  unsigned mode=(k/31)%7;
  uint32_t address=0x100;
  if(n>=0){
   for(int i=0;i<n;i++){
    RrSpuAllocation *r=&initial.records[i+1];
    uint32_t size=1+(next()%64);
    uint32_t type=next()%3;
    r->address_flags=type==2?UINT32_C(0x2FFFFFFF):((next()&UINT32_C(0x0FFFFFFF))|(type?UINT32_C(0x80000000):0));
    r->size=next();if(next()%5==0)r->size=0;
    if(mode==1){r->address_flags=UINT32_C(0x80000000)|address;r->size=size;address+=size;}
    if(mode==2){r->address_flags=address|((i%2)?0:UINT32_C(0x80000000));r->size=size;address+=size+3;}
    if(mode==3 && i%2){r->address_flags=UINT32_C(0x2FFFFFFF);r->size=0;}
    if(mode==4){r->size=0;}
    if(mode==5){r->address_flags=((uint32_t)((n-i)%5)*16)|((i%2)?0:UINT32_C(0x80000000));r->size=size;}
    if(mode==6){r->address_flags=((i%2)?UINT32_C(0x80000000):0)|address;r->size=UINT32_MAX-(uint32_t)i;address+=16;}
   }
   initial.records[n+1].address_flags=UINT32_C(0x40000000)|address;
   initial.records[n+1].size=1+(next()&UINT32_C(0x7FFFFFFF));
   if(mode==6 && n>=2){
    initial.records[1].address_flags=UINT32_C(0x80000100);initial.records[1].size=UINT32_C(0xFFFFFFF0);
    initial.records[2].address_flags=UINT32_C(0x800000F0);initial.records[2].size=(k%2)?16:32;
   }
  }
  actual=expected=initial;
  bind(&actual);_spu_gcSPU();
  bind(&expected);reference();
  if(memcmp(&actual,&expected,sizeof(actual))){fprintf(stderr,"case %u last%d mode%u failed (actual last%d expected%d)\n",k,n,mode,actual.last,expected.last);return 1;}
 }
 puts("131072 native cases: OK (UBSan)");return 0;
}
'''
        with TemporaryDirectory() as temporary:
            p=Path(temporary)
            source=p/'cleanup.c'
            source.write_text(prefix+s+reference)
            executable=p/'cleanup'
            subprocess.run(['clang','-std=gnu11','-O2','-fsanitize=undefined',
                            '-fno-sanitize-recover=all','-I'+str(ROOT/'include'),
                            str(source),'-o',str(executable)],check=True,capture_output=True,text=True)
            result=subprocess.run([str(executable)],check=True,capture_output=True,text=True)
            self.assertIn('131072 native cases: OK (UBSan)',result.stdout)


if __name__=='__main__':
    unittest.main()

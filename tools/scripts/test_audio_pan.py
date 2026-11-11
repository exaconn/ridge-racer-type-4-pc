"""Actual panning body versus independent arithmetic and helper snapshots."""
from pathlib import Path
from c_functions import definition_source, source_owners
import subprocess
from tempfile import TemporaryDirectory
import unittest

ROOT = Path(__file__).resolve().parents[2]


class AudioPanTest(unittest.TestCase):
    def test_actual_body_with_full_width_inputs_and_mutating_helpers(self):
        owner = source_owners(ROOT/'src/main/PAL')['TriggerSfx']
        source = definition_source(owner.read_text(), 'TriggerSfx')
        self.assertNotIn('asm', source, 'Actual panning body needs no native rewrite')
        prefix = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,s)
#define RR_ASSERT_OFFSET(t,f,o)
#include "rr/audio_pan.h"
_Static_assert(sizeof(AudioSettings)==52,"settings size");
_Static_assert(offsetof(AudioSettings,volume31)==49,"volume");
_Static_assert(offsetof(AudioSettings,mono32)==50,"mono");
typedef struct {s16 table[128];RuntimeSettings settings;s16 gain;u16 banks[3];u8 canary[18];} World;
typedef struct {s32 kind,args[8];World snapshot;} Entry;
static World initial,actual,expected,*w;
static Entry entries[2],saved[2];
static int count;
#define PanGainTable (w->table)
#define Options (w->settings)
#define SoundMasterGain (w->gain)
#define AudioPanCenterAngle (w->banks[0])
#define AudioPanRearAngle (w->banks[1])
#define AudioPanFrontAngle (w->banks[2])
static s32 emit(int kind,s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 g,s32 h){
 assert(count<2);Entry *p=&entries[count++];memset(p,0,sizeof(*p));
 p->kind=kind;p->args[0]=a;p->args[1]=b;p->args[2]=c;p->args[3]=d;
 p->args[4]=e;p->args[5]=f;p->args[6]=g;p->args[7]=h;p->snapshot=*w;
 w->gain^=0x7fff;w->settings.audio.volume31+=97;w->settings.audio.mono32^=1;
 for(int i=0;i<3;i++)w->banks[i]+=321;
 for(int i=0;i<128;i++)w->table[i]=(s16)((u16)w->table[i]+0x9f81U);
 return (s32)0xDEADBEEFU;
}
s32 SsUtKeyOn(s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 g){return emit(0,a,b,c,d,e,f,g,0);}
s32 SsUtKeyOnV(s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 g,s32 h){return emit(1,a,b,c,d,e,f,g,h);}
static int narrow(u32 v){v&=65535;return v<32768?(int)v:(int)v-65536;}
static int word(u32 v){return v<=INT32_MAX?(int)v:(int)((int64_t)v-INT64_C(4294967296));}
static void reference(s32 program,s32 level,s32 pan,s32 pitch,s32 voice){
 int bank_index=program<30?0:program<60?1:2;
 int bank=narrow(w->banks[bank_index]);
 int p=narrow((u32)program-30U*(unsigned)bank_index);
 int index=(u32)pan%128U;
 int l=narrow((u32)((int64_t)w->table[index]*narrow((u32)level)/16129));
 int r=narrow((u32)((int64_t)w->table[127-index]*narrow((u32)level)/16129));
 if(w->settings.audio.mono32)l=r=(l+r)/2;
 l=narrow((u32)((int64_t)l*w->gain/128));
 r=narrow((u32)((int64_t)r*w->gain/128));
 l=narrow((u32)((int64_t)l*w->settings.audio.volume31/16));
 r=narrow((u32)((int64_t)r*w->settings.audio.volume31/16));
 int raw=word((u32)pitch+60U),note=narrow((u32)(raw/128)),fine=narrow((u32)(raw%128));
 if(voice==-1){emit(0,bank,p,0,note,fine,r,l,0);emit(0,bank,p,1,note,fine,r,l,0);}
 else{int v=narrow((u32)voice*2U);emit(1,v,bank,p,0,note,fine,r,l);emit(1,narrow((u32)voice*2U+1U),bank,p,1,note,fine,r,l);}
}
static u32 seed=0xAB1CD765;
static u32 random_word(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
int main(void){
 const u32 edges[]={0,1,29,30,59,60,127,128,0xffff,0x10000,0x7fffffff,0x80000000,0xffffffff,0x8000,0x7fff,0x7fffffc4,0x8000003c};
 unsigned modes[2]={0,0};
 for(unsigned k=0;k<131072;k++){
  unsigned char *bytes=(unsigned char*)&initial;
  for(size_t j=0;j<sizeof(initial);j++)bytes[j]=(unsigned char)random_word();
  initial.settings.audio.mono32=(k&2)?(s16)random_word():0;
  initial.settings.audio.volume31=(u8)k;initial.gain=(s16)(u16)k;
  s32 program=word(k<sizeof(edges)/sizeof(*edges)?edges[k]:random_word());
  s32 level=word((random_word()&0xffff0000U)|(u16)k);
  s32 pan=word(random_word());
  s32 pitch=word(k<sizeof(edges)/sizeof(*edges)?edges[k]:random_word());
  s32 voice=(k&1)?-1:word(random_word());
  actual=expected=initial;w=&actual;count=0;memset(entries,0,sizeof(entries));
  TriggerSfx(program,level,pan,pitch,voice);assert(count==2);memcpy(saved,entries,sizeof(saved));
  w=&expected;count=0;memset(entries,0,sizeof(entries));reference(program,level,pan,pitch,voice);
  assert(count==2);assert(!memcmp(saved,entries,sizeof(saved)));assert(!memcmp(&actual,&expected,sizeof(actual)));
  modes[voice==-1]++;
 }
 assert(modes[0]&&modes[1]);puts("131072 native panning cases passed");
}
'''
        # Preserve the original World, model, PRNG and all 131072 cases. The gain and bank
        # lvalues cross into the real complete native owner.
        native_gain = r'''
static struct {u32 before[4];AudioState value;u32 after[4];} native_audio;
static AudioState native_baseline;
static u8 pointer_targets[14];
static int native_active,native_initialized;
#define SoundState native_audio.value
static void native_guard(void){
 AudioState observed;memcpy(&observed,&SoundState,sizeof(observed));
 observed.gain18=native_baseline.gain18;
 for(unsigned i=0;i<3;i++){unsigned bank=i<2?i:7;observed.banks24.ids[bank]=native_baseline.banks24.ids[bank];}
 assert(!memcmp(&observed,&native_baseline,sizeof(observed)));
 for(unsigned i=0;i<4;i++)assert(native_audio.before[i]==0xA7A7A7A7U&&native_audio.after[i]==0xA7A7A7A7U);
}
static void native_import(void){
 native_guard();SoundState.gain18=w->gain;
 for(unsigned i=0;i<3;i++){unsigned bank=i<2?i:7;SoundState.banks24.ids[bank]=(s16)w->banks[i];}
}
static void native_export(void){
 native_guard();w->gain=SoundState.gain18;
 for(unsigned i=0;i<3;i++){unsigned bank=i<2?i:7;w->banks[i]=(u16)SoundState.banks24.ids[bank];}
}
static void native_initialize(void){
 if(native_initialized)return;
 memset(&native_audio,0xA7,sizeof(native_audio));
 assert(sizeof(void*)==8&&(uintptr_t)&SoundState>UINT32_MAX&&(uintptr_t)pointer_targets>UINT32_MAX);
 for(unsigned i=0;i<2;i++){
  SoundState.streamBanks[i].table=pointer_targets+i;SoundState.seqAddr2858[i]=pointer_targets+12+i;
  SequenceChannel *c=&SoundState.sequences[i];
  c->c0=pointer_targets+2+i*5;c->c4=pointer_targets+3+i*5;c->c8=pointer_targets+4+i*5;
  c->cC=pointer_targets+5+i*5;c->p10=pointer_targets+6+i*5;
 }
 memcpy(&native_baseline,&SoundState,sizeof(native_baseline));native_initialized=1;
}
static void run_actual_pan(s32 program,s32 level,s32 pan,s32 pitch,s32 voice);
'''
        native_emit = r'''
static s32 native_emit(int kind,s32 a,s32 b,s32 c,s32 d,s32 e,s32 f,s32 g,s32 h){
 if(native_active)native_export();
 s32 result=emit(kind,a,b,c,d,e,f,g,h);
 if(native_active)native_import();return result;
}
'''
        native_entry = r'''
static void run_actual_pan(s32 program,s32 level,s32 pan,s32 pitch,s32 voice){
 native_initialize();native_import();native_active=1;
 TriggerSfx(program,level,pan,pitch,voice);
 native_export();native_active=0;
}
'''
        prefix = '#include "rr/audio_state_types.h"\n'+prefix
        prefix = prefix.replace('static int count;\n', 'static int count;\n'+native_gain)
        start = prefix.index('s32 SsUtKeyOn(')
        end = prefix.index('static int narrow(', start)
        mocks = prefix[start:end].replace('return emit(', 'return native_emit(')
        prefix = prefix[:start]+native_emit+mocks+prefix[end:]
        prefix = prefix.replace('  TriggerSfx(program,level,pan,pitch,voice);',
                                '  run_actual_pan(program,level,pan,pitch,voice);')
        with TemporaryDirectory() as directory:
            c = Path(directory) / 'pan.c'
            exe = Path(directory) / 'pan'
            c.write_text(prefix + source + native_entry)
            subprocess.run(['cc', '-std=gnu11', '-O2', '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined,bounds',
                            '-fno-sanitize-recover=all', '-I', str(ROOT / 'include'),
                            str(c), '-o', str(exe)], check=True, capture_output=True, text=True)
            result = subprocess.run([str(exe)], check=True, capture_output=True, text=True)
            self.assertIn('131072 native panning cases passed', result.stdout)

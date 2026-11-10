from pathlib import Path
import re,subprocess,tempfile,unittest
root=Path(__file__).resolve().parents[2]
header=''.join((root/'include/rr'/n).read_text() for n in ['tables.h','runtime_settings.h','menu_pulse.h'])
header=re.sub(r'^#include[^\n]*\n|^RR_ASSERT_[^\n]*\n','',header,flags=re.M)
from c_functions import source_owners,definition_source
body=definition_source(source_owners(root/'src/main/PAL')['PadStepPulse'].read_text(),'PadStepPulse');body=re.sub(r'^#include[^\n]*\n','',body,flags=re.M)
code=r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "types.h"
#include "game/system/pad.h"
'''+header+r'''
typedef struct {u8 guard0[32];ControllerState5C outputs[2];PadPulseState18 states[2];RuntimeSettings settings;u8 guard1[32];} World;
static World actual,expected;
static PulseRow rom[2][65552];
static const PulseRow *repeat_owner[12],*once_owner[12];
#define PlayerControllers actual.outputs
#define PadPulseState actual.states
#define Options actual.settings
#define D_8009FFA0 (repeat_owner+4)
#define D_8009FF80 (once_owner+4)
'''+body+r'''
static const PulseRow *mapped(const PulseRow *row) {
 uintptr_t a=(uintptr_t)&actual,b=(uintptr_t)row;
 if(b>=a && b+8<=a+sizeof(actual))return (const PulseRow *)((u8 *)&expected+(b-a));
 return row;
}
static s16 signed16(u16 x){s16 n;memcpy(&n,&x,2);return n;}
static void reference(void) {
 int i;for(i=0;i<2;i++) {
  ControllerState5C *out=expected.outputs+i;PadPulseState18 *state=expected.states+i;
  const PulseRow *row;
  if(expected.settings.controls.pulse[i].disabled02) {out->output19[0]=out->output19[1]=0;continue;}
  if(state->repeat04) {
   int table_index=state->repeat04+(out->kind1C?-1:2);
   row=mapped(D_8009FFA0[table_index]+state->repeatStep0C);
   if(out->kind1C){out->output19[0]=row->first;out->output19[1]=row->second;}
   else {out->output19[0]=64;out->output19[1]=row->first;}
   state->repeatTick08++;
   if(signed16(state->repeatTick08)>=row->duration) {
    state->repeatStep0C=signed16((u16)state->repeatStep0C+1);
    if(row[1].duration<0){state->repeatTick08=0;state->repeatStep0C=0;}
   }
  }else{out->output19[0]=out->output19[1]=0;}
  if(state->once06) {
   int table_index=state->once06+(out->kind1C?-1:2);
   row=mapped(D_8009FF80[table_index]+state->onceStep0E);
   if(out->kind1C){out->output19[0]=row->first;out->output19[1]=row->second;}
   else{out->output19[0]=64;out->output19[1]=row->first;}
   state->onceTick0A++;
   if(signed16(state->onceTick0A)>=row->duration) {
    state->onceStep0E=signed16((u16)state->onceStep0E+1);
    if(row[1].duration<0){state->once06=0;out->output19[0]=out->kind1C?0:64;out->output19[1]=0;}
   }
  }
 }
}
static u32 seed=0x7831ab8d;
static u32 next(void){seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed;}
int main(void) {
 unsigned n,i,j,k;u8 *raw=(u8 *)&actual;
 s16 steps[]={-32768,-1,0,1,2,32767};s16 modes[]={-32768,-1,0,1,32767};
 s16 durations[]={-32768,-1,0,1,32766,32767};u16 ticks[]={0,1,32766,32767,32768,65535};
 assert(sizeof(ControllerState5C)==0x5C && sizeof(PadPulseState18)==24 && sizeof(PulseRow)==4);
 for(i=0;i<2;i++)for(j=0;j<65552;j++){rom[i][j].first=next();rom[i][j].second=next();rom[i][j].duration=next();}
 for(n=0;n<32768;n++) {
  for(i=0;i<sizeof(actual);i++)raw[i]=next();
  for(i=0;i<12;i++){repeat_owner[i]=rom[0]+32768+i;once_owner[i]=rom[1]+32768+i;}
  for(i=0;i<2;i++) {
   PadPulseState18 *s=actual.states+i;actual.outputs[i].kind1C=modes[(n+i)%5];
   actual.settings.controls.pulse[i].disabled02=(n+i)%4==0?(u16)next():0;
   s->repeat04=(s16)((n+i)%5)-1;s->once06=(s16)((n/3+i)%5)-1;
   s->repeatStep0C=steps[(n+i)%6];s->onceStep0E=steps[(n/5+i)%6];
   s->repeatTick08=ticks[(n/7+i)%6];s->onceTick0A=ticks[(n/11+i)%6];
   for(k=0;k<2;k++) {
    s16 selector=k?s->once06:s->repeat04,step=k?s->onceStep0E:s->repeatStep0C;
    if(selector) {
     unsigned index=(unsigned)(selector+(actual.outputs[i].kind1C?-1:2)+4);
     PulseRow *row=rom[k]+32768+index+step;
     row->duration=durations[(n+i+k)%6];row[1].duration=(n+i+k)%2?-1:1;
    }
   }
  }
  if((n&31)==0) {
   actual.settings.controls.pulse[0].disabled02=0;actual.settings.controls.pulse[1].disabled02=1;actual.outputs[0].kind1C=1;
   actual.states[0].repeat04=1;actual.states[0].repeatStep0C=0;actual.states[0].once06=0;
   repeat_owner[4]=(PulseRow *)((u8 *)actual.outputs+0x18);
  }
  memcpy(&expected,&actual,sizeof(actual));reference();PadStepPulse();
  assert(!memcmp(&actual,&expected,sizeof(actual)));
 }
 puts("32768 two-port pulse cases passed with UBSan");return 0;
}
'''
class MenuPulseTest(unittest.TestCase):
 def test_native_two_port_sequences(self):
  with tempfile.TemporaryDirectory() as tmp:
   c=Path(tmp)/'pulse.c';exe=Path(tmp)/'pulse';c.write_text(code)
   subprocess.run(['cc','-std=gnu99','-O2','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(root/'include'),str(c),'-o',str(exe)],check=True)
   subprocess.run([str(exe)],check=True)

if __name__=='__main__': unittest.main()

"""Movie directory conversion: full native pointer ABI and live entry aliases.

The CD converter is a stateful software mock. No physical disc is accessed.
"""
from pathlib import Path
from c_functions import definition_source
from audio_fixture_transport import wire_types, transport
from tempfile import TemporaryDirectory
import hashlib,json,os,re,subprocess
ROOT=Path(__file__).resolve().parents[2]
SOURCE=ROOT/'src/overlays/PAL/668/func_80114910.c'
CASES=8192
C=r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rr/layout.h"
#undef RR_ASSERT_SIZE
#undef RR_ASSERT_OFFSET
#define RR_ASSERT_SIZE(t,n) _Static_assert(sizeof(void*)!=4 || sizeof(t)==n,"PAL size")
#define RR_ASSERT_OFFSET(t,f,n) _Static_assert(sizeof(void*)!=4 || offsetof(t,f)==n,"PAL offset")
#include "rr/overlay668_resource.h"
typedef struct {u8 header[0x130];StrArchiveEntry entries[16];u8 tail[16];} Archive;
typedef struct {StrContextPrefix prefix;u8 tail[0x4B8-0xD4];} Context;
typedef struct {
 u8 guard[16];StrArchiveAudioState audio;Context context;Archive archives[2];
 StrCdFileInfo file;u32 sectors;u8 active[64],end[16];
} World;
static World actual,expected;
static unsigned scenario,bank,id,alias,calls;
static const u32 sectorCounts[8]={0,1,119,120,121,0x7fffffffU,0x80000000U,0xffffffffU};
#define SoundState actual.audio
#define RaceContext actual.context.prefix
static u32 *output(World *w){switch(alias){
 case 0:return &w->sectors;
 case 1:return &w->file.byteSize;
 case 2:return &w->context.prefix.thresholdC8;
 case 3:return &w->context.prefix.budgetCC;
 case 4:return &w->archives[bank].entries[id].sectors;
 case 5:return &w->archives[bank].entries[id].position;
 case 6:return &w->audio.sector2634;
 default:return (u32*)&w->context.prefix.counterC4;
}}
u8 *CdIntToPos(s32 sector,u8 *location){
 unsigned i;u32 value=(actual.archives[bank].entries[id].position&0x0fffffffU)+actual.audio.sector2634;
 assert(calls++==0 && (u32)sector==value && location==actual.file.location.c);
 assert((uintptr_t)location>UINT32_MAX && (uintptr_t)actual.audio.archive2638>UINT32_MAX);
 /* A real full file pointer reaches the converter. The fourth location byte
  * is deliberately untouched; all later copies must preserve it. */
 for(i=0;i<3;i++)location[i]=(u8)((value>>(i*8))^(0x31+i*17));
 actual.archives[bank].entries[id].sectors=(scenario/256)%9<8?sectorCounts[(scenario/256)%9]:scenario*0x9E3779B9U;
 actual.archives[bank].entries[id].position=~(scenario*0x85EBCA6BU);
 actual.audio.archive2638=(u8*)&actual.archives[bank^1];actual.audio.sector2634=~value;
 actual.context.prefix.activeB8=&actual.active[13];
 actual.context.prefix.counterC4=(s32)(scenario*97);actual.context.prefix.phaseB4=(s32)(scenario*31);
 /* Independent expected whole-world state starts after the service effects.
  * Resolve aliases in the cloned owner; no expected pointer is dereferenced
  * through the copied archive root. */
 memcpy(&expected,&actual,sizeof expected);
 value=expected.archives[bank].entries[id].sectors;
 expected.file.byteSize=value*2048U;
 *output(&expected)=expected.archives[bank].entries[id].sectors;
 memcpy(&expected.context.prefix.locationBC,&expected.file.location,4);
 memcpy(&expected.context.prefix.currentC0,&expected.file.location,4);
 value=*output(&expected)/6U-20U;
 expected.context.prefix.thresholdC8=value;expected.context.prefix.budgetCC=value*2U;
 return location;
}
'''
TAIL=r'''
int main(void){unsigned i,b,j;unsigned seen[8]={0},counts[16]={0},roots[2]={0};
 _Static_assert(sizeof(void*)==8,"full native pointers");
 _Static_assert(sizeof(StrArchiveEntry)==8,"directory stride");
 _Static_assert(offsetof(StrArchiveAudioState,archive2638)==0x2638,"native archive offset");
 _Static_assert(sizeof(StrArchiveAudioState)==0xA8D8,"full native audio owner");
 _Static_assert(offsetof(StrContextPrefix,locationBC)==0xC0,"native context offset");
 for(scenario=0;scenario<8192;scenario++){
  for(i=0;i<sizeof actual;i++)((u8*)&actual)[i]=(u8)(scenario*29+i*7);
  id=scenario%16;bank=(scenario/16)%2;alias=(scenario/32)%8;calls=0;
  for(b=0;b<2;b++)for(j=0;j<16;j++){
   actual.archives[b].entries[j].position=scenario*0x9E3779B9U+b*65537+j*97;
   actual.archives[b].entries[j].sectors=~(scenario*0x85EBCA6BU+b*131071+j*193);
  }
  actual.audio.archive2638=(u8*)&actual.archives[bank];
  actual.context.prefix.activeB8=&actual.active[7];
  ovl668_resolve_stream_file(id,&actual.file,output(&actual));
  assert(calls==1 && !memcmp(&actual,&expected,sizeof actual));
  seen[alias]++;counts[id]++;roots[bank]++;
 }
 for(i=0;i<8;i++)assert(seen[i]);for(i=0;i<16;i++)assert(counts[i]);for(i=0;i<2;i++)assert(roots[i]);
 puts("PASS 8192 native cases; full owners and pointers, eight output aliases, live entry and root replacement");
 return 0;
}
'''
def main():
 body=definition_source(SOURCE.read_text(),'ovl668_resolve_stream_file')
 body=re.sub(r'__asm__\("\$\d+"\)','',body)
 # Preserve the original complete native archive fixture and all byte-pattern
 # inputs. Only the current production body sees the expanded AudioState.
 legacy=r'''
typedef struct {
 u8 prefix[0x2634];u32 sector2634;u8 *archive2638;
 u8 suffix[0xA8D0-0x263C];
} StrArchiveAudioState;
'''
 prefix=wire_types()+legacy+C
 position=prefix.index('u8 *CdIntToPos(')
 bridge=transport('archive_wire')
 bridge=bridge.replace('audio_native.value.streamBanks[i].table=targets+i;',
                       'audio_native.value.streamBanks[i].table=i?targets+i:archive_root_expected;')
 bridge=bridge.replace('a->streamBanks[i].table==t+i',
                       'a->streamBanks[i].table==(i?t+i:archive_root_expected)')
 adapter=r'''
static AudioWireState archive_wire;
static u8 *archive_root_expected;
'''+bridge+r'''
static void archive_import(void){
 memcpy(&archive_wire,&actual.audio,0x2638);
 memcpy((u8 *)&archive_wire+0x263C,actual.audio.suffix,sizeof(actual.audio.suffix));
 audio_import();
 archive_root_expected=actual.audio.archive2638;
 audio_native.value.streamBanks[0].table=archive_root_expected;
}
static void archive_export(void){
 audio_export();
 memcpy(&actual.audio,&archive_wire,0x2638);
 memcpy(actual.audio.suffix,(u8 *)&archive_wire+0x263C,sizeof(actual.audio.suffix));
}
'''
 prefix=prefix[:position]+adapter+prefix[position:]
 prefix=prefix.replace('u8 *CdIntToPos(s32 sector,u8 *location){',
                       'static u8 *model_CdIntToPos(s32 sector,u8 *location){')
 service=r'''
u8 *CdIntToPos(s32 sector,u8 *location){
 archive_export();u8 *result=model_CdIntToPos(sector,location);
 archive_import();return result;
}
#undef SoundState
#define SoundState (audio_native.value)
'''
 body=body.replace('void ovl668_resolve_stream_file(', 'static void native_resource(')
 wrapper=r'''
void ovl668_resolve_stream_file(s32 index,StrCdFileInfo *file,u32 *sectors){
 archive_import();
 u32 *native_output=sectors==&actual.audio.sector2634
     ?&audio_native.value.streamBanks[0].sector:sectors;
 native_resource(index,file,native_output);archive_export();
}
#undef SoundState
#define SoundState actual.audio
'''
 with TemporaryDirectory() as tmp:
  c=Path(tmp)/'test.c';exe=Path(tmp)/'test';c.write_text(prefix+service+body+wrapper+TAIL)
  subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-Wno-error=deprecated-non-prototype','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-I',str(ROOT/'include'),str(c),'-o',str(exe)],check=True)
  subprocess.run([str(exe)],check=True)
 sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
 deps=[SOURCE,Path(__file__).resolve()]+list((ROOT/'include').rglob('*.h'))
 audit=dict(cases=CASES,engine='native C with stateful CD boundary',source_sha256=sha(SOURCE),test_sha256=sha(__file__),full_host_pointers=True,full_owner_guards=True,output_aliases=8,resource_indices=16,archive_roots=2,sector_count_profiles=[0,1,119,120,121,2147483647,2147483648,4294967295,"seeded"],converter_arguments=2,mutating_converter=True,captured_entry_reloaded=True,stack_layout='existing unused eight-byte local retained for PAL compiler frame; no program data',sanitizers=['undefined','bounds'],dependencies={str(p):sha(p) for p in deps})
 if os.environ.get('RR_AUDIT_JSON'):Path(os.environ['RR_AUDIT_JSON']).write_text(json.dumps(audit,indent=2)+'\n')
if __name__=='__main__':main()

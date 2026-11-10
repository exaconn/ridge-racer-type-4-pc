"""Test-only PAL protocol transport into the complete pointer-bearing audio owner.

Legacy scalar models keep their original bytes, case generation and oracles.
Production bodies use the real AudioState. Unused native pointer cells and all
native alignment gaps are guarded; consumed pointers need an explicit adapter.
"""
import re

_WIRE_TYPES = r'''
typedef struct {
 u32 flags00; s16 bank04; u16 reserved06;
 s32 voice08,count0C,level10,program14,previous18,reserved1C;
 AudioLevels4 levels20; s16 pan24,reserved26,group28,reserved2A; AudioSpatial4 spatial2C;
 s32 filter30; u8 reserved34[8];
 AudioCurve48 curves3C[2][10];
 u16 programs5DC[2][5];
 u16 duration5F0,gain5F2,spread5F4,reserved5F6;
 AudioPitch4 pitch5F8[2][5];
} AudioWireSlot620;
typedef struct {
    s32 unk00,muted04,master08,active0C,split10,reserved14;
    s16 gain18,fadeLength1A,fadeTick1C,fadeTarget1E,fadeStart20,fadeReserved22;
    s16 bank24[10];
    AudioWireSlot620 slots38[4];
    s32 active18B8,unk18BC;
    u16 mode18C0,submode18C2;
    s32 pos18C4[4];
    AudioRot10 rot18D4;
    s32 unk18E4,unk18E8;
    AudioListenerRecord1C rec18EC[4];
    AudioPlayerVoice1C playerVoices195C[2];
    AudioLoopVoice14 loopVoices1994[2];
    u16 chVolL19BC[2],chVolR19C0[2];
    s32 chActive19C4;
    AudioSpatial4 chProg19C8;
    s32 unk19CC;
    SoundSequenceState sequence19D0;
    u16 left19F8,right19FA;u32 pitch19FC;s32 active1A00;s16 program1A04;u8 reserved1A06[2];
    s32 unk1A08,unk1A0C[7],unk1A28,unk1A2C,unk1A30;
    u8 reserved1A34[0x2634-0x1A34];
    s32 streamBase2634,streamTable2638,unk263C,unk2640;
    u8 reserved2644[0x2658-0x2644];
    u8 cdMode2658,reserved2659[3];
    u8 status265C[8];
    s32 sector2664,sector2668,filter266C,action2670,phase2674,step2678,flag267C,flag2680,reserved2684;
    s32 position2688,previous268C;u32 end2690;
    s32 reserved2694,index2698,state269C,flag26A0,timer26A4,timer26A8,state26AC,reserved26B0,request26B4;
    s32 pausedState26B8,playActive26BC;
    AudioTargetVol4 targetVol26C0;
    u16 currentVol26C4;
    s16 fadeLength26C6,fadeTick26C8;
    u16 fadeTarget26CA,fadeStart26CC,volume26CE;
    s16 tick26D0,half26D2,quarter26D4,reserved26D6;
    s32 tick26D8,previous26DC,reserved26E0,reserved26E4,offset26E8,reserved26EC,base26F0,rate26F4;
    u8 reserved26F8[0x2858-0x26F8];
    s32 seqAddr2858[2];
    s16 seqInit2860,seqInit2862;
    s32 seqHandle2864,seqTrack2868;
    s16 seqTick286C,seqHalf286E,seqQuarter2870,seqReserved2872;
    u32 seqBeat2874,seqPrevBeat2878;
    s32 seqTempo287C,seqMark2880,seqReserved2884;
    u16 seqVol2888;
    s16 seqFadeLen288A,seqFadeTick288C,seqFadeTarget288E,seqFadeStart2890,seqFadeReserved2892;
    s32 revType2894,revDepthL2898,revDepthR289C,revMask28A0;
    s32 track28A4,count28A8,displayed28AC;
    u8 reserved28B0[24];AudioTocEntry8 toc28C8[16][256];
    s32 reservedA8C8,bankA8CC;
} AudioWireState;
'''


def wire_types():
    # Compile the actual public assertions before a legacy fixture overrides
    # unrelated assertion macros for its own protocol objects.
    return '#include "rr/audio_state_types.h"\n' + _WIRE_TYPES


def scalar_body(body):
    assert not re.search(r'(?:\.|->)(?:streamBanks|sequences|seqAddr2858)\b', body), body
    return body


def transport(owner):
    """owner is a real AudioWireState lvalue in the original fixture world."""
    return r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
_Static_assert(sizeof(AudioWireState)==0xA8D0,"unchanged PAL oracle protocol");
_Static_assert(sizeof(AudioWireSlot620)==0x620,"unchanged slot protocol");
_Static_assert(sizeof(AudioState)==0xA920,"complete LP64 owner");
_Static_assert(sizeof(void *)==8,"exercise native pointers");
static struct {u8 before[16];AudioState value;u8 after[16];} audio_native;
static u8 audio_pointer_targets[14];
static int audio_initialized,audio_active,audio_callbacks;
static AudioWireState *audio_model(void){return &(MODEL_OWNER);}
static void audio_initialize(void){
    unsigned i;u8 *targets=audio_pointer_targets;
    if(audio_initialized)return;
    memset(&audio_native,0xA7,sizeof(audio_native));
    assert((uintptr_t)&audio_native>UINT32_MAX && (uintptr_t)targets>UINT32_MAX);
    for(i=0;i<2;i++){
        SequenceChannel *r=&audio_native.value.sequences[i];
        audio_native.value.streamBanks[i].table=targets+i;
        r->c0=targets+2+i*5;r->c4=targets+3+i*5;r->c8=targets+4+i*5;
        r->cC=targets+5+i*5;r->p10=targets+6+i*5;
        audio_native.value.seqAddr2858[i]=targets+12+i;
    }
    audio_initialized=1;
}
static void audio_guard(void){
    unsigned i,j;AudioState *a=&audio_native.value;const u8 *p=(const u8 *)a;
    for(i=0;i<16;i++)assert(audio_native.before[i]==0xA7&&audio_native.after[i]==0xA7);
    for(i=0x2634;i<offsetof(AudioState,streamBanks);i++)assert(p[i]==0xA7);
    for(i=0x26F8+AUDIO_CD_DELTA;i<offsetof(AudioState,sequences);i++)assert(p[i]==0xA7);
    for(i=0;i<2;i++){
        SequenceChannel *r=&a->sequences[i];u8 *t=audio_pointer_targets;
        for(j=4;j<offsetof(AudioStreamBank,table);j++)assert(((u8 *)&a->streamBanks[i])[j]==0xA7);
        assert(a->streamBanks[i].table==t+i && a->seqAddr2858[i]==t+12+i);
        assert(r->c0==t+2+i*5 && r->c4==t+3+i*5 && r->c8==t+4+i*5);
        assert(r->cC==t+5+i*5 && r->p10==t+6+i*5);
        for(j=offsetof(SequenceChannel,wac)+4;j<sizeof(*r);j++)assert(((u8 *)r)[j]==0xA7);
    }
}
static void audio_import(void){
    unsigned i;const u8 *wire=(const u8 *)audio_model();AudioState *a=&audio_native.value;
    audio_initialize();audio_guard();
    memcpy(a,wire,0x2634);
    for(i=0;i<2;i++)memcpy(&a->streamBanks[i].sector,wire+0x2634+i*8,4);
    memcpy(a->reserved2644,wire+0x2644,0x26F8-0x2644);
    for(i=0;i<2;i++)memcpy((u8 *)&a->sequences[i]+offsetof(SequenceChannel,b14),wire+0x26F8+i*176+20,156);
    memcpy(&a->seqInit2860,wire+0x2860,0xA8D0-0x2860);
}
static void audio_export(void){
    unsigned i;u8 *wire=(u8 *)audio_model();AudioState *a=&audio_native.value;
    audio_guard();memcpy(wire,a,0x2634);
    for(i=0;i<2;i++)memcpy(wire+0x2634+i*8,&a->streamBanks[i].sector,4);
    memcpy(wire+0x2644,a->reserved2644,0x26F8-0x2644);
    for(i=0;i<2;i++)memcpy(wire+0x26F8+i*176+20,(u8 *)&a->sequences[i]+offsetof(SequenceChannel,b14),156);
    memcpy(wire+0x2860,&a->seqInit2860,0xA8D0-0x2860);
}
static size_t audio_native_offset(size_t offset){
    if(offset<0x2634)return offset;
    if(offset>=0x2644&&offset<0x26F8)return offset+AUDIO_CD_DELTA;
    if(offset>=0x2860&&offset<0xA8D0)return offset+AUDIO_SEQUENCE_DELTA;
    if(offset>=0x26F8&&offset<0x2858){
        size_t row=(offset-0x26F8)/176,field=(offset-0x26F8)%176;
        assert(field>=20);return offsetof(AudioState,sequences)+row*sizeof(SequenceChannel)+offsetof(SequenceChannel,b14)+field-20;
    }
    if(offset==0x2634||offset==0x263C)return offsetof(AudioState,streamBanks)+(offset-0x2634)/8*sizeof(AudioStreamBank);
    assert(!"extend pointer-field transport explicitly");return 0;
}
static size_t audio_wire_offset(size_t offset){
    if(offset<0x2634)return offset;
    if(offset>=offsetof(AudioState,reserved2644)&&offset<0x26F8+AUDIO_CD_DELTA)return offset-AUDIO_CD_DELTA;
    if(offset>=offsetof(AudioState,seqInit2860)&&offset<0xA8D0+AUDIO_SEQUENCE_DELTA)return offset-AUDIO_SEQUENCE_DELTA;
    if(offset>=offsetof(AudioState,sequences)&&offset<offsetof(AudioState,seqAddr2858)){
        size_t at=offset- offsetof(AudioState,sequences),row=at/sizeof(SequenceChannel),field=at%sizeof(SequenceChannel);
        assert(field>=offsetof(SequenceChannel,b14)&&field<offsetof(SequenceChannel,wac)+4);
        return 0x26F8+row*176+20+field- offsetof(SequenceChannel,b14);
    }
    if(offset==offsetof(AudioState,streamBanks)||offset==offsetof(AudioState,streamBanks)+sizeof(AudioStreamBank))
        return 0x2634+(offset- offsetof(AudioState,streamBanks))/sizeof(AudioStreamBank)*8;
    assert(!"extend pointer-field transport explicitly");return 0;
}
static inline __attribute__((unused)) void *audio_native_pointer(const void *pointer){
    uintptr_t first=(uintptr_t)audio_model(),value=(uintptr_t)pointer;
    if(value>=first&&value-first<sizeof(AudioWireState))return (u8 *)&audio_native.value+audio_native_offset(value-first);
    return (void *)pointer;
}
static inline __attribute__((unused)) void *audio_model_pointer(const void *pointer){
    uintptr_t first=(uintptr_t)&audio_native.value,value=(uintptr_t)pointer;
    if(value>=first&&value-first<sizeof(AudioState))return (u8 *)audio_model()+audio_wire_offset(value-first);
    return (void *)pointer;
}
/* A service buffer must remain entirely within scalar fields. Whole-owner
 * initialization and pointer consumers require their own explicit transport. */
static inline __attribute__((unused)) void *audio_model_buffer(const void *pointer,size_t size){
    uintptr_t first=(uintptr_t)&audio_native.value,value=(uintptr_t)pointer;
    if(value>=first && value-first<sizeof(AudioState)){
        size_t at=value-first,wire=audio_wire_offset(at),i;
        assert(size<=sizeof(AudioState)-at);
        for(i=0;i<size;i++)assert(audio_wire_offset(at+i)==wire+i);
        return (u8 *)audio_model()+wire;
    }
    return (void *)pointer;
}
typedef struct {int export_on_return,callbacks;} AudioCallFrame;
static inline __attribute__((unused)) AudioCallFrame audio_enter(void){
    AudioCallFrame frame={!audio_active||audio_callbacks,audio_callbacks};
    if(frame.export_on_return)audio_import();
    audio_callbacks=0;audio_active++;return frame;
}
static inline __attribute__((unused)) void audio_leave(AudioCallFrame frame){
    assert(audio_active>0);audio_active--;
    if(frame.export_on_return)audio_export();audio_callbacks=frame.callbacks;
}
static inline __attribute__((unused)) void audio_before_callback(void){
    if(audio_active)audio_export();audio_callbacks++;
}
static inline __attribute__((unused)) void audio_after_callback(void){
    assert(audio_callbacks>0);audio_callbacks--;if(audio_active)audio_import();
}
'''.replace('MODEL_OWNER', owner)


def scalar_entry(body, name, owner, result="void", parameters="void", arguments="", aliases=None):
    """Run an actual definition on the complete owner around its original model.

    owner names the wire object directly, without relying on SoundState macros.
    Mocks must export/import at callback boundaries and map owned pointers.
    Production field accesses are never rewritten by this helper.
    """
    body = scalar_body(body)
    native = 'audio_native_' + name
    body, count = re.subn(r'\b' + re.escape(name) + r'\s*\(', native + '(', body, count=1)
    assert count == 1, name
    code = transport(owner) + '\n#undef SoundState\n#define SoundState (audio_native.value)\n'
    for alias, field in (aliases or {}).items():
        code += '\n#undef ' + alias + '\n#define ' + alias + ' (audio_native.value.' + field + ')\n'
    code += body + '\n' + result + ' ' + name + '(' + parameters + '){\n AudioCallFrame frame=audio_enter();\n '
    call = native + '(' + arguments + ')'
    if result == 'void':
        code += call + ';\n audio_leave(frame);\n}\n'
    else:
        code += result + ' audio_entry_result=' + call + ';\n audio_leave(frame);\n return audio_entry_result;\n}\n'
    code += '\n#undef SoundState\n#define SoundState (' + owner + ')\n'
    for alias, field in (aliases or {}).items():
        code += '\n#undef ' + alias + '\n#define ' + alias + ' ((' + owner + ').' + field + ')\n'
    return code

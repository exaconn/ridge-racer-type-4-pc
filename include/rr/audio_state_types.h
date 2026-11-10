#ifndef RR_AUDIO_STATE_TYPES_H
#define RR_AUDIO_STATE_TYPES_H
#include "rr/layout.h"
#include "rr/sound_sequence_types.h"
/* Full resident audio owner: func8004EC28 clears0xA8D0 bytes from this base.
 * Four620-byte slots start38. CD status/control occupies265C..26F8.
 * The TOC address is28C8+bank*2048+entry*8; sixteen256-entry banks endA8C8.
 * D_800F746C is the request26B4 alias, not a separate allocation. */
typedef struct { s32 position,other04; } AudioTocEntry8;
typedef struct { s32 keys[9], values[9]; } AudioCurve48;
typedef struct { u16 coarse, fine; } AudioPitch4;
typedef union { s32 word; struct {s16 level,gain;} halves; } AudioLevels4;
typedef union { s32 word; struct {s16 value,reserved;} halves; } AudioSpatial4;
typedef union { s32 word; struct {u16 value,reserved;} halves; } AudioTargetVol4;
typedef s32 AudioBankStorage32 __attribute__((__may_alias__));
typedef union {
    struct {
        u16 rx00, unk02;
        u16 ry04, unk06;
        u16 rz08, unk0A;
        s32 unk0C;
    } r;
    struct { s32 a, b, c, d; } v4;
} AudioRot10;
typedef struct {
    s32 mode00;
    s16 carIndex04, changed06;
    s32 dist08, prevDist0C, deltaDist10;
    u16 pan14, vol16;
    s32 depth18;
} AudioListenerRecord1C;
typedef struct {
    s32 bank00, voice04, mode08, prev0C, pitch10, vol14, slot18;
} AudioPlayerVoice1C;
typedef struct {
    s32 enabled00, active04;
    AudioSpatial4 bank08, voice0C;
    s16 vol10[2];
} AudioLoopVoice14;
typedef struct {
 u32 flags00; s16 bank04; u16 reserved06;
 s32 voice08,count0C,level10,program14,previous18,reserved1C;
 AudioLevels4 levels20; s16 pan24,reserved26,group28,reserved2A; AudioSpatial4 spatial2C;
 s32 filter30; u8 reserved34[8];
 AudioCurve48 curves3C[2][10];
 u16 programs5DC[2][5];
 u16 duration5F0,gain5F2,spread5F4,reserved5F6;
 AudioPitch4 pitch5F8[2][5];
} AudioSlot620;
RR_ASSERT_SIZE(AudioSlot620,0x620);
RR_ASSERT_OFFSET(AudioSlot620,programs5DC,0x5DC);
RR_ASSERT_OFFSET(AudioSlot620,pitch5F8,0x5F8);
typedef struct {
    s32 unk00,muted04,master08,active0C,split10,reserved14;
    s16 gain18,fadeLength1A,fadeTick1C,fadeTarget1E,fadeStart20,fadeReserved22;
    s16 bank24[10];
    AudioSlot620 slots38[4];
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
} AudioState;
RR_ASSERT_SIZE(AudioState,0xA8D0);
RR_ASSERT_OFFSET(AudioState,sequence19D0,0x19D0);
RR_ASSERT_OFFSET(AudioState,left19F8,0x19F8);
RR_ASSERT_OFFSET(AudioState,track28A4,0x28A4);
RR_ASSERT_OFFSET(AudioState,status265C,0x265C);
RR_ASSERT_OFFSET(AudioState,sector2664,0x2664);
RR_ASSERT_OFFSET(AudioState,phase2674,0x2674);
RR_ASSERT_OFFSET(AudioState,request26B4,0x26B4);
RR_ASSERT_OFFSET(AudioState,tick26D0,0x26D0);
RR_ASSERT_OFFSET(AudioState,rate26F4,0x26F4);
RR_ASSERT_OFFSET(AudioState,toc28C8,0x28C8);
RR_ASSERT_OFFSET(AudioState,bankA8CC,0xA8CC);
extern AudioState SoundState;
extern AudioSlot620 SoundSlots[4];
#endif

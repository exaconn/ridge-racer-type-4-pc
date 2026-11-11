#ifndef RR_CHANNELS_H
#define RR_CHANNELS_H

#include "rr/layout.h"
#include "rr/sequence_channel.h"

typedef void (*ChannelUpdateCallback)(void *);
typedef s32 (*ChannelStatusCallback)(void *);

/* Five-byte actuator descriptions returned by command 0x46. */
typedef struct {u8 opaque00[2],all_bits02,cost03,opaque04;} ChannelGateEntry5;
typedef struct {
    u8 count00;
    u8 reserved01[3];
    u8 *buttons04;
} ChannelCombEntry8;

/* PAL metadata has a 128-byte protocol budget. Native descriptor pointers
 * need additional storage and alignment without enlarging that wire budget. */
enum { PAD_ACT_INFO_BYTES=0x80, PAD_COMB_WIRE_BYTES=8,
       PAD_MAX_COMBINATIONS=PAD_ACT_INFO_BYTES/PAD_COMB_WIRE_BYTES };

/* The transfer gates and six output IDs occupy one contiguous byte area.
 * Payload cursors may also address the three bytes preceding the IDs. */
typedef union {
    u8 bytes[15];
    struct {u8 gates[9],payload[6];} fields;
} ChannelOutput15;

typedef struct PadChannel {
    u16 *modes00;
    ChannelGateEntry5 *entries04;
    ChannelCombEntry8 *combs08;
    struct PadChannel *children0C;
    struct PadChannel *owner10;
    ChannelUpdateCallback update14;
    ChannelStatusCallback complete18;
    u8 reserved1C[4];
    u8 *layout20;
    u8 argument24,reserved25[3];
    u8 *data28,*data2C,*input30;
    u8 length34,pad35,length36,command37,previous38,flag39,reserved3A[2];
    u8 *packet,*buffer40;
    u8 b44,b45,phase,state47,flag48,state49,retry4A,reserved4B;
    s32 type4C;
    u8 b50,modeArgs51[2],modeEqual53;
    ChannelOutput15 output54;
    u8 metadata63[PAD_ACT_INFO_BYTES
        + PAD_MAX_COMBINATIONS*(sizeof(ChannelCombEntry8)-PAD_COMB_WIRE_BYTES)
        + (__alignof__(ChannelCombEntry8)-4)];
    u8 modeCountE3,argumentE4,padE5;
    u16 pending;
    u8 responseE8,entry_countE9,combCountEA,scanIndexEB;
    u16 scanOffsetEC,scanSeenEE;
} PadChannel;

/* Accessed payload pointers must cover offsets -3..-1 for phases 0..2. */
/* The driver computes the second argument for its byte-provider callback.
 * This installed implementation selects bytes from channel state alone. */
typedef u32 (*ChannelByteCallback)(const PadChannel *channel, s32 transferMode);
u32 _padGetTxByte(const PadChannel *channel, s32 transferMode);
void _padSaveCommand(PadChannel *channel);

/* Partial layout of the 0x36-byte control rows beginning at _svm_voice. */
typedef struct {
    s16 value00;
    u16 age02;
    u16 value04;
    u16 value06;
    s16 unk08;
    u8 unk0A;
    u8 pad0B;
    s16 unk0C;
    s16 group0E, match10;
    s16 unk12;
    s16 match14;
#ifndef __mips__
    union {
        u8 pad16[2];
        s16 unk16;
    };
#else
    s16 unk16;
#endif
    s16 match18;
    s16 priority1A;
    u8 pad1C;
    s8 flags1D;
#ifndef __mips__
    union {
        u8 pad1E[24];
        struct {
            s16 field1E;
            s16 field20;
            s16 field22;
            s16 field24;
            s16 field26;
            s16 field28;
            s16 field2A;
            s16 field2C;
            s16 field2E;
            s16 field30;
            s16 field32;
            s16 field34;
        };
    };
#else
    s16 field1E;
    s16 field20;
    s16 field22;
    s16 field24;
    s16 field26;
    s16 field28;
    s16 field2A;
    s16 field2C;
    s16 field2E;
    s16 field30;
    s16 field32;
    s16 field34;
#endif
} ChannelControlRow36;

typedef struct {
    s8 f00, f01, f02, f03, f04;
    u8 f05;
    s8 f06, f07, f08, f09, f0A;
    u8 f0B;
    s8 f0C, f0D;
    u8 f0E;
    s8 f0F, f10, f11;
    u8 f12;
    s8 f13;
    s16 f14, f16, f18;
} SvmCur;

/* Sequence rows use the complete shared owner from sequence_channel.h. */
extern SvmCur _svm_cur;
extern s16 _svm_cur_vab_id;
extern u16 _svm_voice_cursor;
extern s32 _snd_ev_flag;

/* Reset uses the global selector; the caller-supplied mode is unused. */
extern void _SsVmKeyOffNow(s32 unused_mode);
extern void vmNoiseOff(u8 index);
extern ChannelControlRow36 _svm_voice[];
extern u32 _snd_vmask;
/* Uses the valid active-channel count established by SsSetReservedVoice. */
s32 _SsVmKeyOff(s32 match10, s32 match18, s32 match14, u32 group0E);
/* Reset all unblocked rows matching the signed low16 match10 value.
 * Uses the active-channel count1..24 established by SsSetReservedVoice. */
void _SsVmSeqKeyOff(s32 group);
extern s32 SsUtKeyOffV(s32 selected);

/* Active channel count, accepted values are 1 through 24 after byte wrapping. */
extern s8 spuVmMaxVoice;
extern s32 SsSetReservedVoice(u32 count);

typedef void *(*ChannelRecordCallback)(s32 selector);
extern ChannelRecordCallback PadPortToChannelFn;
s32 PadSetMainMode(s32 selector, s32 first, s32 second);
s32 _padSetMainMode(PadChannel *channel, s32 first, s32 second);
void _padMainModeUpdate(void *channel);
s32 _padMainModeComplete(void *channel);

extern PadChannel *PadChannelRecords;
extern ChannelUpdateCallback PadResetChannelFn;
extern s32 PadComActive;
void _padStartCom(void);

typedef void (*ChannelErrorCallback)(s32 status);
extern ChannelErrorCallback PadAdvanceChannelFn;
extern s32 D_800ABCD0, PadFirstChannel, PadLastChannel;
extern s32 PadActiveChannel, PadRecvStage, PadTotalGateCost;
extern s32 PadPortTimeoutCounts[2];
s32 _padInitSioMode(PadChannel *channel);
/* Entry stage must be in 0..4; the handler sees its incremented value. */
typedef s32 (*ChannelStageCallback)(PadChannel *channel);
extern ChannelStageCallback PadRecvStageTable[5];
void _padSioStageStep(PadChannel *channel);
s32 _padIntPad(void);

extern ChannelStatusCallback PadChannelDispatchFn, PadChannelBusyFn;
extern ChannelUpdateCallback PadPacketDoneCallback;

extern s32 _padChannelDispatch(void *channel);
void _padCmdParaMode(void *channel, s32 value);
void _padCmd45(void *channel);
void _padCmd4C(void *channel, s32 value);
void _padCmd46(void *channel, s32 value);
void _padCmd47(void *channel, s32 value);
void _padCmd4B(void *channel);
void _padSendAtLoadInfo(u8 *channel);
extern s32 _padChannelBusy(void *channel);
extern void _padPacketDone(void *channel);

RR_ASSERT_SIZE(ChannelGateEntry5,5);
RR_ASSERT_OFFSET(ChannelGateEntry5,all_bits02,2);
RR_ASSERT_OFFSET(ChannelGateEntry5,cost03,3);
RR_ASSERT_SIZE(ChannelOutput15,15);
#if defined(__mips__)
RR_ASSERT_SIZE(ChannelCombEntry8,8);
RR_ASSERT_SIZE(PadChannel,0xF0);
RR_ASSERT_OFFSET(PadChannel,modes00,0x0);
RR_ASSERT_OFFSET(PadChannel,entries04,0x4);
RR_ASSERT_OFFSET(PadChannel,combs08,0x8);
RR_ASSERT_OFFSET(PadChannel,children0C,0xC);
RR_ASSERT_OFFSET(PadChannel,owner10,0x10);
RR_ASSERT_OFFSET(PadChannel,update14,0x14);
RR_ASSERT_OFFSET(PadChannel,complete18,0x18);
RR_ASSERT_OFFSET(PadChannel,layout20,0x20);
RR_ASSERT_OFFSET(PadChannel,argument24,0x24);
RR_ASSERT_OFFSET(PadChannel,data28,0x28);
RR_ASSERT_OFFSET(PadChannel,data2C,0x2C);
RR_ASSERT_OFFSET(PadChannel,input30,0x30);
RR_ASSERT_OFFSET(PadChannel,length34,0x34);
RR_ASSERT_OFFSET(PadChannel,length36,0x36);
RR_ASSERT_OFFSET(PadChannel,command37,0x37);
RR_ASSERT_OFFSET(PadChannel,previous38,0x38);
RR_ASSERT_OFFSET(PadChannel,packet,0x3C);
RR_ASSERT_OFFSET(PadChannel,buffer40,0x40);
RR_ASSERT_OFFSET(PadChannel,b45,0x45);
RR_ASSERT_OFFSET(PadChannel,phase,0x46);
RR_ASSERT_OFFSET(PadChannel,state47,0x47);
RR_ASSERT_OFFSET(PadChannel,state49,0x49);
RR_ASSERT_OFFSET(PadChannel,type4C,0x4C);
RR_ASSERT_OFFSET(PadChannel,modeArgs51,0x51);
RR_ASSERT_OFFSET(PadChannel,output54,0x54);
RR_ASSERT_OFFSET(PadChannel,metadata63,0x63);
RR_ASSERT_OFFSET(PadChannel,modeCountE3,0xE3);
RR_ASSERT_OFFSET(PadChannel,argumentE4,0xE4);
RR_ASSERT_OFFSET(PadChannel,pending,0xE6);
RR_ASSERT_OFFSET(PadChannel,responseE8,0xE8);
RR_ASSERT_OFFSET(PadChannel,entry_countE9,0xE9);
RR_ASSERT_OFFSET(PadChannel,combCountEA,0xEA);
RR_ASSERT_OFFSET(PadChannel,scanIndexEB,0xEB);
RR_ASSERT_OFFSET(PadChannel,scanOffsetEC,0xEC);
RR_ASSERT_OFFSET(PadChannel,scanSeenEE,0xEE);
#endif
RR_ASSERT_SIZE(ChannelControlRow36, 0x36);
RR_ASSERT_OFFSET(ChannelControlRow36, value04, 4);
RR_ASSERT_OFFSET(ChannelControlRow36, group0E, 0xE);
RR_ASSERT_OFFSET(ChannelControlRow36, match10, 0x10);
RR_ASSERT_OFFSET(ChannelControlRow36, match14, 0x14);
RR_ASSERT_OFFSET(ChannelControlRow36, match18, 0x18);
RR_ASSERT_OFFSET(ChannelControlRow36, flags1D, 0x1D);


#endif

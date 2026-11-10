#ifndef RR_SEQUENCE_CHANNEL_H
#define RR_SEQUENCE_CHANNEL_H
#include "rr/layout.h"

/* One complete SDK sequence row. All five leading cells are CPU pointers.
 * The legacy type spellings name this same owner. Word/byte names whose
 * interpretation is still unresolved are retained explicitly. */
typedef struct SequenceChannel {
    u8 *c0;
    u8 *c4;
    u8 *c8; /* Saved loop cursor, restored by controller 30. */
    u8 *cC;
    u8 *p10; /* Stream marker checked before repeat/reset dispatch. */
    u8 b14, b15, b16;
    u8 b17, b18, b19, b1a, b1b, b1c, b1d, b1e, b1f;
    /* b20 is compared as signed; b21 increments modulo 256. */
    s8 b20;
    u8 b21;
    s8 b22[2];
    u8 p24[2];
    s8 b26;
    u8 b27[0x10];
    u8 b37[0x10];
    char p47[1];
    s16 h48[8];
    u16 h58[4];
    s16 h60[0x10];
    s16 h80;
    u16 reserved82;
    s32 w84;
    s32 w88;
    s32 w8c;
    s32 w90;
    u32 w94;
    s32 w98;
    s32 w9c;
    s32 wa0, wa4, wa8;
    u32 wac;
} SequenceChannel;
typedef SequenceChannel LibStateB0;
typedef SequenceChannel ChannelStreamB0;
typedef SequenceChannel ChannelValueRecordB0;
extern SequenceChannel *_ss_score[32];
RR_ASSERT_OFFSET(SequenceChannel,c0,0);
RR_ASSERT_OFFSET(SequenceChannel,c4,sizeof(u8 *));
RR_ASSERT_OFFSET(SequenceChannel,c8,2*sizeof(u8 *));
RR_ASSERT_OFFSET(SequenceChannel,cC,3*sizeof(u8 *));
RR_ASSERT_OFFSET(SequenceChannel,p10,4*sizeof(u8 *));
RR_ASSERT_OFFSET(SequenceChannel,b14,5*sizeof(u8 *));
RR_ASSERT_OFFSET(SequenceChannel,h48,0x48+5*(sizeof(u8 *)-4));
RR_ASSERT_OFFSET(SequenceChannel,w84,0x84+5*(sizeof(u8 *)-4));
RR_ASSERT_OFFSET(SequenceChannel,w88,0x88+5*(sizeof(u8 *)-4));
RR_ASSERT_OFFSET(SequenceChannel,wac,0xAC+5*(sizeof(u8 *)-4));
RR_ASSERT_SIZE(SequenceChannel,
    ((0xB0+5*(sizeof(u8 *)-4)+sizeof(u8 *)-1)&~(sizeof(u8 *)-1)));
#endif

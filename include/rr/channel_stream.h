#ifndef RR_CHANNEL_STREAM_H
#define RR_CHANNEL_STREAM_H
#include "rr/layout.h"
/* Complete stream-record view. PAL pointers occupy the first five words;
 * native fixtures retain their full pointer width and natural row stride. */
typedef struct {
    u8 *cursor00,*start04,*saved08,*alternate0C,*marker10;
    u8 active14,b15,b16,b17,b18,b19,b1a,b1b,state1C,b1d,b1e,b1f;
    s8 duration20;
    u8 elapsed21;
    s8 restartBank22,restartRow23;
    u8 b24,b25;
    s8 b26;
    u8 b27[0x10];
    u8 b37[0x10];
    u8 pad47;
    s16 h48[3];
    s16 interval4E,scale50;
#ifndef __mips__
    union {
        u8 pad52[2];
        s16 delay52;
    };
#else
    s16 delay52;
#endif
    s16 pitch54;
    u16 h56;
    u16 h58[4];
    u16 h60[0x10];
    s16 h80,pad82;
    s32 w84;
    s32 accumulator88,reserved8C,accumulator90;
    u32 tempo94;
    s32 mode98;
    s32 w9c,wa0,wa4;
    s32 tempoRemainingA8;
    u32 targetTempoAC;
} ChannelStreamB0;
extern ChannelStreamB0 *_ss_score[];
void _SsSeqGetEof();
void _SsSndNextSep(s32,s16);
void _SsVmSeqKeyOff(s32);
RR_ASSERT_SIZE(ChannelStreamB0,0xB0);
RR_ASSERT_OFFSET(ChannelStreamB0,active14,0x14);
RR_ASSERT_OFFSET(ChannelStreamB0,duration20,0x20);
RR_ASSERT_OFFSET(ChannelStreamB0,pitch54,0x54);
RR_ASSERT_OFFSET(ChannelStreamB0,mode98,0x98);
RR_ASSERT_OFFSET(ChannelStreamB0,cursor00,0x0);
RR_ASSERT_OFFSET(ChannelStreamB0,start04,0x4);
RR_ASSERT_OFFSET(ChannelStreamB0,saved08,0x8);
RR_ASSERT_OFFSET(ChannelStreamB0,alternate0C,0xC);
RR_ASSERT_OFFSET(ChannelStreamB0,marker10,0x10);
RR_ASSERT_OFFSET(ChannelStreamB0,state1C,0x1C);
RR_ASSERT_OFFSET(ChannelStreamB0,elapsed21,0x21);
RR_ASSERT_OFFSET(ChannelStreamB0,restartBank22,0x22);
RR_ASSERT_OFFSET(ChannelStreamB0,restartRow23,0x23);
RR_ASSERT_OFFSET(ChannelStreamB0,accumulator88,0x88);
RR_ASSERT_OFFSET(ChannelStreamB0,accumulator90,0x90);

extern u32 VBLANK_MINUS;
void _SsSndTempo(s32 bank,s32 row);
RR_ASSERT_OFFSET(ChannelStreamB0,interval4E,0x4E);
RR_ASSERT_OFFSET(ChannelStreamB0,scale50,0x50);
RR_ASSERT_OFFSET(ChannelStreamB0,tempo94,0x94);
RR_ASSERT_OFFSET(ChannelStreamB0,tempoRemainingA8,0xA8);
RR_ASSERT_OFFSET(ChannelStreamB0,targetTempoAC,0xAC);

#endif

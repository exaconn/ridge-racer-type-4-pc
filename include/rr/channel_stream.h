#ifndef RR_CHANNEL_STREAM_H
#define RR_CHANNEL_STREAM_H
#include "rr/layout.h"
/* Complete stream-record view. PAL pointers occupy the first five words;
 * native fixtures retain their full pointer width and natural row stride. */
typedef struct {
    u8 *cursor00,*start04,*saved08,*alternate0C,*marker10;
    u8 active14,pad15[7],state1C,pad1D[3];
    s8 duration20;
    u8 elapsed21;
    s8 restartBank22,restartRow23;
    u8 pad24[0x4E - 0x24];
    s16 interval4E,scale50;
    u8 pad52[2];
    s16 pitch54;
    u8 pad56[0x88-0x56];
    s32 accumulator88,reserved8C,accumulator90;
    u32 tempo94;
    s32 mode98;
    u8 pad9C[0xA8-0x9C];
    s32 tempoRemainingA8;
    u32 targetTempoAC;
} RrChannelStreamB0;
extern RrChannelStreamB0 *D_8010F068[];
void func_800833A0(s32 bank,s32 row);
void func_80083994(s32,s16);
void func_80089E1C(s32);
RR_ASSERT_SIZE(RrChannelStreamB0,0xB0);
RR_ASSERT_OFFSET(RrChannelStreamB0,active14,0x14);
RR_ASSERT_OFFSET(RrChannelStreamB0,duration20,0x20);
RR_ASSERT_OFFSET(RrChannelStreamB0,pitch54,0x54);
RR_ASSERT_OFFSET(RrChannelStreamB0,mode98,0x98);
RR_ASSERT_OFFSET(RrChannelStreamB0,cursor00,0x0);
RR_ASSERT_OFFSET(RrChannelStreamB0,start04,0x4);
RR_ASSERT_OFFSET(RrChannelStreamB0,saved08,0x8);
RR_ASSERT_OFFSET(RrChannelStreamB0,alternate0C,0xC);
RR_ASSERT_OFFSET(RrChannelStreamB0,marker10,0x10);
RR_ASSERT_OFFSET(RrChannelStreamB0,state1C,0x1C);
RR_ASSERT_OFFSET(RrChannelStreamB0,elapsed21,0x21);
RR_ASSERT_OFFSET(RrChannelStreamB0,restartBank22,0x22);
RR_ASSERT_OFFSET(RrChannelStreamB0,restartRow23,0x23);
RR_ASSERT_OFFSET(RrChannelStreamB0,accumulator88,0x88);
RR_ASSERT_OFFSET(RrChannelStreamB0,accumulator90,0x90);

extern u32 D_8010F064;
void func_80084314(s32 bank,s32 row);
RR_ASSERT_OFFSET(RrChannelStreamB0,interval4E,0x4E);
RR_ASSERT_OFFSET(RrChannelStreamB0,scale50,0x50);
RR_ASSERT_OFFSET(RrChannelStreamB0,tempo94,0x94);
RR_ASSERT_OFFSET(RrChannelStreamB0,tempoRemainingA8,0xA8);
RR_ASSERT_OFFSET(RrChannelStreamB0,targetTempoAC,0xAC);

#endif

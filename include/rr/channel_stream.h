#ifndef RR_CHANNEL_STREAM_H
#define RR_CHANNEL_STREAM_H
#include "rr/sequence_channel.h"
extern u32 VBLANK_MINUS;
void _SsSeqGetEof(s32 bankInput,s32 rowInput);
void _SsSndNextSep(s32,s16);
void _SsVmSeqKeyOff(s32);
void _SsSndTempo(s32 bank,s32 row);
#endif

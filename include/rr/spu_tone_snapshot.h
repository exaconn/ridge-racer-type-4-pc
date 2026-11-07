#ifndef RR_SPU_TONE_SNAPSHOT_H
#define RR_SPU_TONE_SNAPSHOT_H
#include "rr/spu.h"

/* Requires flag storage covering the signed low16 bank and tone storage covering
 * every accessed signed low16 index after adding signed selection.value * 16.
 * The selector's return value is ignored. Selected tone pointers are reloaded
 * for each byte and once before the four halfwords. Source/destination must be
 * halfword aligned. Output bytes 14/15 are preserved; byte 7 precedes byte 6. */
s32 SsUtGetVagAtr(s32 bank, s16 program, s32 tone, u8 output[24]);
#endif

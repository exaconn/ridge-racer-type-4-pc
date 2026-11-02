#ifndef RR_SPU_TONE_UPDATE_H
#define RR_SPU_TONE_UPDATE_H
#include "rr/spu.h"

/* Write the selected tone, using signed low16 bank/program and a signed low16
 * tone index after adding signed selection.value * 16. Accessible flag/tone
 * storage must cover those indices. Selector failure is intentionally ignored.
 * Input and destination must be halfword aligned. Destination bytes 14/15 are
 * preserved. Each byte reloads the selected pointer; the four halfwords share
 * one captured destination. Byte 7 is copied before byte 6. */
s32 func_800867E4(s32 bank, s32 program, s32 tone, const u8 input[24]);
#endif

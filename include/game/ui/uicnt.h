#ifndef RR_GAME_UI_UICNT_H
#define RR_GAME_UI_UICNT_H

#include "rr/gpu.h"

s32 AddOrSubtractByte(s32 arg0, s32 arg1, s32 arg2);
void ByteToCodeChars(s32 arg0, void *arg1);
s32 CodeCharsToByte(s32 arg0, s32 arg1);
void XorBytes37(u8 *arg0, s32 arg1);
/* Byte sum, zero-extended to the public word result. */
s32 ChecksumBytes(u8 *p, int n);
void *DrawBadgeCountdownSprites(GpuDmaTag *ordering, void *cursor, s32 row);

#endif /* RR_GAME_UI_UICNT_H */

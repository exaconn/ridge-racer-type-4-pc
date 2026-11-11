#ifndef RR_GAME_GFX_CLUT_H
#define RR_GAME_GFX_CLUT_H

#include "types.h"

s32 DebugPrintStub(const char *format, ...);
/* Upload entry index of a TIM bank (0x840-byte entries) to CLUT slot; a
 * negative index clears the slot. */
void UploadCarClut(void *tim, s32 index, s32 slot);

#endif /* RR_GAME_GFX_CLUT_H */

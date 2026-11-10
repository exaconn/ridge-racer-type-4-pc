#ifndef RR_GAME_SYSTEM_MODESLOT_H
#define RR_GAME_SYSTEM_MODESLOT_H

#include "types.h"

/* 0x24-byte slots of the mode/menu work table at 0x800F7C0x; the union of the fields the
 * mode setup (modeinit), the pad handler (modepad) and the menu work code (menuwork) touch. */
typedef struct {
    s32 unk00[3];
    s8 b0C, b0D, b0E, b0F;
    s32 unk10;
    u8 b14, b15, b16, b17;
    s32 unk18, unk1C;
    s16 s20, s22;
} W7C0_E;

typedef struct {
    s16 s0, s2;
    s32 unk04;
    s16 s8, sA;
    s32 unk0C;
    s16 s10, s12;
} W7C0_F;

#endif

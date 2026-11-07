/* CC1_FLAGS: -fno-strength-reduce */

#include "rr/hud_number.h"

#include "m2c_macros.h"

/* legacy typing of shared globals in this part of the unit */
#define HudNumberTargets (*(struct W3G0A (*))&HudNumberTargets)

#undef HudNumberTargets

/* legacy typing of shared globals in this part of the unit */
#define HudNumberTargets (*(char (*)[])&HudNumberTargets)

#undef HudNumberTargets

void *EmitHudGlyph12(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    M2C_FIELD(arg1, s8 *, 3) = 4;
    M2C_FIELD(arg1, s8 *, 7) = 0x65;
    M2C_FIELD(arg1, s16 *, 0x10) = 0xC;
    M2C_FIELD(arg1, s16 *, 0x12) = 0x10;
    M2C_FIELD(arg1, s16 *, 8) = arg2;
    M2C_FIELD(arg1, s16 *, 0xA) = arg3;
    M2C_FIELD(arg1, u8 *, 0xD) = 0xF0;
    M2C_FIELD(arg1, s8 *, 0xC) = (s8) (arg4 * 0xC);
    M2C_FIELD(arg1, s16 *, 0xE) = (s16) arg5;
    M2C_FIELD(arg1, s32 *, 0) = (s32) ((M2C_FIELD(arg1, s32 *, 0) & 0xFF000000) | (*arg0 & 0xFFFFFF));
    *arg0 = (*arg0 & 0xFF000000) | ((s32) arg1 & 0xFFFFFF);
    return arg1 + 0x14;
}

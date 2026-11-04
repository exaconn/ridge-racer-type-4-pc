#ifndef RR_GAME_CAR_CARSP_H
#define RR_GAME_CAR_CARSP_H

#include "types.h"

/* Car-physics work area in the scratchpad (0x1F800080). */
typedef struct T16 { s32 a; s32 pa; s32 c; s32 pc; } T16;

typedef struct SP {
    char p0[8]; s32 idx;
    char p1[0x34 - 0xC]; s32 w34; s32 cnt; s32 r3c; s32 r40;
    char p2[0x64 - 0x44];
    u16 h64, h66, h68, h6a, h6c, h6e, h70, h72, h74, h76;
    s32 w78;
    char p3[0x80 - 0x7C]; s32 w80;
    char p4[0x110 - 0x84];
    T16 t110[4];
    s16 s150[16];
} SP;

#endif

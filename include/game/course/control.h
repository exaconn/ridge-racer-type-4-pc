#ifndef RR_GAME_COURSE_CONTROL_H
#define RR_GAME_COURSE_CONTROL_H

#include "types.h"

/* Control block at car+0x1F4 (course position tracking). */
typedef struct {
    u8 pad00[0x58];
    s32 unk58;
    u8 pad5C[0x6E - 0x5C];
    s16 unk6E;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    s32 unk80;
    s32 unk84;
} Control1F4;

#endif

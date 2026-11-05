#ifndef RR_GAME_COURSE_CONTROL_H
#define RR_GAME_COURSE_CONTROL_H

#include "types.h"

typedef struct {
    char pad0[0xA];
    s16 unkA;
} Unk210;

/* Course position tracking block, 0x12C bytes at car+0x1F4. Union of the fields seen by the
 * course projection (trkproj), the race distance code (racedist), the sound events (sndevt)
 * and the steering code (carsteer). */
typedef struct {
    u8 pad00[0x1C];
    Unk210 *unk1C;
    u8 pad20[0x54 - 0x20];
    s32 unk54;
    s32 unk58;
    u8 pad5C[0x6E - 0x5C];
    s16 unk6E;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    u8 pad88[0x94 - 0x88];
    s16 unk94;
    u8 pad96[0xB4 - 0x96];
    s16 unkB4;
    s16 unkB6;
    u8 padB8[0xC0 - 0xB8];
    s32 unkC0;
    s32 unkC4;
    u8 padC8[0x124 - 0xC8];
    s16 unk124;
    u8 pad126[0x12C - 0x126];
} Control;

#endif

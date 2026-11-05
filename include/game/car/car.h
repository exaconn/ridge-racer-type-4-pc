#ifndef RR_GAME_CAR_CAR_H
#define RR_GAME_CAR_CAR_H

#include "types.h"
#include "game/course/control.h"

/* Vehicle state record. Only the members the game code touches are named (by
 * offset); the gaps are padding. One definition for every unit that walks it. */
struct Q4 {
    s32 w[4];
};

typedef struct Car {
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 wC;
    s32 posX6;   /* posX >> 6 (aipath2 refreshes it from posX) */
    s32 w14;
    s32 posZ6;   /* posZ >> 6 */
    s32 w1C;
    union {
        struct Q4 q20;
        struct {
            s32 w20;
            s32 w24;
            s32 w28;
            s32 w2C;
        } s;
    } u20;
    char pad30[0x54 - 0x30];
    s32 unk54;
    char pad58[0x88 - 0x58];
    struct Q4 q88;
    struct Q4 q98;
    struct Q4 qA8;
    struct Q4 qB8;
    char padC8[0xDC - 0xC8];
    s16 unkDC[4];
    s16 unkE4[4];
    char padEC[0xF0 - 0xEC];
    s32 wF0;
    s32 wF4;
    s16 sF8[4];
    char pad100[0x108 - 0x100];
    s32 unk108;
    s16 s10C;
    s16 unk10E;
    char pad110[0x170 - 0x110];
    s16 s170;
    s16 s172;
    char pad174[0x184 - 0x174];
    s32 w184;
    s32 w188;
    s32 w18C;
    s32 w190;
    s16 s194;
    char pad196[0x1A0 - 0x196];
    s16 s1A0;
    char pad1A2[0x1A8 - 0x1A2];
    s16 unk1A8;
    s16 unk1AA;
    s16 unk1AC;
    s16 unk1AE;
    char pad1B0[0x1BC - 0x1B0];
    s16 unk1BC[4];
    char pad1C4[0x1D4 - 0x1C4];
    s16 unk1D4;
    s16 unk1D6;
    s16 unk1D8;
    s16 unk1DA;
    s16 unk1DC;
    s16 unk1DE;
    s32 unk1E0;
    s16 unk1E4;
    u16 unk1E6;
    char pad1E8[0x1EA - 0x1E8];
    s16 unk1EA;
    s16 s1EC;
    char pad1EE[0x1F0 - 0x1EE];
    union {
        s32 w1F0;
        u8 b1F0[4];
    } u1F0;
    Control sub1F4;
} Car;

#endif

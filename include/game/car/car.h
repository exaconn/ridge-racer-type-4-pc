#ifndef RR_GAME_CAR_CAR_H
#define RR_GAME_CAR_CAR_H

#include "types.h"

/* Vehicle state record. Only the members the game code touches are named (by
 * offset); the gaps are padding. One definition for every unit that walks it. */
struct Q4 {
    s32 w[4];
};

typedef struct Car {
    s32 w0;
    s32 w4;
    s32 w8;
    s32 wC;
    s32 w10;
    s32 w14;
    s32 w18;
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
    char pad30[0x88 - 0x30];
    struct Q4 q88;
    struct Q4 q98;
    struct Q4 qA8;
    struct Q4 qB8;
    char padC8[0xF0 - 0xC8];
    s32 wF0;
    s32 wF4;
    s16 sF8[4];
    char pad100[0x10C - 0x100];
    s16 s10C;
    char pad10E[0x170 - 0x10E];
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
    char pad1A2[0x1EC - 0x1A2];
    s16 s1EC;
    char pad1EE[0x1F0 - 0x1EE];
    union {
        s32 w1F0;
        u8 b1F0[4];
    } u1F0;
} Car;

#endif

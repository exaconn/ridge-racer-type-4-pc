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


typedef struct Z52 {
    s32 w0;
    s32 w4;
    s16 s8;
    s16 sA;
    s16 sC;
    s16 sE;
    s16 s10;
    s16 s12;
    s16 s14;
    s16 s16;
    s16 s18;
    s16 s1A;
    s32 w1C;
    struct {
        u8 b0;
        u8 b1;
    } a20[2];
    s16 s24[2];
    s16 s28[2];
    s16 s2C[2];
    s16 s30[2];
} Z52;

typedef struct T16_1 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
} T16_1;

typedef struct SP_1 {
    Car *p0;
    s32 w4;
    s32 w8;
    s32 wC;
    s32 w10;
    s32 w14;
    s32 w18;
    s32 w1C;
    s32 w20;
    s16 s24;
    s16 s26;
    s16 s28;
    s16 s2A;
    s32 w2C;
    s32 w30;
    s32 w34;
    s32 w38;
    s32 w3C;
    s32 w40;
    char pad44[0xB8 - 0x44];
    s16 sB8;
    s16 sBA;
    s16 sBC;
    s16 sBE;
    s16 sC0;
    s16 sC2;
    s16 sC4;
    s16 sC6;
    s16 sC8;
    char padCA[0xDA - 0xCA];
    s16 sDA;
    s16 sDC;
    s16 sDE;
    s16 sE0;
    s16 sE2;
    s16 sE4;
    s16 sE6;
    char padE8[0xF8 - 0xE8];
    s16 sF8;
    s16 sFA;
    s16 sFC;
    char padFE[2];
    s32 w100;
    s32 w104;
    s32 w108;
    s32 w10C;
    T16_1 t110[4];
    s16 s150[4];
    s16 s158;
    s16 s15A;
    s16 s15C;
    s16 s15E;
    s16 s160;
    s16 s162;
    s16 s164;
    s16 s166;
    s16 s168;
    s16 s16A;
    s16 s16C;
    s16 s16E;
    s16 s170;
    s16 s172;
    s16 s174;
    s16 s176;
    char pad178[8];
    Z52 z180[4];
    s16 s250[2][4];
} SP_1;

#endif

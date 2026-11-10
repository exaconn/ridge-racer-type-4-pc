#ifndef RR_GAME_CAR_CARSP_H
#define RR_GAME_CAR_CARSP_H

#include "types.h"

/* RaceVehicle-physics work area in the scratchpad (0x1F800080). */
typedef struct T16 { s32 a; s32 pa; s32 c; s32 pc; } T16;

typedef struct SP {
    s32 w0, w4; s32 idx;
    s32 wC[10]; s32 w34; s32 cnt; s32 r3c; s32 r40;
    s32 w44[8];
    u16 h64, h66, h68, h6a, h6c, h6e, h70, h72, h74, h76;
    s32 w78;
    s32 w7c; s32 w80;
    s32 w84[35];
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
    RaceVehicle *p0;
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
    s32 w44[29];
    s16 sB8;
    s16 sBA;
    s16 sBC;
    s16 sBE;
    s16 sC0;
    s16 sC2;
    s16 sC4;
    s16 sC6;
    s16 sC8;
    s16 sCA[8];
    s16 sDA;
    s16 sDC;
    s16 sDE;
    s16 sE0;
    s16 sE2;
    s16 sE4;
    s16 sE6;
    s32 wE8[4];
    s16 sF8;
    s16 sFA;
    s16 sFC;
    s16 sFE;
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
    s32 w178[2];
    Z52 z180[4];
    s16 s250[2][4];
} SP_1;

typedef struct ScratchCarPad {
    u8 reserved00[0x7C];
    s32 wheelBufIdx7C;
    u8 reserved80[0x100];
    Vector pos180;
    Vector wheels190[4];
} ScratchCarPad;

#endif

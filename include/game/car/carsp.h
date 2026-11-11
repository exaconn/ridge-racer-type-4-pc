#ifndef RR_GAME_CAR_CARSP_H
#define RR_GAME_CAR_CARSP_H

#include "types.h"
#include "rr/geometry.h"
#include "rr/race_types.h"
#include "rr/track_sample_types.h"

/* RaceVehicle-physics work area in the scratchpad (0x1F800080). */

/* NCLIP consumes each signed halfword pair as one packed register word. */
typedef union {
    struct { u16 x, z; } halves;
    s32 word;
} CarTrackPoint4;

/* Search, contact and suspension operate on one owner. The PS1 vehicle
 * pointer at +0 lives in CarTrackWorkVehicle (no host pointers in scratch). */
typedef struct CarTrackWork {
    u32 vehicleSlot00;
    s32 w4;
    s32 wheelIndex08;
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
    s32 w44[8];
    CarTrackPoint4 queryPoint64, rightEdge68, leftEdge6C;
    CarTrackPoint4 nextRight70, nextLeft74;
    s32 originX78, w7C, originZ80;
    s32 w84[13];
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
    VectorWords16 translation100;
    Vector positions110[4];
    /* Track indices are consumed before the suspension phase reuses them. */
    union {
        s16 trackNodes[4];
        s16 heightOffsets[4];
    } wheelState150;
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
    TrackRoadSample34 road180[4];
    s16 wheelHeights250[2][4];
} CarTrackWork;

extern RaceVehicle *CarTrackWorkVehicle;

RR_ASSERT_SIZE(CarTrackPoint4,4);
RR_ASSERT_OFFSET(CarTrackWork,wheelIndex08,0x8);
RR_ASSERT_OFFSET(CarTrackWork,queryPoint64,0x64);
RR_ASSERT_OFFSET(CarTrackWork,nextLeft74,0x74);
RR_ASSERT_OFFSET(CarTrackWork,w4,0x4);
RR_ASSERT_OFFSET(CarTrackWork,translation100,0x100);
RR_ASSERT_OFFSET(CarTrackWork,positions110,0x110);
RR_ASSERT_OFFSET(CarTrackWork,wheelState150,0x150);
RR_ASSERT_OFFSET(CarTrackWork,road180,0x180);
RR_ASSERT_OFFSET(CarTrackWork,wheelHeights250,0x250);

#endif

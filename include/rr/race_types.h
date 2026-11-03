#ifndef RR_RACE_TYPES_H
#define RR_RACE_TYPES_H
#include "rr/layout.h"

/* Six-word interpolation record, shared by the constructor and evaluator. */
typedef struct {
 s32 displacement, duration;
 s32 initialRate, middleRate;
 s32 firstAcceleration, secondAcceleration;
} RrCurve24;
/* Lane-change / steering transition state at vehicle offset 0x288. */
typedef struct {
 s16 mode00,timer02;
 s32 duration04,remaining08,angle0C,delta10,reserved14,targetNode18,midpoint1C;
 s16 finish20,count22;
 s32 lateral24,phase28,range2C,gain30;
 RrCurve24 primary34,secondary4C;
} RrSteeringState;
RR_ASSERT_OFFSET(RrSteeringState,count22,0x22);
enum {RR_RACE_CONTROL_OFFSET=0x1F4};

/* Sub-record at offset 0x1F4 of RrRaceVehicle (used by AI, lane-change and gearbox routines). */
typedef struct {
    u8 pad00[0x30];
    s16 pathDeltaX30, reserved32, pathDeltaZ34, reserved36;
    s16 laneTimer38, laneStart3A, laneTarget3C, laneCurrent3E;
    s32 laneCooldown40;
    s16 boostTimer44, dragFlag46, holdFlag48, pad4A;
    s32 speed4C, accel50;
    u8 pad54[2];
    unsigned short rpm56;
    u8 pad58[0x62 - 0x58];
    s16 shiftDir62, shiftState64, shiftDelay66, gear68, laneActive6A, laneDelta6C;
    u8 pad6E[0x74 - 0x6E];
    s32 relDist74, relDist78;
    u8 pad7C[0x88 - 0x7C];
    s32 laneMode88;
    u8 pad8C[8];
    RrSteeringState steering94;
    s32 maxSpeedF8, gearCountFC;
    s16 gearRatios100[6];
    unsigned short shiftThresholds10C[6];
    s16 maximum118, span11A, divisor11C, factor11E;
    u8 pad120[4];
    s16 steeringEnabled124;
    u8 pad126[4];
    s16 metric12A;
} RrRaceVehicleControl;

/* 0x30-byte performance interpolation row (D_800ACB90 / context table). */
typedef struct {
    s32 baseSpeed00, pad04;
    unsigned short ratios08[6];
    unsigned short thresholds14[6];
    s16 maximum20, span22;
    u8 pad24[4];
    s32 altSpeed28, pad2C;
} RrRaceSpecEntry30;

/* Fields shared by distance comparison, vehicle physics, AI and response calculations. */
typedef struct RrRaceVehicle {
    s32 posScaled00[3], pad0C;
    s32 pos10[3], pad1C;
    s32 pos20[3], pad2C;
    s32 vel30, pad34, vel38, pad3C;
    s32 ir40[3], pad4C;
    s32 rot50, rot54, rot58, pad5C;
    s32 ir60[3], pad6C;
    s32 ir70[4];
    s16 ang80, ang82, ang84;
    u8 pad86[0xC8 - 0x86];
    s32 savedRotC8[4];
    u8 padD8[0xE0 - 0xD8];
    s16 wheelE0, wheelE2;
    u8 padE4[4];
    s16 wheelE8, wheelEA;
    u8 padEC[4];
    s32 trackNodeF0, lateralVelF4;
    u8 padF8[0x108 - 0xF8];
    s32 height108;
    s16 width10C, halfSpan10E;
    s16 corner110, corner112, corner114, corner116, corner118, corner11A, corner11C, corner11E;
    u8 pad120[0x170 - 0x120];
    s16 trackFraction170;
    s16 lateralOffset172;
    u8 pad174[4];
    s32 lateralMotion178;
    u8 pad17C[8];
    s32 first, second;
    u8 pad18C[0x198 - 0x18C];
    s16 shakeX198, shakeY19A, shakeActive19C, shakeTimer19E, pad1A0, steerAngle1A2;
    u8 state1A4, stateTimer1A5, pad1A6[0x1B8 - 0x1A6];
    s16 steeringLock1B8;
    s16 steerInput1BA;
    u8 pad1BC[0x1D4 - 0x1BC];
    s16 heading1D4, prevHeading1D6, input, prevInput1DA;
    u8 pad1DC[0x1E4 - 0x1DC];
    unsigned short flags1E4;
    s16 slotIndex1E6;
    s16 half1E8;
    s16 half1EA;
    unsigned short trackAngle1EC;
    s16 half1EE;
    u8 pad1F0[0x218 - 0x1F0];
    s32 playerFlags218;
    u8 pad21C[0x224 - 0x21C];
    s16 pathDeltaX224, reserved226, pathDeltaZ228, reserved22A;
    s16 laneTimer22C, laneStart22E, laneTarget230, laneCurrent232;
    s32 laneCooldown234;
    s16 blend, dragFlag23A, holdFlag23C, pad23E;
    s32 speed240, accel244;
    u8 pad248[2];
    unsigned short rpm24A;
    u8 pad24C[0x256 - 0x24C];
    s16 shiftDir256, shiftState258, shiftDelay25A, gear25C, laneActive25E, laneDelta260;
    u8 pad262[0x268 - 0x262];
    s32 relDist268, relDist26C;
    u8 pad270[0x27A - 0x270];
    s16 half27A;
    s32 laneMode27C;
    u8 pad280[8];
    RrSteeringState steering288;
    s32 maxSpeed2EC, gearCount2F0;
    s16 gearRatios2F4[6];
    unsigned short shiftThresholds300[6];
    s16 maximum, span, divisor, factor312;
    u8 pad314[4];
    s16 steeringEnabled318;
    u8 pad31A[4];
    s16 metric31E;
} RrRaceVehicle;

typedef struct {
    s32 unk00, totalDist04;
    RrRaceVehicle *vehicle08;
} RrRaceStandingEntry0C;

typedef struct {
    s32 unk00, unk04, dist08, dist0C, unk10, gap14;
    RrRaceStandingEntry0C entries18[8];
} RrRaceStandingsTable;

typedef struct {
    u8 pad00[8];
    s32 first, second, selected;
} RrRaceDistanceResult;

/* Prefix of the race context; the output pointer is at E8 on PAL. */
typedef struct {
    u8 pad00[0x14];
    s32 mode;
    u8 pad18[0xE8-0x18];
    RrRaceDistanceResult *result;
} RrRaceDistanceContextPrefix;

#endif

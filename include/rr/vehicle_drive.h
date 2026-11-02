#ifndef RR_VEHICLE_DRIVE_H
#define RR_VEHICLE_DRIVE_H
#include "rr/race.h"
/* This complete view decodes opaque drive fields without changing the existing
 * canonical vehicle layout. Native compilers permit aliasing the byte-identical
 * record; GCC2.8 predates strict-alias optimizations and sees no attribute. */
#if defined(__GNUC__) && __GNUC__ >= 3
#define RR_DRIVE_ALIAS __attribute__((__may_alias__))
#else
#define RR_DRIVE_ALIAS
#endif
typedef struct {
 u8 reserved00[0x30];s16 directionX30,reserved32,directionZ34,reserved36;
 s16 laneTimer38,laneStart3A,laneTarget3C,laneCurrent3E;s32 laneCooldown40;
 s16 boostTimer44,dragFlag46,holdFlag48,attenuation4A;s32 speed4C,accel50;
 s16 accelerating54;u16 rpm56;u8 reserved58[8];s16 latch60;
 s16 shiftDir62,shiftState64,shiftDelay66,gear68,laneActive6A,laneDelta6C,limit6E;
 u8 reserved70[4];s32 relDist74,relDist78;u8 reserved7C[12];s32 laneMode88;
 s16 steering8C,reserved8E,filtered90,error92;RrSteeringState steering94;
 s32 maxSpeedF8,gearCountFC;u16 gearRatios100[6],shiftThresholds10C[6];
 s16 maximum118,span11A,divisor11C,factor11E;u8 reserved120[4];s16 steeringEnabled124;
 u8 reserved126[4];s16 metric12A;
} RR_DRIVE_ALIAS RrDriveControlView;
typedef struct {
 u8 reserved00[0x50];s32 rotation50;u8 reserved54[0x178-0x54];s32 lateral178;
 u8 reserved17C[0x1B8-0x17C];s16 lock1B8;u8 reserved1BA[0x1D4-0x1BA];
 s16 heading1D4,previous1D6,input1D8;u8 reserved1DA[0x1F4-0x1DA];RrDriveControlView control1F4;
} RR_DRIVE_ALIAS RrDriveVehicleView;
RR_ASSERT_SIZE(RrDriveControlView,0x12C);
RR_ASSERT_SIZE(RrDriveVehicleView,0x320);
RR_ASSERT_OFFSET(RrDriveVehicleView,control1F4,0x1F4);
#undef RR_DRIVE_ALIAS
RR_ASSERT_OFFSET(RrDriveControlView,directionX30,0x30);
RR_ASSERT_OFFSET(RrDriveControlView,limit6E,0x6E);
RR_ASSERT_OFFSET(RrDriveControlView,latch60,0x60);
RR_ASSERT_OFFSET(RrDriveControlView,steering8C,0x8C);
RR_ASSERT_OFFSET(RrDriveControlView,error92,0x92);
RR_ASSERT_OFFSET(RrDriveControlView,gearRatios100,0x100);
/* Legacy road sampler consumes the complete vehicle through a char view and
 * writes three distinct word-sized output locals. */
s32 func_80047DF8(char *vehicle,s32 lane,s32 *,s32 *,s32 *);
void func_80047FBC(RrRaceVehicle *vehicle,s32 speed);
void func_800483D8(RrRaceVehicle *vehicle);
s32 func_800924D4(s32 x,s32 z);
/* Gear after a pending shift addresses one of six ratios. Pure angle/response
 * services retain the vehicle; road sampling writes its three output words.
 * Lane and final drive updates can change the complete live vehicle. Response
 * is signed16-normalized in a full word. Products wrap32 before signed division;
 * steering filtering narrows before clamping and before the averaging step. */
/* Pass the canonical record through this explicitly aliasable complete view. */
void func_800486C4(RrDriveVehicleView *vehicle);
#endif

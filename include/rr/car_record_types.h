#ifndef RR_CAR_RECORD_TYPES_H
#define RR_CAR_RECORD_TYPES_H

#include "rr/layout.h"

typedef struct {
    s16 baseGrip00, unk02, unk04, steerDragDivisor06;
    s16 steeringGain08, steeringMode0A;
    s16 brakeBalance0C, slideRetention0E, unk10, maxSlopePitch12, slopeFactor14;
    s16 slideThreshold16;
    s16 rollFactor18, unk1A;
    s16 headingDamping1C;
    s16 slideSteerDivisor1E;
} VehicleHandlingPrefix;

/* Per-gear curves start at car-record+4, with a 0x3C stride. */
typedef struct {
    s16 scaledRate00, speed02;
    s16 unk04, unk06;
    s16 torqueCurve08[21];
    s16 accumulator32;
    s16 unk34[4];
} CarGearRecord3C;

/* All 640 car assets in PAL R4.BIN carry this 0x218-byte record.
 * InitPlayerCar selects handling at +0x1C8; both pointers share this owner. */
typedef struct {
    s32 lastGear00;
    CarGearRecord3C gears04[7];
    u8 reserved1A8[0x1C8-0x1A8];
    VehicleHandlingPrefix handling1C8;
    s32 targetFrames1E8[2];
    s16 topSpeed1F0;
    u8 reserved1F2, stage1F3, reserved1F4;
    char name1F5[10];
    u8 reserved1FF[0x218-0x1FF];
} CarRecord;

RR_ASSERT_SIZE(CarGearRecord3C,0x3C);
RR_ASSERT_OFFSET(CarGearRecord3C,speed02,2);
RR_ASSERT_OFFSET(CarGearRecord3C,accumulator32,0x32);
RR_ASSERT_OFFSET(CarRecord,gears04,4);
RR_ASSERT_OFFSET(CarRecord,handling1C8,0x1C8);
RR_ASSERT_OFFSET(VehicleHandlingPrefix,slideThreshold16,0x16);

RR_ASSERT_SIZE(CarRecord,0x218);
RR_ASSERT_OFFSET(CarRecord,targetFrames1E8,0x1E8);
RR_ASSERT_OFFSET(CarRecord,topSpeed1F0,0x1F0);
RR_ASSERT_OFFSET(CarRecord,stage1F3,0x1F3);
RR_ASSERT_OFFSET(CarRecord,name1F5,0x1F5);

#endif

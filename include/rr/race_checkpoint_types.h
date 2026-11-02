#ifndef RR_RACE_CHECKPOINT_TYPES_H
#define RR_RACE_CHECKPOINT_TYPES_H
#include "rr/race_types.h"
/* Complete 0x320-byte vehicle view, sharing the control record at 0x1F4. */
typedef struct {
    u8 reserved00[0x184];
    s32 first,second;
    u8 reserved18C[0x68];
    RrRaceVehicleControl control;
} RrRaceCheckpointVehicle;
RR_ASSERT_SIZE(RrRaceCheckpointVehicle,0x320);
RR_ASSERT_OFFSET(RrRaceCheckpointVehicle,first,0x184);
RR_ASSERT_OFFSET(RrRaceCheckpointVehicle,second,0x188);
RR_ASSERT_OFFSET(RrRaceCheckpointVehicle,control,0x1F4);
#endif

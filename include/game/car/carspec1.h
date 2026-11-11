#ifndef RR_GAME_CAR_CARSPEC1_H
#define RR_GAME_CAR_CARSPEC1_H

#include "types.h"
#include "rr/race_types.h"

/* Spec row `index`: the control block of ComCars[index] (CarSpecControlPool). */
RaceVehicleControl *CarSpecControl(s32 index);
void BlendCarSpec(int a0, int a1, int a2, int a3);

#endif /* RR_GAME_CAR_CARSPEC1_H */

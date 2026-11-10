#ifndef RR_GAME_CAR_CARSETUP_H
#define RR_GAME_CAR_CARSETUP_H

#include "rr/race_types.h"

void ComputeWheelOffsets(RaceVehicle *a0);
void InitPlayerCar(RaceVehicle *vehicle, s32 recordIndex, s32 gridIndex);
void UpdatePlayerCar(RaceVehicle *a0, int a1);
void StepCarPhysics(RaceVehicle *arg0);
void UpdateCarStartLineSteering(RaceVehicle *arg0);


#endif /* RR_GAME_CAR_CARSETUP_H */

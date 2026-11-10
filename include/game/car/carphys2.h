#ifndef RR_GAME_CAR_CARPHYS2_H
#define RR_GAME_CAR_CARPHYS2_H

#include "types.h"
#include "rr/race_types.h"

void SteerCarTowardTrack(RaceVehicle *arg0);
int GetTrackTargetHeading(RaceVehicle *a);
int LerpAngle12(int a0, int a1, int a2);
void StepCarStateNormal(RaceVehicle *c);
void StepCarStateSlide(RaceVehicle *c0);
void StepCarStateSpinOut(RaceVehicle *a0);

#endif /* RR_GAME_CAR_CARPHYS2_H */

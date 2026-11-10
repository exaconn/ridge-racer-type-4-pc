#ifndef RR_GAME_CAR_CARPARAM_H
#define RR_GAME_CAR_CARPARAM_H

#include "types.h"
#include "rr/race_types.h"

void UpdatePlayerSteering(RaceVehicle *arg0, int arg1);
void UpdateCarEngine(RaceVehicle *a0);
void IntegrateCarMotion(RaceVehicle *p);

#endif /* RR_GAME_CAR_CARPARAM_H */

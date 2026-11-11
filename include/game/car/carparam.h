#ifndef RR_GAME_CAR_CARPARAM_H
#define RR_GAME_CAR_CARPARAM_H

#include "types.h"
#include "rr/race_types.h"

/* Per-player engine outputs from UpdateCarEngine: smoothed tachometer RPM
 * (0x800F4C70) and engine-load sound flag (0x800F2AB4). */
extern s16 PlayerDisplayRpm[2];
extern s16 PlayerThrottleLoad[2];

void UpdatePlayerSteering(RaceVehicle *arg0, int arg1);
void UpdateCarEngine(RaceVehicle *a0);
void IntegrateCarMotion(RaceVehicle *p);

#endif /* RR_GAME_CAR_CARPARAM_H */

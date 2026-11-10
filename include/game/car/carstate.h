#ifndef RR_GAME_CAR_CARSTATE_H
#define RR_GAME_CAR_CARSTATE_H

#include "types.h"

struct RaceVehicle;
void SetCarImpactState(int state, struct RaceVehicle *vehicle);
void UpdateCarShake(struct RaceVehicle *vehicle);

#endif /* RR_GAME_CAR_CARSTATE_H */

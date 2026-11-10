#ifndef RR_GAME_CAR_AIPATH_H
#define RR_GAME_CAR_AIPATH_H

#include "types.h"
#include "rr/race_types.h"

s32 FindTrackNodeUnbounded(s32 *pos, s32 startIdx);
void UpdateCarTrackNode(RaceVehicle *p);
void ClampAiCarLateralMotion(RaceVehicle *arg0);
void UpdateCarGear(RaceVehicle *a0, int a1);
int FindTrackNodeAtXZ(int a0, int a1);

#endif /* RR_GAME_CAR_AIPATH_H */

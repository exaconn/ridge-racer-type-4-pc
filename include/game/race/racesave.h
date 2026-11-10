#ifndef RR_GAME_RACE_RACESAVE_H
#define RR_GAME_RACE_RACESAVE_H

#include "rr/vehicle_table.h"

void ReadCarInput(RaceVehicle *vehicle, s32 player);
void CheckRaceFinish(RaceVehicle *vehicle);
void SaveCarSnapshots(s32 unused);
void ReplayModeEnter(void);
void ReplayModeUpdate(void);


#endif /* RR_GAME_RACE_RACESAVE_H */

#ifndef RR_GAME_CAR_CARSTAT_H
#define RR_GAME_CAR_CARSTAT_H

#include "types.h"

void InitGamePads(void);
void SetPadButtonMapLayout(s32 player, s32 standard, s32 calibrated, s32 wheel);
void ApplyOptionsPadButtonMaps(void);

#endif /* RR_GAME_CAR_CARSTAT_H */

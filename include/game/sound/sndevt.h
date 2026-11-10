#ifndef RR_GAME_SOUND_SNDEVT_H
#define RR_GAME_SOUND_SNDEVT_H

#include "types.h"

void SoundListenerInit(int arg0, int arg1);
void SoundFindNearestCars(s32 arg0);
void SoundCalcCarParams(s32 idx);
void UpdateCarSpatialAudio(int arg0);

#endif /* RR_GAME_SOUND_SNDEVT_H */

#ifndef RR_GAME_EFFECT_CARFX_H
#define RR_GAME_EFFECT_CARFX_H

#include "types.h"
#include "rr/race_types.h"

int StepCarShowcasePhysics(RaceVehicle *car, s32 time, s32 track, s32 *segment);
void UpdateSparkBurstEffectA();
void UpdateSparkBurstEffectB();
void DrawSparkHudOverlay();
void DrawHudSprite(int a0, int a1, int a2, int a3, int a4);
void DrawHudDigit(s32 a0, s32 a1, s32 a2, s32 a3);


#endif /* RR_GAME_EFFECT_CARFX_H */

#ifndef RR_GAME_EFFECT_CARFX_H
#define RR_GAME_EFFECT_CARFX_H

#include "types.h"
#include "rr/race_types.h"

int StepCarShowcasePhysics(RaceVehicle *car, s32 time, s32 track, s32 *segment);
/* Spark showcase overlays: frame within the effect and its fade-out start. */
void UpdateSparkBurstEffectA(int frame, int fadeStart);
void UpdateSparkBurstEffectB(int frame, int fadeStart);
void DrawSparkHudOverlay(int kind, int frame, int fadeStart);
/* Replay-camera cut sprite (centred when a4 != 0) with draw mode E1000229. */
void DrawHudSprite(int index, int x, int y, int brightness, int centred);
void DrawHudDigit(s32 digit, s32 x, s32 y, s32 brightness);


#endif /* RR_GAME_EFFECT_CARFX_H */

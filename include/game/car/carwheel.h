#ifndef RR_GAME_CAR_CARWHEEL_H
#define RR_GAME_CAR_CARWHEEL_H

#include "types.h"
#include "rr/race_types.h"
#include "rr/preview_rgb.h"

int TriggerWheelBump(RaceVehicle *a, int b);
void DrawPreviewCar(RaceVehicle *vehicle, PreviewRgb3 color, s32 brightness, s32 flags);
void DrawPlayerCar(RaceVehicle *vehicle, s32 viewMode);
void DrawOpponentCar(RaceVehicle *vehicle, s32 viewMode);

#endif /* RR_GAME_CAR_CARWHEEL_H */

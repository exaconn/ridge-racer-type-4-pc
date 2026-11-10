#ifndef RR_GAME_UI_UIPART1_H
#define RR_GAME_UI_UIPART1_H

#include "types.h"

int FramesToMilliseconds(s32 frames, s32 subframe);
void DrawRaceTime();
char *FormatMillisecondsTime(int t);
struct RaceVehicle;
void ResetVehicleShake(struct RaceVehicle *vehicle);
void StartCollisionShake(struct RaceVehicle *vehicle, s32 impactKind);
void SetVehicleShake(struct RaceVehicle *vehicle, s32 x, s32 y);

#endif /* RR_GAME_UI_UIPART1_H */

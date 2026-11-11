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


void ClearScreenOverlayState(void);
void DrawMenuStepArrows(s32 x, s32 y, s32 width, s32 mode, s32 selection, s32 first, s32 count);
void StepMenuValue(s32 mode, s32 width, char *selection, s32 limit, s32 flags);
void SetMenuFrameOptions(s32 first, s32 second, s32 third);
void FormatDecimalDigits(char *output, s32 value, s32 digits);
void FormatRaceTime(char *output, s32 time);

#endif /* RR_GAME_UI_UIPART1_H */

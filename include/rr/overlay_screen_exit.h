#ifndef RR_OVERLAY_SCREEN_EXIT_H
#define RR_OVERLAY_SCREEN_EXIT_H
#include "rr/frame_environment.h"
#include "rr/screen_tiles.h"

extern u16 ModeGroup,ModeIndex;
extern s16 SubmodeState;
void StopRaceAudioAndRumble(void),SetupRaceAndSceneId(void);
void ovl667_exit_race(void);
#endif

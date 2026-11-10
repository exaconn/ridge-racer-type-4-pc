#ifndef RR_GAME_DEMO_OPENING_H
#define RR_GAME_DEMO_OPENING_H

#include "types.h"

extern char CdTrackTitleStrings[];
extern s16 *D_800A18F0[];
extern s16 *D_800A1910[];
extern s16 *D_800A1930[];
void StartOpeningFadeOut(void);
void EmitProjectedMarkerSprt(int a0, int a1, int a2);
void EmitOpeningDrawModePrim(void);
s32 UpdateRaceOpeningOverlay(s32 arg0);


#endif /* RR_GAME_DEMO_OPENING_H */

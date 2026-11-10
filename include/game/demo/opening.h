#ifndef RR_GAME_DEMO_OPENING_H
#define RR_GAME_DEMO_OPENING_H

#include "types.h"

extern char CdTrackTitleStrings[];
extern s16 *OpeningDateGlyphLists[];
extern s16 *OpeningTimeGlyphLists[];
extern s16 *OpeningStageGlyphLists[];
void StartOpeningFadeOut(void);
void EmitProjectedMarkerSprt(int a0, int a1, int a2);
void EmitOpeningDrawModePrim(void);
s32 UpdateRaceOpeningOverlay(s32 arg0);


#endif /* RR_GAME_DEMO_OPENING_H */

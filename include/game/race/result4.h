#ifndef RR_GAME_RACE_RESULT4_H
#define RR_GAME_RACE_RESULT4_H

#include "types.h"
#include "rr/ui_bar.h"

/* Texture rectangles and eight pairs of mode-dependent label indices. */
extern UiUvRect ResultMenuRowTable[24];
extern u8 ResultMenuTableIndices[16];

void DrawResultsStandingsOverlay(int a0);
void InitResultsMenuRows(int a0);
void NewCarModeEnter(void);
void NewCarModeUpdate(void);

#endif /* RR_GAME_RACE_RESULT4_H */

#ifndef RR_GAME_MENU_MENUSTAT_H
#define RR_GAME_MENU_MENUSTAT_H

#include "types.h"

struct GpuDmaTag;

extern void *SavedLoadCursor;

/* Emits up to four badge glyph sprites of pattern a at (y, w); returns the cursor. */
void *AddBadgeSpriteStrip(int a, struct GpuDmaTag *ordering, void *cursor, int y, int w, int alpha);
void CarStandingsModeUpdate(void);


#endif /* RR_GAME_MENU_MENUSTAT_H */

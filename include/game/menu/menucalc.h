#ifndef RR_GAME_MENU_MENUCALC_H
#define RR_GAME_MENU_MENUCALC_H

#include "types.h"

struct GpuDmaTag;
void *DrawCarStatBars(void *cursor, int alpha);
/* Ordering is unused; tiles link at the current frame's first entry. */
void *DrawStepProgressBar(struct GpuDmaTag *ordering, void *cursor);
void DialogueIntroModeUpdate(void);
void DialogueSceneSetup(void);
void DialogueModeUpdate(void);
void AddScreenBackdropTile(void);


#endif /* RR_GAME_MENU_MENUCALC_H */

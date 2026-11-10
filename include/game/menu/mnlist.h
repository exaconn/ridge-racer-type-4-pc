#ifndef RR_GAME_MENU_MNLIST_H
#define RR_GAME_MENU_MNLIST_H

#include "rr/ui_bar.h"

/* Resident entries of the PanelDraw table. Each returns its next arena cursor. */
void *DrawCarSpecBadgePanel(u32 *ordering, void *cursor, s32 row);
void *DrawGpSaveSlotsPanel(u32 *ordering, void *cursor, s32 row);
void *DrawControllerConfigPanel(u32 *ordering, void *cursor, s32 row);
char *FormatCountOfTotal(s32 first, s32 second, s32 digitsMinusOne);
extern UiUvRect D_8009FBEC;

#endif

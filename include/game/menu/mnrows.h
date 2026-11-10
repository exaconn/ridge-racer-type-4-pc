#ifndef RR_GAME_MENU_MNROWS_H
#define RR_GAME_MENU_MNROWS_H

#include "types.h"
#include "rr/menu_arena_state.h"

extern int Rect16RowCount;
extern int D_800ACC1C;
extern int ImageRowCount;
extern int GlyphRowCount;
void UpdateSelectionRank(void);
void ResetScreenCounters(void);
void InitMenuArenas(void);
void InitMenuScript(void);
void UpdateMenuScript(void);


#endif /* RR_GAME_MENU_MNROWS_H */

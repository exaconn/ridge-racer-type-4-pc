#ifndef RR_OVERLAY_MENU_INIT_H
#define RR_OVERLAY_MENU_INIT_H
#include "rr/menu_arenas.h"
#include "rr/menu_setup.h"
#include "rr/menu_selection.h"
#include "rr/overlay_menu.h"
#include "rr/renderer_setup.h"
extern s32 ovl663_D_801168C4,ovl663_D_801168C8,ovl663_D_801168CC;
extern s32 ovl663_D_80116898;
/* Requires two badge rows. The final flag belongs to row1, not to padding
 * beyond a single row; the badge helper may replace that array before return. */
void ovl663_func_80115028(s32 mode);
/* The loader may replace the second arena. Layout calls may replace the UI
 * root and local condition; both are read after all four rows are configured. */
void ovl663_func_801148D8(void);
void ovl665_func_80114FD8(void);
void ovl666_func_801148D4(void);
#endif

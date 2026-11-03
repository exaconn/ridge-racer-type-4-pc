#ifndef RR_MENU_SCREEN_EXIT_H
#define RR_MENU_SCREEN_EXIT_H
#include "rr/frame_environment.h"
#include "rr/screen_tiles.h"
#include "game/race/raceset.h"
/* These scene exits draw through the containing frame's ordering table.
 * The first rectangle is the published screen context. */
extern u16 D_800F4C6A;
extern s16 D_800F3A26,D_800F4D8A;
void func_80036068(void);
void ovl660_func_8011712C(void);
void ovl661_func_80114AA0(void);
#endif

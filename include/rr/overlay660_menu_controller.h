#ifndef RR_OVERLAY660_MENU_CONTROLLER_H
#define RR_OVERLAY660_MENU_CONTROLLER_H
#include "rr/overlay_menu.h"
#include "rr/menu_selection.h"
#include "rr/frame_environment.h"
extern s32 D_800F2DE4,D_800ABEB0,D_800F3A64;
extern s16 D_800AC5D0,D_800F3A1E,D_800F3A26,D_800FFC18;
extern u16 D_800F4D8A,D_800F4C6A;
extern u8 *D_800F2AB0;
extern s32 ovl660_D_8011790C;
s32 func_8003BAB0(void);
void func_8003A198(void);
void func_80033368(s32,s32);
void func_80032F44(s32,s32);
void func_8002AF8C(s32,s32,s32);
void func_8004F24C(s32);
void func_80053AF4(s32,s32);
void func_80053CB0(void);
void func_8003EC88(s32);
void func_800207D4(u32 *,s32,u32);
void func_8002A344(s32);
void ovl660_func_801163C4(void);
/* UI backing covers at least five rows and every positive row count. The
 * highlight cell count is0..8; selection stays0..2 at indexed accesses. Course
 * selectors retain signed16 values; input and sound services may change roots,
 * selections,counts and state. Timers wrap16,counters and title limit wrap32. */
void ovl660_func_80114D38(void);
#endif

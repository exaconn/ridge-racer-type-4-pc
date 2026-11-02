#ifndef RR_OVERLAY663_MENU_HANDLER_H
#define RR_OVERLAY663_MENU_HANDLER_H
#include "rr/overlay_menu_init.h"
#include "rr/frame_environment.h"
extern s32 ovl663_D_801168A0[9];
extern u16 D_800F4D8A,D_800F4C6A;
extern s16 D_800AC5D0,D_800F3A26;
s32 func_8003BEA8(s32);
void ovl663_func_801158B4(void);
void func_800207D4(u32 *,s32,u32);
void func_8003EC88(s32);
void func_8002A344(s32);
/* Selection remains0..8 across callbacks; all nine table entries are readable.
 * UI helpers may replace the complete row allocation. Countdown wraps signed16. */
void ovl663_func_80115964(void);
/* Four complete UI rows are available; the selected row is0..3 after input
 * services. Sound/layout helpers may replace the UI root and selection.
 * Completion requires the full32-bit loader result1. */
void ovl663_func_80114B98(void);
#endif

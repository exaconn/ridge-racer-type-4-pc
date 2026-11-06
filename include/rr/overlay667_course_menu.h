#ifndef RR_OVERLAY667_COURSE_MENU_H
#define RR_OVERLAY667_COURSE_MENU_H
#include "rr/overlay_menu.h"
#include "rr/menu_selection.h"
#include "rr/frame_environment.h"
#include "rr/menu_arenas.h"
#include "rr/tables.h"
#include "rr/driver_control.h"
#include "game/race/racegrid.h"
extern RrMenuItemState5C D_800F3A38[2];
extern s16 D_800AC5D0,D_800AD508,D_800FF688,D_8010C0B8,D_8010C0C8,D_800F3A26;
extern u16 D_800F4D8A,D_800F4C6A;
extern s32 D_8010C078,D_800F3A30,D_800F4CB8,D_800F2DE4,ScreenFrame;
s32 func_80034FC0(void);
int func_80035024();
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
void ovl667_func_801161A8(s32);
/* At least25 complete UI rows are accessible. Menu selection is0..2 at
 * indexed accesses; the live highlight count is0..8. D_800AD508 is0..1
 * before scene8 reset,so initializer161A8 receives a valid player0 or2. Services may replace
 * roots,selections,counts and resources. Both counters wrap32 and the timer
 * wraps signed16. Driver/loader results are compared as complete32-bit words. */
void ovl667_func_80117FA8(void);
#endif

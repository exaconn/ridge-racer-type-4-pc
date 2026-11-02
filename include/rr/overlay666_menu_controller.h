#ifndef RR_OVERLAY666_MENU_CONTROLLER_H
#define RR_OVERLAY666_MENU_CONTROLLER_H
#include "rr/overlay_menu.h"
#include "rr/save_progress.h"
#include "rr/tables.h"
extern RrMenuItemState5C D_800F3A38[2];
extern s32 D_800F2DE4,D_800ABEB0;
extern u16 D_800F4D8A,D_800F4C6A;
extern s16 D_800AC5D0;
extern s16 D_800F3A26;
extern s16 ovl666_D_80116F88[4];
s32 func_8003B47C(void);
void func_8002AF8C(s32,s32,s32);
void func_800207D4(u32 *,s32,u32);
void func_80053AF4(s32,s32);
void func_80053CB0(void);
void func_8003EC88(s32);
void func_8002A344(s32);
void ovl666_func_801152B0(s32,s32);
void ovl666_func_80115A88(void);
/* Three complete UI rows and four glyph rows are accessible. Selection is0..1
 * and glyph selection0..3 at each indexed access; services may change either
 * selection or backing root. Timers wrap signed16. Readiness is the full-word
 * result1,not a truncated halfword. The fade helper inserts into one order head. */
void ovl666_func_80114CCC(void);
#endif

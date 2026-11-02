#ifndef RR_OVERLAY661_COURSE_MENU_H
#define RR_OVERLAY661_COURSE_MENU_H
#include "rr/ui.h"
#include "rr/icon_sprites.h"
#include "rr/tables.h"
#include "rr/selection_types.h"
#include "rr/menu_selection_word.h"
#include "rr/menu_arena_types.h"
extern s32 D_800ABEB0,D_800F2DE4,D_800F3A30,D_800F4CB8,ovl661_D_80118414;
extern s16 D_800AC648,D_800AC64A,D_800AC5E0,D_800AC5D0,D_800F3914,D_800F3A26,D_800F4BE8,D_800F4C6A,D_800F4D44;
extern s16 ovl661_D_80118448[2];
extern s16 D_800ABEC8;
extern u16 D_800F4D8A;
extern RrMenuSelectionWord32 D_800AC59C;
extern RrSelectionRows D_800F4D98;
extern RrMenuItemState5C D_800F3A38[2];
extern RrMenuArenas D_800F4D70;
extern void *D_800ACC18;
extern u8 D_80118374[4];
void func_80014A8C(s32);void func_800207D4(u32*,s32,u32);void func_8002A344(s32);void func_8002AF8C(s32,s32,s32);
void func_8002B12C(s32,s32,s32,s32,s32,s32,s32);void func_8002B330(s32,s32,char*,s32,s32);
s32 func_80032E9C(s32);void func_80032F44(s32,s32);void func_80033368(s32,s32);void func_8003A198(void);s32 func_8003AE4C(s32,s32);s32 func_8003BAB0(void);void func_8003EC88(s32);void func_8004F24C(s32);void func_80053AF4(s32,s32);void func_80053CB0(void);void ovl661_func_80115598(s32);
/* UI roots cover at least26 complete rows. Highlight count is0..10; its byte span may extend beyond cells[8]
 * into later bytes of the same complete row.
 * Parameter table indices are0..3. Menu phase0 accepts selection0..3; phase5
 * with row24 state2 requires selection0..1 before each indexed access,
 * including after services. Confirmation sets selection2 together with row
 * state3, leaving that indexed branch. AC59C owns a complete word edited by
 * width4 input; signed low-half reads use its union member. Input .a owns the
 * resident aliasF3A60. Services may replace roots and valid live selectors. */
void ovl661_func_80117430(void);
RR_ASSERT_SIZE(RrSelectionRows,32);
#endif

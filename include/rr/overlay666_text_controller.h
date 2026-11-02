#ifndef RR_OVERLAY666_TEXT_CONTROLLER_H
#define RR_OVERLAY666_TEXT_CONTROLLER_H
#include "rr/frame_storage_types.h"
extern RrFrameStorage *D_800ACC14;
#include "rr/menu_text_rows.h"

extern s32 D_800F2DE4;
extern u32 D_800ABEB0;
extern u16 D_800F4D8A;
extern s32 D_800F4C64;
extern s32 D_800F3A60;
extern s32 ovl666_D_80116F94;
extern s16 D_800AC5D0;
extern u16 D_800AC64A;
extern s16 D_800F4BE8;
extern s16 D_800F4C6A;
extern s16 D_800F3A26;

extern void func_8002AF8C(s32, s32, s32);
extern void func_8004F24C(s32);
extern s32 ovl666_func_80115510(s32);
extern void func_8003A198(void);
extern void func_8003A1B4(void);
extern void func_800207D4(u32 *, s32, u32);
extern s32 func_8003B12C(void);
extern void func_80053BB4(s32);
extern void ovl666_func_801148D4(void);
extern void func_8002A344(s32);
extern void func_8003662C(s32);
extern void func_80040858(s32);
extern void func_800411D4(void);

/* Entry phase is one of0,3,4,5,6,8. Both live text roots expose at least44
 * complete60-byte rows; the frame root names a complete frame arena.
 * Services may replace roots and mutate all globals. Frame/tick counters wrap32,
 * countdown wraps16, signed animation bytes govern the three transitions.
 * Group is captured before helpers; rendering consumes11 rows of that group.
 */
void ovl666_func_80115590(void);
#endif

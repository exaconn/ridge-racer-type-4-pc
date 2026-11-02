#ifndef RR_OVERLAY659_PREVIEW_INIT_H
#define RR_OVERLAY659_PREVIEW_INIT_H
#include "rr/menu_choice.h"
#include "rr/race_types.h"
extern s32 D_800ABEB0,D_800F2DE4;
extern s16 D_800AC64A,D_800AC5E0,D_800F3A26;
extern u16 D_800F4D8A,D_800F4C6A;
extern u8 *D_800F4D74;
extern RrRaceVehicle D_800ABEF8[2];
void func_8002A344(s32);
void func_8002A604(void);
void func_8002E4F4(RrRaceVehicle *);
void func_8003A1B4(void);
s32 func_8003BAB0(void);
void func_80052800(void);
void func_800528A8(s32,s32);
void func_80053CB0(void);
/* Seven complete UI rows and two vehicles are available. Services may change
 * the UI root, selection and resource pointer. Completion requires the full
 *32-bit loader result1. The selected byte may be outside0..7 (no highlight). */
void ovl659_func_801159E8(void);
#ifndef RR_MENU_PREVIEW_SCRATCH_WORDS
#define RR_MENU_PREVIEW_SCRATCH_WORDS ((s32 *)0x1F800000)
#endif
#endif

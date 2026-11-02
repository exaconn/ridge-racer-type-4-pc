#ifndef RR_OVERLAY659_RESULTS_CONTROLLER_H
#define RR_OVERLAY659_RESULTS_CONTROLLER_H
#include "rr/frame_environment.h"
#include "rr/menu_text_rows.h"
#include "rr/overlay_menu.h"
#include "rr/standings.h"
#include "rr/race_types.h"

extern s32 D_800F2DE4;
extern u16 D_800F4D8A;
extern s16 D_800F3914;
extern s32 D_800F3A30;
extern s16 D_800AC5D0;
extern s16 D_800F4BE8;
extern s16 D_800F4C6A;
extern s16 D_800F3A26;
extern s32 ovl659_D_801165BC;

extern RrRaceVehicle D_800ABEF8[2];

extern void func_8003A1B4(void);
extern s32 func_80090424(s32);
extern void func_8002B814(char *, s32, s32);
extern void func_8002B934(char *, s32);
extern void func_800207D4(u32 *, s32, u32);
extern s32 func_8003B12C(void);
extern void func_8002F958(void);
extern void func_80053BB4(s32);
extern void func_8002A344(s32);
extern void func_8003662C(s32);
extern void func_80040858(s32);
extern void func_800411D4(void);

/* Entry phases are0,3,4,5,6,7,8; settings phase17 remains in0..7,
 * including after callbacks. Both live text roots contain at least44 complete
 * rows, aligned for the state halfword at34. The context pointer exposes the
 * first ordering head of a complete frame arena. All services may replace
 * roots and mutate settings, standings, selection and counters.
 * The vehicle's halfword2A8 is reused as best-lap index after the race.
 * A lap display uses standing entries0..5; rendering uses groups0/33.
 * Frame arithmetic wraps32; timers and signed retry bytes wrap at wire width.
 */
void ovl659_func_80115224(void);
RR_ASSERT_OFFSET(RrSteeringState,finish20,0x20);
#endif

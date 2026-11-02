#ifndef RR_OVERLAY659_PREVIEW_CONTROLLER_H
#define RR_OVERLAY659_PREVIEW_CONTROLLER_H
#include "rr/overlay659_preview_init.h"

#include "rr/preview_rgb.h"
#include "rr/tables.h"

extern u8 *D_800F2AB0;
extern RrMenuItemState5C D_800F3A38[2];
extern s16 D_800AC5D0;
extern u16 D_800F4BE8;

void func_80052A30(s32);
void func_8002B330(s32, s32, char *, s32, s32);
void func_80052B8C(s32, s32);
void func_80037110(void);
void func_800207D4(u32 *, s32, u32);
void func_80033368(s32, s32);
void func_80032F44(s32, s32);
void func_8003EC88(s32);
s32 func_8003FBA8(s32);
void func_8003D6F0(s32);
void func_80033A74(s32);
void func_8006EF8C(s32);
void func_80037198(s32);
void func_8002E5A0(RrRaceVehicle *);
void func_8002CC88(RrRaceVehicle *, RrPreviewRgb3, s32, s32);

/* Seven complete UI rows, two full vehicles, a badge and at least three mode
 * bytes remain accessible through live roots. Selection stays0..2, settings
 * phase17 stays0..7 and copies[0].bytes[3] stays0..1 across callbacks.
 * When the model branch enters with mode1/2, its preparation helpers leave
 * mode1/2 available at the color step. Other modes use the ordinary UI path.
 * Captured settings phase is passed to the two model selectors; UI, badge and
 * mode roots and subsequent selections are fresh after every service.
 * F3A60 is the first input record a field at28, shared with button acceptance.
 * RGB has three defined channels in one aligned ABI word; padding is ignored.
 */
void ovl659_func_80115CE4(void);
#endif

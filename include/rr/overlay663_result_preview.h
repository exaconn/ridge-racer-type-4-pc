#ifndef RR_OVERLAY663_RESULT_PREVIEW_H
#define RR_OVERLAY663_RESULT_PREVIEW_H
#include "rr/ui.h"
#include "rr/badge_rows.h"
#include "rr/race.h"
#include "rr/selection.h"
#include "rr/menu_transition.h"
#include "rr/preview_rgb.h"
extern u8 *D_800F2AB0;
extern s32 ovl663_D_801168C8,D_800F3A60;
void ovl663_func_80115028(s32);
void func_8002AF8C(s32,s32,s32);
void func_8004F24C(s32);
void func_80033A74(s32);
void func_8006EF8C(s32);
void func_80037198(s32);
void func_8002E5A0(RrRaceVehicle *);
void func_8002CC88(RrRaceVehicle *,RrPreviewRgb3,s32,s32);
void func_8003EC88(s32);
void func_8003D6F0(s32);
void func_8003FBA8(s32);
void func_8002A344(s32);
/* Four complete UI rows, a live badge owner and a three-byte mode backing
 * are required. Services may replace UI/badge/mode roots. Byte3 of the full
 * selection owner is set only around the model-selection helper. */
void ovl663_func_80115C34(void);
#endif

#ifndef RR_OVERLAY662_PALETTE_CONTROLLER_H
#define RR_OVERLAY662_PALETTE_CONTROLLER_H
#include "rr/menu_transition.h"
#include "rr/overlay662_palette_initializer.h"
#include "rr/overlay662_image_fade.h"
#include "rr/tables.h"
extern RrMenuItemState5C D_800F3A38[2];
void ovl662_func_80118ED8(void);
s32 func_8003DCEC(s32);
void func_8003EC88(s32);
void func_8003D6F0(s32);
void func_8002A344(s32);
/* Palette selection0..24 indexes the shared mapping; values0..25 name the
 * two existing TIM banks. UI selection names a writable row; both closing
 * rows exist in every freshly published bank. Services may replace roots and
 * modify later inputs/selections. Closing captures the original row/badge
 * phases across sound calls, then reloads the roots. Row count is reloaded
 * after each draw; the caller/services must bound the loop and row backing.
 * Byte-address formation retains the complete native address before adding
 * the record offset. The unsigned scene phase dispatches only0,3,4. */
void ovl662_func_80119E00(void);
#endif

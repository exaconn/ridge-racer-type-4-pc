#ifndef RR_OVERLAY_EXTRA_LABELS_H
#define RR_OVERLAY_EXTRA_LABELS_H
#include "rr/menu_label_render.h"
extern RrUiUvRect ovl663_D_80116890;
extern s32 ovl663_D_80116898,ovl663_D_801168D0[9];
#include "rr/overlay662_name_state.h"
extern s32 D_800ABEB0;
s32 func_800904F4(s32);
/* Row index names an accessible badge. The current unlock index is0..8 and
 * its count fits two digits. Eight label bytes are NUL or supported characters.
 * Helper return values are full native arena pointers. Globals remain live
 * across service calls; the row position and current label byte are captured. */
void *ovl663_func_80116744(u32 *,void *,s32);
void ovl662_func_80118238(u32 *,void *,s32);
#endif

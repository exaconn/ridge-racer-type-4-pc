#ifndef RR_OVERLAY_EXTRA_LABELS_H
#define RR_OVERLAY_EXTRA_LABELS_H
#include "rr/menu_label_render.h"
extern RrUiUvRect ovl663_record_label;
extern s32 ovl663_selected_record,ovl663_course_unlock_levels[9];
#include "rr/overlay662_name_state.h"
extern s32 D_800ABEB0;
s32 rcos(s32);
/* Row index names an accessible badge. The current unlock index is0..8 and
 * its count fits two digits. Eight label bytes are NUL or supported characters.
 * Helper return values are full native arena pointers. Globals remain live
 * across service calls; the row position and current label byte are captured. */
void *ovl663_draw_record_labels(u32 *,void *,s32);
void ovl662_func_80118238(u32 *,void *,s32);
#endif

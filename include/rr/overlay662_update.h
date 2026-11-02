#ifndef RR_OVERLAY662_UPDATE_H
#define RR_OVERLAY662_UPDATE_H
#include "rr/overlay662_render_step.h"
#include "rr/menu_background.h"
#include "rr/overlay662_garage.h"
#include "rr/frame_environment.h"
#include "rr/screen_tiles.h"
#include "rr/menu_text_state.h"
void func_8003D6F0(s32 index);
void func_8003EC88(s32 index);
void func_8002AF8C(s32 first,s32 second,s32 third);
/* D_ACC14 points at the first rectangle of a containing frame owner.
 * Callback writes are visible to every later flag, fade and text-root read. */
#endif

#ifndef RR_OVERLAY662_SCALE_PANEL_H
#define RR_OVERLAY662_SCALE_PANEL_H
#include "rr/overlay662_primitives.h"
#include "rr/animated_sprite_grid.h"
#ifndef RR_GARAGE_SCRATCH
#define RR_GARAGE_SCRATCH RR_GPU_SCRATCH
#endif
/* config1C must be2..255 at both reads. Capture scale and ordering before
 * callbacks; thread each full returned cursor through the shared scratch cell. */
/* Action record51 supplies its word argument(currently0); it is ignored. */
void ovl662_draw_emblem_preview_box(s32 renderArgument);
#endif

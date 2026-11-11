#ifndef RR_OVERLAY662_SCROLL_DRAG_H
#define RR_OVERLAY662_SCROLL_DRAG_H
#include "rr/overlay662_image_drag.h"
#include "rr/overlay662_garage.h"
/* Saved vertical drag metrics: image thumb/travel and track thumb/travel.
 * They are separate original scalar owners at27280..2728C. */
extern s32 ovl662_vscroll_image_thumb,ovl662_vscroll_image_travel,ovl662_vscroll_track_thumb,ovl662_vscroll_track_travel;
extern u16 ovl662_vscroll_grab_offset; /* Grab offset within the projected image thumb. */
/* The original symbol at 8012767E is a field of the cursor working owner. */
#define ovl662_editor_cursor_y_alias (ovl662_editor_cursor_state.points.cursor14.y)
/* Starting requires zoom>=4 and a hit in216,87,4,224. Services may change
 * state;after the clip service zoom must remain4..255. Active updates require
 * previously initialized metrics: image travel32..64,track travel112..223;
 * grab offset is an unsigned halfword. These bounds keep products signed32
 * and divisions nonzero. Release restores clip13,63,294,346 before returning.
 * Vertical scroll narrows to signed16 before clamping to0..imageTravel. */
void ovl662_drag_v_scrollbar(void);
/* Horizontal peer uses hit90,337,112,8 and tool46. Its saved image travel
 * is32..64 and track travel56..112;other service/narrowing rules are shared. */
extern u16 ovl662_hscroll_grab_offset;
extern s32 ovl662_hscroll_image_thumb,ovl662_hscroll_image_travel,ovl662_hscroll_track_thumb,ovl662_hscroll_track_travel;
#define ovl662_editor_cursor_x_alias (ovl662_editor_cursor_state.points.cursor14.x)
void ovl662_drag_h_scrollbar(void);
#endif

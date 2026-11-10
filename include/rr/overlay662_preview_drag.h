#ifndef RR_OVERLAY662_PREVIEW_DRAG_H
#define RR_OVERLAY662_PREVIEW_DRAG_H
#include "rr/overlay662_image_drag.h"
/* Genuine adjacent grab halfwords at27290/27292,followed by image thumb
 * and complementary travel at27294/27298. */
typedef u16 GaragePreviewGrab[2];
extern GaragePreviewGrab ovl662_preview_drag_grab;
extern s32 ovl662_preview_drag_thumb,ovl662_preview_drag_limit;
/* Active updates require saved image travel32..64 and unsigned16 grab values.
 * Start requires zoom>=4 and a hit in237,71,64,128. After clip,zoom remains
 *4..255;the second hit service preserves thumb0..32 and travel32..64.
 * Services may mutate cursor,origin and other state;later reads are fresh.
 * Release clears bit8 and restores clip13,63,294,346. Both output origins
 * narrow to signed16 before clamping. Initial Y grab uses cursorY/2-35;
 * active Y uses (cursorY-71)/2,with signed truncation toward zero in both. */
void ovl662_drag_preview_viewport(void);
RR_ASSERT_SIZE(GaragePreviewGrab,4);
#endif

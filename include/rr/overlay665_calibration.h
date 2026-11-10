#ifndef RR_OVERLAY665_CALIBRATION_H
#define RR_OVERLAY665_CALIBRATION_H
#include "rr/overlay665_panels.h"
#include "rr/frame_storage_types.h"
#include "rr/diagnostics.h"
extern FrameStorage DrawBuffers[2];
extern UiUvRect ovl665_screen_frame_uv_rects[2],ovl665_screen_arrow_uv_rects[2];
extern const char ovl665_screen_offset_debug_fmt[];
void ovl665_init_screen_adjust(void);
/* Two complete frame-storage owners and both two-entry glyph tables exist.
 * The selected frame has a full ordering context. Each threaded emitter cursor
 * and the final8-byte command need aligned writable packet backing; callbacks
 * may change roots,settings,scratch cursor and returned cursor independently.
 * Software pointers retain native width; only DMA links serialize low24 bits.
 * Display adjustments wrap32 before clamping,and countdown wraps signed16. */
void ovl665_update_screen_adjust(void);
#endif

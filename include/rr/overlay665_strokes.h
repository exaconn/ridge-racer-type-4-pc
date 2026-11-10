#ifndef RR_OVERLAY665_STROKES_H
#define RR_OVERLAY665_STROKES_H
#include "rr/layout.h"
#include "rr/line_emit.h"

typedef struct {s16 direction,length;} StrokeStep4;
typedef struct {u8 bytes[4];} StrokeColor4;
extern StrokeColor4 ovl665_callout_stroke_color;
extern u16 ovl665_callout_endpoint_coords[5][2];

/* Five readable path slots; each selected path terminates at direction -1.
 * Other directions are 0..3. Paths remain live during rendering. Arithmetic
 * wraps as MIPS words; coordinate storage wraps to sixteen bits. */
void ovl665_draw_callout_strokes(const StrokeStep4 *const *paths,s32 factor);
RR_ASSERT_SIZE(StrokeStep4,4);
RR_ASSERT_SIZE(StrokeColor4,4);
RR_ASSERT_OFFSET(StrokeStep4,length,2);
#endif

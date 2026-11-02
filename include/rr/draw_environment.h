#ifndef RR_DRAW_ENVIRONMENT_H
#define RR_DRAW_ENVIRONMENT_H

#include "rr/frame_environment.h"

s32 func_800940E0(s32 draw_display,s32 dither,s32 page);
u32 func_80094230(s32 x,s32 y);
u32 func_8009424C(RrGpuRectangleFields *window);
/* Three-word drawing-mode packet; NULL window emits command zero. */
void func_80093C24(void *output,s32 draw_display,s32 dither,s32 page,
    RrGpuRectangleFields *window);
/* Writes drawing commands1..6 and the DMA length byte. When byte18 (the
 * existing rgb24 field) enables background fill, also writes words7..9.
 * Output needs ten words and may occupy environment.draw_packet1C. Other output
 * storage must be disjoint from descriptor fields. Helpers may mutate the
 * descriptor and limits: later fields are read after each helper returns.
 * Negative dimensions clamp to0; nonnegative dimensions compare against the
 * signed limit-1, then retain the low16 of either dimension or limit-1. */
void func_80093C78(u32 *output,RrFrameEnvironment *environment);

/* Same descriptor, output capacity, aliasing and helper-mutation contract.
 * Selects the GPU fill command when clip.x and clamped width are multiples
 * of64; otherwise emits a rectangle relative to the drawing offset. */
void func_80093E70(u32 *output,RrFrameEnvironment *environment);

#endif

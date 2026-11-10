#ifndef RR_DRAW_ENVIRONMENT_H
#define RR_DRAW_ENVIRONMENT_H

#include "rr/gpu_environment_types.h"

s32 get_mode(s32 draw_display,s32 dither,s32 page);
u32 get_ofs(s32 x,s32 y);
u32 get_tw(GpuRectangleFields *window);
/* Three-word drawing-mode packet; NULL window emits command zero. */
void SetDrawMode(void *output,s32 draw_display,s32 dither,s32 page,
    GpuRectangleFields *window);
/* Writes drawing commands1..6 and the DMA length byte. When byte18 (the
 * background field) enables background fill, also writes words7..9.
 * Output needs ten words and may occupy environment.draw_packet1C. Other output
 * storage must be disjoint from descriptor fields. Helpers may mutate the
 * descriptor and limits: later fields are read after each helper returns.
 * Negative dimensions clamp to0; nonnegative dimensions compare against the
 * signed limit-1, then retain the low16 of either dimension or limit-1. */
void SetDrawEnv(u32 *output,DrawEnvironment *environment);

/* Same descriptor, output capacity, aliasing and helper-mutation contract.
 * Selects the GPU fill command when clip.x and clamped width are multiples
 * of64; otherwise emits a rectangle relative to the drawing offset. */
void SetDrawEnv2(u32 *output,DrawEnvironment *environment);

#endif

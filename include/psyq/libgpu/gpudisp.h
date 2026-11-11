#ifndef RR_PSYQ_LIBGPU_GPUDISP_H
#define RR_PSYQ_LIBGPU_GPUDISP_H

#include "rr/gpu_environment_types.h"

/* Horizontal timing for the two video standards and five width classes. */
typedef struct {u16 start,end;} GpuHorizontalRange;
extern GpuHorizontalRange DispHRangeTable[2][5];
extern u8 DispDotClockDivisors[5];
extern const char StrPutDispEnvFmt[];

/* Callbacks may change the descriptor, cached fields, timing tables and live
 * driver. Video-mode results used as table indices must be0 or1. Input and
 * cached descriptors may be identical. The final20-byte copy is read live. */
DisplayEnvironment *PutDispEnv(DisplayEnvironment *environment);
DisplayEnvironment *GetDispEnv(DisplayEnvironment *environment);

RR_ASSERT_SIZE(GpuHorizontalRange,4);

/* Three-word drawing-area packet. Helpers read the rectangle live, including
 * again after get_cs returns. The DMA link's low24 bits are preserved. */
void SetDrawArea(void *packet, GpuRectangleFields *rectangle);
void SetDrawOffset(void *packet, GpuRectangleFields *rectangle);
void SetTexWindow(void *packet, GpuRectangleFields *window);
/* Three-word drawing-mode packet; NULL window emits command zero. */
void SetDrawMode(void *output,s32 draw_display,s32 dither,s32 page,
    GpuRectangleFields *window);
/* Three-word DR_STP command backing, including the zero second command. */
void SetDrawStp(GpuDrawModePacket *packet, int enabled);

/* Encoded GPU commands and the SDK drawing-environment builders. */
u32 get_mode(s32 draw_display,s32 dither,s32 page);
u32 get_ofs(s32 x,s32 y);
/* E2 uses the low coordinate bytes and negated dimensions, quantized to
 * eight-pixel units. A NULL window returns command zero. Input is read live. */
u32 get_tw(GpuRectangleFields *window);
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


#endif /* RR_PSYQ_LIBGPU_GPUDISP_H */

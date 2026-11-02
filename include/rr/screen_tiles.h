#ifndef RR_SCREEN_TILES_H
#define RR_SCREEN_TILES_H
#include "rr/gpu.h"

typedef struct {
    u32 tag;
    u8 r,g,b,code;
    s16 x,y,w,h;
} RrGpuTilePacket16;
/* The drawing context starts with this rectangle. */
extern RrGpuRectangle *D_800ACC14;

/* Requires 4-byte-aligned writable cursor storage with room for four tiles and the following draw-mode packet at the
 * scratchpad cursor. Intensity is clamped0..255; mode uses its low16.
 * Each tile captures the current context after packet initialization. Height
 * is read after x, then again after y/width and before RGB writes. The cursor
 * is published before the final helper and replaced with its returned pointer. */
void func_800207D4(u32 *ordering, s32 intensity, u32 mode);

RR_ASSERT_SIZE(RrGpuTilePacket16, 16);
RR_ASSERT_OFFSET(RrGpuTilePacket16, b, 6);
RR_ASSERT_OFFSET(RrGpuTilePacket16, x, 8);
RR_ASSERT_OFFSET(RrGpuTilePacket16, h, 14);
#endif

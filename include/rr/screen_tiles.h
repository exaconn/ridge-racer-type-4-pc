#ifndef RR_SCREEN_TILES_H
#define RR_SCREEN_TILES_H
#include "rr/frame_storage_types.h"
#include "rr/gpu.h"

/* The drawing context starts with this rectangle. */

/* Requires 4-byte-aligned writable cursor storage with room for four tiles and the following draw-mode packet at the
 * scratchpad cursor. Intensity is clamped0..255; mode uses its low16.
 * Each tile captures the current context after packet initialization. Height
 * is read after x, then again after y/width and before RGB writes. The cursor
 * is published before the final helper and replaced with its returned pointer. */
void AddScreenFadeTiles(GpuDmaTag *ordering, s32 intensity, u32 mode);

#endif

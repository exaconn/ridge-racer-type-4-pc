#ifndef RR_OVERLAY662_DRAW_AREA_H
#define RR_OVERLAY662_DRAW_AREA_H
#include "types.h"
#include "rr/gpu.h"

/* Builds a DR_AREA packet (tag and two command words) at cursor, links it at
 * the head of ordering and returns the cursor past it. Coordinates and
 * dimensions keep their low16 bits in the rectangle. */
u8 *ovl662_emit_draw_area(GpuDmaTag *ordering,void *cursor,u32 x,u32 y,u32 width,u32 height);
#endif

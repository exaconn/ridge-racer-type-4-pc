#ifndef RR_OVERLAY662_DRAW_AREA_H
#define RR_OVERLAY662_DRAW_AREA_H
#include "types.h"

/* Packet supplies three writable aligned words. Coordinates/dimensions arrive
 * as complete argument words and preserve their low16 bits in the rectangle.
 * Ordering may alias packet words. Links use registered PAL low24 addresses;
 * the returned byte cursor keeps its complete native pointer width. */
u8 *ovl662_emit_draw_area(u32 *ordering,u32 *packet,u32 x,u32 y,u32 width,u32 height);
#endif

#ifndef RR_TEXTURED_TRIANGLE_H
#define RR_TEXTURED_TRIANGLE_H
#include "rr/layout.h"
/* Existing POLY_FT3 helper owns a complete32-byte packet. The byte owner
 * preserves the original struct tag; callers provide four-byte alignment
 * and an array with room for the returned next packet. */
typedef struct S32 {char b[0x20];} RrTexturedTriangleStorage;
/* x/y/u/v each expose three four-byte entries. The helper reads their low
 * halfwords/bytes,then narrows material inputs to their wire field widths.
 * Ordering is an actual writable GPU link,never an integer address. */
struct S32 *func_8002039C(u32 *ordering,struct S32 *packet,
    void *x,void *y,void *u,void *v,u16 tpage,u16 clut,u16 mode,u16 shade);
RR_ASSERT_SIZE(RrTexturedTriangleStorage,32);
#endif

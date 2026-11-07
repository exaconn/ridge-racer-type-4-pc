#ifndef RR_LINE_EMIT_H
#define RR_LINE_EMIT_H
#include "types.h"
/* Four coordinate halfwords and three RGB bytes, matching the resident entry.
 * The renderer reads these arrays and emits one LINE_F2 packet. */
void AddLineF2Prim(const s16 coordinates[4],const u8 rgb[3],s32 blend,s32 depth);
#endif

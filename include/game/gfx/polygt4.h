#ifndef RR_GAME_GFX_POLYGT4_H
#define RR_GAME_GFX_POLYGT4_H

#include "types.h"

struct GpuDmaTag;
/* Emits the animated menu grid as POLY_GT4 packets into one ordering entry;
 * a nonzero advance steps the wave phase first. */
void DrawMenuGridMesh(s32 index, struct GpuDmaTag *ordering, s32 advance);

#endif /* RR_GAME_GFX_POLYGT4_H */

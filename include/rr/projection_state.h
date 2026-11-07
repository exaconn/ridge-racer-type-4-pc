#ifndef RR_PROJECTION_STATE_H
#define RR_PROJECTION_STATE_H
#include "rr/animated_sprite_grid.h"
/* Scratchpad pointer cells precede seven scalar words. Native owners retain
 * complete pointer widths; PAL offsets are checked by the target compiler. */
typedef struct {
    RrGpuScratchPointers pointers;
    s32 word08,word0C,word10,word14,word18,word1C,word20;
} RrProjectionScratchPrefix;
typedef struct {u8 pad00[0x20];s32 words20[3];} RrSceneStatePrefix;
extern RrSceneStatePrefix ComCars;
#if defined(mips)
RR_ASSERT_SIZE(RrProjectionScratchPrefix,0x24);
RR_ASSERT_OFFSET(RrProjectionScratchPrefix,word08,8);
RR_ASSERT_OFFSET(RrProjectionScratchPrefix,word18,0x18);
#endif
RR_ASSERT_SIZE(RrSceneStatePrefix,0x2C);
RR_ASSERT_OFFSET(RrSceneStatePrefix,words20,0x20);
#endif

#ifndef RR_HUD_ROTATING_QUAD_H
#define RR_HUD_ROTATING_QUAD_H
#include "rr/hud_number.h"
/* RGB occupies three bytes of a GPU color word. Its fourth byte is union
 * padding: copying the union preserves an unspecified, unused byte. */
typedef union {
    struct {
        u8 r,g,b;
    } rgb;
    u32 word;
} HudQuadColor;
typedef struct {s16 x,y;HudQuadColor color;} HudQuadVertex;
/* Four XY/color pairs follow the first color. The final RGB write is at36,
 * in the next packet's tag storage, so the accessible emission spans40 bytes. */
typedef struct {
    GpuDmaTag tag;
    u8 r,g,b,code;
    HudQuadVertex vertices[4];
} HudQuadEmission;
typedef struct {s8 x,y;} HudQuadCorner;
extern HudQuadCorner UiBoxCornerOffsets[4];
#ifndef RR_HUD_QUAD_SCRATCH
#define RR_HUD_QUAD_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
/* Selected target and bank0..3 are accessible; cursor is4-byte aligned with
 *40 writable bytes. The live ordering head may alias this packet's tag.
 * Position/head/cursor are captured after both trig services. Advances36 bytes;
 * only low24 pointer bits are serialized into the GPU link. */
void DrawHudRotatingQuad(s32 phase,s32 index);
RR_ASSERT_SIZE(HudQuadColor,4);
RR_ASSERT_SIZE(HudQuadVertex,8);
RR_ASSERT_OFFSET(HudQuadVertex,color,4);
RR_ASSERT_SIZE(HudQuadEmission,40);
RR_ASSERT_OFFSET(HudQuadEmission,vertices,8);
RR_ASSERT_SIZE(HudQuadCorner,2);
#endif

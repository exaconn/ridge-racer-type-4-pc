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
/* POLY_G4: first color/code, then four XY/color pairs. The fourth pair's
 * color is not part of the packet (len 8); the emitter stages it anyway, so
 * the cursor advances by the packet without that trailing color. */
typedef struct {
    GpuDmaTag tag;
    u8 r,g,b,code;
    HudQuadVertex vertices[4];
} HudQuadEmission;
typedef struct {s8 x,y;} HudQuadCorner;
extern HudQuadCorner UiBoxCornerOffsets[4];
/* Emits one rotating Gouraud quad at the selected target's XY into the
 * current bank's ordering head. */
void DrawHudRotatingQuad(s32 phase,s32 index);
RR_ASSERT_SIZE(HudQuadColor,4);
RR_ASSERT_SIZE(HudQuadVertex,8);
RR_ASSERT_OFFSET(HudQuadVertex,color,4);
RR_ASSERT_SIZE(HudQuadCorner,2);
#endif

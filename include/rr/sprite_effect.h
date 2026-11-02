#ifndef RR_SPRITE_EFFECT_H
#define RR_SPRITE_EFFECT_H
#include "rr/ui_bar.h"

/* Input sprite geometry and texture coordinates, separate from GPU packets. */
typedef struct {
    s16 x,y;
    u16 page,clut;
    RrUiUvRect uv;
} RrSpriteEffectSource;
extern u8 *D_800ACC14;

/* Nonpositive count returns the cursor without accessing any input storage.
 * Otherwise descriptor storage must remain separate from the output packets.
 * Count 1 uses the supplied intensity directly; other counts use two RNG draws
 * per sprite, signed division of the wrapped intensity product, and jitter.
 * Sprites after index 2 retain the preceding Y coordinate. Helper return values
 * determine the next cursor; the current context and descriptor are re-read. */
u8 *func_8002018C(u32 *ordering,u8 *cursor,RrSpriteEffectSource *descriptor,
                 s32 semi,s32 strength,s32 count);
RR_ASSERT_SIZE(RrSpriteEffectSource,12);
RR_ASSERT_OFFSET(RrSpriteEffectSource,uv,8);
#endif

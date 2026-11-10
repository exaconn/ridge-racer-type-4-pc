#ifndef RR_HUD_NUMBER_H
#define RR_HUD_NUMBER_H
#include "rr/animated_sprite_grid.h"

/* Four bank-specific ordering heads followed by the signed screen position. */
typedef struct {u32 *heads[4];s16 x,y;} NumberTarget;
extern NumberTarget HudNumberTargets[];
extern s32 DrawBufIndex;

/* Writes one 12x16 glyph and advances the cursor by 20 bytes. Coordinate
 * narrowing belongs to packet serialization, after the full-word arguments. */
void *EmitHudGlyph12(u32 *ordering,void *cursor,s32 x,s32 y,s32 glyph,s32 clut);
/* The selected record and bank0..3 must be accessible. Emits a boolean
 * >=10000 prefix and four signed decimal remainders, then draw mode5.
 * Captures head/position before helpers and publishes the returned cursor. */
void DrawHudNumber5(s32 value,s32 index);
RR_ASSERT_SIZE(NumberTarget,20);
RR_ASSERT_OFFSET(NumberTarget,x,16);
RR_ASSERT_OFFSET(NumberTarget,y,18);
#endif

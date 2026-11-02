#ifndef RR_HUD_NUMBER_H
#define RR_HUD_NUMBER_H
#include "rr/animated_sprite_grid.h"

/* Four bank-specific ordering heads followed by the signed screen position. */
typedef struct {u32 *heads[4];s16 x,y;} RrNumberTarget;
extern RrNumberTarget D_8010BF88[];
extern s32 D_800F4BF0;

/* Writes one 12x16 glyph and advances the cursor by 20 bytes. Coordinate
 * narrowing belongs to packet serialization, after the full-word arguments. */
void *func_80021568(u32 *ordering,void *cursor,s32 x,s32 y,s32 glyph,s32 clut);
/* The selected record and bank0..3 must be accessible. Emits a boolean
 * >=10000 prefix and four signed decimal remainders, then draw mode5.
 * Captures head/position before helpers and publishes the returned cursor. */
void func_800217D0(s32 value,s32 index);
RR_ASSERT_SIZE(RrNumberTarget,20);
RR_ASSERT_OFFSET(RrNumberTarget,x,16);
RR_ASSERT_OFFSET(RrNumberTarget,y,18);
#endif

#ifndef RR_HUD_NUMBER_H
#define RR_HUD_NUMBER_H
#include "rr/animated_sprite_grid.h"

/* One HUD number placement per viewport: the ordering head for each frame
 * bank, then the screen positions used by the three-digit counter
 * (DrawHudNumber3), the single digit (DrawHudDigitMod10) and the five-digit
 * counter / rotating quad. PS1: 0x14 bytes (two 4-byte heads, three XY). */
typedef struct {s16 x,y;} HudNumberPoint;
typedef struct {
    GpuDmaTag *heads[2];
    HudNumberPoint number3, digit;
    s16 x,y;
} NumberTarget;
extern NumberTarget HudNumberTargets[];
extern s32 DrawBufIndex;

/* Writes one 12x16 glyph and advances the cursor by 20 bytes. Coordinate
 * narrowing belongs to packet serialization, after the full-word arguments. */
void *EmitHudGlyph12(GpuDmaTag *ordering,void *cursor,s32 x,s32 y,s32 glyph,s32 clut);
/* The selected record and bank0..3 must be accessible. Emits a boolean
 * >=10000 prefix and four signed decimal remainders, then draw mode5.
 * Captures head/position before helpers and publishes the returned cursor. */
void DrawHudNumber5(s32 value,s32 index);
#endif

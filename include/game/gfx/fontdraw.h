#ifndef RR_GAME_GFX_FONTDRAW_H
#define RR_GAME_GFX_FONTDRAW_H

#include "types.h"

extern u8 FontAtlasTileCoords[];
/* Append sprite packets to an arena and return its next cursor. */
void *AddTextSprites(u32 *ordering, void *cursor, s32 x, s32 y,
                     const char *text, s32 clut, s32 spacing, s32 color);
void DrawAnimatedText();
void DrawBigFontString(s32 a0, s32 a1, u8 *a2);
void DrawAnimatedGlyph(s32 x, s32 y, s32 glyph, s32 clut, s16 *animation);


#endif /* RR_GAME_GFX_FONTDRAW_H */

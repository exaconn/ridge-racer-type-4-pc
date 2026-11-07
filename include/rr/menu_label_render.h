#ifndef RR_MENU_LABEL_RENDER_H
#define RR_MENU_LABEL_RENDER_H
#include "rr/badge_rows.h"
void FormatDecimalDigits(char *output,s32 value,s32 digitsMinusOne);
char *FormatCountOfTotal(s32 first,s32 second,s32 digitsMinusOne);
void *AddTextSprites(u32 *ordering,void *cursor,s32 x,s32 y,const char *text,s32 clut,s32 spacing,s32 color);
void *func_8001FF84(u32 *ordering,void *cursor,s32 x,s32 y,const RrUiUvRect *glyph,u16 clut,s32 color);
void *AddDrawModePrim(u32 *ordering,void *cursor,u16 mode);
#endif

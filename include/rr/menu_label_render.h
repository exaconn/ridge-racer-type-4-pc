#ifndef RR_MENU_LABEL_RENDER_H
#define RR_MENU_LABEL_RENDER_H
#include "game/gfx/fontdraw.h"
#include "game/menu/mnlist.h"
#include "rr/badge_rows.h"
void FormatDecimalDigits(char *output,s32 value,s32 digitsMinusOne);
void *AddSpritePrim(u32 *ordering,void *cursor,s32 x,s32 y,const UiUvRect *glyph,u16 clut,s32 color);
void *AddDrawModePrim(u32 *ordering, GpuDrawModePacket *packet, u16 mode);
#endif

#ifndef RR_GAME_UI_BADGEPRIM_H
#define RR_GAME_UI_BADGEPRIM_H
#include "rr/ui_bar.h"
/* Packet storage and the returned cursor retain the host pointer width. */
void *AddSpritePrim(u32 *ordering, void *cursor, s32 x, s32 y,
    UiUvRect *region, u16 clut, s32 flags);
#endif

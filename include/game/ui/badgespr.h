#ifndef RR_GAME_UI_BADGESPR_H
#define RR_GAME_UI_BADGESPR_H

#include "types.h"

void *AddSpriteWithDrawMode(u32 *ordering, void *cursor,
    int x, int y, int width, int height, int u, int v,
    unsigned short page, unsigned short clut, s32 transparency, s32 brightness);

#endif /* RR_GAME_UI_BADGESPR_H */

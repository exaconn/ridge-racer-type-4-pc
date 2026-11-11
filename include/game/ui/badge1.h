#ifndef RR_GAME_UI_BADGE1_H
#define RR_GAME_UI_BADGE1_H
#include "rr/gpu.h"
void *AddDrawAreaPrim(u32 *ordering, void *cursor, s32 x, s32 y,
    s32 width, s32 height);
void *AddDrawModePrim(u32 *ordering, GpuDrawModePacket *packet, u16 page);
void *AddTilePrim(u32 *ordering, void *cursor, s32 x, s32 y, s32 width,
    s32 height, s32 red, s32 green, s32 blue);
void *AddTileGrid2x2(u32 *ordering, void *cursor, int x, int y, int width,
    int height, int red, int green, int blue);

#endif

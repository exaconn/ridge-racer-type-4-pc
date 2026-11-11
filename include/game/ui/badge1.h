#ifndef RR_GAME_UI_BADGE1_H
#define RR_GAME_UI_BADGE1_H
#include "rr/gpu.h"
void *AddDrawAreaPrim(GpuDmaTag *ordering, void *cursor, s32 x, s32 y,
    s32 width, s32 height);
void *AddDrawModePrim(GpuDmaTag *ordering, GpuDrawModePacket *packet, u16 page);
void *AddTilePrim(GpuDmaTag *ordering, void *cursor, s32 x, s32 y, s32 width,
    s32 height, s32 red, s32 green, s32 blue);
void *AddTileGrid2x2(GpuDmaTag *ordering, void *cursor, int x, int y, int width,
    int height, int red, int green, int blue);

#endif

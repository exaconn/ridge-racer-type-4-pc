#ifndef RR_GAME_UI_BADGE1_H
#define RR_GAME_UI_BADGE1_H
#include "rr/gpu.h"
void *AddDrawAreaPrim(u32 *ordering, void *cursor, s32 x, s32 y,
    s32 width, s32 height);
void *AddDrawModePrim(u32 *ordering, GpuDrawModePacket *packet, u16 page);
void *AddTilePrim(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void *AddTileGrid2x2(u32 *a0, void *a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);

#endif

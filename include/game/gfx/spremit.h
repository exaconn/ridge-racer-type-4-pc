#ifndef RR_GAME_GFX_SPREMIT_H
#define RR_GAME_GFX_SPREMIT_H
#include "rr/gpu.h"
/* Emit one raw-texture sprite into an aligned packet stream and return its
 * next cursor. Coordinates, dimensions, UVs and CLUT keep their low wire bits.
 * The ordering pointer is a writable GPU link; native cursors retain full width. */
SpritePacket *GameQueueSprite(u32 *ordering, SpritePacket *packet, s32 x, s32 y,
    s32 width, s32 height, s32 u, s32 v, s32 clut);
#endif

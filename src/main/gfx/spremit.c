#include "game/gfx/spremit.h"
#include "rr/gpu.h"
#include "psyq/libgpu/primattr.h"
#include "rr/animated_glyph.h"

SpritePacket *GameQueueSprite(GpuDmaTag *ordering, SpritePacket *packet, s32 x, s32 y,
    s32 width, s32 height, s32 u, s32 v, s32 clut)
{
    SetSprt(packet);
    SetShadeTex(packet, 1);
    packet->x = x;
    packet->y = y;
    packet->w = width;
    packet->h = height;
    packet->u = u;
    packet->v = v;
    packet->clut = clut;
    AddPrim(ordering, packet++);
    return packet;
}

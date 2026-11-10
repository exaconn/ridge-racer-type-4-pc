#ifndef RR_SPRITE_EMIT_H
#define RR_SPRITE_EMIT_H
#include "rr/ui_bar.h"
#include "game/ui/badgeprim.h"
#include "game/ui/badge1.h"
/* Coordinates remain wide until serialized to the packet's wire halfwords. */
SpritePacketSignedClut *GameQueueSprite(u32 *ordering,
    SpritePacketSignedClut *packet,s32 x,s32 y,s32 width,s32 height,
    s32 u,s32 v,s32 clut);
PolyF3Packet *EmitPolyF3(u32 *ordering,PolyF3Packet *packet,
    s32 xy0,s32 xy1,s32 xy2,const u8 *rgb,s32 flags);
#endif

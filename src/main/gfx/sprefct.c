#include "game/gfx/sprefct.h"
#include "rr/gpu_address.h"
#include "rr/gpu_line.h"
#include "types.h"
#include "rr/frame_storage_types.h"
#include "rr/sprite_effect.h"
#include "rr/sprite_emit.h"
#include "psyq/libgpu/primatt2.h"
#include "psyq/libgpu/primatt3.h"
#include "psyq/libgpu/primatt6.h"
#include "psyq/libgpu/primattr.h"
#include "psyq/libgpu/settrans.h"
#include "rr/animated_glyph.h"
#include "psyq/libc/rand.h"




u8 *AddJitterSprites(u32 *ordering, u8 *cursor, SpriteEffectSource *descriptor, s32 semi, s32 strength, s32 count) {
    s32 i;
    s32 v;
    s32 y;
    u32 page;

    for (i = 0; i < count; i++) {
        SetSprt(cursor);
        v = 0;
        if (count != 1) {
            v = 3 - (PsyqRand() & 7);
        }
        if (i == 0) {
            y = descriptor->y + (v >> 1);
        } else if (i == 1) {
            y = descriptor->y + v * 3;
        } else if (i == 2) {
            y = descriptor->y + v;
        }
        ((SpritePacket *)cursor)->x = descriptor->x;
        ((SpritePacket *)cursor)->y = y;
        ((SpritePacket *)cursor)->w = descriptor->uv.w;
        ((SpritePacket *)cursor)->h = descriptor->uv.h;
        ((SpritePacket *)cursor)->u = descriptor->uv.u;
        ((SpritePacket *)cursor)->v = descriptor->uv.v;
        SetSemiTrans(cursor, *(s32 *)&semi);
        if (count == 1) {
            v = strength;
        } else if (i == 1) {
            v = (s32)((u32)strength * (PsyqRand() & 15)) / 24;
        } else {
            v = (s32)((u32)strength * ((PsyqRand() & 31) + 8)) / 48;
        }
        ((SpritePacket *)cursor)->r = v;
        ((SpritePacket *)cursor)->g = v;
        ((SpritePacket *)cursor)->b = v;
        ((SpritePacket *)cursor)->clut = descriptor->clut;
        {
            u8 *p = cursor;
            cursor += sizeof(SpritePacket);
            AddPrim(ordering, (u32 *)p);
        }
        if (i > 0) {
            page = (u8)descriptor->page | 0x60;
        } else {
            page = descriptor->page;
        }
        cursor = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], (GpuDrawModePacket *)cursor, page);
    }
    return cursor;
}


GpuPolyFT3Packet *EmitPolyFT3(u32 *ordering, GpuPolyFT3Packet *packet,
    const s32 *x, const s32 *y, const s32 *u, const s32 *v,
    u32 pageWord, u32 clutWord, u32 modeWord, u32 shadeWord)
{
    u16 tpage = pageWord, clut = clutWord, mode = modeWord, shade = shadeWord;
    SetPolyFT3(packet);
    packet->x0 = x[0];
    packet->y0 = y[0];
    packet->x1 = x[1];
    packet->y1 = y[1];
    packet->x2 = x[2];
    packet->y2 = y[2];
    packet->u0 = u[0];
    packet->v0 = v[0];
    packet->u1 = u[1];
    packet->v1 = v[1];
    packet->u2 = u[2];
    packet->v2 = v[2];
    SetSemiTrans(packet, mode);
    if (shade != 0x80) {
        SetShadeTex(packet, 0);
        packet->r = shade;
        packet->g = shade;
        packet->b = shade;
    } else {
        SetShadeTex(packet, 1);
    }
    packet->tpage = tpage;
    packet->clut = clut;
    AddPrim(ordering, (u32 *)packet++);
    return packet;
}


void *AddTileSemiTransPrim(u32 *ordering, void *cursor, s32 x, s32 y,
                           s32 width, s32 height, s32 red, s32 green, s32 blue) {
    GpuTilePacket *tile = cursor;
    tile->tag.bytes.length = 3;
    tile->x = x;
    tile->w = width;
    tile->code = 0x62;
    tile->y = y;
    tile->h = height;
    tile->r = red;
    tile->g = green;
    tile->b = blue;
    tile->tag.bits.address = ((GpuDmaTag *)ordering)->bits.address;
    ((GpuDmaTag *)ordering)->bits.address = GpuDmaEncodeAddress(tile);
    return tile + 1;
}

GpuLineF2Packet *GameQueueTileTrans(u32 *ordering, GpuLineF2Packet *line, s32 x0, s32 y0, s32 x1, s32 y1, s32 red, s32 green, s32 blue) {
    SetLineF2(line);
    SetShadeTex(line, 1);
    line->x0 = x0;
    line->y0 = y0;
    line->x1 = x1;
    line->y1 = y1;
    line->r = red;
    line->g = green;
    line->b = blue;
    AddPrim(ordering, (u32 *)line++);
    return line;
}


GpuLineF2Packet *AddLineF2SemiTrans(u32 *ordering, GpuLineF2Packet *line, s32 x0, s32 y0, s32 x1, s32 y1, s32 red, s32 green, s32 blue) {
    SetLineF2(line);
    SetShadeTex(line, 1);
    line->x0 = x0;
    line->y0 = y0;
    line->code |= 2;
    line->x1 = x1;
    line->y1 = y1;
    line->r = red;
    line->g = green;
    line->b = blue;
    AddPrim(ordering, (u32 *)line++);
    return line;
}


PolyF3Packet *EmitPolyF3(u32 *ordering,PolyF3Packet *packet,s32 xy0,s32 xy1,s32 xy2,const u8 *rgb,s32 flags){
 SetPolyF3(packet);
 SetSemiTrans(packet,flags);
 packet->xy0=xy0;
 packet->xy1=xy1;
 packet->xy2=xy2;
 packet->r=rgb[0];
 packet->g=rgb[1];
 packet->b=rgb[2];
 AddPrim(ordering,(u32*)packet++);
 return packet;
}

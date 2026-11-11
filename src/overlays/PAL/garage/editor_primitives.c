#include "rr/overlay662_draw_area.h"
#include "rr/overlay662_garage.h"
#include "rr/frame_loop.h"
#include "psyq/libgpu/gpudisp.h"
#include "rr/gpu_address.h"
#include "rr/overlay662_primitives.h"
#include "rr/overlay662_line.h"
#include "rr/overlay662_gradient.h"
#include "rr/overlay662_sprites.h"
#include "rr/animated_sprite_grid.h"

u8 *ovl662_emit_draw_area(u32 *ordering, u32 *packet, u32 x, u32 y, u32 width, u32 height) {
    u16 w = width, h = height;
    GpuRectangleFields rect;
    s16 py;
    rect.x = ovl662_editor_workspace.halfC8 + x;
    py = ovl662_editor_workspace.halfCA + y;
    if (DrawBuffers[0].environment.display5C.display.y == DrawBuffers[1].environment.display5C.display.y) {
        rect.y = py;
    } else {
        rect.y = py + ((u32)DrawBufIndex * 256U);
    }
    rect.w = w;
    rect.h = h;
    SetDrawArea(packet, &rect);
    *packet = (*packet & 0xFF000000) | (*ordering & 0xFFFFFF);
    *ordering = (*ordering & 0xFF000000) | (GpuDmaEncodeAddress(packet) & 0xFFFFFF);
    return (u8 *)(packet + 3);
}

GpuDrawModePacket *ovl662_emit_draw_mode(u32 *ordering,GpuDrawModePacket *packet,u32 x,u32 y)
{
    s32 page;
    page = ((s32)((y & 0x100) << 16) >> 20) | ((x & 0x3FF) >> 6) | ((s16)(y & 0x200) * 4);
    SetDrawMode(packet,0,1,page,0);
    packet->tag.word=(packet->tag.word & 0xFF000000) | (*ordering & 0xFFFFFF);
    *ordering=(*ordering & 0xFF000000) | ((unsigned long)packet & 0xFFFFFF);
    return packet+1;
}

GpuTilePacket *ovl662_emit_hline_tile(u32 *ordering, GpuTilePacket *packet, u32 x0, u32 y, u32 x1, u32 r, u32 g, u32 b)
{
    u16 baseX = ovl662_editor_workspace.halfC8;
    u16 baseY = ovl662_editor_workspace.halfCA;
    u16 px0;
    u16 px1 = x1;
    u32 mask = 0xFFFFFF;
    u32 t1;

    packet->tag.bytes.length = 3;
    packet->code = 0x40;
    px0 = baseX + x0;
    baseY = baseY + y;
    px1 = px1 + baseX;
    packet->x = px0;
    packet->y = baseY;
    packet->w = px1;
    packet->h = baseY;
    packet->r = r;
    packet->g = g;
    packet->b = b;
    t1 = x0 | 1;
    t1 &= 1;
    t1 |= mask;
    packet->tag.word = (packet->tag.word & 0xFF000000) | (*ordering & (t1 & mask));
    *ordering = (*ordering & 0xFF000000) | ((unsigned long)packet & mask);
    packet++;

    packet->tag.bytes.length = 3;
    packet->code = 0x40;
    baseY = baseY + 1;
    packet->x = px0;
    packet->y = baseY;
    packet->w = px1;
    packet->h = baseY;
    packet->r = r;
    packet->g = g;
    packet->b = b;
    packet->tag.word = (packet->tag.word & 0xFF000000) | (*ordering & mask);
    *ordering = (*ordering & 0xFF000000) | ((unsigned long)packet & mask);
    return packet + 1;
}

GpuLineF2Packet *ovl662_emit_vline_prim(u32 *ordering, GpuLineF2Packet *packet, u32 x, u32 y0, u32 y1, u32 r, u32 g, u32 b)
{
    u16 baseX = ovl662_editor_workspace.halfC8;
    u16 rawY = ovl662_editor_workspace.halfCA;
    u32 baseY = rawY;
    u32 mask = 0xFFFFFF;
    u32 t1;
    packet->tag.bytes.length=3;
    packet->code=0x40;
    packet->x0=baseX+x;
    packet->x1=baseX+x;
    packet->y0=baseY+y0;
    packet->y1=y1+baseY;
    packet->r=r;packet->g=g;packet->b=b;
    t1 = x | 1;
    t1 &= 1;
    t1 |= mask;
    packet->tag.word=(packet->tag.word & 0xFF000000) | (*ordering & (t1 & mask));
    *ordering=(*ordering & 0xFF000000) | ((unsigned long)packet & mask);
    return packet+1;
}

GpuLineF4Packet *ovl662_emit_wire_box(u32 *ordering, GpuLineF4Packet *packet, u32 x, u32 y, u32 w, u32 h, u32 r, u32 g, u32 b)
{
    u16 baseX = ovl662_editor_workspace.halfC8;
    u16 baseY = ovl662_editor_workspace.halfCA;
    u16 xEnd;
    u16 yEnd;

    packet->tag.bytes.length = 6;
    packet->code = 0x4C;
    packet->pad = 0x55555555;
    packet->code &= ~2;
    baseX += x;
    baseY += y;
    xEnd = baseX + w;
    yEnd = baseY + h;
    packet->x0 = baseX;
    packet->y0 = baseY;
    packet->x1 = xEnd - 1;
    packet->y1 = baseY;
    packet->x2 = xEnd - 1;
    packet->y2 = yEnd - 1;
    packet->x3 = baseX;
    packet->y3 = yEnd - 1;
    packet->r = r;
    packet->g = g;
    packet->b = b;
    packet->tag.word = (packet->tag.word & 0xFF000000) | (*ordering & 0xFFFFFF);
    *ordering = (*ordering & 0xFF000000) | ((unsigned long)packet & 0xFFFFFF);
    packet++;

    packet->tag.bytes.length = 6;
    packet->code = 0x4C;
    packet->pad = 0x55555555;
    packet->code &= ~2;
    xEnd -= 2;
    baseY += 1;
    yEnd -= 2;
    packet->x0 = xEnd;
    packet->y0 = baseY;
    packet->x1 = baseX;
    packet->y1 = baseY;
    packet->x2 = baseX;
    packet->y2 = yEnd;
    packet->x3 = xEnd;
    packet->y3 = yEnd;
    packet->r = r;
    packet->g = g;
    packet->b = b;
    packet->tag.word = (packet->tag.word & 0xFF000000) | (*ordering & 0xFFFFFF);
    *ordering = (*ordering & 0xFF000000) | ((unsigned long)packet & 0xFFFFFF);
    return packet + 1;
}

GpuTilePacket *ovl662_emit_filled_tile(u32 *ordering, GpuTilePacket *packet, u32 x, u32 y, u32 w, u32 h, u32 r, u32 g, u32 b)
{
    u16 baseX=ovl662_editor_workspace.halfC8;
    u16 baseY=ovl662_editor_workspace.halfCA;
    packet->tag.bytes.length=3;
    packet->code=0x60;
    packet->x=baseX+x;
    packet->y=baseY+y;
    packet->w=w;
    packet->h=h;
    packet->r=r;packet->g=g;packet->b=b;
    packet->tag.word=(packet->tag.word & 0xFF000000) | (*ordering & 0xFFFFFF);
    *ordering=(*ordering & 0xFF000000) | ((unsigned long)packet & 0xFFFFFF);
    return packet+1;
}

PolyG4Storage *ovl662_emit_gradient_quad(u32 *ordering, PolyG4Storage *packet, u32 x, u32 y, u32 w, u32 h, u32 r0, u32 g0, u32 b0, u32 r1, u32 g1, u32 b1) {
    u16 baseX = ovl662_editor_workspace.halfC8;
    u16 baseY = ovl662_editor_workspace.halfCA;
    u16 x1;
    packet->packet.len = 8;
    packet->packet.code = 0x38;
    baseX += x;
    baseY += y;
    x1 = baseX + w - 1;
    packet->packet.x0 = baseX;
    packet->packet.y0 = baseY;
    packet->packet.x1 = x1;
    packet->packet.y1 = baseY;
    baseY = baseY + h - 1;
    packet->packet.x2 = baseX;
    packet->packet.y2 = baseY;
    packet->packet.x3 = x1;
    packet->packet.y3 = baseY;
    packet->packet.r0 = r0;
    packet->packet.g0 = g0;
    packet->packet.b0 = b0;
    packet->packet.r1 = r1;
    packet->packet.g1 = g1;
    packet->packet.b1 = b1;
    packet->packet.r2 = r0;
    packet->packet.g2 = g0;
    packet->packet.b2 = b0;
    packet->packet.r3 = r1;
    packet->packet.g3 = g1;
    packet->packet.b3 = b1;
    packet->words[0] = (packet->words[0] & 0xFF000000) | (*ordering & 0xFFFFFF);
    *ordering = (*ordering & 0xFF000000) | ((unsigned long)packet & 0xFFFFFF);
    return packet + 1;
}

GpuPolyFT4Packet *ovl662_emit_textured_quad(u32 *ordering, GpuPolyFT4Packet *packet, u32 x, u32 y, u32 w, u32 h, u32 u, u32 v, u32 uw, u32 vh, u32 clut, u32 tpage)
{
    u16 baseX = ovl662_editor_workspace.halfC8;
    u16 baseY = ovl662_editor_workspace.halfCA;
    u16 x1;
    u16 u1;
    u16 v1;

    packet->tag.bytes.length = 9;
    packet->code = 0x2D;
    u1 = u + uw - 1;
    v1 = v + vh - 1;
    baseX += x;
    baseY += y;
    x1 = baseX + w - 1;
    packet->x0 = baseX;
    packet->y0 = baseY;
    packet->x1 = x1;
    packet->y1 = baseY;
    baseY = baseY + h - 1;
    packet->x2 = baseX;
    packet->y2 = baseY;
    packet->x3 = x1;
    packet->y3 = baseY;
    packet->u0 = u;
    packet->v0 = v;
    packet->u1 = u1;
    packet->v1 = v;
    packet->u2 = u;
    packet->v2 = v1;
    packet->u3 = u1;
    packet->v3 = v1;
    packet->clut = clut;
    packet->tpage = tpage;
    packet->tag.word = (packet->tag.word & 0xFF000000) | (*ordering & 0xFFFFFF);
    *ordering = (*ordering & 0xFF000000) | ((unsigned long)packet & 0xFFFFFF);
    return packet + 1;
}

u8 *ovl662_emit_indexed_sprite(s32 index,u8 *cursor,s32 x,s32 y)
{
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
    u32 *ordering=RR_GPU_SCRATCH->order+128;
    SpritePacket *packet=(SpritePacket *)cursor;
    GarageSpriteRecord *record;
    s32 i;
    if (index<0) {
        for (i=0;i<72;i++) {
            record=&ovl662_sprite_records[i];
            ovl662_sprite_lookup[record->key0C]=record;
        }
        return 0;
    }
    record=ovl662_sprite_lookup[index];
    packet->len=4;
    packet->code=0x64;
    if (record->flags10 & 1) packet->code = packet->code | 2;
    else packet->code = packet->code & ~2;
    packet->x=x;packet->y=y;
    packet->code |=1;
    packet->w=record->width04;
    packet->h=record->height06;
    packet->u=record->u08;packet->v=record->v09;
    packet->clut=record->clut0A;
    packet->addr=ordering[record->depth00];
    ordering[record->depth00]=(ordering[record->depth00]&0xFF000000) | ((unsigned long)packet&0xFFFFFF);
    return (u8 *)(packet+1);
}

void ovl662_init_editor_draw_env(void)
{
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
    GpuScratchPointers *scratch=RR_GPU_SCRATCH;
    u32 *order=scratch->order;
    scratch->cursor=ovl662_emit_draw_area(order+146, (u32 *)scratch->cursor,12,61,296,352);
    scratch->cursor=(u8 *)ovl662_emit_draw_mode(order+133,(GpuDrawModePacket *)scratch->cursor,704,0);
    scratch->cursor=(u8 *)ovl662_emit_draw_mode(order+147,(GpuDrawModePacket *)scratch->cursor,704,0);
}

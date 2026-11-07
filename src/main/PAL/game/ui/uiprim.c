#include "scratchpad.h"
#include "rr/gouraud_quad.h"
#include "rr/gpu_line.h"
#include "rr/gpu_scratch_types.h"
#include "rr/frame_environment.h"
#include "rr/ui_primitives.h"
#include "rr/fade_sprite.h"

enum { ORDERING_TABLE_WORD = RR_FIELD_OFFSET(RrFrameOrderingPrefix, ordering70) / sizeof(u32) };

void AddVerticalGradientRect(RrGpuRectangleFields *rect, RrGpuColorCommand *top, RrGpuColorCommand *bottom, s32 depth)
{
    RrPolyG4Storage *primitive = (RrPolyG4Storage *)((RrGpuScratchPointers *)SCRATCHPAD)->cursor;
    u32 *ordering;
    primitive->packet.len = 8;
    primitive->packet.code = 0x3A;
    primitive->packet.x0 = rect->x;
    primitive->packet.y0 = rect->y;
    primitive->packet.x1 = rect->x + rect->w;
    primitive->packet.y1 = rect->y;
    primitive->packet.x2 = rect->x;
    primitive->packet.y2 = rect->y + rect->h;
    primitive->packet.x3 = rect->x + rect->w;
    primitive->packet.y3 = rect->y + rect->h;
    primitive->packet.r0 = top->r;
    primitive->packet.g0 = top->g;
    primitive->packet.b0 = top->b;
    primitive->packet.r1 = top->r;
    primitive->packet.g1 = top->g;
    primitive->packet.b1 = top->b;
    primitive->packet.r2 = bottom->r;
    primitive->packet.g2 = bottom->g;
    primitive->packet.b2 = bottom->b;
    primitive->packet.r3 = bottom->r;
    primitive->packet.g3 = bottom->g;
    primitive->packet.b3 = bottom->b;
    ordering = (u32 *)(depth * sizeof(u32) + (unsigned long)CurrentDrawBuf);
    primitive->words[0] = (primitive->words[0] & 0xFF000000U) | (ordering[ORDERING_TABLE_WORD] & 0xFFFFFFU);
    ordering[ORDERING_TABLE_WORD] = (ordering[ORDERING_TABLE_WORD] & 0xFF000000U) | ((unsigned long)primitive & 0xFFFFFFU);
    primitive++;
    ((RrGpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)primitive;
}

void AddGouraudRect(RrGpuRectangleFields *rect, RrGpuColorCommand *color0, RrGpuColorCommand *color1,
    RrGpuColorCommand *color2, RrGpuColorCommand *color3, s32 depth)
{
    RrPolyG4Storage *primitive = (RrPolyG4Storage *)((RrGpuScratchPointers *)SCRATCHPAD)->cursor;
    u32 *ordering;
    primitive->packet.len = 8;
    primitive->packet.code = 0x3A;
    primitive->packet.x0 = rect->x;
    primitive->packet.y0 = rect->y;
    primitive->packet.x1 = rect->x + rect->w;
    primitive->packet.y1 = rect->y;
    primitive->packet.x2 = rect->x;
    primitive->packet.y2 = rect->y + rect->h;
    primitive->packet.x3 = rect->x + rect->w;
    primitive->packet.y3 = rect->y + rect->h;
    primitive->packet.r0 = color0->r;
    primitive->packet.g0 = color0->g;
    primitive->packet.b0 = color0->b;
    primitive->packet.r1 = color1->r;
    primitive->packet.g1 = color1->g;
    primitive->packet.b1 = color1->b;
    primitive->packet.r2 = color2->r;
    primitive->packet.g2 = color2->g;
    primitive->packet.b2 = color2->b;
    primitive->packet.r3 = color3->r;
    primitive->packet.g3 = color3->g;
    primitive->packet.b3 = color3->b;
    ordering = (u32 *)(depth * sizeof(u32) + (unsigned long)CurrentDrawBuf);
    primitive->words[0] = (primitive->words[0] & 0xFF000000U) | (ordering[ORDERING_TABLE_WORD] & 0xFFFFFFU);
    ordering[ORDERING_TABLE_WORD] = (ordering[ORDERING_TABLE_WORD] & 0xFF000000U) | ((unsigned long)primitive & 0xFFFFFFU);
    primitive++;
    ((RrGpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)primitive;
}

void AddLineF2Prim(const s16 coordinates[4], const u8 rgb[3], s32 blend, s32 depth)
{
    RrGpuLineF2Packet *primitive = (RrGpuLineF2Packet *)((RrGpuScratchPointers *)SCRATCHPAD)->cursor;
    u32 *ordering;
    u32 tag;
    primitive->tag.bytes.length = 3;
    primitive->code = 0x40;
    primitive->code = blend ? 0x42 : 0x40;
    tag = primitive->tag.word;
    primitive->r = rgb[0];
    primitive->g = rgb[1];
    primitive->b = rgb[2];
    primitive->x0 = coordinates[0];
    primitive->y0 = coordinates[1];
    primitive->x1 = coordinates[2];
    primitive->y1 = coordinates[3];
    ordering = (u32 *)(depth * sizeof(u32) + (unsigned long)CurrentDrawBuf);
    primitive->tag.word = (tag & 0xFF000000U) | (ordering[ORDERING_TABLE_WORD] & 0xFFFFFFU);
    ordering[ORDERING_TABLE_WORD] = (ordering[ORDERING_TABLE_WORD] & 0xFF000000U) | ((unsigned long)primitive & 0xFFFFFFU);
    primitive++;
    ((RrGpuScratchPointers *)SCRATCHPAD)->cursor = (u8 *)primitive;
}

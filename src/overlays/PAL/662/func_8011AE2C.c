#include "rr/overlay662_line.h"

GpuLineF2Packet *ovl662_emit_vline_prim(u32 *ordering, GpuLineF2Packet *packet, u32 x, u32 y0, u32 y1, u32 r, u32 g, u32 b)
{
    /* Compiler register metadata preserves incoming stack-argument order. */
    register u16 baseY asm("$10");register u32 lastY asm("$8")=y1;register u32 rValue asm("$11")=r;register u32 gValue asm("$12")=g;register u32 bValue asm("$13")=b;u16 baseX;
    baseX=ovl662_editor_workspace.halfC8;
    baseY=ovl662_editor_workspace.halfCA;
    packet->tag.bytes.length=3;
    packet->code=0x40;
    packet->x0=baseX+x;
    packet->x1=baseX+x;
    packet->y0=baseY+y0;
    packet->y1=lastY+baseY;
    packet->r=rValue;packet->g=gValue;packet->b=bValue;
    packet->tag.word=(packet->tag.word & 0xFF000000) | (*ordering & 0xFFFFFF);
    *ordering=(*ordering & 0xFF000000) | ((unsigned long)packet & 0xFFFFFF);
    return packet+1;
}

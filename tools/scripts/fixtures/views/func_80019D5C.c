#include "rr/tiled_rectangle.h"

#define GS ((SPD *)0x1F800000)

#define W(o) (*(int *)(GS->cur + (o)))

#define LNK(n, t) \
    W(0) = *(int *)(GS->ot + (t)); \
    *(char *)(GS->cur + 3) = (n); \
    *(int *)(GS->ot + (t)) = (int)GS->cur; \
    *(char *)(GS->ot + (t) + 3) = 0

void EmitTiledRect(RrGpuRectangleFields *rectangle, s32 intensity, u32 *head) {
    register s32 value;
    register u32 command;
    register RrGpuRectangleFields *rect = rectangle;
    register u32 *ordering = head;
    register s32 height, y, width, x;
    register s32 tile_h, tile_w;
    /* ASM_BARRIER: keep the common tile limit in S2 across both loops. */
    register s32 limit asm("$18");
    /* ASM_BARRIER: retain the Y texture remainder in V1 until row packing. */
    register s32 v asm("$3");
    /* ASM_BARRIER: retain the X texture remainder in T3 for UV packing. */
    register s32 u asm("$11");
    register s32 shade = intensity;
    /* ASM_BARRIER: materialize the per-packet length in V0 after the tag store. */
    register s32 length asm("$2");
    register u32 packed;
    /* ASM_BARRIER: retain packed row fields in S0/A2/T9/T8 across the X loop. */
    register u32 top asm("$16"), bits_h asm("$6"), vpart asm("$25"), ypage asm("$24");
    /* ASM_BARRIER: stage E1000200 in A0 within each tile. */
    register u32 drawmode asm("$4");
    u32 high, middle;
    /* ASM_BARRIER: retain the signed page-field shift in V1. */
    register s32 xpage asm("$3");
    /* ASM_BARRIER: retain the output/biased-field cursors in T4/T0. */
    register u8 *packet asm("$12"), *fields asm("$8");
    RrGpuScratchPointers *scratch;
    if (shade >= 0) {
        value = shade < 256;

        if (value) goto clamp_low;
        value = 255;
    } else {
clamp_low:
        value = shade;
        if (shade < 0) value = 0;
    }
    shade = value;
    command = 0xE6000000U;
    /* ASM_BARRIER: stage E6000000 before forming the high color byte. */
    __asm__("" : : "r"(command));
    high = (u32)shade<<16;
    packet = *(u8 **)0x1F800000U;
    *(u32 *)(packet+4) = command;
    /* ASM_BARRIER: publish the first mask command before completing color packing. */
    __asm__("" : "+r"(shade) : : "memory");
    middle = (u32)shade<<8;
    middle |= 0x66000000U;
    high |= middle;
    shade = (s32)((u32)shade|high);
    *(u32 *)packet = *ordering;
    packet[3] = 1;
    *ordering = (u32)(unsigned long)packet;
    ((u8 *)ordering)[3] = 0;
    height = rect->h;
    y = rect->y;
    packet+=sizeof(RrGpuSingleCommandPacket);
    if (height > 0) {
        limit = 256;
        do {
            v = y&255;
            value = limit-v;
            /* ASM_BARRIER: retain the unbounded row extent before the minimum. */
            __asm__("" : : "r"(value));
            tile_h = value;
            if (tile_h >= height) tile_h = height;
            width = rect->w;
            x = rect->x;
            value = y-1;
            if (width > 0) {

                top = (u32)value<<16;
                bits_h = (u32)tile_h<<16;
                /* ASM_BARRIER: keep the texture remainder separate from packed V. */
                __asm__("" : : "r"(v));
                vpart = (u32)v<<8;
                value = y&256;
                /* ASM_BARRIER: retain the signed Y-page shift after masking. */
                __asm__("" : "+r"(value));
                ypage = (u32)(value>>4);
                fields = packet+3;
                do {
                    u = x&63;
                    value = limit-u;
                    /* ASM_BARRIER: retain the unbounded column extent before the minimum. */
                    __asm__("" : : "r"(value));
                    tile_w = value;
                    if (tile_w >= width) tile_w = width;
                    drawmode = 0xE1000200U;

                    value = x&65535;
                    xpage = x&1023;
                    x+=tile_w;
                    packed = top|(u32)value;
                    *(u32 *)(fields+9) = packed;
                    packed = bits_h|(u32)tile_w;
                    *(u32 *)(fields+17) = packed;
                    packed = vpart|(u32)u;
                    /* ASM_BARRIER: retain an arithmetic X-page shift. */
                    __asm__("" : "+r"(xpage));
                    xpage >>= 6;
                    xpage |= 256;
                    xpage = (s32)ypage|xpage;
                    *(u32 *)(fields+5) = (u32)shade;
                    *(u32 *)(fields+13) = packed;
                    *(u32 *)(fields+1) = (u32)xpage|drawmode;
                    packed = *ordering;
                    width-=tile_w;
                    *(u32 *)packet = packed;
                    length = 5;

                    fields[0] = length;
                    fields+=sizeof(RrTiledRectanglePacket);
                    *ordering = (u32)(unsigned long)packet;
                    packet+=sizeof(RrTiledRectanglePacket);
                    ((u8 *)ordering)[3] = 0;
                } while (width > 0);
            }
            height-=tile_h;
            y+=tile_h;
        } while (height > 0);
    }
    scratch = (RrGpuScratchPointers *)0x1F800000U;
    *(u32 *)(packet+4) = 0xE6000001U;
    *(u32 *)packet = scratch->order[703];
    packet[3] = 1;
    scratch->order[703] = (u32)(unsigned long)packet;
    ((u8 *)&scratch->order[703])[3] = 0;
    scratch->cursor = packet+sizeof(RrGpuSingleCommandPacket);
}

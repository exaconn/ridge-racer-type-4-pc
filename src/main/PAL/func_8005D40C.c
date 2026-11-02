#include "rr/textured_panel.h"

void func_8005D40C(s32 index) {
    RrPolyFT4Packet *p;
    RrGpuScratchPointers *scratch;
    u32 *order;
    /* ASM_BARRIER: retain the DMA address mask through the final link store. */
    register u32 low __asm__("$7")=0xFFFFFFU;
    u32 high=0xFF000000U;
    /* ASM_BARRIER: keep the left coordinate across both UV calculations. */
    register s32 left __asm__("$6");
    s32 half,vertical;
    s32 clut,length;
    /* ASM_BARRIER: right edge becomes the right texture coordinate after the division. */
    register s32 right __asm__("$3");
    scratch=(RrGpuScratchPointers*)0x1F800000U;
    length=9;
    left=32;
    /* ASM_BARRIER: stage the scratch page and packet constants before capture. */
    __asm__("" : "+r"(scratch), "+r"(length), "+r"(left));
    p=(RrPolyFT4Packet*)scratch->cursor;
    right=288;
    ((u8*)p)[3]=length;
    p->code=0x2D;
    p->y0=112;
    p->y1=112;
    p->y2=368;
    p->y3=368;
    /* ASM_BARRIER: finish the vertical stores before signed index division. */
    __asm__("" : "+r"(index) : : "memory");
    half=index/2;
    p->x1=right;
    p->x3=right;
    /* ASM_BARRIER: publish the right edge before deriving the remainder. */
    __asm__("" : "+r"(half) : : "memory");
    vertical=(u32)(index-half*2)<<7;
    right=255;
    p->v0=vertical;
    p->v1=vertical;
    vertical+=127;
    p->tpage=(((s32)(((u32)half<<6)+320)&1023)>>6)|16;
    clut=0x1BD6;
    p->v2=vertical;
    p->v3=vertical;
    p->x0=left;
    p->x2=left;
    p->u0=0;
    p->u1=right;
    p->u2=0;
    p->u3=right;
    p->clut=clut;
    *(u32*)p=(*(u32*)p&high)|(scratch->order[1]&low);
    order=scratch->order;
    order[1]=(order[1]&high)|((u32)(unsigned long)p&low);
    scratch->cursor=(u8*)(p+1);
}

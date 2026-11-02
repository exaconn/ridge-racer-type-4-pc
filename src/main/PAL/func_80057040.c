#include "rr/gpu.h"
extern u16 D_800111FE;
extern char *D_800ACC14;

void func_80057040(a0, a1, a2, a3) s32 a0; s32 a1; s32 a2; s32 a3; {
    RrSpritePacketSignedClut *p;
    s32 k;
    p = *(RrSpritePacketSignedClut **)0x1F800000;
    k = 0x66;
    p->len = 4;
    p->code = k;
    p->r = a3; p->g = a3; p->b = a3;
    p->y = a2;
    p->u = a0 * 8;
    p->v = 0;
    p->w = 8;
    p->h = 0x10;
    p->x = a1;
    p->clut = 0x7F38;
    p->addr = ((RrSpritePacketSignedClut *)(D_800ACC14 + 0x70))->addr;
    *(u32 *)(D_800ACC14 + 0x70) = (*(u32 *)(D_800ACC14 + 0x70) & 0xFF000000) | ((u32)p & 0xFFFFFF);
    p++;
    *(RrSpritePacketSignedClut **)0x1F800000 = p;
    p->len = 1;
    *((u32 *)p + 1) = (D_800111FE & 0x9FF) | 0xE1000200;
    p->addr = ((RrSpritePacketSignedClut *)(D_800ACC14 + 0x70))->addr;
    *(u32 *)(D_800ACC14 + 0x70) = (*(u32 *)(D_800ACC14 + 0x70) & 0xFF000000) | ((u32)p & 0xFFFFFF);
    *(RrSpritePacketSignedClut **)0x1F800000 = (RrSpritePacketSignedClut *)((u32 *)p + 2);
}

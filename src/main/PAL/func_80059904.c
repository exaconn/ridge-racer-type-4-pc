#include "rr/gpu.h"
extern char D_800A1090[], D_800A1158[];
extern char *D_800ACC14;


void func_80059904(t1, a1, a2)
s16 *t1;
s32 a1;
s32 a2;
{
    s32 t5, s4, t6 = 0, t7, t8, t3, v, w;
    u32 a;
    
    s32 m;
    s32 k;
    RrSpritePacketSignedClut *p;
    u8 *t0;
    t5 = *t1++;
    s4 = *t1++;
    if (a2 == 0) { t8 = 0x7F78; t7 = 0x3E; } else { t8 = 0x7E38; t7 = 0x25; }
    a = *(u16 *)t1;
    if (*t1 >= 0) {
        m = t7 & 0xFFFF;
        do {
            s32 e;
            p = *(RrSpritePacketSignedClut **)0x1F800000;
            t3 = t7;
            if (m == 0x3E) {
                if ((s16)a >= 0x400) t3 = 0x25;
            }
            a = *(u16 *)t1;
            e = *t1++;
            if (e >= 0x400) {
                v = (s32)((a - 0x400) << 16) >> 14;
                t0 = (u8 *)D_800A1158 + v;
            } else {
                v = e * 4;
                if (a2 == 0) t0 = (u8 *)D_800A1090 + v;
                else t0 = (u8 *)D_800A1158 + v;
            }
            k = 0x66;
            p->len = 4;
            p->code = k;
            p->r = a1; p->g = a1; p->b = a1;
            p->x = t5;
            p->y = s4 + t6;
            p->w = t0[2];
            p->h = t0[3];
            p->u = t0[0];
            p->v = t0[1];
            p->clut = t8;
            t6++;
            p->addr = ((RrSpritePacketSignedClut *)(D_800ACC14 + 0x70))->addr;
            *(u32 *)(D_800ACC14 + 0x70) = (*(u32 *)(D_800ACC14 + 0x70) & 0xFF000000) | ((u32)p & 0xFFFFFF);
            p++;
            *(RrSpritePacketSignedClut **)0x1F800000 = p;
            p->len = 1;
            *((u32 *)p + 1) = (t3 & 0x9FF) | 0xE1000200;
            p->addr = ((RrSpritePacketSignedClut *)(D_800ACC14 + 0x70))->addr;
            *(u32 *)(D_800ACC14 + 0x70) = (*(u32 *)(D_800ACC14 + 0x70) & 0xFF000000) | ((u32)p & 0xFFFFFF);
            *(RrSpritePacketSignedClut **)0x1F800000 = (RrSpritePacketSignedClut *)((u32 *)p + 2);
            w = *t1++;
            t5 += t0[2] + w;
            a = *(u16 *)t1;
        } while (*t1 >= 0);
    }
}

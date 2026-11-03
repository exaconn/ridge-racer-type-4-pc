#include "rr/gpu.h"

extern char D_800A1090[], D_800A1158[];
extern char *D_800ACC14;

void func_80059904();
void func_80059B1C(unsigned char *a0x, int a1, int a2, int a3);

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

void func_80059B1C(unsigned char *a0x, int a1, int a2, int a3) {
    register unsigned char *p asm("$10");
    int *g;
    unsigned hi;
    unsigned cmd;
    register unsigned char *q asm("$2");
    int t;
    unsigned t3;
    register unsigned lo asm("$5");
    register int v1 asm("$3");
    register char *d asm("$4");
    p = *(unsigned char **)0x1F800000;
    p[3] = 4;
    p[7] = 0x64;
    if (a3 < 0xFF) {
        p[7] = 0x66;
    } else {
        p[7] = 0x65;
    }
    v1 = a1;
    lo = 0xFFFFFF;
    cmd = 0xE100023E;
    p[4] = a3;
    p[5] = a3;
    p[6] = a3;
    *(short *)(p + 8) = v1;
    *(short *)(p + 0xA) = a2;
    *(short *)(p + 0x10) = a0x[2];
    *(short *)(p + 0x12) = a0x[3];
    p[0xC] = a0x[0];
    t = a0x[1];
    *(short *)(p + 0xE) = 0x7F78;
    p[0xD] = t;
    hi = 0xFF000000;
    g = (int *)0x1F800000;
    d = D_800ACC14;
    *(unsigned int *)p = (*(unsigned int *)p & hi) | (*(unsigned int *)(d + 0x70) & lo);
    *(unsigned int *)(d + 0x70) = (*(unsigned int *)(d + 0x70) & hi) | ((unsigned int)p & lo);
    p += 0x14;
    *g = (int)p;
    p[3] = 1;
    d = D_800ACC14;
    t3 = *(unsigned int *)p;
    *(unsigned int *)(p + 4) = cmd;
    *(unsigned int *)p = (t3 & hi) | (*(unsigned int *)(d + 0x70) & lo);
    *(unsigned int *)(d + 0x70) = (*(unsigned int *)(d + 0x70) & hi) | ((unsigned int)p & lo);
    q = p + 8;
    *g = (int)q;
}

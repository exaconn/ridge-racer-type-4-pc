#include "types.h"
typedef struct { s16 x, y, z, pad; } w3t1_SV;
typedef struct {
    char pad0[8];
    u16 sxy[6];
    char pad1[0x24 - 0x14];
    w3t1_SV v[3];
    char pad2[0x44 - 0x3C];
    u8 c0[3];
    u8 pad3;
    u8 c1[3];
} w3t1_G;

extern s32 func_80090424();
extern s32 func_800904F4();
extern s32 *D_800ACC14;

u8 *func_80044BA0(s32 a0) {
    w3t1_G *g = (w3t1_G *)0x1F80007C;
    u8 *p = *(u8 **)0x1F800000;
    s32 i;
    s32 t;
    g->c0[0] = 0x50;
    g->c0[1] = 0x50;
    g->c0[2] = 0x50;
    g->c1[0] = 0;
    g->c1[1] = 0;
    g->c1[2] = 0;
    g->v[0].x = 0;
    g->v[0].y = 0;
    g->v[0].z = 0;
    i = 0;
    do {
        s32 *ot;
        s32 w, o;
        s32 a = i << 8;
        g->v[1].x = (func_80090424(a) * 800) / 4096;
        g->v[1].y = 0;
        g->v[1].z = (func_800904F4(a) * 800) / 4096;
        i++;
        a = i << 8;
        g->v[2].x = (func_80090424(a) * 800) / 4096;
        g->v[2].y = 0;
        g->v[2].z = (func_800904F4(a) * 800) / 4096;
        __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)\n\tlwc2 $2, 0(%1)\n\tlwc2 $3, 4(%1)\n\tlwc2 $4, 0(%2)\n\tlwc2 $5, 4(%2)" : : "r"(&g->v[0]), "r"(&g->v[1]), "r"(&g->v[2]));
        __asm__ volatile ("nop\n\tnop\n\t.word 0x4A280030");
        __asm__ volatile ("swc2 $12, 0(%0)\n\tswc2 $13, 0(%1)\n\tswc2 $14, 0(%2)" : : "r"(&g->sxy[0]), "r"(&g->sxy[2]), "r"(&g->sxy[4]));
        p[3] = 6;
        p[7] = 0x33;
        *(u16 *)(p + 8) = g->sxy[0];
        *(u16 *)(p + 0xA) = g->sxy[1];
        *(u16 *)(p + 0x10) = g->sxy[2];
        *(u16 *)(p + 0x12) = g->sxy[3];
        *(u16 *)(p + 0x18) = g->sxy[4];
        *(u16 *)(p + 0x1A) = g->sxy[5];
        p[4] = g->c0[0];
        p[5] = g->c0[1];
        p[6] = g->c0[2];
        p[0xC] = g->c1[0];
        p[0xD] = g->c1[1];
        p[0xE] = g->c1[2];
        p[0x14] = g->c1[0];
        p[0x15] = g->c1[1];
        p[0x16] = g->c1[2];
        w = *(s32 *)p;
        ot = (s32 *)(a0 * 4 + (s32)D_800ACC14);
        o = ot[0x70 / 4];
        o = o & 0xFFFFFF;
        w = w & 0xFF000000;
        *(s32 *)p = w | o;
        ot[0x70 / 4] = (ot[0x70 / 4] & 0xFF000000) | ((s32)p & 0xFFFFFF);
        p += 0x1C;
    } while (i < 16);
    {
        s32 *ot;
        p[3] = 1;
        ot = (s32 *)(a0 * 4 + (s32)D_800ACC14);
        *(s32 *)(p + 4) = 0xE1000245;
        *(s32 *)p = (*(s32 *)p & 0xFF000000) | (ot[0x70 / 4] & 0xFFFFFF);
        ot[0x70 / 4] = (ot[0x70 / 4] & 0xFF000000) | ((s32)p & 0xFFFFFF);
    }
    {
        u8 *r;
        /* ASM_BARRIER: forces p+8 computed after final stores (return value copy) */
        __asm__ volatile ("" : "=r"(r) : "0"(p + 8));
        *(u8 **)0x1F800000 = r;
        return r;
    }
}

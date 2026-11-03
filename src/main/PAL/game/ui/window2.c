/* RODATA 0x800103e0 0x1c */
/* CC1_FLAGS: -fno-strength-reduce */
#include "types.h"
#include "rr/hud_rotating_quad.h"
#include "rr/trig.h"
#include "rr/hud_number.h"
#include "m2c_macros.h"

struct W3G0A { char *p0; char *p4; short s8, sA, sC, sE, s10, s12; char *p14; char *p18; short s1C, s1E, s20, s22, s24, s26; };

extern char D_800ADB60[];
extern char D_800D02D8[];
extern char D_800AE660[];
extern char D_800D0DD8[];
extern short D_80107178;
extern int func_80020920(), func_8001FCC8();
extern char D_8009EA90[];
extern char D_8009EACC[];
extern char *D_800ACC14;

void func_80021084(int a0);
void func_800211A4();
void func_80021350(s32 phase,s32 index);
void *func_80021568(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void *func_800215F4(s32 *arg0, void *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5);
void func_80021684(int a0, int a1);

/* legacy typing of shared globals in this part of the unit */
#define D_8010BF88 (*(struct W3G0A (*))&D_8010BF88)

void func_80021084(int a0) {
    switch (a0) {
    case 0: case 1: case 5: break;
    case 2: case 3: case 4: case 6:
        if (a0 == 4 && D_80107178 == 2) {
        } else if (a0 != 6) {
            goto w3g0b;
        }
    }
    D_8010BF88.p0 = D_800ADB60;
    D_8010BF88.p4 = D_800D02D8;
    D_8010BF88.s8 = 0x106;
    D_8010BF88.sA = 0xE6;
    D_8010BF88.sC = 0x122;
    D_8010BF88.sE = 0xDC;
    D_8010BF88.s10 = 0x112;
    D_8010BF88.s12 = 0xCE;
    return;
w3g0b:
    D_8010BF88.p0 = D_800AE660;
    D_8010BF88.p4 = D_800D0DD8;
    D_8010BF88.s8 = 0xFC;
    D_8010BF88.sA = 0x58;
    D_8010BF88.sC = 0x128;
    D_8010BF88.sE = 0x4A;
    D_8010BF88.s10 = 0xFC;
    D_8010BF88.s12 = 0x68;
    D_8010BF88.p14 = D_800AE660;
    D_8010BF88.p18 = D_800D0DD8;
    D_8010BF88.s1C = 0xFC;
    D_8010BF88.s1E = 0xD8;
    D_8010BF88.s20 = 0x122;
    D_8010BF88.s22 = 0xCA;
    D_8010BF88.s24 = 0xFC;
    D_8010BF88.s26 = 0xE8;
}

#undef D_8010BF88

void func_800211A4(a0)
int a0;
{
    unsigned *s0;
    unsigned *s2;
    char *s1;
    char *s4;
    int s3, s5, c3;
    unsigned m1;
    unsigned h1;
    unsigned m2;
    unsigned h2;

    s0 = *(unsigned **)0x1F800000;
    if (a0 >= 2) {
        s2 = (unsigned *)(D_800ACC14 + 0xB70);
        s5 = 4;
        if (a0 == 3) s5 = 2;
        s3 = 0;
        if (s5 != 0) {
            m1 = 0xFFFFFF;
            h1 = 0xFF000000;
            s1 = D_8009EACC;
            do {
                func_80020920(s0, s1);
                s1 += 0x14;
                s3++;
                *s0 = (*s0 & h1) | (*s2 & m1);
                *s2 = (*s2 & h1) | ((unsigned)s0 & m1);
                s0 += 5;
            } while (s3 < s5);
        }
    } else {
        s3 = 0;
        m2 = 0xFFFFFF;
        h2 = 0xFF000000;
        s4 = D_8009EA90;
        s2 = (unsigned *)(D_800ACC14 + 0x70);
        s1 = (char *)s0 + 0xA;
        do {
            func_80020920(s0, s4);
            __asm__ volatile("" : "=r"(c3) : "0"(3)); /* ASM_BARRIER: keep li 3 inside the loop (not hoisted) */
            if (a0 == c3) *(unsigned short *)s1 -= 0x80;
            s1 += 0x14;
            s4 += 0x14;
            s3++;
            *s0 = (*s0 & h2) | (*s2 & m2);
            *s2 = (*s2 & h2) | ((unsigned)s0 & m2);
            s0 += 5;
        } while (s3 < 3);
    }
    *(int *)0x1F800000 = func_8001FCC8(s2, s0, 5);
}

void func_80021350(s32 phase,s32 index) {
    RrHudQuadColor colors[4];
    s32 angle,sine;
    s32 cosine;
    s32 i;
    RrHudQuadColor *color;
    RrHudQuadCorner *corner;
    RrNumberTarget *target;
    u32 *head;
    register RrHudQuadEmission *packet __asm__("$8");
    u8 *output;
    register RrNumberTarget (*base)[] __asm__("$4");
    unsigned long row;
    register u32 x __asm__("$13");
    register u32 y __asm__("$4");
    u32 linkMask;
    colors[0].rgb.r=0xD0;
    colors[0].rgb.g=24;
    colors[0].rgb.b=0x30;
    colors[1].rgb.r=0x68;
    colors[1].rgb.g=12;
    colors[1].rgb.b=24;
    colors[2].rgb.r=0x8A;
    colors[2].rgb.g=0x12;
    colors[2].rgb.b=0x24;
    colors[3].rgb.r=0x34;
    colors[3].rgb.g=6;
    colors[3].rgb.b=12;
    angle=(s32)((u32)phase*0xA00U)/10000+0xE00;
    sine=func_80090424(angle);
    cosine=func_800904F4(angle);
    i=0;color=colors;corner=D_8009EE38;
    base=&D_8010BF88;
    row=(unsigned long)index*sizeof(RrNumberTarget);
    target=(RrNumberTarget *)((u8 *)base+row);
    packet=(RrHudQuadEmission *)RR_HUD_QUAD_SCRATCH->cursor;
    /* Full native-width addresses retain the original sum order. */
    head=*(u32 **)((unsigned long)D_800F4BF0*sizeof(u32 *)+row+(unsigned long)base);
    x=(u16)target->x;y=(u16)target->y;
    output=(u8 *)packet+8;
    packet->r=0xB0;packet->g=0xB0;packet->b=0;
    do {
        *(s16 *)output=(s16)(x+(u32)((s32)((u32)cosine*(u32)(s32)corner->x-(u32)sine*(u32)(s32)corner->y)/4096));
        *(s16 *)(output+2)=(s16)(y+(u32)((s32)((u32)sine*(u32)(s32)corner->x+(u32)cosine*(u32)(s32)corner->y)/4096));
        output+=4;
        /* Keep the initialized color destination after the XY writes. */
        *(RrHudQuadColor *)output=*color;
        color++;corner++;i++;output+=4;
    }while(i<4);
    linkMask=0xFFFFFF;
    packet->tag.bytes.length=8;packet->code=0x38;
    packet->tag.word=(packet->tag.word&0xFF000000)|(*head&linkMask);
    linkMask=(unsigned long)packet&linkMask;
    *head=(*head&0xFF000000)|linkMask;
    RR_HUD_QUAD_SCRATCH->cursor=(u8 *)packet+36;
}

void *func_80021568(u32 *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    M2C_FIELD(arg1, s8 *, 3) = 4;
    M2C_FIELD(arg1, s8 *, 7) = 0x65;
    M2C_FIELD(arg1, s16 *, 0x10) = 0xC;
    M2C_FIELD(arg1, s16 *, 0x12) = 0x10;
    M2C_FIELD(arg1, s16 *, 8) = arg2;
    M2C_FIELD(arg1, s16 *, 0xA) = arg3;
    M2C_FIELD(arg1, u8 *, 0xD) = 0xF0;
    M2C_FIELD(arg1, s8 *, 0xC) = (s8) (arg4 * 0xC);
    M2C_FIELD(arg1, s16 *, 0xE) = (s16) arg5;
    M2C_FIELD(arg1, s32 *, 0) = (s32) ((M2C_FIELD(arg1, s32 *, 0) & 0xFF000000) | (*arg0 & 0xFFFFFF));
    *arg0 = (*arg0 & 0xFF000000) | ((s32) arg1 & 0xFFFFFF);
    return arg1 + 0x14;
}

void *func_800215F4(s32 *arg0, void *arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5) {
    M2C_FIELD(arg1, s8 *, 3) = 4;
    M2C_FIELD(arg1, s8 *, 7) = 0x65;
    M2C_FIELD(arg1, s16 *, 0x10) = 0xC;
    M2C_FIELD(arg1, s16 *, 0x12) = 0x10;
    M2C_FIELD(arg1, s16 *, 8) = arg2;
    M2C_FIELD(arg1, s16 *, 0xA) = arg3;
    M2C_FIELD(arg1, u8 *, 0xD) = 0xF0;
    M2C_FIELD(arg1, s8 *, 0xC) = (s8) ((arg4 * 0xC) + 0x78);
    M2C_FIELD(arg1, s16 *, 0xE) = (s16) arg5;
    M2C_FIELD(arg1, s32 *, 0) = (s32) ((M2C_FIELD(arg1, s32 *, 0) & 0xFF000000) | (*arg0 & 0xFFFFFF));
    *arg0 = (*arg0 & 0xFF000000) | ((s32) arg1 & 0xFFFFFF);
    return arg1 + 0x14;
}

/* legacy typing of shared globals in this part of the unit */
#define D_8010BF88 (*(char (*)[])&D_8010BF88)

void func_80021684(int a0, int a1)
{
    char *b = D_8010BF88;
    int off = a1 * 0x14;
    char *t = b + off;
    short x = *(short *)(t + 8);
    short y = *(short *)(t + 10);
    int s1 = *(int *)(D_800F4BF0 * 4 + off + (int)b);
    int h, tn, on;
    h = a0 / 100;
    tn = (a0 / 10) - h * 10;
    on = a0 % 10;
    *(int *)0x1F800000 = func_8001FCC8(s1, func_80021568(s1, func_80021568(s1, func_80021568(s1, *(int *)0x1F800000, x, y, h, 0x7EFA), x + 8, y, tn, 0x7EFA), x + 0x10, y, on, 0x7EFA), 5);
}

#undef D_8010BF88

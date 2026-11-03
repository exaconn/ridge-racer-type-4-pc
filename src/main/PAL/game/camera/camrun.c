#include "types.h"

#define W(p,o) (*(int *)((char *)(p) + (o)))
#define Q(p,o) ((char *)*(volatile int *)((char *)(p) + (o)))

typedef struct {
    s32 v[4];
} Vec4;

typedef struct {
    s16 s0;
    s16 s2;
    s32 w4;
} Entry8;

typedef struct {
    Vec4 v0;
    Vec4 v1;
    char pad20[0x48 - 0x20];
    s32 f48;
    s32 f4c;
    s32 f50;
    s32 f54;
    s32 f58;
    s32 f5c;
    s32 f60;
    Entry8 *f64;
    s16 f68;
    s16 f6a;
    s32 f6c;
    s32 f70;
} CamQ;

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} PTag;

typedef struct {
    char pad[0xB68];
    PTag ot_b68;
    PTag ot_b6c;
    PTag ot_b70;
} DbEnv;

typedef struct {
    u32 addr : 24;
    u32 len : 8;
    u32 code;
    u32 p2;
} DrStp;

typedef struct {
    u32 addr : 24;
    u32 len : 8;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 w, h;
} TilePrim;

typedef struct {
    void *p00;
    s32 p04;
    Vec4 v08;
    Vec4 v18;
    char pad28[0x60 - 0x28];
    s32 f60;
} Scratch5B304;

extern struct {
    char pad[0x3D8];
    CamQ *q;
} D_800AC650;
extern s32 D_800AC5DC;
extern DbEnv *D_800ACC14;
extern s16 D_800F7488;
extern s16 D_800F7624;
extern void func_8002E828(s32, s32);
extern void func_8005AA2C(void);
extern void func_8005ABAC(s32);
extern void func_8005AC0C(s16 *);
extern void func_8005B9F4(s16 *);
extern void func_8005C024(void);
extern void func_8005D500(s32);
extern void func_8005D544(s16 *);
extern void func_8005D820(void);
extern void func_8005ECD4(s32);
extern void func_8005ED34(s16 *);
extern void func_8005EFBC(void);
extern void func_8006E1AC(void);
extern void func_8006EF8C(s32);
extern void func_800912B4(s32, s32);
extern void func_800912D4(s32);
extern void func_80093BFC(void *, s32);
extern int func_80096434();
extern char D_800A1AA0[];
extern char D_800A1B60[];

void func_8005B304(void);
void func_8005B8D8(int a0);

void func_8005B304(void) {
    Scratch5B304 *g = (Scratch5B304 *)0x1F800000;
    s32 a2 = 0;
    s16 *s0;

    if (D_800AC5DC == 0x80) {
        D_800AC650.q->f4c = 2;
    } else {
        D_800AC650.q->f4c = 1;
    }
    if (D_800AC650.q->f5c == 0) {
        s0 = &D_800F7624;
    } else {
        s0 = &D_800F7488;
    }
    if (*s0 != 0) {
        if (++D_800AC650.q->f6a >= D_800AC650.q->f64[D_800AC650.q->f68].w4) {
            D_800AC650.q->f68++;
            if (D_800AC650.q->f64[D_800AC650.q->f68].s0 < 0) {
                D_800AC650.q->f68 = 0;
            }
            D_800AC650.q->f6c = D_800AC650.q->f64[D_800AC650.q->f68].s0;
            D_800AC650.q->f70 = D_800AC650.q->f64[D_800AC650.q->f68].s2;
            a2 = 1;
            D_800AC650.q->f6a = 0;
        }
    }
    switch (D_800AC650.q->f6c) {
    case 0:
        if (a2 != 0) {
            func_8005B8D8(D_800AC650.q->f70);
        }
        func_8005B9F4(s0);
        break;
    case 1:
        if (a2 != 0) {
            func_8005D500(D_800AC650.q->f70);
        }
        func_8005D544(s0);
        break;
    case 2:
        if (a2 != 0) {
            func_8005ECD4(D_800AC650.q->f70);
        }
        func_8005ED34(s0);
        break;
    case 3:
        if (a2 != 0) {
            func_8005ABAC(D_800AC650.q->f70);
        }
        func_8005AC0C(s0);
        break;
    }
    if (D_800AC650.q->f54 != 0) {
        g->v08 = D_800AC650.q->v0;
        g->v18 = D_800AC650.q->v1;
        func_8006E1AC();
        func_800912D4(D_800AC650.q->f48);
        if (D_800AC650.q->f50 == 0) {
            func_8006EF8C(6);
            func_800912B4(0xA0, 0xE8);
            func_8002E828(2, 0x1F800028);
        }
        g->f60 = 5;
        g->p04 = (s32)&D_800ACC14->ot_b70;
        if (D_800AC650.q->f54 != 0) {
            switch (D_800AC650.q->f6c) {
            case 0:
                func_8005C024();
                break;
            case 1:
                func_8005D820();
                break;
            case 2:
                func_8005EFBC();
                break;
            case 3:
                func_8005AA2C();
                break;
            }
        }
        if (D_800AC650.q->f58 == 0) {
            ((DrStp *)g->p04)->addr = D_800ACC14->ot_b68.addr;
            D_800ACC14->ot_b68.addr = g->p04 + 0xAFC;
        } else {
            DrStp *s5 = ((Scratch5B304 *)0x1F800000)->p00;
            DrStp *s0_p;

            func_80093BFC(s5, 0);
            s5->code |= 2;
            s5->addr = D_800ACC14->ot_b6c.addr;
            D_800ACC14->ot_b6c.addr = (u32)s5;
            s5++;
            ((Scratch5B304 *)0x1F800000)->p00 = s5;
            ((DrStp *)g->p04)->addr = D_800ACC14->ot_b6c.addr;
            D_800ACC14->ot_b6c.addr = g->p04 + 0xAFC;
            s5 = ((Scratch5B304 *)0x1F800000)->p00;
            func_80093BFC(s5, 0);
            s5->addr = D_800ACC14->ot_b6c.addr;
            D_800ACC14->ot_b6c.addr = (u32)s5;
            s5++;
            ((Scratch5B304 *)0x1F800000)->p00 = s5;
            ((TilePrim *)s5)->len = 3;
            ((TilePrim *)s5)->code = 0x60;
            ((TilePrim *)s5)->r0 = 0;
            ((TilePrim *)s5)->g0 = 0;
            ((TilePrim *)s5)->b0 = 0;
            ((TilePrim *)s5)->x0 = 0;
            ((TilePrim *)s5)->y0 = 0;
            ((TilePrim *)s5)->w = 0x140;
            ((TilePrim *)s5)->h = 0xF0;
            s5->addr = D_800ACC14->ot_b6c.addr;
            D_800ACC14->ot_b6c.addr = (u32)s5;
            s0_p = (DrStp *)((TilePrim *)s5 + 1);
            ((Scratch5B304 *)0x1F800000)->p00 = s0_p;
            func_80093BFC(s0_p, 1);
            __asm__ volatile("" : "+r"(s0_p)); /* ASM_BARRIER: prevent CSE from folding s0_p + 1 into s5 + 28 */
            s5 = s0_p + 1;
            s0_p->addr = D_800ACC14->ot_b6c.addr;
            D_800ACC14->ot_b6c.addr = (u32)s0_p;
            ((Scratch5B304 *)0x1F800000)->p00 = s5;
        }
        g->p04 = (s32)&D_800ACC14->ot_b70;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800AC650 (*(char (*)[])&D_800AC650)

void func_8005B8D8(int a0) {
    char *s1 = D_800AC650;
    char *a;
    register char *t asm("$2");
    int v;
    char *t0;
    func_80096434(Q(s1, 0x3D8) + 0x74, 0x70);
    if (a0 < 4) {
        W(Q(s1, 0x3D8), 0x78) = 0;
        W(Q(s1, 0x3D8), 0x80) = (int)(D_800A1AA0 + a0 * 0x30);
    } else {
        a0 -= 4;
        W(Q(s1, 0x3D8), 0x78) = 1;
        W(Q(s1, 0x3D8), 0x80) = (int)(D_800A1B60 + (a0 << 5));
    }
    a = D_800AC650;
    t0 = Q(a, 0x3D8);
    W(t0, 0xE0) = 0x2D0;
    t = Q(a, 0x3D8);
    v = W(W(t, 0x80), 0);
    W(t, 0xB4) = v;
    W(t, 0x84) = v;
    t = Q(a, 0x3D8);
    v = W(W(t, 0x80), 4);
    W(t, 0xB8) = v;
    W(t, 0x88) = v;
    t = Q(a, 0x3D8);
    v = W(W(t, 0x80), 8);
    W(t, 0xBC) = v;
    W(t, 0x8C) = v;
    t0 = Q(a, 0x3D8);
    W(t0, 0x7C) = 1;
}

#undef D_800AC650

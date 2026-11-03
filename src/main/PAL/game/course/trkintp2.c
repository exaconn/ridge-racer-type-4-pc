#include "rr/track_interpolation.h"
#include "game/ui/uicode.h"

typedef struct { int x, y, z, w; } V4;

typedef struct { int a, b, c, d; } V;

extern unsigned char func_80043080();
extern int func_800430BC();
extern int func_800430D8();
extern unsigned char func_80043104();
extern unsigned char D_8009FE2C[];
extern int func_80096424();
extern int func_80096464();
extern char D_8010C158[];
extern char D_8010C168[];
extern void func_80092FFC();
extern void func_80092D50();
extern char *D_800ACBA0;
extern int func_8006E030();
extern int func_8006E09C();
extern int func_8006E108();
extern int func_80090CA4();
extern int func_80090DB4();
extern int func_80090EC4();
extern int func_800912D4();

int func_8004326C(unsigned char *a0, unsigned char *a1);
int func_8004342C();
int func_80043508(int *a, int *b, int *c, int *d, int *e, int f);
void func_80043608(int *p);
void func_800436E8(int *p);
int func_80043730();
void func_800437D8();
void func_800439BC(char *a, int b, int *c);
void func_80043AB0(RrRaceVehicle *vehicle, s32 *output, s32 distance);

int func_8004326C(unsigned char *a0, unsigned char *a1) {
    unsigned char *p = a1;
    unsigned char *q;
    unsigned char *r;
    int i;
    int k;
    int c;
    unsigned v;
    unsigned char *tab;
    c = func_800430BC(p[0], p[1]);
    i = 0;
    q = a0;
L1:
    {
        unsigned char t0 = p[2];
        unsigned char t1 = p[3];
        p += 2;
        i++;
        *q = func_800430BC(t0, t1);
        q++;
    }
    if (i < 6) goto L1;
    q = a0;
    i = 0;
    k = ((unsigned char)c % 11) & 0xFF;
    tab = D_8009FE2C;
    do {
        unsigned char *t;
        r = (unsigned char *)(k + (int)tab);
        t = q + i;
        k++;
        *t = func_80043080(*t, *r, 1);
        if (k >= 11) k = 0;
        i++;
    } while (i < 6);
    a0[5] = a0[5] & 0xF;
    if ((unsigned char)c != func_80043104(a0, 6)) return -1;
    func_800430D8(a0, 6);
    v = (((a0[4] & 0xF) << 16) + (a0[3] << 8) + a0[2]);
    v >>= 2;
    if (v > 0x927BF) return -1;
    v = (a0[1] << 4) + (a0[0] >> 4);
    v &= 0x3FF;
    if (v >= 0x280) return -1;
    v = a0[5] & 0xF;
    if (v >= 6) return -1;
    return 0;
}

int func_8004342C(a0, a1, a2, a3, a4, a5) int a0; int a1; int a2; int a3; int a4; int a5;
{
    unsigned s0; int r;
    s0 = a1 << 4;
    a0 &= 0xF;
    s0 |= a0;
    D_8010C168[0] = s0;
    r = func_80096464(a0);
    a2 = a2 * 4 + a3;
    D_8010C168[1] = (s0 >> 8) + (r & ~0x3F);
    D_8010C168[2] = a2;
    D_8010C168[3] = a2 >> 8;
    D_8010C168[4] = (a2 >> 16) + (func_80096464() & ~0x3F);
    D_8010C168[5] = a4 & 0xF;
    func_80043130(D_8010C168, D_8010C158);
    D_8010C158[0xD] = 0;
    func_80096424(a5, D_8010C158);
    return 0;
}

/* legacy typing of shared globals in this part of the unit */
#define func_80096424 ((void (*)())func_80096424)
#define D_8010C168 (*(unsigned char (*)[])&D_8010C168)

int func_80043508(int *a, int *b, int *c, int *d, int *e, int f) {
    unsigned t;
    func_80096424(D_8010C158, f);
    if (func_8004326C(D_8010C168, D_8010C158) == 0) {
        *a = D_8010C168[0] & 0xF;
        *b = (D_8010C168[1] * 16 + (D_8010C168[0] >> 4)) & 0x3FF;
        t = (D_8010C168[4] << 16) + (D_8010C168[3] << 8) + D_8010C168[2];
        *c = (t >> 2) & 0xFFFFF;
        *d = t & 3;
        *e = D_8010C168[5] & 0xF;
        return 0;
    }
    return -1;
}

#undef func_80096424
#undef D_8010C168

void func_80043608(int *p) {
    short r[4];
    unsigned short *h;
    p++;
    if (*p++ & 8) {
        h = (unsigned short *)p;
        r[0] = h[2];
        r[1] = h[3];
        r[2] = h[4];
        r[3] = h[5];
        func_80092FFC(r, h + 6);
        func_80092D50(0);
        p = (int *)((char *)h + (*(unsigned *)h >> 2 << 2));
    }
    h = (unsigned short *)p;
    r[0] = h[2];
    r[1] = h[3];
    r[2] = h[4];
    r[3] = h[5];
    if (r[2] > 0 && r[3] > 0) {
        func_80092FFC(r, h + 6);
        func_80092D50(0);
    }
}

void func_800436E8(int *p) {
    int n;
    p++;
    while ((n = *p++) > 0) {
        char *q = (char *)p + ((unsigned)n >> 2 << 2);
        func_80043608(p);
        p = (int *)q;
    }
}

int func_80043730(a0) char *a0;
{
    int t2 = 0;
    int t1 = 0x7FFFFFFF;
    char *a2 = D_800ACBA0;
    unsigned short a1 = *(unsigned short *)(a2 + 0x32);
    int a3 = 0;
    int a0v, v1, b;
    if (*(short *)(a2 + 0x32) != -1) {
        a0v = *(int *)(a0 + 0xF0);
        do {
            v1 = (short)a1;
            if (v1 < a0v) v1 = a0v - v1; else v1 = v1 - a0v;
            b = v1;
            if (D_800ABECC / 2 < v1) b = D_800ABECC - v1;
            v1 = b;
            if (v1 < t1) { t2 = a3; t1 = v1; }
            a2 += 0x44;
            a1 = *(unsigned short *)(a2 + 0x32);
            a3++;
        } while (*(short *)(a2 + 0x32) != -1);
    }
    return t2;
}

void func_800437D8(a0, a1) char *a0; int *a1;
{
    struct { int m[16]; int n[8]; short c[4]; short d[4]; V4 e; V4 f; } s;
    int *g;
    func_8006E09C(s.m, 0x800 - *(short *)(a0 + 0x82));
    func_8006E108(s.n, *(short *)(a0 + 0x80));
    func_80090CA4(s.m, s.n);
    func_8006E030(s.n, *(short *)(a0 + 0x84));
    func_80090DB4(s.m, s.n);
    s.c[0] = a1[0] * 4;
    s.c[1] = a1[1] * 4;
    s.c[2] = a1[2] * 4;
    func_80090EC4(s.n, s.c, &s.e);
    func_8006E09C(s.m, a1[5]);
    func_8006E108(s.n, *(short *)(a0 + 0x80));
    func_80090CA4(s.m, s.n);
    func_8006E030(s.n, *(short *)(a0 + 0x84));
    func_80090DB4(s.m, s.n);
    s.d[0] = *(unsigned short *)(a0 + 0x80);
    s.d[1] = 0;
    s.d[2] = *(unsigned short *)(a0 + 0x84);
    func_80090EC4(s.n, s.d, &s.f);
    *(V4 *)0x1F800018 = s.f;
    g = (int *)0x1F800000;
    g[7] = *(short *)(a0 + 0x82) + a1[5];
    g[6] += a1[4];
    g[8] += a1[6];
    *(V4 *)0x1F800008 = *(V4 *)(a0 + 0x70);
    g[2] += s.e.x;
    g[3] += s.e.y;
    g[4] += s.e.z;
    func_800912D4(a1[10]);
}

/* legacy typing of shared globals in this part of the unit */
#define func_8006E030 ((void (*)())func_8006E030)
#define func_8006E09C ((void (*)())func_8006E09C)
#define func_8006E108 ((void (*)())func_8006E108)
#define func_80090CA4 ((void (*)())func_80090CA4)
#define func_80090DB4 ((void (*)())func_80090DB4)
#define func_80090EC4 ((void (*)())func_80090EC4)

void func_800439BC(char *a, int b, int *c) {
    int m1[8];
    int m2[10];
    int t[3];
    func_8006E09C(m1, 0x800 - *(short *)(a + 0x82));
    func_8006E108(m2, *(short *)(a + 0x80));
    func_80090CA4(m1, m2);
    func_8006E030(m2, *(short *)(a + 0x84));
    func_80090DB4(m1, m2);
    func_80090EC4(m2, b, t);
    *(V *)c = *(V *)(a + 0x10);
    c[0] = c[0] + t[0];
    c[1] = c[1] + t[1];
    c[2] = c[2] + t[2];
}

#undef func_8006E030
#undef func_8006E09C
#undef func_8006E108
#undef func_80090CA4
#undef func_80090DB4
#undef func_80090EC4

void func_80043AB0(RrRaceVehicle *vehicle, s32 *output, s32 distance) {
    s16 current_y;
    s32 fraction;
    s32 index, length;
    /* ASM_BARRIER: use v1 for the PAL count page before loading vehicle fields. */
    register u8 *page __asm__("$3");
    s32 neighbour;
    RrTrackSample3C *next;
    RrTrackSample3C *current;

    if (D_800AD522 == 0) {
        index = vehicle->trackNodeF0 - ((s32) (distance - vehicle->trackFraction170) >> 0xA);
        neighbour = index - 1;
        if (index < 0) {
            index += D_800ABECC;
            neighbour = index - 1;
        }
        if (neighbour < 0) {
            neighbour += D_800ABECC;
        }
    } else {
        page=(u8*)0x800B0000U;
        /* ASM_BARRIER: preserve the page base for the signed count displacement. */
        __asm__("" : "+r"(page));
        length=*(s32*)(page-0x4134);
        index = vehicle->trackNodeF0 + ((s32) (distance - vehicle->trackFraction170) >> 0xA);
        neighbour = index + 1;
        if (index >= length) {
            index -= length;
            neighbour = index + 1;
        }
        /* This signed test is present in the original, including negative indices. */
        if (neighbour >= 0) {
            neighbour -= length;
        }
    }
    current = D_800ABEA4 + index;
    next = D_800ABEA4 + neighbour;
    fraction = (distance - vehicle->trackFraction170) & 0x3FF;
    output[0] = (s32) ((((s32) ((next->x08 - current->x08) * fraction) / 1024) + current->x08) * 4);
    current_y = current->y10;
    output[1] = (s32) (((s32) ((next->y10 - current_y) * fraction) / 1024) + current_y);
    output[2] = (s32) ((((s32) ((next->z0C - current->z0C) * fraction) / 1024) + current->z0C) * 4);
}

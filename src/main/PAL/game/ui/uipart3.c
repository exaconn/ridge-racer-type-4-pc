#include "rr/geometry.h"
#include "rr/gpu.h"

typedef struct { u8 c[8]; } B8;

typedef struct { char p[0xA]; unsigned short b; char q[0x5C - 0xC]; } E;

typedef struct {
    char pad[0x2A8];
    short s2A8;
    char pad2[0x76];
} S_800ABEF8;

extern struct { char pad[0xC]; unsigned char a; unsigned char b; short c; } D_800F4CB0;
extern s8 D_8009EFFC[];
extern char D_80010460[];
extern s32 D_800ACC14;
extern s32 func_8002071C();
extern s32 D_8009FDA4;
extern s32 func_8001FD20();
extern E D_800F3A38[];
extern int func_8004F24C();
extern int func_80020CD4();
extern char D_8009F008[];
extern S_800ABEF8 D_800ABEF8[];
extern int D_800F3A30;
extern int D_800F4C74;
extern int D_800F4C80[];
extern int func_80095A44();
extern char D_8009F014[];
extern unsigned short D_8009F01C[];
extern int func_80020E2C();

void func_8002AF8C();
void func_8002AFA4();
void func_8002B12C();
void func_8002B330();
int func_8002B574(int a, int b);
void func_8002B5D0();
void func_8002B75C(char *d, char *s);
void func_8002B7B4();
void func_8002B814();
void func_8002B934(char *a, int b);
void func_8002BA28(int a0);
char *func_8002BB88();
void func_8002BC7C(int a);

void func_8002AF8C(a, b, c) short a; int b; int c; { D_800F4CB0.c = a; D_800F4CB0.a = b; D_800F4CB0.b = c; }

void func_8002AFA4(a0, a1, a2, a3, a4, a5) s32 a0; s32 a1; s32 a2; s32 a3; s32 a4; s32 a5; {
    s32 i, j, k, t0, sel, sg, off;
    register s32 s1 __asm__("$17"); /* ASM_BARRIER: pin i+1 to s1 so loop var i gets s2 (saved reg order) */
    s8 *tb;
    RrVec2s *p, *bp;
    char *ub;
    struct { RrVec2s v[3]; s32 pad; B8 u; } w;
    i = 0;
    bp = w.v;
    tb = D_8009EFFC;
    t0 = *(s32 *)0x1F800000;
    w.u = *(B8 *)D_80010460;
L0:
    j = 0;
L1:
    k = 0;
    sg = 1 - (i & 1) * 2;
    p = bp;
    off = j << 1;
    do {
        p->x = (*(s8 *)(off + (s32)tb) - a2) * sg + a0;
        p->y = *(s8 *)(off + (s32)tb + 1) + a1;
        off += 4;
        k++;
        p++;
    } while (k < 3);
    s1 = i + 1;
    sel = 0;
    if (a5 == s1) sel = 0 < j;
    j++;
    sg = D_800ACC14 + 0x70;
    ub = (char *)&w.u;
    t0 = func_8002071C(sg, t0, ((s32 *)w.v)[0], ((s32 *)w.v)[1], ((s32 *)w.v)[2], ub + sel * 4, 0);
    if (j < 2) goto L1;
    i = s1;
    if (i < 2) goto L0;
    *(s32 *)0x1F800000 = t0;
}

void func_8002B12C(p0, p1, p2, p3, p4, a5, a6) s32 p0; s32 p1; s32 p2; s32 p3; s32 p4; s32 a5; s32 a6; {
    s32 i, k, t0, g, sel, sg;
    register s32 one __asm__("$18"); /* ASM_BARRIER: pin s-reg order (saved reg priority) */
    s32 a3; 
    register s32 a4 __asm__("$20"); /* ASM_BARRIER: pin s-reg order (saved reg priority) */
    s32 a0, a1;
    s32 a2; 
    RrVec2s *p, *bp;
    char *ub;
    s8 *tb, *nv;
    struct { RrVec2s v[3]; s32 pad; B8 u; } w;
    a0 = p0; a1 = p1; a2 = p2; a3 = p3;
    i = 0;
    bp = w.v;
    one = 1;
    a4 = p4;
    t0 = *(s32 *)0x1F800000;
    g = D_8009FDA4;
    nv = D_8009EFFC;
    w.u = *(B8 *)D_80010460;
L0:
    k = 0;
    sg = one - (i & 1) * 2;
    p = bp;
    tb = nv;
    do {
        p->x = (tb[0] - a2) * sg + a0;
        p->y = tb[1] + a1;
        tb += 4;
        k++;
        p++;
    } while (k < 3);
    if (a3 == one) g = 0;
    if (g == one) {
        sel = 1;
        if (i != 0) goto B8;
    } else {
B8:
        sel = 0;
        if (g == 2) sel = i == 1;
    }
    sel ^= a3;
    if (a4 == a5) {
        if (i != 0) goto B12;
    } else {
B12:
        if (a4 == a6 - 1) {
            if (i != one) goto B14;
        } else {
B14:
            ub = (char *)&w.u;
            t0 = func_8002071C(D_800ACC14 + 0x70, t0, ((s32 *)w.v)[0], ((s32 *)w.v)[1], ((s32 *)w.v)[2], ub + (sel << 2), 0);
        }
    }
    i++;
    if (i < 2) goto L0;
    *(s32 *)0x1F800000 = t0;
    if (a3 == 1) {
        *(s32 *)0x1F800000 = func_8001FD20(D_800ACC14 + 0x70, t0, a0, a1 - 0xC, a3, 0x18, 0xFF, 0xFF, 0xFF);
    }
}

void func_8002B330(a0, a1, a2, a3, a4) int a0; int a1; char *a2; int a3; int a4; {
    int s0;
    int a;
    unsigned short b;
    switch (a1) {
    case 2:
        s0 = *(short *)a2;
        a = (a0 >> 2) & 1;
        break;
    case 4:
        s0 = *(int *)a2;
        a = (a0 >> 2) & 1;
        break;
    case 1:
    case -1:
        s0 = *(unsigned char *)a2;
        a = (a0 >> 2) & 1;
        break;
    default:
        a = (a0 >> 2) & 1;
        break;
    }
    if ((a0 & 1) == 0) {
        b = D_800F3A38[a].b;
        if ((b & 0x8000) == 0) {
            if ((b & 0x2000) == 0) {
                if (b & 0x1000) {
                    s0 -= 1;
                    goto L;
                }
                if (b & 0x4000) {
                    s0 += 1;
                    goto L;
                }
            }
        }
    } else {
        b = D_800F3A38[a].b;
        if ((b & 0x1000) == 0) {
          if ((b & 0x4000) == 0) {
            if (b & 0x8000) {
                D_8009FDA4 = 1;
                s0 -= 1;
                goto L;
            }
            if (b & 0x2000) {
                D_8009FDA4 = 2;
                s0 += 1;
                goto L;
            }
          }
        }
        D_8009FDA4 = 0;
        goto M;
    }
    goto M;
L:
    if ((a0 & 2) == 0) {
        if (s0 < a3) {
            if (s0 >= a4) {
                func_8004F24C(0, a);
            } else {
                s0 = a4;
            }
        } else {
            s0 = a3 - 1;
        }
    } else {
        func_8004F24C(0, a);
        if (s0 >= a3) {
            s0 = a4;
        } else if (s0 < a4) {
            s0 = a3 - 1;
        }
    }
M:
    switch (a1) {
    case 2: *(short *)a2 = s0; break;
    case 4: *(int *)a2 = s0; break;
    case 1:
    case -1: *(char *)a2 = s0; break;
    }
}

int func_8002B574(int a, int b)
{
    int q = a / 25;
    a -= q * 25;
    a = a * 40 + b;
    return q * 1000 + a;
}

void func_8002B5D0(a0, a1, b, a3, off)
int a0; int a1; int b; int a3; int off;
{
    if (b >= 0) {
        int m = b / 1000;
        int r = m % 60;
        int t;
        m = m / 60;
        t = b % 1000;
        D_8009F008[1] = m + 0x30;
        D_8009F008[6] = t / 100 + 0x30;
        D_8009F008[7] = t / 10 % 10 + 0x30;
        D_8009F008[8] = t % 10 + 0x30;
        D_8009F008[3] = r / 10 + 0x30;
        D_8009F008[4] = r % 10 + 0x30;
    } else {
        D_8009F008[1] = 0x3A;
        D_8009F008[3] = 0x3A;
        D_8009F008[4] = 0x3A;
        D_8009F008[6] = 0x3A;
        D_8009F008[7] = 0x3A;
        D_8009F008[8] = 0x3A;
    }
    func_80020CD4(a0, a1, D_8009F008 + off, a3);
}

void func_8002B75C(char *d, char *s)
{
    int i = 0;
    char *p;
    if (*s) {
        do {
            d[i] = *s++;
            i++;
        } while (*s);
    }
    while (*(p = d + i++)) {
        *p = 0;
    }
}

void func_8002B7B4(d, s, n) char *d; unsigned char *s; int n; {
    int i = 0;
    if (*s != 0) do { unsigned char c = *s; s++; d[i] = c; i++; } while (*s != 0);
    if (i < n) do { d[i] = 0x20; i++; } while (i < n);
    d[i] = 0;
}

void func_8002B814(a, b, c) char *a; int b; int c; {
    int t;
    a[c] = b % 10 + 0x30;
    t = b / 10;
    switch (c) {
    case 1:
        a[0] = t + 0x30;
        return;
    case 2: {
        int h = b / 100;
        a[0] = h + 0x30;
        a[1] = t - h * 10 + 0x30;
        return;
    }
    case 3: {
        int k = b / 1000;
        int h = b / 100;
        a[0] = k + 0x30;
        a[1] = h - k * 10 + 0x30;
        a[2] = t - h * 10 + 0x30;
        return;
    }
    }
}

void func_8002B934(char *a, int b)
{
  int s;
  int t;
  int m;
  int r;
  if (b >= 0)
  {
    if (s)
    {
s = b / 1000; m = s / 60; r = s % 60; t = b % 1000;
      func_8002B814(a, m, 1);
      func_8002B814(a + 3, r, 1);
      func_8002B814(a + 6, t, 2);
      return;
    }
    else
    {
s = b / 1000; m = s / 60; r = s % 60; t = b % 1000;
      func_8002B814(a, m, 1);
      func_8002B814(a + 3, r, 1);
      func_8002B814(a + 6, t, 2);
      return;
    }
  }
  a[8] = 0x2D; a[7] = 0x2D; a[6] = 0x2D; a[4] = 0x2D; a[3] = 0x2D; a[1] = 0x2D; a[0] = 0x2D;
}

void func_8002BA28(int a0) {
    /* ASM_BARRIER: pin &D_800ABEF8[a0] base pointer to $a0 */
    register S_800ABEF8 *p0 asm("a0");
    /* ASM_BARRIER: reserve $v0 across D_800F3A30 %hi address load so %hi(D_800F3A30) uses $v1 */
    register int dummy asm("v0");
    /* ASM_BARRIER: pin D_800F3A30 value load to $v1 */
    register int n asm("v1");
    int s7;
    int i;
    int s3;
    int v;
    int y;
    RrSpritePacket *p;

    s7 = D_800ABEF8[a0].s2A8;
    p0 = &D_800ABEF8[a0];
    /* ASM_BARRIER: combine D_800ABEF8[a0] address into $a0 and start $v0 live range before D_800F3A30 %hi */
    __asm__("" : "+r"(p0), "=r"(dummy));
    v = D_800F3A30;
    /* ASM_BARRIER: order prologue stack stores before D_800F3A30 %lo load and p0->s2A8 load in sched2 */
    __asm__("" : "=m"(*p0), "=m"(D_800F3A30) : "r"(dummy));
    s7 = p0->s2A8;
    n = D_800F3A30;
    if (n > 0) {
        i = 0;
        do {
            if (i == s7) {
                /* ASM_BARRIER: prevent jump_optimize from merging identical then/else branches and give i higher alloc priority than GIV */
                __asm__("" : : "r"(i));
                s3 = 0x7FF8;
            } else {
                s3 = 0x7FF8;
            }
            v = D_800F4C80[i];
            y = i * 10 + 0x2C;
            func_8002B5D0(0xF0, y, v, s3, 0);
            p = *(RrSpritePacket **)0x1F800000;
            p->len = 4;
            p->code = 0x64;
            p->x = 0xE7;
            p->y = y;
            p->u = i * 12 + 12;
            p->v = 0xA8;
            p->clut = s3;
            p->code |= 1;
            p->w = 0xC;
            p->h = 0xC;
            func_80095A44(D_800ACC14 + 0x70, p);
            p++;
            *(RrSpritePacket **)0x1F800000 = p;
            i++;
        } while (i < D_800F3A30);
    }
    func_8002B5D0(0xF0, 0x1C, D_800F4C74, s3, 0);
}

char *func_8002BB88(a) int a; {
    if (a >= 0) {
        int s = a / 25;
        int m = s / 60;
        int r = s % 60;
        D_8009F014[0] = m / 10 + 0x30;
        D_8009F014[1] = m % 10 + 0x30;
        D_8009F014[3] = r / 10 + 0x30;
        D_8009F014[4] = r % 10 + 0x30;
    } else {
        D_8009F014[0] = 0x2D; D_8009F014[1] = 0x2D; D_8009F014[3] = 0x2D; D_8009F014[4] = 0x2D;
    }
    return D_8009F014;
}

void func_8002BC7C(int a)
{
    char *p = D_800ACC14 + 0x226EC;
    int r = func_8002BB88(a);
    int i = a < 0x5DC;
    int t = D_8009F01C[i];
    int s = (D_800F3A30 - 3) * 10 + 0x50;
    func_80020E2C(D_800ACC14 + 0x70, 0x111, s, r, t);
    *(short *)(p + 0xA) = s;
}

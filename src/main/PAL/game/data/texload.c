/* RODATA 0x8001007c 0x3c */
#include "types.h"
#include "rr/renderer_setup.h"
#include "rr_asm.h"

#define B (*(int *)&D_800ACC18)

struct A { char p[0x14]; unsigned f14; char q[0x20]; unsigned f38; };

struct Q { int a; int v; };

struct E {
    struct A *src; int pad0; int z8, zC, z10;
    struct Q q1[4];
    int z34, z38;
    struct Q q2[4];
    int z5c, z60, z64, z68, z6c, z70;
    int pad74[4];
    struct Q q3[8];
    int fC4;
};

typedef struct {
    s32 header;
    s32 p4, p8, pC, p10, p14;
} Resource;

extern int func_80095C14();
extern unsigned char D_8009DFD8[];
extern unsigned char D_8009DFEC[];
extern int func_80096434();
extern int D_800ACC18;
extern int *D_800F3A08[];
extern struct E D_8010B2E0[];
extern int D_8009DF98;
extern int D_8010943C;
extern int D_8010BF68;
extern int D_8010BF60;
extern int func_80012208();
extern int func_8001292C();
extern int func_80012A58();
extern void func_80012A68();
extern void func_800292A8();
extern void func_800293C4();
extern void func_800436E8();
extern void func_800700AC();
extern void func_80070108();
extern void func_8007026C();
extern void func_8007080C();
extern void func_800713C4();
extern void func_80071434();
extern int func_80077D08();
extern int D_80109440;
extern int D_80109444;
extern int D_80109448;
extern int D_8010944C;
extern int D_80109450;
extern int D_80109454;
extern int func_80012210();
void func_80012AB4(void *, s32);
void func_800293AC(void *, s32);
void func_80092D50(s32);
void func_80092FFC(s16 *, void *);
extern s32 D_80109468[];
extern int func_8001223C();
extern short D_800F3914;
extern short D_8009E02C[];
extern char *D_800F4D90;
extern short D_800ABED0[];
extern short D_800ABED6[];
extern short D_800ABEDC[];

void func_80012C7C(int *a0, int a1);
int func_80012E94(int a0, int a1, int a2, int a3, int a4);
void func_8001301C(struct A *a0, int a1);
int func_800131D0(int a0);
int func_80013200(void);
int func_80013514(int a0, int a1, int a2, int a3, int a4);
s32 func_800135BC(void);
int func_800137B0(void);
int func_800137D4(void);
void func_80013890(s32 shift);
int func_80013904(char *a0_in, int a1_in);

void func_80012C7C(int *a0, int a1) {
    int n = a0[0];
    char *list = (char *)(a0 + 1);
    int x = (a1 << 6) + 0x180;
    int tp_x = (x & 0x3FF) >> 6;
    int clut_x = (x >> 4) & 0x3F;
    int m1 = ~0x1F;
    int m2 = ~0x403C;

    while (n-- > 0) {
        char *p = *(char **)(list + 8);
        unsigned int cmd;
        list += 0xC;
        while (cmd = *(unsigned int *)p, p += 4, cmd != 0) {
            int count = cmd >> 16;
            switch (cmd & 0xFFFF) {
            case 7:
                p += count * 20;
                break;
            case 8:
                p += count * 28;
                break;
            case 1:
                while (count-- > 0) {
                    *(short *)(p + 0x12) = (*(short *)(p + 0x12) & m1) | tp_x;
                    *(short *)(p + 0xE) = (*(short *)(p + 0xE) & m2) | clut_x;
                    p += 0x1C;
                }
                break;
            case 9:
                p += count * 20;
                break;
            case 5:
            case 10:
                p += count * 24;
                break;
            case 3:
                p += count * 20;
                break;
            case 4:
                p += count * 12;
                break;
            case 6:
                p += count * 8;
                break;
            case 11:
                while (count-- > 0) {
                    *(short *)(p + 0x1A) = (*(short *)(p + 0x1A) & m1) | tp_x;
                    *(short *)(p + 0x16) = (*(short *)(p + 0x16) & m2) | clut_x;
                    p += 0x20;
                }
                break;
            case 14:
                while (count-- > 0) {
                    *(short *)(p + 0x16) = (*(short *)(p + 0x16) & m1) | tp_x;
                    *(short *)(p + 0x12) = (*(short *)(p + 0x12) & m2) | clut_x;
                    p += 0x1C;
                }
                break;
            case 15:
                while (count-- > 0) {
                    *(short *)(p + 0x1A) = (*(short *)(p + 0x1A) & m1) | tp_x;
                    *(short *)(p + 0x16) = (*(short *)(p + 0x16) & m2) | clut_x;
                    p += 0x24;
                }
                break;
            }
        }
    }
}

int func_80012E94(int a0, int a1, int a2, int a3, int a4)
{
    int b1 = a1;
    int xx;
    /* ASM_BARRIER: pins a<<4 temp to a3 ($7) to match reg choice */
    register int b16 asm("$7");
    int t;
    int c;
    int a, b, x;
    char *s0, *p0, *p1;
    char * volatile *g;
    /* ASM_BARRIER: keeps 0x1F80007C pointer in a1 ($5) */
    register short *sp asm("$5");

    sp = (short *)0x1F80007C;
    if (a0 == 2 && b1 == 0 && a2 == 0x13)
        return 0x13;
    t = D_8009DFEC[b1 * 16 + a3];
    /* ASM_BARRIER: forces copy t->a (original addu a0,v1,zero) */
    __asm__("" : "=r"(a) : "0"(t));
    xx = (t & 7) * 0x10 + 0x340; x = xx;
    if (t < 0) a = t + 7;
    a >>= 3;
    c = D_8009DFD8[a2];
    b16 = a << 4; b = b16 + (a << 2);
    if (c >= 4) {
        if (c >= 7)
            b += 0x78;
        else
            b += 0x3C;
    }
    g = (char * *)0x1F800000;
    s0 = g[0];
    sp[0] = xx;
    sp[1] = b;
    *(int *)(sp + 2) = 0x140010;
    func_80095C14(s0, sp, (a4 << 6) + 0x1B0, 0xD8);
    *(int *)s0 = *(int *)g[1];
    s0[3] = 5;
    *(int *)g[1] = (int)s0;
    g[1][3] = 0;
    s0 += 0x18;
    g[0] = s0;
    *(int *)(s0 + 4) = 0xE6000000;
    p1 = g[1]; p0 = g[0]; *(int *)p0 = *(int *)p1;
    g[0][3] = 1;
    *(int *)g[1] = (int)g[0];
    g[1][3] = 0;
    g[0] += 8;
    /* ASM_BARRIER: keeps li v0,0 after final sw (stops sched hoisting it) */
    { int r; __asm__ volatile("" : "=r"(r) : "0"(0)); return r; }
}

void func_8001301C(struct A *a0, int a1)
{
    int s1;
    int a3, v1, t, u;
    struct E *s0;
    struct Q *p;

    s0 = (struct E *)((char *)D_8010B2E0 + a1 * 0x320);
    a3 = D_800ACC18;
    u = ((a0->f14 + 2) / 3) * 0x18;
    t = ((a0->f38 + 2) / 3) * 0x18;
    s1 = 4;
    do {
        p = s0->q1;
        v1 = 3;
        s0->src = a0;
        s0->z8 = 0;
        s0->zC = 0;
        s0->z10 = 0;
        s0->z34 = 0;
        s0->z38 = 0;
        s0->z5c = 0;
        s0->z60 = 0;
        s0->z64 = 0;
        s0->z68 = 0;
        s0->z6c = 0;
        s0->z70 = 0;
        do {
            p->v = a3;
            p++;
            v1 -= 1;
            a3 += u;
        } while (v1 >= 0);
        p = s0->q2;
        v1 = 3;
        do {
            p->v = a3;
            p++;
            v1 -= 1;
            a3 += t;
        } while (v1 >= 0);
        p = s0->q3;
        v1 = 7;
        do {
            p->v = a3;
            p++;
            v1 -= 1;
            a3 += t;
        } while (v1 >= 0);
        s0++;
        s1 -= 1;
    } while (s1 > 0);
    D_800ACC18 = a3;
    if (*(unsigned char *)((char *)D_800F3A08[a1] + 0xA) == 2) {
        s0 = (struct E *)((char *)D_8010B2E0 + a1 * 0x320);
        s1 = 4;
        do {
            s1 -= 1;
            s0->fC4 = D_800ACC18;
            func_80096434(D_800ACC18, 0x100);
            s0++;
            D_800ACC18 += 0x100;
        } while (s1 > 0);
    }
}

int func_800131D0(int a0) {
    if (D_8009DF98 != 0) {
        return 0;
    }
    D_8010943C = a0;
    return D_8009DF98 = 1;
}

int func_80013200(void)
{
    int *a1;
    /* ASM_BARRIER: pin saved regs to original assignment */
    register int s1 __asm__("$17");
    register int s4 __asm__("$20");
    int s2,s3,s5,s6,s7,fp;
    int sp10, sp14, sp18;
    /* ASM_BARRIER: pin temps to the original v0/v1/a0 choices */
    register int a0 __asm__("$4");
    register int u __asm__("$2");
    register int t __asm__("$3");
    register int *pw __asm__("$2"); int s0;

    switch (D_8009DF98) {
    case 1:
        if (func_8001292C(func_80012208(D_8010943C), D_800ACC18) == 0)
            return 0;
        D_8009DF98 = 2;
    case 2:
        if (func_80012A58() == 0)
            return 0;
        a0 = *(int *)(D_800ACC18 + 4) + D_800ACC18;
        s0 = *(int *)(D_800ACC18 + 8) + D_800ACC18;
        *(int *)(D_800ACC18 + 4) = a0;
        *(int *)(D_800ACC18 + 8) = s0;
        func_800436E8(a0);
        func_800436E8(s0);
        D_8009DF98 = 3;
    case 3:
        if (func_8001292C(func_80012208(D_8010943C) + 1, D_800ACC18) == 0)
            return 0;
        D_8009DF98 = 4;
    case 4:
        s1 = func_80012A58();
        if (s1 == 0)
            return 0;
        a1 = (int *)B;
        a1++; t = *a1; sp10 = t + B; *a1 = sp10;
        a1++; t = *a1; s2 = t + B; *a1 = s2;
        a1++; t = *a1; sp14 = t + B; *a1 = sp14;
        a1++; t = *a1; sp18 = t + B; *a1 = sp18;
        a1++; t = *a1; fp = t + B; *a1 = fp;
        a1++; t = *a1; s6 = t + B; *a1 = s6;
        a1++; t = *a1; s7 = t + B; *a1 = s7;
        a1++; t = *a1; s3 = t + B; *a1 = s3;
        a1++; t = *a1; s5 = t + B; *a1 = s5;
        /* ASM_BARRIER: hoist lui of D_8010943C above the store (hi part only) */
        a1++; t = *a1; u = B; s4 = t + u; __asm__("" : "=r"(pw) : "0"(0x80110000)); *a1 = s4;
        a1++;
        if (*(int *)((char *)pw - 0x6BC4) == 7) {
            a0 = *a1; a0 += B;
            *a1 = a0;
            func_8007080C(a0);
        }
        D_8010BF68 = s2;
        D_800ACC18 = D_800ACC18 + s1;
        func_8007026C(sp10);
        func_80012A68(s2);
        func_80070108(sp14, 4);
        func_800700AC(sp18);
        func_800292A8(fp);
        func_800293C4(s6);
        D_8010BF60 = s7;
        D_800ACC18 = D_800ACC18 + func_80077D08(s3, D_800ACC18);
        func_800713C4(s5);
        func_80071434(s4);
        D_8009DF98 = 0;
        return 1;
    }
}

int func_80013514(int a0, int a1, int a2, int a3, int a4)
{
    int r;
    register int p0 asm("$17") = a0;
    register int p1 asm("$16") = a1;
    register int p2 asm("$18") = a2;
    register int p3 asm("$19") = a3;
    if (D_8009DF98 != 0) {
        return 0;
    }
    D_80109440 = func_80012210();
    r = 1;
    D_80109444 = a4;
    D_80109448 = p1;
    D_8010944C = p0;
    D_80109450 = p2;
    D_80109454 = p3;
    D_8009DF98 = r;
    return r;
}

/* legacy typing of shared globals in this part of the unit */
#define func_8001292C ((s32 (*)(s32, void *))func_8001292C)
#define func_80012A58 ((s32 (*)(void))func_80012A58)
#define func_80012A68 ((void (*)(void *))func_80012A68)
#define func_80070108 ((void (*)(void *, s32))func_80070108)
#define D_800ACC18 (*(Resource *(*))&D_800ACC18)
#define D_800F3A08 (*(void *(*)[])&D_800F3A08)
#define D_8010BF60 (*(short *(*))&D_8010BF60)

s32 func_800135BC(void) {
    s16 rect[4];
    void *s0;
    void *s1;
    void *s2;
    void *s3;

    switch (D_8009DF98) {
    case 1:
        if (func_8001292C(D_80109440, D_800ACC18) == 0) {
            return 0;
        }
        D_8009DF98 = 2;
    case 2:
        if (func_80012A58() == 0) {
            return 0;
        }
        {
            s32 x;
            D_800ACC18->p4 += (s32)D_800ACC18;
            D_800ACC18->p8 += (s32)D_800ACC18;
            D_800ACC18->pC += (s32)D_800ACC18;
            D_800ACC18->p10 += (s32)D_800ACC18;
            D_800ACC18->p14 += (s32)D_800ACC18;
            s0 = (void *)D_800ACC18->pC;
            s1 = (void *)D_800ACC18->p10;
            s2 = (void *)D_800ACC18->p14;
            s3 = (void *)D_800ACC18->p8;
            func_800293AC((void *)D_800ACC18->p4, D_80109444);
            D_800F3A08[D_80109444] = s3;
            func_80070108(s0, D_80109444);
            /* ASM_BARRIER: preserve local-alloc priority of s0 ($s0) over s3 ($s3) */
            __asm__("" : : "r"(s0));
            func_80012AB4(s0, D_80109444);
            D_80109468[D_80109444] = (s32)s1;
            func_80012A68(s1);
            func_80012C7C(s1, D_80109444);
            rect[2] = 0x40;
            rect[1] = 0;
            rect[3] = 0x100;
            x = (D_80109444 << 6) + 0x180;
            rect[0] = x;
        }
        func_80092FFC(rect, s2);
        func_80092D50(0);
        D_800ACC18 = s2;
        func_8001301C(D_80109468[D_80109444], D_80109444);
        if (D_80109454 != 0) {
            func_80012E94(D_80109448, D_8010944C, D_80109450, D_80109454 - 1, D_80109444);
        }
        D_8009DF98 = 0;
        return 1;
    }
}

int func_800137B0(void) {
    if (D_8009DF98 != 0) return 0;
    return D_8009DF98 = 1;
}

int func_800137D4(void) {
    short sp[4];
    switch (D_8009DF98) {
    case 1:
        if (func_8001292C(func_8001223C(0, 0), D_800ACC18) == 0) return 0;
        D_8009DF98 = 2;
    case 2:
        if (func_80012A58() == 0) return 0;
        sp[0] = 0x340;
        sp[2] = 0x80;
        sp[1] = 0;
        sp[3] = 0xB4;
        func_80092FFC(sp, D_800ACC18);
        func_80092D50(0);
        D_8009DF98 = 0;
        return 1;
    default:
        return 2;
    }
}

void func_80013890(s32 shift) {
    /* ASM_BARRIER: reuse the incoming argument register for the scratch base. */
    register RrRendererScratch *scratch __asm__("$4");
    *(s32*)0x1F800060U=shift;
    scratch=(RrRendererScratch*)0x1F800000U;
    scratch->renderFlags64=D_800AD568;
    D_800F3A20.b=128;
    D_800F3A20.g=128;
    D_800F3A20.r=128;
    D_800F3A20.code=0x2C;
    D_800F3A18.b=255;
    D_800F3A18.g=255;
    D_800F3A18.r=255;
    D_800F3A18.code=0x3C;
    scratch->viewport6C.rect.w=320;
    scratch->viewport6C.rect.x=0;
    scratch->viewport6C.rect.y=0;
    scratch->viewport6C.rect.h=256;
}

int func_80013904(char *a0_in, int a1_in) {
    /* ASM_BARRIER: keep temp in $v0 */
    register int v0 __asm__("$2");
    /* ASM_BARRIER: keep temp in $v1 */
    register int v1 __asm__("$3");
    /* ASM_BARRIER: keep a0_in / temp in $a0 */
    register int a0 __asm__("$4") = (int)a0_in;
    /* ASM_BARRIER: keep a1_in / temp in $a1 */
    register int a1 __asm__("$5") = a1_in;
    /* ASM_BARRIER: keep temp in $a2 */
    register int a2 __asm__("$6");
    /* ASM_BARRIER: keep temp in $a3 */
    register int a3 __asm__("$7");
    /* ASM_BARRIER: keep temp in $t0 */
    register int t0 __asm__("$8");
    /* ASM_BARRIER: keep temp in $t1 */
    register int t1 __asm__("$9");
    /* ASM_BARRIER: keep temp in $t2 */
    register int t2 __asm__("$10");
    /* ASM_BARRIER: keep a1_in copy in $t3 */
    register int t3 __asm__("$11");
    /* ASM_BARRIER: keep scratchpad 0x1F80007C base in $t7 */
    register int *t7 __asm__("$15");
    /* ASM_BARRIER: keep destination pointer in $t8 */
    register short *t8 __asm__("$24");

    t3 = a1;
    t7 = (int *)0x1F80007C;
    /* ASM_BARRIER: order t3 and t7 setup */
    __asm__ volatile("" : "+r"(t3), "+r"(t7));
    t7[0] = t3;
    a3 = *(short *)(a0 + 0x172);
    t2 = *(short *)(a0 + 0x170);
    v0 = *(int *)(a0 + 0xF0);
    v1 = (int)D_8010BF60;
    /* ASM_BARRIER: order initial loads from a0 and D_8010BF60 */
    __asm__ volatile("" : "+r"(a3), "+r"(t2), "+r"(v0), "+r"(v1));
    t1 = a3 << 3;
    a3 = t1 + 0x2000;
    t2 <<= 2;
    v0 = (v0 << 1) + v1;
    /* ASM_BARRIER: order shifts and pointer addition before table loads */
    __asm__ volatile("" : "+r"(t1), "+r"(a3), "+r"(t2), "+r"(v0));
    t0 = *(short *)(v0 + 0);
    a2 = *(short *)(v0 + 2);
    /* ASM_BARRIER: load t0 and a2 before division-by-4096 branch */
    __asm__ volatile("" : "+r"(t0), "+r"(a2), "+r"(a3), "+r"(t1));
    a1 = a3;
    if (a3 < 0) {
        a1 = t1 + 0x2FFF;
    }
    v1 = a1 >> 12;
    v0 = (v1 << 1) + v1;
    a0 = v0 + 3;
    /* ASM_BARRIER: compute bit shift amounts v0 and a0 */
    __asm__ volatile("" : "+r"(v0), "+r"(a0), "+r"(a2), "+r"(t0));
    a1 = a2 >> a0;
    v1 = a2 >> v0;
    a2 = v1 & 7;
    a0 = t0 >> a0;
    v0 = t0 >> v0;
    t0 = v0 & 7;
    a3 &= 0xFFF;
    a1 &= 7;
    /* ASM_BARRIER: order bit extractions before D_800F3914 load */
    __asm__ volatile("" : "+r"(a2), "+r"(t0), "+r"(a3), "+r"(a1));
    /* ASM_BARRIER: load %hi(D_800F3914) before masking a0 */
    __asm__ volatile("lui %0, %%hi(D_800F3914)" : "=r"(v0) : "r"(a0));
    a0 &= 7;
    /* ASM_BARRIER: load %lo(D_800F3914) after masking a0 */
    __asm__ volatile("lh %0, %%lo(D_800F3914)(%1)" : "=r"(v1) : "r"(v0), "r"(a0));
    v0 = (int)D_8009E02C;
    /* ASM_BARRIER: load D_8009E02C address before index shifts */
    __asm__ volatile("" : "+r"(v0));
    a2 <<= 1;
    a1 <<= 1;
    a0 <<= 1;
    v1 = (v1 << 4) + v0;
    /* ASM_BARRIER: order index shifts and row base calculation */
    __asm__ volatile("" : "+r"(a2), "+r"(a1), "+r"(a0), "+r"(v1));
    a2 += v1;
    a1 += v1;
    v0 = (t0 << 1) + v1;
    a0 += v1;
    /* ASM_BARRIER: order table element addresses before loads */
    __asm__ volatile("" : "+r"(a2), "+r"(a1), "+r"(v0), "+r"(a0));
    v1 = *(short *)a2;
    a1 = *(short *)a1;
    a2 = *(short *)v0;
    v0 = *(short *)a0;
    /* ASM_BARRIER: interpolate table values via GTE INTPL */
    __asm__ volatile(
        "mtc2 %4, $8\n\t"
        "ctc2 %0, $21\n\t"
        "ctc2 %2, $22\n\t"
        "mtc2 %3, $9\n\t"
        "mtc2 %1, $10\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A980011\n\t"
        "mtc2 %5, $8\n\t"
        "mfc2 %1, $10\n\t"
        "nop\n\t"
        "ctc2 %1, $21\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A980011\n\t"
        "mfc2 %1, $9\n\t"
        "nop\n\t"
        "mtc2 %1, $8"
        : "+r"(v0), "+r"(v1), "+r"(a1), "+r"(a2)
        : "r"(a3), "r"(t2)
    );

    a0 = t3 & 0xF;
    v0 = (t3 >> 4) & 0xF;
    /* ASM_BARRIER: extract low and high nibbles of t3 and load %hi(D_800F4D90) */
    __asm__ volatile("lui %0, %%hi(D_800F4D90)" : "=r"(a1) : "r"(a0), "r"(v0));
    v1 = (((v0 << 1) + v0) << 2) - v0;
    v1 <<= 2;
    t7[2] = v0;
    v0 = (((a0 << 1) + a0) << 2) - a0;
    /* ASM_BARRIER: load %lo(D_800F4D90) after subu v0, v0, a0 */
    __asm__ volatile("lw %0, %%lo(D_800F4D90)(%0)" : "+r"(a1) : "r"(v1), "r"(v0));
    v0 <<= 2;
    t7[1] = a0;
    /* ASM_BARRIER: store t7[1] before pointer additions */
    __asm__ volatile("" : "+r"(v0));
    v1 = a1 + v1;
    a1 = a1 + v0;
    /* ASM_BARRIER: keep struct pointers in v1 and a1 */
    __asm__ volatile("" : "+r"(v1), "+r"(a1));

    a2 = *(int *)(v1 + 0x10);
    a3 = *(short *)(v1 + 0x14);
    a0 = *(int *)(a1 + 0x10);
    t2 = *(short *)(a1 + 0x14);
    /* ASM_BARRIER: load first vector pair at +0x10 */
    __asm__ volatile("" : "+r"(a2), "+r"(a3), "+r"(a0), "+r"(t2));
    t0 = a2 >> 16;
    v0 = a2 << 16;
    /* ASM_BARRIER: unpack low half of a2 via v0 */
    __asm__ volatile("" : "+r"(t0), "+r"(v0));
    a2 = v0 >> 16;
    t1 = a0 >> 16;
    v0 = a0 << 16;
    /* ASM_BARRIER: unpack low half of a0 via v0 */
    __asm__ volatile("" : "+r"(a2), "+r"(t1), "+r"(v0));
    a0 = v0 >> 16;
    /* ASM_BARRIER: finish unpacking first vector pair */
    __asm__ volatile("" : "+r"(a0));
    RR_GTE_SETFC(a2, t0, a3);
    /* ASM_BARRIER: load IR1..IR3 and run INTPL */
    __asm__ volatile(
        "mtc2 %0, $9\n\t"
        "mtc2 %1, $10\n\t"
        "mtc2 %2, $11\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A980011"
        :
        : "r"(a0), "r"(t1), "r"(t2)
    );

    t8 = D_800ABED0;
    a2 = *(int *)(v1 + 0x18);
    a3 = *(short *)(v1 + 0x1C);
    a0 = *(int *)(a1 + 0x18);
    t2 = *(short *)(a1 + 0x1C);
    /* ASM_BARRIER: load second vector pair at +0x18 */
    __asm__ volatile("" : "+r"(t8), "+r"(a2), "+r"(a3), "+r"(a0), "+r"(t2));
    t0 = a2 >> 16;
    v0 = a2 << 16;
    /* ASM_BARRIER: unpack low half of a2 via v0 */
    __asm__ volatile("" : "+r"(t0), "+r"(v0));
    a2 = v0 >> 16;
    t1 = a0 >> 16;
    v0 = a0 << 16;
    /* ASM_BARRIER: unpack low half of a0 via v0 */
    __asm__ volatile("" : "+r"(a2), "+r"(t1), "+r"(v0));
    a0 = v0 >> 16;
    /* ASM_BARRIER: finish unpacking second vector pair */
    __asm__ volatile("" : "+r"(a0));
    RR_GTE_STORE_IR_S16(t8);
    RR_GTE_SETFC(a2, t0, a3);
    /* ASM_BARRIER: load IR1..IR3 and run INTPL */
    __asm__ volatile(
        "mtc2 %0, $9\n\t"
        "mtc2 %1, $10\n\t"
        "mtc2 %2, $11\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A980011"
        :
        : "r"(a0), "r"(t1), "r"(t2)
    );

    t8 = D_800ABED6;
    a2 = *(int *)(v1 + 0x20);
    a3 = *(short *)(v1 + 0x24);
    a0 = *(int *)(a1 + 0x20);
    t2 = *(short *)(a1 + 0x24);
    /* ASM_BARRIER: load third vector pair at +0x20 */
    __asm__ volatile("" : "+r"(t8), "+r"(a2), "+r"(a3), "+r"(a0), "+r"(t2));
    t0 = a2 >> 16;
    v0 = a2 << 16;
    /* ASM_BARRIER: unpack low half of a2 via v0 */
    __asm__ volatile("" : "+r"(t0), "+r"(v0));
    a2 = v0 >> 16;
    t1 = a0 >> 16;
    v0 = a0 << 16;
    /* ASM_BARRIER: unpack low half of a0 via v0 */
    __asm__ volatile("" : "+r"(a2), "+r"(t1), "+r"(v0));
    a0 = v0 >> 16;
    /* ASM_BARRIER: finish unpacking third vector pair */
    __asm__ volatile("" : "+r"(a0));
    RR_GTE_STORE_IR_S16(t8);
    RR_GTE_SETFC(a2, t0, a3);
    /* ASM_BARRIER: load IR1..IR3 and run INTPL */
    __asm__ volatile(
        "mtc2 %0, $9\n\t"
        "mtc2 %1, $10\n\t"
        "mtc2 %2, $11\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A980011"
        :
        : "r"(a0), "r"(t1), "r"(t2)
    );

    t8 = D_800ABEDC;
    a3 = *(int *)(v1 + 0x28);
    a0 = *(int *)(a1 + 0x28);
    /* ASM_BARRIER: load color pair at +0x28 */
    __asm__ volatile("" : "+r"(t8), "+r"(a3), "+r"(a0));
    v0 = a3 << 4;
    /* ASM_BARRIER: shift R via v0 */
    __asm__ volatile("" : "+r"(v0));
    a2 = v0 & 0xFF0;
    v0 = a3 >> 4;
    /* ASM_BARRIER: shift G via v0 */
    __asm__ volatile("" : "+r"(a2), "+r"(v0));
    t0 = v0 & 0xFF0;
    v0 = a3 >> 12;
    /* ASM_BARRIER: shift B via v0 */
    __asm__ volatile("" : "+r"(t0), "+r"(v0));
    a3 = v0 & 0xFF0;
    /* ASM_BARRIER: finish unpacking RGB at +0x28 */
    __asm__ volatile("" : "+r"(a3));
    RR_GTE_STORE_IR_S16(t8);
    RR_GTE_SETFC(a2, t0, a3);
    /* ASM_BARRIER: load RGB into $6 and run DPCS */
    __asm__ volatile(
        "mtc2 %0, $6\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A780010"
        :
        : "r"(a0)
    );

    a3 = *(int *)(v1 + 0x4);
    a0 = *(int *)(a1 + 0x4);
    /* ASM_BARRIER: load color pair at +0x4 */
    __asm__ volatile("" : "+r"(a3), "+r"(a0));
    v0 = a3 << 4;
    /* ASM_BARRIER: shift R via v0 */
    __asm__ volatile("" : "+r"(v0));
    a2 = v0 & 0xFF0;
    v0 = a3 >> 4;
    /* ASM_BARRIER: shift G via v0 */
    __asm__ volatile("" : "+r"(a2), "+r"(v0));
    t0 = v0 & 0xFF0;
    v0 = a3 >> 12;
    /* ASM_BARRIER: shift B via v0 */
    __asm__ volatile("" : "+r"(t0), "+r"(v0));
    a3 = v0 & 0xFF0;
    /* ASM_BARRIER: finish unpacking RGB at +0x4 */
    __asm__ volatile("" : "+r"(a3));
    /* ASM_BARRIER: transfer interpolated IR1..IR3 to BK ($13..$15) */
    __asm__ volatile(
        "mfc2 $12, $9\n\t"
        "mfc2 $13, $10\n\t"
        "mfc2 $14, $11\n\t"
        "ctc2 $12, $13\n\t"
        "ctc2 $13, $14\n\t"
        "ctc2 $14, $15"
        :
        :
        : "$12", "$13", "$14"
    );
    RR_GTE_SETFC(a2, t0, a3);
    /* ASM_BARRIER: load RGB into $6 and run DPCS */
    __asm__ volatile(
        "mtc2 %0, $6\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A780010"
        :
        : "r"(a0)
    );

    t8 = (short *)0x1F8000B4;
    a3 = *(int *)(v1 + 0x8);
    a0 = *(int *)(a1 + 0x8);
    /* ASM_BARRIER: load color pair at +0x8 */
    __asm__ volatile("" : "+r"(t8), "+r"(a3), "+r"(a0));
    v0 = a3 << 4;
    /* ASM_BARRIER: shift R via v0 */
    __asm__ volatile("" : "+r"(v0));
    a2 = v0 & 0xFF0;
    v0 = a3 >> 4;
    /* ASM_BARRIER: shift G via v0 */
    __asm__ volatile("" : "+r"(a2), "+r"(v0));
    t0 = v0 & 0xFF0;
    v0 = a3 >> 12;
    /* ASM_BARRIER: shift B via v0 */
    __asm__ volatile("" : "+r"(t0), "+r"(v0));
    a3 = v0 & 0xFF0;
    /* ASM_BARRIER: finish unpacking RGB at +0x8 */
    __asm__ volatile("" : "+r"(a3));
    RR_GTE_ST3_COL(t8);
    RR_GTE_SETFC(a2, t0, a3);
    /* ASM_BARRIER: load RGB into $6 and run DPCS */
    __asm__ volatile(
        "mtc2 %0, $6\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A780010"
        :
        : "r"(a0)
    );

    t8 = (short *)0x1F8000B6;
    a3 = *(int *)(v1 + 0xC);
    a0 = *(int *)(a1 + 0xC);
    /* ASM_BARRIER: load color pair at +0xC */
    __asm__ volatile("" : "+r"(t8), "+r"(a3), "+r"(a0));
    v0 = a3 << 4;
    /* ASM_BARRIER: shift R via v0 */
    __asm__ volatile("" : "+r"(v0));
    a2 = v0 & 0xFF0;
    v0 = a3 >> 4;
    /* ASM_BARRIER: shift G via v0 */
    __asm__ volatile("" : "+r"(a2), "+r"(v0));
    t0 = v0 & 0xFF0;
    v0 = a3 >> 12;
    /* ASM_BARRIER: shift B via v0 */
    __asm__ volatile("" : "+r"(t0), "+r"(v0));
    a3 = v0 & 0xFF0;
    /* ASM_BARRIER: finish unpacking RGB at +0xC */
    __asm__ volatile("" : "+r"(a3));
    RR_GTE_ST3_COL(t8);
    RR_GTE_SETFC(a2, t0, a3);
    /* ASM_BARRIER: load RGB into $6 and run DPCS */
    __asm__ volatile(
        "mtc2 %0, $6\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A780010"
        :
        : "r"(a0)
    );

    t8 = (short *)0x1F8000B8;
    /* ASM_BARRIER: stage 0x1F8000B8 in $t8 */
    __asm__ volatile("" : "+r"(t8));
    RR_GTE_ST3_COL(t8);
    t8 = (short *)0x1F8000B4;
    /* ASM_BARRIER: stage 0x1F8000B4 in $t8 */
    __asm__ volatile("" : "+r"(t8));
    RR_GTE_SETLCM(t8);
    return t7[1];
}

#undef func_8001292C
#undef func_80012A58
#undef func_80012A68
#undef func_80070108
#undef D_800ACC18
#undef D_800F3A08
#undef D_8010BF60

#include "types.h"
#include "rr/renderer_setup.h"
#include "rr_asm.h"

typedef struct {
    s32 header;
    s32 p4, p8, pC, p10, p14;
} Resource;

s32 func_8001292C(s32, void *);
s32 func_80012A58(void);
void func_80012A68(void *);
void func_80012AB4(void *, s32);
void func_80012C7C(void *, s32);
void func_80012E94(s32, s32, s32, s32, s32);
void func_8001301C(s32, s32);
void func_800293AC(void *, s32);
void func_80070108(void *, s32);
void func_80092D50(s32);
void func_80092FFC(s16 *, void *);
extern s32 D_8009DF98;
extern Resource *D_800ACC18;
extern void *D_800F3A08[];
extern s32 D_80109440;
extern s32 D_80109444;
extern s32 D_80109448;
extern s32 D_8010944C;
extern s32 D_80109450;
extern s32 D_80109454;
extern s32 D_80109468[];
extern int func_8001223C();
extern short *D_8010BF60;
extern short D_800F3914;
extern short D_8009E02C[];
extern char *D_800F4D90;
extern short D_800ABED0[];
extern short D_800ABED6[];
extern short D_800ABEDC[];

s32 func_800135BC(void);
int func_800137B0(void);
int func_800137D4(void);
void func_80013890(s32 shift);
int func_80013904(char *a0_in, int a1_in);

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

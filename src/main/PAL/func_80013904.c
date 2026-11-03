#include "rr_asm.h"

extern short *D_8010BF60;
extern short D_800F3914;
extern short D_8009E02C[];
extern char *D_800F4D90;
extern short D_800ABED0[];
extern short D_800ABED6[];
extern short D_800ABEDC[];

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

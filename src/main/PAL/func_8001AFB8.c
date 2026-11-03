#include "rr_asm.h"

extern int D_8009E46C[];
extern int D_800ABEB0;

void func_8001AFB8(int a0, int a1) {
    /* ASM_BARRIER: keep scratch pointer 0x1F80007C in $a2 */
    register int *m __asm__("$6") = (int *)0x1F80007C;
    /* ASM_BARRIER: keep 0x1F800000 base / index temp in $a1 */
    register int r_a1 __asm__("$5");
    /* ASM_BARRIER: keep temp/count/vertex ptr in $a0 */
    register int r_a0 __asm__("$4");
    /* ASM_BARRIER: keep temp in $v0 */
    register int r_v0 __asm__("$2");
    /* ASM_BARRIER: keep temp in $v1 */
    register int r_v1 __asm__("$3");
    /* ASM_BARRIER: keep fourth vertex index/sxy2 in $a3 */
    register int r_a3 __asm__("$7");
    /* ASM_BARRIER: keep GTE IR3 / otz index in $t3 */
    register int t3 __asm__("$11");
    /* ASM_BARRIER: keep loop counter in $t8 */
    register int t8 __asm__("$24");
    /* ASM_BARRIER: keep vertex base pointer in $t9 */
    register char *t9 __asm__("$25");
    /* ASM_BARRIER: keep ordering table base in $s0 */
    register int s0 __asm__("$16");
    /* ASM_BARRIER: keep OT shift + 2 in $s2 */
    register int s2 __asm__("$18");
    char *t2, *t7;
    int x0, x1, x2, x3, y0, y1, y2, y3, cval;

    m[0x54 / 4] = a1;
    r_a1 = 0x1F800000;
    /* ASM_BARRIER: load 0x1F800000 into $a1 after storing a1 */
    __asm__ volatile("" : "+r"(r_a1));
    r_v0 = a0 * 12;
    r_v1 = ((int *)r_a1)[0x48 / 4];
    t7 = (char *)((int *)r_a1)[0];
    /* ASM_BARRIER: load 0x48($a1) and 0($a1) before subtracting 4 */
    __asm__ volatile("" : "+r"(r_v1), "+r"(t7), "+r"(r_v0));
    r_v1 -= 4;
    /* ASM_BARRIER: subtract 4 before adding a0*12 */
    __asm__ volatile("" : "+r"(r_v1));
    r_v1 += r_v0;
    t2 = *(char **)(r_v1 + 0xC);
    t9 = *(char **)(r_v1 + 0x4);
    t8 = *(unsigned short *)(t2 + 2);
    t2 += 4;
    /* ASM_BARRIER: advance t2 by 4 before reading +0x1C and +0x1A */
    __asm__ volatile("" : "+r"(t2), "+r"(t8));
    r_v0 = *(int *)(t2 + 0x1C);
    r_v1 = *(unsigned short *)(t2 + 0x1A);
    /* ASM_BARRIER: load +0x1C and +0x1A before copying t8 to $a0 */
    __asm__ volatile("" : "+r"(r_v0), "+r"(r_v1));
    r_a0 = t8;
    t8--;
    /* ASM_BARRIER: copy and decrement t8 before storing to m */
    __asm__ volatile("" : "+r"(r_a0), "+r"(t8));
    m[0] = r_v0;
    *(short *)((char *)m + 4) = r_v1;
    r_v0 = ((int *)r_a1)[1];
    r_v1 = ((int *)r_a1)[0x60 / 4];
    /* ASM_BARRIER: load 4($a1) and 0x60($a1) before computing s0 and s2 */
    __asm__ volatile("" : "+r"(r_v0), "+r"(r_v1));
    s0 = r_v0 + 0x200;
    s2 = r_v1 + 2;
    if (r_a0 > 0) {
        do {
            /* ASM_BARRIER: load V0 from $a2 and run GTE MVMVA (0x4A4A2012) */
            __asm__ volatile ("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(m));
            /* ASM_BARRIER: two delay slot nops before GTE MVMVA */
            __asm__ volatile ("nop; nop");
            /* ASM_BARRIER: GTE MVMVA 1, 1, 0, 1, 0 (0x4A4A2012) */
            __asm__ volatile (".word 0x4A4A2012");

            r_v0 = *(int *)(t2 + 0x3C);
            r_v1 = *(unsigned short *)(t2 + 0x3A);
            /* ASM_BARRIER: load next normal before storing to m */
            __asm__ volatile("" : "+r"(r_v0), "+r"(r_v1));
            m[0] = r_v0;
            *(short *)((char *)m + 4) = r_v1;

            /* ASM_BARRIER: read GTE IR3 into $t3 */
            __asm__ volatile ("mfc2 %0, $11" : "=r"(t3));
            /* ASM_BARRIER: mfc2 load delay slot */
            RR_NOP_DEP_ON(t3);
            if (t3 > 0) {
                /* ASM_BARRIER: keep blez delay slot as nop before srav */
                __asm__ volatile("");
                t3 >>= s2;
                if (t3 < 0x1C0) {
                    m[0x50 / 4] = t3;
                    RR_GTE_TR_FROM_IR();

                    r_v0 = *(int *)t2;
                    r_a1 = *(int *)(t2 + 0x04);
                    /* ASM_BARRIER: load packed vertex indices before unpacking */
                    __asm__ volatile("" : "+r"(r_v0), "+r"(r_a1));
                    r_v1 = r_v0 >> 16;
                    r_a3 = r_a1 >> 16;
                    r_v0 &= 0xFFFF;
                    r_a1 &= 0xFFFF;
                    /* ASM_BARRIER: order index shifts and masks before address math */
                    __asm__ volatile("" : "+r"(r_v1), "+r"(r_a3), "+r"(r_v0), "+r"(r_a1));
                    r_a0 = r_v0 << 3;
                    /* ASM_BARRIER: shift first vertex index into $a0 before adding t9 */
                    __asm__ volatile("" : "+r"(r_a0));
                    r_a0 = (int)(t9 + r_a0);
                    /* ASM_BARRIER: compute first vertex address in $a0 */
                    __asm__ volatile("" : "+r"(r_a0));
                    r_v1 = (int)(t9 + (r_v1 << 3));
                    /* ASM_BARRIER: compute second vertex address in $v1 */
                    __asm__ volatile("" : "+r"(r_v1));
                    r_v0 = (int)(t9 + (r_a1 << 3));
                    /* ASM_BARRIER: compute third vertex address in $v0 */
                    __asm__ volatile("" : "+r"(r_v0));
                    RR_GTE_RTPT_LDV(r_a0, r_v1, r_v0);

                    r_v0 = *(int *)(t2 + 0x0C);
                    r_v1 = *(int *)(t2 + 0x10);
                    /* ASM_BARRIER: load first two UV words before storing */
                    __asm__ volatile("" : "+r"(r_v0), "+r"(r_v1));
                    *(int *)(t7 + 0x0C) = r_v0;
                    *(int *)(t7 + 0x14) = r_v1;
                    r_v0 = *(int *)(t2 + 0x14);
                    /* ASM_BARRIER: load third UV word before shifting */
                    __asm__ volatile("" : "+r"(r_v0));
                    r_v1 = r_v0 >> 16;
                    /* ASM_BARRIER: shift before storing +0x19 */
                    __asm__ volatile("" : "+r"(r_v1));
                    *(int *)(t7 + 0x1C) = r_v0;
                    r_v0 = (int)((char *)m + 0x58);
                    /* ASM_BARRIER: stage m+0x58 in $v0 before storing +0x21 */
                    __asm__ volatile("" : "+r"(r_v0));
                    *(int *)(t7 + 0x24) = r_v1;
                    RR_GTE_SWC2_SXYZ(r_v0);

                    r_v0 = (int)(t9 + (r_a3 << 3));
                    RR_GTE_RTPS_LDV(r_v0);

                    if (m[0x54 / 4] != 0) {
                        cval = D_8009E46C[(D_800ABEB0 + ({
                            /* ASM_BARRIER: extend %hi(D_800ABEB0) lifetime for loop invariant hoisting */
                            __asm__ volatile("");
                            /* ASM_BARRIER: extend %hi(D_800ABEB0) lifetime for loop invariant hoisting */
                            __asm__ volatile("");
                            /* ASM_BARRIER: extend %hi(D_800ABEB0) lifetime for loop invariant hoisting */
                            __asm__ volatile("");
                            /* ASM_BARRIER: extend %hi(D_800ABEB0) lifetime for loop invariant hoisting */
                            __asm__ volatile("");
                            /* ASM_BARRIER: extend %hi(D_800ABEB0) lifetime for loop invariant hoisting */
                            __asm__ volatile("");
                            *(signed char *)(t2 + 0x18);
                        })) & 0x1F];
                    } else {
                        cval = D_8009E46C[0];
                    }
                    r_v0 = cval;

                    r_v1 = m[0x58 / 4];
                    r_a1 = m[0x5C / 4];
                    r_a3 = m[0x60 / 4];
                    /* ASM_BARRIER: load SXY0..SXY2 before storing to packet */
                    __asm__ volatile("" : "+r"(r_v0), "+r"(r_v1), "+r"(r_a1), "+r"(r_a3));
                    *(int *)(t7 + 0x04) = r_v0;
                    r_v0 = (int)((char *)m + 0x64);
                    /* ASM_BARRIER: stage m+0x64 in $v0 before storing SXY0..SXY2 */
                    __asm__ volatile("" : "+r"(r_v0));
                    *(int *)(t7 + 0x08) = r_v1;
                    *(int *)(t7 + 0x10) = r_a1;
                    *(int *)(t7 + 0x18) = r_a3;
                    RR_GTE_SWC2_SXY2(r_v0);
                    r_v0 = m[0x64 / 4];
                    *(int *)(t7 + 0x20) = r_v0;

                    x0 = *(short *)((char *)m + 0x58);
                    x1 = *(short *)((char *)m + 0x5C);
                    x2 = *(short *)((char *)m + 0x60);
                    x3 = *(short *)((char *)m + 0x64);
                    if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                        (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                        y0 = *(short *)((char *)m + 0x5A);
                        y1 = *(short *)((char *)m + 0x5E);
                        y2 = *(short *)((char *)m + 0x62);
                        y3 = *(short *)((char *)m + 0x66);
                        if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                            (y0 < 0x100 || y1 < 0x100 || y2 < 0x100 || y3 < 0x100)) {
                            *(int *)t7 = *(int *)((*(signed char *)(t2 + 0x19) << 2) + ((*(volatile int *)((char *)m + 0x50) << 2) + s0));
                            *(char *)(t7 + 3) = 9;

                            *(int *)((*(signed char *)(t2 + 0x19) << 2) + ((*(volatile int *)((char *)m + 0x50) << 2) + s0)) = (int)t7;
                            t7 += 0x28;

                            *(char *)((*(signed char *)(t2 + 0x19) << 2) + ((*(volatile int *)((char *)m + 0x50) << 2) + s0) + 3) = 0;
                        }
                    }
                }
            }
            t2 += 0x20;
        } while (t8-- > 0);
    }
    *(int *)0x1F800000 = (int)t7;
}

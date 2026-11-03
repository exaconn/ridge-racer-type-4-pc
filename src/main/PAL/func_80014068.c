#include "rr_asm.h"

extern int D_800F2EC8;

void *func_80014068(char *a0_in, int *a1_in, int a2_in) {
    /* ASM_BARRIER: keep temp in $v0 */
    register int r_v0 __asm__("$2");
    /* ASM_BARRIER: keep temp in $v1 */
    register int r_v1 __asm__("$3");
    /* ASM_BARRIER: keep a0_in / temp in $a0 */
    register int r_a0 __asm__("$4") = (int)a0_in;
    /* ASM_BARRIER: keep a1_in / temp in $a1 */
    register int r_a1 __asm__("$5") = (int)a1_in;
    /* ASM_BARRIER: keep loop counter in $a2 */
    register int r_a2 __asm__("$6") = a2_in;
    /* ASM_BARRIER: keep temp in $a3 */
    register int r_a3 __asm__("$7");
    /* ASM_BARRIER: keep temp in $t0 */
    register int t0 __asm__("$8");
    /* ASM_BARRIER: keep temp in $t1 */
    register int t1 __asm__("$9");
    /* ASM_BARRIER: keep context pointer in $t2 */
    register int *t2 __asm__("$10");
    /* ASM_BARRIER: keep packet+0x20 cursor in $t3 */
    register char *t3 __asm__("$11");
    /* ASM_BARRIER: keep fourth vertex/normal index in $t4 */
    register int t4 __asm__("$12");
    /* ASM_BARRIER: keep poly+0x15 cursor in $t5 */
    register char *t5 __asm__("$13");
    /* ASM_BARRIER: keep packet cursor in $t6 */
    register char *t6 __asm__("$14");
    /* ASM_BARRIER: keep vertex table base in $t7 */
    register int t7 __asm__("$15");
    /* ASM_BARRIER: keep poly cursor in $t8 */
    register char *t8 __asm__("$24");
    /* ASM_BARRIER: keep 0xE6000000 constant in $t9 */
    register int t9 __asm__("$25");
    /* ASM_BARRIER: keep OT shift in $s0 */
    register int s0 __asm__("$16");
    int x0, x1, x2, x3, y0, y1, y2, y3;

    t8 = (char *)r_a0;
    t2 = (int *)r_a1;
    t9 = 0xE6000000;
    /* ASM_BARRIER: order t8, t2, t9 setup before initial loads */
    __asm__ volatile("" : "+r"(t8), "+r"(t2), "+r"(t9));
    r_v0 = *(int *)(t8 + 0);
    t0 = *(int *)(t8 + 4);
    t7 = t2[0];
    /* ASM_BARRIER: order initial loads before index shifts */
    __asm__ volatile("" : "+r"(r_v0), "+r"(t0), "+r"(t7));
    r_v1 = r_v0 >> 16;
    t4 = t0 >> 16;
    r_v0 &= 0xFFFF;
    /* ASM_BARRIER: order shifts and mask before first vertex address */
    __asm__ volatile("" : "+r"(r_v1), "+r"(t4), "+r"(r_v0));
    r_v0 = (r_v0 << 3) + t7;
    /* ASM_BARRIER: compute first vertex pointer in $v0 */
    __asm__ volatile("" : "+r"(r_v0));
    r_a1 = *(int *)(r_v0 + 0);
    r_a3 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load first vertex words before second vertex address */
    __asm__ volatile("" : "+r"(r_a1), "+r"(r_a3));
    r_v0 = (r_v1 << 3) + t7;
    t0 &= 0xFFFF;
    /* ASM_BARRIER: compute second vertex pointer and mask t0 */
    __asm__ volatile("" : "+r"(r_v0), "+r"(t0));
    t1 = *(int *)(r_v0 + 0);
    r_v0 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load second vertex words before copying a2 to v1 */
    __asm__ volatile("" : "+r"(t1), "+r"(r_v0));
    r_v1 = r_a2;
    /* ASM_BARRIER: copy a2 in load delay slot before storing 0x34 */
    __asm__ volatile("" : "+r"(r_v1));
    t2[0x34 / 4] = r_v0;
    r_v0 = (t0 << 3) + t7;
    /* ASM_BARRIER: compute third vertex pointer before storing 0x28..0x30 */
    __asm__ volatile("" : "+r"(r_v0));
    t2[0x28 / 4] = r_a1;
    t2[0x2C / 4] = r_a3;
    t2[0x30 / 4] = t1;
    r_a1 = *(int *)(r_v0 + 0);
    r_a3 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load third vertex words before fourth vertex address */
    __asm__ volatile("" : "+r"(r_a1), "+r"(r_a3));
    r_v0 = (t4 << 3) + t7;
    /* ASM_BARRIER: compute fourth vertex pointer in $v0 */
    __asm__ volatile("" : "+r"(r_v0));
    t1 = *(int *)(r_v0 + 0);
    r_v0 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load fourth vertex words before storing 0x44 */
    __asm__ volatile("" : "+r"(t1), "+r"(r_v0));
    t2[0x44 / 4] = r_v0;
    r_v0 = 0x1F800000;
    /* ASM_BARRIER: load 0x1F800000 into $v0 before storing 0x38..0x40 */
    __asm__ volatile("" : "+r"(r_v0));
    t2[0x38 / 4] = r_a1;
    t2[0x3C / 4] = r_a3;
    t2[0x40 / 4] = t1;
    t6 = *(char **)(r_v0 + 0x0);
    s0 = *(int *)(r_v0 + 0x60);
    /* ASM_BARRIER: load scratchpad fields before decrementing a2 and branching */
    __asm__ volatile("" : "+r"(t6), "+r"(s0), "+r"(r_v1));
    r_a2--;
    if (r_v1 > 0) {
        t5 = (char *)r_a0 + 0x15;
        t3 = t6 + 0x20;
        do {
            /* ASM_BARRIER: keep loop pointers live at top of loop */
            __asm__ volatile("" : "+r"(t5), "+r"(t3), "+r"(t6), "+r"(t8));
            r_v0 = (int)((char *)t2 + 0x28);
            RR_GTE14_LWC2_V012(r_v0);
            RR_GTE14_NOP2();
            RR_GTE14_RTPT();

            r_v0 = *(int *)(t5 + 3);
            t0 = *(int *)(t5 + 7);
            /* ASM_BARRIER: load next packed indices before shifts */
            __asm__ volatile("" : "+r"(r_v0), "+r"(t0));
            r_v1 = r_v0 >> 16;
            t4 = t0 >> 16;
            r_v0 &= 0xFFFF;
            /* ASM_BARRIER: order shifts and mask before first vertex address */
            __asm__ volatile("" : "+r"(r_v1), "+r"(t4), "+r"(r_v0));
            r_v0 = (r_v0 << 3) + t7;
            /* ASM_BARRIER: compute first vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_a1 = *(int *)(r_v0 + 0);
            r_a3 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load first vertex words before masking t0 */
            __asm__ volatile("" : "+r"(r_a1), "+r"(r_a3));
            t0 &= 0xFFFF;
            /* ASM_BARRIER: mask t0 before storing 0x28..0x2C */
            __asm__ volatile("" : "+r"(t0));
            t2[0x28 / 4] = r_a1;
            t2[0x2C / 4] = r_a3;
            RR_GTE14_NCLIP();

            r_v0 = (r_v1 << 3) + t7;
            /* ASM_BARRIER: compute second vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_a1 = *(int *)(r_v0 + 0);
            r_a3 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load second vertex words before storing MAC0/SXYZ and loading V0 */
            __asm__ volatile("" : "+r"(r_a1), "+r"(r_a3));
            r_v0 = (int)((char *)t2 + 0x1C);
            RR_GTE14_SWC2_MAC0(r_v0);
            r_v0 = (int)((char *)t2 + 0x50);
            RR_GTE14_SWC2_SXYZ(r_v0);
            r_v0 = (int)((char *)t2 + 0x40);
            RR_GTE14_LWC2_V0(r_v0);
            t2[0x30 / 4] = r_a1;
            t2[0x34 / 4] = r_a3;
            RR_GTE14_RTPS();

            r_v0 = (t0 << 3) + t7;
            /* ASM_BARRIER: compute third vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_a1 = *(int *)(r_v0 + 0);
            r_a3 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load third vertex words before storing 0x38..0x3C */
            __asm__ volatile("" : "+r"(r_a1), "+r"(r_a3));
            t2[0x38 / 4] = r_a1;
            t2[0x3C / 4] = r_a3;
            RR_GTE14_AVSZ4();

            r_v0 = (t4 << 3) + t7;
            /* ASM_BARRIER: compute fourth vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_a1 = *(int *)(r_v0 + 0);
            r_a3 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load fourth vertex words before storing OTZ */
            __asm__ volatile("" : "+r"(r_a1), "+r"(r_a3));
            r_v0 = (int)((char *)t2 + 0x18);
            RR_GTE14_SWC2_OTZ(r_v0);
            r_v0 = t2[0x18 / 4];
            /* ASM_BARRIER: load OTZ before storing 0x40..0x44 */
            __asm__ volatile("" : "+r"(r_v0));
            t2[0x40 / 4] = r_a1;
            t2[0x44 / 4] = r_a3;
            if (r_v0 > 0) {
                r_v0 >>= s0;
                t2[0x18 / 4] = r_v0;
                if (r_v0 < 0x1C0) {
                    RR_GTE14_NCLIP();
                    r_a1 = t2[0x50 / 4];
                    r_a3 = t2[0x54 / 4];
                    t1 = t2[0x58 / 4];
                    /* ASM_BARRIER: load SXY0..SXY2 before staging t2+0x20 */
                    __asm__ volatile("" : "+r"(r_a1), "+r"(r_a3), "+r"(t1));
                    r_v0 = (int)((char *)t2 + 0x20);
                    /* ASM_BARRIER: stage t2+0x20 before stores */
                    __asm__ volatile("" : "+r"(r_v0));
                    *(int *)(t3 - 0x14) = r_a1;
                    *(int *)(t3 - 0x0C) = r_a3;
                    *(int *)(t3 - 0x04) = t1;
                    RR_GTE14_SWC2_MAC0(r_v0);
                    if (t2[0x1C / 4] > 0 || t2[0x20 / 4] < 0) {
                        RR_GTE14_SWC2_SXY2((char *)t2 + 0x5C);
                        r_v0 = t2[0x5C / 4];
                        *(int *)(t3 + 0x04) = r_v0;

                        x0 = *(short *)((char *)t2 + 0x50);
                        x1 = *(short *)((char *)t2 + 0x54);
                        x2 = *(short *)((char *)t2 + 0x58);
                        x3 = *(short *)((char *)t2 + 0x5C);
                        if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                            (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                            y0 = *(short *)((char *)t2 + 0x52);
                            y1 = *(short *)((char *)t2 + 0x56);
                            y2 = *(short *)((char *)t2 + 0x5A);
                            y3 = *(short *)((char *)t2 + 0x5E);
                            if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                                (y0 < 0x200 || y1 < 0x200 || y2 < 0x200 || y3 < 0x200)) {
                                r_v0 = *(int *)(t5 - 0x0D);
                                t0 = *(int *)(t5 - 0x09);
                                r_a1 = t2[1];
                                /* ASM_BARRIER: load packed normal indices and normal table base before shifts */
                                __asm__ volatile("" : "+r"(r_v0), "+r"(t0), "+r"(r_a1));
                                r_v1 = r_v0 >> 16;
                                t4 = t0 >> 16;
                                r_v0 &= 0xFFFF;
                                t0 &= 0xFFFF;
                                /* ASM_BARRIER: order index shifts and masks before address math */
                                __asm__ volatile("" : "+r"(r_v1), "+r"(t4), "+r"(r_v0), "+r"(t0));
                                r_a0 = r_v0 << 3;
                                /* ASM_BARRIER: shift first normal index before adding base */
                                __asm__ volatile("" : "+r"(r_a0));
                                r_a0 = r_a1 + r_a0;
                                /* ASM_BARRIER: compute first normal address in $a0 */
                                __asm__ volatile("" : "+r"(r_a0));
                                r_v1 = r_a1 + (r_v1 << 3);
                                /* ASM_BARRIER: compute second normal address in $v1 */
                                __asm__ volatile("" : "+r"(r_v1));
                                r_v0 = r_a1 + (t0 << 3);
                                /* ASM_BARRIER: compute third normal address in $v0 */
                                __asm__ volatile("" : "+r"(r_v0));
                                RR_GTE14_LWC2_V0_V1_V2(r_a0, r_v1, r_v0);
                                r_v0 = (int)(t8 + 0x10);
                                RR_GTE14_LWC2_RGB(r_v0);
                                RR_GTE14_NOP2();
                                RR_GTE14_LIGHT0();

                                r_v0 = (t4 << 3) + r_a1;
                                /* ASM_BARRIER: compute fourth normal address in $v0 */
                                __asm__ volatile("" : "+r"(r_v0));
                                r_a1 = *(int *)(r_v0 + 0);
                                r_a3 = *(int *)(r_v0 + 4);
                                /* ASM_BARRIER: load fourth normal words before storing 0x48..0x4C */
                                __asm__ volatile("" : "+r"(r_a1), "+r"(r_a3));
                                t2[0x48 / 4] = r_a1;
                                t2[0x4C / 4] = r_a3;
                                RR_GTE14_LIGHT1();

                                r_a0 = t2[0x18 / 4];
                                r_v1 = t2[0x14 / 4];
                                r_v0 = *(signed char *)t5;
                                /* ASM_BARRIER: load otz, ot base, and offset byte before shifts */
                                __asm__ volatile("" : "+r"(r_a0), "+r"(r_v1), "+r"(r_v0));
                                r_a0 <<= 2;
                                r_v1 += r_a0;
                                r_v0 <<= 2;
                                r_v1 += r_v0;
                                /* ASM_BARRIER: keep OT entry address in $v1 */
                                __asm__ volatile("" : "+r"(r_v1));
                                r_v0 = *(int *)r_v1;
                                /* ASM_BARRIER: load-use delay on *(int *)r_v1 */
                                __asm__ volatile("" : "+r"(r_v0));
                                *(int *)t6 = r_v0;
                                r_v0 = 0xA;
                                /* ASM_BARRIER: keep 0xA in $v0 before storing -0x1D(t3) */
                                __asm__ volatile("" : "+r"(r_v0));
                                *(char *)(t3 - 0x1D) = r_v0;
                                /* ASM_BARRIER: store -0x1D(t3) before OT stores and D_800F2EC8 load */
                                __asm__ volatile("" : "+r"(r_v1));
                                *(int *)r_v1 = (int)t6;
                                *(char *)(r_v1 + 3) = 0;
                                r_v1 = D_800F2EC8;
                                /* ASM_BARRIER: load D_800F2EC8 into $v1 before computing t9|1 in load delay slot */
                                __asm__ volatile("" : "+r"(r_v1));
                                r_v0 = t9 | 1;
                                *(int *)(t3 - 0x1C) = r_v0;
                                /* ASM_BARRIER: store -0x1C(t3) before computing t9|r_v1 */
                                __asm__ volatile("" : "+r"(r_v1));
                                r_v1 = t9 | r_v1;
                                *(int *)(t3 + 0x08) = r_v1;
                                RR_GTE14_LIGHT2();
                                r_v0 = (int)(t6 + 4);
                                RR_GTE14_SWC2_RGB012(r_v0);
                                r_v0 = (int)((char *)t2 + 0x48);
                                RR_GTE14_LWC2_V0(r_v0);
                                RR_GTE14_NOP2();
                                RR_GTE14_LIGHT0();
                                RR_GTE14_SWC2_RGB2(t3);
                                t3 += 0x2C;
                                t6 += 0x2C;
                            }
                        }
                    }
                }
            }
            t5 += 0x18;
            t8 += 0x18;
        } while (r_a2-- > 0);
    }
    *(char **)0x1F800000 = t6;
    return t8;
}

#include "rr_asm.h"

void *func_80013D48(char *a0_in, int *a1_in, int a2_in) {
    /* ASM_BARRIER: keep temp in $v0 */
    register int r_v0 __asm__("$2");
    /* ASM_BARRIER: keep temp in $v1 */
    register int r_v1 __asm__("$3");
    /* ASM_BARRIER: keep a0_in / temp in $a0 */
    register int r_a0 __asm__("$4") = (int)a0_in;
    /* ASM_BARRIER: keep context pointer in $a1 */
    register int *r_a1 __asm__("$5") = a1_in;
    /* ASM_BARRIER: keep loop counter in $a2 */
    register int r_a2 __asm__("$6") = a2_in;
    /* ASM_BARRIER: keep temp in $a3 */
    register int r_a3 __asm__("$7");
    /* ASM_BARRIER: keep temp in $t0 */
    register int t0 __asm__("$8");
    /* ASM_BARRIER: keep temp in $t1 */
    register int t1 __asm__("$9");
    /* ASM_BARRIER: keep packet+3 cursor in $t2 */
    register char *t2 __asm__("$10");
    /* ASM_BARRIER: keep poly+0x15 cursor in $t3 */
    register char *t3 __asm__("$11");
    /* ASM_BARRIER: keep packet cursor in $t4 */
    register char *t4 __asm__("$12");
    /* ASM_BARRIER: keep vertex table base in $t5 */
    register int t5 __asm__("$13");
    /* ASM_BARRIER: keep fourth vertex index in $t6 */
    register int t6 __asm__("$14");
    /* ASM_BARRIER: keep poly cursor in $t7 */
    register char *t7 __asm__("$15");
    /* ASM_BARRIER: keep OT shift in $t8 */
    register int t8 __asm__("$24");
    /* ASM_BARRIER: keep 0x2F000000 constant in $t9 */
    register int t9 __asm__("$25");
    int x0, x1, x2, x3, y0, y1, y2, y3;

    t7 = (char *)r_a0;
    t9 = 0x2F000000;
    /* ASM_BARRIER: order t7 and t9 initialization before loads */
    __asm__ volatile("" : "+r"(t7), "+r"(t9));
    r_v0 = *(int *)(t7 + 0);
    r_v1 = *(int *)(t7 + 4);
    t5 = r_a1[0];
    /* ASM_BARRIER: order initial loads before index shifts */
    __asm__ volatile("" : "+r"(r_v0), "+r"(r_v1), "+r"(t5));
    t1 = r_v0 >> 16;
    t6 = r_v1 >> 16;
    r_v0 &= 0xFFFF;
    /* ASM_BARRIER: order shifts and mask before first vertex address */
    __asm__ volatile("" : "+r"(t1), "+r"(t6), "+r"(r_v0));
    r_v0 = (r_v0 << 3) + t5;
    /* ASM_BARRIER: compute first vertex pointer in $v0 */
    __asm__ volatile("" : "+r"(r_v0));
    r_a3 = *(int *)(r_v0 + 0);
    t0 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load first vertex words before second vertex address */
    __asm__ volatile("" : "+r"(r_a3), "+r"(t0));
    r_v0 = (t1 << 3) + t5;
    /* ASM_BARRIER: compute second vertex pointer in $v0 */
    __asm__ volatile("" : "+r"(r_v0));
    t1 = *(int *)(r_v0 + 0);
    r_v0 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load second vertex words before masking v1 */
    __asm__ volatile("" : "+r"(t1), "+r"(r_v0));
    r_v1 &= 0xFFFF;
    /* ASM_BARRIER: mask v1 in load delay slot before storing 0x34 */
    __asm__ volatile("" : "+r"(r_v1));
    r_a1[0x34 / 4] = r_v0;
    r_v0 = (r_v1 << 3) + t5;
    /* ASM_BARRIER: compute third vertex pointer before storing 0x28..0x30 */
    __asm__ volatile("" : "+r"(r_v0));
    r_a1[0x28 / 4] = r_a3;
    r_a1[0x2C / 4] = t0;
    r_a1[0x30 / 4] = t1;
    r_a3 = *(int *)(r_v0 + 0);
    t0 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load third vertex words before fourth vertex address */
    __asm__ volatile("" : "+r"(r_a3), "+r"(t0));
    r_v0 = (t6 << 3) + t5;
    r_v1 = r_a2;
    /* ASM_BARRIER: compute fourth vertex pointer and copy a2 to v1 */
    __asm__ volatile("" : "+r"(r_v0), "+r"(r_v1));
    t1 = *(int *)(r_v0 + 0);
    r_v0 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load fourth vertex words before storing 0x44 */
    __asm__ volatile("" : "+r"(t1), "+r"(r_v0));
    r_a1[0x44 / 4] = r_v0;
    r_v0 = 0x1F800000;
    /* ASM_BARRIER: load 0x1F800000 into $v0 before storing 0x38..0x40 */
    __asm__ volatile("" : "+r"(r_v0));
    r_a1[0x38 / 4] = r_a3;
    r_a1[0x3C / 4] = t0;
    r_a1[0x40 / 4] = t1;
    t4 = *(char **)(r_v0 + 0x0);
    t8 = *(int *)(r_v0 + 0x60);
    /* ASM_BARRIER: load scratchpad fields before decrementing a2 and branching */
    __asm__ volatile("" : "+r"(t4), "+r"(t8), "+r"(r_v1));
    r_a2--;
    if (r_v1 > 0) {
        t3 = (char *)r_a0 + 0x15;
        t2 = t4 + 3;
        do {
            /* ASM_BARRIER: keep loop pointers live at top of loop */
            __asm__ volatile("" : "+r"(t3), "+r"(t2), "+r"(t4), "+r"(t7));
            r_v0 = (int)((char *)r_a1 + 0x28);
            RR_GTE14_LWC2_V012(r_v0);
            RR_GTE14_NOP2();
            RR_GTE14_RTPT();

            r_v0 = *(int *)(t3 + 3);
            r_v1 = *(int *)(t3 + 7);
            /* ASM_BARRIER: load next packed indices before shifts */
            __asm__ volatile("" : "+r"(r_v0), "+r"(r_v1));
            t1 = r_v0 >> 16;
            t6 = r_v1 >> 16;
            r_v0 &= 0xFFFF;
            /* ASM_BARRIER: order shifts and mask before first vertex address */
            __asm__ volatile("" : "+r"(t1), "+r"(t6), "+r"(r_v0));
            r_v0 = (r_v0 << 3) + t5;
            /* ASM_BARRIER: compute first vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_a3 = *(int *)(r_v0 + 0);
            t0 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load first vertex words before masking v1 */
            __asm__ volatile("" : "+r"(r_a3), "+r"(t0));
            r_v1 &= 0xFFFF;
            /* ASM_BARRIER: mask v1 before storing 0x28..0x2C */
            __asm__ volatile("" : "+r"(r_v1));
            r_a1[0x28 / 4] = r_a3;
            r_a1[0x2C / 4] = t0;
            RR_GTE14_NCLIP();

            r_v0 = (t1 << 3) + t5;
            /* ASM_BARRIER: compute second vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_a3 = *(int *)(r_v0 + 0);
            t0 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load second vertex words before storing MAC0/SXYZ and loading V0 */
            __asm__ volatile("" : "+r"(r_a3), "+r"(t0));
            r_v0 = (int)((char *)r_a1 + 0x1C);
            RR_GTE14_SWC2_MAC0(r_v0);
            r_v0 = (int)((char *)r_a1 + 0x50);
            RR_GTE14_SWC2_SXYZ(r_v0);
            r_v0 = (int)((char *)r_a1 + 0x40);
            RR_GTE14_LWC2_V0(r_v0);
            r_a1[0x30 / 4] = r_a3;
            r_a1[0x34 / 4] = t0;
            RR_GTE14_RTPS();

            r_v0 = (r_v1 << 3) + t5;
            /* ASM_BARRIER: compute third vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_a3 = *(int *)(r_v0 + 0);
            t0 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load third vertex words before storing 0x38..0x3C */
            __asm__ volatile("" : "+r"(r_a3), "+r"(t0));
            r_a1[0x38 / 4] = r_a3;
            r_a1[0x3C / 4] = t0;
            RR_GTE14_AVSZ4();

            r_v0 = (t6 << 3) + t5;
            /* ASM_BARRIER: compute fourth vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_a3 = *(int *)(r_v0 + 0);
            t0 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load fourth vertex words before storing OTZ */
            __asm__ volatile("" : "+r"(r_a3), "+r"(t0));
            r_v0 = (int)((char *)r_a1 + 0x18);
            RR_GTE14_SWC2_OTZ(r_v0);
            r_v0 = r_a1[0x18 / 4];
            /* ASM_BARRIER: load OTZ before storing 0x40..0x44 */
            __asm__ volatile("" : "+r"(r_v0));
            r_a1[0x40 / 4] = r_a3;
            r_a1[0x44 / 4] = t0;
            if (r_v0 > 0) {
                r_v0 >>= t8;
                r_a1[0x18 / 4] = r_v0;
                if (r_v0 < 0x1C0) {
                    RR_GTE14_NCLIP();
                    r_a3 = r_a1[0x50 / 4];
                    t0 = r_a1[0x54 / 4];
                    t1 = r_a1[0x58 / 4];
                    /* ASM_BARRIER: load SXY0..SXY2 before staging a1+0x20 */
                    __asm__ volatile("" : "+r"(r_a3), "+r"(t0), "+r"(t1));
                    r_v0 = (int)((char *)r_a1 + 0x20);
                    /* ASM_BARRIER: stage a1+0x20 before stores */
                    __asm__ volatile("" : "+r"(r_v0));
                    *(int *)(t2 + 0x05) = r_a3;
                    *(int *)(t2 + 0x0D) = t0;
                    *(int *)(t2 + 0x15) = t1;
                    RR_GTE14_SWC2_MAC0(r_v0);
                    if (r_a1[0x1C / 4] > 0 || r_a1[0x20 / 4] < 0) {
                        RR_GTE14_SWC2_SXY2((char *)r_a1 + 0x5C);
                        r_v0 = r_a1[0x5C / 4];
                        *(int *)(t2 + 0x1D) = r_v0;

                        x0 = *(short *)((char *)r_a1 + 0x50);
                        x1 = *(short *)((char *)r_a1 + 0x54);
                        x2 = *(short *)((char *)r_a1 + 0x58);
                        x3 = *(short *)((char *)r_a1 + 0x5C);
                        if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                            (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                            y0 = *(short *)((char *)r_a1 + 0x52);
                            y1 = *(short *)((char *)r_a1 + 0x56);
                            y2 = *(short *)((char *)r_a1 + 0x5A);
                            y3 = *(short *)((char *)r_a1 + 0x5E);
                            if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                                (y0 < 0x200 || y1 < 0x200 || y2 < 0x200 || y3 < 0x200)) {
                                *(int *)(t2 + 0x01) = t9;
                                r_a3 = *(int *)(t3 - 0x0D);
                                t1 = *(int *)(t3 - 0x05);
                                t0 = *(int *)(t3 - 0x09);
                                /* ASM_BARRIER: load UV words before shift and stores */
                                __asm__ volatile("" : "+r"(r_a3), "+r"(t1), "+r"(t0));
                                r_v0 = t1 >> 16;
                                /* ASM_BARRIER: shift before UV stores */
                                __asm__ volatile("" : "+r"(r_v0));
                                *(int *)(t2 + 0x09) = r_a3;
                                *(int *)(t2 + 0x11) = t0;
                                *(int *)(t2 + 0x19) = t1;
                                *(int *)(t2 + 0x21) = r_v0;

                                r_v1 = r_a1[0x18 / 4];
                                r_a0 = r_a1[0x14 / 4];
                                r_v0 = *(signed char *)t3;
                                /* ASM_BARRIER: load otz, ot base, and offset byte before shifts */
                                __asm__ volatile("" : "+r"(r_v1), "+r"(r_a0), "+r"(r_v0));
                                r_v1 <<= 2;
                                r_a0 += r_v1;
                                r_v0 <<= 2;
                                r_a0 += r_v0;
                                /* ASM_BARRIER: keep OT entry address in $a0 */
                                __asm__ volatile("" : "+r"(r_a0));
                                r_v0 = *(int *)r_a0;
                                *(int *)t4 = r_v0;
                                *t2 = 9;
                                t2 += 0x28;
                                /* ASM_BARRIER: advance t2 before storing t4 to OT */
                                __asm__ volatile("" : "+r"(t2));
                                *(int *)r_a0 = (int)t4;
                                t4 += 0x28;
                                /* ASM_BARRIER: advance t4 before clearing top byte of OT entry */
                                __asm__ volatile("" : "+r"(t4));
                                *(char *)(r_a0 + 3) = 0;
                            }
                        }
                    }
                }
            }
            t3 += 0x18;
            t7 += 0x18;
        } while (r_a2-- > 0);
    }
    *(char **)0x1F800000 = t4;
    return t7;
}

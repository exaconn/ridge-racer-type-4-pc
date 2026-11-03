#include "types.h"
#include "rr_asm.h"
#include "m2c_macros.h"

extern int D_800F2EC8;
extern short *D_800ACC14;
extern s32 D_8009E0AC;
extern int func_8001E84C();
extern int func_8006EE3C();
extern int D_800100B8;

void *func_80013D48(char *a0_in, int *a1_in, int a2_in);
void *func_80014068(char *a0_in, int *a1_in, int a2_in);
void *func_8001446C(char *a0_in, int *a1_in, int a2_in);
void func_80014A8C(s32 arg0);
int func_80014A98(int a0, int a1, int a2, int a3);

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

void *func_8001446C(char *a0_in, int *a1_in, int a2_in) {
    /* ASM_BARRIER: keep temp in $v0 */
    register int r_v0 __asm__("$2");
    /* ASM_BARRIER: keep temp in $v1 */
    register int r_v1 __asm__("$3");
    /* ASM_BARRIER: keep a0_in / temp in $a0 */
    register int r_a0 __asm__("$4") = (int)a0_in;
    /* ASM_BARRIER: keep a1_in / temp in $a1 */
    register int r_a1 __asm__("$5") = (int)a1_in;
    /* ASM_BARRIER: keep a2_in / temp in $a2 */
    register int r_a2 __asm__("$6") = a2_in;
    /* ASM_BARRIER: keep packet+0x6C cursor in $a3 */
    register char *r_a3 __asm__("$7");
    /* ASM_BARRIER: keep context pointer in $t0 */
    register int *t0 __asm__("$8");
    /* ASM_BARRIER: keep envmap U in $t1 */
    register int t1 __asm__("$9");
    /* ASM_BARRIER: keep envmap V in $t2 */
    register int t2 __asm__("$10");
    /* ASM_BARRIER: keep packet cursor in $t3 */
    register char *t3 __asm__("$11");
    /* ASM_BARRIER: keep poly+0x1E cursor in $t4 */
    register char *t4 __asm__("$12");
    /* ASM_BARRIER: keep fourth vertex index in $t5 */
    register int t5 __asm__("$13");
    /* ASM_BARRIER: keep vertex table base in $t6 */
    register int t6 __asm__("$14");
    /* ASM_BARRIER: keep temp in $t7 */
    register int t7 __asm__("$15");
    /* ASM_BARRIER: keep envmap V shift in $t8 */
    register int t8 __asm__("$24");
    /* ASM_BARRIER: keep loop counter in $t9 */
    register int t9 __asm__("$25");
    /* ASM_BARRIER: keep poly cursor in $s0 */
    register char *s0 __asm__("$16");
    /* ASM_BARRIER: keep 0xE6000000 constant in $s1 */
    register int s1 __asm__("$17");
    /* ASM_BARRIER: keep 0xD tag size in $s2 */
    register int s2 __asm__("$18");
    /* ASM_BARRIER: keep OTZ shift in $s3 */
    register int s3 __asm__("$19");
    /* ASM_BARRIER: keep D_800F2EC8 | 0xE6000000 in $s4 */
    register int s4 __asm__("$20");
    int x0, x1, x2, x3, y0, y1, y2, y3;

    s0 = (char *)r_a0;
    t0 = (int *)r_a1;
    t9 = r_a2;
    /* ASM_BARRIER: keep s0, t0, t9 in their registers before initial loads */
    __asm__("" : "+r"(s0), "+r"(t0), "+r"(t9));
    s1 = 0xE6000000;
    r_v0 = *(int *)(s0 + 0);
    r_a2 = *(int *)(s0 + 4);
    t6 = t0[0];
    /* ASM_BARRIER: order initial loads before shifts */
    __asm__ volatile("" : "+r"(r_v0), "+r"(r_a2), "+r"(t6), "+r"(s1));
    r_a1 = r_v0 >> 16;
    t5 = r_a2 >> 16;
    r_v0 &= 0xFFFF;
    /* ASM_BARRIER: order shifts and mask before first vertex address */
    __asm__ volatile("" : "+r"(r_a1), "+r"(t5), "+r"(r_v0));
    r_v0 = (r_v0 << 3) + t6;
    /* ASM_BARRIER: compute first vertex pointer in $v0 */
    __asm__ volatile("" : "+r"(r_v0));
    r_v1 = *(int *)(r_v0 + 0);
    r_a0 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load first vertex words before second vertex address */
    __asm__ volatile("" : "+r"(r_v1), "+r"(r_a0));
    r_v0 = (r_a1 << 3) + t6;
    r_a2 &= 0xFFFF;
    /* ASM_BARRIER: compute second vertex pointer and mask a2 */
    __asm__ volatile("" : "+r"(r_v0), "+r"(r_a2));
    r_a1 = *(int *)(r_v0 + 0);
    t7 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load second vertex words before third vertex address */
    __asm__ volatile("" : "+r"(r_a1), "+r"(t7));
    r_v0 = (r_a2 << 3) + t6;
    /* ASM_BARRIER: compute third vertex pointer before storing 0x28..0x34 */
    __asm__ volatile("" : "+r"(r_v0));
    t0[0x28 / 4] = r_v1;
    t0[0x2C / 4] = r_a0;
    t0[0x30 / 4] = r_a1;
    t0[0x34 / 4] = t7;
    r_v1 = *(int *)(r_v0 + 0);
    r_a0 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load third vertex words before fourth vertex address */
    __asm__ volatile("" : "+r"(r_v1), "+r"(r_a0));
    r_v0 = (t5 << 3) + t6;
    /* ASM_BARRIER: compute fourth vertex pointer in $v0 */
    __asm__ volatile("" : "+r"(r_v0));
    r_a1 = *(int *)(r_v0 + 0);
    t7 = *(int *)(r_v0 + 4);
    /* ASM_BARRIER: load fourth vertex words before D_800F2EC8 and stores */
    __asm__ volatile("" : "+r"(r_a1), "+r"(t7));
    /* ASM_BARRIER: materialize %hi(D_800F2EC8) before storing r_a0 to 0x3C(t0) */
    __asm__("" : "+r"(r_a0) : "m"(D_800F2EC8));
    *(int *)((char *)t0 + 0x3C) = r_a0;
    r_a0 = 0x1F800000;
    *(int *)((char *)t0 + 0x38) = r_v1;
    *(int *)((char *)t0 + 0x40) = r_a1;
    r_v0 = D_800F2EC8;
    /* ASM_BARRIER: load D_800F2EC8 before setting v1=0x100 and storing 0x44 */
    __asm__ volatile("" : "+r"(r_v0), "+r"(r_a0));
    r_v1 = 0x100;
    /* ASM_BARRIER: set v1=0x100 before storing 0x44 */
    __asm__ volatile("" : "+r"(r_v1));
    *(int *)((char *)t0 + 0x44) = t7;
    /* ASM_BARRIER: store 0x44 before computing s4 */
    __asm__ volatile("" : "+r"(r_v0));
    s4 = r_v0 | s1;
    /* ASM_BARRIER: compute s4 before loading D_800ACC14 */
    __asm__ volatile("" : "+r"(s4));
    r_v0 = (int)D_800ACC14;
    t3 = *(char **)(r_a0 + 0x0);
    /* ASM_BARRIER: load D_800ACC14 and t3 before loading D_800ACC14[3] and s3 */
    __asm__ volatile("" : "+r"(r_v0), "+r"(t3));
    r_v0 = *(short *)(r_v0 + 6);
    s3 = *(int *)(r_a0 + 0x60);
    /* ASM_BARRIER: load D_800ACC14[3] and s3 before branch */
    __asm__ volatile("" : "+r"(r_v0), "+r"(s3));
    t8 = 7;
    if (r_v0 == r_v1) {
        t8 = 6;
    }
    /* ASM_BARRIER: keep t8 after branch */
    __asm__ volatile("" : "+r"(t8), "+r"(t9));
    if (t9-- > 0) {
        s2 = 0xD;
        t4 = s0 + 0x1E;
        r_a3 = t3 + 0x6C;
        do {
            /* ASM_BARRIER: keep loop pointers live at top of loop */
            __asm__ volatile("" : "+r"(s2), "+r"(t4), "+r"(r_a3) : "r"(t3), "r"(s0));
            r_v0 = (int)((char *)t0 + 0x28);
            RR_GTE14_LWC2_V012(r_v0);
            RR_GTE14_NOP2();
            RR_GTE14_RTPT();

            r_v0 = *(int *)(t4 + 2);
            r_a2 = *(int *)(t4 + 6);
            /* ASM_BARRIER: load next packed indices before shifts */
            __asm__ volatile("" : "+r"(r_v0), "+r"(r_a2));
            r_a1 = r_v0 >> 16;
            t5 = r_a2 >> 16;
            r_v0 &= 0xFFFF;
            /* ASM_BARRIER: order shifts and mask before first vertex address */
            __asm__ volatile("" : "+r"(r_a1), "+r"(t5), "+r"(r_v0));
            r_v0 = (r_v0 << 3) + t6;
            /* ASM_BARRIER: compute first vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_v1 = *(int *)(r_v0 + 0);
            r_a0 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load first vertex words before masking a2 */
            __asm__ volatile("" : "+r"(r_v1), "+r"(r_a0));
            r_a2 &= 0xFFFF;
            /* ASM_BARRIER: mask a2 before storing 0x28..0x2C */
            __asm__ volatile("" : "+r"(r_a2));
            t0[0x28 / 4] = r_v1;
            t0[0x2C / 4] = r_a0;
            RR_GTE14_NCLIP();

            r_v0 = (r_a1 << 3) + t6;
            /* ASM_BARRIER: compute second vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_v1 = *(int *)(r_v0 + 0);
            r_a0 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load second vertex words before storing MAC0/SXYZ and loading V0 */
            __asm__ volatile("" : "+r"(r_v1), "+r"(r_a0));
            r_v0 = (int)((char *)t0 + 0x1C);
            RR_GTE14_SWC2_MAC0(r_v0);
            r_v0 = (int)((char *)t0 + 0x50);
            RR_GTE14_SWC2_SXYZ(r_v0);
            r_v0 = (int)((char *)t0 + 0x40);
            RR_GTE14_LWC2_V0(r_v0);
            t0[0x30 / 4] = r_v1;
            t0[0x34 / 4] = r_a0;
            RR_GTE14_RTPS();

            r_v0 = (r_a2 << 3) + t6;
            /* ASM_BARRIER: compute third vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_v1 = *(int *)(r_v0 + 0);
            r_a0 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load third vertex words before storing 0x38..0x3C */
            __asm__ volatile("" : "+r"(r_v1), "+r"(r_a0));
            t0[0x38 / 4] = r_v1;
            t0[0x3C / 4] = r_a0;
            RR_GTE14_AVSZ4();

            r_v0 = (t5 << 3) + t6;
            /* ASM_BARRIER: compute fourth vertex pointer in $v0 */
            __asm__ volatile("" : "+r"(r_v0));
            r_v1 = *(int *)(r_v0 + 0);
            r_a0 = *(int *)(r_v0 + 4);
            /* ASM_BARRIER: load fourth vertex words before storing OTZ */
            __asm__ volatile("" : "+r"(r_v1), "+r"(r_a0));
            r_v0 = (int)((char *)t0 + 0x18);
            RR_GTE14_SWC2_OTZ(r_v0);
            r_v0 = t0[0x18 / 4];
            /* ASM_BARRIER: load OTZ before storing 0x40..0x44 */
            __asm__ volatile("" : "+r"(r_v0));
            t0[0x40 / 4] = r_v1;
            t0[0x44 / 4] = r_a0;
            if (r_v0 > 0) {
                r_v0 >>= s3;
                t0[0x18 / 4] = r_v0;
                if (r_v0 < 0x1C0) {
                    RR_GTE14_NCLIP();
                    r_v1 = t0[0x50 / 4];
                    r_a0 = t0[0x54 / 4];
                    r_a1 = t0[0x58 / 4];
                    /* ASM_BARRIER: load SXY0..SXY2 before staging t0+0x20 */
                    __asm__ volatile("" : "+r"(r_v1), "+r"(r_a0), "+r"(r_a1));
                    r_v0 = (int)((char *)t0 + 0x20);
                    /* ASM_BARRIER: stage t0+0x20 before stores */
                    __asm__ volatile("" : "+r"(r_v0));
                    *(int *)(r_a3 - 0x60) = r_v1;
                    *(int *)(r_a3 - 0x28) = r_v1;
                    *(int *)(r_a3 - 0x54) = r_a0;
                    *(int *)(r_a3 - 0x1C) = r_a0;
                    *(int *)(r_a3 - 0x48) = r_a1;
                    *(int *)(r_a3 - 0x10) = r_a1;
                    RR_GTE14_SWC2_MAC0(r_v0);
                    if (t0[0x1C / 4] > 0 || t0[0x20 / 4] < 0) {
                        /* ASM_BARRIER: read SXY2 from GTE $14 into $t7 */
                        __asm__ volatile("mfc2 %0, $14" : "=r"(t7));
                        RR_NOP_DEP();
                        t0[0x5C / 4] = t7;
                        *(int *)(r_a3 - 0x3C) = t7;
                        *(int *)(r_a3 - 0x04) = t7;

                        x0 = *(short *)((char *)t0 + 0x50);
                        x1 = *(short *)((char *)t0 + 0x54);
                        x2 = *(short *)((char *)t0 + 0x58);
                        x3 = *(short *)((char *)t0 + 0x5C);
                        if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                            (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                            y0 = *(short *)((char *)t0 + 0x52);
                            y1 = *(short *)((char *)t0 + 0x56);
                            y2 = *(short *)((char *)t0 + 0x5A);
                            y3 = *(short *)((char *)t0 + 0x5E);
                            if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                                (y0 < 0x200 || y1 < 0x200 || y2 < 0x200 || y3 < 0x200)) {
                                r_v1 = *(int *)(t4 - 0x16);
                                r_v0 = *(int *)(t4 - 0x12);
                                r_a2 = t0[1];
                                /* ASM_BARRIER: load packed normal indices and normal table base before shifts */
                                __asm__ volatile("" : "+r"(r_v1), "+r"(r_v0), "+r"(r_a2));
                                r_a0 = r_v1 >> 16;
                                r_v1 &= 0xFFFF;
                                r_a1 = r_v0 >> 16;
                                r_v0 &= 0xFFFF;
                                /* ASM_BARRIER: order shifts and masks before normal address math */
                                __asm__ volatile("" : "+r"(r_a0), "+r"(r_v1), "+r"(r_a1), "+r"(r_v0));
                                r_v1 <<= 3;
                                /* ASM_BARRIER: shift first normal index before adding base */
                                __asm__ volatile("" : "+r"(r_v1));
                                r_v1 = r_a2 + r_v1;
                                /* ASM_BARRIER: compute first normal address in $v1 */
                                __asm__ volatile("" : "+r"(r_v1));
                                r_a0 = r_a2 + (r_a0 << 3);
                                /* ASM_BARRIER: compute second normal address in $a0 */
                                __asm__ volatile("" : "+r"(r_a0));
                                r_v0 = r_a2 + (r_v0 << 3);
                                /* ASM_BARRIER: compute third normal address in $v0 */
                                __asm__ volatile("" : "+r"(r_v0));
                                RR_GTE14_LWC2_V0_V1_V2(r_v1, r_a0, r_v0);
                                RR_GTE14_NOP2();
                                RR_GTE_OP(0x4A4A6412);

                                r_a1 = (r_a1 << 3) + r_a2;
                                /* ASM_BARRIER: compute fourth normal address in $a1 */
                                __asm__ volatile("" : "+r"(r_a1));
                                r_v1 = *(int *)(r_a1 + 0);
                                r_a0 = *(int *)(r_a1 + 4);
                                /* ASM_BARRIER: load fourth normal words before storing 0x48..0x4C */
                                __asm__ volatile("" : "+r"(r_v1), "+r"(r_a0));
                                t0[0x48 / 4] = r_v1;
                                t0[0x4C / 4] = r_a0;
                                RR_GTE_SQR12();
                                RR_GTE_SQR12();
                                RR_GTE_OP(0x4B38041C);

                                r_v1 = *(int *)(t4 - 0x0E);
                                r_a0 = *(int *)(t4 - 0x0A);
                                /* ASM_BARRIER: load UV0/UV1 before MVMVA V1 */
                                __asm__ volatile("" : "+r"(r_v1), "+r"(r_a0));
                                RR_GTE_OP(0x4A4AE412);
                                r_v0 = t0[0x08 / 4];
                                /* ASM_BARRIER: load-use delay on t0[2] */
                                __asm__ volatile("" : "+r"(r_v0));
                                r_v0 = r_v1 + r_v0;
                                *(int *)(r_a3 - 0x5C) = r_v0;
                                *(int *)(r_a3 - 0x50) = r_a0;
                                RR_GTE_SQR12();
                                RR_GTE_SQR12();
                                RR_GTE_OP(0x4B38041C);

                                r_v1 = *(int *)(t4 - 0x06);
                                /* ASM_BARRIER: load-use delay on *(int *)(t4 - 6) */
                                __asm__ volatile("" : "+r"(r_v1));
                                r_a0 = r_v1 >> 16;
                                /* ASM_BARRIER: shift r_a0 before MVMVA V2 */
                                __asm__ volatile("" : "+r"(r_a0));
                                RR_GTE_OP(0x4A4B6412);
                                *(int *)(r_a3 - 0x44) = r_v1;
                                *(int *)(r_a3 - 0x38) = r_a0;
                                RR_GTE_SQR12();
                                RR_GTE_SQR12();
                                RR_GTE_OP(0x4B38041C);

                                r_a2 = (int)(t3 + 4);
                                /* ASM_BARRIER: store RGB0..RGB2 into POLY_GT4 packet */
                                __asm__ volatile("swc2 $20, 4(%0); swc2 $21, 16(%0); swc2 $22, 28(%0)" : : "r"(r_a2) : "memory");

                                r_v0 = t0[0x18 / 4];
                                r_a0 = t0[0x14 / 4];
                                r_v1 = *(signed char *)(t4 - 1);
                                r_a1 = *(unsigned short *)(t4 + 0);
                                /* ASM_BARRIER: load otz, ot base, offset, and flags before shifts */
                                __asm__ volatile("" : "+r"(r_v0), "+r"(r_a0), "+r"(r_v1), "+r"(r_a1));
                                r_v0 <<= 2;
                                r_a0 += r_v0;
                                r_v1 <<= 2;
                                r_v0 = r_a1 & 3;
                                /* ASM_BARRIER: compute r_a1 & 3 before adding r_v1 in bnez delay slot */
                                __asm__ volatile("" : "+r"(r_v0), "+r"(r_a0), "+r"(r_v1));
                                r_a0 += r_v1;
                                if (r_v0 == 0) {
                                    r_v0 = (int)((char *)t0 + 0x48);
                                    RR_GTE14_LWC2_V0(r_v0);
                                    RR_GTE14_NOP2();
                                    RR_GTE14_LIGHT0();
                                    r_v0 = (unsigned int)r_a1 >> 1;
                                    /* ASM_BARRIER: shift r_a1 >> 1 into $v0 before storing -0x61(a3) */
                                    __asm__ volatile("" : "+r"(r_v0), "+r"(r_a0));
                                    *(char *)(r_a3 - 0x61) = r_v0;
                                    r_v0 = *(int *)r_a0;
                                    /* ASM_BARRIER: load-use delay on *(int *)r_a0 */
                                    __asm__ volatile("" : "+r"(r_v0));
                                    *(int *)(r_a3 - 0x68) = r_v0;
                                    r_v0 = 0xC;
                                    /* ASM_BARRIER: keep 0xC in $v0 before storing -0x65(a3) */
                                    __asm__ volatile("" : "+r"(r_v0));
                                    *(char *)(r_a3 - 0x65) = r_v0;
                                    r_v0 = (int)(t3 + 0x2C);
                                    /* ASM_BARRIER: compute t3+0x2C before OT stores */
                                    __asm__ volatile("" : "+r"(r_v0));
                                    *(int *)r_a0 = r_a2;
                                    *(char *)(r_a0 + 3) = 0;
                                    RR_GTE14_SWC2_RGB2(r_v0);
                                    r_a3 += 0x38;
                                    t3 += 0x38;
                                } else {
                                    /* ASM_BARRIER: prevent delay slot filling into bnez */
                                    __asm__ volatile("" : "+r"(r_a0));
                                    *(int *)(r_a3 - 0x68) = s1;
                                    r_v0 = *(unsigned short *)(t4 + 0);
                                    /* ASM_BARRIER: load-use delay on *(unsigned short *)t4 */
                                    __asm__ volatile("" : "+r"(r_v0));
                                    r_v0 &= 3;
                                    r_v0 = s1 | r_v0;
                                    *(int *)(r_a3 - 0x30) = r_v0;
                                    r_v0 = (int)(t3 + 0x3C);
                                    /* ASM_BARRIER: store RGB0..RGB2 into second POLY_GT4 packet */
                                    __asm__ volatile("swc2 $20, 4(%0); swc2 $21, 16(%0); swc2 $22, 28(%0)" : : "r"(r_v0) : "memory");
                                    r_v0 = 0x3E;
                                    /* ASM_BARRIER: keep 0x3E in $v0 before storing -0x29(a3) */
                                    __asm__ volatile("" : "+r"(r_v0));
                                    *(char *)(r_a3 - 0x29) = r_v0;
                                    RR_GTE_OP(0x4A486012);
                                    r_v0 = *(int *)r_a0;
                                    /* ASM_BARRIER: load-use delay on *(int *)r_a0 */
                                    __asm__ volatile("" : "+r"(r_v0));
                                    r_v1 = *(unsigned char *)(r_v0 + 4);
                                    r_v0 = 0xE6;
                                    /* ASM_BARRIER: load 0xE6 in load delay slot before beq */
                                    __asm__ volatile("" : "+r"(r_v1), "+r"(r_v0));
                                    if (r_v1 != r_v0) {
                                        /* ASM_BARRIER: prevent delay slot filling into beq */
                                        __asm__ volatile("" : "+r"(r_a0));
                                        *(int *)(r_a3 + 0x08) = s4;
                                        r_v0 = *(int *)r_a0;
                                        /* ASM_BARRIER: load-use delay on *(int *)r_a0 */
                                        __asm__ volatile("" : "+r"(r_v0));
                                        *(int *)(r_a3 + 0x04) = r_v0;
                                        r_v0 = 1;
                                        /* ASM_BARRIER: keep 1 in $v0 before storing 0x7(a3) */
                                        __asm__ volatile("" : "+r"(r_v0));
                                        *(char *)(r_a3 + 0x07) = r_v0;
                                        r_v0 = (int)(t3 + 0x70);
                                        /* ASM_BARRIER: compute t3+0x70 before OT stores */
                                        __asm__ volatile("" : "+r"(r_v0));
                                        *(int *)r_a0 = r_v0;
                                        *(char *)(r_a0 + 3) = 0;
                                    }
                                    /* ASM_BARRIER: join at .L8001492C */
                                    __asm__ volatile("" : "+r"(r_a0));
                                    r_v0 = *(int *)r_a0;
                                    /* ASM_BARRIER: load-use delay on *(int *)r_a0 */
                                    __asm__ volatile("" : "+r"(r_v0));
                                    *(int *)(r_a3 - 0x34) = r_v0;
                                    r_v0 = (int)(t3 + 0x38);
                                    /* ASM_BARRIER: compute t3+0x38 before sb and OT stores */
                                    __asm__ volatile("" : "+r"(r_v0));
                                    *(char *)(r_a3 - 0x31) = s2;
                                    *(int *)r_a0 = r_v0;
                                    *(char *)(r_a0 + 3) = 0;
                                    /* ASM_BARRIER: order first OT link before second OT link */
                                    __asm__ volatile("" : "+r"(r_a0) : : "memory");
                                    r_v0 = *(int *)r_a0;
                                    /* ASM_BARRIER: load-use delay on *(int *)r_a0 */
                                    __asm__ volatile("" : "+r"(r_v0));
                                    *(int *)t3 = r_v0;
                                    *(char *)(r_a3 - 0x69) = s2;
                                    *(int *)r_a0 = (int)t3;
                                    *(char *)(r_a0 + 3) = 0;

                                    /* ASM_BARRIER: read IR1 and IR2 from GTE $9..$10 */
                                    __asm__ volatile("mfc2 %0, $9; mfc2 %1, $10" : "=r"(t1), "=r"(t2));
                                    r_v0 = t1 >> 6;
                                    t1 = r_v0 + 0xC0;
                                    r_v0 = t2 >> t8;
                                    t2 = r_v0 + 0xC0;
                                    r_v0 = t2 << 8;
                                    r_v1 = t0[0x0C / 4];
                                    /* ASM_BARRIER: load t0[3] before ORing t1 and v1 */
                                    __asm__ volatile("" : "+r"(r_v0), "+r"(r_v1), "+r"(t1));
                                    r_v0 |= t1;
                                    r_v0 |= r_v1;
                                    *(int *)(r_a3 - 0x24) = r_v0;

                                    r_v0 = (int)((char *)t0 + 0x48);
                                    RR_GTE14_LWC2_V0(r_v0);
                                    RR_GTE14_NOP2();
                                    RR_GTE14_LIGHT0();
                                    r_v0 = (int)(t3 + 0x2C);
                                    RR_GTE14_SWC2_RGB2(r_v0);
                                    r_v0 = (int)(t3 + 0x64);
                                    RR_GTE14_SWC2_RGB2(r_v0);
                                    RR_NOP_DEP();
                                    RR_GTE_OP(0x4A48E012);
                                    /* ASM_BARRIER: read IR1 and IR2 from GTE $9..$10 */
                                    __asm__ volatile("mfc2 %0, $9; mfc2 %1, $10" : "=r"(t1), "=r"(t2));
                                    RR_NOP_DEP();
                                    RR_GTE_OP(0x4A496012);

                                    r_v0 = t1 >> 6;
                                    t1 = r_v0 + 0xC0;
                                    r_v0 = t2 >> t8;
                                    t2 = r_v0 + 0xC0;
                                    r_v0 = t2 << 8;
                                    r_v1 = t0[0x10 / 4];
                                    /* ASM_BARRIER: load t0[4] before ORing t1 and v1 */
                                    __asm__ volatile("" : "+r"(r_v0), "+r"(r_v1), "+r"(t1));
                                    r_v0 |= t1;
                                    r_v0 |= r_v1;
                                    *(int *)(r_a3 - 0x18) = r_v0;

                                    /* ASM_BARRIER: read IR1 and IR2 from GTE $9..$10 */
                                    __asm__ volatile("mfc2 %0, $9; mfc2 %1, $10" : "=r"(t1), "=r"(t2));
                                    r_v0 = t1 >> 6;
                                    t1 = r_v0 + 0xC0;
                                    r_v0 = t2 >> t8;
                                    t2 = r_v0 + 0xC0;
                                    /* ASM_BARRIER: compute t1 and t2 before MVMVA V0 */
                                    __asm__ volatile("" : "+r"(t1), "+r"(t2));
                                    RR_GTE_OP(0x4A486012);
                                    r_v0 = (t2 << 8) | t1;
                                    *(int *)(r_a3 - 0x0C) = r_v0;

                                    /* ASM_BARRIER: read IR1 and IR2 from GTE $9..$10 */
                                    __asm__ volatile("mfc2 %0, $9; mfc2 %1, $10" : "=r"(t1), "=r"(t2));
                                    r_v0 = t1 >> 6;
                                    t1 = r_v0 + 0xC0;
                                    r_v0 = t2 >> t8;
                                    t2 = r_v0 + 0xC0;
                                    /* ASM_BARRIER: compute t1 and t2 before final UV store */
                                    __asm__ volatile("" : "+r"(t1), "+r"(t2));
                                    r_v0 = (t2 << 8) | t1;
                                    *(int *)(r_a3 + 0x00) = r_v0;
                                    r_a3 += 0x78;
                                    t3 += 0x78;
                                }
                            }
                        }
                    }
                }
            }
            t4 += 0x20;
            s0 += 0x20;
        } while (t9-- > 0);
    }
    *(char **)0x1F800000 = t3;
    return s0;
}

void func_80014A8C(s32 arg0) {
    D_8009E0AC = arg0;
}

int func_80014A98(int a0, int a1, int a2, int a3)
{
  int *s0 = (int *) 0x1F80007C;
  int *p;
  int *g;
  int w;
  unsigned new_var3;
  unsigned new_var2;
  int t;
  /* ASM_BARRIER: pin a to $5 to match original register allocation */
  register unsigned a __asm__("$5");
  unsigned long new_var;
  unsigned b;
  if (D_8009E0AC < 0)
  {
    return 0;
  }
  func_8006EE3C(0x1F80007C, a0, a1);
  new_var = 3;
  s0[2] = a3;
  a = D_8009E0AC & 0xF;
  a <<= 6;
  b = D_8009E0AC & 0x10;
  b <<= 4;
  s0[3] = (((b + 0x7F) << 6) | ((a >> 4) | new_var)) << 16;
  new_var3 = b >> 4;
  new_var2 = (a >> 6) | 0x20;
  s0[4] = (new_var3 | new_var2) << 16;
  g = (int *) 0x1F800000;
  s0[0] = g[0x54 / 4];
  s0[1] = g[0x58 / 4];
  s0[5] = g[1] + 0x200;
  p = *((int **) ((a2 * 4) + g[0x50 / 4]));
  while ((w = *(p++)) != 0)
  {
    t = w >> 16;
    w &= 0xFFFF;
    switch (w)
    {
      case 0:
        p = (int *) (((char *) p) + (t * 0x10));
        break;

      case 1:
        p = (int *) (((char *) p) + (t * 0x18));
        break;

      case 2:
        p = (int *) func_80014068(p, s0, t);
        break;

      case 3:
        asm volatile("mtc2 %0, $6" : : "r"(0x3C808080));
        p = (int *) func_8001446C(p, s0, t);
        break;

      default:
        return func_8001E84C(&D_800100B8, w, t);

    }

  }

  return 1;
}

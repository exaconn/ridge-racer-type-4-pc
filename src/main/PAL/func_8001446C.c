#include "rr_asm.h"

extern int D_800F2EC8;
extern short *D_800ACC14;

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

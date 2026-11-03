#include "rr_asm.h"

char *func_800183EC(char *a0, char *a1) {
    volatile int *hw;
    int n;
    int *pkt;
    int shift;
    char *verts;

    *(int *)(a1 + 0xF4) = 0x3C808080;
    RR_GTE14_LWC2_RGB(a1 + 0xF4);

    hw = (volatile int *)0x1F800000;
    n = *(int *)(a1 + 0xE4);
    pkt = (int *)hw[0];
    shift = hw[0x60 / 4];
    verts = *(char **)(a1 + 0x70);

    while (n-- > 0) {
        RR_GTE_RTPT_LDV(
            verts + (*(unsigned short *)(a0 + 0) << 3),
            verts + (*(unsigned short *)(a0 + 2) << 3),
            verts + (*(unsigned short *)(a0 + 4) << 3)
        );
        RR_GTE14_NOP2();
        RR_GTE14_NCLIP();
        RR_GTE14_SWC2_MAC0(a1 + 0xEC);
        RR_GTE14_SWC2_SXYZ(a1 + 0x140);
        RR_GTE_RTPS_LDV(verts + (*(unsigned short *)(a0 + 6) << 3));
        pkt[2] = *(int *)(a1 + 0x140);
        pkt[5] = *(int *)(a1 + 0x144);
        pkt[8] = *(int *)(a1 + 0x148);
        RR_GTE14_NOP2();
        RR_GTE14_NCLIP();
        RR_GTE14_SWC2_SXY2(a1 + 0x14C);
        RR_GTE14_SWC2_MAC0(a1 + 0xF0);

        if (*(int *)(a1 + 0xEC) > 0 || *(int *)(a1 + 0xF0) < 0) {
            RR_GTE14_NOP2();
            RR_GTE14_AVSZ4();
            pkt[11] = *(int *)(a1 + 0x14C);

            if (*(short *)(a1 + 0x140) >= 0 || *(short *)(a1 + 0x144) >= 0 || *(short *)(a1 + 0x148) >= 0 || *(short *)(a1 + 0x14C) >= 0) {
                if (*(short *)(a1 + 0x140) < 0x140 || *(short *)(a1 + 0x144) < 0x140 || *(short *)(a1 + 0x148) < 0x140 || *(short *)(a1 + 0x14C) < 0x140) {
                    if (*(short *)(a1 + 0x142) >= 0 || *(short *)(a1 + 0x146) >= 0 || *(short *)(a1 + 0x14A) >= 0 || *(short *)(a1 + 0x14E) >= 0) {
                        if (*(short *)(a1 + 0x142) < 0x200 || *(short *)(a1 + 0x146) < 0x200 || *(short *)(a1 + 0x14A) < 0x200 || *(short *)(a1 + 0x14E) < 0x200) {
                            int u, v, z;

                            RR_GTE14_SWC2_OTZ(a1 + 0xE8);
                            RR_GTE14_LWC2_V0_V1_V2(
                                *(char **)(a1 + 0x70) + (*(unsigned short *)(a0 + 8) << 3),
                                *(char **)(a1 + 0x70) + (*(unsigned short *)(a0 + 10) << 3),
                                *(char **)(a1 + 0x70) + (*(unsigned short *)(a0 + 12) << 3)
                            );
                            RR_GTE14_NOP2();
                            RR_GTE14_LIGHT0();
                            RR_GTE14_LIGHT1();
                            RR_GTE14_LIGHT2();
                            /* ASM_BARRIER: store GTE RGB0/RGB1/RGB2 into packet offsets 4, 16, 28 */
                            __asm__ volatile("swc2 $20, 4(%0); swc2 $21, 16(%0); swc2 $22, 28(%0)" : : "r"(pkt) : "memory");
                            RR_GTE14_NOP2();
                            RR_GTE_OP(0x4A486012);
                            /* ASM_BARRIER: store GTE IR1/IR2/IR3 to context at +0x100 with memory clobber */
                            __asm__ volatile("swc2 $9, 0(%0); swc2 $10, 4(%0); swc2 $11, 8(%0)" : : "r"(a1 + 0x100) : "memory");
                            RR_GTE14_NOP2();
                            RR_GTE_OP(0x4A48E012);

                            u = (*(int *)(a1 + 0x100) >> 6) + 0x40;
                            v = (*(int *)(a1 + 0x104) >> 7) + 0x40;
                            if (u < 0) {
                                u = 0;
                            } else if (u >= 0x80) {
                                u = 0x7F;
                            }
                            if (v < 0) {
                                v = 0;
                            } else if (v >= 0x80) {
                                v = 0x7F;
                            }
                            pkt[3] = (v << 8) | u | *(int *)(a1 + 0xFC);

                            /* ASM_BARRIER: store GTE IR1/IR2/IR3 to context at +0x10C with memory clobber */
                            __asm__ volatile("swc2 $9, 0(%0); swc2 $10, 4(%0); swc2 $11, 8(%0)" : : "r"(a1 + 0x10C) : "memory");
                            RR_GTE14_NOP2();
                            RR_GTE_OP(0x4A496012);

                            u = (*(int *)(a1 + 0x10C) >> 6) + 0x40;
                            v = (*(int *)(a1 + 0x110) >> 7) + 0x40;
                            if (u < 0) {
                                u = 0;
                            } else if (u >= 0x80) {
                                u = 0x7F;
                            }
                            if (v < 0) {
                                v = 0;
                            } else if (v >= 0x80) {
                                v = 0x7F;
                            }
                            pkt[6] = (v << 8) | u | *(int *)(a1 + 0xF8);

                            /* ASM_BARRIER: store GTE IR1/IR2/IR3 to context at +0x118 with memory clobber */
                            __asm__ volatile("swc2 $9, 0(%0); swc2 $10, 4(%0); swc2 $11, 8(%0)" : : "r"(a1 + 0x118) : "memory");
                            RR_GTE14_LWC2_V0(*(char **)(a1 + 0x70) + (*(unsigned short *)(a0 + 14) << 3));
                            RR_GTE14_NOP2();
                            RR_GTE14_LIGHT0();
                            RR_GTE14_SWC2_RGB2(pkt + 10);
                            RR_GTE14_NOP2();
                            RR_GTE_OP(0x4A486012);

                            u = (*(int *)(a1 + 0x118) >> 6) + 0x40;
                            v = (*(int *)(a1 + 0x11C) >> 7) + 0x40;
                            if (u < 0) {
                                u = 0;
                            } else if (u >= 0x80) {
                                u = 0x7F;
                            }
                            if (v < 0) {
                                v = 0;
                            } else if (v >= 0x80) {
                                v = 0x7F;
                            }
                            pkt[9] = (v << 8) | u;

                            /* ASM_BARRIER: store GTE IR1/IR2/IR3 to context at +0x124 with memory clobber */
                            __asm__ volatile("swc2 $9, 0(%0); swc2 $10, 4(%0); swc2 $11, 8(%0)" : : "r"(a1 + 0x124) : "memory");
                            u = (*(int *)(a1 + 0x124) >> 6) + 0x40;
                            v = (*(int *)(a1 + 0x128) >> 7) + 0x40;
                            if (u < 0) {
                                u = 0;
                            } else if (u >= 0x80) {
                                u = 0x7F;
                            }
                            if (v < 0) {
                                v = 0;
                            } else if (v >= 0x80) {
                                v = 0x7F;
                            }
                            pkt[12] = (v << 8) | u;

                            z = *(int *)(a1 + 0xE8) >> shift;
                            if (z > 0) {
                                if (z < 0x1C0) {
                                    volatile int *g = (volatile int *)0x1F800000;
                                    pkt[0] = *(int *)((*(signed char *)(a0 + 0x15) << 2) + ((z << 2) + g[1]) + 0x200);
                                    ((char *)pkt)[3] = 0xC;
                                    *(int *)((*(signed char *)(a0 + 0x15) << 2) + ((z << 2) + g[1]) + 0x200) = (int)pkt;
                                    pkt += 13;
                                    *(char *)((*(signed char *)(a0 + 0x15) << 2) + ((z << 2) + g[1]) + 0x203) = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        a0 += 0x18;
    }

    /* ASM_BARRIER: extend live range of n past loop for register allocation priority */
    __asm__("" : : "r"(n), "r"(n), "r"(n));
    *(int **)0x1F800000 = pkt;
    return a0;
}

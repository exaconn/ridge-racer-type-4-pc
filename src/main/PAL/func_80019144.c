#include "rr_asm.h"

char *func_80019144(char *a0, char *a1) {
    volatile int *hw = (volatile int *)0x1F800000;
    int n = *(int *)(a1 + 0xE4);
    int *pkt = (int *)hw[0];
    int shift = hw[0x60 / 4];
    char *verts = *(char **)(a1 + 0x70);

    while (n-- > 0) {
        RR_GTE_RTPT_LDV(
            verts + (*(unsigned short *)(a0 + 0) << 3),
            verts + (*(unsigned short *)(a0 + 2) << 3),
            verts + (*(unsigned short *)(a0 + 4) << 3)
        );
        RR_GTE14_NOP2();
        RR_GTE14_NCLIP();
        RR_GTE14_SWC2_MAC0(a1 + 0xEC);
        RR_GTE14_NOP2();
        RR_GTE_OP(0x4B58002D);
        RR_GTE14_SWC2_OTZ(a1 + 0xE8);
        if (*(int *)(a1 + 0xE8) > 0) {
            RR_GTE14_SWC2_SXYZ(a1 + 0x140);
            RR_GTE_RTPS_LDV(verts + (*(unsigned short *)(a0 + 6) << 3));
            RR_GTE14_NOP2();
            RR_GTE14_NCLIP();
            RR_GTE14_SWC2_MAC0(a1 + 0xF0);
            if (*(int *)(a1 + 0xEC) > 0 || *(int *)(a1 + 0xF0) < 0) {
                RR_GTE14_SWC2_SXY2(a1 + 0x14C);
                pkt[2] = *(int *)(a1 + 0x140);
                pkt[4] = *(int *)(a1 + 0x144);
                pkt[6] = *(int *)(a1 + 0x148);
                pkt[8] = *(int *)(a1 + 0x14C);
                if ((*(short *)(a1 + 0x140) >= 0 || *(short *)(a1 + 0x144) >= 0 || *(short *)(a1 + 0x148) >= 0 || *(short *)(a1 + 0x14C) >= 0) &&
                    (*(short *)(a1 + 0x140) < 0x140 || *(short *)(a1 + 0x144) < 0x140 || *(short *)(a1 + 0x148) < 0x140 || *(short *)(a1 + 0x14C) < 0x140)) {
                    if ((*(short *)(a1 + 0x142) >= 0 || *(short *)(a1 + 0x146) >= 0 || *(short *)(a1 + 0x14A) >= 0 || *(short *)(a1 + 0x14E) >= 0) &&
                        (*(short *)(a1 + 0x142) < 0x200 || *(short *)(a1 + 0x146) < 0x200 || *(short *)(a1 + 0x14A) < 0x200 || *(short *)(a1 + 0x14E) < 0x200)) {
                        int z;
                        pkt[3] = *(int *)(a0 + 0x14);
                        pkt[5] = *(int *)(a0 + 0x18);
                        pkt[7] = *(int *)(a0 + 0x1C);
                        pkt[9] = *(short *)(a0 + 0x1E);
                        z = *(int *)(a1 + 0xE8) >> shift;
                        if (z > 0) {
                            if (z < 0x1C0) {
                                volatile int *g = (volatile int *)0x1F800000;
                                RR_GTE14_LWC2_RGB(a0 + 0x10);
                                RR_GTE14_LWC2_V0(*(char **)(a1 + 0x70) + (*(unsigned short *)(a0 + 8) << 3));
                                RR_GTE14_NOP2();
                                RR_GTE14_LIGHT0();
                                RR_GTE14_SWC2_RGB2(pkt + 1);
                                pkt[0] = *(int *)((*(signed char *)(a0 + 0x21) << 2) + ((z << 2) + g[1]) + 0x200);
                                ((char *)pkt)[3] = 9;
                                *(int *)((*(signed char *)(a0 + 0x21) << 2) + ((z << 2) + g[1]) + 0x200) = (int)pkt;
                                pkt += 10;
                                *(char *)((*(signed char *)(a0 + 0x21) << 2) + ((z << 2) + g[1]) + 0x203) = 0;
                            }
                        }
                    }
                }
            }
        }
        a0 += 0x24;
        /* ASM_BARRIER: weight pkt, n, a0 loop references for register allocation order */
        __asm__("" : "+r"(n) : "r"(pkt), "r"(a0));
    }
    /* ASM_BARRIER: extend verts live range after loop so pkt=$t0, n=$t1, a0=$t2, verts=$t3 */
    __asm__("" : : "r"(pkt), "r"(n), "r"(a0), "r"(verts));
    *(int **)0x1F800000 = pkt;
    return a0;
}

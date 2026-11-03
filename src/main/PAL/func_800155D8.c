#include "rr_asm.h"

extern int D_8009E0E0[];

void func_800155D8(char *a0, char *a1) {
    int otz;
    int *p;
    int *dst;
    int i;
    int col;
    int sxy0;
    int *pkt;
    int *ot;
    int *spd;
    int e1;
    int t;

    RR_GTE_SETTR(a1 + 0x14);
    *(short *)(a1 + 0xC8) = 0;
    *(short *)(a1 + 0xCA) = -0x5C;
    *(short *)(a1 + 0xCC) = 0x98;
    RR_GTE_RTPS_LDV(a1 + 0xC8);
    RR_GTE_SWC2_SXY2(a1 + 0x1A0);
    RR_GTE_TR_FROM_IR();
    /* ASM_BARRIER: read GTE IR3 ($11) with pipeline delay nop */
    __asm__ volatile("mfc2 %0, $11\n\tnop" : "=r"(otz));
    if (otz >= 0) {
        otz >>= *(int *)0x1F800060 + 2;
        if (otz < 0x1C0) {
            /* ASM_BARRIER: set identity GTE rotation matrix ($0..$4) and clear VXY/VZ ($1,$3,$5) */
            __asm__ volatile(
                "ctc2 %0, $0\n\t"
                "ctc2 $0, $1\n\t"
                "ctc2 %0, $2\n\t"
                "ctc2 $0, $3\n\t"
                "ctc2 %0, $4\n\t"
                "mtc2 $0, $1\n\t"
                "mtc2 $0, $3\n\t"
                "mtc2 $0, $5"
                :
                : "r"(0x1000)
            );
            p = D_8009E0E0;
            dst = (int *)(a1 + 0x198);
            i = 0x11;
            do {
                /* ASM_BARRIER: load VXY0/VXY1/VXY2 and run RTPT */
                __asm__ volatile(
                    "lwc2 $0, 0(%0)\n\t"
                    "lwc2 $2, 4(%0)\n\t"
                    "lwc2 $4, 8(%0)\n\t"
                    "nop\n\t"
                    "nop\n\t"
                    ".word 0x4A280030"
                    :
                    : "r"(p)
                );
                p += 3;
                dst += 3;
                RR_GTE_SWC2_SXYZ(dst);
                i -= 3;
            } while (i > 0);
            e1 = 0xE1000220;
            dst = (int *)(a1 + 0x1A4);
            i = 0x10;
            col = *(int *)(a0 + 8);
            spd = (int *)0x1F800000;
            t = 0x32000000;
            sxy0 = *(int *)(a1 + 0x1A0);
            pkt = (int *)spd[0];
            col = (col << 8) | (col | t);
            ot = (int *)((otz << 2) + spd[1] + 0x1A4);
            do {
                pkt[3] = sxy0;
                pkt[5] = dst[0];
                pkt[7] = dst[1];
                i--;
                pkt[2] = col;
                pkt[6] = 0;
                pkt[4] = 0;
                pkt[1] = e1;
                dst++;
                pkt[0] = *ot;
                ((char *)pkt)[3] = 7;
                *ot = (int)pkt;
                pkt += 8;
                ((char *)ot)[3] = 0;
            } while (i > 0);
            *(int **)0x1F800000 = pkt;
        }
    }
}

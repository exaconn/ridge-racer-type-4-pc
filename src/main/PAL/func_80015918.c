#include "rr_asm.h"

typedef struct {
    unsigned short i0;
    unsigned short i1;
    unsigned short i2;
    unsigned short i3;
    char pad08;
    signed char unk09;
    short pad0A;
} Tri0C;

void func_80015918(Tri0C *a0, int a1, char *a2) {
    int *spd = (int *)0x1F800000;
    int *pkt = (int *)spd[0];
    int c160 = *(int *)(a2 + 0x160) | 0x3A000000;
    int shift = spd[0x18] + 3;
    int v_base = *(int *)(a2 + 0xD0);
    int v_alt = *(int *)(a2 + 0xD4);
    int c164 = *(int *)(a2 + 0x164);
    int xor_val = *(int *)(a2 + 0xBC);
    int e1 = 0xE1000220;

    while (a1-- > 0) {
        int sxy0 = *(int *)((a0->i0 << 3) + v_base);
        int sxy1 = *(int *)((a0->i1 << 3) + v_base);
        int sxy2 = *(int *)((a0->i2 << 3) + v_base);
        int z;

        /* ASM_BARRIER: load SXY0..SXY2 into GTE */
        __asm__ volatile(
            "mtc2 %0, $12\n\t"
            "mtc2 %2, $14\n\t"
            "mtc2 %1, $13"
            :
            : "r"(sxy0), "r"(sxy1), "r"(sxy2)
        );
        z = *(int *)((a0->i3 << 3) + v_base + 4) >> shift;
        *(int *)(a2 + 0x70) = z;
        if ((unsigned int)(z - 1) < 0x1BF) {
            int x0, x1, x2, x3, x4, x5;
            int y0, y1, y2, y3, y4, y5;

            RR_GTE14_NCLIP();
            pkt[4] = sxy0;
            pkt[13] = sxy1;
            pkt[22] = sxy2;
            x0 = *(short *)((a0->i0 << 3) + v_base);
            x1 = *(short *)((a0->i1 << 3) + v_base);
            x2 = *(short *)((a0->i2 << 3) + v_base);
            x3 = *(short *)((a0->i0 << 3) + v_alt);
            x4 = *(short *)((a0->i1 << 3) + v_alt);
            x5 = *(short *)((a0->i2 << 3) + v_alt);
            RR_GTE14_SWC2_MAC0(a2 + 0x74);
            if ((*(int *)(a2 + 0x74) ^ xor_val) > 0) {
                if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0 || x4 >= 0 || x5 >= 0) &&
                    (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140 || x4 < 0x140 || x5 < 0x140)) {
                    y0 = *(short *)((a0->i0 << 3) + v_base + 2);
                    y1 = *(short *)((a0->i1 << 3) + v_base + 2);
                    y2 = *(short *)((a0->i2 << 3) + v_base + 2);
                    y3 = *(short *)((a0->i0 << 3) + v_alt + 2);
                    y4 = *(short *)((a0->i1 << 3) + v_alt + 2);
                    y5 = *(short *)((a0->i2 << 3) + v_alt + 2);
                    if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0 || y4 >= 0 || y5 >= 0) &&
                        (y0 < 0x100 || y1 < 0x100 || y2 < 0x100 || y3 < 0x100 || y4 < 0x100 || y5 < 0x100)) {
                        int s3_0, s3_1;
                        int *ot;

                        sxy0 = *(int *)((a0->i0 << 3) + v_alt);
                        sxy1 = *(int *)((a0->i1 << 3) + v_alt);
                        sxy2 = *(int *)((a0->i2 << 3) + v_alt);
                        pkt[8] = sxy0;
                        pkt[17] = sxy1;
                        pkt[26] = sxy2;
                        s3_0 = *(int *)((a0->i3 << 3) + v_base);
                        pkt[20] = s3_0;
                        pkt[11] = s3_0;
                        pkt[2] = s3_0;
                        s3_1 = *(int *)((a0->i3 << 3) + v_alt);
                        pkt[19] = c160;
                        pkt[10] = c160;
                        pkt[1] = c160;
                        pkt[23] = c164;
                        pkt[14] = c164;
                        pkt[5] = c164;
                        pkt[25] = 0;
                        pkt[16] = 0;
                        pkt[7] = 0;
                        pkt[21] = 0;
                        pkt[12] = 0;
                        pkt[3] = 0;
                        pkt[24] = s3_1;
                        pkt[15] = s3_1;
                        pkt[6] = s3_1;
                        ot = (int *)(*(int *)(a2 + 0x6C) + (*(int *)(a2 + 0x70) << 2) + (a0->unk09 << 2));
                        pkt[0] = *ot;
                        ((char *)pkt)[3] = 8;
                        *ot = (int)pkt;
                        ((char *)ot)[3] = 0;
                        pkt[9] = *ot;
                        ((char *)pkt)[0x27] = 8;
                        *ot = (int)(pkt + 9);
                        ((char *)ot)[3] = 0;
                        pkt[18] = *ot;
                        ((char *)pkt)[0x4B] = 8;
                        *ot = (int)(pkt + 18);
                        ((char *)ot)[3] = 0;
                        pkt += 27;
                        pkt[1] = e1;
                        pkt[0] = *ot;
                        ((char *)pkt)[3] = 1;
                        *ot = (int)pkt;
                        pkt += 2;
                        ((char *)ot)[3] = 0;
                    }
                }
            }
        }
        a0++;
    }
    *(int **)0x1F800000 = pkt;
}

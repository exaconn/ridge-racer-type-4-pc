#include "rr_asm.h"

typedef struct {
    int v01;
    int v23;
    int pad08;
    int c0;
    int c1;
    int c2;
    struct {
        short pad0;
        short c3;
        char pad4;
        signed char unk5;
        short pad6;
    } tail;
} Quad1C;

int *func_8001577C(int *a0, int a1, char *a2) {
    int verts = (int)(a2 + 0x1A0);
    volatile int *hw = (volatile int *)0x1F800000;
    int shift = hw[0x60 / 4] + 4;
    int *pkt = (int *)hw[0];
    int xor_val = *(int *)(a2 + 0xBC);

    while (a1-- > 0) {
        int v01 = a0[0];
        int v23 = a0[1];
        int i1 = v01 >> 16;
        int i0 = v01 & 0xFFFF;
        int i3 = v23 >> 16;
        int i2 = v23 & 0xFFFF;
        int *p0 = (int *)((i0 << 3) + verts);
        int *p1 = (int *)((i1 << 3) + verts);
        int *p2 = (int *)((i2 << 3) + verts);
        int *p3 = (int *)((i3 << 3) + verts);
        int sxy0 = p0[0];
        int sxy1 = p1[0];
        int sxy2 = p2[0];
        int sxy3 = p3[0];
        int z;

        /* ASM_BARRIER: load SXY0..SXY2 and run NCLIP */
        __asm__ volatile(
            "mtc2 %0, $12\n\t"
            "mtc2 %2, $14\n\t"
            "mtc2 %1, $13\n\t"
            "nop\n\t"
            "nop\n\t"
            ".word 0x4B400006"
            :
            : "r"(sxy0), "r"(sxy1), "r"(sxy2)
        );
        pkt[2] = sxy0;
        pkt[4] = sxy1;
        pkt[6] = sxy2;
        pkt[8] = sxy3;
        z = p0[1] + p1[1] + p2[1] + p3[1];
        RR_GTE14_SWC2_MAC0(a2 + 0x74);
        if ((*(int *)(a2 + 0x74) ^ xor_val) > 0 && z > 0) {
            z >>= shift;
            if (z < 0x1C0) {
                int *ot = (int *)(*(int *)(a2 + 0x6C) + (z << 2) + (((signed char *)a0)[0x19] << 2));
                pkt[0] = *ot;
                ((char *)pkt)[3] = 9;
                *ot = (int)pkt;
                ((char *)ot)[3] = 0;
                pkt[3] = a0[3];
                pkt[5] = a0[4];
                pkt[7] = a0[5];
                pkt[9] = *(short *)((char *)a0 + 0x16);
                /* ASM_BARRIER: make a0+0x14 the last giv in the loop so loop strength reduction picks a0+0x14 as the base */
                __asm__ volatile("" : : "m"(a0[5]));
                pkt[1] = *(int *)(a2 + 0x160);
                pkt += 10;
            }
        }
        a0 += 7;
    }
    *(int **)0x1F800000 = pkt;
    return a0;
}

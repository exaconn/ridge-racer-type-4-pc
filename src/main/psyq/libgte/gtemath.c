#include "rr_asm.h"

int RotTransPers(int *a0, int *a1, int *a2, int *a3) {
    int v, f;
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
    RR_GTE_CMD_1NOP(0x4A180001);
    RR_GTE_SWC2_AT(14, 0, a1);
    RR_GTE_SWC2_AT(8, 0, a2);
    RR_GTE_CFC2(f, 31);
    RR_GTE_MFC2(v, 19);
    *a3 = f;
    return v >> 2;
}

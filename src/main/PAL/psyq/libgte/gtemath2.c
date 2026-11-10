#include "rr_asm.h"
#include "psyq/libgte/gtemath2.h"
void RotTrans(int *a0, int *a1, int *a2) {
    int f;
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
    RR_GTE_CMD_1NOP(0x4A480012);
    RR_GTE_SWC2_TRIPLE_AT(25, 0, 26, 4, 27, 8, a1);
    RR_GTE_CFC2(f, 31);
    *a2 = f;
}

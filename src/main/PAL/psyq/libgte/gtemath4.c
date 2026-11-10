#include "rr_asm.h"
#include "psyq/libgte/gtemath4.h"

int RotAverage3(int *a0, int *a1, int *a2, int *a3, int *a4, int *a5, int *a6, int *a7) {
    int v;
    (void)a4; (void)a5; (void)a6; (void)a7;
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
    RR_GTE_LWC2_PAIR_AT(2, 0, 3, 4, a1);
    RR_GTE_LWC2_PAIR_AT(4, 0, 5, 4, a2);
    RR_GTE_CMD_1NOP(0x4A280030);
    RR_GTE_ROT_AVG3_STORE(a3);
    RR_GTE_OP(0x4B58002D);
    RR_GTE_MFC2(v, 7);
    return v;
}

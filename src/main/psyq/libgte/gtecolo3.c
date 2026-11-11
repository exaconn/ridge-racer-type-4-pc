/* Candidate module: GTE AverageZ3 / AverageZ4 wrappers, 80091498..800914B8. */
#include "rr_asm.h"
#include "psyq/libgte/gtecolo3.h"

void LocalLight(int *a0, int *a1) {
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
    RR_GTE_CMD_1NOP(0x4A4A6412);
    RR_GTE_SWC2_TRIPLE_AT(9, 0, 10, 4, 11, 8, a1);
}

void DpqColor(const GpuColorCommand *input, s32 factor, GpuColorCommand *output) {
    RR_GTE_LWC2_AT(6, 0, input);
    RR_GTE_MTC2(factor, 8);
    RR_GTE_CMD_1NOP(0x4A780010);
    RR_GTE_SWC2_AT(22, 0, output);
}

void NormalColor(int *a0, int *a1) {
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
    RR_GTE_CMD_1NOP(0x4AC8041E);
    RR_GTE_SWC2_AT(22, 0, a1);
}

void NormalColor3(int *a0, int *a1, int *a2, int *a3, int *a4, int *a5) {
    (void)a4; (void)a5;
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
    RR_GTE_LWC2_PAIR_AT(2, 0, 3, 4, a1);
    RR_GTE_LWC2_PAIR_AT(4, 0, 5, 4, a2);
    RR_GTE_CMD_1NOP(0x4AD80420);
    RR_GTE_SWC2_RGB012_A3_SP16_20(a3);
}

void NormalColorDpq(int *a0, int *a1, int a2, int *a3) {
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
    RR_GTE_LWC2_AT(6, 0, a1);
    RR_GTE_MTC2(a2, 8);
    RR_GTE_CMD_1NOP(0x4AE80413);
    RR_GTE_SWC2_AT(22, 0, a3);
}

void NormalColorDpq3(int *a0, int *a1, int *a2, int *a3, int a4, int *a5, int *a6, int *a7) {
    (void)a4; (void)a5; (void)a6; (void)a7;
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
    RR_GTE_LWC2_PAIR_AT(2, 0, 3, 4, a1);
    RR_GTE_LWC2_PAIR_AT(4, 0, 5, 4, a2);
    RR_GTE_LWC2_AT(6, 0, a3);
    RR_GTE_LWC2_SP(8, 16);
    RR_GTE_CMD_1NOP(0x4AF80416);
    RR_GTE_SWC2_RGB012_SP(20, 24, 28);
}

void NormalColorCol(int *a0, int *a1, int *a2) {
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
    RR_GTE_LWC2_AT(6, 0, a1);
    RR_GTE_CMD_1NOP(0x4B08041B);
    RR_GTE_SWC2_AT(22, 0, a2);
}

void NormalColorCol3(int *a0, int *a1, int *a2, int *a3, int *a4, int *a5, int *a6) {
    (void)a4; (void)a5; (void)a6;
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
    RR_GTE_LWC2_PAIR_AT(2, 0, 3, 4, a1);
    RR_GTE_LWC2_PAIR_AT(4, 0, 5, 4, a2);
    RR_GTE_LWC2_AT(6, 0, a3);
    RR_GTE_CMD_1NOP(0x4B18043F);
    RR_GTE_SWC2_RGB012_SP(16, 20, 24);
}

void ColorDpq(int *a0, int *a1, int a2, int *a3) {
    RR_GTE_LWC2_IR(a0);
    RR_GTE_LWC2_AT(6, 0, a1);
    RR_GTE_MTC2(a2, 8);
    RR_GTE_CMD_1NOP(0x4B280414);
    RR_GTE_SWC2_AT(22, 0, a3);
}

void ColorCol(int *a0, int *a1, int *a2) {
    RR_GTE_LWC2_IR(a0);
    RR_GTE_LWC2_AT(6, 0, a1);
    RR_GTE_CMD_1NOP(0x4B38041C);
    RR_GTE_SWC2_AT(22, 0, a2);
}

int GteAvsz3(void) {
    int v;
    RR_GTE_OP(0x4B58002D);
    RR_GTE_MFC2(v, 7);
    return v;
}

int GteAvsz4(void) {
    int v;
    RR_GTE_OP(0x4B68002E);
    RR_GTE_MFC2(v, 7);
    return v;
}

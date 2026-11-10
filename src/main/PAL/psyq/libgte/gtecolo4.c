#include "rr/geometry.h"
#include "rr_asm.h"
#include "psyq/libgte/gtecolo4.h"

void LightColor(int *a0, int *a1) {
    RR_GTE_LWC2_IR(a0);
    RR_GTE_CMD_1NOP(0x4A4DA412);
    RR_GTE_SWC2_TRIPLE_AT(9, 0, 10, 4, 11, 8, a1);
}

void DpqColorLight(int *a0, int *a1, int a2, int *a3) {
    RR_GTE_LWC2_IR(a0);
    RR_GTE_LWC2_AT(6, 0, a1);
    RR_GTE_MTC2(a2, 8);
    RR_GTE_CMD_1NOP(0x4A680029);
    RR_GTE_SWC2_AT(22, 0, a3);
}

void DpqColor3(int *a0, int *a1, int *a2, int a3, int *a4, int *a5, int *a6) {
    (void)a4; (void)a5; (void)a6;
    RR_GTE_LWC2_AT(20, 0, a0);
    RR_GTE_LWC2_AT(21, 0, a1);
    RR_GTE_LWC2_AT(22, 0, a2);
    RR_GTE_LWC2_AT(6, 0, a2);
    RR_GTE_MTC2(a3, 8);
    RR_GTE_CMD_1NOP(0x4AF8002A);
    RR_GTE_SWC2_RGB012_SP(16, 20, 24);
}

void Intpl(int *a0, int a1, int *a2) {
    RR_GTE_LWC2_IR(a0);
    RR_GTE_MTC2(a1, 8);
    RR_GTE_CMD_1NOP(0x4A980011);
    RR_GTE_SWC2_AT(22, 0, a2);
}

Vec3i *Square12(const Vec3i *input,Vec3i *output) {
    RR_GTE_LWC2_IR(input);
    RR_GTE_CMD_1NOP(0x4AA80428);
    RR_GTE_SWC2_MAC123(output);
    /* ASM_BARRIER: Finish hardware stores before forming the returned pointer. */
    __asm__("" : : "r"(output) : "$2");
    return output;
}

Vec3i *Square0(const Vec3i *input,Vec3i *output) {
    RR_GTE_LWC2_IR(input);
    RR_GTE_CMD_1NOP(0x4AA00428);
    RR_GTE_SWC2_MAC123(output);
    /* ASM_BARRIER: Finish hardware stores before forming the returned pointer. */
    __asm__("" : : "r"(output) : "$2");
    return output;
}

int AverageZ3(int a0, int a1, int a2) {
    int v;
    RR_GTE_MTC2(a0, 17);
    RR_GTE_MTC2(a1, 18);
    RR_GTE_MTC2(a2, 19);
    RR_GTE_CMD_1NOP(0x4B58002D);
    RR_GTE_MFC2(v, 7);
    return v;
}

int AverageZ4(int a0, int a1, int a2, int a3) {
    int v;
    RR_GTE_MTC2(a0, 16);
    RR_GTE_MTC2(a1, 17);
    RR_GTE_MTC2(a2, 18);
    RR_GTE_MTC2(a3, 19);
    RR_GTE_CMD_1NOP(0x4B68002E);
    RR_GTE_MFC2(v, 7);
    return v;
}

void OuterProduct12(int *a0, int *a1, int *a2) {
    RR_GTE_OUTER_PRODUCT(a0, a1, a2, 0x4B78000C);
}

void OuterProduct0(int *a0, int *a1, int *a2) {
    RR_GTE_OUTER_PRODUCT(a0, a1, a2, 0x4B70000C);
}

int Lzc(int a0) {
    int v;
    RR_GTE_MTC2(a0, 30);
    RR_GTE14_NOP2();
    RR_GTE_MFC2(v, 31);
    return v;
}

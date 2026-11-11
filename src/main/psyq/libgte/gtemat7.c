#include "rr_asm.h"
#include "psyq/libgte/gtemat7.h"
void SetVertex0(int *a0);
void SetVertex1(int *a0);
void SetVertex2(int *a0);
void SetVertexTri(int *a0, int *a1, int *a2);
void SetRGBfifo(int *a0, int *a1, int *a2);
void SetIR123(int a0, int a1, int a2);
void SetIR0(int a0);
void SetSZfifo3(int a0, int a1, int a2);
void SetSZfifo4(int a0, int a1, int a2, int a3);
void SetSXSYfifo(int a0, int a1, int a2);
void SetRii(int a0, int a1, int a2);
void SetMAC123(int a0, int a1, int a2);
void SetData32(int a0);
void SetGeomOffset(int a0, int a1);

void SetRotMatrix(int *a0) {
    RR_GTE_SETROT_5W(a0);
}

void SetLightMatrix(const MatrixWords32 *matrix) {
    RR_GTE_SETLIGHT_5W(matrix);
}

void SetColorMatrix(const MatrixWords32 *matrix) {
    RR_GTE_SETCOL_5W(matrix);
}

void SetTransMatrix(int *a0) {
    RR_GTE_SETTR_3W(a0);
}

void SetVertex0(int *a0) {
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
}

void SetVertex1(int *a0) {
    RR_GTE_LWC2_PAIR_AT(2, 0, 3, 4, a0);
}

void SetVertex2(int *a0) {
    RR_GTE_LWC2_PAIR_AT(4, 0, 5, 4, a0);
}

void SetVertexTri(int *a0, int *a1, int *a2) {
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, a0);
    RR_GTE_LWC2_PAIR_AT(2, 0, 3, 4, a1);
    RR_GTE_LWC2_PAIR_AT(4, 0, 5, 4, a2);
}

void SetRGBfifo(int *a0, int *a1, int *a2) {
    RR_GTE_LWC2_AT(20, 0, a0);
    RR_GTE_LWC2_AT(21, 0, a1);
    RR_GTE_LWC2_AT(22, 0, a2);
}

void SetIR123(int a0, int a1, int a2) {
    RR_GTE_MTC2(a0, 9);
    RR_GTE_MTC2(a1, 10);
    RR_GTE_MTC2(a2, 11);
}

void SetIR0(int a0) {
    RR_GTE_MTC2(a0, 8);
}

void SetSZfifo3(int a0, int a1, int a2) {
    RR_GTE_MTC2(a0, 17);
    RR_GTE_MTC2(a1, 18);
    RR_GTE_MTC2(a2, 19);
}

void SetSZfifo4(int a0, int a1, int a2, int a3) {
    RR_GTE_MTC2(a0, 16);
    RR_GTE_MTC2(a1, 17);
    RR_GTE_MTC2(a2, 18);
    RR_GTE_MTC2(a3, 19);
}

void SetSXSYfifo(int a0, int a1, int a2) {
    RR_GTE_MTC2(a0, 12);
    RR_GTE_MTC2(a1, 13);
    RR_GTE_MTC2(a2, 14);
}

void SetRii(int a0, int a1, int a2) {
    RR_GTE_CTC2(a0, 0);
    RR_GTE_CTC2(a1, 2);
    RR_GTE_CTC2(a2, 4);
}

void SetMAC123(int a0, int a1, int a2) {
    RR_GTE_MTC2(a0, 25);
    RR_GTE_MTC2(a1, 26);
    RR_GTE_MTC2(a2, 27);
}

void SetData32(int a0) {
    RR_GTE_MTC2(a0, 30);
}

void SetDQA(int a0) {
    RR_GTE_CTC2(a0, 27);
}

void SetDQB(int a0) {
    RR_GTE_CTC2(a0, 28);
}

void SetBackColor(s32 red, s32 green, s32 blue) {
    red = red * 16U;
    green = green * 16U;
    blue = blue * 16U;
    RR_GTE_CTC2(red, 13);
    RR_GTE_CTC2(green, 14);
    RR_GTE_CTC2(blue, 15);
}

void SetFarColor(s32 red, s32 green, s32 blue) {
    red = red * 16U;
    green = green * 16U;
    blue = blue * 16U;
    RR_GTE_CTC2(red, 21);
    RR_GTE_CTC2(green, 22);
    RR_GTE_CTC2(blue, 23);
}

void SetGeomOffset(int a0, int a1) {
    a0 <<= 16; a1 <<= 16;
    RR_GTE_CTC2(a0, 24);
    RR_GTE_CTC2(a1, 25);
}

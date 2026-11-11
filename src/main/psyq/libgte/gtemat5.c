#include "psyq/libgte/gtemat5.h"
#include "rr_asm.h"


Vec3i *ApplyMatrix(const MatrixWords32 *matrix, const Vec3s8 *input, Vec3i *output) {
    Vec3i *r;
    RR_GTE_SETROT_5W(matrix);
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, input);
    RR_GTE_MVMVA_ROT_V0_1NOP();
    RR_GTE_SWC2_TRIPLE_AT(25, 0, 26, 4, 27, 8, output);
    return r;
}

s16 *ApplyMatrixSV(const MatrixWords32 *matrix, const Vec3s8 *input, s16 output[3]) {
    s16 *r;
    RR_GTE_SETROT_5W(matrix);
    RR_GTE_LWC2_PAIR_AT(0, 0, 1, 4, input);
    RR_GTE_MVMVA_ROT_V0_1NOP();
    RR_GTE_STORE_IR_S16_T012(output);
    return r;
}

/* libgte matrix helpers the game calls that PsyZ does not implement:
 * MulMatrix0, MulRotMatrix0, MulMatrix2, ApplyMatrixLV and ApplyMatrixSV.
 * They run on the PsyZ GTE so the register side effects of the original
 * routines (rotation matrix, IR/MAC, FLAG) are preserved. */
#include "rr_asm.h"
#include "psyq/libgte/gtemat.h"
#include "psyq/libgte/gtemat2.h"
#include "psyq/libgte/gtemat3.h"
#include "psyq/libgte/gtemat4.h"
#include "psyq/libgte/gtemat5.h"

#define GTE_MVMVA_SF_ROT_V0 0x0486012u  /* MVMVA sf=1 RT*V0, no translation */
#define GTE_MVMVA_SF_ROT_IR 0x049E012u  /* MVMVA sf=1 RT*IR, no translation */
#define GTE_MVMVA_ROT_IR 0x041E012u     /* MVMVA sf=0 RT*IR, no translation */

static void LoadRotation(const MatrixWords32 *matrix) {
    int i;
    for (i = 0; i < 5; i++) {
        Psyz_GteCtrlWrite(i, (u32)matrix->w[i]);
    }
}

/* output.rotation = RT * right.rotation, column by column. Every input
 * column is read before the first store, so right and output may alias.
 * Word 4 receives the raw IR3 of the last column, as the SDK does. */
static MatrixWords32 *MulRotationColumns(const MatrixWords32 *right, MatrixWords32 *output) {
    u32 ir[3][3];
    int column;
    for (column = 0; column < 3; column++) {
        u16 x = (u16)right->fields.rotation00[0][column];
        u16 y = (u16)right->fields.rotation00[1][column];
        s16 z = right->fields.rotation00[2][column];
        Psyz_GteDataWrite(0, (u32)x | ((u32)y << 16));
        Psyz_GteDataWrite(1, (u32)(s32)z);
        Psyz_GteCommand(GTE_MVMVA_SF_ROT_V0);
        ir[column][0] = Psyz_GteDataRead(9);
        ir[column][1] = Psyz_GteDataRead(10);
        ir[column][2] = Psyz_GteDataRead(11);
    }
    output->w[0] = (s32)((ir[0][0] & 0xFFFF) | (ir[1][0] << 16));
    output->w[1] = (s32)((ir[2][0] & 0xFFFF) | (ir[0][1] << 16));
    output->w[2] = (s32)((ir[1][1] & 0xFFFF) | (ir[2][1] << 16));
    output->w[3] = (s32)((ir[0][2] & 0xFFFF) | (ir[1][2] << 16));
    output->w[4] = (s32)ir[2][2];
    return output;
}

MatrixWords32 *MulMatrix0(const MatrixWords32 *basis, const MatrixWords32 *right, MatrixWords32 *output) {
    LoadRotation(basis);
    return MulRotationColumns(right, output);
}

MatrixWords32 *MulRotMatrix0(const MatrixWords32 *right, MatrixWords32 *output) {
    return MulRotationColumns(right, output);
}

MatrixWords32 *MulMatrix2(const MatrixWords32 *basis, MatrixWords32 *right) {
    LoadRotation(basis);
    return MulRotationColumns(right, right);
}

/* Full-word vector: each component is split into a signed high part
 * (|v| >> 15) and low part (|v| & 0x7FFF) carrying the sign of v. */
Vec3i *ApplyMatrixLV(const MatrixWords32 *matrix, const Vec3i *input, Vec3i *output) {
    s32 value[3];
    s32 high[3];
    s32 low[3];
    s32 product;
    int i;
    LoadRotation(matrix);
    value[0] = input->x;
    value[1] = input->y;
    value[2] = input->z;
    for (i = 0; i < 3; i++) {
        u32 magnitude = value[i] < 0 ? 0u - (u32)value[i] : (u32)value[i];
        high[i] = (s32)(magnitude >> 15);
        low[i] = (s32)(magnitude & 0x7FFF);
        if (value[i] < 0) {
            high[i] = -high[i];
            low[i] = -low[i];
        }
    }
    for (i = 0; i < 3; i++) {
        Psyz_GteDataWrite(9 + i, (u32)high[i]);
    }
    Psyz_GteCommand(GTE_MVMVA_ROT_IR);
    for (i = 0; i < 3; i++) {
        high[i] = (s32)Psyz_GteDataRead(25 + i);
    }
    for (i = 0; i < 3; i++) {
        Psyz_GteDataWrite(9 + i, (u32)low[i]);
    }
    Psyz_GteCommand(GTE_MVMVA_SF_ROT_IR);
    for (i = 0; i < 3; i++) {
        product = high[i] < 0 ? -(s32)((u32)-high[i] << 3) : (s32)((u32)high[i] << 3);
        value[i] = (s32)((u32)Psyz_GteDataRead(25 + i) + (u32)product);
    }
    output->x = value[0];
    output->y = value[1];
    output->z = value[2];
    return output;
}

s16 *ApplyMatrixSV(const MatrixWords32 *matrix, const Vec3s8 *input, s16 output[3]) {
    LoadRotation(matrix);
    Psyz_GteDataWrite(0, input->words[0]);
    Psyz_GteDataWrite(1, input->words[1]);
    Psyz_GteCommand(GTE_MVMVA_SF_ROT_V0);
    output[0] = (s16)Psyz_GteDataRead(9);
    output[1] = (s16)Psyz_GteDataRead(10);
    output[2] = (s16)Psyz_GteDataRead(11);
    return output;
}

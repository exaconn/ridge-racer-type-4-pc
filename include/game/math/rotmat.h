#ifndef RR_GAME_MATH_ROTMAT_H
#define RR_GAME_MATH_ROTMAT_H

#include "types.h"
#include "rr/matrix_words.h"

/* Angles use 4096 units per turn. Only rotation00 is written; the alignment
 * halfword and translation14 retain their existing values. */
void BuildRotMatrixZ(MatrixWords32 *matrix, s32 angle);
void BuildRotMatrixY(MatrixWords32 *matrix, s32 angle);
void BuildRotMatrixX(MatrixWords32 *matrix, s32 angle);
void SetIdentityMatrix(MatrixWords32 *matrix);
void BuildCameraMatrices(void);
void BuildRotMatrixAngles(int *a0);
s32 Atan2Table(int a0, int a1);

#endif /* RR_GAME_MATH_ROTMAT_H */

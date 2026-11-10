#ifndef RR_GAME_MATH_ROTMAT_H
#define RR_GAME_MATH_ROTMAT_H

#include "types.h"
#include "rr/matrix_words.h"

void BuildRotMatrixZ();
void BuildRotMatrixY();
void BuildRotMatrixX();
void SetIdentityMatrix(MatrixWords32 *matrix);
void BuildCameraMatrices(void);
void BuildRotMatrixAngles(int *a0);
s32 Atan2Table(int a0, int a1);

#endif /* RR_GAME_MATH_ROTMAT_H */

#ifndef RR_PSYQ_LIBGTE_ROTMATZ_H
#define RR_PSYQ_LIBGTE_ROTMATZ_H

#include "rr/matrix_words.h"

/* Rotates the existing matrix in place and returns the same owner. */
MatrixWords32 *RotMatrixZ(s32 angle, MatrixWords32 *matrix);

#endif

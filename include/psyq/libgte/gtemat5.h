#ifndef RR_PSYQ_LIBGTE_GTEMAT5_H
#define RR_PSYQ_LIBGTE_GTEMAT5_H

#include "rr/matrix_words.h"
#include "rr/geometry.h"

/* The input is two packed words (x,y,z,padding), aligned for word loads.
 * ApplyMatrix returns the three full MAC words; ApplyMatrixSV returns three
 * saturated IR halfwords. Only those output components are written. */
Vec3i *ApplyMatrix(const MatrixWords32 *matrix, const Vec3s8 *input, Vec3i *output);
s16 *ApplyMatrixSV(const MatrixWords32 *matrix, const Vec3s8 *input, s16 output[3]);

#endif

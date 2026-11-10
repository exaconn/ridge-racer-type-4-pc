#ifndef RR_PSYQ_LIBGTE_GTEMAT3_H
#define RR_PSYQ_LIBGTE_GTEMAT3_H

#include "types.h"
#include "rr/matrix_words.h"
#include "rr/geometry.h"

/* Full-word input is split into signed high/low parts before GTE multiplication.
 * Three full output words are written; the returned pointer is output. */
Vec3i *ApplyMatrixLV(const MatrixWords32 *matrix, const Vec3i *input, Vec3i *output);
/* Uses the current GTE rotation and writes three sign-extended saturated IRs. */
Vec3i *ApplyRotMatrix(const Vec3s8 *input, Vec3i *output);
/* Replace basis's rotation with basis * right. Returns basis; translation stays. */
MatrixWords32 *MulMatrix(MatrixWords32 *basis, const MatrixWords32 *right);

#endif /* RR_PSYQ_LIBGTE_GTEMAT3_H */

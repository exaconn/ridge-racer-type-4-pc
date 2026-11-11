#ifndef RR_PSYQ_LIBGTE_GTEMAT3_H
#define RR_PSYQ_LIBGTE_GTEMAT3_H

#include "types.h"
#include "rr/matrix_words.h"
#include "rr/geometry.h"

/* Not in PsyZ (src/main/psyq/libgte/gtemat.c). Full-word input is split into
 * signed high/low parts before GTE multiplication. Returns output. */
Vec3i *ApplyMatrixLV(const MatrixWords32 *matrix, const Vec3i *input, Vec3i *output);
/* PsyZ: void ApplyRotMatrix(SVECTOR *v0, VECTOR *v1). Uses the current GTE
 * rotation and writes three sign-extended saturated IRs. */
void ApplyRotMatrix(const Vec3s8 *input, Vec3i *output);
/* PsyZ: MATRIX *MulMatrix(MATRIX *m0, MATRIX *m1). Replace basis's rotation
 * with basis * right. Returns basis; translation stays. */
MatrixWords32 *MulMatrix(MatrixWords32 *basis, const MatrixWords32 *right);

#endif /* RR_PSYQ_LIBGTE_GTEMAT3_H */

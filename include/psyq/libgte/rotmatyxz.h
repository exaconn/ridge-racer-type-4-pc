#ifndef RR_PSYQ_LIBGTE_ROTMATYXZ_H
#define RR_PSYQ_LIBGTE_ROTMATYXZ_H
#include "rr/geometry.h"
#include "rr/matrix_words.h"

/* PsyZ: MATRIX *RotMatrixYXZ(SVECTOR *r, MATRIX *m); returns m. */
MatrixWords32 *RotMatrixYXZ(const Vec3sFields8 *angles, MatrixWords32 *output);
#endif

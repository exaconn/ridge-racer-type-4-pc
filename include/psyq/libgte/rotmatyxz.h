#ifndef RR_PSYQ_LIBGTE_ROTMATYXZ_H
#define RR_PSYQ_LIBGTE_ROTMATYXZ_H
#include "rr/geometry.h"
#include "rr/matrix_words.h"

/* Handwritten SDK routine: returns the output pointer in v0 (PAL+4). */
MatrixWords32 *RotMatrixYXZ(const Vec3sFields8 *angles, MatrixWords32 *output);
#endif

#ifndef RR_PSYQ_LIBGTE_ROTMATZ_H
#define RR_PSYQ_LIBGTE_ROTMATZ_H

#include "rr/matrix_words.h"

/* PsyZ: MATRIX *RotMatrixZ(long r, MATRIX *m). Rotates in place, returns m. */
MatrixWords32 *RotMatrixZ(long angle, MatrixWords32 *matrix);

#endif

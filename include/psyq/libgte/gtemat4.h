#ifndef RR_PSYQ_LIBGTE_GTEMAT4_H
#define RR_PSYQ_LIBGTE_GTEMAT4_H
#include "rr/matrix_words.h"

/* Not in PsyZ (src/main/psyq/libgte/gtemat.c). Replace right's rotation with
 * basis * right, retaining its translation. Both inputs may name the same
 * matrix. Returns right. */
MatrixWords32 *MulMatrix2(const MatrixWords32 *basis, MatrixWords32 *right);
#endif

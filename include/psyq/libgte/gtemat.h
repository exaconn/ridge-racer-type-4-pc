#ifndef RR_PSYQ_LIBGTE_GTEMAT_H
#define RR_PSYQ_LIBGTE_GTEMAT_H
#include "rr/matrix_words.h"

/* Products write the first five words, including the rotation padding half;
 * the three translation words retain their previous values. Returns output. */
MatrixWords32 *MulMatrix0(const MatrixWords32 *basis,
                           const MatrixWords32 *right, MatrixWords32 *output);
#endif

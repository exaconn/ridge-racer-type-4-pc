#ifndef RR_PSYQ_LIBGTE_GTEMAT_H
#define RR_PSYQ_LIBGTE_GTEMAT_H
#include "rr/matrix_words.h"

/* PsyZ declares MulMatrix0 but does not implement it; the game copy lives in
 * src/main/psyq/libgte/gtemat.c. Products write the first five words,
 * including the rotation padding half; the translation words are kept.
 * Returns output. */
MatrixWords32 *MulMatrix0(const MatrixWords32 *basis,
                           const MatrixWords32 *right, MatrixWords32 *output);
#endif

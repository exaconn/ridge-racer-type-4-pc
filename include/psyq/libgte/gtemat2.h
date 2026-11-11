#ifndef RR_PSYQ_LIBGTE_GTEMAT2_H
#define RR_PSYQ_LIBGTE_GTEMAT2_H

#include "rr/matrix_words.h"

/* Not in PsyZ (src/main/psyq/libgte/gtemat.c). Multiply by the current GTE
 * rotation; retain output's translation words. Returns output. */
MatrixWords32 *MulRotMatrix0(const MatrixWords32 *right, MatrixWords32 *output);

#endif /* RR_PSYQ_LIBGTE_GTEMAT2_H */

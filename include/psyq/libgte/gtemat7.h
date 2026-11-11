#ifndef RR_PSYQ_LIBGTE_GTEMAT7_H
#define RR_PSYQ_LIBGTE_GTEMAT7_H

#include "types.h"
#include "rr/matrix_words.h"

/* PsyZ libgte (MATRIX * arguments, long colour components). */
void SetRotMatrix(int *matrix);
void SetLightMatrix(const MatrixWords32 *matrix);
void SetColorMatrix(const MatrixWords32 *matrix);
void SetTransMatrix(int *matrix);
void SetBackColor(long red, long green, long blue);
void SetFarColor(long red, long green, long blue);

#endif /* RR_PSYQ_LIBGTE_GTEMAT7_H */

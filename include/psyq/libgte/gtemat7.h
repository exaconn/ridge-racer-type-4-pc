#ifndef RR_PSYQ_LIBGTE_GTEMAT7_H
#define RR_PSYQ_LIBGTE_GTEMAT7_H

#include "types.h"
#include "rr/matrix_words.h"

void SetRotMatrix(int *a0);
void SetLightMatrix(const MatrixWords32 *matrix);
/* Five words are transferred; GTE retains the signed low half of the last. */
void SetColorMatrix(const MatrixWords32 *matrix);
void SetTransMatrix(int *a0);
void SetDQA(int a0);
void SetDQB(int a0);
void SetBackColor(s32 red, s32 green, s32 blue);
void SetFarColor(s32 red, s32 green, s32 blue);

#endif /* RR_PSYQ_LIBGTE_GTEMAT7_H */

#ifndef RR_PSYQ_LIBGTE_GTEMAT7_H
#define RR_PSYQ_LIBGTE_GTEMAT7_H

#include "types.h"

void SetRotMatrix(int *a0);
void SetColorMatrix(int *a0);
void SetTransMatrix(int *a0);
void SetDQA(int a0);
void SetDQB(int a0);
int SetBackColor();
void SetFarColor(int a0, int a1, int a2);

#endif /* RR_PSYQ_LIBGTE_GTEMAT7_H */

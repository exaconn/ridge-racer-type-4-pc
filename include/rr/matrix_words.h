#ifndef RR_MATRIX_WORDS_H
#define RR_MATRIX_WORDS_H
#include "rr/layout.h"
/* libgte MATRIX: nine rotation halfwords, alignment halfword and translation. */
typedef struct {
    s16 rotation00[3][3], reserved12;
    s32 translation14[3];
} MatrixFields32;
typedef union {
    s32 w[8];
    u16 halves[16];
    MatrixFields32 fields;
} MatrixWords32;
RR_ASSERT_SIZE(MatrixWords32, 0x20);
RR_ASSERT_OFFSET(MatrixFields32, translation14, 0x14);
#endif

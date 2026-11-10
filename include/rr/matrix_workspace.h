#ifndef RR_MATRIX_WORKSPACE_H
#define RR_MATRIX_WORKSPACE_H
#include "rr/layout.h"
#include "rr/matrix_words.h"
#include "psyq/libgte/gtemat4.h"
#include "psyq/libgte/gtemat7.h"

/* Matrix storage is32 bytes/aligned4. The GTE rotation view is the first
 * nine halfwords; word operations may also touch its following padding half. */
typedef union {
    u32 words[8];
    u16 halves[16];
    MatrixWords32 matrix;
} PreviewMatrixWorkspace;
extern PreviewMatrixWorkspace LightMatrix;
RR_ASSERT_SIZE(PreviewMatrixWorkspace,32);
#endif

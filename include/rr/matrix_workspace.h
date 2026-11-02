#ifndef RR_MATRIX_WORKSPACE_H
#define RR_MATRIX_WORKSPACE_H
#include "rr/layout.h"
#include "rr/matrix_words.h"

/* Matrix storage is32 bytes/aligned4. The GTE rotation view is the first
 * nine halfwords; word operations may also touch its following padding half. */
typedef union {u32 words[8];u16 halves[16];RrMatrixWords32 matrix;} RrPreviewMatrixWorkspace;
extern RrPreviewMatrixWorkspace D_800ABED0;
s32 *func_80090DB4(s32 *basis,u16 *matrix);
void func_800910E4(s32 *matrix);
RR_ASSERT_SIZE(RrPreviewMatrixWorkspace,32);
#endif

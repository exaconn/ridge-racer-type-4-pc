#ifndef RR_PSYQ_LIBGTE_ROTMAT_H
#define RR_PSYQ_LIBGTE_ROTMAT_H
#include "rr/geometry.h"
#include "rr/matrix_words.h"

/* Signed halfword angles, with 4096 units per turn. Only x/y/z are read; the
 * input padding halfword is ignored. Writes the nine rotation halfwords,
 * preserves the matrix padding/translation, and returns output. Input and
 * output may overlap; all three angles are captured before the first store.
 * The PAL implementation is original hand-written assembly. */
MatrixWords32 *RotMatrix(const Vec3sFields8 *angles, MatrixWords32 *output);

#endif

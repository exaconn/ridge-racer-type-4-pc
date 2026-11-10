#ifndef RR_OBJECT_TRANSFORM_TYPES_H
#define RR_OBJECT_TRANSFORM_TYPES_H

#include "rr/layout.h"
#include "rr/matrix_words.h"
#include "rr/geometry.h"

/* Object-local translation, its rotated result and the translation consumed by
 * SetTransMatrix. The renderer reuses the intervening words for lighting. */
typedef struct {
    Vec3s8 relative00;
    Vec3i transformed08;
    u8 reserved14[4];
    MatrixWords32 matrix18;
} ObjectTransformFields38;
typedef union {
    ObjectTransformFields38 fields;
    s16 halves[28];
    s32 words[14];
} ObjectTransform38;

RR_ASSERT_SIZE(ObjectTransform38, 0x38);
RR_ASSERT_OFFSET(ObjectTransformFields38, transformed08, 8);
RR_ASSERT_OFFSET(ObjectTransformFields38, matrix18, 0x18);

#endif

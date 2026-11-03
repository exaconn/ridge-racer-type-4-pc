#ifndef RR_GEOMETRY_H
#define RR_GEOMETRY_H

#include "rr/layout.h"

/* Small fixed-width vectors and table pairs shared by matching C units. */

typedef struct {
    s16 x, y;
} RrVec2s;

/* Three word-sized components; GTE IR loads consume each word's signed low 16 bits. */
typedef struct {
    s32 x, y, z;
} RrVec3i;

/* Full-word libgte vector layout. The fourth word is reserved by the SDK. */
typedef struct {
    s32 x, y, z, pad;
} RrVector;

/* SQR hardware helpers return MAC1..MAC3, with shift 12 and shift 0 respectively.
 * Input and output may overlap. GTE IR/MAC registers and FLAG are also modified. */
RrVec3i *func_80091574(const RrVec3i *input, RrVec3i *output);
RrVec3i *func_8009159C(const RrVec3i *input, RrVec3i *output);

RR_ASSERT_SIZE(RrVec3i, 12);
RR_ASSERT_SIZE(RrVector, 16);
RR_ASSERT_OFFSET(RrVector, z, 8);
RR_ASSERT_OFFSET(RrVec3i, y, 4);
RR_ASSERT_OFFSET(RrVec3i, z, 8);

typedef struct RrPairS32 {
    s32 a, b;
} RrPairS32;

typedef struct RrPairS16 {
    s16 a, b;
} RrPairS16;

RR_ASSERT_SIZE(RrVec2s, 0x4);
RR_ASSERT_SIZE(RrPairS32, 0x8);
RR_ASSERT_SIZE(RrPairS16, 0x4);

#endif

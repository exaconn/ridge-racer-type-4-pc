#ifndef GAME_CAR_BOUNDS_H
#define GAME_CAR_BOUNDS_H

#include "rr/layout.h"

/* Dimensions are stored in quarter-world units. The rendering code also uses
 * the wheel offset and rotation axis from the same model-bank record. */
typedef struct {
    s16 halfWidth;
    u16 wheelOffsetY;
    u16 halfLength;
    s16 bodyHeight;
    s16 unk08;
    u8 wheelRotationAxis;
    u8 reserved0B;
} CarBounds;

extern CarBounds *CarBoundsTable[4]; /* 0x800F3A08, one per player car slot */

RR_ASSERT_SIZE(CarBounds, 0xC);
RR_ASSERT_OFFSET(CarBounds, bodyHeight, 6);
RR_ASSERT_OFFSET(CarBounds, wheelRotationAxis, 0xA);

#endif

#ifndef RR_BADGE_ROW_TYPES_H
#define RR_BADGE_ROW_TYPES_H

#include "rr/layout.h"

typedef struct {
    u16 status00;
    s16 half02;
    s16 x04, y06, x08, y0A, width0C;
    s16 extent0E;
    s16 state10;
    u8 pad12[2];
    s16 x14, y16, w18, h1A;
} BadgeRow1C;

#endif

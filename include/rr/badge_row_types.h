#ifndef RR_BADGE_ROW_TYPES_H
#define RR_BADGE_ROW_TYPES_H

#include "rr/layout.h"

/* Animation phases inferred from the row controller. */
enum {
    BADGE_ANIM_INITIALIZE = 0,
    BADGE_ANIM_REVEAL_WIDTH = 1,
    BADGE_ANIM_REVEAL_HEIGHT = 2,
    BADGE_ANIM_VISIBLE = 3,
    BADGE_ANIM_HIDE_HEIGHT = 4,
    BADGE_ANIM_HIDE_WIDTH = 5,
    BADGE_ANIM_REVEAL_WIDTH_FULL_HEIGHT = 7,
    BADGE_ANIM_REVEAL_HEIGHT_CENTERED = 8,
    BADGE_ANIM_WAIT_FOR_ENABLE = 10,
    BADGE_ANIM_HIDDEN = 11
};

typedef struct {
    u16 status00;
    s16 half02;
    s16 x04, y06, x08, y0A, width0C;
    s16 extent0E;
    s16 state10;
    s16 animationTick12;
    s16 x14, y16, w18, h1A;
} BadgeRow1C;

#endif

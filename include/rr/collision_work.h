#ifndef RR_COLLISION_WORK_H
#define RR_COLLISION_WORK_H

#include "rr/geometry.h"

/* NormalClip consumes each signed x/y pair as one packed GTE input word. */
typedef union {
    Vec2s pt;
    s32 word;
} CollisionPoint;

/* The controller probes center, corners and edge midpoints in this order.
 * The corner perimeter used by PointInQuadPtr is 2,3,1,0. */
enum {
    COLLISION_CENTER,
    COLLISION_CORNER0, COLLISION_CORNER1, COLLISION_CORNER2, COLLISION_CORNER3,
    COLLISION_EDGE01, COLLISION_EDGE02, COLLISION_EDGE13, COLLISION_EDGE23
};

/* Scratch-car work used together by the collision controller and geometry
 * helpers. Two car footprints follow the rotation/relative-position work. */
typedef struct {
    s32 reserved00[5];
    s32 angles14[4];
    s32 reserved24[3];
    s32 dx30, dy34;
    s32 reserved38[2];
    CollisionPoint pts40[2][9];
} ScratchpadCollision;

RR_ASSERT_SIZE(CollisionPoint, 4);
RR_ASSERT_OFFSET(ScratchpadCollision, pts40, 0x40);
RR_ASSERT_SIZE(ScratchpadCollision, 0x88);

#endif

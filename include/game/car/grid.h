#ifndef RR_GAME_CAR_GRID_H
#define RR_GAME_CAR_GRID_H

#include "types.h"

/* Start grid positions. */
typedef struct {
    s32 f0;
    s32 pad[3];
    struct {
        s32 x, z;
    } pos[8];
    s16 rot[8];
} Grid53C;

#endif

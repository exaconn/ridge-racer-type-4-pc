#include "rr/overlay662_distance.h"

s32 ovl662_func_8011DBD8(const GaragePoint *a, const GaragePoint *b) {
    u32 dx = (b->x - a->x) * 2U;
    u32 dy = (b->y - a->y);

    return 4096 / (((CordicSqrt(((dx * dx) + (dy * dy))) >> 6) / 64) + 1);
}

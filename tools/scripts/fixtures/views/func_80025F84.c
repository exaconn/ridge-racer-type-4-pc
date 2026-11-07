#include "types.h"

#include "rr/track_sample_types.h"

#include "rr/race_types.h"

int AngleDiffAbs(int a0, int a1) {
    int d;
    a0 &= 0xFFF;
    a1 &= 0xFFF;
    if (a0 < a1) d = a1 - a0; else d = a0 - a1;
    if (d > 0x800) d = 0x1000 - d;
    return d;
}

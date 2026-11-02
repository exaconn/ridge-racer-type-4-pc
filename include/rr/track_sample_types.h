#ifndef RR_TRACK_SAMPLE_TYPES_H
#define RR_TRACK_SAMPLE_TYPES_H
#include "rr/layout.h"
/* Track samples are 0x3C bytes apart. Only these coordinate fields are known. */
typedef struct {
    u8 pad00[8];
    s32 x08, z0C;
    s16 y10,heading12;
    u8 pad14[0x3C - 0x14];
} RrTrackSample3C;

RR_ASSERT_OFFSET(RrTrackSample3C,heading12,0x12);
#endif

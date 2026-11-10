#ifndef RR_TRACK_PROJECTION_H
#define RR_TRACK_PROJECTION_H

#include "rr/race_types.h"
#include "rr/track_sample_types.h"

/* PAL passes each signed offset in a four-byte aggregate aligned to two.
 * The upper halfword is padding; the projection reads only value. */
typedef struct { s16 value; unsigned short :16; } TrackOffsetArgument;

void ProjectToTrackSegment(TrackOffsetArgument x, TrackOffsetArgument z, s32 node,
    s16 *fraction, s16 *lateral, s32 *remaining);
void ComputeCarTrackSurfaceProj(RaceVehicle *vehicle);
void UpdateCarTrackProjection(RaceVehicle *vehicle);

RR_ASSERT_SIZE(TrackOffsetArgument,4);
RR_ASSERT_OFFSET(TrackOffsetArgument,value,0);

#endif

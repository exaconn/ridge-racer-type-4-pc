#ifndef RR_TRACK_INTERPOLATION_H
#define RR_TRACK_INTERPOLATION_H

#include "rr/race.h"

#include "rr/track_sample_types.h"

extern RrTrackSample3C *TrackNodes;
extern s32 TrackNodeCount;
extern s16 ReverseCourse;

/* The original performs a single wrap, including the signed neighbour index.
 * Both selected records must be accessible. Differences, interpolation products,
 * additions and the final x/z scale must fit s32. Output may overlap samples. */
void GetTrackPointAhead(RrRaceVehicle *vehicle, s32 *output, s32 distance);

#endif

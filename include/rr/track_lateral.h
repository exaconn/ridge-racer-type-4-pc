#ifndef RR_TRACK_LATERAL_H
#define RR_TRACK_LATERAL_H

#include "rr/race_types.h"
#include "rr/track_sample_types.h"

/* Lateral and longitudinal fractions use 1024 units. The XZ variant leaves
 * output[1] alone; the XYZ variant also interpolates the centre-line height.
 * Widths must be nonzero and the original signed products must fit s32. */
void InterpolateLanePointXZ(const TrackSample3C *first, const TrackSample3C *second,
    s32 lateral, s32 fraction, s32 *output);
void InterpolateLanePointXYZ(const TrackSample3C *first, const TrackSample3C *second,
    s32 lateral, s32 fraction, s32 *output);
void UpdateComCarTargetPoint(RaceVehicle *vehicle);
/* The PAL caller ABI uses a full word, normalized to signed sixteen bits. */
s32 GetTrackNodeHeading(s32 node, s32 lane);

RR_ASSERT_OFFSET(RaceVehicle, trackFraction170, 0x170);
#if defined(mips)
RR_ASSERT_OFFSET(RaceVehicleControl, pathDeltaX30, 0x30);
RR_ASSERT_OFFSET(RaceVehicleControl, pathDeltaZ34, 0x34);
#endif

#endif

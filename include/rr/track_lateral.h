#ifndef RR_TRACK_LATERAL_H
#define RR_TRACK_LATERAL_H

#include "rr/race_types.h"
#include "rr/track_sample_types.h"

/* Lateral and longitudinal fractions use 1024 units. The XZ variant leaves
 * output[1] alone; the XYZ variant also interpolates the centre-line height.
 * Widths must be nonzero and the original signed products must fit s32. */
void func_80047230(const RrTrackSample3C *first, const RrTrackSample3C *second,
    s32 lateral, s32 fraction, s32 *output);
void func_80047610(const RrTrackSample3C *first, const RrTrackSample3C *second,
    s32 lateral, s32 fraction, s32 *output);
void func_80047A50(RrRaceVehicle *vehicle);
/* The PAL caller ABI uses a full word, normalized to signed sixteen bits. */
s32 func_8004C454(s32 node, s32 lane);
void func_8004D58C(RrTrackLaneOffset8 *offsets);

RR_ASSERT_OFFSET(RrRaceVehicle, trackFraction170, 0x170);
RR_ASSERT_OFFSET(RrRaceVehicle, pathDeltaX224, 0x224);
RR_ASSERT_OFFSET(RrRaceVehicle, pathDeltaZ228, 0x228);
RR_ASSERT_OFFSET(RrRaceVehicleControl, pathDeltaX30, 0x30);
RR_ASSERT_OFFSET(RrRaceVehicleControl, pathDeltaZ34, 0x34);

#endif

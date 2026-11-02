#ifndef RR_TRACK_INTERPOLATION_H
#define RR_TRACK_INTERPOLATION_H

#include "rr/race.h"

#include "rr/track_sample_types.h"

extern RrTrackSample3C *D_800ABEA4;
extern s32 D_800ABECC;
extern s16 D_800AD522;

/* The original performs a single wrap, including the signed neighbour index.
 * Both selected records must be accessible. Differences, interpolation products,
 * additions and the final x/z scale must fit s32. Output may overlap samples. */
void func_80043AB0(RrRaceVehicle *vehicle, s32 *output, s32 distance);

RR_ASSERT_SIZE(RrTrackSample3C, 0x3C);
RR_ASSERT_OFFSET(RrTrackSample3C, x08, 8);
RR_ASSERT_OFFSET(RrTrackSample3C, z0C, 0xC);
RR_ASSERT_OFFSET(RrTrackSample3C, y10, 0x10);

#endif

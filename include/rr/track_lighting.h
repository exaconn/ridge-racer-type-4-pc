#ifndef RR_TRACK_LIGHTING_H
#define RR_TRACK_LIGHTING_H

#include "rr/lighting_types.h"

/* One course-light row: flags, three RGB rows/directions and ambient color. */
typedef struct {
    u16 reserved00, flags02;
    PreviewLighting40 lighting04;
} TrackLighting2C;

extern TrackLighting2C *TrackLightTable;
RR_ASSERT_SIZE(TrackLighting2C, 0x2C);
RR_ASSERT_OFFSET(TrackLighting2C, lighting04, 4);

#endif

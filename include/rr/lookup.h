#ifndef RR_LOOKUP_H
#define RR_LOOKUP_H

#include "rr/layout.h"

#include "rr/scene_camera_state.h"


s32 LookupTrackEventPair(s32 key, s32 selection);
/* A terminal threshold of sampleCount - 1 denotes a range wrapping to zero. */
s32 FindTrackEventIndex(const TrackEventEntry *entries, s32 count, s32 key);
s32 LookupTrackEventTable0(s32 key);
s32 LookupTrackEventTable1(s32 key);
s32 LookupTrackEventTable2(s32 key);
s32 LookupTrackEventTable3(s32 key);
s32 LookupTrackEventPair2(s32 key);
/* Select one of the seven tables (0..6); other selectors return zero. */
s32 LookupTrackEvent(s32 selection, s32 key);

#endif

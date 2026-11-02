#ifndef RR_OVERLAY668_TRACK_PROJECT_H
#define RR_OVERLAY668_TRACK_PROJECT_H
#include "rr/overlay668_camera.h"
/* The PAL by-value argument occupies four bytes with alignment two. Only the
 * signed low halfword is consumed (LH at incoming SP+0 and SP+4); the unnamed
 * halfword is ABI padding, not a second coordinate. */
typedef struct {s16 value;unsigned short :16;} RrTrackOffsetArgument;
s32 func_80026A74(s32 *position,s32 node,s32 count);
void func_800464E8(RrTrackOffsetArgument x,RrTrackOffsetArgument z,s32 node,
                  s16 *fraction,s16 *lateral,s32 *height);
/* Search uses arithmetic-shifted coordinates; interpolation uses division
 * toward zero. Reload positions and the sample root after the search service.
 * The returned node addresses a complete track sample. Coordinate arguments
 * retain their low sixteen bits; all software pointers remain full width. */
RR_ASSERT_SIZE(RrTrackOffsetArgument,4);
RR_ASSERT_OFFSET(RrTrackOffsetArgument,value,0);
#endif

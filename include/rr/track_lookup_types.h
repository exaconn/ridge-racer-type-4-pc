#ifndef RR_TRACK_LOOKUP_TYPES_H
#define RR_TRACK_LOOKUP_TYPES_H
#include "rr/layout.h"

/* Track-event tables associate descending keys with node thresholds. */
typedef struct {
    s16 key, threshold;
    union {s32 word; s16 half;} value;
} TrackEventEntry;
typedef struct {s16 key, value;} TrackEventPair;

/* Signed relative offsets are active before relocation; entries are active
 * afterwards. Native importers construct these descriptors with real owners. */
typedef union {s32 offset; TrackEventPair *entries;} TrackPairReference;
typedef union {s32 offset; TrackEventEntry *entries;} TrackEventReference;
typedef struct {
    s16 pairCount0, pairCount1;
    s16 count0, count1, count2, count3;
    s16 pairCount2, reserved0E;
    TrackPairReference pairs0, pairs1;
    TrackEventReference table0, table1, table2, table3;
    TrackPairReference pairs2;
} TrackEventDirectory;

RR_ASSERT_SIZE(TrackEventEntry,8);
RR_ASSERT_SIZE(TrackEventPair,4);
RR_ASSERT_OFFSET(TrackEventEntry,value,4);
RR_ASSERT_SIZE(TrackPairReference,sizeof(void*));
RR_ASSERT_SIZE(TrackEventReference,sizeof(void*));
RR_ASSERT_SIZE(TrackEventDirectory,0x10+7*sizeof(void*));
RR_ASSERT_OFFSET(TrackEventDirectory,pairCount0,0);
RR_ASSERT_OFFSET(TrackEventDirectory,pairCount1,2);
RR_ASSERT_OFFSET(TrackEventDirectory,count0,4);
RR_ASSERT_OFFSET(TrackEventDirectory,count3,0xA);
RR_ASSERT_OFFSET(TrackEventDirectory,pairCount2,0xC);
RR_ASSERT_OFFSET(TrackEventDirectory,pairs0,0x10);
RR_ASSERT_OFFSET(TrackEventDirectory,table0,0x10+2*sizeof(void*));
RR_ASSERT_OFFSET(TrackEventDirectory,table3,0x10+5*sizeof(void*));
RR_ASSERT_OFFSET(TrackEventDirectory,pairs2,0x10+6*sizeof(void*));
#endif

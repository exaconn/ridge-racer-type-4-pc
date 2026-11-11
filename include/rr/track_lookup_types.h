#ifndef RR_TRACK_LOOKUP_TYPES_H
#define RR_TRACK_LOOKUP_TYPES_H
#include "rr/layout.h"

/* Track-event tables associate descending keys with node thresholds. */
typedef struct {
    s16 key, threshold;
    union {s32 word; s16 half;} value;
} TrackEventEntry;
typedef struct {s16 key, value;} TrackEventPair;

/* Serialized directory in the track resource file: counts, then seven
 * signed offsets relative to the directory itself. */
typedef struct {
    s16 pairCount0, pairCount1;
    s16 count0, count1, count2, count3;
    s16 pairCount2, reserved0E;
    s32 pairs0, pairs1;
    s32 table0, table1, table2, table3;
    s32 pairs2;
} TrackEventDirectoryWire;

/* Host view published by RelocateTrackLookupTables: the same counts and the
 * resolved table addresses (the wire directory is not rewritten). */
typedef struct {TrackEventPair *entries;} TrackPairReference;
typedef struct {TrackEventEntry *entries;} TrackEventReference;
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
RR_ASSERT_SIZE(TrackEventDirectoryWire,0x2C);
RR_ASSERT_OFFSET(TrackEventDirectoryWire,pairCount2,0xC);
RR_ASSERT_OFFSET(TrackEventDirectoryWire,pairs0,0x10);
RR_ASSERT_OFFSET(TrackEventDirectoryWire,pairs2,0x28);
#endif

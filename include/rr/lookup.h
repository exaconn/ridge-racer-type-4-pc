#ifndef RR_LOOKUP_H
#define RR_LOOKUP_H

#include "rr/layout.h"

/* Track-event tables associate ordered keys with values and node thresholds. */
typedef struct {
    s16 key, threshold;
    union {
        s32 word;
        s16 half;
    } value;
} RrTrackEventEntry;

typedef struct {
    s16 key, value;
} RrTrackEventPair;

typedef struct {
    char pad00[4];
    s16 count0, count1, count2, count3;
    s32 reserved0C;
    void *auxiliary10;
    void *auxiliary14;
    RrTrackEventEntry *table0;
    RrTrackEventEntry *table1;
    RrTrackEventEntry *table2;
    RrTrackEventEntry *table3;
    void *auxiliary28;
} RrLookupTables32;

typedef struct {
    s16 count0, count1;
    char pad04[8];
    s16 count2;
    char pad0E[2];
    RrTrackEventPair *table0;
    RrTrackEventPair *table1;
    char pad18[0x10];
    RrTrackEventPair *table2;
} RrLookupTables16;

typedef struct {
    char pad000[0x550];
    void *cameraNodes550;
    u8 *workspace554;
    u8 *workspace558;
    u8 *workspace55C;
    char reserved560[8];
    RrLookupTables32 *lookupTables;
} RrTrackResourceContext;

extern RrTrackResourceContext RaceContext;
extern RrLookupTables16 *TrackEventTables;

s32 LookupTrackEventPair(s32 key, s32 selection);
/* A terminal threshold of sampleCount - 1 denotes a range wrapping to zero. */
s32 FindTrackEventIndex(const RrTrackEventEntry *entries, s32 count, s32 key);
s32 LookupTrackEventTable0(s32 key);
s32 LookupTrackEventTable1(s32 key);
s32 LookupTrackEventTable2(s32 key);
s32 LookupTrackEventTable3(s32 key);
s32 LookupTrackEventPair2(s32 key);
/* selection is one of the seven track-event tables (0..6). */
s32 LookupTrackEvent(s32 selection, s32 key);

RR_ASSERT_SIZE(RrTrackEventEntry, 8);
RR_ASSERT_SIZE(RrTrackEventPair, 4);
RR_ASSERT_OFFSET(RrTrackEventEntry, value, 4);
RR_ASSERT_SIZE(RrLookupTables32, 0x2C);
RR_ASSERT_OFFSET(RrLookupTables32, count0, 4);
RR_ASSERT_OFFSET(RrLookupTables32, count3, 0xA);
RR_ASSERT_OFFSET(RrLookupTables32, table0, 0x18);
RR_ASSERT_OFFSET(RrLookupTables32, table3, 0x24);
RR_ASSERT_SIZE(RrLookupTables16, 0x2C);
RR_ASSERT_OFFSET(RrLookupTables16, count2, 0xC);
RR_ASSERT_OFFSET(RrLookupTables16, table0, 0x10);
RR_ASSERT_OFFSET(RrLookupTables16, table2, 0x28);
RR_ASSERT_OFFSET(RrTrackResourceContext, lookupTables, 0x568);

#endif

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
} TrackEventEntry;

typedef struct {
    s16 key, value;
} TrackEventPair;

typedef struct {
    char pad00[4];
    s16 count0, count1, count2, count3;
    s32 reserved0C;
    void *auxiliary10;
    void *auxiliary14;
    TrackEventEntry *table0;
    TrackEventEntry *table1;
    TrackEventEntry *table2;
    TrackEventEntry *table3;
    void *auxiliary28;
} LookupTables32;

typedef struct {
    s16 count0, count1;
    char pad04[8];
    s16 count2;
    char pad0E[2];
    TrackEventPair *table0;
    TrackEventPair *table1;
    char pad18[0x10];
    TrackEventPair *table2;
} LookupTables16;

typedef struct {
    char pad000[0x550];
    void *cameraNodes550;
    u8 *workspace554;
    u8 *workspace558;
    u8 *workspace55C;
    char reserved560[8];
    LookupTables32 *lookupTables;
} TrackResourceContext;

extern TrackResourceContext RaceContext;
extern LookupTables16 *TrackEventTables;

s32 LookupTrackEventPair(s32 key, s32 selection);
/* A terminal threshold of sampleCount - 1 denotes a range wrapping to zero. */
s32 FindTrackEventIndex(const TrackEventEntry *entries, s32 count, s32 key);
s32 LookupTrackEventTable0(s32 key);
s32 LookupTrackEventTable1(s32 key);
s32 LookupTrackEventTable2(s32 key);
s32 LookupTrackEventTable3(s32 key);
s32 LookupTrackEventPair2(s32 key);
/* selection is one of the seven track-event tables (0..6). */
s32 LookupTrackEvent(s32 selection, s32 key);

RR_ASSERT_SIZE(TrackEventEntry, 8);
RR_ASSERT_SIZE(TrackEventPair, 4);
RR_ASSERT_OFFSET(TrackEventEntry, value, 4);
RR_ASSERT_SIZE(LookupTables32, 0x2C);
RR_ASSERT_OFFSET(LookupTables32, count0, 4);
RR_ASSERT_OFFSET(LookupTables32, count3, 0xA);
RR_ASSERT_OFFSET(LookupTables32, table0, 0x18);
RR_ASSERT_OFFSET(LookupTables32, table3, 0x24);
RR_ASSERT_SIZE(LookupTables16, 0x2C);
RR_ASSERT_OFFSET(LookupTables16, count2, 0xC);
RR_ASSERT_OFFSET(LookupTables16, table0, 0x10);
RR_ASSERT_OFFSET(LookupTables16, table2, 0x28);
RR_ASSERT_OFFSET(TrackResourceContext, lookupTables, 0x568);

#endif

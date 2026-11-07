/* RODATA 0x800111B0 0x1C */
/* Track event tables are sorted in descending key order. */
#include "rr/lookup.h"

#include "rr/track_sample_types.h"

s32 LookupTrackEventPair(s32 key, s32 selection) {
    s32 lower = 0, upper, middle;
    RrTrackEventPair *entries;
    if (selection == 0) {
        upper = D_800ACBB8->count0;
        entries = D_800ACBB8->table0;
    } else {
        upper = D_800ACBB8->count1;
        entries = D_800ACBB8->table1;
    }
    while (lower < upper) {
        middle = (lower + upper) / 2;
        if (entries[middle].key >= key) lower = middle + 1;
        else upper = middle;
    }
    if (lower != 0) lower--;
    if (key == entries[lower].key) return entries[lower].value;
    return -1;
}

s32 FindTrackEventIndex(const RrTrackEventEntry *entries, s32 count, s32 key) {
    s32 index = 0;
    s32 middle;
    s32 threshold;
    if (count > 0) {
        do {
            middle = (index + count) / 2;
            if (entries[middle].key >= key) {
                index = middle + 1;
            } else {
                count = middle;
            }
        } while (index < count);
    }
    if (index == 0) goto notFound;
    index--;
    threshold = entries[index].threshold;
    if (threshold == TrackNodeCount - 1) {
        if (key >= 0) return index;
notFound:
        return -1;
    }
    if (key >= threshold) return index;
    return -1;
}

s32 LookupTrackEventTable0(s32 key) {
    s32 index = FindTrackEventIndex(RaceContext.lookupTables->table0, RaceContext.lookupTables->count0, key);
    if (index >= 0) return RaceContext.lookupTables->table0[index].value.word;
    return -1;
}

s32 LookupTrackEventTable1(s32 key) {
    s32 index = FindTrackEventIndex(RaceContext.lookupTables->table1, RaceContext.lookupTables->count1, key);
    if (index >= 0) return RaceContext.lookupTables->table1[index].value.word;
    return -1;
}

s32 LookupTrackEventTable2(s32 key) {
    s32 index = FindTrackEventIndex(RaceContext.lookupTables->table2, RaceContext.lookupTables->count2, key);
    if (index >= 0) return RaceContext.lookupTables->table2[index].value.word;
    return -1;
}

s32 LookupTrackEventTable3(s32 key) {
    s32 index = FindTrackEventIndex(RaceContext.lookupTables->table3, RaceContext.lookupTables->count3, key);
    if (index >= 0) return RaceContext.lookupTables->table3[index].value.half;
    return -1;
}

s32 LookupTrackEventPair2(s32 key) {
    RrLookupTables16 *tables = D_800ACBB8;
    s32 upper = tables->count2;
    RrTrackEventPair *entries = tables->table2;
    s32 lower = 0, middle;
    while (lower < upper) {
        middle = (lower + upper) / 2;
        if (entries[middle].key >= key) lower = middle + 1;
        else upper = middle;
    }
    if (lower != 0) lower--;
    if (key == entries[lower].key) return entries[lower].value;
    return -1;
}

s32 LookupTrackEvent(s32 selection, s32 key) {
    switch (selection) {
    case 0: return LookupTrackEventPair(key, 0);
    case 1: return LookupTrackEventPair(key, 1);
    case 2: return LookupTrackEventTable0(key);
    case 3: return LookupTrackEventTable1(key);
    case 4: return LookupTrackEventTable2(key);
    case 5: return LookupTrackEventTable3(key);
    case 6: return LookupTrackEventPair2(key);
    }
}

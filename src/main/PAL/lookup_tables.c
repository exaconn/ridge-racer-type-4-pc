/* RODATA 0x800111B0 0x1C */
/* Track event tables are sorted in descending key order. */
#include "rr/lookup.h"

#include "rr/track_sample_types.h"

s32 func_80054EF4(s32 key, s32 selection) {
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

s32 func_80054FA4(const RrTrackEventEntry *entries, s32 count, s32 key) {
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
    if (threshold == D_800ABECC - 1) {
        if (key >= 0) return index;
notFound:
        return -1;
    }
    if (key >= threshold) return index;
    return -1;
}

s32 func_80055040(s32 key) {
    s32 index = func_80054FA4(D_800AC650.lookupTables->table0, D_800AC650.lookupTables->count0, key);
    if (index >= 0) return D_800AC650.lookupTables->table0[index].value.word;
    return -1;
}

s32 func_800550A0(s32 key) {
    s32 index = func_80054FA4(D_800AC650.lookupTables->table1, D_800AC650.lookupTables->count1, key);
    if (index >= 0) return D_800AC650.lookupTables->table1[index].value.word;
    return -1;
}

s32 func_80055100(s32 key) {
    s32 index = func_80054FA4(D_800AC650.lookupTables->table2, D_800AC650.lookupTables->count2, key);
    if (index >= 0) return D_800AC650.lookupTables->table2[index].value.word;
    return -1;
}

s32 func_80055160(s32 key) {
    s32 index = func_80054FA4(D_800AC650.lookupTables->table3, D_800AC650.lookupTables->count3, key);
    if (index >= 0) return D_800AC650.lookupTables->table3[index].value.half;
    return -1;
}

s32 func_800551C0(s32 key) {
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

s32 func_80055250(s32 selection, s32 key) {
    switch (selection) {
    case 0: return func_80054EF4(key, 0);
    case 1: return func_80054EF4(key, 1);
    case 2: return func_80055040(key);
    case 3: return func_800550A0(key);
    case 4: return func_80055100(key);
    case 5: return func_80055160(key);
    case 6: return func_800551C0(key);
    }
}

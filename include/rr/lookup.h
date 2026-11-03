#ifndef RR_LOOKUP_H
#define RR_LOOKUP_H

#include "rr/geometry.h"

/* Track-event tables associate ordered keys with values and node thresholds. */
typedef struct {
    s16 key, threshold;
    s16 value, pad06;
} RrLookupEntry16;

typedef struct {
    s16 key, threshold;
    s32 value;
} RrLookupEntry32;

typedef struct {
    char pad00[4];
    s16 count0, count1, count2, count3;
    s32 reserved0C;
    void *auxiliary10;
    void *auxiliary14;
    RrLookupEntry32 *table0;
    RrLookupEntry32 *table1;
    RrLookupEntry32 *table2;
    RrLookupEntry16 *table3;
    void *auxiliary28;
} RrLookupTables32;

typedef struct {
    s16 count0, count1;
    char pad04[8];
    s16 count2;
    char pad0E[2];
    RrPairS16 *table0;
    RrPairS16 *table1;
    char pad18[0x10];
    RrPairS16 *table2;
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

RR_ASSERT_SIZE(RrLookupEntry16, 8);
RR_ASSERT_SIZE(RrLookupEntry32, 8);
RR_ASSERT_OFFSET(RrLookupEntry16, value, 4);
RR_ASSERT_OFFSET(RrLookupEntry32, value, 4);
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

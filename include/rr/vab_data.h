#ifndef RR_VAB_DATA_H
#define RR_VAB_DATA_H
#include "rr/layout.h"

/* The loader publishes the writable VAB header/program buffer and the
 * read-only sample payload independently. */
typedef struct {
    u8 *header;
    const u16 *samples;
} VabData;
extern VabData VabLoadArgs;

/* Three-word load-table rows: disc file, open mode and destination handle slot.
 * The old 0x800A0234/0x800A0270 views select rows 1 and 6 of this same table. */
typedef struct {
    s32 file_index;
    s32 open_mode;
    s32 bank_slot;
} VabLoadConfig;
extern VabLoadConfig CourseVabLoadConfigs[];
RR_ASSERT_SIZE(VabLoadConfig, 12);
RR_ASSERT_OFFSET(VabLoadConfig, open_mode, 4);
RR_ASSERT_OFFSET(VabLoadConfig, bank_slot, 8);

#if defined(__mips__)
RR_ASSERT_SIZE(VabData, 8);
RR_ASSERT_OFFSET(VabData, samples, 4);
#endif
#endif

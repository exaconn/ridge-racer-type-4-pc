#ifndef RR_CARD_FORMAT_H
#define RR_CARD_FORMAT_H

#include "rr/bios_devices.h"

/* Byte-aligned wire views preserve unaligned card-buffer loads/stores. */
typedef struct {u32 value;} __attribute__((packed)) CardWireWord;
typedef union {u8 bytes[32];CardWireWord words[8];} CardDirectoryWire;
typedef union {u8 bytes[4];CardWireWord word;} CardBrokenWire;
typedef struct {
    u32 state,size;
    u16 next;
    u8 remainder[22];
} CardDirectoryFields;
typedef union {
    CardDirectoryFields fields;
    CardDirectoryWire wire;
} CardDirectoryRecord;
typedef union {s32 value;CardBrokenWire wire;} CardBrokenRecord;
typedef union {
    CardDirectoryWire directory;
    CardBrokenWire broken;
    u8 bytes[128];
} CardFormatSector;

extern CardDirectoryRecord CardFormatDirRecords[15];
extern CardBrokenRecord D_801142D8[20];
extern CardFormatSector D_80114328;
/* Initialize directory sectors1..15, broken-sector records16..35 and MC header0.
 * Return1 only if all36 writes return1; stop immediately on any other status.
 * Clear/copy operations occur before each write, so failure preserves that
 * sector's prepared buffer and all prior in-memory changes. Port remains32bit. */
s32 _card_format(s32 port);

typedef char CardAddressWordMustHoldPointer[
    sizeof(uintptr_t)>=sizeof(void*)?1:-1];
RR_ASSERT_SIZE(CardWireWord,4);
RR_ASSERT_SIZE(CardDirectoryWire,32);
RR_ASSERT_SIZE(CardBrokenWire,4);
RR_ASSERT_SIZE(CardDirectoryRecord,32);
RR_ASSERT_OFFSET(CardDirectoryFields,size,4);
RR_ASSERT_OFFSET(CardDirectoryFields,next,8);
RR_ASSERT_SIZE(CardBrokenRecord,4);
RR_ASSERT_SIZE(CardFormatSector,128);

#endif

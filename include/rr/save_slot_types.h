#ifndef RR_SAVE_SLOT_TYPES_H
#define RR_SAVE_SLOT_TYPES_H
#include "rr/menu_state_types.h"
enum {RR_SAVE_SLOT_COUNT=3, RR_SAVE_SLOT_HALFWORDS=22, RR_SAVE_SLOT_CHECKSUM_HALFWORD=8};
/* One persistent menu record. Runtime fields and the serialized checksum
 * operate on the same 44 bytes, without crossing a member array boundary. */
typedef union {
    MenuSettings2C fields;
    s16 signedHalves[RR_SAVE_SLOT_HALFWORDS];
    u16 halves[RR_SAVE_SLOT_HALFWORDS];
    s8 signedBytes[0x2C];
    u8 bytes[0x2C];
} SavedMenuSettings;
RR_ASSERT_SIZE(SavedMenuSettings,0x2C);
RR_ASSERT_OFFSET(SavedMenuSettings,fields,0);
RR_ASSERT_OFFSET(SavedMenuSettings,halves,0);
#endif

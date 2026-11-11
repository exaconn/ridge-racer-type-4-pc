#ifndef RR_CURRENT_SELECTION_TYPES_H
#define RR_CURRENT_SELECTION_TYPES_H
#include "rr/layout.h"
#include "rr/menu_choice_types.h"
#include "rr/selection_types.h"

/* The selector and the record list share the same resident buffer. */
typedef union {
    MenuChoice4 choice;
    SelectionRows32 selection;
    SelectionRow entry;
    SelectionRow entries[8];
    MenuChoice4 records[8];
    u8 bytes[32];
} CurrentSelection;
RR_ASSERT_SIZE(CurrentSelection, 0x20);
RR_ASSERT_OFFSET(CurrentSelection, entries, 0);
RR_ASSERT_OFFSET(CurrentSelection, selection, 0);
RR_ASSERT_OFFSET(CurrentSelection, choice, 0);
#endif

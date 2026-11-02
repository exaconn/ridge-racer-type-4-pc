#ifndef RR_CURRENT_SELECTION_TYPES_H
#define RR_CURRENT_SELECTION_TYPES_H
#include "rr/menu_state_types.h"
#include "rr/selection_types.h"

/* The selector and the record list share the same resident buffer. */
typedef union {
    RrMenuChoice4 choice;
    RrSelectionRows selection;
    RrSelectionRow entry;
} RrCurrentSelection;
#endif

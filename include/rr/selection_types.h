#ifndef RR_SELECTION_TYPES_H
#define RR_SELECTION_TYPES_H
#include "types.h"
typedef struct {
    u8 selection, variant, parameter, reserved;
} SelectionRow;

typedef struct {
    u8 primary, variant, pad02[2];
    SelectionRow rows[7];
} SelectionRows32;

#endif

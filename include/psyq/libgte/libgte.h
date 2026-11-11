#ifndef RR_PSYQ_LIBGTE_H
#define RR_PSYQ_LIBGTE_H

#include "types.h"

/* libgte VECTOR/SVECTOR; identical to the PsyZ (__psyz) layouts, which use
 * int (not long) components. Checked in src/main/psyq/sdk_abi.c. */
typedef struct {
    s32 vx, vy, vz, pad;
} VECTOR;

typedef struct {
    s16 vx, vy, vz, pad;
} SVECTOR;

#endif

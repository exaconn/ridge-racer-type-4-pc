#ifndef RR_TABLES_H
#define RR_TABLES_H

#include "rr/layout.h"

/* Partial entry layout for MenuStateTable (8-byte stride). */

typedef struct {
    char p[4];
    u8 i;
    char q[3];
} Entry800A4258;

/* Four-byte rows selected by Entry800A4258.i. The two tables use
 * different subsets of the row, so retain neutral field names. */
typedef struct {
    u8 a, b, c, d;
} MenuChoice;

RR_ASSERT_SIZE(Entry800A4258, 0x8);
RR_ASSERT_OFFSET(Entry800A4258, i, 0x4);
RR_ASSERT_SIZE(MenuChoice, 4);

#endif

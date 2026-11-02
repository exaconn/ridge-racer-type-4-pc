#ifndef RR_OVL659_STANDINGS_H
#define RR_OVL659_STANDINGS_H
#include "rr/race.h"
#include "rr/standings.h"

/* Active entries stop at the first negative value; at most six are present.
 * If D_800F4C64 == -1, the signed halfword at vehicle + 0x2AA must be <= 6.
 * Positive values select the corresponding entry; nonpositive values skip it.
 * The halfword at +0x2A8 is then set to -1. */
void ovl659_func_801158B4(void);
#endif

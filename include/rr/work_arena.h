#ifndef RR_WORK_ARENA_H
#define RR_WORK_ARENA_H
#include "types.h"

/* Shared work memory has different lifetimes: loaded assets, menu rows or replay.
 * Each user selects its actual payload layout; the backing is not a row array. */
extern u8 SharedWorkArena[];

#endif

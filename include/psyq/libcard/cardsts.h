#ifndef RR_PSYQ_LIBCARD_CARDSTS_H
#define RR_PSYQ_LIBCARD_CARDSTS_H
#include "types.h"
/* PocketStation (libmcx McxSync-style) completion of the command started by
 * _cardCmd5ABegin. mode 1 polls, mode 0 waits; returns 1 when finished and
 * stores the command and its McxErr* result (0 PocketStation answered,
 * 1 no card, 2 plain memory card, 3 swapped PocketStation). */
int MemCardSync(int mode, int *command, int *result);
#endif /* RR_PSYQ_LIBCARD_CARDSTS_H */

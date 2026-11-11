#ifndef RR_PSYQ_LIBCARD_CARDSTS_H
#define RR_PSYQ_LIBCARD_CARDSTS_H

#include "types.h"

/* mode1 polls; mode0 waits for completion. Both outputs are signed words. */
int MemCardSync(int mode, int *command, int *result);

#endif /* RR_PSYQ_LIBCARD_CARDSTS_H */

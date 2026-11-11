#ifndef RR_PSYQ_LIBSPU_SPUMEM_H
#define RR_PSYQ_LIBSPU_SPUMEM_H

#include "psyq/libspu/libspu.h"

/* PsyZ libsnd. Opening patches the program table in the supplied writable
 * VAB buffer. */
void _spu_gcSPU(void);
short SsVabOpenHeadSticky(u8 *data, short bank, PsyqULong address);
short SsVabFakeHead(u8 *data, short bank, PsyqULong address);

#endif /* RR_PSYQ_LIBSPU_SPUMEM_H */

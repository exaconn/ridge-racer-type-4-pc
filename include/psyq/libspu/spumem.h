#ifndef RR_PSYQ_LIBSPU_SPUMEM_H
#define RR_PSYQ_LIBSPU_SPUMEM_H
#include "types.h"

void _spu_gcSPU(void);
/* Opening patches the program table in the supplied writable VAB buffer.
 * Both wrappers narrow the raw bank and their result to signed low16. */
s32 SsVabOpenHeadSticky(u8 *data, s32 bank, s32 address);
s32 SsVabFakeHead(u8 *data, s32 bank, s32 address);
/* Allocation callback: the sticky wrappers use the requested SPU address. */
s32 VsGetAddr(u32 bytes, s32 address, s16 bank);

#endif

#ifndef RR_PSYQ_LIBSPU_SPUMODE_H
#define RR_PSYQ_LIBSPU_SPUMODE_H
#include "types.h"

void SpuSetTransferMode(s32 mode);
/* The wrapper normalizes both the raw wait word and the returned status
 * through signed low16, matching the original call boundary. */
s32 SsVabTransCompleted(s32 wait);

#endif

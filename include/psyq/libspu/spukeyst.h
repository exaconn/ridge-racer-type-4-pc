#ifndef RR_PSYQ_LIBSPU_SPUKEYST_H
#define RR_PSYQ_LIBSPU_SPUKEYST_H

#include "types.h"

/* Query the lowest selected voice among bits0..23; no selection returns -1.
 * Results: 0=key off/ENVX zero, 1=key on/ENVX nonzero,
 * 2=key off/ENVX nonzero, 3=key on/ENVX zero. */
int SpuGetKeyStatus(s32 voiceMask);

#endif /* RR_PSYQ_LIBSPU_SPUKEYST_H */

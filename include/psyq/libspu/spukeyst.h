#ifndef RR_PSYQ_LIBSPU_SPUKEYST_H
#define RR_PSYQ_LIBSPU_SPUKEYST_H

#include "psyq/libspu/libspu.h"

/* PsyZ: long SpuGetKeyStatus(u_long voice_bit). Results: 0=key off/ENVX
 * zero, 1=key on/ENVX nonzero, 2=key off/ENVX nonzero, 3=key on/ENVX zero. */
long SpuGetKeyStatus(PsyqULong voiceMask);

#endif /* RR_PSYQ_LIBSPU_SPUKEYST_H */

#ifndef RR_PSYQ_LIBSPU_SMALLOC2_H
#define RR_PSYQ_LIBSPU_SMALLOC2_H

#include "types.h"

/* PsyZ: void SsSetMarkCallback(short, short, SsMarkCallbackProc). */
typedef void (*SsMarkCallbackProc)(short access, short sequence, short data);
void SsSetMarkCallback(short access, short sequence, SsMarkCallbackProc proc);

#endif /* RR_PSYQ_LIBSPU_SMALLOC2_H */

#ifndef RR_PSYQ_LIBSND_SSVOLSET_H
#define RR_PSYQ_LIBSND_SSVOLSET_H

#include "types.h"

/* PsyZ libsnd. */
void SsSeqSetVol(short access, short left, short right);
void SsSeqGetVol(short access, short sequence, short *left, short *right);

#endif /* RR_PSYQ_LIBSND_SSVOLSET_H */

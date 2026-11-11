#ifndef RR_PSYQ_LIBSND_SSPAUSE_H
#define RR_PSYQ_LIBSND_SSPAUSE_H

#include "types.h"

/* PsyZ declares SsSeqPause but does not implement it. */
void SsSeqPause(short access);
/* PsyZ: void SsSeqPlay(short seq_access_num, char play_mode, short l_count). */
void SsSeqPlay(short access, char mode, short count);

#endif /* RR_PSYQ_LIBSND_SSPAUSE_H */

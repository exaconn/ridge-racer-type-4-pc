#ifndef RR_PSYQ_LIBSND_SSVOLSET_H
#define RR_PSYQ_LIBSND_SSVOLSET_H

#include "types.h"

void SsSeqSetVol(s16 bank, s32 left, s32 right);
void SsSeqGetVol(s32 bank,s32 row,u16 *left,u16 *right);

#endif /* RR_PSYQ_LIBSND_SSVOLSET_H */

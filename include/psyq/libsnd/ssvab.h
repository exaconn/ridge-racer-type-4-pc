#ifndef RR_PSYQ_LIBSND_SSVAB_H
#define RR_PSYQ_LIBSND_SSVAB_H

#include "types.h"

typedef s32 (*VabTransFunc)(u32, s32, s16);

s32 _SsVabOpenHeadWithMode(void *addr, s16 vabid, VabTransFunc func, s32 mode);

#endif /* RR_PSYQ_LIBSND_SSVAB_H */

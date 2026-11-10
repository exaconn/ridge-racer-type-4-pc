#ifndef RR_PSYQ_LIBSND_CC_6_H
#define RR_PSYQ_LIBSND_CC_6_H

#include "types.h"

/* Controller words normalize to signed bank/row halfwords and a data byte. */
void _SsContDataEntry(s32 bank, s32 row, s32 data);

#endif /* RR_PSYQ_LIBSND_CC_6_H */

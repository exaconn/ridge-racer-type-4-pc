#ifndef RR_PSYQ_LIBCARD_CARDC162_H
#define RR_PSYQ_LIBCARD_CARDC162_H

#include "types.h"

s32 _cardCmd58Rx(s32 value);
s32 _cardCmd5ABegin(s32 request, u8 *buffer);

#endif /* RR_PSYQ_LIBCARD_CARDC162_H */

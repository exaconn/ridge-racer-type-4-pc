#ifndef RR_PSYQ_LIBCARD_CARDC04_H
#define RR_PSYQ_LIBCARD_CARDC04_H

#include "types.h"

extern unsigned char D_80114726;
extern unsigned char D_80114727;
int _cardCmd5BRangeTx();
s32 _cardCmd5BRangeRx(s32 value);

#endif /* RR_PSYQ_LIBCARD_CARDC04_H */

#ifndef RR_PSYQ_LIBCARD_INIT_H
#define RR_PSYQ_LIBCARD_INIT_H

#include "types.h"

/* PsyZ libcard. */
void InitCARD(long pad_enable);
long StartCARD(void);
long StopCARD(void);

#endif /* RR_PSYQ_LIBCARD_INIT_H */

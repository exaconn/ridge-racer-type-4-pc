#ifndef RR_PSYQ_LIBCD_DSSTRM32_H
#define RR_PSYQ_LIBCD_DSSTRM32_H

#include "types.h"

extern volatile u8 *D_800A6870;
extern int D_801102EC;
extern int D_801102F4;
extern int D_801102F8;
extern int D_80110300;
extern int D_80110308;
void StCdInterrupt(void);

#endif /* RR_PSYQ_LIBCD_DSSTRM32_H */

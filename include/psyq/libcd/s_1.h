#ifndef RR_PSYQ_LIBCD_S_1_H
#define RR_PSYQ_LIBCD_S_1_H

#include "types.h"

u8 CdStatus(void);
s32 CdMode(void);
u8 *CdLastPos(void);
int CdReset();
s32 CdSetDebug(s32 value);
int CdSyncCallback();
s32 CdReadyCallback(s32 value);

#endif /* RR_PSYQ_LIBCD_S_1_H */

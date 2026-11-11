#ifndef RR_PSYQ_LIBCD_S_1_H
#define RR_PSYQ_LIBCD_S_1_H

#include "types.h"

/* PsyZ libcd (__psyz) status/reset/callback entry points. */
typedef void (*CdlCB)(u8 status, u8 *result);

int CdStatus(void);
int CdMode(void);
u8 *CdLastPos(void);
int CdReset(int mode);
int CdSetDebug(int level);
CdlCB CdSyncCallback(CdlCB callback);
CdlCB CdReadyCallback(CdlCB callback);

#endif /* RR_PSYQ_LIBCD_S_1_H */

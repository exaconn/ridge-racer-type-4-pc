#ifndef RR_PSYQ_LIBCD_DSMEDIA2_H
#define RR_PSYQ_LIBCD_DSMEDIA2_H

#include "types.h"

extern int StCurFrameNo;
extern short StCurSectorOffset;
extern int CdReadModeFlag;
void StClearRing(void);
void data_ready_callback(void);

#endif /* RR_PSYQ_LIBCD_DSMEDIA2_H */

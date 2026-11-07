#ifndef RR_CD_READ_STOP_H
#define RR_CD_READ_STOP_H
#include "rr/cd_read_request.h"

/* Clears the remaining count. A naturally completed initial wait returns;
 * otherwise restores callbacks, issues stop command9 and waits for completion.
 * Both waits restore the current saved callback on unsigned elapsed>=121. */
void CdReadBreak(void);
#endif
